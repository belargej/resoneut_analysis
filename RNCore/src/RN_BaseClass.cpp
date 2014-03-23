#ifndef __RNBaseClass_CXX
#define __RNBaseClass_CXX

#include "RN_BaseClass.hpp"


ClassImp(RN_BaseClass);
 

RN_BaseClass::RN_BaseClass(const TString&name, const TString&title,const unsigned int& id):TNamed(name,title)	 
{
  SetUniqueID(id);
}
void RN_BaseClass::Bind(){
  return;
}

void RN_BaseClass::Execute(){
  return;
}


#endif
