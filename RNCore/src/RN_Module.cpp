/***************************************************************
Class: RN_Module, RN_Module_Stack
Author: Sean A Kuvin
****************************************************************/
#ifndef _RN_MODULE_CXX_
#define _RN_MODULE_CXX_
#include "RN_Module.hpp"


ClassImp(RN_Module);
ClassImp(RN_Module_Stack);


RN_Module::RN_Module(const TString& name,const UInt_t& geoaddress):RN_BaseClass(name,name),
								  fNumOfCh(MODULE_MAX_CHANNELS),
								  fZeroSuppression(1),
								  fValid(0),
								  fModuleType(0),
								  fGeoAddress(geoaddress)
{
  
  
  
}

void RN_Module::Reset(){
  //Reset all values stored in module channels
  fValid = 0;
  for(unsigned int i=0;i<NumOfCh();i++){
    fChValue[i] = 0;
  }
  
  return;
}

UInt_t RN_Module::SortChVal(const UInt_t& ch,const UInt_t & val){
  fChValue[ch]=(Float_t)val+gRandom->Rndm();
  return 1;
}


void RN_Module::Print(){
  std::cout<<GetName()<<": "<<"GeoAddress: "<<GeoAddress()<<"\n";
  return ;
}


UInt_t RN_Module::AddBranch(TTree* _tree){
  b_module = _tree->Branch(GetName(),fChValue,Form("%s[%d]/F",GetName(),NumOfCh()));
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


RN_Module_Stack::RN_Module_Stack(const TString& name):RN_BaseClass_Stack(name)						     
{
  
}



Bool_t RN_Module_Stack::UnpackModules(unsigned short *& gpointer,int filepos){
  TIter next(fRNStack);
  while( RN_Module*obj = (RN_Module*)next()){
    if(!obj->Unpack(gpointer))
      std::cout<<"error at file pos: "<< filepos<<std::endl;      
  }  
  
  return 1;
}


UInt_t RN_Module_Stack::GetNum(UInt_t modtype){
  UInt_t count=0;
  if(!fRNStack)
    return 0;
  if(modtype == 0)
    return GetSize();
  TIter next(fRNStack);
  
  //count how many modules in the stack are equal to a particular type
  while( RN_Module*obj = (RN_Module*)next()){
    if(obj->ModuleType() == modtype)
      count++;
  }
  return count;
}


UInt_t RN_Module_Stack::AddBranches(TTree *_tree){
  TIter next(fRNStack);
  while( RN_Module*obj = (RN_Module*)next()){
    obj->AddBranch(_tree);
  }
  return 1;
}

UInt_t RN_Module_Stack::SetBranches(TTree *_tree){
  TIter next(fRNStack);
  while( RN_Module*obj = (RN_Module*)next()){
    obj->SetBranch(_tree);
  }
  return 1;
}

UInt_t RN_Module_Stack::SortGeoChVal(const UShort_t& geoaddress,const UInt_t& ch, const UInt_t& val){
  TIter next(fRNStack);
  while( RN_Module*obj = (RN_Module*)next()){
    if(obj->GeoAddress() == geoaddress){
      obj->SortChVal(ch,val);
      return 1;
    } 
  }
  return 0;
}

UInt_t RN_Module_Stack::Add(RN_BaseClass *base){
  if(!base->InheritsFrom("RN_Module")){
    return 0;
  }
  if(!fRNStack){
    Init();
  }
  fRNStack->Add(base);
  return 1;
}



CAEN_ADC::CAEN_ADC(const TString& name,const UInt_t& geoaddress):RN_Module(name,geoaddress){
  fModuleType =kCAENtype;  //CAEN
}

Bool_t CAEN_ADC::Unpack(unsigned short *& gpointer){
  short dat(0);
  short chan(0);
  short ch_hits(0);
  short geoaddress(0);
  bool flag(1);
  
  //module is suppressed
  if(*gpointer==0xffff){
    gpointer=gpointer+2; //jump over ffff block
    return 1; //returning 1 cause this is NOT bad
  }
  
  ch_hits = ( *gpointer++ & 0xff00 ) >> 8; //read high byte as low byte
  geoaddress = ( *gpointer++ & 0xf800) >> 11;
  
  if(geoaddress != fGeoAddress){
    std::cout<<geoaddress<<" different from geoaddress in stack: "<< GeoAddress()<<std::endl;
    gpointer+=ch_hits*2;
    flag =  0; //THIS is bad, means stack is probably not set up right or there was an incorrectly formatted buffer(which can happen).  Check to find the cause...probably your module stack is not in the right order
  }
  else{
    for (short jj=0;jj<ch_hits;jj++){
      dat =  *gpointer++ & 0xfff;
      chan = *gpointer++ & 0x1f;
      SortChVal(chan,dat);
      
    }
  }
  fCounter= *gpointer++;
  gpointer = gpointer + 3 ; //jump 3 words to skip rest of CAEN End of Block
  return flag;

}


MESY_QDC::MESY_QDC(const TString& name,const UInt_t& geoaddress):RN_Module(name,geoaddress){
  fModuleType = kMESYtype;  //MESY
}

Bool_t MESY_QDC::Unpack(unsigned short *& gpointer){
  short dat, chan;
  short shortwords;
  short ModuleID;
  unsigned short * zpointer; zpointer = gpointer;
  int flag(1);

  if(*zpointer==0xffff){ //module is suppressed
    gpointer = gpointer + 2 ;
    return 1; //this is not bad so just return 1;
  }

  shortwords = 2 * ( *zpointer++ & 0x0fff ); 
  ModuleID = *zpointer++ & 0x00ff;
  
  if(ModuleID != fGeoAddress ){
    std::cout<<ModuleID<<" different from geoaddress in stack: "<< GeoAddress()<<std::endl;
    gpointer+=shortwords; //move to expected end of this part of buffer

    flag = 0; //THIS is bad, means stack is probably not set up right or there was an incorrectly formatted buffer(which can happen).  Check to find the cause...probably your module stack is not in the right order
  }
  else{
    while( zpointer < ( gpointer + shortwords)){
      dat = *zpointer++ & 0xfff;
      chan = *zpointer++ & 0x1f;
      
      SortChVal(chan,dat);
    }
  }
  fCounter = *zpointer++;
  zpointer = zpointer + 3; //jump over EOB + ffff
  gpointer = zpointer; //move gpointer to end of this zpointer

  return flag;
}

  

#endif
