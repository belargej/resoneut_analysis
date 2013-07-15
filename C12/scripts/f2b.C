#define f2b_cxx
// The class definition in f2b.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("f2b.C")
// Root > T->Process("f2b.C","some options")
// Root > T->Process("f2b.C+")
//

#include "f2b.h"
#include <TH2.h>
#include <TStyle.h>
#include "RN_NeutDetectorArray.hpp"
#include "RN_SiliconDetectors.hpp"
#include "RN_IonChamber.hpp"
#include "RN_VariableMap.hpp"

RN_S2Detector si_a("si_a",16,16);
RN_S2Calibrator s2(16,"si_a.front",60,3000);
RN_VariableMap VMap();
VMap.LoadParams("si_a_b2f.in");
si_a.SetCalibrations(VMap);

void f2b::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   TString option = GetOption();

   si_a.front.SetELimits(60,3000);
   si_a.back.SetELimits(60,3000);
   RN_VariableMap DetVar;
   DetVar.LoadParams("si_a_b2f.in");
   si_a.SetCalibrations(DetVar);


}

void f2b::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t f2b::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either f2b::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  si_a.Reset();
 
  GetEntry(entry);
  for(int i=0;i<16;i++){
    if(ADC1->fCh[i]>0)si_a.front.InsertHit(ADC1->fCh[i],0,i);
    if(ADC1->fCh[i+16]>0)si_a.back.InsertHit(ADC1->fCh[i+16],0,i);
  }
  si_a.ApplyCalibrations();

  if(si_a.back.fMult>0&& si_a.back.fChlist[0]==7){
    if(si_a.front.fMult>0)
      s2.AddHit(si_a.back.fE[0],si_a.front.fE[0],si_a.front.fChlist[0]);
  }

  if(entry%30000==0)std::cout<<entry<<std::endl;
   return kTRUE;
}

void f2b::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void f2b::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  s2.PerformFit();
  s2.PrintCoefficients("si_a_f2b.in");
  
}
