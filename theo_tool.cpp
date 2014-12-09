#include "theo_tool.h"

theo_tool::~theo_tool(void)
{
cout << "Inside theo_tool::Destructor()" << endl;

}

theo_tool::theo_tool(string code, string system, string path)
{
cout << "Inside theo_tool::theo_tool()" << endl;
fcode=code;
fpath=path;
fsystem=system;

GetInput();

fOutputFile= new TFile("./output/theo_tool_output.root","RECREATE");
//fOutputFile->cd();
fDrawer= new draw_tool("./output/theo_tool_drawer.root") ;
}

void theo_tool::Show()
{
cout << "Inside theo_tool::Show()" << endl;
cout << "=================================================" << endl;
cout << "CODE (THEO)		" <<fcode<< endl;
cout << "SYSTEM (THEO) 		" <<fsystem<< endl;
cout << "PATH (THEO) 		" <<fpath<< endl;
cout << "-------------------------------------------------" << endl;

cout<<" THEORY CROSS SECTION CM : "<<ftheta_cm_theo.size()<< " Angles "<<endl;
for (unsigned i=0; i< ftheta_cm_theo.size();i++)
{
//if (i%20==0)
//cout<<i%20<<" " << ftheta_cm_theo.at(i)<<" / "<<ftheta_cm_theo_inv.at(i)<<" <--> "<<fsigma_cm_theo.at(i)<<endl;
}

cout << "-------------------------------------------------" << endl;

cout<<" THEORY CROSS SECTION LAB : "<<ftheta_lab_theo.size()<< " Angles "<<endl;
for (unsigned i=0; i< ftheta_lab_theo.size();i++)
{
//if (i%20==0)
//cout<<i%20<<" " <<ftheta_lab_theo.at(i)<<" / "<<ftheta_lab_theo_inv.at(i)<<" <--> "<<fsigma_lab_theo.at(i)<<endl;
}

cout << "=================================================" << endl;
}

void theo_tool::GetInput(void)
{
cout << "Inside theo_tool::GetInput()" << endl;
double angle,cs,buffer;

ifstream inf;
inf.open(fpath.c_str(), ifstream::in);
if (!inf) {cout<<"ERROR IN OPENING CROSS SECTION FILE : "<<fpath<<endl; exit(-1);}
else
	{
//----------------------------------------------------------------------
	if (fcode=="DWUCK")
		{
		cout<<" READING FILE "<<fpath<<" (DWUCK) IN PROGRESS "<<endl;
		while(!inf.eof())
			{
			inf>>angle>>cs;
			fsigma_cm_theo.push_back(cs);
			ftheta_cm_theo.push_back(angle);
			ftheta_cm_theo_inv.push_back(180-angle);
			}
		}		
//----------------------------------------------------------------------
	if (fcode=="TWOFNR")
		{
		cout<<" READING FILE "<<fpath<<" (TWOFNR) IN PROGRESS "<<endl;
		while(!inf.eof())
			{
			inf>>angle>>cs>>buffer;
			fsigma_cm_theo.push_back(cs);
			ftheta_cm_theo.push_back(angle);
			ftheta_cm_theo_inv.push_back(180-angle);
			}
		}
//----------------------------------------------------------------------
// 	if (fcode=="OTHER")
// 		{
// 		cout<<" READING FILE "<<path_cs_theo<<" (DWUCK) IN PROGRESS "<<endl;
// 		while(!inf.eof())
// 			{
// 			inf>>angle>>cs;
// 			sigma_cm_theo.push_back(cs);
// 			theta_cm_theo.push_back(angle);
// 			}
// 	cout<<" THEORY CEOSS SECTION "<<endl;
// 	for (int i=0; i<(int)  theta_cm_theo.size();i++)
// 	if (i%10==0)cout<<"  "<< theta_cm_theo.at(i)<<"  "<<sigma_cm_theo.at(i)<<endl;	
// 		}

	}

  inf.close();

}

double theo_tool::GetSigmaTheo(double theta_cm)
{

for (unsigned i=0; i< ftheta_cm_theo.size()-1;i++)
{
if (theta_cm>=ftheta_cm_theo.at(i) && theta_cm<=ftheta_cm_theo.at(i+1) )
return ( fsigma_cm_theo.at(i) + fsigma_cm_theo.at(i+1) ) / 2.0;
}

}


void theo_tool::ShowIntegral(void)
{

double sum=0;
for (unsigned i=0; i< ftheta_cm_theo.size()-1;i++)
{
double theta_1=ftheta_cm_theo.at(i);
double theta_2=ftheta_cm_theo.at(i+1);
double sigma_1=fsigma_cm_theo.at(i);
double sigma_2=fsigma_cm_theo.at(i+1);
sum=sum+(fabs(theta_2-theta_1)*degree)*sin((theta_2+theta_1)*0.5*degree)*(sigma_1+sigma_2)*0.5;
}
sum=sum*2*pi;

cout<<" THEORY CROSS SECTION INTEGRAL (CM) : "<<sum<<endl;

// sum=0;
//  Interpolator*    Interpol_sigma = new Interpolator( ftheta_cm_theo , fsigma_cm_theo  );
// sum= Interpol_sigma->Integ(ftheta_cm_theo.back(), ftheta_cm_theo.front());
// sum=sum*2*pi;
// cout<<" THEORY CROSS SECTION INTEGRAL (CM) using interpolator : "<<sum<<endl;

sum=0;
for (unsigned i=0; i< ftheta_cm_theo_inv.size()-1;i++)
{
double theta_1=ftheta_cm_theo_inv.at(i);
double theta_2=ftheta_cm_theo_inv.at(i+1);
double sigma_1=fsigma_cm_theo.at(i);
double sigma_2=fsigma_cm_theo.at(i+1);
sum=sum+(fabs(theta_2-theta_1)*degree)*sin((theta_2+theta_1)*0.5*degree)*(sigma_1+sigma_2)*0.5;
}
sum=sum*2*pi;
cout<<" THEORY CROSS SECTION INTEGRAL (CM / THETA INVERSED) : "<<sum<<endl;

// sum=0;
// Interpol_sigma = new Interpolator( ftheta_cm_theo_inv , fsigma_cm_theo  );
// sum= Interpol_sigma->Integ(ftheta_cm_theo_inv.back(), ftheta_cm_theo_inv.front());
// sum=sum*2*pi;
// cout<<" THEORY CROSS SECTION INTEGRAL  (CM / THETA INVERSED) using interpolator : "<<sum<<endl;

sum=0;
for (unsigned i=0; i< ftheta_lab_theo.size()-1;i++)
{
double theta_1=ftheta_lab_theo.at(i);
double theta_2=ftheta_lab_theo.at(i+1);
double sigma_1=fsigma_lab_theo.at(i);
double sigma_2=fsigma_lab_theo.at(i+1);
sum=sum+(fabs(theta_2-theta_1)*degree)*sin((theta_2+theta_1)*0.5*degree)*(sigma_1+sigma_2)*0.5;
}
sum=sum*2*pi;
cout<<" THEORY CROSS SECTION INTEGRAL (LAB) : "<<sum<<endl;

// sum=0;
// Interpol_sigma = new Interpolator( ftheta_lab_theo , fsigma_lab_theo  );
// sum= Interpol_sigma->Integ(ftheta_lab_theo.back(),ftheta_lab_theo.front());
// sum=sum*2*pi;
// cout<<" THEORY CROSS SECTION INTEGRAL  (LAB) using interpolator : "<<sum<<endl;

sum=0;
for (unsigned i=0; i< ftheta_lab_theo_inv.size()-1;i++)
{
double theta_1=ftheta_lab_theo_inv.at(i);
double theta_2=ftheta_lab_theo_inv.at(i+1);
double sigma_1=fsigma_lab_theo.at(i);
double sigma_2=fsigma_lab_theo.at(i+1);
sum=sum+(fabs(theta_2-theta_1)*degree)*sin((theta_2+theta_1)*0.5*degree)*(sigma_1+sigma_2)*0.5;
}
sum=sum*2*pi;
cout<<" THEORY CROSS SECTION INTEGRAL (LAB / THETA INVERSED) : "<<sum<<endl;

// sum=0;
// Interpol_sigma = new Interpolator( ftheta_lab_theo_inv , fsigma_lab_theo  );
// sum= Interpol_sigma->Integ(ftheta_lab_theo_inv.back(),ftheta_lab_theo_inv.front());
// sum=sum*2*pi;
// cout<<" THEORY CROSS SECTION INTEGRAL  (LAB / THETA INVERSED)  using interpolator : "<<sum<<endl;


}

void theo_tool::ApplyFactor(double SF)
{

cout << "Inside kine_tool::ApplyFactor()" << endl;

for (unsigned i=0; i<fsigma_cm_theo.size();i++)
{
//cout<<" before "<<fsigma_cm_theo.at(i)<<endl;
fsigma_cm_theo.at(i) =( fsigma_cm_theo.at(i) )*SF;
//cout<<" after "<<fsigma_cm_theo.at(i)<<endl;
}

}


void theo_tool::WriteDown(void)
{
cout << "Inside kine_tool::ConvertAngle()" << endl;


int tab[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
 fDrawer->SetAttributes("sigma_cm","sigma","theta_cm_theo","sigma_cm_theo",  tab );
 TGraph* gr_sigma_theo_cm_theta_cm_theo = fDrawer->ConvertToGraph(ftheta_cm_theo,fsigma_cm_theo);
fOutputFile->cd();
gr_sigma_theo_cm_theta_cm_theo->Write();

int tab1[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
 fDrawer->SetAttributes("sigma cm inv","sigma cm inv","theta_cm_theo_inv","sigma_cm_theo",  tab1 );
 TGraph* gr_sigma_theo_cm_theta_cm_theo_inv = fDrawer->ConvertToGraph(ftheta_cm_theo_inv,fsigma_cm_theo);
fOutputFile->cd();
gr_sigma_theo_cm_theta_cm_theo_inv->Write();

int tab2[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("sigma lab","sigma lab","theta_lab_theo","sigma_lab_theo",  tab2 );
TGraph* gr_sigma_theo_lab_theta_lab_theo = fDrawer->ConvertToGraph(ftheta_lab_theo,fsigma_lab_theo);
fOutputFile->cd();
gr_sigma_theo_lab_theta_lab_theo->Write();

int tab3[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("sigma lab inv","sigma lab inv","theta_lab_theo_inv","sigma_lab_theo",  tab3 );
TGraph* gr_cs_theo_lab_theta_lab_theo_inv = fDrawer->ConvertToGraph(ftheta_lab_theo_inv,fsigma_lab_theo);
fOutputFile->cd();
gr_cs_theo_lab_theta_lab_theo_inv->Write();

int tab4[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("theta_lab","theta_lab","theta_cm_theo","theta_lab_theo",  tab4 );
TGraph* gr_theta_theo_lab_theta_theo_cm = fDrawer->ConvertToGraph(ftheta_cm_theo, ftheta_lab_theo);
fOutputFile->cd();
gr_theta_theo_lab_theta_theo_cm->Write();

int tab5[6]={/*lcolor*/4,  /*lstyle*/1,  /*lwidth*/2,  /*mcolor*/3,  /*mstyle*/1,  /*msize*/1};
fDrawer->SetAttributes("sigma vs sigma","sigma vs sigma","sigma_cm_theo","sigma_lab_theo",  tab5 );
TGraph* gr_sigma_theo_lab_sigma_theo_cm = fDrawer->ConvertToGraph(fsigma_cm_theo, fsigma_lab_theo);
fOutputFile->cd();
gr_sigma_theo_lab_sigma_theo_cm->Write();

}


