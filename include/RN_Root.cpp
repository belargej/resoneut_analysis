#ifndef _RNROOT_CORE_CXX
#define _RNROOT_CORE_CXX
#include "RN_Root.hpp"

using namespace std;

//Global////////////////////////////
////////Histograms//////////////////
//////////////////Cuts//////////////
//////////////////////etc///////////

RN_ParticleCollection particle;
RN_TriggerBitCollection triggerbit;
RN_NeutDetectorArray Narray;	     
RN_NeutCollection neut;	     
RN_S2Collection si_;		     
RN_S2ClusterCollection si_cluster_;
RN_RFCollection rftime;	     
RN_IonChamber ic("ic");		     
RN_NaICollection nai;		     
RN_VariableMap DetVar;
RN_MassTable MassTable;
int RN_DetectorSet(0);

TFile * rootfile;

namespace global{
  double beam_e(0);
  double beam_eloss(0);
  double beam_est(0);
  double m_beam(0);
  double m_target(0);
  double m_frag(0);
  double m_recoil(0);
  double m_heavy_decay(0);
  double m_decay_product(0);
  double hi_ex_set(0);
  double d_ex_set(0);
  TRandom3 myRnd(0);
  
  
  void LoadGlobalParams(){
    DetVar.GetParam("global.beam_e",beam_e);
    DetVar.GetParam("global.beam_eloss",beam_eloss);
    DetVar.GetParam("global.hi_ex_set",hi_ex_set);

    beam_est = beam_e - beam_eloss*.5;

    //Set Up Experiment Reaction Here
    MassTable.GetParam("24Mg",m_beam);
    MassTable.GetParam("d",m_target);
    MassTable.GetParam("n",m_recoil);
    MassTable.GetParam("25Al",m_frag);
    MassTable.GetParam("24Mg",m_heavy_decay);
    MassTable.GetParam("p",m_decay_product);
   
 
  }
}

void RN_RootInit(){
  if(!RN_DetectorSet){
    neut.reserve(16);
    rftime.reserve(2);
    si_.reserve(2);
    si_cluster_.reserve(2);
    nai.reserve(20);
    particle.reserve(6);
    triggerbit.reserve(5);
  }
  else{
    neut.clear();
    rftime.clear();
    si_.clear();
    si_cluster_.clear();
    nai.clear();
    particle.clear();
    triggerbit.clear();
  }

  particle.push_back(RN_Particle("24Mg"));
  particle.push_back(RN_Particle("d"));
  particle.push_back(RN_Particle("n")); 
  particle.push_back(RN_Particle("25Al"));
  particle.push_back(RN_Particle("p"));  
  particle.push_back(RN_Particle("24Mg"));
  
  


  si_.push_back(RN_S1Detector("si_a",16,16));
  si_.push_back(RN_S2Detector("si_b",16,16));
  
  si_cluster_.push_back(RN_S2Cluster("si_cluster_a",16));
  si_cluster_.push_back(RN_S2Cluster("si_cluster_b",16));
  
  rftime.push_back(RN_RFTime("rftime"));
  rftime.push_back(RN_RFTime("MCP"));
  
  triggerbit.push_back(RN_TriggerBit("S1bit"));
  triggerbit.push_back(RN_TriggerBit("ICbit"));

  neut.push_back(RN_NeutDetector("neut0",4,1));
  neut.push_back(RN_NeutDetector("neut1",4,4));
  neut.push_back(RN_NeutDetector("neut2",4,5));
  neut.push_back(RN_NeutDetector("neut3",4,6));
  neut.push_back(RN_NeutDetector("neut4",4,7));
  neut.push_back(RN_NeutDetector("neut5",4,11));
  neut.push_back(RN_NeutDetector("neut6",4,14));
  neut.push_back(RN_NeutDetector("neut7",4,10));
  neut.push_back(RN_NeutDetector("neut8",4,12));
  neut.push_back(RN_NeutDetector("neut9",4,15));
  
  nai.push_back(RN_NaIDetector("nai_l1"));
  nai.push_back(RN_NaIDetector("nai_l2"));
  nai.push_back(RN_NaIDetector("nai_l3"));
  nai.push_back(RN_NaIDetector("nai_l4"));
  nai.push_back(RN_NaIDetector("nai_l5"));
  nai.push_back(RN_NaIDetector("nai_l6"));
  nai.push_back(RN_NaIDetector("nai_l7"));
  nai.push_back(RN_NaIDetector("nai_l8"));
  nai.push_back(RN_NaIDetector("nai_l9"));
  nai.push_back(RN_NaIDetector("nai_l10"));
  nai.push_back(RN_NaIDetector("nai_r1"));
  nai.push_back(RN_NaIDetector("nai_r2"));
  nai.push_back(RN_NaIDetector("nai_r3"));
  nai.push_back(RN_NaIDetector("nai_r4"));
  nai.push_back(RN_NaIDetector("nai_r5"));
  nai.push_back(RN_NaIDetector("nai_r6"));
  nai.push_back(RN_NaIDetector("nai_r7"));
  nai.push_back(RN_NaIDetector("nai_r8"));
  nai.push_back(RN_NaIDetector("nai_r9"));
  nai.push_back(RN_NaIDetector("nai_r10"));
  
  RN_DetectorSet = 1;
    
  
}

void LoadVariableFile(const std::string& f){DetVar.LoadParams(f);}

void SetCalibrations(){
  global::LoadGlobalParams();
  
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    (*it).SetCalibrations(DetVar);
  }
  
  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    (*it).SetCalibrations(DetVar);
  }
  
  for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++){
    (*it).SetCalibrations(DetVar);
  }
  for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++){
    (*it).SetCalibrations(DetVar);
  }
  
  ic.SetCalibrations(DetVar);
  
  for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
    (*it).SetCalibrations(DetVar);
  }

  
  
}

#endif




///////////////////////////////////////////////////////////////////////
#ifdef MAKE_RN_EXE

TRint *myRint;

int main(int argc, char*argv[]){  
  myRint=new TRint("myRint",&argc,argv);
  myRint->Run();
  return 0;
}

#endif



