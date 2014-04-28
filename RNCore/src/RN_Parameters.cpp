//////////////////////////////////////////////////////////////
/// RN_Parameters.
/// RN_Parameter_Stack consists of a TList of RN_Parameters,
/// Author: Sean Kuvin
//////////////////////////////////////////////////////////////
#ifndef _RN_Parameter_CXX_
#define _RN_Parameter_CXX_

#include "RN_Parameters.hpp"



RN_Parameter::RN_Parameter(const TString& name, const int &bins,const Double32_t& min, const Double32_t& max):RN_BaseClass(name,name),
													      fBins(bins),
													      fMin(min),
													      fMax(max),
													      fValue(0)
{
  
  
  
}

UInt_t RN_Parameter::Reset(){
  //Reset all values stored in module channels
  fValue = 0;
  
  return 0;
}

void RN_Parameter::Print(){
  std::cout<<GetName()<<": "<< fValue <<"\n";
  return ;
}


UInt_t RN_Parameter::AddBranch(TTree* _tree){
  _tree->Branch(GetName(),&fValue,Form("%s/F",GetName()));
  return 0;
}

UInt_t RN_Parameter::SetBranch(TTree* _tree){
  if(_tree->GetBranch(GetName())){   
    _tree->SetBranchAddress(GetName(),&fValue, &b_param);  
  }
  else{
    std::cout<<"no "<<GetName()<<" present in Tree, Are you sure this rootfile has this variable name"<<std::endl;
    return 0;
  }

  return 1;
}

Bool_t RN_Parameter::operator == (const RN_Parameter& param) const {
  return (param.fValue == fValue) ? 1 : 0;
}


Bool_t RN_Parameter::operator != (const RN_Parameter& param) const {
  return (param.fValue != fValue) ? 1 : 0;
}

Bool_t RN_Parameter::operator == (const Double32_t& val) const {
  return (val == fValue) ? 1 : 0;
}


Bool_t RN_Parameter::operator != (const Double32_t& val) const {
  return (val != fValue) ? 1 : 0;
}


Double32_t RN_Parameter::operator = (const Double32_t& val) {
  fValue = val;
  return fValue;
}

Double32_t RN_Parameter::operator = (const RN_Parameter& param) {
  fValue = param.fValue;
  return fValue;
}

Double32_t RN_Parameter::operator  += (const Double32_t &val){
  fValue += val;
  return fValue;
}

Double32_t RN_Parameter::operator  -= (const Double32_t &val){
  fValue -= val;
  return fValue;
}

Double32_t RN_Parameter::operator  /= (const Double32_t &val){
  if(val>0)
    fValue /= val;
  return fValue;
}

Double32_t RN_Parameter::operator  *= (const Double32_t &val){
  fValue *= val;
  return fValue;
}

Double32_t RN_Parameter::operator  += (const RN_Parameter &param){
  fValue += param.fValue;
  return fValue;
}

Double32_t RN_Parameter::operator  -= (const RN_Parameter &param){
  fValue -= param.fValue;
  return fValue;
}
  
Double32_t RN_Parameter::operator  /= (const RN_Parameter &param){
  if(param.fValue>0)
    fValue /= param.fValue;
  return fValue;
}
Double32_t RN_Parameter::operator  *= (const RN_Parameter &param){
  fValue *= param.fValue;
  return fValue;
}




/**********************************************************************
 **********************************************************************/


RN_Parameter_Stack::RN_Parameter_Stack(const TString& name):RN_BaseClass(name,name)						     
{
  
}

void RN_Parameter_Stack::Init(){
  fRNParameters=new TList();
}


UInt_t RN_Parameter_Stack::Reset(){
  //loop over all modules stored in TList of Modules and reset all of them
  TIter next(fRNParameters);
  while( RN_Parameter*obj = (RN_Parameter*)next()){
    obj->Reset();
  }
  return 1;
}


UInt_t RN_Parameter_Stack::AddParameter(RN_Parameter *param){
  if(!fRNParameters){
    Init();
  }
  fRNParameters->Add(param);
  return 1;
}

UInt_t RN_Parameter_Stack::AddBranches(TTree *_tree){
  TIter next(fRNParameters);
  while( RN_Parameter*obj = (RN_Parameter*)next()){
    obj->AddBranch(_tree);
  }
  return 1;
}

void RN_Parameter_Stack::Print(){
  TIter next(fRNParameters);
  while( RN_Parameter*obj = (RN_Parameter*)next()){
    obj->Print();
  }
  return ;
}

UInt_t RN_Parameter_Stack::SetBranches(TTree *_tree){
  TIter next(fRNParameters);
  while( RN_Parameter*obj = (RN_Parameter*)next()){
    obj->SetBranch(_tree);
  }
  return 1;
}



Double32_t operator * (const RN_Parameter& a, const RN_Parameter& b){
  return a.fValue * b.fValue;
}
Double32_t operator * (const Double32_t& a, const RN_Parameter& b){
  return a * b.fValue;
}
Double32_t operator * (const RN_Parameter& a, const Double32_t& b){
  return a.fValue * b;
}
Double32_t operator / (const RN_Parameter& a, const RN_Parameter& b){
  if(b.fValue>0)
    return a.fValue / b.fValue;
  else return 0;
}
Double32_t operator / (const Double32_t& a, const RN_Parameter& b) {
  if(b.fValue>0)
    return a / b.fValue;
  else return 0;
}
Double32_t operator / (const RN_Parameter& a, const Double32_t& b){
  if(b>0)
    return a.fValue / b;
  else return 0;
}

Double32_t operator + (const RN_Parameter& a, const RN_Parameter& b){
  return a.fValue + b.fValue;
}
Double32_t operator + (const Double32_t& a, const RN_Parameter& b){
  return a + b.fValue;
}
Double32_t operator + (const RN_Parameter& a, const Double32_t& b){
  return a.fValue +b;
}
Double32_t operator - (const RN_Parameter& a, const RN_Parameter& b){
  return a.fValue - b.fValue;

}
Double32_t operator - (const Double32_t& a, const RN_Parameter& b){
  return a - b.fValue;
}
Double32_t operator - (const RN_Parameter& a, const Double32_t& b){
  return a.fValue - b;
}



 

#endif
