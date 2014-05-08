/////////////////////////////////////////////////////////////
///Sean Kuvin 2013
///Implementation of RN_VariableMap which stores Mass values.
////////////////////////////////////////////////////////////

#ifndef _Mass_MAP
#define _Mass_MAP
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
#include <TObject.h>
#include "RN_VariableMap.hpp"


class RN_MassTable:public RN_VariableMap{
private:
  
public:
  RN_MassTable();
  virtual ~RN_MassTable(){};

  ClassDef(RN_MassTable,1);
};


#endif
