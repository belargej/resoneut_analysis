/***************************************************************/
//Class: NeutAnalyzer
//
//Author:Sean Kuvin
//
//NeutAnalyzer is added to the analyzer list to sort neutron detector
//parameters.  Make histograms, load cuts, and check cuts 
//using parameters related to the neutron detector pulse shape
// discrimination spectra.  Also, neutron timing.
/********************************************************************/
#ifndef _PSD_ANALYZER_CXX
#define _PSD_ANALYZER_CXX

#include "PSD_Analyzer.hpp"
#include "RN_Root.hpp"
using namespace RNROOT;


namespace psd{
  
  //declare Gates
  TCutG* n0_neuts;
  TCutG* n1_neuts;
  TCutG* n2_neuts;
  TCutG* n3_neuts;
  TCutG* n4_neuts;
  TCutG* n5_neuts;
  TCutG* n6_neuts;
  TCutG* n7_neuts;
  TCutG* n8_neuts;
  TCutG* n9_neuts;

  TCutG* n0_gammas;
  TCutG* n1_gammas;
  TCutG* n2_gammas;
  TCutG* n3_gammas;
  TCutG* n4_gammas;
  TCutG* n5_gammas;
  TCutG* n6_gammas;
  TCutG* n7_gammas;
  TCutG* n8_gammas;
  TCutG* n9_gammas;

  TCutG* n0_neuts_raw;
  TCutG* n1_neuts_raw;
  TCutG* n2_neuts_raw;
  TCutG* n3_neuts_raw;
  TCutG* n4_neuts_raw;
  TCutG* n5_neuts_raw;
  TCutG* n6_neuts_raw;
  TCutG* n7_neuts_raw;
  TCutG* n8_neuts_raw;
  TCutG* n9_neuts_raw;

  TCutG* n0_gammas_raw;
  TCutG* n1_gammas_raw;
  TCutG* n2_gammas_raw;
  TCutG* n3_gammas_raw;
  TCutG* n4_gammas_raw;
  TCutG* n5_gammas_raw;
  TCutG* n6_gammas_raw;
  TCutG* n7_gammas_raw;
  TCutG* n8_gammas_raw;
  TCutG* n9_gammas_raw;

  TCutG* n0_evt;
  TCutG* n1_evt;
  TCutG* n2_evt;
  TCutG* n3_evt;
  TCutG* n4_evt;
  TCutG* n5_evt;
  TCutG* n6_evt;
  TCutG* n7_evt;
  TCutG* n8_evt;
  TCutG* n9_evt;


  int neutcheck[NEUTNUM];
  int evtcheck[NEUTNUM];
  int neut_sansgamma[NEUTNUM];
  int rawneutcheck[NEUTNUM];
  int rawneut_sansrawgamma[NEUTNUM];
  int gammacheck[NEUTNUM];
  int rawgammacheck[NEUTNUM];
  int neut_orcheck(0);
  int rawneut_orcheck(0);
  int neut_sansgamma_orcheck(0);
  int rawneut_sansrawgamma_orcheck(0);
  int rawgamma_orcheck(0);

  int _require_neut_orcheck(0);
  int _require_rawneut_orcheck(0);
  int _require_neut_sansgamma_orcheck(0);
  int _require_rawneut_sansrawgamma_orcheck(0);
  int _require_rawgamma_orcheck(0);
  std::vector<int>OmitNeutDet;


  //declare histograms here

  //n_detmult
  TH1D *h_ndetMult;
  TH1D *h_ndetMult_ngated;

  //neut_psd
  TH2D *hPSDq_n[NEUTNUM];
  TH2D *hPSD_n_[NEUTNUM];

  //neut_time
  TH2D *hQvT_n[NEUTNUM];
  TH2D *hQvT_ngated[NEUTNUM];
  
  TH2D *hPSDq_n_evtgated[NEUTNUM];


  NeutAnalyzer::NeutAnalyzer():RN_Analyzer("NeutAnalyzer","NeutAnalyzer")
  {
  }
  


  bool NeutAnalyzer::Begin(){

    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    //create directory structure
    fgRootFile->mkdir("neut");
    fgRootFile->cd("neut");
    gDirectory->mkdir("PSD");
    gDirectory->cd("PSD");
    gDirectory->mkdir("raw");
    gDirectory->mkdir("cal");
    
    fgRootFile->cd("neut");
    gDirectory->mkdir("TRel");
    gDirectory->mkdir("mult");
  

    //create histograms
    fgRootFile->cd("neut/mult");
    h_ndetMult=new TH1D("h_ndetmult","nmult;mult",NEUTNUM+1,0,NEUTNUM);
    h_ndetMult_ngated=new TH1D("h_ndetmult_ngated","nmult_ngated;mult",NEUTNUM+1,0,NEUTNUM);
    

    for(unsigned int i=0;i<NEUTNUM;i++){  
      fgRootFile->cd("neut/TRel");
      hQvT_n[i]=new TH2D(Form("hQvT_n%d",i),Form("hQvT_n%d;T;Q",i),128,0,128,1024,0,4095);
      hQvT_ngated[i]=new TH2D(Form("hQvT_n%d_ngated",i),Form("hQvT_n%d_ngated;T;Q",i),128,0,128,1024,0,4095);
      fgRootFile->cd("neut/PSD/raw");
      hPSDq_n[i]=new TH2D(Form("hPSDq_n%d",i),Form("hPSDq_n%d",i),1024,0,4096,1024,0,4096);
      hPSDq_n_evtgated[i]=new TH2D(Form("hPSDq_n%d_evtgated",i),Form("hPSDq_n%d_evtgated",i),1024,0,4096,1024,0,4096);
  
      fgRootFile->cd("neut/PSD/cal");
      hPSD_n_[i]=new TH2D(Form("hPSD_neut%d",i),Form("hPSD_neut%d",i),256,-2.,2.,1024,50,4096);
    } 
    fgRootFile->cd();

    
    return 1;
    
  }
  void NeutAnalyzer::Reset(){
    for(unsigned int i=0;i<NEUTNUM;i++){
      neutcheck[i]=0;
      neut_sansgamma[i]=0;
      rawneut_sansrawgamma[i]=0;
      rawneutcheck[i]=0;
      gammacheck[i]=0;
      rawgammacheck[i]=0;
      evtcheck[i]=0;
    }
    neut_orcheck = 0;
    rawneut_orcheck = 0;
    neut_sansgamma_orcheck = 0;
    rawneut_sansrawgamma_orcheck = 0;
    rawgamma_orcheck = 0;
    
  }
  

  bool NeutAnalyzer::Process(){ 
 
    
    neutcheck[0] = (n0_neuts && n0_neuts->IsInside(neut[0].fPSD,neut[0].fQ_long));
    gammacheck[0] = (n0_gammas && n0_gammas->IsInside(neut[0].fPSD,neut[0].fQ_long));
   
		   
    neutcheck[1] = (n1_neuts && n1_neuts->IsInside(neut[1].fPSD,neut[1].fQ_long));
    gammacheck[1] = (n1_gammas && n1_gammas->IsInside(neut[1].fPSD,neut[1].fQ_long));
   

    neutcheck[2] = (n2_neuts && n2_neuts->IsInside(neut[2].fPSD,neut[2].fQ_long));
    gammacheck[2] = (n2_gammas && n2_gammas->IsInside(neut[2].fPSD,neut[2].fQ_long));
    
    neutcheck[3] = (n3_neuts && n3_neuts->IsInside(neut[3].fPSD,neut[3].fQ_long));
    gammacheck[3] =(n3_gammas && n3_gammas->IsInside(neut[3].fPSD,neut[3].fQ_long));
    
    neutcheck[4] = (n4_neuts && n4_neuts->IsInside(neut[4].fPSD,neut[4].fQ_long));
    gammacheck[4] = (n4_gammas && n4_gammas->IsInside(neut[4].fPSD,neut[4].fQ_long));
    
    neutcheck[5] = (n5_neuts && n5_neuts->IsInside(neut[5].fPSD,neut[5].fQ_long));
    gammacheck[5] = (n5_gammas && n5_gammas->IsInside(neut[5].fPSD,neut[5].fQ_long));
    
    neutcheck[6] = (n6_neuts && n6_neuts->IsInside(neut[6].fPSD,neut[6].fQ_long));
    gammacheck[6] = (n6_gammas && n6_gammas->IsInside(neut[6].fPSD,neut[6].fQ_long));
    
    neutcheck[7] = (n7_neuts && n7_neuts->IsInside(neut[7].fPSD,neut[7].fQ_long));
    gammacheck[7]= (n7_gammas && n7_gammas->IsInside(neut[7].fPSD,neut[7].fQ_long));
    
    neutcheck[8] = (n8_neuts && n8_neuts->IsInside(neut[8].fPSD,neut[8].fQ_long));
    gammacheck[8] = (n8_gammas && n8_gammas->IsInside(neut[8].fPSD,neut[8].fQ_long));
    
    neutcheck[9] = (n9_neuts && n9_neuts->IsInside(neut[9].fPSD,neut[9].fQ_long));
    gammacheck[9] =(n9_gammas && n9_gammas->IsInside(neut[9].fPSD,neut[9].fQ_long));
    
    rawneutcheck[0] = (n0_neuts_raw && n0_neuts_raw->IsInside(neut[0].fQ_long,neut[0].fQ_short));
    rawgammacheck[0] = (n0_gammas_raw && n0_gammas_raw->IsInside(neut[0].fQ_long,neut[0].fQ_short));

    rawneutcheck[1] = (n1_neuts_raw &&  n1_neuts_raw->IsInside(neut[1].fQ_long,neut[1].fQ_short));
    rawgammacheck[1] = (n1_gammas_raw && n1_gammas_raw->IsInside(neut[1].fQ_long,neut[1].fQ_short));

    rawneutcheck[2] = (n2_neuts_raw && n2_neuts_raw->IsInside(neut[2].fQ_long,neut[2].fQ_short));
    rawgammacheck[2] = (n2_gammas_raw && n2_gammas_raw->IsInside(neut[2].fQ_long,neut[2].fQ_short));

    rawneutcheck[3] = (n3_neuts_raw && n3_neuts_raw->IsInside(neut[3].fQ_long,neut[3].fQ_short));
    rawgammacheck[3] = (n3_gammas_raw && n3_gammas_raw->IsInside(neut[3].fQ_long,neut[3].fQ_short));

    rawneutcheck[4] = (n4_neuts_raw && n4_neuts_raw->IsInside(neut[4].fQ_long,neut[4].fQ_short));
    rawgammacheck[4] = (n4_gammas_raw && n4_gammas_raw->IsInside(neut[4].fQ_long,neut[4].fQ_short));

    rawneutcheck[5] = (n5_neuts_raw && n5_neuts_raw->IsInside(neut[5].fQ_long,neut[5].fQ_short));
    rawgammacheck[5] = (n5_gammas_raw && n5_gammas_raw->IsInside(neut[5].fQ_long,neut[5].fQ_short));

    rawneutcheck[6] = (n6_neuts_raw && n6_neuts_raw->IsInside(neut[6].fQ_long,neut[6].fQ_short));
    rawgammacheck[6] = (n6_gammas_raw && n6_gammas_raw->IsInside(neut[6].fQ_long,neut[6].fQ_short));

    rawneutcheck[7] = (n7_neuts_raw && n7_neuts_raw->IsInside(neut[7].fQ_long,neut[7].fQ_short));
    rawgammacheck[7] = (n7_gammas_raw && n7_gammas_raw->IsInside(neut[7].fQ_long,neut[7].fQ_short));

    rawneutcheck[8] = (n8_neuts_raw && n8_neuts_raw->IsInside(neut[8].fQ_long,neut[8].fQ_short));
    rawgammacheck[8] = (n8_gammas_raw && n8_gammas_raw->IsInside(neut[8].fQ_long,neut[8].fQ_short));

    rawneutcheck[9] = (n9_neuts_raw && n9_neuts_raw->IsInside(neut[9].fQ_long,neut[9].fQ_short));
    rawgammacheck[9] = (n9_gammas_raw && n9_gammas_raw->IsInside(neut[9].fQ_long,neut[9].fQ_short));


    evtcheck[0] = (n0_evt && n0_evt->IsInside(neut[0].fTrel,neut[0].fQ_long));
    evtcheck[1] = (n1_evt && n1_evt->IsInside(neut[1].fTrel,neut[1].fQ_long));    
    evtcheck[2] = (n2_evt && n2_evt->IsInside(neut[2].fTrel,neut[2].fQ_long));
    evtcheck[3] = (n3_evt && n3_evt->IsInside(neut[3].fTrel,neut[3].fQ_long));
    evtcheck[4] = (n4_evt && n4_evt->IsInside(neut[4].fTrel,neut[4].fQ_long));
    evtcheck[5] = (n5_evt && n5_evt->IsInside(neut[5].fTrel,neut[5].fQ_long));
    evtcheck[6] = (n6_evt && n6_evt->IsInside(neut[6].fTrel,neut[6].fQ_long));
    evtcheck[7] = (n7_evt && n7_evt->IsInside(neut[7].fTrel,neut[7].fQ_long));
    evtcheck[8] = (n8_evt && n8_evt->IsInside(neut[8].fTrel,neut[8].fQ_long));
    evtcheck[9] = (n9_evt && n9_evt->IsInside(neut[9].fTrel,neut[9].fQ_long));
    

    
    for (int i=0;i<10;i++){	    
      neut_sansgamma[i] = neutcheck[i] && !(gammacheck[i]);
      rawneut_sansrawgamma[i] = rawneutcheck[i] && !(rawgammacheck[i]);
      
      //check if this detector is in the omitneut list and leave it out of the orcheck if it is
      int omitneut = std::count(OmitNeutDet.begin(),OmitNeutDet.end(),i);
      if(omitneut)
	continue;
      rawgamma_orcheck = rawgammacheck[i] || rawgamma_orcheck;
      neut_orcheck = neutcheck[i] || neut_orcheck;
      rawneut_orcheck = rawneutcheck[i] || rawneut_orcheck;
      neut_sansgamma_orcheck = neut_sansgamma[i] || neut_sansgamma_orcheck;
      rawneut_sansrawgamma_orcheck = rawneut_sansrawgamma[i] || rawneut_sansrawgamma_orcheck;
    }
    
    if (_require_neut_orcheck &&!neut_orcheck){
      return 0;
    }
    if (_require_rawneut_orcheck &&!rawneut_orcheck){
      return 0;
    }
    if (_require_neut_sansgamma_orcheck &&!neut_sansgamma_orcheck){
      return 0;
    }
    if (_require_rawneut_sansrawgamma_orcheck &&!rawneut_sansrawgamma_orcheck){
      return 0;
   }

    if (_require_rawgamma_orcheck && !rawgamma_orcheck){
      return 0;
    }

		           
    return 1; 
  }

  bool NeutAnalyzer::ProcessFill(){
    
    h_ndetMult->Fill(Narray.fMult);  
    
    for(unsigned int i=0;i<NEUTNUM;i++){
      if(i>=neut.size())
	break;
      hPSD_n_[i]->Fill(neut[i].fPSD,neut[i].fQ_long); 
      hPSDq_n[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);    
    
    hQvT_n[i]->Fill(neut[i].fTrel,neut[i].fQ_long);
    if(rawneut_orcheck){
      h_ndetMult_ngated->Fill(Narray.fMult);
      hQvT_ngated[i]->Fill(neut[i].fTrel,neut[i].fQ_long);
    }
    if(evtcheck[i])
      hPSDq_n_evtgated[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);
    }
    
    return 1;
  }

  
  bool NeutAnalyzer::Terminate(){
    return 1;
  }
  
  bool NeutAnalyzer::TerminateIfLast(){
    fgRootFile->Write();
    fgRootFile->Close();
    return 1;
  }

  void LoadGates(const std::string& input){
    TFile in(input.c_str());    
    if(in.Get("n0_neuts") && !n0_neuts)
      n0_neuts=new TCutG(*(TCutG*)in.Get("n0_neuts"));
    if(in.Get("n1_neuts") && !n1_neuts)
      n1_neuts=new TCutG(*(TCutG*)in.Get("n1_neuts"));
    if(in.Get("n2_neuts") && !n2_neuts)
      n2_neuts=new TCutG(*(TCutG*)in.Get("n2_neuts"));   
    if(in.Get("n3_neuts") && !n3_neuts)
      n3_neuts=new TCutG(*(TCutG*)in.Get("n3_neuts"));
    if(in.Get("n4_neuts") && !n4_neuts)
      n4_neuts=new TCutG(*(TCutG*)in.Get("n4_neuts"));
    if(in.Get("n5_neuts") && !n5_neuts)
      n5_neuts=new TCutG(*(TCutG*)in.Get("n5_neuts"));
    if(in.Get("n6_neuts") && !n6_neuts)
      n6_neuts=new TCutG(*(TCutG*)in.Get("n6_neuts"));   
    if(in.Get("n7_neuts") && !n7_neuts)
      n7_neuts=new TCutG(*(TCutG*)in.Get("n7_neuts"));
    if(in.Get("n8_neuts") && !n8_neuts)
      n8_neuts=new TCutG(*(TCutG*)in.Get("n8_neuts"));
    if(in.Get("n9_neuts") && !n9_neuts)
      n9_neuts=new TCutG(*(TCutG*)in.Get("n9_neuts"));
    
    if(in.Get("n0_gammas") && !n0_gammas)
      n0_gammas=new TCutG(*(TCutG*)in.Get("n0_gammas"));
    if(in.Get("n1_gammas") && !n1_gammas)
      n1_gammas=new TCutG(*(TCutG*)in.Get("n1_gammas"));
    if(in.Get("n2_gammas") && !n2_gammas)
      n2_gammas=new TCutG(*(TCutG*)in.Get("n2_gammas"));   
    if(in.Get("n3_gammas") && !n3_gammas)
      n3_gammas=new TCutG(*(TCutG*)in.Get("n3_gammas"));
    if(in.Get("n4_gammas") && !n4_gammas)
      n4_gammas=new TCutG(*(TCutG*)in.Get("n4_gammas"));
    if(in.Get("n5_gammas") && !n5_gammas)
      n5_gammas=new TCutG(*(TCutG*)in.Get("n5_gammas"));
    if(in.Get("n6_gammas") && !n6_gammas)
      n6_gammas=new TCutG(*(TCutG*)in.Get("n6_gammas"));   
    if(in.Get("n7_gammas") && !n7_gammas)
      n7_gammas=new TCutG(*(TCutG*)in.Get("n7_gammas"));
    if(in.Get("n8_gammas") && !n8_gammas)
      n8_gammas=new TCutG(*(TCutG*)in.Get("n8_gammas"));
    if(in.Get("n9_gammas") && !n9_gammas)
      n9_gammas=new TCutG(*(TCutG*)in.Get("n9_gammas"));
    
    if(in.Get("n0_neuts_raw") && !n0_neuts_raw)
      n0_neuts_raw=new TCutG(*(TCutG*)in.Get("n0_neuts_raw"));
    if(in.Get("n1_neuts_raw") && !n1_neuts_raw)
      n1_neuts_raw=new TCutG(*(TCutG*)in.Get("n1_neuts_raw"));
    if(in.Get("n2_neuts_raw") && !n2_neuts_raw)
      n2_neuts_raw=new TCutG(*(TCutG*)in.Get("n2_neuts_raw"));   
    if(in.Get("n3_neuts_raw") && !n3_neuts_raw)
      n3_neuts_raw=new TCutG(*(TCutG*)in.Get("n3_neuts_raw"));
    if(in.Get("n4_neuts_raw") && !n4_neuts_raw)
      n4_neuts_raw=new TCutG(*(TCutG*)in.Get("n4_neuts_raw"));
    if(in.Get("n5_neuts_raw") && !n5_neuts_raw)
      n5_neuts_raw=new TCutG(*(TCutG*)in.Get("n5_neuts_raw"));
    if(in.Get("n6_neuts_raw") && !n6_neuts_raw)
      n6_neuts_raw=new TCutG(*(TCutG*)in.Get("n6_neuts_raw"));   
    if(in.Get("n7_neuts_raw") && !n7_neuts_raw)
      n7_neuts_raw=new TCutG(*(TCutG*)in.Get("n7_neuts_raw"));
    if(in.Get("n8_neuts_raw") && !n8_neuts_raw)
      n8_neuts_raw=new TCutG(*(TCutG*)in.Get("n8_neuts_raw"));
    if(in.Get("n9_neuts_raw") && !n9_neuts_raw)
      n9_neuts_raw=new TCutG(*(TCutG*)in.Get("n9_neuts_raw"));

    
    if(in.Get("n0_gammas_raw") && !n0_gammas_raw)
      n0_gammas_raw=new TCutG(*(TCutG*)in.Get("n0_gammas_raw"));
    if(in.Get("n1_gammas_raw") && !n1_gammas_raw)
      n1_gammas_raw=new TCutG(*(TCutG*)in.Get("n1_gammas_raw"));
    if(in.Get("n2_gammas_raw") && !n2_gammas_raw)
      n2_gammas_raw=new TCutG(*(TCutG*)in.Get("n2_gammas_raw"));   
    if(in.Get("n3_gammas_raw") && !n3_gammas_raw)
      n3_gammas_raw=new TCutG(*(TCutG*)in.Get("n3_gammas_raw"));
    if(in.Get("n4_gammas_raw") && !n4_gammas_raw)
      n4_gammas_raw=new TCutG(*(TCutG*)in.Get("n4_gammas_raw"));
    if(in.Get("n5_gammas_raw") && !n5_gammas_raw)
      n5_gammas_raw=new TCutG(*(TCutG*)in.Get("n5_gammas_raw"));
    if(in.Get("n6_gammas_raw") && !n6_gammas_raw)
      n6_gammas_raw=new TCutG(*(TCutG*)in.Get("n6_gammas_raw"));   
    if(in.Get("n7_gammas_raw") && !n7_gammas_raw)
      n7_gammas_raw=new TCutG(*(TCutG*)in.Get("n7_gammas_raw"));
    if(in.Get("n8_gammas_raw") && !n8_gammas_raw)
      n8_gammas_raw=new TCutG(*(TCutG*)in.Get("n8_gammas_raw"));
    if(in.Get("n9_gammas_raw") && !n9_gammas_raw)
      n9_gammas_raw=new TCutG(*(TCutG*)in.Get("n9_gammas_raw"));
    
    if(in.Get("n0_evt") && !n0_evt)
      n0_evt=new TCutG(*(TCutG*)in.Get("n0_evt"));
    if(in.Get("n1_evt") && !n1_evt)
      n1_evt=new TCutG(*(TCutG*)in.Get("n1_evt"));
    if(in.Get("n2_evt") && !n2_evt)
      n2_evt=new TCutG(*(TCutG*)in.Get("n2_evt"));   
    if(in.Get("n3_evt") && !n3_evt)
      n3_evt=new TCutG(*(TCutG*)in.Get("n3_evt"));
    if(in.Get("n4_evt") && !n4_evt)
      n4_evt=new TCutG(*(TCutG*)in.Get("n4_evt"));
    if(in.Get("n5_evt") && !n5_evt)
      n5_evt=new TCutG(*(TCutG*)in.Get("n5_evt"));
    if(in.Get("n6_evt") && !n6_evt)
      n6_evt=new TCutG(*(TCutG*)in.Get("n6_evt"));   
    if(in.Get("n7_evt") && !n7_evt)
      n7_evt=new TCutG(*(TCutG*)in.Get("n7_evt"));
    if(in.Get("n8_evt") && !n8_evt)
      n8_evt=new TCutG(*(TCutG*)in.Get("n8_evt"));
    if(in.Get("n9_evt") && !n9_evt)
      n9_evt=new TCutG(*(TCutG*)in.Get("n9_evt"));
    
    
    
  
    in.Close();
  }

  void Require_RawNeut_ORCheck(){
    _require_rawneut_orcheck = 1; 
  }


  void Require_Neut_ORCheck(){
    _require_neut_orcheck = 1;
  }

  void Require_Neut_SansGamma_ORCheck(){
    _require_neut_sansgamma_orcheck = 1;
    
  }
  
  void Require_RawNeut_SansRawGamma_ORCheck(){
    _require_rawneut_sansrawgamma_orcheck = 1;
  }

  void Require_RawGamma_ORCheck(){
    _require_rawgamma_orcheck = 1;
  }
  void ClearGates(){
    if(n0_neuts)delete n0_neuts;
    if(n1_neuts)delete n1_neuts;
    if(n2_neuts)delete n2_neuts;
    if(n3_neuts)delete n3_neuts;
    if(n4_neuts)delete n4_neuts;
    if(n5_neuts)delete n5_neuts;
    if(n6_neuts)delete n6_neuts;
    if(n7_neuts)delete n7_neuts;
    if(n8_neuts)delete n8_neuts;
    if(n9_neuts)delete n9_neuts;
    
    if(n0_gammas)delete n0_gammas;
    if(n1_gammas)delete n1_gammas;
    if(n2_gammas)delete n2_gammas;
    if(n3_gammas)delete n3_gammas;
    if(n4_gammas)delete n4_gammas;
    if(n5_gammas)delete n5_gammas;
    if(n6_gammas)delete n6_gammas;
    if(n7_gammas)delete n7_gammas;
    if(n8_gammas)delete n8_gammas;
    if(n9_gammas)delete n9_gammas;
    
    if(n0_neuts_raw)delete n0_neuts_raw;
    if(n1_neuts_raw)delete n1_neuts_raw;
    if(n2_neuts_raw)delete n2_neuts_raw;
    if(n3_neuts_raw)delete n3_neuts_raw;
    if(n4_neuts_raw)delete n4_neuts_raw;
    if(n5_neuts_raw)delete n5_neuts_raw;
    if(n6_neuts_raw)delete n6_neuts_raw;
    if(n7_neuts_raw)delete n7_neuts_raw;
    if(n8_neuts_raw)delete n8_neuts_raw;
    if(n9_neuts_raw)delete n9_neuts_raw;
      
    
    if(n0_gammas_raw)delete n0_gammas_raw;
    if(n1_gammas_raw)delete n1_gammas_raw;
    if(n2_gammas_raw)delete n2_gammas_raw;
    if(n3_gammas_raw)delete n3_gammas_raw;
    if(n4_gammas_raw)delete n4_gammas_raw;
    if(n5_gammas_raw)delete n5_gammas_raw;
    if(n6_gammas_raw)delete n6_gammas_raw;
    if(n7_gammas_raw)delete n7_gammas_raw;
    if(n8_gammas_raw)delete n8_gammas_raw;
    if(n9_gammas_raw)delete n9_gammas_raw;
  }
    
  
}

  
	

#endif	       



