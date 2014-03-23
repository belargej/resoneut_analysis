//////////////////////////////////////////////////////////////
///Base Class for handling all RN_Analysis classes
/// primarily, to identify the name and type of object:
/// ie) "neut0" with type that identifies it as a neutron detector.
/// Author: Sean Kuvin
//////////////////////////////////////////////////////////////

#ifndef __RNBASECLASS_H
#define __RNBASECLASS_H
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

//ROOT libraties
#include <TString.h>
#include <TObject.h>
#include <TNamed.h>



class RN_BaseClass:public TNamed{
protected:
public:
  RN_BaseClass(){}
  RN_BaseClass(const TString&name, const TString&title="",const unsigned int& id=0);
  virtual void Bind();
  virtual void Execute();

  ClassDef(RN_BaseClass,1);  
};




#endif
