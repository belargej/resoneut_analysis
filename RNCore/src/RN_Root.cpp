#ifndef _RNROOT_CORE_CXX
#define _RNROOT_CORE_CXX
#include "RN_Root.hpp"

using namespace std;

namespace RNROOT{

  //define detectors/physics containers
  RN_ParticleCollection particle;
  RN_TriggerBitCollection triggerbit;
  RN_NeutDetectorArray Narray;	     
  RN_NeutCollection neut;	     
  RN_S2Collection si_;		     
  RN_S2ClusterCollection si_cluster_;
  RN_RFTime rftime("rftime");
  //  RN_MCPDetector MCP("MCP");
  RN_IonChamber ic("ic");		     
  RN_NaICollection nai;	
  RN_NaIArray nai_array("nai_array",20);	     
  

  TFile * gRootFile;
  TTree * gNewTree;
  RN_VariableMap gVariableMap;
  RN_MassTable gMassTable;
  RN_Analyzer gMainAnalyzer;
  RN_Analyzer_Stack gAnalyzer_stack;
  RN_Module_Stack gModule_stack;
  RN_Parameter_Stack gParameter_stack;
  RN_PrimaryReaction gPrimaryReaction;
  RN_ReactionInfo gReactionInfo;
  int RN_RootSet(0);
}


namespace global{
  TRandom3 myRnd;
}

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

namespace RNROOT{
  int Initialize(){
   
    //initialize or clear the containers which hold the detector info
    if(!RN_RootSet){
      neut.reserve(NEUTNUM);
      si_.reserve(SI_NUM);
      si_cluster_.reserve(SI_NUM);
      nai.reserve(NAI_NUM);
      triggerbit.reserve(TRIGBIT_NUM);
    }
    else{
      neut.clear();
      si_.clear();
      si_cluster_.clear();
      nai.clear();
      triggerbit.clear();
      gModule_stack.ClearStack();
      gAnalyzer_stack.ClearStack();
      gParameter_stack.ClearStack();
      gReactionInfo.ClearStack();
      gVariableMap.ClearParams();
      gPrimaryReaction.Clear();  
    }


    //add the modules declared above to the stack...this should match
    //daqconfig for the runs you are analyzing.
    
    //gModule_stack.Add(&ADC1);
    gModule_stack.Add(&ADC2);
    gModule_stack.Add(&ADC3);
    gModule_stack.Add(&ADC4);
    gModule_stack.Add(&ADC5);
    gModule_stack.Add(&ADC6);
    gModule_stack.Add(&ADC7);
    gModule_stack.Add(&TDC1);
    gModule_stack.Add(&TDC2);
    gModule_stack.Add(&TDC3);
    gModule_stack.Add(&TDC4);
    gModule_stack.Add(&QDC1);
    gModule_stack.Add(&QDC2);
    gModule_stack.Add(&QDC3);
    
    //a,b,c,d in order of moving downstream from target.
    for(unsigned int i=0;i<SI_NUM;i++){
      static char si_idx= 'a';
      si_.push_back(RN_S2Detector(Form("si_%c",si_idx),16,16));
      //clusters for identifying matching front and back segments   
      si_cluster_.push_back(RN_S2Cluster(Form("si_cluster_%c",si_idx),16));    
      si_idx++; 
    }
    
    for(unsigned int i=0;i<TRIGBIT_NUM;i++){
      triggerbit.push_back(RN_TriggerBit(Form("triggerbit_%d",i)));
    }
    //neutron detectors - name - pos grid channel num - slot num
    for(unsigned int i=0;i<NEUTNUM;i++){
      neut.push_back(RN_NeutDetector(Form("neut%d",i)));
    }    
    for(unsigned int i=0; i<NAI_NUM;i++){
      nai.push_back(RN_NaIDetector(Form("nai_%d",i)));
    }      
   
    RN_RootSet = 1;
    return RN_RootSet;
  }
  /********************************************************/
  
  void LoadVariableFile(const std::string& f){
    gVariableMap.LoadParams(f);
  }
 
  void SetRootOutputFile(const std::string& filename){
    
  if(gRootFile){
    std::cout<<"root output file already open, please write out and close first"<<std::endl;
    return ;
  }
  gRootFile=new TFile(filename.c_str(),"RECREATE");
  }
  
  void SetRootOutputFileAndTree(const std::string& filename,const std::string& treename){
    if(gRootFile){
      std::cout<<"root output file already open, please write out and close first"<<std::endl;
      return ;
    }
    
    gRootFile=new TFile(filename.c_str(),"RECREATE");
    gNewTree=new TTree(treename.c_str(),treename.c_str());
  }
  
  
  //Apply any calibrations loaded into the variable map gVariableMap 
  
  void SetCalibrations(RN_VariableMap &VarMap){
    gPrimaryReaction.SetCalibrations(VarMap);// beam energy etc
    
    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){
      (*it).SetCalibrations(VarMap);
    }
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      (*it).SetCalibrations(VarMap);
    }
    for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++){
      (*it).SetCalibrations(VarMap);
    }
    rftime.SetCalibrations(VarMap);
    ic.SetCalibrations(VarMap);
    for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
      (*it).SetCalibrations(VarMap);
    }
  }
}
#endif


