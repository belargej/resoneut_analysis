#include "RN_SimRun.hpp"

RN_SimRun::RN_SimRun():def(0){
}


void RN_SimRun::Loop(Long64_t evnum,std::string options){
  SetVariables();
  Long64_t evcount=0;
  while(evcount<evnum){
    if(GenerateEvents(evcount,options))
      evcount++;
    else
      continue;
    if(def==1)FillHistograms();
    if(evcount%30000==0)std::cout<<evcount<<std::endl;
  }  
}

void RN_SimRun::initHists(){
  def=1;
  /*tree= new TTree("sim","sim");
  for(unsigned int i=0;i<plist.size();i++)
    tree->Branch(Form("%s.",plist[i].Name().c_str()),"RN_Particle",&plist[i]);
  for(unsigned int i=0;i<pterph.size();i++)
    tree->Branch(Form("%s.",pterph[i].Name().c_str()),"RN_PTerph",&pterph[i]);
  */
  hn_CM=new TH1D("h_nCM","h_nCM",512,1,180);
  hn_CMvLab=new TH2D("hn_CMvLAB","n_CMvLAB",512,1,180,512,1,180);
  hn_tof=new TH1D("hn_tof","hn_tof",512,1,128);
  htof_n=new TH2D("htof_n0","htof_n0",17,0,16,512,1,128);
  hE_n=new TH2D("hE_n","hE_n",17,0,16,512,0,5);
  hQ=new TH1D("hQ","hQ",96,-4,0);
  h_nKE=new TH1D("h_nKE","h_nKE",1024,0,1);
  h_hiKE=new TH1D("h_hiKE","h_hiKE",1024,0,100);
  hE_v_theta=new TH2D("hE_v_theta","hE_v_theta",180,0,179,512,0,20);
  hT_v_theta=new TH2D("hT_v_theta","hT_v_theta",180,0,179,512,1,128);
  hpos=new TH2D("hpos","hpos",1024,-256,256,1024,-256,256);
}

void RN_SimRun::FillHistograms(){
  //tree->Fill();
  hE_v_theta->Fill(plist[2].LV.Theta()*180/3.14,plist[2].LV.E()-plist[2].LV.M());
  hn_CM->Fill(n_cm*180/3.14);
  hn_CMvLab->Fill(n_cm*180/3.14,plist[2].LV.Theta()*180/3.14);
 
  int cref=0;
  for(RN_PTerphCollectionRef it=pterph.begin();it!=pterph.end();it++){ 
    if((*it).fT>0){
      hn_tof->Fill(((*it).fT));
      htof_n->Fill(cref,(*it).fT);
      hE_n->Fill(cref,(*it).fEsum);
      hT_v_theta->Fill(plist[2].LV.Theta()*180/3.14,(*it).fT);
      hpos->Fill((*it).GetHitPos().X(),(*it).GetHitPos().Y());
      double nKE=0,hiKE=0;
      double q_value=QValue((*it).GetPosVect().Z(),(*it).fT,nKE,hiKE);
      hQ->Fill(q_value);
      h_nKE->Fill(nKE);
      h_hiKE->Fill(hiKE);
    }
    cref++;
  }
 
}

void RN_SimRun::StartRun(std::string input){
  Init();


  std::string temp;
  ifstream cfg(input.c_str());
  if(!cfg){
    std::cout<<"could not cfg "<<input<<std::endl;
    std::cin>>temp;
    return ;
  }

  do{
    std::vector<std::string>input;
    sak::ReadLine(cfg,input);
    if(input.size()==0)
      continue;
    if(input[0]=="fParam")
      {      
	global.AddParam(input[1],sak::string_to_double(input[2]));
      }
    else if(input[0]=="fDWBA")
      {
	SetAngularDistribution(input[1]);
	
      }
    else if(input[0]=="fOutput")
      {
	rootfile=new TFile(input[1].c_str(),"RECREATE");
	if(input.size()==2)
	  initHists();
	else
	  std::cout<<"invalid number of arguments for fOutput"<<std::endl;
      }
    else if(input[0]=="fEvents")
      {
	totevents=sak::string_to_int(input[1]);
      }
    else if(input[0]=="fOption")
      {
	option=input[1];
      }

    else
      continue;
  } while(!cfg.eof());
  
  Loop(totevents,option);

    

}
