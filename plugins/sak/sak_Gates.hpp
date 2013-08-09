#ifndef _SAKGATES_
#define _SAKGATES_
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
       
    bool Check() const;

    ClassDef(Gate,1);
  };


  typedef std::vector<sak::Gate> GateList;
  typedef std::vector<sak::Gate>::iterator GateListRef;
  typedef std::vector<sak::Gate>::const_iterator GateListCRef;
  bool OrCheck(const GateList& gl);
  bool AndCheck(const GateList& gl);



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
