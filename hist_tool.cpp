#include "hist_tool.h"

hist_tool::~hist_tool(void)
{
cout << "Inside hist_tool::Destructor()" << endl;

}

hist_tool::hist_tool(string path)
{
cout << "Inside hist_tool::hist_tool()" << endl;

fpath = path;

fOutputFile= new TFile("./output/hist_tool_output.root","RECREATE");

//Show();

}

void hist_tool::Treat(void)
{
cout << "Inside hist_tool::Treat()" << endl;


}

void hist_tool::Show(void)
{
cout << "Inside hist_tool::Show()" << endl;
//temporaire pour verifier
// TFile* output_file= new TFile("./output/test_new.root","RECREATE");
// output_file->cd();

//can->Write();
//can1->Write();
//can2->Write();
// fHist->Write();
// fHistEff->Write();
// fHistNormal->Write();
// 
// output_file->Write();

}

void hist_tool::GetInput(string path_h_2b_normalized,string path_h_norm)
{
cout << "Inside hist_tool::GetInput()" << endl;

//================  Get Data ====================
TFile* f=new TFile(path_h_2b_normalized.c_str());
f->cd(); f->ls();
TCanvas* can1 = (TCanvas*) f->Get("c1") ;

fHist = new TH1F();
TH1F *h1 = (TH1F*)  can1->GetListOfPrimitives()->FindObject("htemp") ;
fHist = (TH1F*)h1->Clone("fHist");
//fHist->Rebin(4);

TFile* f2=new TFile(path_h_norm.c_str());
f2->cd(); f2->ls();
TCanvas* can2 = (TCanvas*) f2->Get("c1") ;

fHistEff = new TH1F();
TH1F *h2 = (TH1F*)can2->GetListOfPrimitives()->FindObject("hist_eff_nor") ;
fHistEff = (TH1F*)h2->Clone("fHistEff");
//fHistEff->Rebin();

fHistSolidAngle = new TH1F();

}

TH1F*  hist_tool::Synchronize(void)
{
cout << "Inside hist_tool::Synchronize()" << endl;

fOutputFile->cd();

Int_t Nbin_h_norm=0;
Nbin_h_norm=fHistEff->GetNbinsX();
TAxis *axis_norm = fHistEff->GetXaxis();
Double_t bw_h_norm = axis_norm->GetBinWidth(1);
 cout<<" BinWidth in Hist_Normal : " <<bw_h_norm<<endl;
 cout<<" Number of Bins in Hist_Normal : " <<Nbin_h_norm<<endl;

fHist->Write();
fHistEff->Write();

TAxis *axis = fHist->GetXaxis();
Double_t bw_h = axis->GetBinWidth(1);
Double_t xmin = axis->GetBinLowEdge(1);
Double_t xmax = axis->GetBinLowEdge(fHist->GetNbinsX())+bw_h;
Int_t Nbin_h=fHist->GetNbinsX();
int bin_number = (int) (xmax-xmin)/bw_h;

//clean h from small stat bin to avoid bin explosion!
// for (Int_t i=1;i<=fHist->GetNbinsX();i++) // loop on h_norm
// {
// 
// Int_t n=fHist->GetBinContent(i);
// if (n<5) fHist->SetBinContent(i,0);
// 
// }

TH1F* h_norm_scaled = new TH1F("h_norm_scaled","h_norm_scaled",Nbin_h,xmin, xmax);
TAxis *axis_norm_scaled = h_norm_scaled->GetXaxis();
Double_t bw_h_norm_scaled = axis_norm_scaled->GetBinWidth(1);
// cout<<" bw_h_norm_scaled : " <<bw_h_norm_scaled<<endl;
// cout<<" bw_h/bw_h_norm_scaled : " <<bw_h/bw_h_norm_scaled<<endl;

Double_t n,x,factor=1;
Int_t j=0; //compteur sur le h_norm_scaled

for (Int_t i=1;i<=Nbin_h_norm;i++) // loop on h_norm
{

x = fHistEff->GetBinLowEdge(i);
 // 	first bin               middle bins           last bin
if( (x<xmin && x+bw_h_norm>xmin) || (x>xmin && x<xmax) || (x<xmax && x+bw_h_norm>=xmax) )
// begin to fill when conditions are good
	{
	n=fHistEff->GetBinContent(i);// le contenu du bin ds le h_norm
	j=fHist->FindBin(x); // le bin correspondant dans le histo à normaliser

	Double_t XMIN_ = axis_norm->GetBinLowEdge(i);
	Double_t XMAX_ = axis_norm->GetBinLowEdge(i)+bw_h_norm;
	Double_t xmax_ = axis->GetBinLowEdge(j)+bw_h;
	
	if(xmax_<XMAX_ && xmax_>XMIN_)
	{
	factor=(xmax_-XMIN_)/(XMAX_-XMIN_);
	}
	else factor=1 ;

	h_norm_scaled->SetBinContent( j, (factor)*n + h_norm_scaled->GetBinContent(j) ) ;
	h_norm_scaled->SetBinContent( j+1, (1.0-factor)*n + h_norm_scaled->GetBinContent(j+1) ) ;
	}
}

axis_norm->SetRangeUser(xmin,xmax);
Double_t max_value_in_range = fHistEff->GetMaximum(1);
// cout<<" max_value_in_range : " <<max_value_in_range<<endl;
h_norm_scaled->Scale(  max_value_in_range/h_norm_scaled->GetBinContent(  h_norm_scaled->GetMaximumBin()  ) , "");

//clean h from small stat bin to avoid bin explosion!
for (Int_t i=1;i<=h_norm_scaled->GetNbinsX();i++) // loop on h_norm
{
Double_t n=h_norm_scaled->GetBinContent(i);
if (n<0.1*fHistEff->GetMaximum()) h_norm_scaled->SetBinContent(i,0);
}

axis_norm->UnZoom();

h_norm_scaled->SetLineColor(3);
h_norm_scaled->SetLineStyle(1);
h_norm_scaled->SetLineWidth(2);
h_norm_scaled->Write();

fHistSolidAngle = (TH1F*) h_norm_scaled->Clone("fHistSolidAngle");

return h_norm_scaled ;
}

pair <double , double>  hist_tool::GetNormValue(double inf , double sup )
{
//cout << "Inside hist_tool::GetNormVector" << endl;
cout<<" ***********************************************************************************************" <<endl;
cout<<" ***********************************************************************************************" <<endl;
double y_mean_err=0 ;
double y_mean=0 ;
double y_min=0 ;
double y_max=0 ;

int bin_min=fHistEff->FindBin(inf);
int bin_max=fHistEff->FindBin(sup);

//  cout<<" bin_min : " <<bin_min<<endl;
//  cout<<" bin_max : " <<bin_max<<endl;
//  cout<<" fabs( bin_min-bin_max ) : " <<fabs( bin_min-bin_max )<<endl;

y_mean = fHistEff->IntegralAndError(bin_min,bin_max,y_mean_err,"");

//  cout<<" sum : " <<y_mean<<endl;
//  cout<<" sum_err : " <<y_mean_err<<endl;

y_mean = y_mean/(fabs( bin_min-bin_max )+1);

//old changed on 21 Jan 2012
//y_mean_err=y_mean_err/(fabs( bin_min-bin_max )+1);

//calculate Variance on Y for error
Double_t variance = 0;
Int_t a = 0;
for (Int_t binx = bin_min ; binx <= bin_max ; ++binx)
	{
	Int_t bin  = fHistEff->GetBin(binx);
	variance += (fHistEff->GetBinContent(bin) - y_mean) * (fHistEff->GetBinContent(bin) - y_mean) ;
	a++;
         }
variance = variance/a;
y_mean_err =  sqrt(variance);

pair <double,double> value = make_pair (y_mean , y_mean_err);

return value ;

}

pair <double , double>  hist_tool::GetSumValue(double inf , double sup )
{
//cout << "Inside hist_tool::GetNormVector" << endl;

// TFile* output_file= new TFile("./test_new.root","RECREATE");
// output_file->cd();
// fHist->Write();
// fHistEff->Write();
// fHistNormal->Write();
// output_file->Write();

fOutputFile->cd();
fHist->Write();

double y_sum_err=0 ;
double y_sum=0 ;

int bin_min=fHist->FindBin(inf);
int bin_max=fHist->FindBin(sup);

y_sum = fHist->IntegralAndError(bin_min,bin_max,y_sum_err,"");

//  cout<<" bin_min : " <<bin_min<<endl;
//  cout<<" bin_max : " <<bin_max<<endl;
//  cout<<" sum : " <<y_sum<<endl;
//  cout<<" sum_err : " <<y_sum_err<<endl;

pair <double,double> value = make_pair (y_sum , y_sum_err);

return value ;

}

pair <double , double>  hist_tool::GetSumNormValue(double inf , double sup )
{
//cout << "Inside hist_tool::GetNormVector" << endl;

double y_sum_err=0 ;
double y_sum=0 ;

int bin_min=fHistNormal->FindBin(inf);
int bin_max=fHistNormal->FindBin(sup);

y_sum = fHistNormal->IntegralAndError(bin_min,bin_max,y_sum_err,"");

//  cout<<" bin_min : " <<bin_min<<endl;
//  cout<<" bin_max : " <<bin_max<<endl;
//  cout<<" sum : " <<y_sum<<endl;
//  cout<<" sum_err : " <<y_sum_err<<endl;

pair <double,double> value = make_pair (y_sum , y_sum_err);

return value ;

}



TH1F* hist_tool::GetSolidAngle(void)
{
cout << "Inside hist_tool::Normalize()" << endl;

fOutputFile->cd();

Int_t Nbin_h_synchro=0;
Nbin_h_synchro=fHistSolidAngle->GetNbinsX();
TAxis *axis_synchro = fHistSolidAngle->GetXaxis();
Double_t bw_h_synchro = axis_synchro->GetBinWidth(1);
 cout<<" BinWidth in Hist_Normal : " <<bw_h_synchro<<endl;
 cout<<" Number of Bins in Hist_Normal : " <<Nbin_h_synchro<<endl;

Double_t width,center,content, new_content;
Int_t j=0; //compteur sur le h_synchro

for (Int_t i=1;i<=Nbin_h_synchro;i++) // loop on h_norm
{
content = fHistSolidAngle->GetBinContent(i) ;
width = fHistSolidAngle->GetBinWidth(i) * degree;
center = fHistSolidAngle->GetBinCenter(i) * degree ;
new_content =  content * (2*pi) * sin(center) * width;  // faux à changer !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// cout<<" bin : "<<i<<endl;
// cout<< content <<"  "<< width/degree <<"  "<< center/degree <<"  "<< new_content <<endl;
// cout<< 2*pi <<"  "<< width <<" "<<sin(center) <<endl<<" ------------------------- "<<endl;

fHistSolidAngle->SetBinContent(i, new_content ) ;
}


fHistSolidAngle->SetLineColor(3);
fHistSolidAngle->SetLineStyle(1);
fHistSolidAngle->SetLineWidth(2);
fHistSolidAngle->Write();

//Normalize
fHistNormal = (TH1F*)fHist->Clone("fHistNormal");
fHistNormal->Divide(fHistSolidAngle);

fHistNormal->SetLineColor(2);
fHistNormal->SetLineStyle(1);
fHistNormal->SetLineWidth(3);
fHistNormal->Write();

return fHistNormal;
}


vector <double>  hist_tool::GetExpVector(vector <double> theta_lab_node)
{
// cout << "Inside hist_tool::GetExpVector()" << endl;
// 
// 	TAxis * axis=fHistNormal->GetXaxis();
// 	unsigned size = theta_lab_node.size();
// 	int range_nb = 0;
// 
// 	for ( unsigned i =0 ; i < size - 1; i++)
// 	{
// 	//cout<< " INSIDE LOOP ON NODES "<<i<<endl;
// 	double theta_min=ftheta_lab_node.at(i);
// 	double theta_max=ftheta_lab_node.at(i+1);
// 	
// 	if(	theta_min==-1    || 	theta_max==-1 ) continue;
//      	range_nb++;
//      
// 	int bin_min,bin_max;
// 
// 	bin_min=fHistNormal->FindBin(theta_min);//chercher le bin correspendant aux limites
// 	bin_max=fHistNormal->FindBin(theta_max);//chercher le bin correspendant aux limites
// 
// 	double sum=0;
// 
// 	cout<< " theta_min "<<theta_min<<"\t";
// 	cout<< " theta_max "<<theta_max<<endl;
// 	//cout<< " bin_min "<<bin_min<<endl;
// 	//cout<< " bin_max "<<bin_max<<endl;
// 			
// 	for (int bin =bin_min; bin<=bin_max;bin++)
// 		{
// 
// 		double W = axis->GetBinWidth(bin);
// 		double XMIN = axis->GetBinLowEdge(bin);
// 		double XMAX = axis->GetBinLowEdge(bin)+W;
// 		double factor=0;
// 
// 		//cout<< " W "<<W<<endl;
// 		//cout<< " XMIN "<<XMIN<<endl;
// 		//cout<< " XMAX "<<XMAX<<endl;
// 
// 		if( theta_min>XMIN && theta_min<XMAX ) // if we inside the first bin
// 		{
// 		factor=(XMAX- theta_min)/W;
// 		}
// 		else
// 		if(theta_max>XMIN && theta_max<XMAX)  // if we inside the last bin
// 		{
// 		factor=(theta_max - XMIN)/W;
// 		}
// 		else factor=1 ;
// 
// 		//cout<< " factor "<<factor<<endl;
// 		//cout<< " fHistNormal->GetBinContent( bin ) "<<fHistNormal->GetBinContent( bin )<<endl;
// 		sum=sum+factor*fHistNormal->GetBinContent( bin );
// 		//cout<< " sum after "<<sum<<endl;
// 		}// end of for on one range
// 			
// 	//cout<<" range_nb (in progress) "<<range_nb<<endl;
// 	
// 	fsigma_lab_exp.push_back(sum);
// 
// 	}// end of for on nodes

return theta_lab_node; // à modifier
}//end

// TFile* output_file= new TFile("./test_new.root","RECREATE");
// output_file->cd();
// fHist->Write();
// fHistEff->Write();
// fHistNormal->Write();
// output_file->Write();

