#ifndef __RFTIME_CXX
#define __RFTIME_CXX

#include "RN_Timing.hpp"
#include <TCanvas.h>

ClassImp(RN_RFTime);

RN_RFTime::RN_RFTime(std::string name):RN_BaseClass(name,name),
				       fTLin(1),
				       fTShift(0),
				       fTo(0)
{ 
  
}


void RN_RFTime::SetCalibrations(RN_VariableMap&detvar){
  detvar.GetParam(Form("%s.tlin",GetName()),fTLin);
  detvar.GetParam(Form("%s.tshift",GetName()),fTShift);
  detvar.GetParam(Form("%s.tzero",GetName()),fTo);
 

}
void RN_RFTime::InsertHit(const double&t){
  fT=t;
}

void RN_RFTime::Reset(){
  fT=0;
}

#endif
