/***********************************************************/
// Sean Kuvin 2013
// RNVariableMap class
//
// This is the default parameter map that can be added to 
// any number of times depending on the implementation.  
// (i.e. load a silicon calibration file and the silicon will
// be automatically calibrated by the std::strings that that particular
// detector knows how to get such as "si_a.elin").  If no parameter
// is at some point loaded into the parameter map then the default values
// should be used.
/***********************************************************/

//I need to modify this code to use templates so that the 
//std::map, maps entries of any type, instead of doubles



#ifndef VARIABLE_MAP
#define VARIABLE_MAP
//C and C++ libraries.
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>



class RN_VariableMap{
protected:
  std::map<std::string,double>vmap;
  std::map<std::string,double>::iterator it;
public:
  RN_VariableMap(){}
  int GetParam(std::string,float&);
  int GetParam(std::string,double&);
  int GetParam(std::string,int&);
  void LoadParams(std::string filename);
  int AddParam(std::string,double);
  void ClearParams();

};


#endif
