#ifndef _GETCS_CLASS
#define _GETCS_CLASS

#include "Defines.h"

#include "draw_tool.h"
#include "hist_tool.h"
#include "kine_tool.h"
#include "theo_tool.h"
#include "error_tool.h"

class get_cs
{
 public:

//Champ statiques
draw_tool* fDrawer;
kine_tool* fKinematic;
hist_tool* fHistManager;
error_tool* fError;
vector <theo_tool*>  fTheoCs;

TH1F* fExpHist;
TH1F* fEffHist;

vector <double> ftheta_lab_node;
vector <double> ftheta_cm_node;

vector <double> ftheta_lab_inf;
vector <double> ftheta_lab_sup;

vector <double> ftheta_cm_inf;
vector <double> ftheta_cm_sup;

vector <double> ftheta_lab_exp;
vector <double> ftheta_lab_exp_err_left;
vector <double> ftheta_lab_exp_err_right;

vector <double> ftheta_cm_exp;
vector <double> ftheta_cm_exp_err_left;
vector <double> ftheta_cm_exp_err_right;

vector <double> fsigma_lab_exp;
vector <double> fsigma_lab_exp_err;

vector <double> fsigma_cm_exp;
vector <double> fsigma_cm_exp_err;

vector <double> fnorm_lab_exp;
vector <double> fnorm_lab_exp_err;

vector <double> fcounts_lab_exp;
vector <double> fcounts_lab_exp_err;

vector <double> fSpectroFactor;
vector <double> fSpChi2;

vector <double> fjacob_LabtoCM ; 
vector <double> fjacob_LabtoCM_err ;

vector <double>  fSpectroFactorEstimate ;
vector <double>  fChi2Estimate ;
vector <double>  fSpectroFactorUpper ;
vector <double>  fChi2EstimateUpper ;
vector <double>  fSpectroFactorLower ;
vector <double>  fChi2EstimateLower ;
int fFit;


string fsystem;
string fpath;
string fpath_hist;
string fpath_hist_normal;
vector <string>  fpath_theo;
vector <string>  fcode_theo;
vector <string>  fsystem_theo;
vector <int>  fcolor_theo;


TFile *fOutputFile;


//Methodes
get_cs(string path);
~get_cs(void);

void Show(void);
void Treat1(void); // 1st method
void Treat2(void); // 2nd method
void TreatNodes(void);
void GetInput(void);

void GetSumExpVectorLab(void);
void GetSumExpVectorLab_ROOT(void); // from a histo using root functions
void GetSumExpVectorLab_file(void); // get the counts from a file

void GetNormalVectorLab(void);
void GetSigmaExpVectorLab1(void);
void GetSigmaExpVectorLab2(void);

void GetSigmaExpVectorCM(void);

void TransformTheoryCrossSection(unsigned);
void GetSpectroscopicFactor(unsigned);
void ApplySpectroscopicFactor(void);

void DrawNormSumVector(void);
void DrawDeltaCosVector(void);
void DrawJacobVector(void);
void DrawChi2(void) ;

void CalculateDbkg(int D_bkg_Counts, double efficiency,  double theta_min, double theta_max );


//void AddGraphs(vector < TGraph* > GraphVector);
void AddGraphs(void);

 };

#endif
