/***********************************************************/
//Class: Si_Analyzer
//
//Author:Sean Kuvin
//
//S2 Analyzer class to be inserted in the analyzer list for 
//sorting S1 and S2 type detectors. Making Histograms, 
//Loading Cuts, and checking validity of cuts should 
//all be performed here when considering Silicon detector 
//parameters on their own. 
/***********************************************************/

#ifndef _SIANALYZER_H
#define _SIANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace silicon{



  class Si_Analyzer:public RN_Analyzer{
  private:    
  public:
    
    Si_Analyzer();
    virtual ~Si_Analyzer(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    virtual void Reset();
    
    
    ClassDef(Si_Analyzer,0);
  };

  extern TCutG* prots1;
  extern TCutG* prots2;
  extern TCutG* alphas;
  extern TCutG* deuterons;
  extern TCutG* ptheta_cut;
  extern TCutG* ptheta2_cut;
  extern TCutG* thetatheta_cut;

  extern int protcheck;
  extern int prot2check;
  extern int alphacheck;
  extern int deutcheck;
  extern int pthetacheck;
  extern int ptheta2check;
  extern int alphacheck;
  extern int thetathetacheck;
  extern int deutcheck;

  void RequireProton(int i=1);
  void RequireProton2(int i=1);
  void RequireDeuteron(int i=1);
  void RequireAlpha(int i=1);
  void RequirePThetaCut(int i=1);
  void RequirePTheta2Cut(int i=1);
  void RequireThetaTheta(int i=1);
  void LoadGates(const std::string &input);  
  void ClearGates();  



  
}


#endif
