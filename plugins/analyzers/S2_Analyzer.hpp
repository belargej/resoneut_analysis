/***********************************************************/
//Class: S2_Analyzer
//
//Author:Sean Kuvin
//
//S2 Analyzer class to be inserted in the analyzer list for 
//sorting S1 and S2 type detectors. Making Histograms, 
//Loading Cuts, and checking validity of cuts should 
//all be performed here when considering Silicon detector 
//parameters on their own. 
/***********************************************************/

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
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    virtual void ResetGlobals();
    
    
    ClassDef(S2_Analyzer,0);
  };

  extern TCutG* prots1;
  extern TCutG* prots2;
  extern TCutG* alphas;
  extern TCutG* ptheta_cut;
  extern TCutG* thetatheta_cut;

  extern int _require_proton;
  extern int _require_alpha;
  extern int _require_thetatheta;
  extern int _require_ptheta;
  extern int protcheck;
  extern int prot2check;
  extern int alphacheck;
  extern double prot_E;
  extern double prot_dE;
  extern double prot_theta;
  extern double rel_angle;
  extern double rel_z;
  extern double rel_transverse;


  void RequireProton();
  void RequireAlpha();
  void RequirePThetaCut();
  void RequireThetaTheta();
  void LoadGates(const std::string &input);  
  void ClearGates();  

  void producehists(const char * input,const char* output,int index,const char* config="");
  void autocalibrate(const char* input,int fmatch,int bmatch);
  
}


#endif
