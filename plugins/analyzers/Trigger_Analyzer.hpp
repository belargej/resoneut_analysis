/*****************************************************************
Class: Trigger_Analyzer
Author: Sean Kuvin
V1.0  10/17/2013

Trigger Analyzer can be inserted into the Analyzer list
to obtain information about which detector was the trigger or
which detector in a coincidence trigger was the time determining
(later in time) one.
The information which becomes available are the "t-firsts" for situations
where there are multiple hits in one detector(or set of detectors). 
and the
std::vector<float>triggerinfo, which provides the trigger information.
each element in the triggerinfo is a ratio of how close the TDC time was
to the expected location of the time determining peak which the user must
specify using SetTriggerLocs.

Require_____() can also be set to make explicit filtering requirements on 
the data since Trigger_Analyzer will typically come first or early in the
analyzer list.  If RequireS1() is set before sorting the data
then events which do not have a valid S1 time will be prevented from proceeding
to analyzers which follow this one.
*******************************************************************/


#ifndef _TRIGANALYZER
#define _TRIGANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"

namespace trigger{

class Trigger_Analyzer:public RN_Analyzer{
private:

public:  
  Trigger_Analyzer();
  virtual ~Trigger_Analyzer(){};

  virtual bool Begin();
  virtual bool Process();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual void ResetGlobals();


  ClassDef(Trigger_Analyzer,0);
};

  int SetTriggerLocs(const double& n_trig,const double& s1_trig,const double& nai_trig_up,const double& nai_trig_down);
  void SetICdsOFF();
  void RequireS1();
  void RequireNeut();
  void RequireNaI();
  void Require_NTMult(int mult);
  void Require_NEMult(int mult);  

  //extern double n_triggerloc;
  //extern double nai_up_triggerloc;
  //extern double nai_down_triggerloc;
  //extern double s1_triggerloc;
  extern double n_tfirst;
  extern double nai_up_tfirst;
  extern double nai_down_tfirst;
  extern double s1_tfirst;
  extern double s1raw_tfirst;

  extern int n_tmult;       
  extern int s1_tmult;      
  extern int s1raw_tmult;   
  extern int nai_up_tmult;  
  extern int nai_down_tmult;
  					       
  extern int n_emult;       
  extern int s1_emult;      
  extern int s1raw_emult;   
  extern int nai_up_emult;  
  extern int nai_down_emult;

  extern int icds_OFF;
  extern int require_S1;
  extern int require_neut;
  extern int require_nai;
  extern int require_ic;
  extern int require_ntmult;
  extern int require_nemult;

  extern double S1_eps;
  extern double neut_eps;
  extern double nai_eps;
  





  extern std::vector<float> triggerinfo;
  extern std::vector<std::string> triggernames;

}
#endif
