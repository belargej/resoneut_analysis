#ifndef __RNBaseClass_CXX
#define __RNBaseClass_CXX

#include "RN_BaseClass.hpp"


ClassImp(RN_BaseClass);
 

RN_BaseClass::RN_BaseClass(const TString&name, const TString&title,const unsigned int& id):TNamed(name,title)	 
{
  SetUniqueID(id);
}


RN_BaseClass_Stack::RN_BaseClass_Stack(const TString& name):RN_BaseClass(name,name){

}

void RN_BaseClass_Stack::Init(){
  fRNStack=new TList();
}


void RN_BaseClass_Stack::Reset(){
  //loop over all modules stored in TList of Modules and reset all of them
  TIter next(fRNStack);
  while( RN_BaseClass*obj = (RN_BaseClass*)next()){
    obj->Reset();
  }
  return;
}


UInt_t RN_BaseClass_Stack::Add(RN_BaseClass *base){
  if(!fRNStack){
    Init();
  }
  fRNStack->Add(base);
  return 1;
}

void RN_BaseClass_Stack::Print(){
  TIter next(fRNStack);
  while( RN_BaseClass*obj = (RN_BaseClass*)next()){
    obj->Print();
  }
  return ;
}





#endif
