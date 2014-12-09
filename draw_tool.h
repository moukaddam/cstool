#ifndef _DRAWTOOL_CLASS
#define _DRAWTOOL_CLASS

#include "Defines.h"

class draw_tool
{
 public:

//Champ statiques
TFile *fOutputFile;
string ffilename;

string fname;
string ftitle;
string ftitlex;
string ftitley;
int fline_color;
int fline_style;
int fline_width;
int fmarker_color;
int fmarker_style;
int fmarker_size;

//Methodes
draw_tool(string path);
~draw_tool(void);

void Show(void);
void SetAttributes(string,string,string,string, int* );
TGraphAsymmErrors* ConvertToGraphAsymmErrors(vector<double> vec_x ,vector<double> vec_x_left , vector<double> vec_x_right, vector<double> vec_y,vector<double> vec_y_errors);

// TGraphErrors* ConvertToGraphErrors(vector<double> vec_x , vector<double> vec_y,
// vector<double> vec_x_e,vector<double> vec_y_errors);

TGraph* ConvertToGraph(vector<double>, vector<double> );

 };

#endif
