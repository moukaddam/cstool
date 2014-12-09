#include "kine_tool.h"

kine_tool::~kine_tool(void)
{
cout << "Inside kine_tool::Destructor()" << endl;

}

kine_tool::kine_tool(string path)
{
cout << "Inside kine_tool::kine_tool()" << endl;

fpath=path;
GetInput();
SetKinematics();

fOutputFile= new TFile("./output/kine_tool_output.root","RECREATE");
fOutputFile->cd();

fDrawer= new draw_tool("./output/theo_tool_drawer.root") ;

WriteDown();

}

void kine_tool::GetInput(void)
{
cout << "Inside kine_tool::GetInput()" << endl;

//================  Get Data ====================
	ifstream inf;
	string mot;
	int input = 0;
  inf.open(fpath.c_str(), ifstream::in);
  if(!inf)
    {
      cout << "ERROR  : Could not open the calibration file :"<<fpath<< endl;
      exit(-1);
    }
  else
    {
      while(inf>>mot)
		{			
//cout<<mot<<endl;
		 if(mot == "REACTION")
		    {
		input=input+1;
		inf>>fReaction;
		cout<<"LOADING REACTION : "<<fReaction<<endl;

		inf>>fAtomMass[0]>>fAtomMass[1]>>fAtomMass[2]>>fAtomMass[3];
		inf>>fAtomCharge[0]>>fAtomCharge[1]>>fAtomCharge[2]>>fAtomCharge[3];
		inf>>fMassExcess[0]>>fMassExcess[1]>>fMassExcess[2]>>fMassExcess[3];
  		
		cout<<"A1: "<<fAtomMass[0]<<"   A2: "<<fAtomMass[1]<<"   A3: "<<fAtomMass[2]<<"   A4: "<<fAtomMass[3]<<endl;
		cout<<"Z1: "<<fAtomCharge[0]<<"   Z2: "<<fAtomCharge[1]<<"   Z3: "<<fAtomCharge[2]<<"   Z4: "<<fAtomCharge[3]<<endl;
  	
		inf>>fEx4;
		inf>>fBeamEnergy>>fElectronMass>>fAmu;
		cout<<"Excitation (MeV) "<<fEx4<<endl;
		cout<<"Beam Total Kinetic Energy (MeV) "<<fBeamEnergy<<endl;
		cout<<"Electron Mass (MeV)  "<<fElectronMass<<endl;
		cout<<"Unified Atomic Mass Unit (MeV) "<<fAmu<<endl;
		     } 
		     
   		 }// while
   		 
   	 }//else 
inf.close();
	 
if(input!=1)
{
cout<<"ERROR: SOMETHING WRONG IN INPUTS NUMBER IN FILE "<<fpath<<endl;
cout<<"NUMBER OF INPUTS FOUND IS "<<input<<endl;
exit(-1);
}


if((fAtomMass[3] + fAtomMass[2] != fAtomMass[0] + fAtomMass[1]  ) ||
   (fAtomCharge[3] + fAtomCharge[2]) != fAtomCharge[0] + fAtomCharge[1]  )
		{
		cout <<"ERROR : Conservation law violation!"<<endl;
		exit(-1);
		}

}

void  kine_tool::SetKinematics(void )
{
cout << "Inside kine_tool::SetKinematics()" << endl;
// we calculate correlated ftheta_cm->ftheta_lab->k_lab->T3->Jacobien_lab to cm

// Calculate masses
  for(int i=0;i<4;i++)
    {
// if (fAtomMass[i]==1  && fAtomCharge[i]==1)
//  fMass[i] = (fAmu*1.00727646676);
// else
      fMass[i] = (fAmu*fAtomMass[i]) + fMassExcess[i] - (fElectronMass*fAtomCharge[i]);
cout<<"fMass : "<<i<<" "<<fMass[i]<<endl;
    }     
    fQ=fMass[0]+fMass[1]-fMass[2]-fMass[3];
    
// Calculate the collision consant parameter
//LAB
  double Ex=fEx4*MeV;
  
  double M1 = fMass[0];
  double M2 = fMass[1];
  double M3 = fMass[2];
  double M4 = fMass[3] + Ex;

//LAB  
  double T1 = fBeamEnergy;
  double W1 =  T1 + M1 ;
  double Wt =  W1 + M2 ;
  double P1 = sqrt(T1*(T1 + 2*M1));
  //cout<<"  P1:  "<<P1<<"  T1:  "<<T1<<"  M1:  "<<M1<<endl;
  
  double BETA=P1/(W1+M2);
  double GAMMA=1/sqrt(1-BETA*BETA);
  //double T_back_threshold=-fQ*( 1 + M1/(M2-M3) - fQ / ( 2* (M2-M3) ) );
cout<<"BETA : "<<BETA<<" --> ";
cout<<"GAMMA : "<<GAMMA<<" --> ";

 double E_star = GAMMA*(T1+M1+M2-BETA*P1);
 double P_f = sqrt(  pow(/**/E_star*E_star-(M3*M3+M4*M4)/**/,2) - pow(/**/(2*M3*M4)/**/,2) ) /(2*E_star);
 double To = BETA*sqrt(P_f*P_f+M3*M3) / P_f;

cout<<endl;
//CM 
  //double W_t =  Wt/GAMMA  ;
  double W_3 =  ( Wt*Wt + GAMMA*GAMMA*(M3*M3-M4*M4) ) / ( 2*GAMMA*Wt) ;
   cout<<"W_3 --> "<<W_3<<" / ";
  double BETA_3=sqrt( 1 - (M3*M3)/(W_3*W_3) ) ;
  double k_cm =BETA/BETA_3;
cout<<"To --> "<<To<<endl;
   cout<<"k_cm --> "<<k_cm<<" / ";
cout<<endl;
 
  double  ANGLE=0;
  double  ANGLE_LAB=0;
  double  k_lab;
  double  JACOB;
  double  T3;
  double  W3;
  double  BETA3;


  double STEP=0.1;
  int STEP_NB= (int)180.0/STEP;
//  int i =90;  
 for (int i=0; i<=STEP_NB;i++)
	  {
	  //Angle CM
	ANGLE = (i*STEP);
	ftheta_cm.push_back(ANGLE);
	ftheta_cm_inv.push_back(180-ANGLE);
// cout<<"ANGLE : "<<ANGLE<<" --> ";
	  //Angle Lab correspondant
	ANGLE_LAB=(1/GAMMA)*sin(ANGLE*degree)/(cos(ANGLE*degree)+k_cm);
	ANGLE_LAB=atan(ANGLE_LAB)/degree;
	if(ANGLE_LAB<0) ANGLE_LAB=ANGLE_LAB+180;
	ftheta_lab.push_back(ANGLE_LAB);
// cout<<"ANGLE_LAB : "<<ANGLE_LAB<<" --> ";
	W3=W_3*GAMMA*(1+BETA*BETA_3*cos(ANGLE*degree));
	T3=W3 - M3;  
	BETA3=sqrt( 1 - (M3*M3)/(W3*W3) ) ;
	k_lab = BETA/BETA3 ;

	fk_ratio_lab.push_back(k_lab);  
	fT3_lab.push_back(T3);
// cout<<"T3 : "<<T3<<" --> ";

double JACOB_N, JACOB_D;

JACOB_N= GAMMA*(1+k_cm*cos(ANGLE*degree));
JACOB_D= pow(GAMMA*GAMMA*pow((cos(ANGLE*degree)+k_cm),2)+pow(sin(ANGLE*degree),2),1.5);
JACOB=JACOB_N/JACOB_D;

//cout<<"JACOB LAB->CM: "<<JACOB<<endl;
	fjacob_lab_cm.push_back(JACOB);
// cout<<"JACOB CM->LAB : "<<1/JACOB<<endl;
	fjacob_cm_lab.push_back(1/JACOB);

	  }

}


void kine_tool::Show(void)
{
cout << "====================================================================== " << endl;

cout<<"A1: "<<fAtomMass[0]<<"   A2: "<<fAtomMass[1]<<"   A3: "<<fAtomMass[2]<<"   A4: "<<fAtomMass[3]<<endl;
cout<<"Z1: "<<fAtomCharge[0]<<"   Z2: "<<fAtomCharge[1]<<"   Z3: "<<fAtomCharge[2]<<"   Z4: "<<fAtomCharge[3]<<endl;

cout << "---------------------------------------------------------------------- " << endl;

cout<<"Beam Total Kinetic Energy (MeV) "<<fBeamEnergy<<endl;
cout<<"Electron Mass (MeV)  "<<fElectronMass<<endl;
cout<<"Unified Atomic Mass Unit (MeV) "<<fAmu<<endl;

cout << "---------------------------------------------------------------------- " << endl;

  for(int i=0;i<4;i++)
    {
      cout<<"mMass : "<<i<<"   "<<fMass[i]<<endl;
    }     
    cout<<" T1 = "<<fBeamEnergy<<endl;
    cout<<" Q = "<<fQ<<endl;

cout << "====================================================================== " << endl;

}


vector<double> kine_tool::ConvertAngleVector(string system, vector<double> AngVec)
{
cout << "Inside kine_tool::ConvertAngleVector()" << endl;

vector<double> AngVecPrime;

Interpolator*  Interpol_theta;
if(system=="LAB")
 Interpol_theta = new Interpolator( ftheta_lab, ftheta_cm );
else
if(system=="CM")
 Interpol_theta = new Interpolator( ftheta_cm ,ftheta_lab );

	for (unsigned i=0 ; i<AngVec.size();i++)
	{
	double theta = AngVec.at(i);
	if (AngVec.at(i)>0)
		{
		double theta_prime= Interpol_theta->Eval(AngVec.at(i));
		//cout <<AngVec.at(i)<< " ---> "<< theta_prime << endl;
		AngVecPrime.push_back( theta_prime ); // since the already calculated jacobien goes from lab->cm
		}
	else
		{
		//cout <<theta<<" < 0 "<< endl;
		AngVecPrime.push_back( theta );
		}
	}

// 	for (unsigned i=0 ; i<AngVec.size();i++)
// 	{
// 	 cout <<AngVec.at(i)<< " ---> "<< AngVecPrime.at(i) << endl;
// 	}

return AngVecPrime;

}

double kine_tool::ConvertAngle(string system, double Ang)
{
cout << "Inside kine_tool::ConvertAngle()" << endl;

double AngPrime;

Interpolator*  Interpol_theta;
if(system=="LAB")
 Interpol_theta = new Interpolator( ftheta_lab, ftheta_cm );
else
if(system=="CM")
 Interpol_theta = new Interpolator( ftheta_cm ,ftheta_lab );


	if (Ang>0)
		{
		AngPrime= Interpol_theta->Eval(Ang);
		}
	else
		{
		AngPrime=Ang;
		}

return AngPrime;

}

void kine_tool::WriteDown(void)
{
cout << "Inside kine_tool::ConvertAngle()" << endl;


int tab1[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("angle lab inv","angle lab inv","angle cm [deg]","angle lab [deg]", tab1 );
TGraph* gr_theta_lab_theta_cm_inv = fDrawer->ConvertToGraph(ftheta_cm_inv,ftheta_lab);
fOutputFile->cd();
gr_theta_lab_theta_cm_inv->Write();

int tab2[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("angle lab","angle lab","angle cm [deg]","angle lab [deg]", tab2 );
TGraph* gr_theta_lab_theta_cm = fDrawer->ConvertToGraph(ftheta_cm,ftheta_lab);
fOutputFile->cd();
gr_theta_lab_theta_cm->Write();

int tab3[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("Jacob lab->cm theta cm","Jacob lab->cm","angle cm [deg]"," Jacob lab->cm", tab3 );
TGraph* gr_jacob_lab_cm_theta_cm = fDrawer->ConvertToGraph(ftheta_cm,fjacob_lab_cm);
fOutputFile->cd();
gr_jacob_lab_cm_theta_cm->Write();

int tab4[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("Jacob cm->lab angle cm","Jacob cm->lab","angle cm [deg]","Jacob cm->lab", tab4 );
TGraph* gr_jacob_cm_lab_theta_cm = fDrawer->ConvertToGraph(ftheta_cm,fjacob_cm_lab);
fOutputFile->cd();
gr_jacob_cm_lab_theta_cm->Write();

int tab5[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("Jacob lab->cm theta lab","Jacob lab->cm theta lab","angle lab [deg]","Jacob lab->cm", tab5 );
TGraph* gr_jacob_cm_lab_theta_lab = fDrawer->ConvertToGraph(ftheta_lab,fjacob_lab_cm);
fOutputFile->cd();
gr_jacob_cm_lab_theta_lab->Write();

int tab6[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("Energy","Energy","angle lab [deg]","Energy [MeV]", tab6 );
TGraph* gr_theta_lab_T3_lab = fDrawer->ConvertToGraph(ftheta_lab,fT3_lab);
fOutputFile->cd();
gr_theta_lab_T3_lab->Write();

int tab7[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("Jacob cm->lab (inv)","Jacob cm->lab (inv)","angle cm (inv) [deg]","Jacob cm->lab", tab4 );
TGraph* gr_jacob_lab_cm_theta_cm_inv = fDrawer->ConvertToGraph(ftheta_cm_inv,fjacob_lab_cm);
fOutputFile->cd();
gr_jacob_lab_cm_theta_cm_inv->Write();
}

