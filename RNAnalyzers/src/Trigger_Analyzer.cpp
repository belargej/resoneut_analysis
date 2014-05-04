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
		       
#ifndef _TRIG_ANALYZER_CXX
#define _TRIG_ANALYZER_CXX

#include <numeric>		       
#include "Trigger_Analyzer.hpp"
#include "RN_Root.hpp"

		       
		       
using namespace RNROOT;

namespace trigger{
 
  //call SetTriggerLocs in your code to set these values
  double n_triggerloc(0);
  double nai_up_triggerloc(0);
  double nai_down_triggerloc(0);
  double s1_triggerloc(0);
  

  int s1_tmult(0);
  int s1raw_tmult(0);
  int nai_up_tmult(0);
  int nai_down_tmult(0);
  int s2_tmult(0);

  int n_emult(0);
  int s1_emult(0);
  int s1raw_emult(0);
  int nai_up_emult(0);
  int nai_down_emult(0);
  
  int icds_OFF(0);
  int require_S1(0);
  int _require_S2(0);
  int require_neut(0);
  int require_nai(0);
  int _require_ic(0);
  int require_ntmult(0);
  int require_nemult(0);
  int _requireOFF_nai(0);
  int _requireOFF_S2(0);



  double S1_eps(0.0);
  double neut_eps(0.0);
  double nai_eps(0.0);
  
  //initialize all tfirsts to values above TDC range
  double n_tfirst(5000);
  double nai_up_tfirst(5000);
  double nai_down_tfirst(5000);
  double s1_tfirst(5000);
  double s1raw_tfirst(5000);

  //vectors where the trigger information will be stored
  std::vector<float> triggerinfo(6,0);
  std::vector<std::string> triggernames;
  
  void SetICdsOFF(){icds_OFF = 1;}
  void RequireS1(){require_S1 = 1;}
  void RequireNeut(){require_neut = 1 ;}
  void RequireNaI(){require_nai = 1;}
  void Require_NTMult(int mult){require_ntmult=mult;}
  void Require_NEMult(int mult){require_nemult=mult;}
  void RequireOFF_NaI(){_requireOFF_nai = 1;}
  void RequireS2(){_require_S2 = 1;}
  void RequireOFF_S2(){_requireOFF_S2 = 1;}
  void RequireIC_E(){_require_ic = 1;}
  Trigger_Analyzer::Trigger_Analyzer()
  {
    
  }
  
  bool Trigger_Analyzer::Begin(){
    int enter;
    std::cout<<"verify triggerlocs set"<<std::endl;
    std::cout<<"neut: "<<n_triggerloc<<std::endl;
    std::cout<<"nai_up: "<<nai_up_triggerloc<<std::endl;
    std::cout<<"nai_down: "<<nai_down_triggerloc<<std::endl;
    std::cout<<"s1: "<<s1_triggerloc<<std::endl;
    
    std::cin>>enter;
    //hidden exit option
    if(enter==100)
      exit(EXIT_FAILURE);
    
    return 1;
  }
  
  void Trigger_Analyzer::ResetGlobals(){
    //reset all trigger infos
    for(unsigned int i=0; i<triggerinfo.size();i++){
      triggerinfo[i]=0;
    }
    //reset all tfirsts to value above TDC range
    n_tfirst=5000;
    nai_up_tfirst=5000;
    nai_down_tfirst=5000;
    s1_tfirst=5000;
    s1raw_tfirst=5000;

   
    s1_tmult=0;   
    s2_tmult = 0;
    s1raw_tmult=0;   
    nai_up_tmult=0;  
    nai_down_tmult=0;
    
    n_emult=0;       
    s1_emult=0;      
    s1raw_emult=0;   
    nai_up_emult=0;  
    nai_down_emult=0;
    

  }
  
  bool Trigger_Analyzer::Process(){
    
    //find the earliest neut time
    for(unsigned int i=0;i<neut.size();i++){
      if(neut[i].fQ_long>0){
	n_emult++;
      }
    }
    
    //find the earliest si time, this requires an E hit
    for(unsigned int i=0;i<si_[0].back.fMult;i++){
      if(si_[0].back.fE[i]>0){
	s1_emult++;
      }
      if(si_[0].back.fT[i]>0){
	s1_tmult++;
	if(si_[0].Back_T(i)<s1_tfirst){
	  s1_tfirst=si_[0].Back_T(i);
	}
      }
    }

    //find both the earliest upstream/downstream nai time
    for(unsigned int i=0;i<nai.size();i++){
      if(nai[i].fE[0]>0){
	nai_up_emult++;
      }
      if(nai[i].fE[1]>0){
	nai_down_emult++;
      }

      if(nai[i].fT[0]>0){
	nai_up_tmult++;
	if(nai[i].T1()<nai_up_tfirst){
	  nai_up_tfirst=nai[i].T1();  
	}
      }
      if(nai[i].fT[1]>0){
	nai_down_tmult++;
	if(nai[i].T2()<nai_down_tfirst){
	  nai_down_tfirst=nai[i].T2();  
	}
      }
    }

    //find the raw_unsorted TDC time corresponding to the S1 detector
    for(unsigned int i=0;i<16;i++){
      if(TDC2[i]>0){
	s1raw_tmult++;
	if(TDC2[i] < s1raw_tfirst){
	  s1raw_tfirst=TDC2[i];
	}
      }
      if(TDC2[i+16]>0){
	s2_tmult++;	
      }
      
      
    }



    //compare with the expected time determining start/self-stop
    if(n_tfirst<5000)
      triggerinfo[0] = 1 - (TMath::Abs(n_tfirst-n_triggerloc) / n_triggerloc);

    if(s1_tfirst<5000)
      triggerinfo[1] = 1 - (TMath::Abs(s1_tfirst-s1_triggerloc) / s1_triggerloc);

    if(nai_up_tfirst<5000)
      triggerinfo[2] = 1 - (TMath::Abs(nai_up_tfirst-nai_up_triggerloc) / nai_up_triggerloc);
   
    if(nai_down_tfirst<5000)
      triggerinfo[3] = 1 - (TMath::Abs(nai_down_tfirst-nai_down_triggerloc) / nai_down_triggerloc);
   
    if(s1raw_tfirst<5000)
      triggerinfo[5] = 1 - (TMath::Abs(s1raw_tfirst-s1_triggerloc) / s1_triggerloc);
   
    //check the icds bits
    if(TDC1[4]>0)
      triggerinfo[4]=1;

    //check the s1 bits
    if(TDC1[3]>0)
      triggerinfo[1]=1;


    if(Narray.fT_mult<require_ntmult){
      return 0;
    }
    if(n_emult<require_nemult){
      return 0;
    }
    
    //if icds_OFF bit is set then we break from all future processes
    if(triggerinfo[4]&&icds_OFF){
      return 0;
    }
    
    if(require_S1 && !triggerinfo[1])
      return 0;
    
    if(require_neut && !triggerinfo[0])
      return 0;

    if(require_nai && !(triggerinfo[2]||triggerinfo[3]))
      return 0;

    if(_requireOFF_nai && (triggerinfo[2]||triggerinfo[3]))
      return 0;
    
    if(_requireOFF_S2 && si_cluster_[1].fMult>0)
      return 0;
    
    if(_require_S2 && !si_cluster_[1].fMult>0)
      return 0;
    
    if(_require_ic && !(ic.fE && ic.fdE))
      return 0;

    /*
    std::cout<<"*******************\n";
    std::cout<<"tfirsts:  "<<n_tfirst<<" "<<s1_tfirst<<" "<<nai_up_tfirst<<" "<<nai_down_tfirst<<" "<<TDC1[4]<<" "<<s1raw_tfirst<<"\n";
    std::cout<<"triggers: "<<triggerinfo[0]<<" "<<triggerinfo[1]<<" "<<triggerinfo[2]<<" "<<triggerinfo[3]<<" "<<triggerinfo[4]<<" "<<triggerinfo[5]<<"\n";
    std::cout<<"tmults:    "<<Narray.fT_mult<<" "<<s1_tmult<<" "<<nai_up_tmult<<" "<<nai_down_tmult<<" "<<"  "<<" "<<s1raw_tmult<<"\n";
    std::cout<<"emults:    "<<n_emult<<" "<<s1_emult<<" "<<nai_up_emult<<" "<<nai_down_emult<<" "<<"  "<<" "<<"  "<<std::endl;
    */
    return 1;
   
  }
  
  
  
  bool Trigger_Analyzer::Terminate(){
    return 1;
    
  }

  int SetTriggerLocs(const double& n_trig,const double& s1_trig,const double& nai_trig_up,const double& nai_trig_down){
    n_triggerloc=n_trig;
    s1_triggerloc=s1_trig;
    nai_up_triggerloc=nai_trig_up;
    nai_down_triggerloc=nai_trig_down;
    return 1;
  }


}

#endif
