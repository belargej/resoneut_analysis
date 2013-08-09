#ifndef _SAKGATES_cxx
#define _SAKGATES_cxx

#include "sak_Gates.hpp"
namespace sak{


  Gate::Gate(double *a,double *b,TCutG cut):TCutG(cut),
					    aptr(a),
					    bptr(b){ 
  }
  
  Gate::~Gate(){
    aptr=0;
    bptr=0;
  }
  
  bool Gate::Check() const{
    if(IsInside(*aptr,*bptr)) 
      return true;
    return false;
  }
  
  bool AndCheck(const GateList& gl){
    for(GateListCRef it=gl.begin();it!=gl.end();it++){
      if ((*it).Check()==0)
	return 0;
    } 
    return 1;
  }
  
  
  bool OrCheck(const GateList& gl){
    
    for(GateListCRef it=gl.begin();it!=gl.end();it++){
      if ((*it).Check()==1)
	return 1;
    } 
    return 0;
  }

  

  LoadCuts::LoadCuts(const char* a):TFile(a){
  }

  TCutG* LoadCuts::getCut(const std::string& a){
      return (TCutG*)Get(a.c_str());
  }
  
  
  

  SaveCuts::SaveCuts(const std::string&a):cutfile(a.c_str(),"UPDATE"){
  }
  int SaveCuts::save(TCutG*a){
    cutfile.cd();
    a->Write();
    return 1;
    

  }
  


}
/*
namespace RNMemTest{
  bool TestGateValid(double *a,double *b,sak::Gate f){
    if(f.aptr==a && f.bptr==b)
      return true;
    else 
      return false;
  }
}
*/

#endif
