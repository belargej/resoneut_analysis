#ifndef _S2ANALYZER
#define _S2ANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"
#include "RN_Calibrator.hpp"

namespace si_cal{

  class S2_Analyzer:public RN_Analyzer{
  private:
    int ind_;
    
  public:
    
    S2_Analyzer();
    virtual ~S2_Analyzer(){};
    void AutoCalibrate(int matchfront, int matchback);//auto calibrate all elements of Collection
    
    virtual int initHists(std::string,int ind=0);
    virtual void Process();
    virtual void Terminate();
    virtual void Clear();
    
    
    ClassDef(S2_Analyzer,0);
  };

  
  
  
  void producehists(const char * input,const char* output,int index,const char* config="");
  void autocalibrate(const char* input,int fmatch,int bmatch);
  
}


#endif
