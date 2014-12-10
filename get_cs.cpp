#include "get_cs.h"

get_cs::~get_cs(void)
{
cout << "Inside get_cs::Destructor()" << endl;
}

get_cs::get_cs(string path)
{

cout << "Inside get_cs::get_cs()" << endl;

fExpHist = new TH1F();
fpath=path;
GetInput();

fKinematic= new kine_tool(path) ;
fHistManager = new hist_tool(path) ;
fError = new error_tool("./input/BeamTarget.dat") ;
fDrawer= new draw_tool("./output/get_cs_drawer.root") ;

for (unsigned i=0;i<fpath_theo.size();i++) {

fTheoCs.push_back(new theo_tool(fcode_theo.at(i),fsystem_theo.at(i),fpath_theo.at(i), false) ) ;

fSpectroFactorEstimate.push_back(0);
fSpectroFactorUpper.push_back(0);
fSpectroFactorLower.push_back(0);

fChi2Estimate.push_back(0);
fChi2EstimateUpper.push_back(0);
fChi2EstimateLower.push_back(0);

fMixFactorEstimate.push_back(0); // pure 
fMixFactorEstimateUpper.push_back(0); 
fMixFactorEstimateLower.push_back(0);
}

fOutputFile= new TFile("./output/get_cs_output.root","RECREATE");
fOutputFile->cd();

}

void get_cs::GetInput(void)
{
cout << "Inside get_cs::GetInput()" << endl;

//================  Get Data ====================
	ifstream inf;
	string bufstring,mot;
  	inf.open(fpath.c_str(), ios::in);
	int input_count=0;
  if(!inf)
    {
      cout << "ERROR : Could not open the file :"<<fpath<< endl;
      exit(-1);
    }
  else
    {
      while(inf>>mot)
	{
//cout<<mot<<" ";
		if(mot == "HIST_NORM")
		{
		input_count++;
		inf>>fpath_hist_normal;
		}
//--------------------------------------
		if(mot == "HIST")
		{
		input_count++;
		inf>>fpath_hist;
		}
//--------------------------------------
		if(mot == "THEO_CROSS_SECTION")
		{
		input_count++;
		string buffer;
		int buffer_int;
		inf>>fFit;
		inf>>buffer;

		while ( buffer!="END")
			{
			fcode_theo.push_back(buffer);
			inf>>buffer;
			fsystem_theo.push_back(buffer);

			inf>>buffer_int;
			fcolor_theo.push_back(buffer_int);

			inf>>buffer;
			fpath_theo.push_back(buffer);

			inf>>buffer;
			}
for (unsigned i=0;i<fpath_theo.size();i++)
{
cout<<fsystem_theo.at(i)<<"   "<<fpath_theo.at(i)<<endl;
}
		}
//--------------------------------------
		if(mot == "ANGLE_NODES")
		{
		input_count++;
		double buffer;
		inf>>fsystem;
		do
		{
			inf>>buffer;
			if (fsystem=="LAB")
			ftheta_lab_node.push_back(buffer);
			else
			ftheta_cm_node.push_back(buffer);
		}
		while ( buffer!=0)	;
  	
		if (fsystem=="LAB")
		ftheta_lab_node.pop_back();
		else
		ftheta_cm_node.pop_back();
		}	      
//--------------------------------------
         }// while
    }//else
	if(input_count!=4)
	{
	cout<<"ERROR: SOMETHING WRONG IN INPUTS NUMBER IN FILE "<<fpath<<endl;
	cout<<"NUMBER OF INPUTS FOUND IS "<<input_count<<endl;
	exit(-1);
	}

  inf.close();

}

void get_cs::Show(void)
{
cout << " PATH TO LIST OF DATA : " <<fpath<< endl;

cout << "====================================================================== " << endl;
cout << "fpath 			"<<fpath << endl;
cout << "fpath_hist 		"<<fpath_hist << endl;
cout << "fpath_hist_normal 	"<<fpath_hist_normal << endl;

cout << "---------------------------------------------------------------------- " << endl;
	for (unsigned i=0;i<fpath_theo.size();i++)
	{
	cout << "fpath_theo		"<<fpath_theo.at(i)<<"("<<fsystem_theo.at(i)<<")"<<endl;
	}

cout << "---------------------------------------------------------------------- " << endl;

cout << "fsystem 		"<<fsystem << endl;

cout << "---------------------------------------------------------------------- " << endl;

cout<<" LAB ANGLE NODES "<<endl;
for (unsigned i=0; i<ftheta_lab_node.size();i++)
	cout<<"  "<<ftheta_lab_node.at(i);
	cout<<endl;

cout<<" CM ANGLE NODES "<<endl;
for (unsigned i=0; i< ftheta_cm_node.size();i++)
	cout<<"  "<<ftheta_cm_node.at(i);
	cout<<endl;

cout << "---------------------------------------------------------------------- " << endl;

cout<<" LAB ANGLE :  INF, SUP, DIFF"<<endl;
for (unsigned i=0; i<ftheta_lab_inf.size();i++)
	cout<<"  "<<ftheta_lab_inf.at(i)<<"  "<<ftheta_lab_sup.at(i)
            <<"  "<<ftheta_lab_sup.at(i)-ftheta_lab_inf.at(i)<<endl;

cout<<" CM ANGLE :  INF, SUP, DIFF"<<endl;
for (unsigned i=0; i< ftheta_cm_inf.size();i++)
	cout<<"  "<<ftheta_cm_inf.at(i)<<"  "<<ftheta_cm_sup.at(i)
            <<"  "<<ftheta_cm_sup.at(i)-ftheta_cm_inf.at(i)<<endl;

cout << "---------------------------------------------------------------------- " << endl;

double sum=0;
cout<<" EXPERIMENTAL VECTOR LAB : "<<endl;
cout<<" theta "<<endl;
	for (unsigned i =0;i<ftheta_lab_exp.size();i++)
	{
	cout<<ftheta_lab_exp.at(i)<<"  ";
	sum+=fsigma_lab_exp.at(i);
	}
cout<<endl;
cout<<"Ndet "<<endl;
	for (unsigned i =0;i<fcounts_lab_exp.size();i++)
	{
	cout<<fcounts_lab_exp.at(i)<<"  ";
	}
cout<<endl;
cout<<"Norm_factor "<<endl;
	for (unsigned i =0;i<fnorm_lab_exp.size();i++)
	{
	cout<<fnorm_lab_exp.at(i)<<"  ";
	}
cout<<endl;
cout<<"Norm_factor_err"<<endl;
	for (unsigned i =0;i<fnorm_lab_exp_err.size();i++)
	{
	cout<<fnorm_lab_exp_err.at(i)<<"  ";
	}
cout<<endl;
cout<<" sigma "<<endl;
	for (unsigned i =0;i<fsigma_lab_exp.size();i++)
	{
	cout<<fsigma_lab_exp.at(i)<<"  ";
	}
cout<<endl;
cout<<" error sigma "<<endl;
	for (unsigned i =0;i<fsigma_lab_exp_err.size();i++)
	{
	cout<<"("<<fsigma_lab_exp_err.at(i)<<")  ";
	}
cout<<endl;
cout<<" jacob Lab->CM "<<endl;
	for (unsigned i =0;i<fjacob_LabtoCM.size();i++)
	{
	cout<<fjacob_LabtoCM.at(i)<<"  ";
	}
cout<<endl;



cout<<" Sum of all sigma (lab) : "<<sum<<endl;
cout<<" Histogram number of entries : "<<fExpHist->GetEntries()<<endl;

cout << "---------------------------------------------------------------------- " << endl;

double sum2=0;
cout<<" EXPERIMENTAL VECTOR CM : "<<endl;
cout<<" theta "<<endl;
	for (unsigned i =0;i<ftheta_cm_exp.size();i++)
	{
	cout<<ftheta_cm_exp.at(i)<<"  ";
	sum2+=fsigma_cm_exp.at(i);
	}
cout<<endl;
cout<<" sigma "<<endl;
	for (unsigned i =0;i<fsigma_cm_exp.size();i++)
	{
	cout<<fsigma_cm_exp.at(i)<<"  ";
	}
cout<<endl;
cout<<" error sigma "<<endl;
	for (unsigned i =0;i<fsigma_cm_exp_err.size();i++)
	{
	cout<<"("<<fsigma_cm_exp_err.at(i)<<")  ";
	}
cout<<endl;

cout<<" Sum of all sigma (cm) : "<<sum2<<endl;


cout << "============================== R E S U L T =================================== " << endl;
for (unsigned curve_num = 0; curve_num < fSpectroFactorEstimate.size();curve_num++)
	{
cout << "---------------------------------------------------------------------- " << endl;
	cout<<" Upper chi2 \t\t"<<fChi2EstimateUpper.at(curve_num) <<endl ;
	cout<<" Lower chi2 \t\t"<<fChi2EstimateLower.at(curve_num) <<endl ;
	cout<<" SFUpper \t\t"<<fSpectroFactorUpper.at(curve_num) <<endl ;
	cout<<" SFLower \t\t"<<fSpectroFactorLower.at(curve_num) <<endl ;
	cout<<" Upper error \t\t"<< fSpectroFactorUpper.at(curve_num)-fSpectroFactorEstimate.at(curve_num) <<endl ;
	cout<<" Lower error \t\t"<< fSpectroFactorEstimate.at(curve_num)-fSpectroFactorLower.at(curve_num) <<endl ;
	double maxerror = max(fSpectroFactorUpper.at(curve_num)-fSpectroFactorEstimate.at(curve_num),fSpectroFactorEstimate.at(curve_num)-fSpectroFactorLower.at(curve_num));
	cout << endl;
	printf(" SF \t\t\t%1.2f +/- %1.2f \t",fSpectroFactorEstimate.at(curve_num),maxerror);
	printf(" \t\tChi2 \t%1.2f ",fChi2Estimate.at(curve_num));
	printf("\n \tMixing factor  \t%1.2f", fMixFactorEstimate.at(curve_num));
	if (fSpectroFactorEstimate.at(curve_num)>1) printf("\n \t\t\tViolation of Sum Rule ");
	cout << endl;
	}
cout << "=============================================================================== " << endl;
}

void get_cs::Treat1(void)
{
cout << "Inside get_cs::Treat()" << endl;

TreatNodes();
//------------------------------------

fHistManager->GetInput(fpath_hist,fpath_hist_normal);
// return efficiency histogram normalized to the theta histogram
fEffHist = fHistManager->Synchronize(); 
//Normalize and get the Experimental hisotgram
fExpHist= (TH1F*) fHistManager->fHist->Clone("fExpHist");
fExpHist->Divide(fEffHist);
//-----------------------------------------
GetNormalVectorLab(); //irrelevant
//-----------------------------------------
fOutputFile->cd();
fEffHist->Write();
fExpHist->Write();
//-----------------------------------------
GetSumExpVectorLab_ROOT();
//GetSumExpVectorLab();
GetSigmaExpVectorLab1();
//-----------------------------------------
GetSigmaExpVectorCM();
//-----------------------------------------

for (unsigned i=0;i<fTheoCs.size();i++)
{
TransformTheoryCrossSection(i);
fTheoCs.at(i)->Show();
fTheoCs.at(i)->ShowIntegral();
fTheoCs.at(i)->WriteDown();
//----------------------------------------
GetSpectroscopicFactor(i);
}


if (fFit!=0) ApplySpectroscopicFactor(); //on all vector

//----------------------------------------
DrawNormSumVector();
AddGraphs();
Show();

//fOutputFile->Write();

}


void get_cs::Treat2(void)
{
cout << "---------------------------------------------------------" << endl;
cout << "Inside get_cs::Treat2()" << endl;
cout << "---------------------------------------------------------" << endl;

TreatNodes();
//------------------------------------
// get the input for histograms from files..
fHistManager->GetInput(fpath_hist,fpath_hist_normal);
// get the Experimental hisotgram with no division by efficiency histogram
fExpHist= (TH1F*) fHistManager->fHist->Clone("fExpHist");
//-----------------------------------------
// get the vector used for normalisation
GetNormalVectorLab();
//-----------------------------------------
GetSumExpVectorLab_file();  // get number of counts from a file
GetSigmaExpVectorLab2(); // get sigma in lab using fsum and fnorm
GetSigmaExpVectorCM(); // get sigma in cm
//-----------------------------------------

// pure curves 
for (unsigned i=0;i<fTheoCs.size();i++)
GetSpectroscopicFactor(i);

// mixed curves 
GetMixedSpectroscopicFactor(0,1); 
GetMixedSpectroscopicFactor(2,3); 

for (unsigned i=0;i<fTheoCs.size();i++) {
TransformTheoryCrossSection(i);
fTheoCs.at(i)->Show();
fTheoCs.at(i)->ShowIntegral();
fTheoCs.at(i)->WriteDown();
}


//----------------------------------------

if (fFit!=0) ApplySpectroscopicFactor(); //on all vector

//----------------------------------------
DrawChi2();
DrawNormSumVector();
DrawDeltaCosVector();
DrawJacobVector();
//----------------------------------------

//------------------
// 106° - 112° lab (Ex 10MeV) :
//  CM angles 26.0748° - 22.3292°
//  effeciency 0.209484
// Counts 920
//------------------
// 105° - 113° lab (Ex 10MeV) :
//  CM angles 26.755  21.7571
//  effeciency 0.189697
// Counts 980 , 940

CalculateDbkg(/*Deutron Bkgr Counts*/ 980 , /*efficiency*/ 0.189697,  /*CM angle min*/ 21.7571 , /*CM angle max*/ 26.755);
//----------------------------------------

AddGraphs();
Show();

fOutputFile->cd();
fHistManager->fHistEff->Write();
fExpHist->Write();

}


void get_cs::CalculateDbkg(/*Deutron Bkgr Counts*/ int C,  /*efficiency*/double E,
                    /*CM angle min*/ double theta_min, /*CM angle max*/ double theta_max)
{
cout << " D E U T E R O N      B R E A K - U P " << endl;
cout << " ---------------------------------------" << endl;
double C_bar=0;
double A_bar=0;
double E_bar=0;
double A = fabs(cos(theta_min*degree) - cos(theta_max*degree)) ;

cout<<"  "<<"theta min:theta max"<<" -> "<<"angular range cm"<<" --> "<<"counts"<<endl;
for (unsigned i=0; i< ftheta_cm_inf.size();i++)
	{
	A_bar = fabs(cos(ftheta_cm_inf.at(i)*degree) - cos(ftheta_cm_sup.at(i)*degree)) ;
	E_bar = fnorm_lab_exp.at(i);
	C_bar = fabs(  (A_bar*E_bar*C)/(A*E) ) ;
	//cout<<"  A_bar:"<<A_bar<<"  E_bar:"<<E_bar<<" C:"<<C<<" A:"<<A<<" E:"<<E<<endl;
	cout<<"lab : "<<ftheta_lab_inf.at(i)<<" - "<<ftheta_lab_sup.at(i)
            <<" / CM : "<<ftheta_cm_inf.at(i)<<" - "<<ftheta_cm_sup.at(i)
            <<" = "<<A_bar<<" --> "<<C_bar<<endl;
	}
cout << " ---------------------------------------" << endl;
}

void get_cs::ApplySpectroscopicFactor(void)
{

int ans=0;
cout << "DO YOU WANT TO SUPERPOSE ALL THE CURVES ? (No->0 / Yes->1)" << endl;
cin>>ans;

for (unsigned i=0;i<fTheoCs.size();i++)
{
if (ans==1) fTheoCs.at(i)->ApplyFactor(fSpectroFactorEstimate.at(i));
else fTheoCs.at(i)->ApplyFactor(fSpectroFactorEstimate.at(0));
}

}

void get_cs::GetNormalVectorLab(void) // checked  14 octobre
{

	for ( unsigned i =0 ; i < ftheta_lab_inf.size() ; i++)
	{
		double theta_inf = ftheta_lab_inf.at(i);
		double theta_sup = ftheta_lab_sup.at(i);
		pair <double, double> value = fHistManager->GetNormValue(theta_inf,theta_sup);
		fnorm_lab_exp.push_back(value.first);
		fnorm_lab_exp_err.push_back(value.second);
// WHEN DEUTERON BREAK-UP IS CALCULATED : WE MUST TAKE OUT THE EFFICIENCY
// 		fnorm_lab_exp.push_back(1);
// 		fnorm_lab_exp_err.push_back(0);
		}
}

void get_cs::TreatNodes(void)
{
cout << "Inside get_cs::TreatNodes()" << endl;

	//get therange
	if (fsystem=="CM")
	{
	ftheta_lab_node = fKinematic->ConvertAngleVector(fsystem,ftheta_cm_node);
	}
	else
	{
	ftheta_cm_node = fKinematic->ConvertAngleVector(fsystem,ftheta_lab_node);
	}

	for ( unsigned i =0 ; i < ftheta_lab_node.size() - 1; i++)
	{

	if ( ftheta_lab_node.at(i)!=-1 && ftheta_lab_node.at(i+1)!=-1)
		{
		ftheta_lab_inf.push_back(ftheta_lab_node.at(i));
		ftheta_lab_sup.push_back(ftheta_lab_node.at(i+1));
		}
	}
}


void get_cs::GetSumExpVectorLab()
{
cout << "Inside get_cs::GetSumExpVectorLab()" << endl;

	TAxis* axis = fExpHist->GetXaxis();
	int binx = fExpHist->GetNbinsX();
	double HMIN = axis->GetBinLowEdge(0);
	double HMAX = axis->GetBinLowEdge(binx) + axis->GetBinWidth(binx);

	for ( unsigned i =0 ; i < ftheta_lab_inf.size(); i++)
	{
	cout<< " INSIDE LOOP ON NODES "<<i<<endl;
	double theta_min=ftheta_lab_inf.at(i);
	double theta_max=ftheta_lab_sup.at(i);

	if(theta_min>=theta_max )
		{
		cout<< " ERROR IN NODE SEQUENCE >>  theta_min : "<<theta_min<< "  theta_max : "<<theta_max<<endl;
		exit(-1);
		}

// 	if(theta_min < HMIN && theta_max<  HMIN  )
// 		{
// 		cout<< " WARNING THE RANGE NUMBER "<<i+1<< "  LIES TOTALY OUT SIDE THE HISTOGRAM! "<<endl;
// 		cout<< " it's minimum : "<<theta_min<<" it will be replaced by (-1) "<<endl;
// 		ftheta_lab_inf.at(i)=-1;
// 	        ftheta_lab_sup.at(i)=-1;
// 		}
// 	else
	cout<< " theta_min "<<theta_min<<"\t theta_max "<<theta_max<<endl;
	if(theta_min<  HMIN  && theta_max > HMIN )
		{
		cout<< " WARNING THE RANGE NUMBER "<<i+1<< "  LIES PARTLY OUT SIDE THE HISTOGRAM! "<<endl;
		cout<< " it's minimum : "<<theta_min<<" will be shifted to  "<< HMIN<<endl;
		theta_min=HMIN;
		ftheta_lab_inf.at(i)=HMIN;
		}
	else
	if(theta_min<  HMAX  && theta_max>  HMAX )
		{
		cout<< " WARNING THE RANGE NUMBER "<<i+1<< "  LIES PARTLY OUT SIDE THE HISTOGRAM! "<<endl;
		cout<< " it's maximum : "<<theta_max<<" will be reduced to  "<< HMAX<<endl;
		theta_max=HMAX;
	        ftheta_lab_sup.at(i)=HMAX;
		}
// 	else
// 	if(theta_min >  HMAX  && theta_max>  HMAX )
// 		{
// 		cout<< " WARNING THE RANGE NUMBER "<<i+1<< "  LIES PARTLY OUT SIDE THE HISTOGRAM! "<<endl;
// 		cout<< " it's maximum : "<<theta_max<<" will be replaced by (-1)  "<<endl;
// 		ftheta_lab_inf.at(i)=-1;
// 	        ftheta_lab_sup.at(i)=-1;
// 		}

 	if(	theta_min==-1    || 	theta_max==-1 ) continue;
	int bin_min=fExpHist->FindBin(theta_min);//chercher le bin correspendant aux limites
	int bin_max=fExpHist->FindBin(theta_max);//chercher le bin correspendant aux limites
	double sum=0;

	cout<< " bin_min "<<bin_min<<endl;
	cout<< " bin_max "<<bin_max<<endl;

	for (int bin =bin_min; bin<=bin_max;bin++)
		{

		double W = axis->GetBinWidth(bin);
		double XMIN = axis->GetBinLowEdge(bin);
		double XMAX = axis->GetBinLowEdge(bin)+W;
		double factor=0;

		//cout<< " W "<<W<<endl;
		//cout<< " XMIN "<<XMIN<<endl;
		//cout<< " XMAX "<<XMAX<<endl;

		if( theta_min>XMIN && theta_min<XMAX ) // if we inside the first bin
		{
		factor=(XMAX- theta_min)/W;
		}
		else
		if(theta_max>XMIN && theta_max<XMAX)  // if we inside the last bin
		{
		factor=(theta_max - XMIN)/W;
		}
		else factor=1 ;

		//cout<< " factor "<<factor<<endl;
		//cout<< " fExpHist->GetBinContent( bin ) "<<fExpHist->GetBinContent( bin )<<endl;
		sum=sum+factor*fExpHist->GetBinContent( bin );
		//cout<< " sum after "<<sum<<endl;
		}// end of for on one range

	fcounts_lab_exp.push_back(sum);
	}// end of for all ranges


}

void get_cs::GetSumExpVectorLab_ROOT()
{
cout << "Inside get_cs::GetSumExpVectorLab_ROOT()" << endl;

	for ( unsigned i =0 ; i < ftheta_lab_inf.size() ; i++)
	{
		double theta_inf = ftheta_lab_inf.at(i);
		double theta_sup = ftheta_lab_sup.at(i);
		pair <double, double> value = fHistManager->GetSumValue(theta_inf,theta_sup);
		fcounts_lab_exp.push_back(value.first);
		fcounts_lab_exp_err.push_back(value.second);
		}

}

void get_cs::GetSigmaExpVectorLab1()
{
cout << "Inside get_cs::GetSigmaExpVectorLab1()" << endl;


	for ( unsigned i =0 ; i < ftheta_lab_inf.size(); i++)
	{
	cout<< " Inside get_cs::GetSigmaExpVectorLab1() : INSIDE LOOP ON NODES "<<i<<endl;
	double theta_min=ftheta_lab_inf.at(i);
	double theta_max=ftheta_lab_sup.at(i);
	double counts=fcounts_lab_exp.at(i);
	double counts_err=fcounts_lab_exp_err.at(i);

	//First round of crossection calulation
	pair < double, double > cross_section_1;

	// in case Ndet is normalized
        cross_section_1 = fError->GetSigmaAndError(counts, counts_err,theta_min*degree, theta_max*degree,1,0);
// 	 cout<<" value "<<cross_section_1.first<<endl;
// 	 cout<<" error "<<cross_section_1.second<<endl;

	ftheta_lab_exp.push_back((theta_min+theta_max)*0.5);
	fsigma_lab_exp_err.push_back(cross_section_1.second);
	fsigma_lab_exp.push_back(cross_section_1.first);
	} // loop on ranges


}

void get_cs::GetSigmaExpVectorLab2()
{
cout << "Inside get_cs::GetSigmaExpVectorLab2()" << endl;


	for ( unsigned i =0 ; i < ftheta_lab_inf.size(); i++)
	{
	//cout<< " INSIDE LOOP ON NODES "<<i<<endl;
	double theta_min = ftheta_lab_inf.at(i);
	double theta_max = ftheta_lab_sup.at(i);
	double  counts = fcounts_lab_exp.at(i);
	double  counts_err = fcounts_lab_exp_err.at(i);
	double 	norm = fnorm_lab_exp.at(i);
	double 	norm_err = fnorm_lab_exp_err.at(i);
	//cout<< " bin_min "<<bin_min<<endl;
	//cout<< " bin_max "<<bin_max<<endl;

	//First round of crossection calulation
	pair < double, double > cross_section_1;
	
	cross_section_1 = fError->GetSigmaAndError(counts, counts_err ,theta_min*degree, theta_max*degree , norm , norm_err );

	// cout<<" value "<<cross_section_1.first<<endl;
	// cout<<" error "<<cross_section_1.second<<endl;
	
	ftheta_lab_exp.push_back((theta_min+theta_max)*0.5);
	fsigma_lab_exp_err.push_back(cross_section_1.second);
	fsigma_lab_exp.push_back(cross_section_1.first);
	}// end of for all ranges


}

void get_cs::GetSumExpVectorLab_file(void)
{
cout << "Inside get_cs::GetSumExpVectorLab_file()" << endl;

//================  Get Data ====================
	ifstream inf;
	string bufstring,mot;
  	inf.open(fpath.c_str(), ios::in);
	int input_count=0;
  if(!inf)
    {
      cout << "ERROR : Could not open the file :"<<fpath<< endl;
      exit(-1);
    }
  else
    {
      while(inf>>mot)
	{
//--------------------------------------
		if(mot == "COUNTS")
		{
		input_count++;
		double buffer;
		do
			{
			inf>>buffer;
			fcounts_lab_exp.push_back(buffer);
			}
		while ( buffer>0)	;  fcounts_lab_exp.pop_back();
		} //if

		if(mot == "COUNTS_ERR")
		{
		input_count++;
		double buffer;
		do
			{
			inf>>buffer;
			fcounts_lab_exp_err.push_back(buffer);
			}
		while ( buffer>0)	;  fcounts_lab_exp_err.pop_back();
		} //if 
//--------------------------------------
         }// while
    }//else

	if(input_count!=2)
	{
	cout<<"ERROR: SOMETHING WRONG IN INPUTS NUMBER IN FILE "<<fpath<<endl;
	cout<<"NUMBER OF INPUTS FOUND IS "<<input_count<<endl;
	exit(-1);
	}

  inf.close();

}

void get_cs::TransformTheoryCrossSection(unsigned i)
{

// fTheoCs.at(i);
 Interpolator*    Interpol_theta;
 Interpolator*    Interpol_jacob;

 Interpol_theta = new Interpolator( fKinematic->ftheta_cm , fKinematic->ftheta_lab );
 Interpol_jacob = new Interpolator( fKinematic->ftheta_cm , fKinematic->fjacob_lab_cm  );

// transform theo cm -> lab
if (fTheoCs.at(i)->fsystem=="CM" && (fTheoCs.at(i)->fcode=="DWUCK" || fTheoCs.at(i)->fcode=="TWOFNR"))
	{
	for (unsigned j=0 ; j<fTheoCs.at(i)->ftheta_cm_theo.size();j++)
		{
		double theta = fTheoCs.at(i)->ftheta_cm_theo.at(j);
		double theta_inv = fTheoCs.at(i)->ftheta_cm_theo_inv.at(j);

	//Sigma CM-->LAB
	// we use theta with no iversion to convert the cross section
		double jacob = Interpol_jacob->Eval(theta);
		fTheoCs.at(i)->fsigma_lab_theo.push_back( (1/jacob)*fTheoCs.at(i)->fsigma_cm_theo.at(j) );

	//Thetas CM-->LAB
	// Since in Dwuck the  x'ox goes from light to heavy  (CHECK)
		double theta_lab = Interpol_theta->Eval(theta);
		double theta_lab_inv = Interpol_theta->Eval(180-theta);
               // purily kinematic transformation
		//fTheoCs.at(i)->ftheta_lab_theo_inv.push_back( theta_lab_inv ); // <-- do not give a good sum result
		fTheoCs.at(i)->ftheta_lab_theo_inv.push_back( 180-theta_lab ); // <-- good sum result!
		fTheoCs.at(i)->ftheta_lab_theo.push_back( theta_lab ); // in case its the good one
		}
	}
}


void get_cs::GetSigmaExpVectorCM(void)
{
cout << "Inside get_cs::GetSigmaExpVectorCM()" << endl;
// transform angles lab -> cm 

 Interpolator*    Interpol_theta;
 Interpolator*    Interpol_jacob;

 Interpol_theta = new Interpolator( fKinematic->ftheta_lab , fKinematic->ftheta_cm ); // theta_cm vs Theta_lab
 Interpol_jacob = new Interpolator( fKinematic->ftheta_lab , fKinematic->fjacob_lab_cm  ); // jacob_lab_cm vs Theta_lab

cout << "----------------------------------------------------------------------------------------------------" << endl;
for (unsigned i=0 ; i<ftheta_lab_exp.size();i++)
	{
	double borne_inf = ftheta_lab_inf.at(i); //cout<<" borne_inf : "<<borne_inf<<"   ";
	double borne_sup = ftheta_lab_sup.at(i); //cout<<" borne_sup: "<<borne_sup<<"    ";
	double jacob_mean_high = Interpol_jacob->Eval(borne_sup) ;
	double jacob_mean_low = Interpol_jacob->Eval(borne_inf) ;

//Sigma Lab-->CM
	double average=0, jacob_i=0, variance=0;
	int step_num = 100;

//------------------------------------------------ 1st approach 
//  jacob_mean by integral
	double jacob_mean= (Interpol_jacob->Integ(borne_inf, borne_sup) ) / fabs(borne_sup-borne_inf);
//  maximum error
	//double jacob_mean_error = max (fabs(jacob_mean-jacob_mean_low),fabs(jacob_mean-jacob_mean_high));

//------------------------------------------------ 2nd approach

// 2nd approach : by discretization
// 	Since no function to calculate variances ;
// 	idea : calculate the ecart type of hundred values with a fixed step within the angular range

// 	for (int j = 0; j<step_num; j++)
// 	{
// 	jacob_i = Interpol_jacob->Eval(borne_inf + (borne_sup-borne_inf)/(step_num-1) * i ) ;
// 	average = average + jacob_i ;
// 	}
// 	average = average/step_num;
// 	double jacob_mean_error = average ;

// 2nd approach : ecart type
	for (int j = 0; j<step_num; j++)
	{
	jacob_i = Interpol_jacob->Eval(borne_inf + (borne_sup-borne_inf)/(step_num-1) * j ) ;
	variance = variance + (jacob_i - jacob_mean)*(jacob_i - jacob_mean) ;
	}
	variance = variance/step_num;
	double jacob_mean_error = sqrt(variance) ;

//------------------------------------------------

	fjacob_LabtoCM.push_back(jacob_mean);
	fjacob_LabtoCM_err.push_back(jacob_mean_error);

	// cout<<borne_inf<<" | "<<borne_sup<<endl;
	// cout<<jacob_mean<<" * "<<fsigma_lab_exp.at(i)<<" = "<<jacob_mean*fsigma_lab_exp.at(i)<<endl;
	// cout<<jacob_mean_low<<" < "<<jacob_mean<<" < "<<jacob_mean_high<<endl;
	// cout<<jacob_mean_error<<" <--- error "<<endl;

	// function (Jacobien )
	pair < double, double > cross_section_3;
	pair < double, double > cross_section_2;
	pair < double, double > jacob;
	
	cross_section_2 = make_pair (fsigma_lab_exp.at(i), fsigma_lab_exp_err.at(i));
	jacob = make_pair (jacob_mean, jacob_mean_error);

	cross_section_3 = fError->GetSigmaCMfromSigmaLab(cross_section_2,jacob);

	fsigma_cm_exp.push_back( cross_section_3.first );
	fsigma_cm_exp_err.push_back( cross_section_3.second ); // only the statistic error for the mean while
	
//Thetas Lab-->CM
	// Since in Dwuck the  x'ox goes from light to heavy we add 180 - ... (CHECK)
	ftheta_cm_exp.push_back( 180 - Interpol_theta->Eval(ftheta_lab_exp.at(i)) );
	// when adding the error left and right --> they become right and left
	// this will be taken into account in the drawing step
	ftheta_cm_inf.push_back(180 - Interpol_theta->Eval( borne_inf ));
	ftheta_cm_sup.push_back(180 - Interpol_theta->Eval( borne_sup ));
	}

}

void get_cs::DrawJacobVector(void)
{
cout << "Inside get_cs::DrawJacobVector()" << endl;

// Show();

 int tab6[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/1,  /*mcolor*/2,  /*mstyle*/28,  /*msize*/1};
 fDrawer->SetAttributes("jacob vs theta (LAB)","jacob vs theta (LAB)","#theta_{Lab}","jacob. (Lab to CM)",  tab6 );

 TGraphAsymmErrors* gr_jacob_lab_to_cm =
fDrawer->ConvertToGraphAsymmErrors( ftheta_lab_exp,ftheta_lab_inf,ftheta_lab_sup,fjacob_LabtoCM,fjacob_LabtoCM_err);

fOutputFile->cd();
gr_jacob_lab_to_cm->Write();

}


void get_cs::DrawNormSumVector(void)
{
cout << "Inside get_cs::DrawNormSumVector()" << endl;

// Show();

 int tab6[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/1,  /*mcolor*/2,  /*mstyle*/28,  /*msize*/1};
 fDrawer->SetAttributes("norm vs theta (LAB)","norm vs theta (LAB)","#theta_{Lab}","#epsilon_{Lab}",  tab6 );

 TGraphAsymmErrors* gr_norm_lab =
fDrawer->ConvertToGraphAsymmErrors( ftheta_lab_exp,ftheta_lab_inf,ftheta_lab_sup,fnorm_lab_exp,fnorm_lab_exp_err);

fOutputFile->cd();
gr_norm_lab->Write();

 int tab7[6]={/*lcolor*/3,  /*lstyle*/1,  /*lwidth*/1,  /*mcolor*/2,  /*mstyle*/28,  /*msize*/1};
 fDrawer->SetAttributes("sum vs theta (LAB)","sum vs theta (LAB)","#theta_{Lab}","Counts_{Lab}}",  tab7 );

 TGraphAsymmErrors*  gr_sum_lab =
fDrawer->ConvertToGraphAsymmErrors(ftheta_lab_exp, ftheta_lab_inf, ftheta_lab_sup, fcounts_lab_exp , fcounts_lab_exp_err);

fOutputFile->cd();
gr_sum_lab->Write();

}


void get_cs::DrawDeltaCosVector(void)
{
cout << "Inside get_cs::DrawDeltaCosVector()" << endl;

vector <double> delta_cos_lab;
vector <double> sin_dtheta_lab;
double delta_cos=0;
double sin_dtheta=0;

for(unsigned i=0;i< ftheta_lab_inf.size();i++)
	{
	double theta_min=ftheta_lab_inf.at(i) * degree;
	double theta_max=ftheta_lab_sup.at(i) * degree;

	delta_cos = fabs(cos(theta_min) - cos(theta_max)) ;
	delta_cos_lab.push_back(delta_cos);

double dtheta = fabs(theta_min-theta_max);
sin_dtheta = sin((theta_min+theta_max)*0.5)*dtheta;
sin_dtheta_lab.push_back(sin_dtheta);

	}


cout<<"INFOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo : delta_cos_lab"<<endl;
	for (unsigned i =0;i<delta_cos_lab.size();i++)
	{
	cout<<delta_cos_lab.at(i)<<"  ";
	}
cout<<endl;


 int tab[6]={/*lcolor*/2,  /*lstyle*/1,  /*lwidth*/1,  /*mcolor*/2,  /*mstyle*/28,  /*msize*/1};
 fDrawer->SetAttributes("delta_cos_lab","|#Delta cos(#theta_{max}-#theta_{min})|","#theta_{Lab}","|#Delta cos(#theta_{max}-#theta_{min}})|",  tab );
TGraph*  gr_delta_cos_lab = fDrawer->ConvertToGraph(ftheta_lab_exp, delta_cos_lab);

fOutputFile->cd();
gr_delta_cos_lab->Write();

 int tab1[6]={/*lcolor*/3,  /*lstyle*/1,  /*lwidth*/1,  /*mcolor*/3,  /*mstyle*/28,  /*msize*/1};
 fDrawer->SetAttributes("sin_dtheta_lab","|sin(#theta_{mean} d#theta|","#theta_{Lab}","|#Delta cos(#theta_{max}-#theta_{min}})|",  tab1 );
TGraph*  gr_sin_dtheta_lab = fDrawer->ConvertToGraph(ftheta_lab_exp, sin_dtheta_lab);

fOutputFile->cd();
gr_sin_dtheta_lab->Write();

}

void get_cs::DrawChi2(void)
{
cout << "Inside get_cs::DrawChi2()" << endl;


 int tab6[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/1,  /*mcolor*/2,  /*mstyle*/28,  /*msize*/1};
 fDrawer->SetAttributes("#chi^{2} vs SF","#chi^{2} vs SF","SF","#chi^{2}",  tab6 );

 TGraph* gr_chi2_SF = fDrawer->ConvertToGraph( fSpectroFactor,fSpChi2);

fOutputFile->cd();
gr_chi2_SF->Write();

}


void get_cs::GetSpectroscopicFactor(unsigned curve_num)
{
cout << "Inside get_cs::GetSpectroscopicFactor()" << endl;

// fsigma_cm_exp;
// fsigma_cm_exp_err; // only the statistic error for the mean while

fSpectroFactorEstimate.at(curve_num) = 0  ; 
fChi2Estimate.at(curve_num) = 10000  ; 
fSpectroFactorUpper.at(curve_num) = 0 ; 
fChi2EstimateUpper.at(curve_num) = 10000 ; 
fSpectroFactorLower.at(curve_num) = 0 ; 
fChi2EstimateLower.at(curve_num) = 10000 ; 

fMixFactorEstimate.at(curve_num) = 0 ; 
fMixFactorEstimateUpper.at(curve_num) = 0 ;
fMixFactorEstimateLower.at(curve_num) = 0 ;

fSpectroFactor.clear();
fSpChi2.clear();

double Sf = 0.0001 ;
double Sf_step = 0.0005 ;
double chi2 = 0 ;
int N =0 ;

while (Sf<=10)
	{
	N=0 ;
	double chi2_element = 0 ;
	double sigma_cm_theo = 0 ;


	// en cas ou le fit se fait sur les points les plus proches de zero CM:
	for (unsigned i = 0; i < fsigma_cm_exp.size(); i++)
		{
		//if (i==2) continue ;
		chi2_element = 0 ;
		N++;
		sigma_cm_theo = fTheoCs.at(curve_num)->GetSigmaTheo(ftheta_cm_exp.at(i)) ;
		//chi2_element = fabs( (fsigma_cm_exp.at(i) - sigma_cm_theo*Sf)/fsigma_cm_exp_err.at(i)) ; //def 1
		chi2_element = pow( (fsigma_cm_exp.at(i) - sigma_cm_theo*Sf)/fsigma_cm_exp_err.at(i),2) ;  //def 2
		chi2 = chi2 + chi2_element ;
		}

	chi2 = chi2/(N-1) ; // normalise chi2
	
	//search for minimum
	if ( chi2 < fChi2Estimate.at(curve_num) )
		{
		//cout<<" minimum...................... : SF - chi2 : "<<Sf<<" - "<<chi2<<endl ;
		fChi2Estimate.at(curve_num) = chi2 ;
		fSpectroFactorEstimate.at(curve_num) = Sf ;
		fMixFactorEstimate.at(curve_num) = 0 ;
		}
	
	//store chi2 and associated SF
	fSpChi2.push_back(chi2);
	fSpectroFactor.push_back(Sf);
	chi2=0;
	
	//change Sf
	Sf = Sf + Sf_step;
	}

	cout<<" # # # # # # # # # # # # # # # # # # # # "<<endl ;
	cout<<" #                                     # "<<endl ;
	cout<<" # Fit is made on  : "<<N<<"/"<<fsigma_cm_exp.size()<<" point(s)      #"<<endl ;
	cout<<" #                                     # "<<endl ;
	cout<<" # # # # # # # # # # # # # # # # # # # # "<<endl ;

//GetSpectroscopicFactorError
	for (unsigned i = 0 ; i<fSpChi2.size() ; i++)
	{
		if ( fabs((fChi2Estimate.at(curve_num) + 1) - fSpChi2.at(i)) < 0.01 )
		{
// 		cout<<" fabs((Chi2min+1)-chi2) : "<<fabs((fChi2Estimate+1) - fSpChi2.at(i))<<endl ;
// 		cout<<" SF - chi2 : "<<fSpectroFactor.at(i)<<" - "<<fSpChi2.at(i)<<endl ;
		
		if (fSpChi2.at(i)<fSpChi2.at(i+1))
			{
// 			cout<<" Tag SF + "<<endl ;
			fSpectroFactorUpper.at(curve_num) = fSpectroFactor.at(i) ;
			fChi2EstimateUpper.at(curve_num) = fSpChi2.at(i);
			}
		else
			{
// 			cout<<" Tag SF - "<<endl ;
			fSpectroFactorLower.at(curve_num) = fSpectroFactor.at(i) ;
			fChi2EstimateLower.at(curve_num) = fSpChi2.at(i) ;
			}
		}
	}


}



void get_cs::GetMixedSpectroscopicFactor(unsigned curve1, unsigned curve2)
{
cout << "Inside get_cs::GetMixedSpectroscopicFactor()" << endl;

// fsigma_cm_exp;
// fsigma_cm_exp_err; // only the statistic error for the mean while

// it adds an element to the existing vectors
fSpectroFactorEstimate.push_back(0)  ; 
fChi2Estimate.push_back(10000)  ; 
fSpectroFactorUpper.push_back(0) ; 
fChi2EstimateUpper.push_back(10000) ; 
fSpectroFactorLower.push_back(0); 
fChi2EstimateLower.push_back(10000) ; 

fMixFactorEstimate.push_back(0); 
fMixFactorEstimateLower.push_back(0) ; 
fMixFactorEstimateUpper.push_back(0) ; 
fcolor_theo.push_back(1);

fSpectroFactor.clear();
fSpChi2.clear();

double Sf = 0.0001 ;
double Sf_step = 0.0005 ;
double chi2 = 0 ;
int N = 0 ;

//create the new combined curve
double MixFactor = 0 ;  // start value 
int MixFactorSteps = 50 ; 

for (int i = 0 ; i <= MixFactorSteps ; i++) {

// mixing factor 
MixFactor =  (1.0*i) / (MixFactorSteps) ; 
Sf = 0.0001 ;

while (Sf<=10)	{

	chi2 = 0 ;
	N=0 ;
	double chi2_element = 0 ;
	double sigma_cm_theo = 0 ;

	// en cas ou le fit se fait sur les points les plus proches de zero CM:
	for (unsigned i = 0; i < fsigma_cm_exp.size(); i++) {
		//if (i==2) continue ;
		chi2_element = 0 ; 
		N++;
		sigma_cm_theo = MixFactor*fTheoCs.at(curve1)->GetSigmaTheo(ftheta_cm_exp.at(i)) ;
		sigma_cm_theo = sigma_cm_theo  + (1-MixFactor)*fTheoCs.at(curve2)->GetSigmaTheo(ftheta_cm_exp.at(i)) ;
		//chi2_element = fabs( (fsigma_cm_exp.at(i) - sigma_cm_theo*Sf)/fsigma_cm_exp_err.at(i)) ; //def 1
		chi2_element = pow( (fsigma_cm_exp.at(i) - sigma_cm_theo*Sf)/fsigma_cm_exp_err.at(i),2) ;  //def 2
		chi2 = chi2 + chi2_element ;
		}
	
	// normalise chi2
	chi2 = chi2/(N-1) ; 
	
	//search for minimum
	if ( chi2 < fChi2Estimate.back() )
		{
		//cout<<" minimum...................... : SF - chi2 : "<<Sf<<" - "<<chi2<<endl ;
		fChi2Estimate.back() = chi2 ;
		fSpectroFactorEstimate.back() = Sf ;
                fMixFactorEstimate.back() = MixFactor ; 
		}
	
	//store chi2 and associated SF
	fSpChi2.push_back(chi2);
	fSpectroFactor.push_back(Sf);
	
	//change Sf
	Sf = Sf + Sf_step;
	}// end of while 

} // end of for

// create add an element to the theo_tool
string code = fTheoCs.at(curve1)->fcode;  // Mixing different codes doesnt seem write
string system = fTheoCs.at(curve1)->fsystem; 
theo_tool* mixed = new theo_tool(code,system,"", true) ; 
mixed->Mix( fTheoCs.at(curve1)->fpath , fTheoCs.at(curve2)->fpath , fMixFactorEstimate.back()); 
fTheoCs.push_back(mixed) ; 


	cout<<" # # # # # # # # # # # # # # # # # # # # "<<endl ;
	cout<<" #                MIXED                # "<<endl ;
	cout<<" # Fit is made on  : "<<N<<"/"<<fsigma_cm_exp.size()<<" point(s)      #"<<endl ;
	cout<<" #                                     # "<<endl ;
	cout<<" # # # # # # # # # # # # # # # # # # # # "<<endl ;

//GetSpectroscopicFactorError
	for (unsigned i = 0 ; i<fSpChi2.size() ; i++)
	{
		if ( fabs((fChi2Estimate.back() + 1) - fSpChi2.at(i)) < 0.01 )
		{
// 		cout<<" fabs((Chi2min+1)-chi2) : "<<fabs((fChi2Estimate+1) - fSpChi2.at(i))<<endl ;
// 		cout<<" SF - chi2 : "<<fSpectroFactor.at(i)<<" - "<<fSpChi2.at(i)<<endl ;
		
		if (fSpChi2.at(i)<fSpChi2.at(i+1))
			{
// 			cout<<" Tag SF + "<<endl ;
			fSpectroFactorUpper.back() = fSpectroFactor.at(i) ;
			fChi2EstimateUpper.back() = fSpChi2.at(i);
			}
		else
			{
// 			cout<<" Tag SF - "<<endl ;
			fSpectroFactorLower.back() = fSpectroFactor.at(i) ;
			fChi2EstimateLower.back() = fSpChi2.at(i) ;
			}
		}
	}


}





void get_cs::AddGraphs()
{

cout << "Inside get_cs::AddGraphs()" << endl;


 int tab7[6]={/*lcolor*/1,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/2,  /*mstyle*/28,  /*msize*/0};
 fDrawer->SetAttributes("sigma vs theta (CM)","sigma vs theta (CM)","#theta(CM) [#circ]","d#sigma/d#Omega [mb/sr]", tab7 );
//Attention inversion des positions de ftheta_cm_inf et ftheta_cm_sup
 TGraphAsymmErrors* gr_sigma_cm =
fDrawer->ConvertToGraphAsymmErrors(ftheta_cm_exp,ftheta_cm_sup,ftheta_cm_inf,fsigma_cm_exp,fsigma_cm_exp_err);

	fOutputFile->cd();
	gr_sigma_cm->Write("Simulation");

int tab[6]={/*lcolor*/1,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/0};
fDrawer->SetAttributes("sigma vs theta (CM)","sigma vs theta (CM)","#theta(CM) [#circ]","d#sigma/d#Omega [mb/sr]", tab );

vector < TGraph* > GraphVector;
for (unsigned i=0;i<fTheoCs.size();i++)
{

 if (i==0)
	{
	fDrawer->fline_width=4;
	fDrawer->fmarker_style=7;
	}
    else
	{
	fDrawer->fline_width=2;
	fDrawer->fmarker_style=6;
	}

fDrawer->fline_color=fcolor_theo.at(i);			
fDrawer->fmarker_color=fcolor_theo.at(i);
GraphVector.push_back(fDrawer->ConvertToGraph(fTheoCs.at(i)->ftheta_cm_theo,fTheoCs.at(i)->fsigma_cm_theo)) ;
}



TMultiGraph *mg = new TMultiGraph();
TMultiGraph *mg_theo = new TMultiGraph();

// 30 graphs for indivicual graphs
TCanvas *c_IndiviGraph[50];

for (unsigned i=0;i<fTheoCs.size();i++)
{
mg->Add(GraphVector.at(i)) ;
mg_theo->Add(GraphVector.at(i)) ;

TString nameTitle = Form("c_graph%d",(int)i) ;
c_IndiviGraph[i]= new TCanvas(nameTitle,nameTitle);
c_IndiviGraph[i]->SetLogy();
GraphVector.at(i)->SetTitle(Form("E* = %1.3f MeV",fKinematic->fEx4));
GraphVector.at(i)->GetXaxis()->SetRangeUser(0,35);
GraphVector.at(i)->GetXaxis()->SetLabelSize(0.05);
GraphVector.at(i)->GetXaxis()->SetTitleSize(0.05);
GraphVector.at(i)->GetXaxis()->SetTitle("#theta(CM) [#circ]");
GraphVector.at(i)->GetYaxis()->SetRangeUser(0.1,20.0);
GraphVector.at(i)->GetYaxis()->SetLabelSize(0.05);
GraphVector.at(i)->GetYaxis()->SetTitleSize(0.05);
GraphVector.at(i)->GetYaxis()->SetTitle("d#sigma/d#Omega [mb/sr]");
GraphVector.at(i)->Draw("al");


// Write SF
double SfError= max(fSpectroFactorUpper.at(i) - fSpectroFactorEstimate.at(i), fSpectroFactorEstimate.at(i) - fSpectroFactorLower.at(i));
TLatex* sf = new TLatex(20,6.3,Form("SF = %1.2f #pm %1.2f",fSpectroFactorEstimate.at(i),SfError));
sf->SetTextFont(32);
sf->SetTextSize(0.07);
sf->SetLineWidth(2);
sf->Draw();


// Write Xi2
TLatex* chi2 = new TLatex(25.7,4.3,Form("#chi^{2} = %1.2f",fChi2Estimate.at(i)));
chi2->SetTextFont(32);
chi2->SetTextSize(0.07);
chi2->SetLineWidth(2);
chi2->Draw();

//Add experimental (on individual)
if (fFit!=0) gr_sigma_cm->Draw("same p");
c_IndiviGraph[i]->Write();
//c_IndiviGraph[i]->SaveAs(Form("./output/prlni69/level0/graph%d_level0.png",(int)i));
c_IndiviGraph[i]->SaveAs(Form("./graph%d_level0.png",(int)i));

}

//Add experimental (on all the graphs) 
if (fFit!=0) mg->Add(gr_sigma_cm);

fOutputFile->cd();
mg->Write("TheoryAndExperiment");


TCanvas *c_graph= new TCanvas("c_graph","c_graph");
   c_graph->SetHighLightColor(0);
  // c_graph->Range(-5.087456,-1.384271,44.97818,1.657298);
   c_graph->Range(-4.996511,-1.34227,45.06913,1.699299);
   c_graph->SetFillColor(0);
   c_graph->SetBorderMode(0);
   c_graph->SetBorderSize(2);
   c_graph->SetTopMargin(0.08515535);
   c_graph->SetBottomMargin(0.1162255);
   c_graph->SetFrameBorderMode(0);
   c_graph->SetLogy();
   c_graph->Modified();

// c_graph->Divide(2,2);

// c_graph->cd(1);
// TVirtualPad* pad1 = c_graph->GetPad(1); pad1->SetLogy();
// mg_theo->DrawClone("al");

// c_graph->cd(2);
// TVirtualPad* pad2= c_graph->GetPad(2); pad2->SetLogy();
// gr_sigma_cm->Draw("ap");
// 
// 
// c_graph->cd(3);
// TVirtualPad* pad3= c_graph->GetPad(3); pad3->SetLogy();
// mg->DrawClone("ap");
// 
// c_graph->cd(4);
// TVirtualPad* pad4= c_graph->GetPad(4); pad4->SetLogy();
// mg->Draw("ap");

mg->SetTitle(Form("E* = %1.3f MeV",fKinematic->fEx4));
mg->Draw("al");

    mg->GetXaxis()->SetRangeUser(0,35);
//     mg->GetXaxis()->SetTitleOffset(0.085);
//     mg->GetXaxis()->SetLabelOffset(0.005);
    mg->GetXaxis()->SetLabelSize(0.05);
    mg->GetXaxis()->SetTitleSize(0.05);
    mg->GetXaxis()->SetTitle("#theta(CM) [#circ]");

    mg->GetYaxis()->SetRangeUser(0.1,20.0);
//     mg->GetYaxis()->SetRangeUser(0.05,61.0);
//     mg->GetYaxis()->SetLabelOffset(0.005);
//     mg->GetYaxis()->SetTitleOffset(0.080);
    mg->GetYaxis()->SetLabelSize(0.05);
    mg->GetYaxis()->SetTitleSize(0.05);
    mg->GetYaxis()->SetTitle("d#sigma/d#Omega [mb/sr]");


if(1)
{

   TLatex *    tex =  new TLatex(19.37948,14.51215,"dummy");


for (unsigned i=0;i<fcolor_theo.size();i++)
{

if (fcolor_theo.at(i) == 4) {
	tex =  new TLatex(19,14.5,"l = 0");
// 	tex =  new TLatex(19,14.5,"l = 0 , (3s1/2)");
        tex->SetTextColor(fcolor_theo.at(i));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
// 	tex->SetTextSize(0.055);
	tex->SetLineWidth(2);
	tex->Draw(); }

if (fcolor_theo.at(i) == 6) {
	tex = new TLatex(30.4,10.24726,"l = 4");
// 	tex = new TLatex(30.4,10.24726,"l = 1 , (2p1/2)");
        tex->SetTextColor(fcolor_theo.at(i));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw(); }

if (fcolor_theo.at(i) == 418) {
	tex =  new TLatex(19,14.7,"l = 2");
// 	tex =  new TLatex(19,14.7,"l = 2 , (2d5/2)");
        tex->SetTextColor(fcolor_theo.at(i));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw(); }
if (fcolor_theo.at(i) == 9) {
	tex =  new TLatex(19,14.7,"l = 2");
// 	tex =  new TLatex(19,14.7,"l = 2 , (2d3/2)");
        tex->SetTextColor(fcolor_theo.at(i));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw(); }


if (fcolor_theo.at(i) == 2) {
	tex = new TLatex(30.4,14.7,"l = 4");
// 	tex = new TLatex(30.4,14.7,"l = 4 , (1g9/2)");
        tex->SetTextColor(fcolor_theo.at(i));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw(); }
if (fcolor_theo.at(i) == 22) {
	tex = new TLatex(30.4,14.7,"l = 4 , (1g7/2)");
        tex->SetTextColor(fcolor_theo.at(i));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw(); }

 if (fcolor_theo.at(i) == 7) {
	tex =  new TLatex(19,14.7,"l = 5");
// 	tex =  new TLatex(19,14.7,"l = 5 , (1h11/2)");
        tex->SetTextColor(fcolor_theo.at(i));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw(); }


 if (fcolor_theo.at(i) == 1) {
	tex =  new TLatex(19,14.7,"mixed");
// 	tex =  new TLatex(19,14.7,"l = 5 , (1h11/2)");
        tex->SetTextColor(fcolor_theo.at(i));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw(); }

}

      tex = new TLatex(28.7,4.3,Form("#chi^{2} = %1.2f",fChi2Estimate.at(0)));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw();

double SfError= max(fSpectroFactorUpper.at(0) - fSpectroFactorEstimate.at(0), fSpectroFactorEstimate.at(0) - fSpectroFactorLower.at(0));
	tex = new TLatex(24,6,Form("SF = %1.2f #pm %1.2f",fSpectroFactorEstimate.at(0),SfError));
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineWidth(2);
	tex->Draw();

      tex = new TLatex(1.42781,0.148522,"E2 #otimes S");
	tex->SetTextFont(32);
	tex->SetTextSize(0.07);
	tex->SetLineColor(2);
	tex->SetLineWidth(2);
	tex->Draw();

//    TLine *line = new TLine(21,9,35,9);
//    line->SetLineWidth(2);
//    line->Draw();

}


c_graph->Write();


}

// Rappel
// =================================================================
// fk_ratio_lab;
// fT3_lab;
// ftheta_cm;  //  theta_cm sequence not related to experimental or theoretical values
// ftheta_lab;  //  theta_lab sequence not related to experimental or theoretical values
// fjacob_lab_cm;  // f(theta_lab) : Convert experimental cross section from lab to cm
// fjacob_cm_lab; // f(theta_cm) :Convert theoretical cross section from cm to lab
// -----------------------------------------------------------------------------
// ftheta_lab_exp;
// ftheta_lab_exp_err_left;
// ftheta_lab_exp_err_right;
// 
// ftheta_lab_inf;
// ftheta_lab_sup;
// 
// ftheta_cm_inf;
// ftheta_cm_sup;
// 
// ftheta_cm_exp;
// ftheta_cm_exp_err_left;
// ftheta_cm_exp_err_right;
// 
// fsigma_lab_exp;
// fsigma_lab_exp_err;
// 
// fsigma_cm_exp;
// fsigma_cm_exp_err;
// ---------------------------------------------------------------------------
// ConvertToGraph(vec_x,vec_y,line_color,line_style,line_width,marker_color,marker_style,marker_size)
// =================================================================


/*
TCanvas *c1 = new TCanvas("can_draw","multigraph",200,10,700,500);
c1->cd();
c1->SetLogy();

   mg->Draw("al");
   
     //force drawing of canvas to generate the fit TPaveStats
   c1->Update();
   TPaveStats *stats1 = (TPaveStats*)GraphVector.at(0)->GetListOfFunctions()->FindObject("stats");
   TPaveStats *stats2 = (TPaveStats*)GraphVector.at(1)->GetListOfFunctions()->FindObject("stats");
   stats1->SetTextColor(kBlue); 
   stats2->SetTextColor(kRed); 
   stats1->SetX1NDC(0.12); stats1->SetX2NDC(0.32); stats1->SetY1NDC(0.75);
   stats2->SetX1NDC(0.72); stats2->SetX2NDC(0.92); stats2->SetY1NDC(0.78);
   c1->Modified();

TFile* output_file= new TFile("./output/test_draw.root","RECREATE");
output_file->cd();

//TVirtualPad* pad1 = c1->GetPad(1);
//pad1->SetLogy();

c1->Write();
*/

//output_file->Write();
