//////////////////////////////////////////////////////////////
/// RN_Module for handling the modules needed for the unpacker.
///
///
/// Author: Sean Kuvin
//////////////////////////////////////////////////////////////
#ifndef _RN_MODULE_CXX_
#define _RN_MODULE_CXX_

#include "RN_Module.hpp"



RN_Module::RN_Module(const TString& name,const UInt_t& geoaddress):RN_BaseClass(name,name),
								  fNumOfCh(MODULE_MAX_CHANNELS),
								  fZeroSuppression(1),
								  fValid(0),
								  fModuleType(0),
								  fGeoAddress(geoaddress)
{
  
  
  
}

UInt_t RN_Module::Reset(){
  //Reset all values stored in module channels
  fValid = 0;
  for(unsigned int i=0;i<NumOfCh();i++){
    fChValue[i] = 0;
  }
  
  return 0;
}

UInt_t RN_Module::SortChVal(const UInt_t& ch,const UInt_t & val){
  fChValue[ch]=(Float_t)val+gRandom->Rndm();
  return 1;
}


UInt_t RN_Module::AddBranch(TTree* _tree){
  _tree->Branch(GetName(),fChValue,Form("%s[%d]/F",GetName(),NumOfCh()));
  return 0;
}

UInt_t RN_Module::SetBranch(TTree* _tree){
  if(_tree->GetBranch(GetName())){   
    _tree->SetBranchAddress(GetName(),&fChValue, &b_module);  
  }
  else{
    std::cout<<"no "<<GetName()<<" present in Tree, Are you sure this rootfile is compatible with this stack?"<<std::endl;
    return 0;
  }

  return 1;
}


RN_Module_Stack::RN_Module_Stack(const TString& name):RN_BaseClass(name,name),						     
						      fModIdx(-1),
						      fCurrentModule(0)
{
  
}

void RN_Module_Stack::Init(){
  fRNModules=new TList();
}


UInt_t RN_Module_Stack::Reset(){
  //loop over all modules stored in TList of Modules and reset all of them
  fModIdx = -1;
  TIter next(fRNModules);
  while( RN_Module*obj = (RN_Module*)next()){
    obj->Reset();
  }
  return 1;
}

UInt_t RN_Module_Stack::AddModule(RN_Module *mod){
  if(!fRNModules){
    Init();
  }
  fRNModules->Add(mod);
  return 1;
}


UInt_t RN_Module_Stack::NextModule(const UShort_t&geoaddress){
  //go to next module and check to make sure the geoaddress matches.
  fCurrentModule = (RN_Module*)fRNModules->At(++fModIdx);
  if(geoaddress != fCurrentModule->GeoAddress()){
    return 0;
    //if false this means that something is wrong with the stack configuration
    //it should match daqconfig.tcl
   
  }
  return 1;
}

UInt_t RN_Module_Stack::AddBranches(TTree *_tree){
  TIter next(fRNModules);
  while( RN_Module*obj = (RN_Module*)next()){
    obj->AddBranch(_tree);
  }
  return 1;
}

UInt_t RN_Module_Stack::SetBranches(TTree *_tree){
  TIter next(fRNModules);
  while( RN_Module*obj = (RN_Module*)next()){
    obj->SetBranch(_tree);
  }
  return 1;
}

UInt_t RN_Module_Stack::SortGeoChVal(const UShort_t& geoaddress,const UInt_t& ch, const UInt_t& val){
  TIter next(fRNModules);
  while( RN_Module*obj = (RN_Module*)next()){
    if(obj->GeoAddress() == geoaddress){
      obj->SortChVal(ch,val);
      return 1;
    } 
  }
  return 0;
}


UInt_t RN_Module_Stack::SortChVal(const UInt_t& ch, const UInt_t& val){
  if(ch < fCurrentModule->NumOfCh()){
    fCurrentModule->SortChVal(ch,val);
    return 1; 
  }
  return 0;
  
}


CAEN_ADC::CAEN_ADC(const TString& name,const UInt_t& geoaddress):RN_Module(name,geoaddress){
  fModuleType = 1;  //CAEN
}
MESY_QDC::MESY_QDC(const TString& name,const UInt_t& geoaddress):RN_Module(name,geoaddress){
  fModuleType = 2;  //MESY
}


#endif
