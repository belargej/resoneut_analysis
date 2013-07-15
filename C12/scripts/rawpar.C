#define rawpar_cxx
// The class definition in rawpar.h has been generated automatically
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
// Root > T->Process("rawpar.C")
// Root > T->Process("rawpar.C","some options")
// Root > T->Process("rawpar.C+")
//

#include "rawpar.h"
#include <TH2.h>
#include <TStyle.h>
#include "RN_NeutDetectorArray.hpp"
#include "RN_SiliconDetectors.hpp"
#include "RN_IonChamber.hpp"
#include "RN_VariableMap.hpp"

TFile newfile("neutrons2.root","RECREATE");
TTree newtree("rawpar","rawpar");

RN_NeutDetector neut0("neut0",4,0);
RN_NeutDetector neut1("neut1",4,1);
RN_NeutDetector neut2("neut2",4,2);
RN_NeutDetector neut3("neut3",4,3);
RN_IonChamber ic("ion_chamber");
RN_S2Detector si_a("si_a",16,16);
RN_BaseDetector rftime("rftime",5);



void rawpar::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
  newtree.Branch(Form("%s",neut0.Name().c_str()),"RN_NeutDetector",&neut0,32000,0);
  newtree.Branch(Form("%s",neut1.Name().c_str()),"RN_NeutDetector",&neut1,32000,0);
  newtree.Branch(Form("%s",neut2.Name().c_str()),"RN_NeutDetector",&neut2,32000,0);
  newtree.Branch(Form("%s",neut3.Name().c_str()),"RN_NeutDetector",&neut3,32000,0);
  newtree.Branch(Form("%s",si_a.Name().c_str()),"RN_S2Detector",&si_a,32000,0);
  newtree.Branch(Form("%s",ic.Name().c_str()),"RN_IonChamber",&ic,32000,0);
  newtree.Branch(Form("%s",rftime.Name().c_str()),"RN_BaseDetector",&rftime,32000,0);
   TString option = GetOption();

  
  
  
   si_a.front.SetELimits(60,3000);
   si_a.back.SetELimits(60,3000);
  


}

void rawpar::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t rawpar::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either rawpar::GetEntry() or TBranch::GetEntry()
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

  neut0.Reset();
  neut1.Reset();
  neut2.Reset();
  neut3.Reset();
  si_a.Reset();
  ic.Reset();
  rftime.Reset();
  GetEntry(entry);
   if(QDC1->fCh[3]>0)neut0.InsertHit(QDC1->fCh[3],QDC1->fCh[19]);
   if(QDC1->fCh[4]>0)neut1.InsertHit(QDC1->fCh[4],QDC1->fCh[20]);
   if(QDC1->fCh[5]>0)neut2.InsertHit(QDC1->fCh[5],QDC1->fCh[21]);
   if(QDC1->fCh[6]>0)neut3.InsertHit(QDC1->fCh[6],QDC1->fCh[22]);
  for(int i=0;i<16;i++){
    if(ADC1->fCh[i]>0)si_a.front.InsertHit(ADC1->fCh[i],0,i);
    if(ADC1->fCh[i+16]>0)si_a.back.InsertHit(ADC1->fCh[i+16],0,i);
  }
   if(TDC1->fCh[16]>0)rftime.InsertHit(0,TDC1->fCh[16],0);
  
   si_a.ApplyCalibrations();
  



  if(entry%30000==0)std::cout<<entry<<std::endl;
  newtree.Fill();
   return kTRUE;
}

void rawpar::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void rawpar::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  newtree.Write();
  newfile.Close();
  
}
