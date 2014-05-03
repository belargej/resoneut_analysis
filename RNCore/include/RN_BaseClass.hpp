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
#include <TTree.h>
#include <TList.h>

class RN_BaseClass:public TNamed{
protected:
public:
  RN_BaseClass(){}
  RN_BaseClass(const TString&name, const TString&title="",const unsigned int& id=0);
  virtual void Bind(){};
  virtual void Execute(){};
  virtual void Print(){};
  virtual void Reset(){};

  ClassDef(RN_BaseClass,1);  
};

class RN_BaseClass_Stack:public RN_BaseClass{
protected:
  TList *fRNStack;//!  
public: 
 
  RN_BaseClass_Stack(const TString& name="");
  ~RN_BaseClass_Stack(){
    if(fRNStack){
      ClearStack();
      delete fRNStack;
      fRNStack=NULL;
    }
  }
  virtual void Init();
  virtual UInt_t GetSize()const{return fRNStack ? fRNStack->GetSize() : 0;}
  virtual UInt_t GetNum() const{return GetSize();}
  virtual UInt_t AddBranches(TTree* _tree){return 0;}//
  virtual UInt_t SetBranches(TTree* _tree){return 0;}//
  virtual UInt_t Add(RN_BaseClass * base);
  virtual void ClearStack(){if(fRNStack)fRNStack->Clear();} 
  virtual void Print();
  virtual void Reset();
  

  ClassDef(RN_BaseClass_Stack,1);
};




#endif
