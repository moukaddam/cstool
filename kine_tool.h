#ifndef _KINETOOL_CLASS
#define _KINETOOL_CLASS

#include "Defines.h"
#include "draw_tool.h"

class kine_tool
{
 public:

//Champ statiques
string fpath;

vector<double> fk_ratio_lab; 
vector<double> fT3_lab;
vector<double> ftheta_cm_inv;  //  theta_cm sequence not related to experimental or theoretical values
vector<double> ftheta_cm;  //  theta_cm sequence not related to experimental or theoretical values
vector<double> ftheta_lab;  //  theta_lab sequence not related to experimental or theoretical values
vector<double> fjacob_lab_cm;  // f(theta_lab) : Convert experimental cross section from lab to cm
vector<double> fjacob_cm_lab; // f(theta_cm) :Convert theoretical cross section from cm to lab

int  fReaction;
int  fAtomMass[4];
int  fAtomCharge[4];
double fMassExcess[4];
double fMass[4];
double fElectronMass;
double fAmu;
double fBeamEnergy;
double fQ;
double fEx4;

TFile *fOutputFile;
draw_tool* fDrawer;

//Methodes
kine_tool(string path);
~kine_tool(void);


void GetInput(void);
void SetKinematics(void);

vector<double> ConvertAngleVector(string, vector<double>);
double ConvertAngle(string, double);
vector<double> ConvertVector(string,vector<double> );

void Show(void);
void WriteDown(void);
 };

#endif
