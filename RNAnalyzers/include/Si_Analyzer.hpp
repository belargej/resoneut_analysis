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
    int ind_;
    
  public:
    
    Si_Analyzer();
    virtual ~Si_Analyzer(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    virtual void ResetGlobals();
    
    
    ClassDef(Si_Analyzer,0);
  };

  extern TCutG* prots1;
  extern TCutG* prots2;
  extern TCutG* alphas;
  extern TCutG* deuterons;
  extern TCutG* ptheta_cut;
  extern TCutG* ptheta2_cut;
  extern TCutG* thetatheta_cut;

  extern int _require_proton;
  extern int _require_proton2;
  extern int _require_alpha;
  extern int _require_deuteron;
  extern int _require_thetatheta;
  extern int _require_ptheta;
  extern int _require_ptheta2;
  extern int protcheck;
  extern int prot2check;
  extern int alphacheck;
  extern int deut_check;


  void RequireProton();
  void RequireProton2();
  void RequireDeuteron();
  void RequireAlpha();
  void RequirePThetaCut();
  void RequirePTheta2Cut();
  void RequireThetaTheta();
  void SetZMinMax(const double &,const double&);
  void LoadGates(const std::string &input);  
  void ClearGates();  



  
}


#endif
