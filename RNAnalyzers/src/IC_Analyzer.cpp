
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

#include "Si_Analyzer.hpp"

using namespace RNROOT;

namespace ionchamber{
  TCutG* ede_hi1;
  TCutG* ede_hi2;
  TCutG* ede_hi3;
  TCutG* posede;
  TCutG* pileup;

  // JAB
  TCutG* Ne19;
  TCutG* F19_HE_CS;
  TCutG* F19_LE_CS;



  int  _require_hi1(0);
  int  _require_hi2(0);
  int  _require_hi3(0);
  int  gRequirePosEDE(0);

  // JAB
  static int gRequire_Ne19(0);
  static int gRequire_F19_HE_CS(0);
  static int gRequire_F19_LE_CS(0);

  int hi_check[3];
  int PosEdECheck(0);

  // JAB
  int Ne19Check(0);
  int F19HECSCheck(0);
  int F19LECSCheck(0);




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

  // JAB
  TH2D* IC_dEE_ProtCut;
  TH2D* IC_dEE_Prot2Cut;
  TH2D* IC_dEE_AlphaCut;
  TH1D* TotalEn_Raw;
  TH2D* SiPhi_vs_ICPhi;
  
  TH1D* IC_XGrid_WiresFired_WithWire0;
  TH1D* IC_XGrid_WiresFired_WithWire1;
  TH1D* IC_XGrid_WiresFired_WithWire2;
  TH1D* IC_XGrid_WiresFired_WithWire3;
  TH1D* IC_XGrid_WiresFired_WithWire4;
  TH1D* IC_XGrid_WiresFired_WithWire5;
  TH1D* IC_XGrid_WiresFired_WithWire6;
  TH1D* IC_XGrid_WiresFired_WithWire7;
  TH1D* IC_XGrid_WiresFired_WithWire8;
  TH1D* IC_XGrid_WiresFired_WithWire9;
  TH1D* IC_XGrid_WiresFired_WithWire10;
  TH1D* IC_XGrid_WiresFired_WithWire11;
  TH1D* IC_XGrid_WiresFired_WithWire12;
  TH1D* IC_XGrid_WiresFired_WithWire13;
  TH1D* IC_XGrid_WiresFired_WithWire14;
  TH1D* IC_XGrid_WiresFired_WithWire15;
  TH1D* IC_XGrid_WiresFired_WithWire16;
  TH1D* IC_XGrid_WiresFired_WithWire17;
  TH1D* IC_XGrid_WiresFired_WithWire18;
  TH1D* IC_XGrid_WiresFired_WithWire19;
  TH1D* IC_XGrid_WiresFired_WithWire20;
  TH1D* IC_XGrid_WiresFired_WithWire21;
  TH1D* IC_XGrid_WiresFired_WithWire22;
  TH1D* IC_XGrid_WiresFired_WithWire23;
  TH1D* IC_XGrid_WiresFired_WithWire24;
  TH1D* IC_XGrid_WiresFired_WithWire25;
  TH1D* IC_XGrid_WiresFired_WithWire26;
  TH1D* IC_XGrid_WiresFired_WithWire27;
  TH1D* IC_XGrid_WiresFired_WithWire28;
  TH1D* IC_XGrid_WiresFired_WithWire29;
  TH1D* IC_XGrid_WiresFired_WithWire30;
  TH1D* IC_XGrid_WiresFired_WithWire31;
  
  TH1D* IC_YGrid_WiresFired_WithWire0;
  TH1D* IC_YGrid_WiresFired_WithWire1;
  TH1D* IC_YGrid_WiresFired_WithWire2;
  TH1D* IC_YGrid_WiresFired_WithWire3;
  TH1D* IC_YGrid_WiresFired_WithWire4;
  TH1D* IC_YGrid_WiresFired_WithWire5;
  TH1D* IC_YGrid_WiresFired_WithWire6;
  TH1D* IC_YGrid_WiresFired_WithWire7;
  TH1D* IC_YGrid_WiresFired_WithWire8;
  TH1D* IC_YGrid_WiresFired_WithWire9;
  TH1D* IC_YGrid_WiresFired_WithWire10;
  TH1D* IC_YGrid_WiresFired_WithWire11;
  TH1D* IC_YGrid_WiresFired_WithWire12;
  TH1D* IC_YGrid_WiresFired_WithWire13;
  TH1D* IC_YGrid_WiresFired_WithWire14;
  TH1D* IC_YGrid_WiresFired_WithWire15;
  TH1D* IC_YGrid_WiresFired_WithWire16;
  TH1D* IC_YGrid_WiresFired_WithWire17;
  TH1D* IC_YGrid_WiresFired_WithWire18;
  TH1D* IC_YGrid_WiresFired_WithWire19;
  TH1D* IC_YGrid_WiresFired_WithWire20;
  TH1D* IC_YGrid_WiresFired_WithWire21;
  TH1D* IC_YGrid_WiresFired_WithWire22;
  TH1D* IC_YGrid_WiresFired_WithWire23;
  TH1D* IC_YGrid_WiresFired_WithWire24;
  TH1D* IC_YGrid_WiresFired_WithWire25;
  TH1D* IC_YGrid_WiresFired_WithWire26;
  TH1D* IC_YGrid_WiresFired_WithWire27;
  TH1D* IC_YGrid_WiresFired_WithWire28;
  TH1D* IC_YGrid_WiresFired_WithWire29;
  TH1D* IC_YGrid_WiresFired_WithWire30;
  TH1D* IC_YGrid_WiresFired_WithWire31;

  IC_Analyzer::IC_Analyzer():OneHitRecon(1),
			     TwoHitRecon(0)
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
    //gDirectory->mkdir("XYGridWiresFired");

    //create histograms
    fgRootFile->cd("IC/xy");
    hXvY= new TH2D("hXvY","hXvY;XChan;YChan",1024,-64,62,1024,-64,62);
    hXvYRaw= new TH2D("hXvYRaw","hXvYRaw;XChanHighestE;YChanHighestE",64,-16,47,64,-16,47);
    hICTheta = new TH1D("hICTheta","hICTheta;Theta",180,0,20);
    hICEvTheta = new TH2D("hICEvTheta","hICEvTheta;Theta;E",512,0,15,512,0,128);
    hICEAllvTheta = new TH2D("hICEAllvTheta","hICEAllvTheta;Theta;EAll",512,0,15,512,0,128);
    hICPhi = new TH1D("hICPhi","hICPhi;Phi",180,0,179);
    SiPhi_vs_ICPhi = new TH2D("SiPhi_vs_ICPhi","Si Phi vs IC Phi;Si Phi;IC Phi",360,-180,180,360,-180,180);


    fgRootFile->cd("IC/ede/rawpar");
    hRawICDEvE=new TH2D("hRawICDEvE","IRawCDEvE; E + dE[arb];dE [arb]",1024,0,4096,1024,0,4096);
    hRawICfDEvfE=new TH2D("hRawICfDEvfE","hRawICfDEvfE;E [arb];dE [arb]",1024,0,4096,1024,0,4096);
    hRawICPosEvE=new TH2D("hRawICPosEvE","hRawICPosEvE;E + dE + dEX + dEY [arb];dEX + dEY [arb]",1024,0,4095,1024,0,8124);
    IC_dEE_ProtCut = new TH2D("IC_dEE_ProtCut","IC Raw dE-E:Prot Cut; E + dE[arb];dE [arb]",1024,0,4096,1024,0,4096);
    IC_dEE_Prot2Cut = new TH2D("IC_dEE_Prot2Cut","IC Raw dE-E:Prot 2 Cut; E + dE[arb];dE [arb]",1024,0,4096,1024,0,4096);
    IC_dEE_AlphaCut = new TH2D("IC_dEE_AlphaCut","IC Raw dE-E:Alpha Cut; E + dE[arb];dE [arb]",1024,0,4096,1024,0,4096);
    fgRootFile->cd("IC/ede/calpar");
    hCalICDEvE=new TH2D("hCalICDEvE","hCalICDEvE; E + dE[arb];dE [arb]",1024,0,4095,1024,0,1023);
    hCalICfDEvfE=new TH2D("hCalICfDEvfE","hCalICfDEvfE;E [arb];dE [arb]",1024,0,4095,1024,0,1023);
    hCalICPosEvE=new TH2D("hCalICPosEvE","hCalICPosEvE;E + dE + dEX + dEY [arb];dEX + dEY [arb]",1024,0,4095,1024,0,1023);
    hICCalE = new TH1D("hICCalE","hICCalE;IC.E()",2048,0,1024);
    hICCalDE = new TH1D("hICCalDE","hICCalDE;IC.DE()",2048,0,1024);
    hICESumX = new TH1D("hICESumX","hICESumX;IC.ESumX()",4096,0,4095);
    hICESumY = new TH1D("hICESumY","hICESumY;IC.ESumY()",4096,0,4095);
    TotalEn_Raw = new TH1D("TotalEn_Raw","Total Raw Energy",1024,0,8124);

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


    fgRootFile->cd("IC");
    IC_XGrid_WiresFired_WithWire0 = new TH1D("IC_XGrid_WiresFired_WithWire0","Fired with 0 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire1 = new TH1D("IC_XGrid_WiresFired_WithWire1","Fired with 1 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire2 = new TH1D("IC_XGrid_WiresFired_WithWire2","Fired with 2 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire3 = new TH1D("IC_XGrid_WiresFired_WithWire3","Fired with 3 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire4 = new TH1D("IC_XGrid_WiresFired_WithWire4","Fired with 4 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire5 = new TH1D("IC_XGrid_WiresFired_WithWire5","Fired with 5 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire6 = new TH1D("IC_XGrid_WiresFired_WithWire6","Fired with 6 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire7 = new TH1D("IC_XGrid_WiresFired_WithWire7","Fired with 7 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire8 = new TH1D("IC_XGrid_WiresFired_WithWire8","Fired with 8 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire9 = new TH1D("IC_XGrid_WiresFired_WithWire9","Fired with 9 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire10 = new TH1D("IC_XGrid_WiresFired_WithWire10","Fired with 10 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire11 = new TH1D("IC_XGrid_WiresFired_WithWire11","Fired with 11 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire12 = new TH1D("IC_XGrid_WiresFired_WithWire12","Fired with 12 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire13 = new TH1D("IC_XGrid_WiresFired_WithWire13","Fired with 13 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire14 = new TH1D("IC_XGrid_WiresFired_WithWire14","Fired with 14 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire15 = new TH1D("IC_XGrid_WiresFired_WithWire15","Fired with 15 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire16 = new TH1D("IC_XGrid_WiresFired_WithWire16","Fired with 16 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire17 = new TH1D("IC_XGrid_WiresFired_WithWire17","Fired with 17 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire18 = new TH1D("IC_XGrid_WiresFired_WithWire18","Fired with 18 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire19 = new TH1D("IC_XGrid_WiresFired_WithWire19","Fired with 19 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire20 = new TH1D("IC_XGrid_WiresFired_WithWire20","Fired with 20 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire21 = new TH1D("IC_XGrid_WiresFired_WithWire21","Fired with 21 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire22 = new TH1D("IC_XGrid_WiresFired_WithWire22","Fired with 22 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire23 = new TH1D("IC_XGrid_WiresFired_WithWire23","Fired with 23 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire24 = new TH1D("IC_XGrid_WiresFired_WithWire24","Fired with 24 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire25 = new TH1D("IC_XGrid_WiresFired_WithWire25","Fired with 25 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire26 = new TH1D("IC_XGrid_WiresFired_WithWire26","Fired with 26 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire27 = new TH1D("IC_XGrid_WiresFired_WithWire27","Fired with 27 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire28 = new TH1D("IC_XGrid_WiresFired_WithWire28","Fired with 28 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire29 = new TH1D("IC_XGrid_WiresFired_WithWire29","Fired with 29 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire30 = new TH1D("IC_XGrid_WiresFired_WithWire30","Fired with 30 X",35,-1,34);
    IC_XGrid_WiresFired_WithWire31 = new TH1D("IC_XGrid_WiresFired_WithWire31","Fired with 31 X",35,-1,34);

    IC_YGrid_WiresFired_WithWire0 = new TH1D("IC_YGrid_WiresFired_WithWire0","Fired with 0 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire1 = new TH1D("IC_YGrid_WiresFired_WithWire1","Fired with 1 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire2 = new TH1D("IC_YGrid_WiresFired_WithWire2","Fired with 2 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire3 = new TH1D("IC_YGrid_WiresFired_WithWire3","Fired with 3 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire4 = new TH1D("IC_YGrid_WiresFired_WithWire4","Fired with 4 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire5 = new TH1D("IC_YGrid_WiresFired_WithWire5","Fired with 5 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire6 = new TH1D("IC_YGrid_WiresFired_WithWire6","Fired with 6 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire7 = new TH1D("IC_YGrid_WiresFired_WithWire7","Fired with 7 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire8 = new TH1D("IC_YGrid_WiresFired_WithWire8","Fired with 8 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire9 = new TH1D("IC_YGrid_WiresFired_WithWire9","Fired with 9 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire10 = new TH1D("IC_YGrid_WiresFired_WithWire10","Fired with 10 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire11 = new TH1D("IC_YGrid_WiresFired_WithWire11","Fired with 11 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire12 = new TH1D("IC_YGrid_WiresFired_WithWire12","Fired with 12 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire13 = new TH1D("IC_YGrid_WiresFired_WithWire13","Fired with 13 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire14 = new TH1D("IC_YGrid_WiresFired_WithWire14","Fired with 14 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire15 = new TH1D("IC_YGrid_WiresFired_WithWire15","Fired with 15 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire16 = new TH1D("IC_YGrid_WiresFired_WithWire16","Fired with 16 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire17 = new TH1D("IC_YGrid_WiresFired_WithWire17","Fired with 17 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire18 = new TH1D("IC_YGrid_WiresFired_WithWire18","Fired with 18 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire19 = new TH1D("IC_YGrid_WiresFired_WithWire19","Fired with 19 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire20 = new TH1D("IC_YGrid_WiresFired_WithWire20","Fired with 20 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire21 = new TH1D("IC_YGrid_WiresFired_WithWire21","Fired with 21 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire22 = new TH1D("IC_YGrid_WiresFired_WithWire22","Fired with 22 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire23 = new TH1D("IC_YGrid_WiresFired_WithWire23","Fired with 23 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire24 = new TH1D("IC_YGrid_WiresFired_WithWire24","Fired with 24 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire25 = new TH1D("IC_YGrid_WiresFired_WithWire25","Fired with 25 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire26 = new TH1D("IC_YGrid_WiresFired_WithWire26","Fired with 26 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire27 = new TH1D("IC_YGrid_WiresFired_WithWire27","Fired with 27 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire28 = new TH1D("IC_YGrid_WiresFired_WithWire28","Fired with 28 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire29 = new TH1D("IC_YGrid_WiresFired_WithWire29","Fired with 29 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire30 = new TH1D("IC_YGrid_WiresFired_WithWire30","Fired with 30 Y",35,-1,34);
    IC_YGrid_WiresFired_WithWire31 = new TH1D("IC_YGrid_WiresFired_WithWire31","Fired with 31 Y",35,-1,34);
    




    return 1;
  }

  void IC_Analyzer::Reset(){
    for(int i=0;i<3;i++){
      hi_check[i]=0;
    }
    PosEdECheck = 0;
    Ne19Check = 0;
    F19HECSCheck = 0;
    F19LECSCheck = 0;

  }

  bool IC_Analyzer::Process(){


    if(OneHitRecon)
      ic.ReconstructHitPos();
    //std::cout << "> One Theta : " << ic.Theta() << std::endl;
    
    if(TwoHitRecon)
      ic.ReconstructTwoPosHitPos();
    //std::cout << "> Two Theta : " << ic.Theta() << std::endl;
    /*
    // Not Raw????
    hi_check[0]= (ede_hi1 && ede_hi1->IsInside(ic.TotalE(),ic.DE()));
    //std::cout << "hi_check[0] = "<<  hi_check[0] << std::endl;
    hi_check[1]= (ede_hi2 && ede_hi2->IsInside(ic.TotalE(),ic.DE()));
    hi_check[2]= (ede_hi3 && ede_hi3->IsInside(ic.TotalE(),ic.DE()));
    */

    // Definitely Raw: 
    hi_check[0]= (ede_hi1 && ede_hi1->IsInside(ic.ERaw() + ic.DERaw(), ic.DERaw()));
    hi_check[1]= (ede_hi2 && ede_hi2->IsInside(ic.ERaw() + ic.DERaw(), ic.DERaw()));
    hi_check[2]= (ede_hi3 && ede_hi3->IsInside(ic.ERaw() + ic.DERaw(), ic.DERaw()));
    PosEdECheck = (posede && posede->IsInside(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y()));

    // JAB
    if(Ne19)
      Ne19Check = Ne19->IsInside(ic.ERaw() + ic.DERaw(), ic.DERaw());
    if(F19_HE_CS)
      F19HECSCheck = F19_HE_CS->IsInside(ic.ERaw() + ic.DERaw(), ic.DERaw());
    if(F19_LE_CS)
      F19LECSCheck = F19_LE_CS->IsInside(ic.ERaw() + ic.DERaw(), ic.DERaw());
    
    if (_require_hi1 && !hi_check[0])
      return 0;
    if (_require_hi2 && !hi_check[1])
      return 0;
    if (_require_hi3 && !hi_check[2])
      return 0;
    if(gRequire_Ne19 && !Ne19Check)
      return 0;  
    if(gRequire_F19_HE_CS && !F19HECSCheck)
      return 0;
    if(gRequire_F19_LE_CS && !F19LECSCheck)
      return 0;

    if(gRequirePosEDE && !PosEdECheck)
      return 0;

    //std::cout << "Made it here " << std::endl;
    return 1;
  }
  
  bool IC_Analyzer::ProcessFill(){

    //multiple configurations of the E-DE:    
    //SumEPos = SumXE + SumYE
    //std::cout << " Made it through this" << std::endl;
    hRawICDEvE->Fill(ic.ERaw() + ic.DERaw(), ic.DERaw());// std::cout << " Made it through this" << std::endl;
    hRawICfDEvfE->Fill(ic.ERaw(),ic.DERaw()); //std::cout << " Made it through this" << std::endl;
    /*std::cout << "-- IC Stuff Summary -----------------" << std::endl;
    std::cout << " ERaw       : " << ic.ERaw() << std::endl;
    std::cout << " DERaw      : " << ic.DERaw() << std::endl;
    std::cout << " xgrid.ERaw : " << ic.xgrid.ERaw(0) << std::endl;
    std::cout << " ygrid.ERaw : " << ic.ygrid.ERaw(0) << std::endl;
    */
    hRawICPosEvE->Fill(ic.ERaw() + ic.DERaw()+ic.xgrid.ERaw(0) + ic.ygrid.ERaw(0),ic.xgrid.ERaw(0)+ic.ygrid.ERaw(0)); //std::cout << " Made it through this" << std::endl;
    //std::cout << " Made it through this" << std::endl;
    hCalICDEvE->Fill(ic.E()+ic.DE(),ic.DE());
    hCalICfDEvfE->Fill(ic.E(),ic.DE());
    hCalICPosEvE->Fill(ic.E() + ic.DE()+ic.SumE_X()+ic.SumE_Y(),ic.SumE_X()+ic.SumE_Y());
    

    TotalEn_Raw->Fill(ic.ERaw()+ic.DERaw());
    SiPhi_vs_ICPhi->Fill(si_array.Phi_A()*TMath::RadToDeg(),ic.Phi());
    

    hICCalE->Fill(ic.E());
    hICCalDE->Fill(ic.DE());
    hICESumX->Fill(ic.SumE_X());
    hICESumY->Fill(ic.SumE_Y());

    if(ic.xgrid.Mult()>1){
      if(ic.xgrid.Ch(0) == 0){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire0->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 1){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire1->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 2){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire2->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 3){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire3->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 4){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire4->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 5){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire5->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 6){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire6->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 7){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire7->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 8){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire8->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 9){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire9->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 10){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire10->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 11){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire11->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 12){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire12->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 13){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire13->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 14){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire14->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 15){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire15->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 16){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire16->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 17){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire17->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 18){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire18->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 19){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire19->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 20){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire20->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 21){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire21->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 22){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire22->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 23){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire23->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 24){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire24->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 25){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire25->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 26){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire26->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 27){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire27->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 28){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire28->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 29){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire29->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 30){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire30->Fill(ic.xgrid.Ch(i_c));
	}
      }
      else if(ic.xgrid.Ch(0) == 31){
	for(int i_c = 0; i_c<ic.xgrid.Mult();i_c++){
	  IC_XGrid_WiresFired_WithWire31->Fill(ic.xgrid.Ch(i_c));
	}
      }
    }

    if(ic.ygrid.Mult()>1){
      if(ic.ygrid.Ch(0) == 0){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire0->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 1){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire1->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 2){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire2->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 3){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire3->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 4){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire4->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 5){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire5->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 6){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire6->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 7){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire7->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 8){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire8->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 9){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire9->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 10){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire10->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 11){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire11->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 12){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire12->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 13){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire13->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 14){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire14->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 15){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire15->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 16){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire16->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 17){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire17->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 18){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire18->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 19){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire19->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 20){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire20->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 21){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire21->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 22){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire22->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 23){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire23->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 24){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire24->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 25){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire25->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 26){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire26->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 27){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire27->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 28){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire28->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 29){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire29->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 30){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire30->Fill(ic.ygrid.Ch(i_c));
	}
      }
      else if(ic.ygrid.Ch(0) == 31){
	for(int i_c = 0; i_c<ic.ygrid.Mult();i_c++){
	  IC_YGrid_WiresFired_WithWire31->Fill(ic.ygrid.Ch(i_c));
	}
      }
    }


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

    if(silicon::protcheck){
      IC_dEE_ProtCut->Fill(ic.ERaw() + ic.DERaw(), ic.DERaw());
    }
    
    if(silicon::prot2check){
      IC_dEE_Prot2Cut->Fill(ic.ERaw() + ic.DERaw(), ic.DERaw());
    }

    if(silicon::alphacheck){
      IC_dEE_AlphaCut->Fill(ic.ERaw() + ic.DERaw(), ic.DERaw());

    }

    //std::cout << " Made it through this" << std::endl;
    
    
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
  //JAB
  void RequireNe19(int i){
    gRequire_Ne19 = i;
  }
  void RequireF19_HE_CS(int i){
    gRequire_F19_HE_CS = i;
  }
  void RequireF19_LE_CS(int i){
    gRequire_F19_LE_CS = i;
  }
  

  void LoadGates(const std::string& input){
    TFile in(input.c_str());  
    std::cout << "file loaded" << std::endl;

    // JAB
    if(in.Get("Ne19") && !Ne19)
      Ne19=new TCutG(*(TCutG*)in.Get("Ne19"));
    if(in.Get("F19_HE_CS") && !F19_HE_CS)
      F19_HE_CS=new TCutG(*(TCutG*)in.Get("F19_HE_CS"));
    if(in.Get("F19_LE_CS") && !F19_LE_CS)
      F19_LE_CS=new TCutG(*(TCutG*)in.Get("F19_LE_CS"));    
    // END JAB
    if(in.Get("ede_hi1") && !ede_hi1){
      ede_hi1=new TCutG(*(TCutG*)in.Get("ede_hi1"));
      std::cout << "ede_hi1 loaded" << std::endl;
    }
    if(in.Get("ede_hi2") && !ede_hi2)
      ede_hi2=new TCutG(*(TCutG*)in.Get("ede_hi2"));
    if(in.Get("ede_hi3") && !ede_hi3)
      ede_hi3=new TCutG(*(TCutG*)in.Get("ede_hi3"));   
    if(in.Get("posede") && !posede)
      posede=new TCutG(*(TCutG*)in.Get("posede"));   
    if(in.Get("pileup") && !pileup)
      pileup=new TCutG(*(TCutG*)in.Get("pileup"));   
    
  
    in.Close();
    std::cout << " All Gates loaded " << std::endl;
  }
  
  void ClearGates(){
    if(ede_hi1)delete ede_hi1;
    if(ede_hi2)delete ede_hi2;
    if(ede_hi3)delete ede_hi3;

    // JAB
    if(Ne19)delete Ne19;
    if(F19_HE_CS)delete F19_HE_CS;
    if(F19_LE_CS)delete F19_LE_CS;


  }
}

#endif
