#include "S2_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
namespace si_cal{
  TCutG* prots1;
  TCutG* prots2;
  TCutG* alphas;

  int protcheck(0);
  int prot2check(0);
  int alphacheck(0);
  double prot_E(0);
  double prot_dE(0);
  double prot_theta(0);

  sak::Histogram2D *hpede;  
  sak::Histogram1D *h_si_[2];
  sak::Histogram1D *h_si_a[2];
  sak::Histogram2D *h_evtheta[2];
  sak::Histogram2D *h_evtheta_protgated[2];
  sak::Histogram2D *h_evtheta_neutgated[2];
  sak::Histogram2D *h_si_x_y[2];
  sak::Histogram2D *h_si_x_y_prot[2];
  sak::Histogram2D *front[2][16];
  sak::Histogram1D *h_si_fmult[2];
  sak::Histogram1D *h_si_bmult[2];
  sak::Histogram1D *h_si_cluster_mult[2];

  sak::Histogram2D* rfvs1_t;
  sak::Histogram2D *rfvs2_t;
  sak::Histogram2D *rfvs1_t_rel;
  sak::Histogram2D *rfvs2_t_rel;
  
  sak::Histogram2D *rfvs1_t_prot;
  sak::Histogram2D *rfvs2_t_prot;
  sak::Histogram2D *rfvs1_t_rel_prot;
  sak::Histogram2D *rfvs2_t_rel_prot;
  
  sak::Histogram2D *rfvs1_t_prot2;
  sak::Histogram2D *rfvs1_t_alpha;
  sak::Histogram2D *rfvs1_t_rel_prot2;
  sak::Histogram2D *rfvs1_t_rel_alpha;

  sak::Histogram2D *mcpvs1_t;
  sak::Histogram2D *mcpvs2_t;
  sak::Histogram2D *mcpvs1_t_rel;
  sak::Histogram2D *mcpvs2_t_rel;
  
 
  sak::Histogram2D *mcpvs1_t_prot;
  sak::Histogram2D *mcpvs2_t_prot;
  sak::Histogram2D *mcpvs1_t_rel_prot;
  sak::Histogram2D *mcpvs2_t_rel_prot;

  sak::Histogram1D *h_s1_t;
  sak::Histogram1D *h_s1_t_noticds;



  S2_Analyzer::S2_Analyzer():ind_(0)
  {
    
  }

  void S2_Analyzer::ResetGlobals(){
    prot_E=0;
    prot_dE=0;
    protcheck=0;
    prot2check=0;
    alphacheck=0;
    prot_theta=0;

  }


  void S2_Analyzer::AutoCalibrate(int matchfront, int matchback){
    std::cout<<"calibrating back to front match channel : "<<matchfront<<std::endl;
    RN_S2CalCollection s2front;
    RN_S2CalCollection s2back;
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      s2front.push_back(RN_S2Calibrator(16,Form("%s.front",(*it).Name().c_str()),60,3000));
      s2back.push_back(RN_S2Calibrator(16,Form("%s.back",(*it).Name().c_str()),60,3000));
      
    }
    int idx=0;
    
    Long64_t totentries= TotEntries();
    for (Long64_t i=0;i<totentries;i++){
      GetDetectorEntry(i);
      
      idx=0;
      for(RN_S2CalCollectionRef it=s2back.begin();it!=s2back.end();it++){
      
	if(idx<si_.size()){
	  if(si_[idx].front.fMult>0&&si_[idx].front.fChlist[0]==matchfront){
	    if(si_[idx].back.fMult>0)
	      (*it).AddHit(si_[idx].front.fE[0],si_[idx].back.fE[0],si_[idx].back.fChlist[0]);
	  }
	}
	idx++;
	
      } 
  }
    idx=0;
    for(RN_S2CalCollectionRef it=s2back.begin();it!=s2back.end();it++){
      (*it).PerformFit();
      (*it).PrintCoefficients(Form("%s_back2front.in",si_[idx].Name().c_str()));
      //now load file and apply it to that detector
      
      std::cout<<"sleeping for 5 seconds"<<std::endl;
      usleep(5000000);
      
      
      DetVar.LoadParams(Form("%s_back2front.in",si_[idx].Name().c_str()));
      si_[idx].SetCalibrations(DetVar);
      idx++;
      
    }
    

    std::cout<<"repeating now for front 2 back channel : "<<matchback<<std::endl;
    
    for (Long64_t i=0;i<totentries;i++){
      GetDetectorEntry(i);
      idx=0;
      for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
	
	if(idx<si_.size()){
	  if(si_[idx].back.fMult>0&&si_[idx].back.fChlist[0]==matchback){
	    if(si_[idx].front.fMult>0)
	      (*it).AddHit(si_[idx].Back_E(0),si_[idx].Front_E(0),si_[idx].front.fChlist[0]);
	  }
	}
	idx++;

      } 
    }
    idx=0;
    for(RN_S2CalCollectionRef it=s2front.begin();it!=s2front.end();it++){
      (*it).PerformFit();
      (*it).PrintCoefficients(Form("%s_front2back.in",si_[idx].Name().c_str()));
      
      idx++;
      
    }
    
    
    
  }
  
  bool S2_Analyzer::Begin(){   
    
    if(!rootfile){
      std::cout<<"output file has not been created"<<std::endl;
      ClearGates();
    exit(EXIT_FAILURE);
    }
    
    rootfile->mkdir("Silicon/f2b_ratio");
    rootfile->mkdir("Silicon/f2b_ratio/S1");
    rootfile->mkdir("Silicon/f2b_ratio/S2");
    rootfile->mkdir("Silicon/ede");
    rootfile->mkdir("Silicon/evtheta");
    rootfile->mkdir("Silicon/Timing");
    rootfile->mkdir("mult/Silicon");
    
    rootfile->cd("Silicon");
    h_si_a[0]=new sak::Hist1D("h_si_1a","E[MeV]",4096,0,4095);
    h_si_a[1]=new sak::Hist1D("h_si_2a","E[MeV]",4096,0,4095);
    h_si_[0]=new sak::Hist1D("h_si_1","E[MeV]",512,1,16);
    h_si_[1]=new sak::Hist1D("h_si_2","E[MeV]",512,1,16);
    
  rootfile->cd("Silicon/f2b_ratio");
  for(int i=0;i<16;i++){
    rootfile->cd("Silicon/f2b_ratio/S1");
    front[0][i]=new sak::Hist2D(Form("s1_fc%d_corr",i),"channel","ratio",17,0,16,512,0,2);
    rootfile->cd("Silicon/f2b_ratio/S2");
    front[1][i]=new sak::Hist2D(Form("s2_fc%d_corr",i),"channel","ratio",17,0,16,512,0,2);
  }
  rootfile->cd("Silicon/ede");
   hpede=new sak::Hist2D("hpEdE","E [MeV]","dE [MeV]",64,0,20,64,0,6);
  
  for(int i=0;i<2;i++){
    rootfile->cd("Silicon/evtheta");
    h_evtheta[i]=new sak::Hist2D(Form("h_evtheta[%d]",i+1),"theta[deg]","E",256,10,42,128,0,32);
    h_evtheta_protgated[i]=new sak::Hist2D(Form("h_evtheta_prot[%d]",i+1),"theta[deg]","E",256,10,42,128,0,32);
    h_si_x_y[i]=new sak::Hist2D(Form("h_si_x_y[%d]",i+1),"X","Y",512,-100,100,512,-100,100);
    h_si_x_y_prot[i]=new sak::Hist2D(Form("h_si_x_y_prot[%d]",i+1),"X","Y",512,-100,100,512,-100,100);


    rootfile->cd("mult/Silicon");
    h_si_fmult[i]= new sak::Hist1D(Form("h_si_fmult_%d",i),"fmult",32,0,31);
    h_si_bmult[i]= new sak::Hist1D(Form("h_si_bmult_%d",i),"bmult",32,0,31);
    h_si_cluster_mult[i] = new sak::Hist1D(Form("h_si_%d_cluster_mult",i),"mult",32,0,31);
  } 
   
  rootfile->cd("Silicon/Timing");
  h_s1_t=new sak::Hist1D("h_s1_t","time",1024,0,4096);
  h_s1_t_noticds=new sak::Hist1D("h_s1_t_noticds","time",1024,0,4096);; 


  rootfile->cd("Silicon/Timing/rftime");
  
  rfvs1_t=new sak::Hist2D("rfvs1_t","rftime","s1_t",1024,256,2304,1024,0,4096);
  rfvs2_t=new sak::Hist2D("rfvs2_t","rftime","s2_t",1024,256,2304,1024,0,4096);
  rfvs1_t_rel=new sak::Hist2D("rfvs1_t_rel","rftime","s1_t_rel",1024,256,2304,1024,-2048,2047);
  rfvs2_t_rel=new sak::Hist2D("rfvs2_t_rel","rftime","s2_t_rel",1024,256,2304,1024,-2048,2047);

  rfvs1_t_prot=new sak::Hist2D("rfvs1_t_prot","rftime","s1_t",1024,256,2304,1024,0,4096);
  rfvs2_t_prot=new sak::Hist2D("rfvs2_t_prot","rftime","s2_t",1024,256,2304,1024,0,4096);
  rfvs1_t_rel_prot=new sak::Hist2D("rfvs1_t_rel_prot","rftime","s1_t_rel",1024,256,2304,1024,-2048,2047);
  rfvs2_t_rel_prot=new sak::Hist2D("rfvs2_t_rel_prot","rftime","s2_t_rel",1024,256,2304,1024,-2048,2047);
  
  rfvs1_t_prot2=new sak::Hist2D("rfvs1_t_prot2","rftime","s1_t",1024,256,2304,1024,0,4096);
  rfvs1_t_rel_prot2=new sak::Hist2D("rfvs1_t_rel_prot2","rftime","s1_t_rel",1024,256,2304,1024,-2048,2047);
  rfvs1_t_alpha=new sak::Hist2D("rfvs1_t_alpha","rftime","s1_t",1024,256,2304,1024,0,4096);
  rfvs1_t_rel_alpha=new sak::Hist2D("rfvs1_t_rel_alpha","rftime","s1_t_rel",1024,256,2304,1024,-2048,2047);
 

  
  rootfile->cd("Silicon/Timing/mcp");
  mcpvs1_t=new sak::Hist2D("mcpvs1_t","mcp","s1_t",1024,0,4095,1024,0,4095);
  mcpvs2_t=new sak::Hist2D("mcpvs2_t","mcp","s2_t",1024,0,4095,1024,0,4095);
  mcpvs1_t_rel=new sak::Hist2D("mcpvs1_t_rel","mcp","s1_t_rel",1024,0,4095,1024,-2048,2047);
  mcpvs2_t_rel=new sak::Hist2D("mcpvs2_t_rel","mcp","s2_t_rel",1024,0,4095,1024,-2048,2047);
  
 
  mcpvs1_t_prot=new sak::Hist2D("mcpvs1_t_prot","mcp","s1_t",1024,0,4095,1024,0,4095);
  mcpvs2_t_prot=new sak::Hist2D("mcpvs2_t_prot",",mcp","s2_t",1024,0,4095,1024,0,4095);
  mcpvs1_t_rel_prot=new sak::Hist2D("mcpvs1_t_rel_prot","mcp","s1_t_rel",1024,0,4095,1024,-2048,2047);
  mcpvs2_t_rel_prot=new sak::Hist2D("mcpvs2_t_rel_prot","mcp","s2_t_rel",1024,0,4095,1024,-2048,2047);
  

  return 1;
  }  
    
    
  

bool S2_Analyzer::Process(){
  protcheck=0;
  prot_E=0;
  prot_dE=0;
  prot_theta=0;
  
  h_s1_t->Fill(si_[0].Back_T(0));
  if(!unpacker::TDC1[4]>0)
    h_s1_t_noticds->Fill(si_[0].Back_T(0));
  
  if(si_cluster_[1].fMult>0&&si_cluster_[0].fMult>0){
    prot_dE=si_cluster_[1].fE[0];
    prot_E=si_cluster_[0].fE[0]+prot_dE;
    hpede->Fill(prot_E,prot_dE);
    prot_theta=si_cluster_[1].fPos[0].Theta()*(180/3.14);
  }
  if(si_cal::prots1)
    protcheck=si_cal::prots1->IsInside(prot_E,prot_dE);
  
  if(si_cal::prots2)
    prot2check=si_cal::prots2->IsInside(prot_E,prot_dE);
  
  if(si_cal::alphas)
    alphacheck=si_cal::alphas->IsInside(prot_E,prot_dE);
  


  for(int i=0;i<2;i++){
    int idx=(int)si_[i].front.fChlist[0];	      
    if(si_[i].front.fMult>0&&si_[i].back.fMult>0){
      front[i][idx]->Fill(si_[i].back.fChlist[0],(si_[i].Back_E(0)/si_[i].Front_E(0)));
      if(si_cluster_[i].fMult>0){
	h_evtheta[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,prot_E);
	h_si_x_y[i]->Fill(si_cluster_[i].fPos[0].X(),si_cluster_[i].fPos[0].Y());
	if(protcheck){
	  h_evtheta_protgated[i]->Fill(si_cluster_[i].fPos[0].Theta()*180/3.14,prot_E);
	  h_si_x_y_prot[i]->Fill(si_cluster_[i].fPos[0].X(),si_cluster_[i].fPos[0].Y());
	}
      }
    h_si_[i]->Fill(si_[i].Front_E(0));
    h_si_a[i]->Fill(si_[i].Front_E(0));
    h_si_fmult[i]->Fill(si_[i].front.fMult);
    h_si_bmult[i]->Fill(si_[i].back.fMult);
    h_si_cluster_mult[i]->Fill(si_cluster_[i].fMult);
    }
    
  }

  if(si_[0].Back_T(0)>0){
    rfvs1_t->Fill(rftime[0].fT,si_[0].Back_T(0));
    rfvs2_t->Fill(rftime[0].fT,si_[1].Back_T(0));
    rfvs1_t_rel->Fill(rftime[0].fT,rftime[0].fT-si_[0].Back_T(0));
    rfvs2_t_rel->Fill(rftime[0].fT,rftime[0].fT-si_[1].Back_T(0));
    
    mcpvs1_t->Fill(rftime[1].fT,si_[0].Back_T(0));
    mcpvs2_t->Fill(rftime[1].fT,si_[1].Back_T(0));
    mcpvs1_t_rel->Fill(rftime[1].fT,rftime[0].fT-si_[0].Back_T(0));
    mcpvs2_t_rel->Fill(rftime[1].fT,rftime[0].fT-si_[1].Back_T(0));

    if(protcheck){
      rfvs1_t_prot->Fill(rftime[0].fT,si_[0].Back_T(0));
      rfvs2_t_prot->Fill(rftime[0].fT,si_[1].Back_T(0));
      rfvs1_t_rel_prot->Fill(rftime[0].fT,rftime[0].fT-si_[0].Back_T(0));
      rfvs2_t_rel_prot->Fill(rftime[0].fT,rftime[0].fT-si_[1].Back_T(0));

      mcpvs1_t_prot->Fill(rftime[1].fT,si_[0].Back_T(0));
      mcpvs2_t_prot->Fill(rftime[1].fT,si_[1].Back_T(0));
      mcpvs1_t_rel_prot->Fill(rftime[1].fT,rftime[0].fT-si_[0].Back_T(0));
      mcpvs2_t_rel_prot->Fill(rftime[1].fT,rftime[0].fT-si_[1].Back_T(0));

    }
    
    if(prot2check){
      rfvs1_t_prot2->Fill(rftime[0].fT,si_[0].Back_T(0));
      rfvs1_t_rel_prot2->Fill(rftime[0].fT,rftime[0].fT-si_[0].Back_T(0));
    }
    
    
    if(alphacheck){
      rfvs1_t_alpha->Fill(rftime[0].fT,si_[0].Back_T(0));
      rfvs1_t_rel_alpha->Fill(rftime[0].fT,rftime[0].fT-si_[0].Back_T(0));
      
      
      
    }

    
  }

  return 1;

}


  bool S2_Analyzer::Terminate(){
    rootfile->Write();
    rootfile->Close();
    
    return 1;
  }
  
  void S2_Analyzer::Clear(){
    
  }
  
  
  
  
  void producehists(const char * input,const char* output,int index,const char* config){
    S2_Analyzer a;
    a.Init(input);
    if(config){
      LoadVariableFile(config);
      SetCalibrations();
    }
    a.Loop();
    
    
  }

  void autocalibrate(const char* input,int fmatch,int bmatch){
    S2_Analyzer a;
    a.Init(input);
    a.AutoCalibrate(fmatch,bmatch);
  }
  

  void LoadGates(const std::string& input){
    sak::LoadCuts in(input.c_str());    
    if(in.getCut("prots1") && !prots1)
      prots1=new TCutG(*in.getCut("prots1"));
    if(in.getCut("prots2") && !prots2)
      prots2=new TCutG(*in.getCut("prots2"));
    if(in.getCut("alphas") && !alphas)
      alphas=new TCutG(*in.getCut("alphas"));   

  
    in.Close();
  }
  
  void ClearGates(){
    if(prots1)delete prots1;
    if(prots2)delete prots2;
    if(alphas)delete alphas;



  }
}
