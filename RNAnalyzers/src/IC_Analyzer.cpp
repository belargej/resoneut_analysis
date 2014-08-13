
/***********************************************************/
//Class: IC_Analyzer
//
//Author:Sean Kuvin
//
//IC Analyzer class to be inserted in the analyzer list for 
//sorting Ion Chamber parameters. Making Histograms, 
//Loading Cuts, and checking validity of cuts should 
//all be performed here when considering Ion chamber 
//parameters on their own. 
/***********************************************************/

#ifndef _IC_ANALYZER_CXX
#define _IC_ANALYZER_CXX

#include "IC_Analyzer.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;

namespace ionchamber{
  TCutG* ede_hi1;
  TCutG* ede_hi2;
  TCutG* ede_hi3;
  TCutG* posede;
  TCutG* pileup;

  int  _require_hi1(0);
  int  _require_hi2(0);
  int  _require_hi3(0);
  int  gRequirePosEDE(0);


  int hi_check[3];
  int PosEdECheck(0);

  TH2D *hRawICDEvE;  
  TH2D *hRawICPosEvE;  
  TH2D *hRawICfDEvfE; 
  TH2D *hCalICDEvE;  
  TH2D *hCalICPosEvE;  
  TH2D *hCalICfDEvfE;   
  TH1D *hICT;
  TH1D *hICTheta;
  TH2D *hICEvRF;
  TH2D *hICEvT;
  TH1D *hICPhi;
  TH2D *hICEvTheta;
  TH2D *hICEAllvTheta;
  TH2D *hXvY;
  TH2D *hXvYRaw;
  TH1D *hXGridMult;
  TH1D *hYGridMult;
  TH2D *hXMultvYMult;
  TH1D *hICXCh0; 
  TH1D *hICYCh0;
  TH1D *hICCalDE;
  TH1D *hICCalE;
  TH1D *hICESumX;
  TH1D *hICESumY;
  TH2D *hICXE1vE0;  
  TH2D *hICYE1vE0;  
  TH2D *hICXCH1vCH0;  
  TH2D *hICYCH1vCH0; 
  TH2D *hICXE2vE0;  
  TH2D *hICYE2vE0;  
  TH2D *hICXE2vE1;  
  TH2D *hICYE2vE1; 
  TH2D *hICXCH2vCH0;  
  TH2D *hICYCH2vCH0; 
  TH2D *hICXCH2vCH1;  
  TH2D *hICYCH2vCH1; 
  TH1D *hICXE0Mult1;
  TH1D *hICYE0Mult1;
  TH2D *hPileupChs; 




  IC_Analyzer::IC_Analyzer()
  {
    
  }  
  bool IC_Analyzer::Begin(){   
    
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      //      ClearGates();
      exit(EXIT_FAILURE);
    }
    
    //make directory structure
    fgRootFile->mkdir("IC");
    fgRootFile->cd("IC");
    gDirectory->mkdir("ede");
    gDirectory->mkdir("t");
    gDirectory->mkdir("xy");
    gDirectory->mkdir("mult");
    gDirectory->cd("ede");
    gDirectory->mkdir("rawpar");
    gDirectory->mkdir("calpar");

    //create histograms
    fgRootFile->cd("IC/xy");
    hXvY= new TH2D("hXvY","hXvY;XChan;YChan",128,-128,128,128,-128,128);
    hXvYRaw= new TH2D("hXvYRaw","hXvYRaw;XChanHighestE;YChanHighestE",64,-16,47,64,-16,47);
    hICTheta = new TH1D("hICTheta","hICTheta;Theta",180,0,20);
    hICEvTheta = new TH2D("hICEvTheta","hICEvTheta;Theta;E",512,0,15,512,0,128);
    hICEAllvTheta = new TH2D("hICEAllvTheta","hICEAllvTheta;Theta;EAll",512,0,15,512,0,128);
    hICPhi = new TH1D("hICPhi","hICPhi;Phi",180,0,179);



    fgRootFile->cd("IC/ede/rawpar");
    hRawICDEvE=new TH2D("hRawICDEvE","IRawCDEvE; E + dE[arb];dE [arb]",1024,0,4096,1024,0,4096);
    hRawICfDEvfE=new TH2D("hRawICfDEvfE","hRawICfDEvfE;E [arb];dE [arb]",1024,0,4096,1024,0,4096);
    hRawICPosEvE=new TH2D("hRawICPosEvE","hRawICPosEvE;E + dE + dEX + dEY [arb];dEX + dEY [arb]",1024,0,4095,1024,0,8124);


    fgRootFile->cd("IC/ede/calpar");
    hCalICDEvE=new TH2D("hCalICDEvE","hCalICDEvE; E + dE[arb];dE [arb]",1024,0,4095,1024,0,1023);
    hCalICfDEvfE=new TH2D("hCalICfDEvfE","hCalICfDEvfE;E [arb];dE [arb]",1024,0,4095,1024,0,1023);
    hCalICPosEvE=new TH2D("hCalICPosEvE","hCalICPosEvE;E + dE + dEX + dEY [arb];dEX + dEY [arb]",1024,0,4095,1024,0,1023);
    hICCalE = new TH1D("hICCalE","hICCalE;IC.E()",2048,0,1024);
    hICCalDE = new TH1D("hICCalDE","hICCalDE;IC.DE()",2048,0,1024);
    hICESumX = new TH1D("hICESumX","hICESumX;IC.ESumX()",4096,0,4095);
    hICESumY = new TH1D("hICESumY","hICESumY;IC.ESumY()",4096,0,4095);


    fgRootFile->cd("IC/t");
    hICT = new TH1D("hICT","ICT;t",1024,0,4095);
    hICEvT = new  TH2D("hICEvT","hICEvT;T;E",1280,0,1279,1024,0,4095);
    hICEvRF = new TH2D("hICEvRF","hICEvRF;RF;E",1024,0,4095,1024,0,4095);


    fgRootFile->cd("IC/mult");    
    hXGridMult = new TH1D("hXGridMult","hXGridMult;mult",32,0,31);
    hICXCh0 = new TH1D("hICXCh0","hICXCh0;XCh0",32,0,31);
    hICYCh0 = new TH1D("hICYCh0","hICYCh0;YCh0",32,0,31);
    hICXE1vE0 = new TH2D("hICXE1vE0","hICXE1vE0",512,0,4095,512,0,4095);  
    hICYE1vE0 = new TH2D("hICYE1vE0","hICYE1vE0",512,0,4095,512,0,4095);   
    hICXCH1vCH0 = new TH2D("hICXCH1vCH0","hICXCH1vCH0",32,0,31,32,0,31);  
    hICYCH1vCH0 = new TH2D("hICYCH1vCH0","hICYCH1vCH0",32,0,31,32,0,31);  
    hICXE2vE0 = new TH2D("hICXE2vE0","hICXE2vE0",512,0,4095,512,0,4095);  
    hICYE2vE0 = new TH2D("hICYE2vE0","hICYE2vE0",512,0,4095,512,0,4095);   
    hICXCH2vCH0 = new TH2D("hICXCH2vCH0","hICXCH2vCH0",32,0,31,32,0,31);  
    hICYCH2vCH0 = new TH2D("hICYCH2vCH0","hICYCH2vCH0",32,0,31,32,0,31); 
    hICXE2vE1 = new TH2D("hICXE2vE1","hICXE2vE1",512,0,4095,512,0,4095);  
    hICYE2vE1 = new TH2D("hICYE2vE1","hICYE2vE1",512,0,4095,512,0,4095);   
    hICXCH2vCH1 = new TH2D("hICXCH2vCH1","hICXCH2vCH1",32,0,31,32,0,31);  
    hICYCH2vCH1 = new TH2D("hICYCH2vCH1","hICYCH2vCH1",32,0,31,32,0,31); 
    hICXE0Mult1 = new TH1D("hICXE0Mult1","hICXE0Mult1",4096,0,4095);
    hICYE0Mult1 = new TH1D("hICYE0Mult1","hICYE0Mult1",4096,0,4095);
    hYGridMult = new TH1D("hYGridMult","hYGridMult;mult",32,0,31);
    hXMultvYMult = new TH2D("hXMultvYMult","hXMultvYGridMult;XMult;YMult",32,0,31,32,0,31);
    hPileupChs = new TH2D("hPileup","hPileup",32,0,31,32,0,31);  





    return 1;
  }

  void IC_Analyzer::Reset(){
    for(int i=0;i<3;i++){
      hi_check[i]=0;
    }
    PosEdECheck = 0;

  }

  bool IC_Analyzer::Process(){
    ic.ReconstructHitPos();
    
    hi_check[0]= (ede_hi1 && ede_hi1->IsInside(ic.TotalE(),ic.DE()));
    hi_check[1]= (ede_hi2 && ede_hi2->IsInside(ic.TotalE(),ic.DE()));
    hi_check[2]= (ede_hi3 && ede_hi3->IsInside(ic.TotalE(),ic.DE()));
    PosEdECheck = (posede && posede->IsInside(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y()));

 
    if (_require_hi1 && !hi_check[0])
      return 0;
    if (_require_hi2 && !hi_check[1])
      return 0;
    if (_require_hi3 && !hi_check[2])
      return 0;

    if(gRequirePosEDE && !PosEdECheck)
      return 0;


    return 1;
  }
  
  bool IC_Analyzer::ProcessFill(){

    //multiple configurations of the E-DE:    
    //SumEPos = SumXE + SumYE
 
    hRawICDEvE->Fill(ic.ERaw() + ic.DERaw(), ic.DERaw());
    hRawICfDEvfE->Fill(ic.ERaw(),ic.DERaw());
    hRawICPosEvE->Fill(ic.ERaw() + ic.DERaw()+ic.xgrid.ERaw(0) + ic.ygrid.ERaw(0),ic.xgrid.ERaw(0)+ic.ygrid.ERaw(0));

    hCalICDEvE->Fill(ic.E()+ic.DE(),ic.DE());
    hCalICfDEvfE->Fill(ic.E(),ic.DE());
    hCalICPosEvE->Fill(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y());
    

    hICCalE->Fill(ic.E());
    hICCalDE->Fill(ic.DE());
    hICESumX->Fill(ic.SumE_X());
    hICESumY->Fill(ic.SumE_Y());

    hXGridMult->Fill(ic.xgrid.Mult());
    hYGridMult->Fill(ic.ygrid.Mult());
    hXMultvYMult->Fill(ic.xgrid.Mult(),ic.ygrid.Mult());
    hICXCh0->Fill(ic.xgrid.Ch(0));
    hICYCh0->Fill(ic.xgrid.Ch(0));
    hICXE1vE0->Fill(ic.xgrid.ELocal(0),ic.xgrid.ELocal(1));  
    hICYE1vE0->Fill(ic.ygrid.ELocal(0),ic.ygrid.ELocal(1)); 
    hICXE2vE0->Fill(ic.xgrid.ELocal(0),ic.xgrid.ELocal(2));  
    hICYE2vE0->Fill(ic.ygrid.ELocal(0),ic.ygrid.ELocal(2)); 
    hICXE2vE1->Fill(ic.xgrid.ELocal(1),ic.xgrid.ELocal(2));  
    hICYE2vE1->Fill(ic.ygrid.ELocal(1),ic.ygrid.ELocal(2)); 
    hICXCH1vCH0->Fill(ic.xgrid.Ch(0),ic.xgrid.Ch(1));  
    hICYCH1vCH0->Fill(ic.ygrid.Ch(0),ic.ygrid.Ch(1));  
    hICXCH2vCH0->Fill(ic.xgrid.Ch(0),ic.xgrid.Ch(2));  
    hICYCH2vCH0->Fill(ic.ygrid.Ch(0),ic.ygrid.Ch(2)); 
    hICXCH2vCH1->Fill(ic.xgrid.Ch(1),ic.xgrid.Ch(2));  
    hICYCH2vCH1->Fill(ic.ygrid.Ch(1),ic.ygrid.Ch(2)); 
    if(ic.xgrid.Mult()==1){
      hICXE0Mult1->Fill(ic.xgrid.ELocal(0));
    }
    if(ic.ygrid.Mult()==1){
      hICYE0Mult1->Fill(ic.ygrid.ELocal(0));
    }

    if(pileup && pileup->IsInside(ic.ygrid.ELocal(0),ic.ygrid.ELocal(1))){
	 hPileupChs->Fill(ic.ygrid.Ch(0),ic.ygrid.Ch(1));
       }
   
    hXvY->Fill(ic.GetHitPos().X(),ic.GetHitPos().Y());
    hXvYRaw->Fill(ic.xgrid.Ch(0),ic.ygrid.Ch(0));
    hICTheta->Fill(ic.Theta());
    hICEvTheta->Fill(ic.Theta(),ic.TotalE());
    hICEAllvTheta->Fill(ic.Theta(),ic.TotalE()+ic.SumE_X()+ic.SumE_Y());
    hICPhi->Fill(ic.Phi());
    hICT->Fill(ic.T());
    hICEvRF->Fill(rftime.TRaw(),ic.TotalE());
    hICEvT->Fill(ic.T(),ic.TotalE());


    
    
    return 1;
  }


  bool IC_Analyzer::Terminate(){

    return 1;
  }

  bool IC_Analyzer::TerminateIfLast(){
    fgRootFile->Write();
    fgRootFile->Close();
    
    return 1;
  }

  
  void IC_Analyzer::Clear(){
    
  }
  
  void RequireHI1(){
    _require_hi1=1;
  }
 void RequireHI2(){
    _require_hi2=1;
  }
  void RequireHI3(){
    _require_hi3=1;
  }
  void RequirePosEDE(){
    gRequirePosEDE=1;
  }


  void LoadGates(const std::string& input){
    TFile in(input.c_str());    
    if(in.Get("ede_hi1") && !ede_hi1)
      ede_hi1=new TCutG(*(TCutG*)in.Get("ede_hi1"));
    if(in.Get("ede_hi2") && !ede_hi2)
      ede_hi2=new TCutG(*(TCutG*)in.Get("ede_hi2"));
    if(in.Get("ede_hi3") && !ede_hi3)
      ede_hi3=new TCutG(*(TCutG*)in.Get("ede_hi3"));   
    if(in.Get("posede") && !posede)
      posede=new TCutG(*(TCutG*)in.Get("posede"));   
    if(in.Get("pileup") && !pileup)
      pileup=new TCutG(*(TCutG*)in.Get("pileup"));   
    
  
    in.Close();
  }
  
  void ClearGates(){
    if(ede_hi1)delete ede_hi1;
    if(ede_hi2)delete ede_hi2;
    if(ede_hi3)delete ede_hi3;



  }
}

#endif
