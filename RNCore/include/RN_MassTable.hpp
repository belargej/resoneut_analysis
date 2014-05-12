/**********************************************************************
Sean Kuvin 2013

Implementation of RN_VariableMap which stores Mass values.
Provides a simple look up table for the reaction info and simulator so that
the details for reaction can be provided using the particle names.
***********************************************************************/
#ifndef _Mass_MAP
#define _Mass_MAP
//C and C++ libraries.
#include <string>

#include "RN_VariableMap.hpp"


class RN_MassTable:public RN_VariableMap{
private:
  
public:
  RN_MassTable(const std::string & name = "");
  virtual ~RN_MassTable(){};

  ClassDef(RN_MassTable,1);
};


#endif
