#include "draw_tool.h"

draw_tool::~draw_tool(void)
{
cout << "Inside draw_tool::Destructor()" << endl;

}

draw_tool::draw_tool(string filename)
{
cout << "Inside draw_tool::draw_tool()" << endl;

ffilename=filename;
cout << "Inside draw_tool::draw_tool()" << endl;
fOutputFile= new TFile(ffilename.c_str(),"RECREATE");
cout << "Inside draw_tool::draw_tool()" << endl;
//Show();
}

void draw_tool::Show()
{
cout << "Inside draw_tool::Show()" << endl;

cout << "ffilename " <<ffilename<< endl;

cout << "ftitle " <<ftitle<< endl;
cout << "ftitlex " <<ftitlex<< endl;
cout << "ftitley " <<ftitley<< endl;

cout << "fline_color " <<fline_color<< endl;
cout << "fline_style " <<fline_style<< endl;
cout << "fline_width " <<fline_width<< endl;
cout << "fmarker_color " <<fmarker_color<< endl;
cout << "fmarker_style " <<fmarker_style<< endl;
cout << "fmarker_size " <<fmarker_size<< endl;

}

void draw_tool::SetAttributes(string name,string title,string titlex,string titley, int* att)
{
//cout<<"Inside draw_tool::SetAttributes() "<<endl;
fname = name ;
ftitle = title;
ftitlex = titlex;
ftitley = titley;

fline_color=att[0];
fline_style=att[1];
fline_width=att[2];
fmarker_color=att[3];
fmarker_style=att[4];
fmarker_size=att[5];

}

TGraph*  draw_tool::ConvertToGraph(
vector<double> vec_x , vector<double> vec_y)
{
//check if sizes are the same
double *x;  double *y;  
int size = vec_x.size(); int size_y = vec_y.size();
	if(size!=size_y) 
	{
	cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<endl;
	cout<<"ERROR in draw_tool::ConvertToGraphAsymmErrors() : Vectors have different sizes "<<endl;
	cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<endl;
	Show();
	exit(-1);
	}
else
	{
	    x=new double[size];
	    y=new double[size];
	    
	    for(int i=0;i<size;i++)
	    {
	    x[i]=vec_x.at(i);
	    y[i]=vec_y.at(i);
	    }

	}

   TGraph *gr = new TGraph(size, x , y);
   gr->SetName(fname.c_str());
   gr->SetTitle(ftitle.c_str());
    gr->GetXaxis()->SetTitle(ftitlex.c_str());
    gr->GetYaxis()->SetTitle(ftitley.c_str());

   gr->SetLineColor(fline_color);
   gr->SetLineStyle(fline_style);
   gr->SetLineWidth(fline_width);
   gr->SetMarkerColor(fmarker_color);
   gr->SetMarkerStyle(fmarker_style);
   gr->SetMarkerSize(fmarker_size);

// fOutputFile->cd();
// gr->Write();

	return gr ;
}

TGraphAsymmErrors*  draw_tool::ConvertToGraphAsymmErrors(
vector<double> vec_x ,vector<double> vec_x_left , vector<double> vec_x_right,
vector<double> vec_y,vector<double> vec_y_errors)
{
cout<<"Inside draw_tool::ConvertToGraphAsymmErrors() "<<endl;
//check if sizes are the same
double *x;  double *y;
double *exl;  double *eyl;
double *exh;  double *eyh;

int size = vec_x.size(); int size_y = vec_y.size();
int size_x_left = vec_x_left.size(); int size_x_right = vec_x_right.size(); int size_y_errors = vec_y.size();


	if(size!=size_y) 
	{
	cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<endl;
	cout<<"ERROR in draw_tool::ConvertToGraphAsymmErrors() : Vectors have different sizes "<<endl;
	cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<endl;
	cout<<size<<endl;
	cout<<size_x_left<<endl;
	cout<<size_x_right<<endl;
	cout<<size_y<<endl;
	cout<<size_y_errors<<endl;
	Show();
	exit(-1);
	}
else
	{
	    x=new double[size];
	    y=new double[size];
	    exl=new double[size];
	    exh=new double[size];
	    eyl=new double[size];
	    eyh=new double[size];
	    for(int i=0;i<size;i++)
	    {
	    x[i]=vec_x.at(i);
	    y[i]=vec_y.at(i);
		exl[i]=vec_x.at(i)-vec_x_left.at(i);
		exh[i]=vec_x_right.at(i)-vec_x.at(i);
// 		eyl[i]=vec_y.at(i)-vec_y_left.at(i);
// 		eyh[i]=vec_y_right.at(i)-vec_y.at(i);
		eyl[i]=vec_y_errors.at(i)*0.5;
		eyh[i]=vec_y_errors.at(i)*0.5;
// cout<<"-----------------"<<endl;
// cout<<x[i]<<" : ";
// cout<<exl[i]<<" "<<exh[i]<<endl;
// cout<<y[i]<<" : ";
// cout<<eyl[i]<<" "<<eyh[i]<<endl;
// cout<<"-----------------"<<endl;
	    }
	}

  TGraphAsymmErrors* gr = new TGraphAsymmErrors(size,x,y,exl,exh,eyl,eyh);
   gr->SetName(fname.c_str());
   gr->SetTitle(ftitle.c_str());
    gr->GetXaxis()->SetTitle(ftitlex.c_str());
    gr->GetXaxis()->SetRangeUser(0,35);
    gr->GetYaxis()->SetTitle(ftitley.c_str());
    gr->GetYaxis()->SetRangeUser(0.1,100.0);

   gr->SetLineColor(fline_color);
   gr->SetLineStyle(fline_style);
   gr->SetLineWidth(fline_width);
   gr->SetMarkerColor(fmarker_color);
   gr->SetMarkerStyle(fmarker_style);
   gr->SetMarkerSize(fmarker_size);

 cout<<"fOutputFile->cd(): "<<fOutputFile->cd()<<endl;
 gr->Write();
 cout<<"written! "<<fOutputFile->cd()<<endl;
	return gr ;
}



