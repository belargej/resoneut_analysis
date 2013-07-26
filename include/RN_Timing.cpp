#include "RN_Timing.hpp"
#include <TCanvas.h>

ClassImp(RN_RFTime);

RN_RFTime::RN_RFTime(std::string name):fName(name),
				       tlin(1),
				       tshift(0),
				       fTo(0)
{ 
  
}
void RN_RFTime::ApplyCalibrations(){
  fT=(fT*tlin)+tshift;
  if(fTo){
     fT = fmod((fT - fTo),82.417);
     if (fT < 0)
       fT+=82.417;
     fT=fabs(82.417-fT);
  }
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
