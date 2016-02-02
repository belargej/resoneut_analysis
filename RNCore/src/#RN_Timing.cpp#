#ifndef __RFTIME_CXX
#define __RFTIME_CXX

#include "sak_ReadBuffer.hpp"
#include "RN_Timing.hpp"
#include <TCanvas.h>

ClassImp(RN_RFTime);

RN_RFTime::RN_RFTime(std::string name):RN_BaseClass(name,name),
				       fTLin(1),
				       fTShift(0),
				       fTo(0),
				       fCurrentRun(0)
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

void RN_RFTime::InsertHit(const double&t,const int&run){
  InsertHit(t);
  ChangeRun(run);
}

void RN_RFTime::Reset(){
  fT=0;
}


Int_t RN_RFTime::LoadTable(const std::string &filename){
  
  std::ifstream cal;
  int key;double value;
  cal.open(filename.c_str());
  if (!cal.is_open()){
    std::cout << "  Could not open " << filename << std::endl;
    return 0;
  }
  do{
    std::vector<std::string>input;
    sak::ReadLine(cal,input,2);
    if (input.size()!=2){
      std::cout<<"rf offset file has line with diff than 2 entries"<<std::endl;
      continue;
    }
    key=sak::string_to_int(input[0]);
    value=sak::string_to_double(input[1]);
    
    fTable.insert(std::pair<int,double>(key,value));
  }while(!cal.eof());

  return 1;
}

void RN_RFTime::ClearTable(){
  fTable.clear();
}

Int_t RN_RFTime::ChangeRun(const Int_t & currentrun){
  if(currentrun == fCurrentRun){
    return 0; //no change in state
  }
  else{
    fCurrentRun = currentrun;
    std::map<int,double>::iterator it;
    it = fTable.find(fCurrentRun);
    if(it!=fTable.end()){
      fTShift = it->second;
    } 
  }
  return 1;
}











#endif
