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
int RN_RootSet(0);

TFile * rootfile;
TTree * newtree;
TList * analyzers;

RN_Analyzer MainAnalyzer;

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
  
  

  void SetReaction(std::string a,
		   std::string b,
		   std::string c,
		   std::string d,
		   std::string e,
		   std::string f){
    
    if(particle.size()!=0){
      std::cout<<"clearing current particle list"<<std::endl;
      particle.clear();
    }
    particle.push_back(a);
    particle.push_back(b);
    particle.push_back(c);
    particle.push_back(d);
    particle.push_back(e);    
    particle.push_back(f);

    m_beam = particle[0].mass;
    m_target = particle[1].mass;
    m_recoil = particle[2].mass;
    m_frag = particle[3].mass;
    m_decay_product = particle[4].mass;
    m_heavy_decay = particle[5].mass;
  }
}


void RN_RootReset(){
  global::beam_e=0;
  global::beam_eloss=0;
  global::beam_est=0;
  global::m_beam=0;
  global::m_target=0;
  global::m_frag=0;
  global::m_recoil=0;
  global::m_heavy_decay=0;
  global::m_decay_product=0;
  global::hi_ex_set=0;
  global::d_ex_set=0;

  if(analyzers)analyzers->Clear();
  neut.clear();
  rftime.clear();
  si_.clear();
  si_cluster_.clear();
  nai.clear();
  particle.clear();
  triggerbit.clear();
  DetVar.ClearParams();
  RN_RootSet=0;
}


void RN_RootInit(){
  if(!analyzers)analyzers = new TList();

  if(!RN_RootSet){
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
  //neut.push_back(RN_NeutDetector("neut10",4,2));
  //neut.push_back(RN_NeutDetector("neut11",4,3));
  //neut.push_back(RN_NeutDetector("neut12",4,8));
  //neut.push_back(RN_NeutDetector("neut13",4,9));
  //neut.push_back(RN_NeutDetector("neut14",4,13));
  //neut.push_back(RN_NeutDetector("neut15",4,16));


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
  
  RN_RootSet = 1;
    
  
}

void LoadVariableFile(const std::string& f){DetVar.LoadParams(f);}
void SetRootOutputFile(std::string filename){
  rootfile=new TFile(filename.c_str(),"RECREATE");
}
void SetRootOutputFileAndTree(std::string filename,std::string treename){
  rootfile=new TFile(filename.c_str(),"RECREATE");
  newtree=new TTree(treename.c_str(),treename.c_str());
}

int AddAnalyzer(TObject *obj){
  if(!analyzers){
    std::cout<<"analyzer list not set"<<std::endl;
    return 0;
  }
  analyzers->Add(obj);
  return 1;
}


//Apply any calibrations loaded into the variable map DetVar 

void SetCalibrations(){
  DetVar.GetParam("global.beam_e",global::beam_e);
  DetVar.GetParam("global.beam_eloss",global::beam_eloss);
  DetVar.GetParam("global.hi_ex_set",global::hi_ex_set);
  if(global::beam_e>0) global::beam_est = global::beam_e - global::beam_eloss*.5;
  
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


