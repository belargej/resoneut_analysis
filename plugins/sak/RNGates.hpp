#ifndef _RNGATES_
#define _RNGATES_
#include <TCutG.h>
#include <TFile.h>
namespace sak{

  class Gate:public TCutG{
  private:
  public:
    double * aptr;//!
    double * bptr;//!
    Gate(){}
    ~Gate();
    Gate(double *a,double *b,TCutG cut);
       
    bool Check();

    ClassDef(Gate,1);
  };



  class LoadCuts:public TFile{
  private:
  public:
    LoadCuts(const char* a);
    TCutG* getCut(const std::string& a);
    
    ClassDef(LoadCuts,1);
  }; 
    
  class SaveCuts{
  private:
  public:
    TFile cutfile;
    SaveCuts(const std::string&a);
    int save(TCutG*a);
  };


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
