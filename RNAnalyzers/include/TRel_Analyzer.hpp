/***************************************************************
//Class: TRel_Analyzer
//
//Author:Sean Kuvin
//
//TRel_Analyzer is added to the analyzer list
//to look at the timing difference between different 
//detectors
********************************************************************/

#ifndef _TREL_ANALYZER_H
#define _TREL_ANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"

namespace coinc{

  class TRel_Analyzer:public RN_Analyzer{
  private:
    Double32_t fSiaRFTrel;
    Double32_t fSibRFTrel;
    Double32_t fICRFTrel;
    Double32_t fNeutRFTrel;
    Double32_t fNaIRFTrel;
    Bool_t fCheckSiaIC; 
    Bool_t fCheckSibIC; 
    Bool_t fCheckICNeut;
    Bool_t fCheckNaINeut;
    Int_t fRequireSiaIC;
    Int_t fRequireSibIC;
    Int_t fRequireICNeut;
    Int_t fRequireNaINeut;


  public:
    
    TRel_Analyzer();
    virtual ~TRel_Analyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    void RequireSiaICTrel();
    void RequireSibICTRel();
    void RequireICNeutTRel();
    void RequireNaINeutTRel();
    
    ClassDef(TRel_Analyzer,0);

  };

  void LoadTRelGates(const std::string & filename);
 

 
}


#endif
