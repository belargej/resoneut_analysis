/*************************************************************************
Class: RN_Analyzer
Author: Sean Kuvin

Analyzer class follows the form of a simple MakeClass designed to
read the tree structure of data extracted by the Unpack2Root Method.
Approach this class in two ways,first by overriding the Begin(),Process(), 
and Terminate() methods to create, fill and write histograms or new
trees. The second by instantiating an object of RN_Analyzer and calling the
GetDetectorEntry() function   
from a Loop function in a different application. 
Add user analyzers to the "analyzer" list(which is global in RN_Root) 

*******************************************************************/

#ifndef _RN_ANALYZER_CXX
#define _RN_ANALYZER_CXX

//RN_Analyzer headers.
#include "RN_Analyzer.hpp"
#include "RN_Root.hpp"
#include "RN_Unpack2Root.hpp"
#include "RN_Module.hpp"

using namespace std;
using namespace unpacker;            

namespace silicon{
  double prot_E(0);
  double prot_dE(0);
  double prot_theta(0);
  double rel_angle(0);
  double rel_transverse(0);
  double rel_z(0);
  double target_z[2] ={0};
}

namespace ionchamber{
  Double32_t IC_TotalE(0);
  Double32_t IC_ELoss(0);

}

namespace coinc{
  Double32_t sia_neut_trel(0);
  Double32_t sia_ic_trel(0);
  Double32_t sib_neut_trel(0);
  Double32_t sib_ic_trel(0);
  Double32_t sia_rf_trel(0);
  Double32_t sib_rf_trel(0);
  Double32_t ic_rf_trel(0);
  Double32_t neut_rf_trel(0);
  Double32_t neut_nai_trel(0);
  Double32_t nai_rf_trel(0);
}


RN_Analyzer::RN_Analyzer(const std::string&a,const std::string &b):RN_BaseClass(a.c_str(),b.c_str())
{

}

void RN_Analyzer::Init(TString rootfilename)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed)

  if(!RN_RootSet)
    RN_RootInit();
  
  
  fChain=new TChain("DataTree");
  fChain->Add(rootfilename);

  if(fChain->GetBranch("Event"))
    fChain->SetBranchAddress("Event",&Event, &b_Event);
  else
    std::cout<<"No Event Branch Present, check in sorting code"<<std::endl;

  caen_stack.SetBranches(fChain);
  mesy_stack.SetBranches(fChain);
  
}

bool RN_Analyzer::Begin(){
  
  std::cout<<"Total Entries in Chain: "<<TotEntries()<<std::endl;
  return 1;
}

void RN_Analyzer::Loop(Long64_t start,Long64_t evnum){
  if(!fChain){
    std::cout<<"fChain not initialized, no root files added to Analyzer\n";
    return;  	
  }
  
  Long64_t totentries= TotEntries();    
  if (start!=0&&evnum!=0)
    if(start+evnum<totentries)
      totentries=start+evnum;
  
  Begin();
  TIter next(analyzers); // point to analyzers added to the list
  int fillbit = 1;
  //loop over all analyzer Begin() 's;
  
  while(RN_Analyzer * obj =  (RN_Analyzer*)next()){
    obj->Begin();
  }
  
  next.Reset();
  
  for (Long64_t i=start;i<totentries;i++){
    fillbit = 1; //1 means fill histograms
    ResetGlobals();
    //loop over all analyzer ResetGlobals() 's
    while(RN_Analyzer * obj = (RN_Analyzer*)next()){
      obj->ResetGlobals();
    }
    next.Reset();



    //extract from the tree all of the module parameters
    if(!GetEntry(i)){
      continue;
      
    }
    
    
    if(i%30000==0)std::cout<<i<<std::endl;
    
    GetDetectorEntry();
    
    //loop over all analyzer Process() 's
    Process();
    while(RN_Analyzer * obj = (RN_Analyzer*)next()){
      if(!obj->Process()){
	//0 means process returned 0
	//do not fill histograms
	//go ahead and break out of event

	fillbit=0;
       	break;
      }
    }
    next.Reset();
    if(fillbit){
      while(RN_Analyzer * obj = (RN_Analyzer*)next()){
	obj->ProcessFill();
      }
    }
    
    next.Reset();

  }
 
  next.Reset();

  //execute the TerminateIfLast() of the last analyzer in the list. 
  
  if(RN_Analyzer * obj =  (RN_Analyzer*)analyzers->Last())
    obj->TerminateIfLast();

  while(RN_Analyzer * obj = (RN_Analyzer*)next()){
    obj->Terminate();
  }
}


RN_Analyzer::~RN_Analyzer(){

}


void RN_Analyzer::ResetGlobals(){
  //silicon telescope parameters
  silicon::prot_E = 0;
  silicon::prot_dE = 0;
  silicon::prot_theta = 0;  
  silicon::rel_angle = 0;
  silicon::rel_z = 0;
  silicon::rel_transverse = 0;
  silicon::target_z[0] = 0;
  silicon::target_z[1] = 0;
    
  ionchamber::IC_ELoss = 0;
  ionchamber::IC_TotalE = 0;

  coinc::sia_neut_trel = 0;
  coinc::sia_ic_trel = 0;
  coinc::sib_neut_trel = 0;
  coinc::sib_ic_trel = 0;
  coinc::sia_rf_trel = 0;
  coinc::sib_rf_trel = 0;
  coinc::ic_rf_trel = 0;
  coinc::neut_rf_trel = 0;
  coinc::neut_nai_trel = 0;
  coinc::nai_rf_trel = 0;


}

bool RN_Analyzer::Process(){

  
  
  //DE-E Telescope calculations
  if(si_cluster_[1].fMult>0&&si_cluster_[0].fMult>0){
    silicon::prot_dE=si_cluster_[0].fE[0];
    silicon::prot_E=si_cluster_[1].fE[0]+silicon::prot_dE;
    silicon::prot_theta=si_cluster_[1].fPos[0].Theta()*(180.0 / TMath::Pi());
    silicon::rel_transverse = (si_cluster_[0].fPos[0].Perp()-si_cluster_[1].fPos[0].Perp());
    silicon::rel_z = (si_cluster_[0].fPos[0] - si_cluster_[1].fPos[0]).Z();
    silicon::rel_angle = (si_cluster_[0].fPos[0] - si_cluster_[1].fPos[0]).Theta() * (180.0 / TMath::Pi());
    silicon::target_z[0] = (si_cluster_[1].fPos[0]-global::target_pos).Perp()/ tan ( silicon::rel_angle * 3.14 / 180); 
    silicon::target_z[1] = (si_cluster_[0].fPos[0]-global::target_pos).Perp() / tan ( silicon::rel_angle * 3.14 / 180); 
    
    if(Narray.fT_mult>0){
      coinc::sia_neut_trel = si_cluster_[0].fT[0] - Narray.fT_first;  
      coinc::sib_neut_trel = si_cluster_[1].fT[0] - Narray.fT_first;
    }
    coinc::sia_ic_trel = si_cluster_[0].fT[0] - ic.T();
    coinc::sib_ic_trel = si_cluster_[1].fT[0] - ic.T();
    coinc::sia_rf_trel = si_cluster_[0].fT[0] - rftime[0].T() ;
    coinc::sib_rf_trel = si_cluster_[1].fT[0] - rftime[0].T();

  }
  coinc::ic_rf_trel = ic.T()-rftime[0].T();
  if(nai_array.fT[0])
    coinc::nai_rf_trel = nai_array.fT[0] - rftime[0].T();
  
  if(Narray.fT_mult>0){
    coinc::neut_rf_trel = Narray.fT_first - rftime[0].T();
    if(nai_array.fT[0])
      coinc::neut_nai_trel = Narray.fT_first - nai_array.fT[0];
  }

  

//IonChamber Calculations
  ionchamber::IC_ELoss = ic.fdE;
  ionchamber::IC_TotalE = ic.fE + ionchamber::IC_ELoss;
  
  
  return true;
}

bool RN_Analyzer::ProcessFill(){
  return true;
}


#endif
