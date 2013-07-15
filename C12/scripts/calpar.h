//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jul 14 12:25:43 2013 by ROOT version 5.34/03
// from TTree DataTree/DataTree
// found on file: run_all_nosplit.root
//////////////////////////////////////////////////////////

#ifndef calpar_h
#define calpar_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.
#include "./RN_Unpack2Root.hpp"

// Fixed size dimensions of array or collections stored in the TTree if any.

class calpar : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   RN_module       *ADC1;
   RN_module       *ADC2;
   RN_module       *ADC3;
   RN_module       *ADC4;
   RN_module       *TDC1;
   RN_module       *TDC2;
   RN_module       *QDC1;

   // List of branches
   TBranch        *b_ADC1;   //!
   TBranch        *b_ADC2;   //!
   TBranch        *b_ADC3;   //!
   TBranch        *b_ADC4;   //!
   TBranch        *b_TDC1;   //!
   TBranch        *b_TDC2;   //!
   TBranch        *b_QDC1;   //!

   calpar(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~calpar() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(calpar,0);
};

#endif

#ifdef calpar_cxx
void calpar::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   ADC1 = 0;
   ADC2 = 0;
   ADC3 = 0;
   ADC4 = 0;
   TDC1 = 0;
   TDC2 = 0;
   QDC1 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ADC1", &ADC1, &b_ADC1);
   fChain->SetBranchAddress("ADC2", &ADC2, &b_ADC2);
   fChain->SetBranchAddress("ADC3", &ADC3, &b_ADC3);
   fChain->SetBranchAddress("ADC4", &ADC4, &b_ADC4);
   fChain->SetBranchAddress("TDC1", &TDC1, &b_TDC1);
   fChain->SetBranchAddress("TDC2", &TDC2, &b_TDC2);
   fChain->SetBranchAddress("QDC1", &QDC1, &b_QDC1);
}

Bool_t calpar::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef calpar_cxx
