#ifndef _HISTTOOL_CLASS
#define _HISTTOOL_CLASS

#include "kine_tool.h"
#include "Defines.h"

class hist_tool
{
 public:

//Champ statiques
string fpath;
string fsystem;
string fpath_h_norm;
string fpath_h_2b_normalized;


TH1F *fHistEff; // efficacité
TH1F *fHist; // data 
TH1F *fHistNormal; // data normalisé
TH1F *fHistSolidAngle; // angle solide 

TFile *fOutputFile;

//Methodes
hist_tool(string path );
~hist_tool(void);

void Show(void);
void Treat(void);
void GetInput(string,string);
TH1F* Synchronize(void);
TH1F* GetSolidAngle(void);

pair <double , double>  GetNormValue(double, double ) ;
pair <double , double>  GetSumValue(double, double ) ;
pair <double , double>  GetSumNormValue(double, double ) ;

vector <double> GetExpVector(vector <double>);

 };

#endif

