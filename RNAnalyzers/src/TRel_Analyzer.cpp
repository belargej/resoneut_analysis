/***************************************************************
//Class: TRel_Analyzer
//
//Author:Sean Kuvin
//
//TRel_Analyzer is added to the analyzer list
//to look at the timing difference between different 
//detectors
********************************************************************/
#ifndef _TREL_ANALYZER_CXX
#define _TREL_ANALYZER_CXX

#include "TRel_Analyzer.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;

namespace coinc{
  
  TCutG* gCutSiaIC;
  TCutG* gCutSibIC;
  TCutG* gCutICNeut;
  TCutG* gCutNaINeut;

  TH2D *hntrel_v_ictrel;
  TH2D *hntrel_v_naitrel;
  TH2D *hntrel_v_siatrel;
  TH2D *hntrel_v_sibtrel;
  TH2D *hsiatrel_v_ictrel;
  TH2D *hsibtrel_v_ictrel;

  TRel_Analyzer::TRel_Analyzer():fSiaRFTrel(0),
				 fSibRFTrel(0),
				 fICRFTrel(0),
				 fNeutRFTrel(0),
				 fNaIRFTrel(0),
				 fCheckSiaIC(0),
				 fCheckSibIC(0),
				 fCheckICNeut(0),
				 fCheckNaINeut(0),
				 fRequireSiaIC(0),
				 fRequireSibIC(0),
				 fRequireICNeut(0),
				 fRequireNaINeut(0)
  {
    
  }
  
  void TRel_Analyzer::Reset(){
    fCheckSiaIC = 0;
    fCheckSibIC = 0;
    fCheckICNeut = 0;
    fCheckNaINeut = 0;
  
    fSiaRFTrel = 0;
    fSibRFTrel = 0;
    fICRFTrel  = 0;
    fNeutRFTrel = 0;
    fNaIRFTrel = 0;


  }
  
  bool TRel_Analyzer::Begin(){   
    
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }

    //create directory structure
    if(!fgRootFile->GetDirectory("coinc"))
      fgRootFile->mkdir("coinc");
    fgRootFile->cd("coinc");
    gDirectory->mkdir("TRel");
   
 
    //create histograms
    fgRootFile->cd("coinc/TRel");
    hntrel_v_ictrel = new  TH2D("hntrel_v_ictrel","hntrel_v_ictrel;ictrel_rf;ntrel_rf",1024,-2048,2047,1024,-2048,2047);
    hntrel_v_naitrel = new  TH2D("hntrel_v_naitrel","hntrel_v_naitrel;naitrel_rf;ntrel_rf",1024,-2048,2047,1024,-2048,2047);
    hntrel_v_siatrel = new  TH2D("hntrel_v_siatrel","hntrel_v_siatrel;siatrel_rf;ntrel_rf",1024,-2048,2047,1024,-2048,2047);
    hntrel_v_sibtrel = new  TH2D("hntrel_v_sibtrel","hntrel_v_sibtrel;sibtrel_rf;ntrel_rf",1024,-2048,2047,1024,-2048,2047);
    hsiatrel_v_ictrel = new  TH2D("hsiatrel_v_ictrel","hsiatrel_v_ictrel;ictrel_rf;siatrel_rf",1024,-2048,2047,1024,-2048,2047);
    hsibtrel_v_ictrel = new  TH2D("hsibtrel_v_ictrel","hsibtrel_v_ictrel;ictrel_rf;sibtrel_rf",1024,-2048,2047,1024,-2048,2047);

    
    return 1;
  }

  bool TRel_Analyzer::Process(){
    //get TRel variables  the + sign comes because TWrapped already flips the sign of the delayed times.
    fSiaRFTrel = si_array.T_A() - rftime.TMod2();
    fSibRFTrel = si_array.T_B() - rftime.TMod2();
    fICRFTrel = ic.T() - rftime.TMod2();
    fNeutRFTrel = Narray.T() - rftime.TMod2();
    fNaIRFTrel = nai_array.T() - rftime.TMod2();
    
    //check variables across the global cuts
    fCheckSiaIC = (gCutSiaIC && gCutSiaIC->IsInside(fICRFTrel,fSiaRFTrel));
    fCheckSibIC = (gCutSibIC && gCutSibIC->IsInside(fICRFTrel,fSibRFTrel));
    fCheckICNeut = (gCutICNeut && gCutICNeut->IsInside(fICRFTrel,fNeutRFTrel));
    fCheckNaINeut = (gCutNaINeut && gCutNaINeut->IsInside(fNaIRFTrel,fNeutRFTrel));

    
    //if filter bits are set, compare with results from the cuts above
    if(fRequireSiaIC && !fCheckSiaIC){
      return 0;
    }
    if(fRequireSibIC && !fCheckSibIC){
      return 0;
    }
    if(fRequireICNeut && !fCheckICNeut){
      return 0;
    }
   if(fRequireNaINeut && !fCheckNaINeut){
      return 0;
    }

    
    return 1 ; 
  }    
  
  bool TRel_Analyzer::ProcessFill(){
    
    hntrel_v_ictrel->Fill(fICRFTrel,fNeutRFTrel);
    hntrel_v_naitrel->Fill(fNaIRFTrel,fNeutRFTrel);
    hntrel_v_siatrel->Fill(fSiaRFTrel,fNeutRFTrel);
    hntrel_v_sibtrel->Fill(fSibRFTrel,fNeutRFTrel);
    hsiatrel_v_ictrel->Fill(fICRFTrel,fSiaRFTrel);
    hsibtrel_v_ictrel->Fill(fICRFTrel,fSibRFTrel);
  
 
    return 1;
  }


  bool TRel_Analyzer::Terminate(){
    
    return 1;    
  }
  

  void TRel_Analyzer::Clear(){
    
  }

  void TRel_Analyzer::RequireSiaICTrel(){
    fRequireSiaIC = 1;
    
  }
  void  TRel_Analyzer::RequireSibICTRel(){
    fRequireSibIC= 1;
  
  }
  void  TRel_Analyzer::RequireICNeutTRel(){
    fRequireICNeut= 1;
  
  }
  void  TRel_Analyzer::RequireNaINeutTRel(){
    fRequireNaINeut= 1;
  
  }



  void LoadTRelGates(const std::string & input){
    TFile in(input.c_str());    
    if(in.Get("gCutSiaIC") && !gCutSiaIC)
      gCutSiaIC=new TCutG(*(TCutG*)in.Get("gCutSiaIC"));
    if(in.Get("gCutSibIC") && !gCutSibIC)
      gCutSibIC=new TCutG(*(TCutG*)in.Get("gCutSibIC"));
    if(in.Get("gCutICNeut") && !gCutICNeut)
      gCutICNeut=new TCutG(*(TCutG*)in.Get("gCutICNeut"));
    if(in.Get("gCutNaINeut") && !gCutNaINeut)
      gCutNaINeut=new TCutG(*(TCutG*)in.Get("gCutNaINeut"));


    in.Close();
  }
  
  
}
#endif
