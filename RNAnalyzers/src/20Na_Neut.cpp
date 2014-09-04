/***************************************************************/
//Class: 20Na_Neut
//
//Author:Joe Belarge
//
//NeutPSDAnalyzer is added to the analyzer list to sort neutron detector
//parameters.  Make histograms, load cuts, and check cuts 
//using parameters related to the neutron detector pulse shape
// discrimination spectra.  Also, neutron timing.
/********************************************************************/
#ifndef _20NA_NEUT_CXX
#define _20NA_NEUT_CXX

#include "20Na_Neut.hpp"
#include "RN_Root.hpp"
using namespace RNROOT;


namespace Neut_20Na{
  
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


  //declare histograms here

  //n_detmult
  //TH1D *h_ndetMult;
  //TH1D *h_ndetMult_ngated; 
  TH1D *NeutDetMult;
  TH1D *NeutDetMult_NeutGated;

  //neut_psd
  //TH2D *hPSDq_n[NEUTNUM];
  //TH2D *hPSD_n_[NEUTNUM];
  TH2D *PSD_Det_[NEUTNUM];
  TH2D *PSD_n_[NEUTNUM];

  //neut_time
  TH2D *QvT_Det_[NEUTNUM];
  TH2D *QvT_NeutGated_Det_[NEUTNUM];
  
  //TH2D *hPSDq_n_evtgated[NEUTNUM];


  Neut_20Na::Neut_20Na():RN_Analyzer("Neut_20Na","Neut_20Na")
  {
  }
  


  bool Neut_20Na::Begin(){

    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    //create directory structure
    fgRootFile->mkdir("Neutron");
    fgRootFile->cd("Neutron");
    gDirectory->mkdir("PSD");
    gDirectory->cd("PSD");
    gDirectory->mkdir("Raw");
    gDirectory->mkdir("Cal");
    
    fgRootFile->cd("Neutron");
    gDirectory->mkdir("TimeVsQ");
    gDirectory->mkdir("Mult");
  

    //create histograms
    fgRootFile->cd("Neutron/Mult");
    NeutDetMult=new TH1D("NeutDetMult","Neutron Detector Multiplicity;Detector;Multiplicity",NEUTNUM+1,0,NEUTNUM);
    NeutDetMult_NeutGated=new TH1D("NeutDetMult_NeutGated","Neutron Detector Multiplicity:Neutron Gated;Detector;Multiplicity",NEUTNUM+1,0,NEUTNUM);
    

    for(unsigned int i=0;i<NEUTNUM;i++){  
      fgRootFile->cd("Neutron/TimeVsQ");
      QvT_Det_[i]=new TH2D(Form("QvT_Det_%d",i),Form("QvT Det %d;Time;Q",i),128,0,128,1024,0,4095);
      QvT_NeutGated_Det_[i]=new TH2D(Form("QvT_NeutGated_Det_%d",i),Form("QvT Det %d Neutron Gated;Time;Q",i),128,0,128,1024,0,4095);
      fgRootFile->cd("Neutron/PSD/Raw");
      PSD_Det_[i]=new TH2D(Form("PSD_Det_%d",i),Form("PSD Detector %d;Long Q;Short Q",i),1024,0,4096,1024,0,4096);
      //hPSDq_n_evtgated[i]=new TH2D(Form("hPSDq_n%d_evtgated",i),Form("hPSDq_n%d_evtgated",i),1024,0,4096,1024,0,4096);
  
      fgRootFile->cd("Neutron/PSD/Cal");
      PSD_n_[i]=new TH2D(Form("PSD_neut%d",i),Form("PSD_neut%d;PSD;Long Q",i),256,-2.,2.,1024,50,4096);
    } 
    fgRootFile->cd();

    
    return 1;
    
  }
  void Neut_20Na::Reset(){
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
  

  bool Neut_20Na::Process(){ 
 
    
    neutcheck[0] = (n0_neuts && n0_neuts->IsInside(neut[0].PSD(),neut[0].QLong()));
    gammacheck[0] = (n0_gammas && n0_gammas->IsInside(neut[0].PSD(),neut[0].QLong()));
   
		   
    neutcheck[1] = (n1_neuts && n1_neuts->IsInside(neut[1].PSD(),neut[1].QLong()));
    gammacheck[1] = (n1_gammas && n1_gammas->IsInside(neut[1].PSD(),neut[1].QLong()));
   

    neutcheck[2] = (n2_neuts && n2_neuts->IsInside(neut[2].PSD(),neut[2].QLong()));
    gammacheck[2] = (n2_gammas && n2_gammas->IsInside(neut[2].PSD(),neut[2].QLong()));
    
    neutcheck[3] = (n3_neuts && n3_neuts->IsInside(neut[3].PSD(),neut[3].QLong()));
    gammacheck[3] =(n3_gammas && n3_gammas->IsInside(neut[3].PSD(),neut[3].QLong()));
    
    neutcheck[4] = (n4_neuts && n4_neuts->IsInside(neut[4].PSD(),neut[4].QLong()));
    gammacheck[4] = (n4_gammas && n4_gammas->IsInside(neut[4].PSD(),neut[4].QLong()));
    
    neutcheck[5] = (n5_neuts && n5_neuts->IsInside(neut[5].PSD(),neut[5].QLong()));
    gammacheck[5] = (n5_gammas && n5_gammas->IsInside(neut[5].PSD(),neut[5].QLong()));
    
    neutcheck[6] = (n6_neuts && n6_neuts->IsInside(neut[6].PSD(),neut[6].QLong()));
    gammacheck[6] = (n6_gammas && n6_gammas->IsInside(neut[6].PSD(),neut[6].QLong()));
    
    neutcheck[7] = (n7_neuts && n7_neuts->IsInside(neut[7].PSD(),neut[7].QLong()));
    gammacheck[7]= (n7_gammas && n7_gammas->IsInside(neut[7].PSD(),neut[7].QLong()));
    
    neutcheck[8] = (n8_neuts && n8_neuts->IsInside(neut[8].PSD(),neut[8].QLong()));
    gammacheck[8] = (n8_gammas && n8_gammas->IsInside(neut[8].PSD(),neut[8].QLong()));
    
    neutcheck[9] = (n9_neuts && n9_neuts->IsInside(neut[9].PSD(),neut[9].QLong()));
    gammacheck[9] =(n9_gammas && n9_gammas->IsInside(neut[9].PSD(),neut[9].QLong()));
    
    rawneutcheck[0] = (n0_neuts_raw && n0_neuts_raw->IsInside(neut[0].QLong(),neut[0].QShort()));
    rawgammacheck[0] = (n0_gammas_raw && n0_gammas_raw->IsInside(neut[0].QLong(),neut[0].QShort()));

    rawneutcheck[1] = (n1_neuts_raw &&  n1_neuts_raw->IsInside(neut[1].QLong(),neut[1].QShort()));
    rawgammacheck[1] = (n1_gammas_raw && n1_gammas_raw->IsInside(neut[1].QLong(),neut[1].QShort()));

    rawneutcheck[2] = (n2_neuts_raw && n2_neuts_raw->IsInside(neut[2].QLong(),neut[2].QShort()));
    rawgammacheck[2] = (n2_gammas_raw && n2_gammas_raw->IsInside(neut[2].QLong(),neut[2].QShort()));

    rawneutcheck[3] = (n3_neuts_raw && n3_neuts_raw->IsInside(neut[3].QLong(),neut[3].QShort()));
    rawgammacheck[3] = (n3_gammas_raw && n3_gammas_raw->IsInside(neut[3].QLong(),neut[3].QShort()));

    rawneutcheck[4] = (n4_neuts_raw && n4_neuts_raw->IsInside(neut[4].QLong(),neut[4].QShort()));
    rawgammacheck[4] = (n4_gammas_raw && n4_gammas_raw->IsInside(neut[4].QLong(),neut[4].QShort()));

    rawneutcheck[5] = (n5_neuts_raw && n5_neuts_raw->IsInside(neut[5].QLong(),neut[5].QShort()));
    rawgammacheck[5] = (n5_gammas_raw && n5_gammas_raw->IsInside(neut[5].QLong(),neut[5].QShort()));

    rawneutcheck[6] = (n6_neuts_raw && n6_neuts_raw->IsInside(neut[6].QLong(),neut[6].QShort()));
    rawgammacheck[6] = (n6_gammas_raw && n6_gammas_raw->IsInside(neut[6].QLong(),neut[6].QShort()));

    rawneutcheck[7] = (n7_neuts_raw && n7_neuts_raw->IsInside(neut[7].QLong(),neut[7].QShort()));
    rawgammacheck[7] = (n7_gammas_raw && n7_gammas_raw->IsInside(neut[7].QLong(),neut[7].QShort()));

    rawneutcheck[8] = (n8_neuts_raw && n8_neuts_raw->IsInside(neut[8].QLong(),neut[8].QShort()));
    rawgammacheck[8] = (n8_gammas_raw && n8_gammas_raw->IsInside(neut[8].QLong(),neut[8].QShort()));

    rawneutcheck[9] = (n9_neuts_raw && n9_neuts_raw->IsInside(neut[9].QLong(),neut[9].QShort()));
    rawgammacheck[9] = (n9_gammas_raw && n9_gammas_raw->IsInside(neut[9].QLong(),neut[9].QShort()));


    evtcheck[0] = (n0_evt && n0_evt->IsInside(neut[0].TRel(),neut[0].QLong()));
    evtcheck[1] = (n1_evt && n1_evt->IsInside(neut[1].TRel(),neut[1].QLong()));    
    evtcheck[2] = (n2_evt && n2_evt->IsInside(neut[2].TRel(),neut[2].QLong()));
    evtcheck[3] = (n3_evt && n3_evt->IsInside(neut[3].TRel(),neut[3].QLong()));
    evtcheck[4] = (n4_evt && n4_evt->IsInside(neut[4].TRel(),neut[4].QLong()));
    evtcheck[5] = (n5_evt && n5_evt->IsInside(neut[5].TRel(),neut[5].QLong()));
    evtcheck[6] = (n6_evt && n6_evt->IsInside(neut[6].TRel(),neut[6].QLong()));
    evtcheck[7] = (n7_evt && n7_evt->IsInside(neut[7].TRel(),neut[7].QLong()));
    evtcheck[8] = (n8_evt && n8_evt->IsInside(neut[8].TRel(),neut[8].QLong()));
    evtcheck[9] = (n9_evt && n9_evt->IsInside(neut[9].TRel(),neut[9].QLong()));
    

    
    for (int i=0;i<10;i++){	    
      neut_sansgamma[i] = neutcheck[i] && !(gammacheck[i]);
      rawneut_sansrawgamma[i] = rawneutcheck[i] && !(rawgammacheck[i]);
      
      //check if this detector is in the omitneut list and leave it out of the orcheck if it is
      //int omitneut = std::count(OmitNeutDet.begin(),OmitNeutDet.end(),i);
      //if(omitneut)
      //	continue;
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

  bool Neut_20Na::ProcessFill(){
    
    NeutDetMult->Fill(Narray.fMult);  
    if(rawneut_orcheck){
      NeutDetMult_NeutGated->Fill(Narray.fMult);
    }

    for(unsigned int i=0;i<NEUTNUM;i++){
      if(i>=neut.size())
	break;
      PSD_n_[i]->Fill(neut[i].PSD(),neut[i].QLong()); 
      PSD_Det_[i]->Fill(neut[i].QLong(),neut[i].QShort());    
    
    QvT_Det_[i]->Fill(neut[i].TRel(),neut[i].QLong());
    if(rawneutcheck[i]){
      QvT_NeutGated_Det_[i]->Fill(neut[i].TRel(),neut[i].QLong());
    }
    //if(evtcheck[i])
      //hPSDq_n_evtgated[i]->Fill(neut[i].QLong(),neut[i].QShort());
    }
    
    return 1;
  }

  
  bool Neut_20Na::Terminate(){
    return 1;
  }

  void LoadPSDGates(const std::string& input){
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



