#include "RN_SimRun.hpp"

RN_SimRun::RN_SimRun():def(0){
}


void RN_SimRun::Loop(Long64_t evnum,std::string options){
  SetVariables();
  Long64_t evcount=0;
  while(evcount<evnum){
    Reset();
    if(GenerateEvents(evcount,options))
      evcount++;
    else
      continue;

    if(def)FillHistograms();

  }  
}

void RN_SimRun::initHists(){
  def=1;
  hn_t=new TH1D("hN_t","hN_t",512,0,128);

}


void RN_SimRun::FillHistograms(){
  hn_t->Fill(n_tof);

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
	initHists();	
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
