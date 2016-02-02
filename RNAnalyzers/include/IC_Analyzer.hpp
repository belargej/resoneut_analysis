
/***********************************************************/
//Class: IC_Analyzer
//
//Author:Sean Kuvin
//
//IC Analyzer class to be inserted in the analyzer list for 
//sorting Ion Chamber parameters. Making Histograms, 
//Loading Cuts, and checking validity of cuts should 
//all be performed here when considering Ion chamber 
//parameters on their own. 
/***********************************************************/



#ifndef _ICANALYZER_H
#define _ICANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"

namespace ionchamber{

  class IC_Analyzer:public RN_Analyzer{
  private:
    bool OneHitRecon;
    bool TwoHitRecon;
    
  public:

    IC_Analyzer();
    virtual ~IC_Analyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual void Reset();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    
    void TwoHitReconOn(){TwoHitRecon = true;};
    ClassDef(IC_Analyzer,0);
  };

  extern TCutG* ede_hi1;
  extern TCutG* ede_hi2;
  extern TCutG* ede_hi3;
  extern TCutG* posede;

  // JAB
  extern TCutG* Ne19;
  extern TCutG* F19_HE_CS;
  extern TCutG* F19_LE_CS;
  extern int Ne19Check;
  extern int F19HECSCheck;
  extern int F19LECSCheck;


  extern int hi_check[3];
  void RequireHI1();
  void RequireHI2();
  void RequireHI3();
  void RequirePosEDE();

  // JAB
  void RequireNe19(int i = 1);
  void RequireF19_HE_CS(int i=1);
  void RequireF19_LE_CS(int i=1);


  void LoadGates(const std::string &input);  
  void ClearGates();  
 

}


#endif
