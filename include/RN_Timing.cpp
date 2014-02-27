#ifndef __RFTIME_CXX
#define __RFTIME_CXX

#include "RN_Timing.hpp"
#include <TCanvas.h>

ClassImp(RN_RFTime);

RN_RFTime::RN_RFTime(std::string name):fName(name),
				       tlin(1),
				       tshift(0),
				       fTo(0)
{ 
  
}

Double_t RN_RFTime::T() const{
  if(fT>0)
    return ((fT * tlin) + tshift);
  else
    return 0}

Double_t RN_RFTime::T_Wrapped() const{
  double time=T();
  if(fT>0 && fTo){
    time = fmod((time - fTo),82.417);
    if (time < 0)
      time+=82.417;
    time=fabs(82.417-time);  
    return time;
  }
  else return -1;
  
}

void RN_RFTime::SetCalibrations(RN_VariableMap&detvar){
  detvar.GetParam(Form("%s.tlin",Name().c_str()),tlin);
  detvar.GetParam(Form("%s.tshift",Name().c_str()),tshift);
  detvar.GetParam(Form("%s.tzero",Name().c_str()),fTo);
 

}
void RN_RFTime::InsertHit(const double&t){
  fT=t;
}

void RN_RFTime::Reset(){
  fT=0;
}

#endif
