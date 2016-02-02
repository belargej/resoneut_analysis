/***************************************************************/
//Class: Neut_TOF_Analyzer
//
//Author:Sean Kuvin
//
//Neut_TOF_Analyzer is added to the analyzer list to sort neutron detector
//timing results.
/********************************************************************/


#ifndef _NEUTTOFANALYZER_H
#define _NEUTTOFANALYZER_H
#include <TH2.h>
#include <TFile.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Gates.hpp"
#include "RN_Root.hpp"

namespace neut_tof{
  
  
  class Neut_TOF_Analyzer:public RN_Analyzer{
  private:
    // JAB
    int NeutTOF_LL;
    int NeutTOF_UL;
    bool RequireNTG;
  public:
    
    Neut_TOF_Analyzer();
    virtual ~Neut_TOF_Analyzer(){};
    
    virtual bool Begin();
    
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear(){};
    virtual void Reset();

    // JAB
    void SetNeutronTimeGate(const int& LowerLimit,const int& UpperLimit);
    void RequireNeutTimeGate(){RequireNTG = true;};
    

  ClassDef(Neut_TOF_Analyzer,0);

};
  

}


#endif
