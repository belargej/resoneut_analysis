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

#ifndef RN_VARIABLE_MAP
#define RN_VARIABLE_MAP
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

#include "RN_BaseClass.hpp"

class RN_VariableMap: public RN_BaseClass{
protected:
  std::map<std::string,double>vmap;
  std::map<std::string,double>::iterator it;
public:
  RN_VariableMap(){}
  RN_VariableMap(const std::string &name):RN_BaseClass(name,name){}
  int GetParam(std::string,float&);
  int GetParam(std::string,double&);
  int GetParam(std::string,int&);
  void LoadParams(std::string filename);
  int AddParam(std::string,double);
  void ClearParams();
  void Print();
  
  ClassDef(RN_VariableMap,1);
};


#endif
