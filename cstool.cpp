
#include "get_cs.h"

int main()
{
  cout << "Inside main!" << endl;

vector <string> inputfile;
vector < get_cs* > CrossSection;

//================  Get Data ====================
	ifstream inf;
	string bufstring;
	string filename="./input/main.dat";
  	inf.open(filename.c_str());
  if(!inf)
    {
      cout << "ERROR : Could not open the file :"<<filename<< endl;
      exit(-1);
    }
  else
    {
      while(inf>>bufstring)
	{
	//inf>>bufstring;
         inputfile.push_back(bufstring);
        }	// while 		 
   }//else   		 			
  inf.close();

cout<<"The following data will used to estimate cross sections"<<endl;
for(unsigned i=0 ; i<inputfile.size();i++)
cout<<"data "<<i+1<<" : "<<inputfile.at(i)<<endl;

//initiate 
CrossSection.resize(inputfile.size());
for(unsigned i=0 ; i<inputfile.size();i++)
  CrossSection.at(i) = new get_cs(inputfile.at(i));


//treat 
for(unsigned i=0 ; i<inputfile.size();i++)
  //CrossSection.at(i)->Treat1(); // 1st method
  CrossSection.at(i)->Treat2(); // 2nd method


//delete ? 
// for(unsigned i=0 ; i<inputfile.size();i++)
//    delete CrossSection.at(i) ;


   cout << "End of main" << endl;

 return 0;
}

