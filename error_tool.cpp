#include "error_tool.h"

error_tool::~error_tool(void)
{
cout << "Inside error_tool::Destructor()" << endl;

}

error_tool::error_tool(string filename)
{
cout << "Inside error_tool::error_tool()" << endl;

ffilename=filename;
GetInput();
Show();

}

void error_tool::Show()
{

cout << "Inside error_tool::Show()" << endl;
cout << "=====================================================================" << endl;

cout << "ffilename " <<ffilename<< endl;

cout << "------------------------------------------------------------------- Beam " << endl;

cout << "fBeamIncident (error) (particule) : " <<fBeamIncident.first<<"("<<fBeamIncident.second<<")"<< endl;
cout << "fBeamPurity (error) : " <<fBeamPurity.first<<"("<<fBeamPurity.second<<")"<< endl;

cout << "----------------------------------------------------------------- TARGET " << endl;

cout << "fTargetThickness (error) (mg/cm2) : " <<fTargetThickness.first/(mg/cm2)<<"("<<fTargetThickness.second/(mg/cm2)<<")"<< endl;
//cout << "fTargetDensity (g/cm3) : " <<fTargetDensity/(g/cm3) << endl;
cout << "fTargetMolarMass (g/mole): " <<fTargetMolarMass/(g/mole)<< endl;
cout << "fTargetFactor : " <<fTargetFactor<< endl;

cout << "----------------------------------------------------------------- TARGET " << endl;

cout << "fNInc(error) [particule] : " <<fNInc<<"("<<fNInc_error<<")"<< endl;
cout << "fNTarget(error) [atome/cm2] : " <<fNTarget/(1/cm2)<<"("<<fNTarget_error/(1/cm2)<<")"<< endl;

cout << "=========================================================================" << endl;
}

void error_tool::GetInput()
{
cout<<"Inside error_tool::GetInput() "<<endl;

//Beam
 double BeamIncident, BeamIncident_error;
 double BeamPurity, BeamPurity_error;

// Target
 double TargetThickness, TargetThickness_error ;
 double TargetMolarMass;
 int TargetFactor;
 double FactorSpect;


//================  Get Data ====================
	ifstream inf;
	string mot;
	int  input=0;
  inf.open(ffilename.c_str(), ios::in);
//ifstream binData("Trafficlog_Data.txt", ios::in | ios::binary); 
  if(!inf)
    {
      cout << "ERROR  : Could not open the calibration file :"<<ffilename<< endl;
      exit(-1);
    }
  else
    {
      while(inf>>mot)
		{			

		 if(mot == "BEAM")
		    {
		input++;
		cout<<"LOADING BEAM : "<<endl;
		inf>>BeamIncident>>BeamPurity>>mot>>BeamPurity_error>>mot;
  		 }

 		if(mot == "FACTORSPEC")
		  {
		input++;
		cout<<"LOADING FACTOR SPECTROSCOPIQUE : "<<endl;
		inf>>FactorSpect;
		cout<<"spectroscopique FACTOR : "<<FactorSpect<<endl;
  		 }
		
		 if(mot == "TARGET")
		    {
		input++;
		cout<<"LOADING TARGET : "<<endl;
		inf>>TargetThickness>>TargetThickness_error>>mot;
		inf>>TargetMolarMass;
		inf>>TargetFactor;
  		 }

   		 }// while
   	 }//else
   		 
		if(input!=3)
		{
		cout<<"ERROR: SOMETHING WRONG IN INPUTS NUMBER IN FILE "<<ffilename<<endl;
		cout<<"NUMBER OF INPUTS FOUND IS "<<input<<endl;
		exit(-1);
		}			
  inf.close();

// operations
BeamPurity = BeamPurity*perCent;
BeamPurity_error = BeamPurity*(BeamPurity_error*perCent);
fBeamPurity = make_pair (BeamPurity,BeamPurity_error) ;

BeamIncident_error = sqrt(BeamIncident);
fBeamIncident = make_pair(BeamIncident,BeamIncident_error);

// operations
TargetThickness=TargetThickness*(mg/cm2); // mg/cm2
TargetThickness_error=TargetThickness*(TargetThickness_error*perCent); // %
TargetMolarMass=TargetMolarMass*(g/mole); // (g/mole)

fTargetThickness = make_pair (TargetThickness,TargetThickness_error);
fTargetMolarMass = TargetMolarMass ;
fTargetFactor = TargetFactor ;
fFactorSpect = FactorSpect ;

// calculate their errors and their values
fNTarget = fTargetThickness.first * Avogadro * fTargetFactor / fTargetMolarMass;
fNTarget_error = pow((fNTarget/fTargetThickness.first*fTargetThickness.second),2);
fNTarget_error = sqrt(fNTarget_error);

fNInc = fBeamIncident.first * (fBeamPurity.first);
double a2 =pow( (fNInc / fBeamIncident.first * fBeamIncident.second),2);
double b2 =pow((fNInc / fBeamPurity.first * fBeamPurity.second),2) ;
fNInc_error = sqrt(a2+b2);

}

pair <double,double>  error_tool::GetSigmaAndError(double Ndet ,double Ndet_error , double theta_min, double theta_max, double eff, double eff_error)
{
// checked

pair <double,double> cross_section_1 ;

double delta_cos = fabs(cos(theta_min) - cos(theta_max)) ; //delta_cos = 1 ; //test
double theta_error = 0.5*degree; // à determiner selon l'angle.... S1 != MUST
double delta_cos_error = ( fabs(sin(theta_min)) + fabs(sin(theta_max)) ) * theta_error ;

double value =  Ndet  /  ( (2*pi) * delta_cos * eff * fNInc * fNTarget );
double error =  pow((value/Ndet * Ndet_error ),2)  +
		pow((value/delta_cos * delta_cos_error),2) +
		pow((value/eff * eff_error),2) +
		pow((value/fNInc * fNInc_error),2)+
		pow((value/fNTarget * fNTarget_error),2) +
                 0 ;

 error = sqrt(error);

// cout<<" all errors percentage : "<<endl;
// cout<<"Ndet      : "<<pow((value/Ndet * Ndet_error ),2)/error*100<<endl;
// cout<<"delta_cos : "<<pow((value/delta_cos * delta_cos_error),2)/error*100<<endl;
// cout<<"eff       : "<<pow((value/eff * eff_error),2)/error*100<<endl;
// cout<<"fNInc     : "<<pow((value/fNInc * fNInc_error),2)/error*100<<endl;
// cout<<"fNTarget  : "<<pow((value/fNTarget * fNTarget_error),2)/error*100<<endl;

//cout<<"value/millibarn  : "<<value/millibarn <<endl;

cross_section_1 = make_pair (value/millibarn , error/millibarn);

return cross_section_1 ;

}


pair <double,double>  error_tool::GetSigmaCMfromSigmaLab(
pair <double,double>  cross_section_2 , pair <double,double>  jacob)
{
cout<<"Inside error_tool::GetSigmaCMfromSigmaLab() "<<endl;

pair <double,double> cross_section_3 ;
double y = cross_section_2.first ;
double y_error = cross_section_2.second;
double j = jacob.first ;
double j_error = jacob.second;

double value =  y * j ;
//  cout<<"y : "<<y<<endl;
//  cout<<"j : "<<j<<endl;
//  cout<<"value : "<<value<<endl;

double error = pow((value/y*y_error ),2) +
		  pow((value/j*j_error),2) +
                    0 ;

//  cout<<" all errors vs Jacobian error : "<<endl;
//   cout<<"all       : "<<pow((value/y*y_error ),2)/error*100<<endl;
//   cout<<"jacob     : "<<pow((value/j*j_error),2)/error*100<<endl;

error = sqrt(error);

//cout<<"value/j*j_error : "<<value/j*j_error<<endl;

cross_section_3 = make_pair (value, error);

return cross_section_3 ;
}

pair <double,double>  error_tool::ApplySpectroFactor( pair <double,double>  cross_section_2 , int a)
{
cout<<"Inside error_tool::SpectroFactor() "<<endl;

pair <double,double> cross_section_3 ;
double value = cross_section_2.first ;
double error = cross_section_2.second;

if (a==1 ) value = value/fFactorSpect ;

cross_section_3 = make_pair (value , error);

return cross_section_3 ;
}


