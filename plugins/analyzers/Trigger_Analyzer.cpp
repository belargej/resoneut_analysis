#include "Trigger_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "../../include/RN_Unpack2Root.hpp"
#include <numeric>
namespace trigger{
 
  //call SetTriggerLocs in your code to set these values
  double n_triggerloc(0);
  double nai_up_triggerloc(0);
  double nai_down_triggerloc(0);
  double s1_triggerloc(0);
  
  //initialize all tfirsts to values above TDC range
  double n_tfirst(5000);
  double nai_up_tfirst(5000);
  double nai_down_tfirst(5000);
  double s1_tfirst(5000);
  double s1raw_tfirst(5000);

  //vectors where the trigger information will be stored
  std::vector<float> triggerinfo(6,0);
  std::vector<std::string> triggernames;
  

  Trigger_Analyzer::Trigger_Analyzer()
  {
    
  }
  
  void Trigger_Analyzer::Begin(){
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
  }
  
  void Trigger_Analyzer::Process(){
    
    //find the earliest neut time
    for(unsigned int i=0;i<neut.size();i++){
      if(neut[i].fT_Q>0 && neut[i].T()<n_tfirst)
	n_tfirst=neut[i].T();
    }
    
    //find the earliest si time, this requires an E hit
    for(int i=0;i<si_[0].back.fMult;i++){
      if(si_[0].back.fT[i]>0&&si_[0].Back_T(i)<s1_tfirst)
	s1_tfirst=si_[0].Back_T(i);
    }

    //find both the earliest upstream/downstream nai time
    for(int i=0;i<nai.size();i++){
      if(nai[i].fT[0]>0 && nai[i].T(0)<nai_up_tfirst)
	nai_up_tfirst=nai[i].T(0);  
      if(nai[i].fT[1]>0 && nai[i].T(1)<nai_down_tfirst)
	nai_down_tfirst=nai[i].T(1);  

    }

    //find the raw_unsorted TDC time corresponding to the S1 detector
    for(unsigned int i=0;i<16;i++){
      if(unpacker::TDC2[i]>0&&unpacker::TDC2[i] < s1raw_tfirst)
	s1raw_tfirst=unpacker::TDC2[i];
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
    if(unpacker::TDC1[4]>0)
      triggerinfo[4]=1;

    //check the s1 bits
    if(unpacker::TDC1[3]>0)
      triggerinfo[1]=1;

    std::cout<<"*******************"<<std::endl;
    std::cout<<"tfirsts: "<<n_tfirst<<" "<<s1_tfirst<<" "<<nai_up_tfirst<<" "<<nai_down_tfirst<<" "<<unpacker::TDC1[4]<<" "<<s1raw_tfirst<<std::endl;
    std::cout<<"triggers: "<<triggerinfo[0]<<" "<<triggerinfo[1]<<" "<<triggerinfo[2]<<" "<<triggerinfo[3]<<" "<<triggerinfo[4]<<" "<<triggerinfo[5]<<std::endl;


   
  }
  
  
  
  void Trigger_Analyzer::Terminate(){
    
    
  }

  int SetTriggerLocs(const double& n_trig,const double& s1_trig,const double& nai_trig_up,const double& nai_trig_down){
    n_triggerloc=n_trig;
    s1_triggerloc=s1_trig;
    nai_up_triggerloc=nai_trig_up;
    nai_down_triggerloc=nai_trig_down;
    
  }


}
