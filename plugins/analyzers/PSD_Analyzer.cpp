#include "PSD_Analyzer.hpp"



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

  int neutcheck[NEUTNUM];
  int neut_sansgamma[NEUTNUM];
  int rawneutcheck[NEUTNUM];
  int rawneut_sansrawgamma[NEUTNUM];
  int gammacheck[NEUTNUM];
  int rawgammacheck[NEUTNUM];
  int neut_orcheck(0);
  int rawneut_orcheck(0);
  int neut_sansgamma_orcheck(0);
  int rawneut_sansrawgamma_orcheck(0);
  
  std::vector<int>OmitNeutDet;


  //declare histograms here

  //n_detmult
  sak::Hist1D *h_ndetMult;
  sak::Hist1D *h_ndetMult_ngated;

  //neut_psd
  sak::Histogram2D *hPSDq_n[NEUTNUM];
  sak::Histogram2D *hPSD_n_[NEUTNUM];


  NeutAnalyzer::NeutAnalyzer()
  {
  }



  void NeutAnalyzer::Begin(){
    int idx=0;

    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    rootfile->mkdir("neut_PSD/raw");
    rootfile->mkdir("neut_PSD/cal");
    rootfile->mkdir("mult/neut");

    rootfile->cd("mult/neut");
    h_ndetMult=new sak::Hist1D("h_ndetmult","mult",NEUTNUM+1,0,NEUTNUM);
    h_ndetMult_ngated=new sak::Hist1D("h_ndetmult_ngated","mult",NEUTNUM+1,0,NEUTNUM);
    

    for(int i=0;i<NEUTNUM;i++){  
      rootfile->cd("neut_PSD/raw");
      hPSDq_n[i]=new sak::Histogram2D(Form("hPSDq_n%d",i),"fQ_long","fQ_short",1024,0,4096,1024,0,4096);
      rootfile->cd("neut_PSD/cal");
      hPSD_n_[i]=new sak::Histogram2D(Form("hPSD_neut%d",i),"fPSD","fQ_long",256,0.,1.,1024,50,4096);
    } 
    rootfile->cd();

    
  
    
  }
  

  void NeutAnalyzer::Process(){ 
    for(int i=0;i<NEUTNUM;i++){
      neutcheck[i]=0;
      neut_sansgamma[i]=0;
      rawneut_sansrawgamma[i]=0;
      rawneutcheck[i]=0;
      gammacheck[i]=0;
      rawgammacheck[i]=0;
    }
    neut_orcheck = 0;
    rawneut_orcheck = 0;
    neut_sansgamma_orcheck = 0;
    rawneut_sansrawgamma_orcheck = 0;
    
    
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
    
    for (int i=0;i<10;i++){	    
      neut_sansgamma[i] = neutcheck[i] && !(gammacheck[i]);
      rawneut_sansrawgamma[i] = rawneutcheck[i] && !(rawgammacheck[i]);
      
      //check if this detector is in the omitneut list and leave it out of the orcheck if it is
      int omitneut = std::count(OmitNeutDet.begin(),OmitNeutDet.end(),i);
      if(omitneut)
	continue;
      
      neut_orcheck = neutcheck[i] || neut_orcheck;
      rawneut_orcheck = rawneutcheck[i] || rawneut_orcheck;
      neut_sansgamma_orcheck = neut_sansgamma[i] || neut_sansgamma_orcheck;
      rawneut_sansrawgamma_orcheck = rawneut_sansrawgamma[i] || rawneut_sansrawgamma_orcheck;
    }
		       
    for (int i=0;i<NEUTNUM;i++){
      if(i>=neut.size())
	break;
      hPSDq_n[i]->Fill(neut[i].fQ_long,neut[i].fQ_short);
    }
    
 
    //Fill calpar.Histograms below this line
    /*************************************************************/
    h_ndetMult->Fill(Narray.fMult);  
    
    for(int i=0;i<NEUTNUM;i++){
      if(i>=neut.size())
	break;
    hPSD_n_[i]->Fill(neut[i].fPSD,neut[i].fQ_long); 
    }
    
    if(rawneut_orcheck){
      h_ndetMult_ngated->Fill(Narray.fMult);
    }
  }

  
  void NeutAnalyzer::Terminate(){
    rootfile->Write();
    rootfile->Close();
    
  }
  
  void LoadGates(const std::string& input){
    sak::LoadCuts in(input.c_str());    
    if(in.getCut("n0_neuts") && !n0_neuts)
      n0_neuts=new TCutG(*in.getCut("n0_neuts"));
    if(in.getCut("n1_neuts") && !n1_neuts)
      n1_neuts=new TCutG(*in.getCut("n1_neuts"));
    if(in.getCut("n2_neuts") && !n2_neuts)
      n2_neuts=new TCutG(*in.getCut("n2_neuts"));   
    if(in.getCut("n3_neuts") && !n3_neuts)
      n3_neuts=new TCutG(*in.getCut("n3_neuts"));
    if(in.getCut("n4_neuts") && !n4_neuts)
      n4_neuts=new TCutG(*in.getCut("n4_neuts"));
    if(in.getCut("n5_neuts") && !n5_neuts)
      n5_neuts=new TCutG(*in.getCut("n5_neuts"));
    if(in.getCut("n6_neuts") && !n6_neuts)
      n6_neuts=new TCutG(*in.getCut("n6_neuts"));   
    if(in.getCut("n7_neuts") && !n7_neuts)
      n7_neuts=new TCutG(*in.getCut("n7_neuts"));
    if(in.getCut("n8_neuts") && !n8_neuts)
      n8_neuts=new TCutG(*in.getCut("n8_neuts"));
    if(in.getCut("n9_neuts") && !n9_neuts)
      n9_neuts=new TCutG(*in.getCut("n9_neuts"));
    
    if(in.getCut("n0_gammas") && !n0_gammas)
      n0_gammas=new TCutG(*in.getCut("n0_gammas"));
    if(in.getCut("n1_gammas") && !n1_gammas)
      n1_gammas=new TCutG(*in.getCut("n1_gammas"));
    if(in.getCut("n2_gammas") && !n2_gammas)
      n2_gammas=new TCutG(*in.getCut("n2_gammas"));   
    if(in.getCut("n3_gammas") && !n3_gammas)
      n3_gammas=new TCutG(*in.getCut("n3_gammas"));
    if(in.getCut("n4_gammas") && !n4_gammas)
      n4_gammas=new TCutG(*in.getCut("n4_gammas"));
    if(in.getCut("n5_gammas") && !n5_gammas)
      n5_gammas=new TCutG(*in.getCut("n5_gammas"));
    if(in.getCut("n6_gammas") && !n6_gammas)
      n6_gammas=new TCutG(*in.getCut("n6_gammas"));   
    if(in.getCut("n7_gammas") && !n7_gammas)
      n7_gammas=new TCutG(*in.getCut("n7_gammas"));
    if(in.getCut("n8_gammas") && !n8_gammas)
      n8_gammas=new TCutG(*in.getCut("n8_gammas"));
    if(in.getCut("n9_gammas") && !n9_gammas)
      n9_gammas=new TCutG(*in.getCut("n9_gammas"));
    
    if(in.getCut("n0_neuts_raw") && !n0_neuts_raw)
      n0_neuts_raw=new TCutG(*in.getCut("n0_neuts_raw"));
    if(in.getCut("n1_neuts_raw") && !n1_neuts_raw)
      n1_neuts_raw=new TCutG(*in.getCut("n1_neuts_raw"));
    if(in.getCut("n2_neuts_raw") && !n2_neuts_raw)
      n2_neuts_raw=new TCutG(*in.getCut("n2_neuts_raw"));   
    if(in.getCut("n3_neuts_raw") && !n3_neuts_raw)
      n3_neuts_raw=new TCutG(*in.getCut("n3_neuts_raw"));
    if(in.getCut("n4_neuts_raw") && !n4_neuts_raw)
      n4_neuts_raw=new TCutG(*in.getCut("n4_neuts_raw"));
    if(in.getCut("n5_neuts_raw") && !n5_neuts_raw)
      n5_neuts_raw=new TCutG(*in.getCut("n5_neuts_raw"));
    if(in.getCut("n6_neuts_raw") && !n6_neuts_raw)
      n6_neuts_raw=new TCutG(*in.getCut("n6_neuts_raw"));   
    if(in.getCut("n7_neuts_raw") && !n7_neuts_raw)
      n7_neuts_raw=new TCutG(*in.getCut("n7_neuts_raw"));
    if(in.getCut("n8_neuts_raw") && !n8_neuts_raw)
      n8_neuts_raw=new TCutG(*in.getCut("n8_neuts_raw"));
    if(in.getCut("n9_neuts_raw") && !n9_neuts_raw)
      n9_neuts_raw=new TCutG(*in.getCut("n9_neuts_raw"));

    
    if(in.getCut("n0_gammas_raw") && !n0_gammas_raw)
      n0_gammas_raw=new TCutG(*in.getCut("n0_gammas_raw"));
    if(in.getCut("n1_gammas_raw") && !n1_gammas_raw)
      n1_gammas_raw=new TCutG(*in.getCut("n1_gammas_raw"));
    if(in.getCut("n2_gammas_raw") && !n2_gammas_raw)
      n2_gammas_raw=new TCutG(*in.getCut("n2_gammas_raw"));   
    if(in.getCut("n3_gammas_raw") && !n3_gammas_raw)
      n3_gammas_raw=new TCutG(*in.getCut("n3_gammas_raw"));
    if(in.getCut("n4_gammas_raw") && !n4_gammas_raw)
      n4_gammas_raw=new TCutG(*in.getCut("n4_gammas_raw"));
    if(in.getCut("n5_gammas_raw") && !n5_gammas_raw)
      n5_gammas_raw=new TCutG(*in.getCut("n5_gammas_raw"));
    if(in.getCut("n6_gammas_raw") && !n6_gammas_raw)
      n6_gammas_raw=new TCutG(*in.getCut("n6_gammas_raw"));   
    if(in.getCut("n7_gammas_raw") && !n7_gammas_raw)
      n7_gammas_raw=new TCutG(*in.getCut("n7_gammas_raw"));
    if(in.getCut("n8_gammas_raw") && !n8_gammas_raw)
      n8_gammas_raw=new TCutG(*in.getCut("n8_gammas_raw"));
    if(in.getCut("n9_gammas_raw") && !n9_gammas_raw)
      n9_gammas_raw=new TCutG(*in.getCut("n9_gammas_raw"));
  

  
    in.Close();
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

  
		       



