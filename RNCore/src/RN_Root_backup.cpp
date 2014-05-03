#ifndef _RNROOT_CORE_CXX
#define _RNROOT_CORE_CXX
#include "RN_Root.hpp"

using namespace std;


////////////////////////////////////////////////////
/////USER CHANGES ARE MADE HERE:////////////////////
////////////////////////////////////////////////////

//Define Modules
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

//define detectors/physics containers
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

RN_VariableMap gVariableMap;
RN_MassTable gMassTable;
RN_Analyzer gMainAnalyzer;

RN_Analyzer_Stack gAnalyzer_stack;
RN_Module_Stack gModule_stack;
RN_Parameter_Stack gParameter_stack;
RNROOT gRNROOT;

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


//Apply any calibrations loaded into the variable map DetVar 

void RNROOT::SetCalibrations(){

 
}


RNROOT::~RNROOT(){
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


