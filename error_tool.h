#ifndef _ERRORTOOL_CLASS
#define _ERRORTOOL_CLASS

#include "Defines.h"

class error_tool
{
 public:

//Champ statiques
string ffilename;

// Beam
 pair <double,double> fBeamIncident;
 pair <double,double> fBeamPurity;

// Target
 pair <double,double> fTargetThickness;
 double fTargetMolarMass; // conversion to grams..
 int fTargetFactor;

double fNTarget;
double fNTarget_error;
double fNInc;
double fNInc_error;

double fFactorSpect;

//Methodes
error_tool(string path);
~error_tool(void);

void Show(void);
void GetInput(void);
pair <double,double> GetSigmaAndError(double Ndet , double Ndet_error , double theta_min, double theta_max, double eff, double eff_err);
pair <double,double>  GetSigmaCMfromSigmaLab(pair <double,double>  cross_section_2 , pair <double,double>  jacob);
pair <double,double>  ApplySpectroFactor( pair <double,double>  cross_section_3 , int a ) ;
//vector<  pair <double,double>  > GetVectorError(vector<double> value , vector<double> error);

 };

#endif

// Quantity = make_pair (val1,val2);
//Quantity.first
//Quantity.second

