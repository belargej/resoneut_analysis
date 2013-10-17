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

  virtual void Begin();
  virtual void Process();
  virtual void Clear(){};
  virtual void Terminate();
  virtual void ResetGlobals();


  ClassDef(Trigger_Analyzer,0);
};

  int SetTriggerLocs(const double& n_trig,const double& s1_trig,const double& nai_trig_up,const double& nai_trig_down);

  //extern double n_triggerloc;
  //extern double nai_up_triggerloc;
  //extern double nai_down_triggerloc;
  //extern double s1_triggerloc;
  extern double n_tfirst;
  extern double nai_up_tfirst;
  extern double nai_down_tfirst;
  extern double s1_tfirst;
  extern double s1raw_tfirst;

  extern std::vector<float> triggerinfo;
  extern std::vector<std::string> triggernames;

}
#endif
