/***************************************************************
//Class: Si_NEUT_IC_NAI_Analyzer
//
//Author:Sean Kuvin
//
//Si_NEUT_IC_Analyzer is added to the analyzer list just as S2_Analyzer
//is except now we are looking at Silicon detector parameters,Ion 
//Chamber parameters and Neutron Detector parameters.  
//The results of sorting through
//the Si_Analyzer,the IC_analyzer,S2_IC_Analyzer, and the NeutAnalyzer 
//are necessary for proper function of this class. 
//Therefore, all 4 analyzers must be 
//added to the analyzer list and they all must be
// added before this analyzer is added
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
