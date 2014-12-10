#ifndef _THEOTOOL_CLASS
#define _THEOTOOL_CLASS

#include "Defines.h"
#include "draw_tool.h"

class theo_tool
{
 public:

//Champ statiques
bool   fMixed; // in case it's mixed 
string fpath;
string fcode;
string fsystem;

vector <double> ftheta_cm_theo;
vector <double> ftheta_cm_theo_inv;
vector <double> fsigma_cm_theo;

vector <double> ftheta_lab_theo;
vector <double> ftheta_lab_theo_inv;
vector <double> fsigma_lab_theo;

TFile *fOutputFile;
draw_tool* fDrawer;

//Methodes
theo_tool(string,string,string, bool);
~theo_tool(void);

void Show(void);
double GetSigmaTheo(double); // return theo_cs for theta_cm input
void GetInput(string);
void ShowIntegral(void);
void WriteDown(void);
void ApplyFactor(double SF);

void Mix(string path1, string path2, double MixFactor);

 };

#endif
