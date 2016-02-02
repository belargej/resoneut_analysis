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
  RN_SiArray si_array("si_array",SI_NUM);
  RN_NaICollection nai;	 
  RN_RFTime rftime("rftime");
  //  RN_MCPDetector MCP("MCP");
  RN_IonChamber ic("ic");		     
  RN_NaIArray nai_array("nai_array",20);  

  TFile * gRootFile;
  TTree * gNewTree;

  RN_VariableMap gVariableMap("Variable_Map");
  RN_MassTable gMassTable("Mass_Table");
  RN_Analyzer gMainAnalyzer("Main_Analyzer");
  RN_Analyzer_Stack gAnalyzer_stack("Analyzer_Stack");
  RN_Module_Stack gModule_stack("Module_Stack");
  RN_Parameter_Stack gParameter_stack("Parameter_Stack");
  RN_ReactionInfo gReactionInfo("ReactionInfo");
  // RN_ReactionInfo_Stack gReaction_Stack("ReactionInfo_Stack");
  
  int RN_RootSet(0);
}


namespace global{
  TRandom3 myRnd(0);
  //TRandom3 myRnd;
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
CAEN_TDC TDC0("TDC0",9);
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
    }
    gModule_stack.ClearStack();
    gAnalyzer_stack.ClearStack();
    gParameter_stack.ClearStack();
    //gReactionInfo_stack.ClearStack();
    gVariableMap.ClearParams();
    gReactionInfo.Clear();



    //add the modules declared above to the stack...this should match
    //daqconfig for the runs you are analyzing.
    
    //gModule_stack.Add(&ADC1);
    gModule_stack.Add(&ADC2);
    gModule_stack.Add(&ADC3);
    gModule_stack.Add(&ADC4);
    gModule_stack.Add(&ADC5);
    gModule_stack.Add(&ADC6);
    gModule_stack.Add(&ADC7);
    gModule_stack.Add(&TDC0);
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
    
    triggerbit.push_back(RN_TriggerBit("sibBit"));
    triggerbit.push_back(RN_TriggerBit("icdsBit"));
    triggerbit.push_back(RN_TriggerBit("siaBit"));


    //neutron detectors - name - pos grid channel num - slot num
    for(unsigned int i=0;i<NEUTNUM;i++){
      neut.push_back(RN_NeutDetector(Form("neut%d",i),4,i+1));
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
    std::cout << "> Loading File " << f << std::endl;
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
  
  void ResetRNROOTGlobals(){
    gReactionInfo.Reset();
    gModule_stack.Reset();
    ResetRNROOTDetectors();

  }

  
  void ResetRNROOTDetectors(){
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

    si_array.Reset();
    rftime.Reset();
    ic.Reset();
    
    for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
      (*it).Reset();
    }

    nai_array.Reset();

    for(RN_TriggerBitCollectionRef it=triggerbit.begin();it!=triggerbit.end();it++){
      (*it).Reset();
    }
    
   
    


  }

  
  void SetCalibrations(RN_VariableMap &VarMap){
    gReactionInfo.SetCalibrations(VarMap);// beam energy etc
    
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
    si_array.SetCalibrations(VarMap);
    for(RN_NaICollectionRef it=nai.begin();it!=nai.end();it++){
      (*it).SetCalibrations(VarMap);
    }

    for(RN_TriggerBitCollectionRef it=triggerbit.begin();it!=triggerbit.end();it++){
      (*it).SetCalibrations(VarMap);
    }
    
    nai_array.SetCalibrations(VarMap);


  }
}
#endif


