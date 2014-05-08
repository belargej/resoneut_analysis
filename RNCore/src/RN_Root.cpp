#ifndef _RNROOT_CORE_CXX
#define _RNROOT_CORE_CXX
#include "RN_Root.hpp"

using namespace std;


////////////////////////////////////////////////////
/////USER CHANGES ARE MADE HERE:////////////////////
////////////////////////////////////////////////////

CAEN_ADC ADC1("ADC1",2);
CAEN_ADC ADC2("ADC2",3);
CAEN_ADC ADC3("ADC3",4);
CAEN_ADC ADC4("ADC4",5);
CAEN_ADC ADC5("ADC5",6);
CAEN_ADC ADC6("ADC6",7);
CAEN_ADC ADC7("ADC7",8);
CAEN_TDC TDC1("TDC1",10);
CAEN_TDC TDC2("TDC2",11);
CAEN_TDC TDC3("TDC3",12);
CAEN_TDC TDC4("TDC4",13);
MESY_QDC QDC1("QDC1",14);
MESY_QDC QDC2("QDC2",16);
MESY_QDC QDC3("QDC3",18);

////////////////////////////////////////////////////////////////

RN_ParticleCollection particle;
RN_TriggerBitCollection triggerbit;
RN_NeutDetectorArray Narray;	     
RN_NeutCollection neut;	     
RN_S2Collection si_;		     
RN_S2ClusterCollection si_cluster_;
RN_RFCollection rftime;
RN_IonChamber ic("ic");		     
RN_NaICollection nai;	
RN_NaIArray nai_array("nai_array",20);	     

RN_VariableMap DetVar;
RN_MassTable MassTable;
int RN_RootSet(0);

TFile * rootfile;
TTree * newtree;
TList * analyzers;

RN_Analyzer MainAnalyzer;

RNROOT gRNROOT;


int GetDetectorEntry(){


  //Reset all detectors
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    (*it).Reset();
  }

  Narray.Reset();

  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    (*it).Reset();
  }
  
  for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++){
    (*it).Reset();
  }
  for(RN_RFCollectionRef it=rftime.begin();it!=rftime.end();it++){
    (*it).Reset();
  }

  ic.Reset();

  for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
    (*it).Reset();
  }
  
  nai_array.Reset();


  //ChanneltoDetector
  
  //Neutron Detector Long and Short gate Hits
  int idx=1; //neut detectors start from channel 1 in QDC
  for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
    if(QDC1[idx]>0){
      (*it).InsertPSDHit(QDC1[idx],QDC1[idx+16]);
    }
    if(TDC1[idx+16]>0){  
      (*it).fT_Q=TDC1[idx+16];
    }  
    idx++;
  }
  //Neutron Detector position channel hits
  //insert here

  //Silicon Hits
  for(int j=0;j<16;j++){
    //si_a de front
    if(ADC3[j+16]>0)si_[0].front.InsertHit(ADC3[j+16],0,j);
    //si_b e front
    if(ADC2[j+16]>0)si_[1].front.InsertHit(ADC2[j+16],0,j);
    //si_a de back
    if(ADC3[j]>0)si_[0].back.InsertHit(ADC3[j],TDC2[j+16],j);
    //si_b e back
    if(ADC2[j]>0)si_[1].back.InsertHit(ADC2[j],TDC2[j],j);

  }

  //NaI E and T hits
  int k=0;
  for(int j=0;j<8;j++){
    if(ADC4[k+16]>0)nai[j].fE[0]=ADC4[k+16];
    if(ADC4[k+17]>0)nai[j].fE[1]=ADC4[k+17];
    if(TDC3[k+16]>0)nai[j+8].fT[0]=TDC3[k+16];
    if(TDC3[k+17]>0)nai[j+8].fT[1]=TDC3[k+17];
    
    //if(TDC3[k]>0)nai[j].fT[0]=TDC3[k];  //first 16 channels don't look good
    //if(TDC3[k+1]>0)nai[j].fT[1]=TDC3[k+1];//moving to TDC4 back 16

   
    if(TDC4[k+16]>0)nai[j].fT[0]=TDC4[k+16];
    if(TDC4[k+17]>0)nai[j].fT[1]=TDC4[k+17];

    if(j<4){
      if(TDC4[k]>0)nai[j+16].fT[0]=TDC4[k];
      if(TDC4[k+1]>0)nai[j+16].fT[1]=TDC4[k+1];
    }
    
    k+=2;
  }

  k=0;
  for(int j=0;j<12;j++){
    if(ADC7[k]>0)nai[j+8].fE[0]=ADC7[k];
    if(ADC7[k+1]>0)nai[j+8].fE[1]=ADC7[k+1];
    k+=2;
  }



  //IonChamber Position grid hits
  for(int k=0;k<32;k++){
    if(ADC5[k]>0){
      ic.xgrid.InsertHit(ADC5[k],0,k);
    }
    if(ADC6[k]>0){
      ic.ygrid.InsertHit(ADC6[k],0,k);
    }
  }

  //IonChamber E+dE hits
  if(ADC4[14]>0)ic.fE=ADC4[14];
  if(ADC4[15]>0)ic.fdE=ADC4[15];
  //if(ADC4[13]>0)ic.fE=ADC4[13];
  //if(ADC4[14]>0)ic.fdE=ADC4[14];  
  if(TDC1[1]>0) ic.fT = TDC1[1];

  //rftime
  if(TDC1[0]>0)rftime[0].InsertHit(TDC1[0]);
  
  //mcptime
  if(TDC1[2]>0)rftime[1].InsertHit(TDC1[2]);


  if(TDC1[3]>0)triggerbit[0].fBit=TDC1[3];
  if(TDC1[4]>0)triggerbit[1].fBit=TDC1[4];
  

  //recontruct clusters for all silicon detectors in S2Collection
  unsigned int cref=0;
  for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
    if(cref<si_cluster_.size())
      si_cluster_[cref].ReconstructClusters(*it);
    cref++;
    
  }

  //reconstruct neutron detector hits for all neutron detectors 
  //in NeutCollection
  Narray.ReconstructHits(neut);
  RNArray::ReconstructTREL(neut,Narray.fT_mult,Narray.fT_first,Narray.fDetfirst);
  
  nai_array.ReconstructHits(nai);
  
  return 1;
}




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
  double E_fragment(0);
  TRandom3 myRnd(0);
  TVector3 target_pos(0,0,0);
  

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
    unpacker::gModule_stack.ClearStack();
  }

  //add the modules declared above to the stack...this should match
  //daqconfig for the runs you are analyzing.

  //  unpacker::caen_stack.AddModule(&ADC1);
  unpacker::gModule_stack.AddModule(&ADC2);
  unpacker::gModule_stack.AddModule(&ADC3);
  unpacker::gModule_stack.AddModule(&ADC4);
  unpacker::gModule_stack.AddModule(&ADC5);
  unpacker::gModule_stack.AddModule(&ADC6);
  unpacker::gModule_stack.AddModule(&ADC7);
  unpacker::gModule_stack.AddModule(&TDC1);
  unpacker::gModule_stack.AddModule(&TDC2);
  unpacker::gModule_stack.AddModule(&TDC3);
  unpacker::gModule_stack.AddModule(&TDC4);
  unpacker::gModule_stack.AddModule(&QDC1);
  unpacker::gModule_stack.AddModule(&QDC2);
  unpacker::gModule_stack.AddModule(&QDC3);

  //a,b,c,d in order of moving downstream from target.
  si_.push_back(RN_S2Detector("si_a",16,16));
  si_.push_back(RN_S2Detector("si_b",16,16));

  //clusters for identifying matching front and back segments
  si_cluster_.push_back(RN_S2Cluster("si_cluster_a",16));
  si_cluster_.push_back(RN_S2Cluster("si_cluster_b",16));
  

  rftime.push_back(RN_RFTime("rftime"));
  rftime.push_back(RN_RFTime("MCP"));
  
  triggerbit.push_back(RN_TriggerBit("S1bit"));
  triggerbit.push_back(RN_TriggerBit("ICbit"));

  //neutron detectors - name - pos grid channel num - slot num
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
  /*
  neut.push_back(RN_NeutDetector("neut10",4,2));
  neut.push_back(RN_NeutDetector("neut11",4,3));
  neut.push_back(RN_NeutDetector("neut12",4,8));
  neut.push_back(RN_NeutDetector("neut13",4,9));
  neut.push_back(RN_NeutDetector("neut14",4,13));
  neut.push_back(RN_NeutDetector("neut15",4,16));
  */

  //nai detectors -- name
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

  if(rootfile){
    std::cout<<"root output file already open, please write out and close first"<<std::endl;
    return ;
  }
    rootfile=new TFile(filename.c_str(),"RECREATE");
}

void SetRootOutputFileAndTree(std::string filename,std::string treename){
   if(rootfile){
    std::cout<<"root output file already open, please write out and close first"<<std::endl;
    return ;
  }
 
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
  double temp(0);
  DetVar.GetParam("global.beam_e",global::beam_e);
  DetVar.GetParam("global.beam_eloss",global::beam_eloss);
  DetVar.GetParam("global.hi_ex_set",global::hi_ex_set);
  DetVar.GetParam("global.E_fragment",global::E_fragment);
  if(DetVar.GetParam("global.target_pos.x",temp))
    global::target_pos.SetX(temp);
  if(DetVar.GetParam("global.target_pos.y",temp))
    global::target_pos.SetY(temp);
  if(DetVar.GetParam("global.target_pos.z",temp))
    global::target_pos.SetZ(temp);
  


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


RNROOT::~RNROOT(){
  if(rootfile)delete rootfile;
  if(newtree)delete newtree;
  if(fRNAnalyzers)delete fRNAnalyzers;
  if(fRNDetectors)delete fRNDetectors;
  if(fRNClasses)delete fRNClasses;
  if(fRNVariables)delete fRNVariables;
  if(fRNParameters)delete fRNParameters;
  
}




void RNROOT::SetReaction(std::string a,
			 std::string b,
			 std::string c,
			 std::string d,
			 std::string e,
			 std::string f)
{
  
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
}



void RNROOT::Reset(){
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

  fRNDetectors->Clear();
  fRNAnalyzers->Clear();
  fRNClasses->Clear();
  DetVar.ClearParams();

  neut.clear();
  rftime.clear();
  si_.clear();
  si_cluster_.clear();
  nai.clear();
  particle.clear();
  triggerbit.clear();
  
  RN_RootSet=0;
}



void RNROOT::Init(){
  if(!RN_RootSet){
    neut.reserve(16);
    rftime.reserve(2);
    si_.reserve(2);
    si_cluster_.reserve(2);
    nai.reserve(20);
    particle.reserve(6);
    triggerbit.reserve(5);
    if(!fRNAnalyzers)fRNAnalyzers = new TList();
    if(!fRNDetectors)fRNDetectors = new TList();
    if(!fRNClasses)fRNClasses = new TList();
    if(!fRNVariables)fRNVariables =new TList();
    if(!fRNParameters)fRNParameters =new TList();

  }
  else{
    neut.clear();
    rftime.clear();
    si_.clear();
    si_cluster_.clear();
    nai.clear();
    particle.clear();
    triggerbit.clear();
    if(fRNAnalyzers)fRNAnalyzers->Clear();
    if(fRNDetectors)fRNDetectors->Clear();
    if(fRNClasses)fRNClasses->Clear();
    if(fRNVariables)fRNVariables->Clear();
    if(fRNParameters)fRNParameters->Clear();
  }


  new RN_S1Detector("si_a",16,16);
  new RN_S2Detector("si_b",16,16);
  
  new RN_S2Cluster("si_cluster_a",16);
  new RN_S2Cluster("si_cluster_b",16);
  
  new RN_RFTime("rftime");
  new RN_RFTime("MCP");
  
  new RN_TriggerBit("S1bit");
  new RN_TriggerBit("ICbit");
  
  new RN_NeutDetector("neut0",4,1);
  new RN_NeutDetector("neut1",4,4);
  new RN_NeutDetector("neut2",4,5);
  new RN_NeutDetector("neut3",4,6);
  new RN_NeutDetector("neut4",4,7);
  new RN_NeutDetector("neut5",4,11);
  new RN_NeutDetector("neut6",4,14);
  new RN_NeutDetector("neut7",4,10);
  new RN_NeutDetector("neut8",4,12);
  new RN_NeutDetector("neut9",4,15);
  //neut.push_back(RN_NeutDetector("neut10",4,2));
  //neut.push_back(RN_NeutDetector("neut11",4,3));
  //neut.push_back(RN_NeutDetector("neut12",4,8));
  //neut.push_back(RN_NeutDetector("neut13",4,9));
  //neut.push_back(RN_NeutDetector("neut14",4,13));
  //neut.push_back(RN_NeutDetector("neut15",4,16));


  new RN_NaIDetector("nai_l1");
  new RN_NaIDetector("nai_l2");
  new RN_NaIDetector("nai_l3");
  new RN_NaIDetector("nai_l4");
  new RN_NaIDetector("nai_l5");
  new RN_NaIDetector("nai_l6");
  new RN_NaIDetector("nai_l7");
  new RN_NaIDetector("nai_l8");
  new RN_NaIDetector("nai_l9");
  new RN_NaIDetector("nai_l10");
  new RN_NaIDetector("nai_r1");
  new RN_NaIDetector("nai_r2");
  new RN_NaIDetector("nai_r3");
  new RN_NaIDetector("nai_r4");
  new RN_NaIDetector("nai_r5");
  new RN_NaIDetector("nai_r6");
  new RN_NaIDetector("nai_r7");
  new RN_NaIDetector("nai_r8");
  new RN_NaIDetector("nai_r9");
  new RN_NaIDetector("nai_r10");
  
  RN_RootSet = 1;  
}

void RNROOT::LoadVariableFile(const std::string& f){DetVar.LoadParams(f);}
void RNROOT::SetRootOutputFile(std::string filename){

  if(rootfile){
    std::cout<<"root output file already open, please write out and close first"<<std::endl;
    return ;
  }
    rootfile=new TFile(filename.c_str(),"RECREATE");
}

void RNROOT::SetRootOutputFileAndTree(std::string filename,std::string treename){
   if(rootfile){
    std::cout<<"root output file already open, please write out and close first"<<std::endl;
    return ;
  }
 
  rootfile=new TFile(filename.c_str(),"RECREATE");
  newtree=new TTree(treename.c_str(),treename.c_str());
}

int RNROOT::AddAnalyzer(TObject *obj){
  if(!fRNAnalyzers){
    fRNAnalyzers=new TList();
  }
  fRNAnalyzers->Add(obj);
  return 1;
}

int RNROOT::AddDetector(TObject *obj){
  if(!fRNDetectors){
    fRNDetectors=new TList();
  }
  fRNDetectors->Add(obj);
  return 1;
}

int RNROOT::AddParameter(TObject *obj){
  if(!fRNParameters){
    fRNParameters=new TList();
  }
  fRNParameters->Add(obj);
  return 1;
}

int RNROOT::AddVariable(TObject *obj){
  if(!fRNVariables){
    fRNVariables=new TList();
  }
  fRNVariables->Add(obj);
  return 1;
}

int RNROOT::AddClass(TObject *obj){
  if(!fRNClasses){
    fRNClasses=new TList();
  }
  fRNClasses->Add(obj);
  return 1;
}

//Apply any calibrations loaded into the variable map DetVar 

void RNROOT::SetCalibrations(){
  double temp(0);
  DetVar.GetParam("global.beam_e",global::beam_e);
  DetVar.GetParam("global.beam_eloss",global::beam_eloss);
  DetVar.GetParam("global.hi_ex_set",global::hi_ex_set);
  DetVar.GetParam("global.E_fragment",global::E_fragment);
  if(DetVar.GetParam("global.target_pos.x",temp))
    global::target_pos.SetX(temp);
  if(DetVar.GetParam("global.target_pos.y",temp))
    global::target_pos.SetY(temp);
  if(DetVar.GetParam("global.target_pos.z",temp))
    global::target_pos.SetZ(temp);
  


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


