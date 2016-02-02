/****************************************************
//Class: Analyzer_Template
//
//

**************************************************/
#include "Check_Alpha_Calibration.hpp"
#include "RN_Root.hpp"
#include "TGraph.h"
#include "TF1.h"
#include "TSpectrum.h"

using namespace RNROOT;

namespace Check_Alpha_Cal{
  
  TH1D *AlphaSpecRaw[2][2][16][4];
  TH1D *AlphaSpecOffset[2][2][16];//[3];
  TH1D *AlphaSpecLocal[2][2][16];//[3];
  TH1D *AlphaSpecE[2][2][16];//[3];

  TH1D *AlphaSpecRaw_Sum[2][2];
  TH1D *AlphaSpecOffset_Sum[2][2];
  TH1D *AlphaSpecLocal_Sum[2][2];
  TH1D *AlphaSpecE_Sum[2][2];

  TH2D *AlphaSpecRaw_2D[2][2];
  TH2D *AlphaSpecOffset_2D[2][2];
  TH2D *AlphaSpecLocal_2D[2][2];
  TH2D *AlphaSpecE_2D[2][2];
  

  TF1 *EnergyFit[2][2][16];
  TF1 *FinalEnergyFit[2][2];
  
  Check_Alpha_Calibration::Check_Alpha_Calibration()
  {
    LocEnCal = false;
    FinEnCal = false;
    OneStepCal = false;
    RefChan = 0;
  }
  
  
  void Check_Alpha_Calibration::Reset(){
    
  }

  bool Check_Alpha_Calibration::Begin(){

    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      Clear();
      exit(EXIT_FAILURE);
    }
    
    fgRootFile->mkdir("AlphaCalCheck");
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->mkdir("Si_A");
    gDirectory->mkdir("Si_B");
    
    gDirectory->cd("Si_A");
    gDirectory->mkdir("Front");
    gDirectory->mkdir("Back");

    gDirectory->cd("Front");
    gDirectory->mkdir("Raw_Energy");
    gDirectory->cd("Raw_Energy");
    for(int i = 0; i<16; i++){
      AlphaSpecRaw[0][0][i][0] = new TH1D(Form("AlphaSpecRaw_A_F_%d_F=B",i),Form("Raw Alphas: Det A.Fr.Ch %d Ba.Ch %d",i,i),500,500,2000);
      AlphaSpecRaw[0][0][i][1] = new TH1D(Form("AlphaSpecRaw_A_F_%d_F=B+1",i),Form("Raw Alphas: Det A.Fr.Ch %d Ba.Ch %d",i,i+1),500,500,2000);
      AlphaSpecRaw[0][0][i][2] = new TH1D(Form("AlphaSpecRaw_A_F_%d_else",i),Form("Raw Alphas: Det A.Fr.Ch %d Ba.Ch else",i),500,500,2000);
      AlphaSpecRaw[0][0][i][3] = new TH1D(Form("AlphaSpecRaw_A_F_%d_All",i),Form("Raw Alphas: Det A.Fr.Ch %d Ba.Ch Any",i),500,500,2000);
      EnergyFit[0][0][i] = new TF1(Form("AlphaFit_DetA_Front_Chan_%d",i),"pol1",50,4096);
    }
    AlphaSpecRaw_2D[0][0] = new TH2D("AlphaSpecRaw_2D_A_F","Raw Alphas Det A;Front Channel; Energy [Channels]",17,0,16,500,500,2000);
    AlphaSpecRaw_Sum[0][0] = new TH1D("AlphaSpecRaw_Sum_A_F","Raw Alpha: Det A All Fr Ch",500,500,2000);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_A");
    gDirectory->cd("Front");   
    gDirectory->mkdir("Offset");
    gDirectory->cd("Offset");
    for(int i = 0; i<16; i++){
      AlphaSpecOffset[0][0][i] = new TH1D(Form("AlphaSpecOffset_A_F_%d",i),Form("Offset Alphas: Det A.Fr.Ch %d",i),500,500,2000);
    }
    AlphaSpecOffset_2D[0][0] = new TH2D("AlphaSpecOffset_2D_A_F","Offset Alphas Det A;Front Channel; Energy[Ch+Offset]",17,0,16,500,500,2000);
    AlphaSpecOffset_Sum[0][0] = new TH1D("AlphaSpecOffset_Sum_A_F","Offset Alpha: Det A All Fr Ch",500,500,2000);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_A");
    gDirectory->cd("Front");   
    gDirectory->mkdir("Local");
    gDirectory->cd("Local");
    for(int i = 0; i<16; i++){
      AlphaSpecLocal[0][0][i] = new TH1D(Form("AlphaSpecLocal_A_F_%d",i),Form("Local Alphas: Det A.Fr.Ch %d",i),500,500,2000);
    }
    AlphaSpecLocal_2D[0][0] = new TH2D("AlphaSpecLocal_2D_A_F","Local Alphas Det A;Front Channel;Energy[Channels]",17,0,16,500,500,2000);
    AlphaSpecLocal_Sum[0][0] = new TH1D("AlphaSpecLocal_Sum_A_F","Local Alpha: Det A All Fr Ch",500,500,2000);
    FinalEnergyFit[0][0] = new TF1("EnergyFit_DetA_Front","pol1",50,4096);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_A");
    gDirectory->cd("Front");      
    gDirectory->mkdir("Calibrated_Energy");
    gDirectory->cd("Calibrated_Energy");
    for(int i = 0; i<16; i++){
      AlphaSpecE[0][0][i] = new TH1D(Form("AlphaSpecE_A_F_%d",i),Form("Final Energy Alphas: Det A.Fr.Ch %d",i),500,0,15);
    }
    AlphaSpecE_2D[0][0] = new TH2D("AlphaSpecE_2D_A_F","Energy Alphas Det A;Front Channel;Energy [MeV]",17,0,16,500,0,15);
    AlphaSpecE_Sum[0][0] = new TH1D("AlphaSpecE_Sum_A_F","Energy Alpha: Det A All Fr Ch",500,0,15);

    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_A");
    gDirectory->cd("Back");
    gDirectory->mkdir("Raw_Energy");
    gDirectory->cd("Raw_Energy");
    for(int i = 0; i<16; i++){
      AlphaSpecRaw[0][1][i][0] = new TH1D(Form("AlphaSpecRaw_A_B_%d_B=F",i),Form("Raw Alphas: Det A.Ba.Ch %d Fr.Ch %d",i,i),500,500,2000);
      AlphaSpecRaw[0][1][i][1] = new TH1D(Form("AlphaSpecRaw_A_B_%d_B=F+1",i),Form("Raw Alphas: Det A.Ba.Ch %d Fr.Ch %d",i,i+1),500,500,2000);
      AlphaSpecRaw[0][1][i][2] = new TH1D(Form("AlphaSpecRaw_A_B_%d_else",i),Form("Raw Alphas: Det A.Ba.Ch %d Fr.Ch else",i),500,500,2000);
      AlphaSpecRaw[0][1][i][3] = new TH1D(Form("AlphaSpecRaw_A_B_%d_All",i),Form("Raw Alphas: Det A.Ba.Ch %d Fr.Ch Any",i),500,500,2000);
      EnergyFit[0][1][i] = new TF1(Form("AlphaFit_DetA_Back_Chan_%d",i),"pol1",50,4096);
    }
    AlphaSpecRaw_2D[0][1] = new TH2D("AlphaSpecRaw_2D_A_B","Raw Alphas Det A;Back Channel; Energy [Channels]",17,0,16,500,500,2000);
    AlphaSpecRaw_Sum[0][1] = new TH1D("AlphaSpecRaw_Sum_A_B","Raw Alpha: Det A All Ba Ch",500,500,2000);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_A");
    gDirectory->cd("Back");   
    gDirectory->mkdir("Offset");
    gDirectory->cd("Offset");
    for(int i = 0; i<16; i++){
      AlphaSpecOffset[0][1][i] = new TH1D(Form("AlphaSpecOffset_A_B_%d",i),Form("Offset Alphas: Det A.Ba.Ch %d",i),500,500,2000);
    }
    AlphaSpecOffset_2D[0][1] = new TH2D("AlphaSpecOffset_2D_A_B","Offset Alphas Det A;Back Channel; Energy[Ch+Offset]",17,0,16,500,500,2000);
    AlphaSpecOffset_Sum[0][1] = new TH1D("AlphaSpecOffset_Sum_A_B","Offset Alpha: Det A All Ba Ch",500,500,2000);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_A");
    gDirectory->cd("Back");   
    gDirectory->mkdir("Local");
    gDirectory->cd("Local");
    for(int i = 0; i<16; i++){
      AlphaSpecLocal[0][1][i] = new TH1D(Form("AlphaSpecLocal_A_B_%d",i),Form("Local Alphas: Det A.Ba.Ch %d",i),500,500,2000);
    }
    AlphaSpecLocal_2D[0][1] = new TH2D("AlphaSpecLocal_2D_A_B","Local Alphas Det A;Back Channel;Energy[Channels]",17,0,16,500,500,2000);
    AlphaSpecLocal_Sum[0][1] = new TH1D("AlphaSpecLocal_Sum_A_B","Local Alpha: Det A All Ba Ch",500,500,2000);
    FinalEnergyFit[0][1] = new TF1("EnergyFit_DetA_Back","pol1",50,4096);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_A");
    gDirectory->cd("Back");    
    gDirectory->mkdir("Calibrated_Energy");
    gDirectory->cd("Calibrated_Energy");
    for(int i = 0; i<16; i++){
      AlphaSpecE[0][1][i] = new TH1D(Form("AlphaSpecE_A_B_%d",i),Form("Final Energy Alphas: Det A.Ba.Ch %d",i),500,0,15);
    }
    AlphaSpecE_2D[0][1] = new TH2D("AlphaSpecE_2D_A_B","Energy Alphas Det A;Back Channel;Energy [MeV]",17,0,16,500,0,15);
    AlphaSpecE_Sum[0][1] = new TH1D("AlphaSpecEnergy_Sum_A_B","Energy Alpha: Det A All Ba Ch",500,0,15);

    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->mkdir("Front");
    gDirectory->mkdir("Back");
    gDirectory->cd("Front");
    gDirectory->mkdir("Raw_Energy");
    gDirectory->cd("Raw_Energy");
    for(int i = 0; i<16; i++){
      AlphaSpecRaw[1][0][i][0] = new TH1D(Form("AlphaSpecRaw_B_F_%d_F=B",i),Form("Raw Alphas: Det B.Fr.Ch %d Ba.Ch %d",i,i),500,500,2000);
      AlphaSpecRaw[1][0][i][1] = new TH1D(Form("AlphaSpecRaw_B_F_%d_F=B+1",i),Form("Raw Alphas: Det B.Fr.Ch %d Ba.Ch %d",i,i+1),500,500,2000);
      AlphaSpecRaw[1][0][i][2] = new TH1D(Form("AlphaSpecRaw_B_F_%d_else",i),Form("Raw Alphas: Det B.Fr.Ch %d Ba.Ch else",i),500,500,2000);
      AlphaSpecRaw[1][0][i][3] = new TH1D(Form("AlphaSpecRaw_B_F_%d_All",i),Form("Raw Alphas: Det B.Fr.Ch %d Ba.Ch Any",i),500,500,2000);
      EnergyFit[1][0][i] = new TF1(Form("AlphaFit_DetB_Front_Chan_%d",i),"pol1",50,4096);

    }
    AlphaSpecRaw_2D[1][0] = new TH2D("AlphaSpecRaw_2D_B_F","Raw Alphas Det B;Front Channel; Energy [Channels]",17,0,16,500,500,2000);
    AlphaSpecRaw_Sum[1][0] = new TH1D("AlphaSpecRaw_Sum_B_F","Raw Alpha: Det B All Fr Ch",500,500,2000);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Front");   
    gDirectory->mkdir("Offset");
    gDirectory->cd("Offset");
    for(int i = 0; i<16; i++){
      AlphaSpecOffset[1][0][i] = new TH1D(Form("AlphaSpecOffset_B_F_%d",i),Form("Offset Alphas: Det B.Fr.Ch %d",i),500,500,2000);
    }
    AlphaSpecOffset_2D[1][0] = new TH2D("AlphaSpecOffset_2D_B_F","Offset Alphas Det B;Front Channel; Energy[Ch+Offset]",17,0,16,500,500,2000);
    AlphaSpecOffset_Sum[1][0] = new TH1D("AlphaSpecOffset_Sum_B_F","Offset Alpha: Det B All Fr Ch",500,500,2000);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Front");   
    gDirectory->mkdir("Local");
    gDirectory->cd("Local");
    for(int i = 0; i<16; i++){
      AlphaSpecLocal[1][0][i] = new TH1D(Form("AlphaSpecLocal_B_F_%d",i),Form("Local Alphas: Det B.Fr.Ch %d",i),500,500,2000);
    }
    AlphaSpecLocal_2D[1][0] = new TH2D("AlphaSpecLocal_2D_B_F","Local Alphas Det B;Front Channel;Energy[Channels]",17,0,16,500,500,2000);
    AlphaSpecLocal_Sum[1][0] = new TH1D("AlphaSpecLocal_Sum_B_F","Local Alpha: Det B All Fr Ch",500,500,2000);
    FinalEnergyFit[1][0] = new TF1("EnergyFit_DetB_Front","pol1",50,4096);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Front");      
    gDirectory->mkdir("Calibrated_Energy");
    gDirectory->cd("Calibrated_Energy");
    for(int i = 0; i<16; i++){
      AlphaSpecE[1][0][i] = new TH1D(Form("AlphaSpecE_B_F_%d",i),Form("Final Energy Alphas: Det B.Fr.Ch %d",i),500,0,15);
    }
    AlphaSpecE_2D[1][0] = new TH2D("AlphaSpecE_2D_B_F","Energy Alphas Det B;Front Channel;Energy [MeV]",17,0,16,500,0,15);
    AlphaSpecE_Sum[1][0] = new TH1D("AlphaSpecE_Sum_B_F","Energy Alpha: Det B All Fr Ch",500,0,15);


    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Back");
    gDirectory->mkdir("Raw_Energy");
    gDirectory->cd("Raw_Energy");
    for(int i = 0; i<16; i++){
      AlphaSpecRaw[1][1][i][0] = new TH1D(Form("AlphaSpecRaw_B_B_%d_B=F",i),Form("Raw Alphas: Det B.Ba.Ch %d Fr.Ch %d",i,i),500,500,2000);
      AlphaSpecRaw[1][1][i][1] = new TH1D(Form("AlphaSpecRaw_B_B_%d_B=F+1",i),Form("Raw Alphas: Det B.Ba.Ch %d Fr.Ch %d",i,i+1),500,500,2000);
      AlphaSpecRaw[1][1][i][2] = new TH1D(Form("AlphaSpecRaw_B_B_%d_else",i),Form("Raw Alphas: Det B.Ba.Ch %d else",i),500,500,2000);
      AlphaSpecRaw[1][1][i][3] = new TH1D(Form("AlphaSpecRaw_B_B_%d_All",i),Form("Raw Alphas: Det B.Ba.Ch %d Any",i),500,500,2000);
      EnergyFit[1][1][i] = new TF1(Form("AlphaFit_DetB_Back_Chan_%d",i),"pol1",50,4096);
    }
    AlphaSpecRaw_2D[1][1] = new TH2D("AlphaSpecRaw_2D_B_B","Raw Alphas Det B;Back Channel; Energy [Channels]",17,0,16,500,500,2000);
    AlphaSpecRaw_Sum[1][1] = new TH1D("AlphaSpecRaw_Sum_B_B","Raw Alpha: Det B All Ba Ch",500,500,2000);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Back");   
    gDirectory->mkdir("Offset");
    gDirectory->cd("Offset");
    for(int i = 0; i<16; i++){
      AlphaSpecOffset[1][1][i] = new TH1D(Form("AlphaSpecOffset_B_B_%d",i),Form("Offset Alphas: Det B.Ba.Ch %d",i),500,500,2000);
    }
    AlphaSpecOffset_2D[1][1] = new TH2D("AlphaSpecOffset_2D_B_B","Offset Alphas Det B;Back Channel; Energy[Ch+Offset]",17,0,16,500,500,2000);
    AlphaSpecOffset_Sum[1][1] = new TH1D("AlphaSpecOffset_Sum_B_B","Offset Alpha: Det B All Ba Ch",500,500,2000);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Back");   
    gDirectory->mkdir("Local");
    gDirectory->cd("Local");
    for(int i = 0; i<16; i++){
      AlphaSpecLocal[1][1][i] = new TH1D(Form("AlphaSpecLocal_B_B_%d",i),Form("Local Alphas: Det B.Ba.Ch %d",i),500,500,2000);
    }
    AlphaSpecLocal_2D[1][1] = new TH2D("AlphaSpecLocal_2D_B_B","Local Alphas Det B;Back Channel;Energy[Channels]",17,0,16,500,500,2000);
    AlphaSpecLocal_Sum[1][1] = new TH1D("AlphaSpecLocal_Sum_B_B","Local Alpha: Det B All Ba Ch",500,500,2000);
    FinalEnergyFit[1][1] = new TF1("EnergyFit_DetB_Back","pol1",50,4096);
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Back");    
    gDirectory->mkdir("Calibrated_Energy");
    gDirectory->cd("Calibrated_Energy");
    for(int i = 0; i<16; i++){
      AlphaSpecE[1][1][i] = new TH1D(Form("AlphaSpecE_B_B_%d",i),Form("Final Energy Alphas: Det B.Ba.Ch %d",i),500,0,15);
    }
    AlphaSpecE_2D[1][1] = new TH2D("AlphaSpecE_2D_B_B","Energy Alphas Det B;Back Channel;Energy [MeV]",17,0,16,500,0,15);
    AlphaSpecE_Sum[1][1] = new TH1D("AlphaSpecE_Sum_B_B","Energy Alpha: Det B All Ba Ch",500,0,15);
    
    return 1;
    
  }
  
  bool Check_Alpha_Calibration::Process(){
    
    return 1;
  }
  
  bool Check_Alpha_Calibration::ProcessFill(){
    int FrChA = -1;
    int BaChA = -1;
    int FrChB = -1;
    int BaChB = -1;
    int FBSwitchA = -1;
    int FBSwitchB = -1;
    
    FrChA = int(si_[0].front.Ch());
    BaChA = int(si_[0].back.Ch());
    FrChB = int(si_[1].front.Ch());
    BaChB = int(si_[1].back.Ch());
    
    if(FrChA >=0 && BaChA >=0){
      
      FBSwitchA = 4;

      if(BaChA == FrChA)
	FBSwitchA = 1;
      if(BaChA == FrChA+1)
	FBSwitchA = 2;
      if(FrChA == BaChA+1)
	FBSwitchA = 3;
      
      switch(FBSwitchA){
      case 1:
	AlphaSpecRaw[0][0][FrChA][0]->Fill(si_[0].front.ERaw());
	AlphaSpecRaw[0][1][BaChA][0]->Fill(si_[0].back.ERaw());
	break;
      case 2:
	AlphaSpecRaw[0][1][BaChA][1]->Fill(si_[0].back.ERaw());
	AlphaSpecRaw[0][0][FrChA][2]->Fill(si_[0].front.ERaw());
	break;
      case 3:
	AlphaSpecRaw[0][0][FrChA][1]->Fill(si_[0].front.ERaw());
	AlphaSpecRaw[0][1][BaChA][2]->Fill(si_[0].back.ERaw());
	break;
      case 4:
	AlphaSpecRaw[0][0][FrChA][2]->Fill(si_[0].front.ERaw());
	AlphaSpecRaw[0][1][BaChA][2]->Fill(si_[0].back.ERaw());

	
	AlphaSpecRaw_Sum[0][0]->Fill(si_[0].front.ERaw());
	AlphaSpecRaw_Sum[0][1]->Fill(si_[0].back.ERaw());
	AlphaSpecOffset_Sum[0][0]->Fill(si_[0].front.EOffset());
	AlphaSpecOffset_Sum[0][1]->Fill(si_[0].back.EOffset());
	AlphaSpecLocal_Sum[0][0]->Fill(si_[0].front.ELocal());
	AlphaSpecLocal_Sum[0][1]->Fill(si_[0].back.ELocal());
	AlphaSpecE_Sum[0][0]->Fill(si_[0].front.E());
	AlphaSpecE_Sum[0][1]->Fill(si_[0].back.E());
	break;
      default:
	break;
      }

      
      AlphaSpecRaw[0][0][FrChA][3]->Fill(si_[0].front.ERaw());
      AlphaSpecRaw[0][1][BaChA][3]->Fill(si_[0].back.ERaw());

      AlphaSpecOffset[0][0][FrChA]->Fill(si_[0].front.EOffset());
      AlphaSpecLocal[0][0][FrChA]->Fill(si_[0].front.ELocal());
      AlphaSpecE[0][0][FrChA]->Fill(si_[0].front.E());
      AlphaSpecOffset[0][1][BaChA]->Fill(si_[0].back.EOffset());
      AlphaSpecLocal[0][1][BaChA]->Fill(si_[0].back.ELocal());
      AlphaSpecE[0][1][BaChA]->Fill(si_[0].back.E());

      AlphaSpecRaw_2D[0][0]->Fill(FrChA,si_[0].front.ERaw());
      AlphaSpecRaw_2D[0][1]->Fill(BaChA,si_[0].back.ERaw());
      AlphaSpecOffset_2D[0][0]->Fill(FrChA,si_[0].front.EOffset());
      AlphaSpecOffset_2D[0][1]->Fill(BaChA,si_[0].back.EOffset());
      AlphaSpecLocal_2D[0][0]->Fill(FrChA,si_[0].front.ELocal());
      AlphaSpecLocal_2D[0][1]->Fill(BaChA,si_[0].back.ELocal());
      AlphaSpecE_2D[0][0]->Fill(FrChA,si_[0].front.E());
      AlphaSpecE_2D[0][1]->Fill(BaChA,si_[0].back.E());
      
    }
    if(FrChB >=0 && BaChB>=0){

      FBSwitchB = 4;

      if(BaChB == FrChB)
	FBSwitchB = 1;
      if(BaChB == FrChB+1)
	FBSwitchB = 2;
      if(FrChB == BaChB+1)
	FBSwitchB = 3;
      
      switch(FBSwitchB){
      case 1:
	AlphaSpecRaw[1][0][FrChB][0]->Fill(si_[1].front.ERaw());
	AlphaSpecRaw[1][1][BaChB][0]->Fill(si_[1].back.ERaw());
	break;
      case 2:
	AlphaSpecRaw[1][1][BaChB][1]->Fill(si_[1].back.ERaw());
	AlphaSpecRaw[1][0][FrChB][2]->Fill(si_[1].front.ERaw());
	break;
      case 3:
	AlphaSpecRaw[1][0][FrChB][1]->Fill(si_[1].front.ERaw());
	AlphaSpecRaw[1][1][BaChB][2]->Fill(si_[1].back.ERaw());
	break;
      case 4:
	AlphaSpecRaw[1][0][FrChB][2]->Fill(si_[1].front.ERaw());
	AlphaSpecRaw[1][1][BaChB][2]->Fill(si_[1].back.ERaw());

	
	AlphaSpecRaw_Sum[1][0]->Fill(si_[1].front.ERaw());
	AlphaSpecRaw_Sum[1][1]->Fill(si_[1].back.ERaw());
	AlphaSpecOffset_Sum[1][0]->Fill(si_[1].front.EOffset());
	AlphaSpecOffset_Sum[1][1]->Fill(si_[1].back.EOffset());
	AlphaSpecLocal_Sum[1][0]->Fill(si_[1].front.ELocal());
	AlphaSpecLocal_Sum[1][1]->Fill(si_[1].back.ELocal());
	AlphaSpecE_Sum[1][0]->Fill(si_[1].front.E());
	AlphaSpecE_Sum[1][1]->Fill(si_[1].back.E());
	break;
      default:
	break;
      }



      AlphaSpecRaw[1][0][FrChB][3]->Fill(si_[1].front.ERaw());
      AlphaSpecRaw[1][1][BaChB][3]->Fill(si_[1].back.ERaw());

      AlphaSpecOffset[1][0][FrChB]->Fill(si_[1].front.EOffset());
      AlphaSpecLocal[1][0][FrChB]->Fill(si_[1].front.ELocal());
      AlphaSpecE[0][1][FrChB]->Fill(si_[1].front.E());
      AlphaSpecOffset[1][1][BaChB]->Fill(si_[1].back.EOffset());
      AlphaSpecLocal[1][1][BaChB]->Fill(si_[1].back.ELocal());
      AlphaSpecE[1][1][BaChB]->Fill(si_[1].back.E());

      AlphaSpecRaw_2D[1][0]->Fill(FrChB,si_[1].front.ERaw());
      AlphaSpecRaw_2D[1][1]->Fill(BaChB,si_[1].back.ERaw());
      AlphaSpecOffset_2D[1][0]->Fill(FrChB,si_[1].front.EOffset());
      AlphaSpecOffset_2D[1][1]->Fill(BaChB,si_[1].back.EOffset());
      AlphaSpecLocal_2D[1][0]->Fill(FrChB,si_[1].front.ELocal());
      AlphaSpecLocal_2D[1][1]->Fill(BaChB,si_[1].back.ELocal());
      AlphaSpecE_2D[1][0]->Fill(FrChB,si_[1].front.E());
      AlphaSpecE_2D[1][1]->Fill(BaChB,si_[1].back.E());

    }
    return 1;
  }
  


  bool Check_Alpha_Calibration::Terminate(){
    
    
  


    //======================================
    // Make file to put slope and such into:
    ofstream Output;
    Output.open("AlphaFit_Params.out");
    if(!Output.is_open()){
      std::cout << "> Could not open the alpha param output file." << std::endl;
      std::cout << "> aborting fit. " << std::endl;
      return 0;
    }						
    TString DetName;
    
    if(OneStepCal){

      //=============================
      // Declare thorium peak values:
      //Double_t Th228Peaks[6] = {5.42315,5.68537,6.05,6.28808,6.77803,8.74886};
      //Double_t Th228Peaks_5[5] = {5.42315,5.68537,6.28808,6.77803,8.74886};

      // These have proton energy through dead layer.  More Correct.
      Double_t Th228Peaks[6] = {5.28624,5.55238,5.92206,6.16311,6.65879,8.64765};
      Double_t Th228Peaks_5[5] = {5.28624,5.55238,6.16311,6.65879,8.64765};

      //========================
      // Declare some root stuff:
      TSpectrum *FrontSpec[2];
      TSpectrum *BackSpec[2];
      TGraph *FrontGraph[2];
      TGraph *BackGraph[2];
      TCanvas *FitCanvas = new TCanvas("FitCanvas","FitCanvas",800,10,700,500);
      TCanvas *FitCanvas2 = new TCanvas("FitCanvas2","FitCanvas2",10,10,700,500);
      
      for(int i = 0; i < 2; i++){
	FrontSpec[i] = new TSpectrum();
	BackSpec[i] = new TSpectrum();
	FrontGraph[i] = new TGraph();
	BackGraph[i] = new TGraph();
      }
      
      //================================
      // Declare some regular variables:
      Int_t N_InFG[2] = {0,0};
      Int_t N_InBG[2] = {0,0};
      char a;
      
     
      //=============================
      // Begin loop over 16 channels:
      for(Int_t i = 0; i <16; i++){
	
	//=====================================================
	  // At the beginning of the loop, initialize everything:
	  for(Int_t j = 0; j < 2; j++){
	    
	    //=========================================
	    // Get the number of points in the TGraphs:
	    N_InFG[j] = FrontGraph[j]->GetN();
	    N_InBG[j] = BackGraph[j]->GetN();
	    
	    //===================================================================================
	    // If there are a non zero amount of points in the TGraphs, remove all of the points:
	    if(N_InFG[j]>0)
	      for(Int_t k = N_InFG[j]; k>=0; k--)
		FrontGraph[j]->RemovePoint(k);
	    
	    if(N_InBG[j]>0)
	      for(Int_t k = N_InBG[j]; k>=0;  k--)
		BackGraph[j]->RemovePoint(k);
	  }// end loop over j = 0,1

	  //==================================================
	  // At this point everything is initialized.  
	  // Now I want to get the stuff from the histograms:
	  N_InFG[0] = FrontSpec[0]->Search(AlphaSpecRaw[0][0][i][2],2,"",0.15);
	  N_InFG[1] = FrontSpec[1]->Search(AlphaSpecRaw[1][0][i][2],2,"",0.15);
      
	  N_InBG[0] = BackSpec[0]->Search(AlphaSpecRaw[0][1][i][2],2,"",0.15);
	  N_InBG[1] = BackSpec[1]->Search(AlphaSpecRaw[1][1][i][2],2,"",0.15);
      
	  Float_t *PeaksFront_X[2];
	  Float_t *PeaksBack_X[2];
	  for(Int_t j = 0; j <2; j++){
	    PeaksFront_X[j] = FrontSpec[j]->GetPositionX();
	    PeaksBack_X[j] = BackSpec[j]->GetPositionX();
	  }

	  std::vector<Float_t> SortedPeaksF_A;
	  std::vector<Float_t> SortedPeaksF_B;
	  std::vector<Float_t> SortedPeaksB_A;
	  std::vector<Float_t> SortedPeaksB_B;

	  for(Int_t j = 0; j < N_InFG[0]; j++){
	    SortedPeaksF_A.push_back(PeaksFront_X[0][j]);
	  }
	  for(int j = 0; j < N_InFG[1]; j++){
	    SortedPeaksF_B.push_back(PeaksFront_X[1][j]);
	  }
	  for(int j = 0; j < N_InBG[0]; j++){
	    SortedPeaksB_A.push_back(PeaksBack_X[0][j]);
	  }
	  for(int j = 0; j < N_InBG[1]; j++){
	    SortedPeaksB_B.push_back(PeaksBack_X[1][j]);
	  }

	  //======================
	  // Sort the peak values:
	  std::sort(SortedPeaksF_A.begin(),SortedPeaksF_A.end());
	  std::sort(SortedPeaksB_A.begin(),SortedPeaksB_A.end());
	  std::sort(SortedPeaksF_B.begin(),SortedPeaksF_B.end());
	  std::sort(SortedPeaksB_B.begin(),SortedPeaksB_B.end());

	  for(int j = 0; j < N_InFG[0]; j++){
	    if(N_InFG[0] == 6){
	      FrontGraph[0]->SetPoint(j,SortedPeaksF_A[j],Th228Peaks[j]);
	    }
	    else if (N_InFG[0] == 5){
	      FrontGraph[0]->SetPoint(j,SortedPeaksF_A[j],Th228Peaks_5[j]);
	    }
	    else{}
	  }

	  for(int j = 0; j < N_InFG[1]; j++){
	    if(N_InFG[1] == 6){
	      FrontGraph[1]->SetPoint(j,SortedPeaksF_B[j],Th228Peaks[j]);
	    }
	    else if (N_InFG[1] == 5){
	      FrontGraph[1]->SetPoint(j,SortedPeaksF_B[j],Th228Peaks_5[j]);
	    }
	    else{}
	  }

	  for(int j = 0; j < N_InBG[0]; j++){
	    if(N_InBG[0] == 6){
	      BackGraph[0]->SetPoint(j,SortedPeaksB_A[j],Th228Peaks[j]);
	    }
	    else if (N_InBG[0] == 5){
	      BackGraph[0]->SetPoint(j,SortedPeaksB_A[j],Th228Peaks_5[j]);
	    }
	    else{}
	  }

	  for(int j = 0; j < N_InBG[1]; j++){
	    if(N_InBG[1] == 6){
	      BackGraph[1]->SetPoint(j,SortedPeaksB_B[j],Th228Peaks[j]);
	    }
	    else if (N_InBG[1] == 5){
	      BackGraph[1]->SetPoint(j,SortedPeaksB_B[j],Th228Peaks_5[j]);
	    }
	    else{}
	  }

	  // Now all of the plots are made, we can go ahead and show them and fit them.

	  if(FrontGraph[0]->GetN()>0){
	    std::cout << "> Fitting Det A Front Channel " << i << " ------------" << std::endl;
	    FitCanvas->cd();
	    AlphaSpecRaw[0][0][i][2]->Draw();
	    FitCanvas->Update();
	    FitCanvas2->cd();
	    FrontGraph[0]->Draw("A*");
	    FrontGraph[0]->Fit(EnergyFit[0][0][i],"Q");
	    FitCanvas2->Update();
	    std::cout << "> Your values are : " << std::endl;
	    std::cout << ">  a0 = " << EnergyFit[0][0][i]->GetParameter(0) << std::endl;
	    std::cout << ">  a1 = " << EnergyFit[0][0][i]->GetParameter(1) << std::endl;
	    std::cout << "> Enter a character to move on: ";
	    std::cin >> a;
	    DetName = si_[0].GetName();
	    Output << DetName+".front.a0[" << i << "] " << EnergyFit[0][0][i]->GetParameter(0)<<"\n"; 
	    Output << DetName+".front.a1[" << i << "] " << EnergyFit[0][0][i]->GetParameter(1)<<"\n"; 
	  }
      
      
	  if(BackGraph[0]->GetN()>0){
	    std::cout << "> Fitting Det A Back Channel " << i << " ------------" << std::endl;
	    FitCanvas->cd();
	    AlphaSpecRaw[0][1][i][2]->Draw();
	    FitCanvas->Update();
	    FitCanvas2->cd();
	    BackGraph[0]->Draw("A*");
	    BackGraph[0]->Fit(EnergyFit[0][1][i],"Q");
	    FitCanvas2->Update();
	    std::cout << "> Your values are : " << std::endl;
	    std::cout << ">  a0 = " << EnergyFit[0][1][i]->GetParameter(0) << std::endl;
	    std::cout << ">  a1 = " << EnergyFit[0][1][i]->GetParameter(1) << std::endl;
	    std::cout << "> Enter a character to move on: ";
	    std::cin >> a;
	    DetName = si_[0].GetName();
	    Output << DetName+".back.a0[" << i << "] " << EnergyFit[0][1][i]->GetParameter(0)<<"\n"; 
	    Output << DetName+".back.a1[" << i << "] " << EnergyFit[0][1][i]->GetParameter(1)<<"\n"; 
	  }

	  if(FrontGraph[1]->GetN()>0){
	    std::cout << "> Fitting Det B Front Channel " << i << " ------------" << std::endl;
	    FitCanvas->cd();
	    AlphaSpecRaw[1][0][i][2]->Draw();
	    FitCanvas->Update();
	    FitCanvas2->cd();
	    FrontGraph[1]->Draw("A*");
	    FrontGraph[1]->Fit(EnergyFit[1][0][i],"Q");
	    FitCanvas2->Update();
	    std::cout << "> Your values are : " << std::endl;
	    std::cout << ">  a0 = " << EnergyFit[1][0][i]->GetParameter(0) << std::endl;
	    std::cout << ">  a1 = " << EnergyFit[1][0][i]->GetParameter(1) << std::endl;
	    std::cout << "> Enter a character to move on: ";
	    std::cin >> a;
	    DetName = si_[1].GetName();
	    Output << DetName+".front.a0[" << i << "] " << EnergyFit[1][0][i]->GetParameter(0)<<"\n"; 
	    Output << DetName+".front.a1[" << i << "] " << EnergyFit[1][0][i]->GetParameter(1)<<"\n"; 
	  }

	  if(BackGraph[1]->GetN()>0){
	    std::cout << "> Fitting Det B Back Channel " << i << " ------------" << std::endl;
	    FitCanvas->cd();
	    AlphaSpecRaw[1][1][i][2]->Draw();
	    FitCanvas->Update();
	    FitCanvas2->cd();
	    BackGraph[1]->Draw("A*");
	    BackGraph[1]->Fit(EnergyFit[1][1][i],"Q");
	    FitCanvas2->Update();
	    std::cout << "> Your values are : " << std::endl;
	    std::cout << ">  a0 = " << EnergyFit[1][1][i]->GetParameter(0) << std::endl;
	    std::cout << ">  a1 = " << EnergyFit[1][1][i]->GetParameter(1) << std::endl;
	    std::cout << "> Enter a character to move on: ";
	    std::cin >> a;
	    DetName = si_[1].GetName();
	    Output << DetName+".back.a0[" << i << "] " << EnergyFit[1][1][i]->GetParameter(0)<<"\n"; 
	    Output << DetName+".back.a1[" << i << "] " << EnergyFit[1][1][i]->GetParameter(1)<<"\n"; 
	  }


	}

	


    }
    else{
      if(FinEnCal){
	//=============================
	// Declare thorium peak values:
	//Double_t Th228Peaks[6] = {5.42315,5.68537,6.05,6.28808,6.77803,8.74886};
	//Double_t Th228Peaks_5[5] = {5.42315,5.68537,6.28808,6.77803,8.74886};

	// These have proton energy through dead layer.  More Correct.
	Double_t Th228Peaks[6] = {5.28624,5.55238,5.92206,6.16311,6.65879,8.64765};
	Double_t Th228Peaks_5[5] = {5.28624,5.55238,6.16311,6.65879,8.64765};
	
	//========================
	// Declare some root stuff:
	TSpectrum *FrontSpec[2];
	TSpectrum *BackSpec[2];
	TGraph *FrontGraph[2];
	TGraph *BackGraph[2];
	TCanvas *FitCanvas = new TCanvas("FitCanvas","FitCanvas",800,10,700,500);
	TCanvas *FitCanvas2 = new TCanvas("FitCanvas2","FitCanvas2",10,10,700,500);
	//TCanvas FitCanvas;
	// FitCanvas->Divide(1,2);
    
	for(int i = 0; i < 2; i++){
	  FrontSpec[i] = new TSpectrum();
	  BackSpec[i] = new TSpectrum();
	  FrontGraph[i] = new TGraph();
	  BackGraph[i] = new TGraph();
	}

	//================================
	// Declare some regular variables:
	Int_t N_InFG[2] = {0,0};
	Int_t N_InBG[2] = {0,0};
	char a;

	//=====================================================
	// At the beginning of the loop, initialize everything:
	for(Int_t j = 0; j < 2; j++){
	
	  //=========================================
	  // Get the number of points in the TGraphs:
	  N_InFG[j] = FrontGraph[j]->GetN();
	  N_InBG[j] = BackGraph[j]->GetN();
	

	  //===================================================================================
	  // If there are a non zero amount of points in the TGraphs, remove all of the points:
	  if(N_InFG[j]>0)
	    for(Int_t k = N_InFG[j]; k>=0; k--)
	      FrontGraph[j]->RemovePoint(k);

	  if(N_InBG[j]>0)
	    for(Int_t k = N_InBG[j]; k>=0;  k--)
	      BackGraph[j]->RemovePoint(k);
	  
	}
	
	//==================================================
	// At this point everything is initialized.  
	// Now I want to get the stuff from the histograms:
	N_InFG[0] = FrontSpec[0]->Search(AlphaSpecLocal_Sum[0][0],2,"",0.15);
	N_InFG[1] = FrontSpec[1]->Search(AlphaSpecLocal_Sum[1][0],2,"",0.15);
      
	N_InBG[0] = BackSpec[0]->Search(AlphaSpecLocal_Sum[0][1],2,"",0.15);
	N_InBG[1] = BackSpec[1]->Search(AlphaSpecLocal_Sum[1][1],2,"",0.15);
      
	Float_t *PeaksFront_X[2];
	Float_t *PeaksBack_X[2];
	for(Int_t j = 0; j <2; j++){
	  PeaksFront_X[j] = FrontSpec[j]->GetPositionX();
	  PeaksBack_X[j] = BackSpec[j]->GetPositionX();
	}

	std::vector<Float_t> SortedPeaksF_A;
	std::vector<Float_t> SortedPeaksF_B;
	std::vector<Float_t> SortedPeaksB_A;
	std::vector<Float_t> SortedPeaksB_B;

	for(Int_t j = 0; j < N_InFG[0]; j++){
	  SortedPeaksF_A.push_back(PeaksFront_X[0][j]);
	}
	for(int j = 0; j < N_InFG[1]; j++){
	  SortedPeaksF_B.push_back(PeaksFront_X[1][j]);
	}
	for(int j = 0; j < N_InBG[0]; j++){
	  SortedPeaksB_A.push_back(PeaksBack_X[0][j]);
	}
	for(int j = 0; j < N_InBG[1]; j++){
	  SortedPeaksB_B.push_back(PeaksBack_X[1][j]);
	}

	// Sort the peak valuse:
	std::sort(SortedPeaksF_A.begin(),SortedPeaksF_A.end());
	std::sort(SortedPeaksB_A.begin(),SortedPeaksB_A.end());
	std::sort(SortedPeaksF_B.begin(),SortedPeaksF_B.end());
	std::sort(SortedPeaksB_B.begin(),SortedPeaksB_B.end());

	for(int j = 0; j < N_InFG[0]; j++){
	  if(N_InFG[0] == 6){
	    FrontGraph[0]->SetPoint(j,SortedPeaksF_A[j],Th228Peaks[j]);
	  }
	  else if (N_InFG[0] == 5){
	    FrontGraph[0]->SetPoint(j,SortedPeaksF_A[j],Th228Peaks_5[j]);
	  }
	  else{}
	}

	for(int j = 0; j < N_InFG[1]; j++){
	  if(N_InFG[1] == 6){
	    FrontGraph[1]->SetPoint(j,SortedPeaksF_B[j],Th228Peaks[j]);
	  }
	  else if (N_InFG[1] == 5){
	    FrontGraph[1]->SetPoint(j,SortedPeaksF_B[j],Th228Peaks_5[j]);
	  }
	  else{}
	}

	for(int j = 0; j < N_InBG[0]; j++){
	  if(N_InBG[0] == 6){
	    BackGraph[0]->SetPoint(j,SortedPeaksB_A[j],Th228Peaks[j]);
	  }
	  else if (N_InBG[0] == 5){
	    BackGraph[0]->SetPoint(j,SortedPeaksB_A[j],Th228Peaks_5[j]);
	  }
	  else{}
	}

	for(int j = 0; j < N_InBG[1]; j++){
	  if(N_InBG[1] == 6){
	    BackGraph[1]->SetPoint(j,SortedPeaksB_B[j],Th228Peaks[j]);
	  }
	  else if (N_InBG[1] == 5){
	    BackGraph[1]->SetPoint(j,SortedPeaksB_B[j],Th228Peaks_5[j]);
	  }
	  else{}
	}

	// Now all of the plots are made, we can go ahead and show them and fit them.

	if(FrontGraph[0]->GetN()>0){
	  std::cout << "> Energy Fitting Det A Front ------------" << std::endl;
	  FitCanvas->cd();
	  AlphaSpecLocal_Sum[0][0]->Draw();
	  FitCanvas->Update();
	  FitCanvas2->cd();
	  FrontGraph[0]->Draw("A*");
	  FrontGraph[0]->Fit(FinalEnergyFit[0][0],"Q");
	  FitCanvas2->Update();
	  std::cout << "> Your values are : " << std::endl;
	  std::cout << ">  eshift = " << FinalEnergyFit[0][0]->GetParameter(0) << std::endl;
	  std::cout << ">  elin   = " << FinalEnergyFit[0][0]->GetParameter(1) << std::endl;
	  std::cout << "> Enter a character to move on: ";
	  std::cin >> a;
	  DetName = si_[0].GetName();
	  Output << DetName+".front.eshift " << FinalEnergyFit[0][0]->GetParameter(0)<<"\n"; 
	  Output << DetName+".front.elin " << FinalEnergyFit[0][0]->GetParameter(1)<<"\n"; 
	}
      
      
	if(BackGraph[0]->GetN()>0){
	  std::cout << "> Energy Fitting Det A Back ------------" << std::endl;
	  FitCanvas->cd();
	  AlphaSpecLocal_Sum[0][1]->Draw();
	  FitCanvas->Update();
	  FitCanvas2->cd();
	  BackGraph[0]->Draw("A*");
	  BackGraph[0]->Fit(FinalEnergyFit[0][1],"Q");
	  FitCanvas2->Update();
	  std::cout << "> Your values are : " << std::endl;
	  std::cout << ">  eshift = " << FinalEnergyFit[0][1]->GetParameter(0) << std::endl;
	  std::cout << ">  elin   = " << FinalEnergyFit[0][1]->GetParameter(1) << std::endl;
	  std::cout << "> Enter a character to move on: ";
	  std::cin >> a;
	  DetName = si_[0].GetName();
	  Output << DetName+".back.eshift " << FinalEnergyFit[0][1]->GetParameter(0)<<"\n"; 
	  Output << DetName+".back.elin " << FinalEnergyFit[0][1]->GetParameter(1)<<"\n"; 
	}

	if(FrontGraph[1]->GetN()>0){
	  std::cout << "> Energy Fitting Det B Front ------------" << std::endl;
	  FitCanvas->cd();
	  AlphaSpecLocal_Sum[1][0]->Draw();
	  FitCanvas->Update();
	  FitCanvas2->cd();
	  FrontGraph[1]->Draw("A*");
	  FrontGraph[1]->Fit(FinalEnergyFit[1][0],"Q");
	  FitCanvas2->Update();
	  std::cout << "> Your values are : " << std::endl;
	  std::cout << ">  eshift = " << FinalEnergyFit[1][0]->GetParameter(0) << std::endl;
	  std::cout << ">  elin   = " << FinalEnergyFit[1][0]->GetParameter(1) << std::endl;
	  std::cout << "> Enter a character to move on: ";
	  std::cin >> a;
	  DetName = si_[1].GetName();
	  Output << DetName+".front.eshift " << FinalEnergyFit[1][0]->GetParameter(0)<<"\n"; 
	  Output << DetName+".front.elin " << FinalEnergyFit[1][0]->GetParameter(1)<<"\n"; 
	}

	if(BackGraph[1]->GetN()>0){
	  std::cout << "> Energy Fitting Det B Back ------------" << std::endl;
	  FitCanvas->cd();
	  AlphaSpecLocal_Sum[1][1]->Draw();
	  FitCanvas->Update();
	  FitCanvas2->cd();
	  BackGraph[1]->Draw("A*");
	  BackGraph[1]->Fit(FinalEnergyFit[1][1],"Q");
	  FitCanvas2->Update();
	  std::cout << "> Your values are : " << std::endl;
	  std::cout << ">  eshift = " << FinalEnergyFit[1][1]->GetParameter(0) << std::endl;
	  std::cout << ">  elin   = " << FinalEnergyFit[1][1]->GetParameter(1) << std::endl;
	  std::cout << "> Enter a character to move on: ";
	  std::cin >> a;
	  DetName = si_[1].GetName();
	  Output << DetName+".back.eshift " << FinalEnergyFit[1][1]->GetParameter(0)<<"\n"; 
	  Output << DetName+".back.elin " << FinalEnergyFit[1][1]->GetParameter(1)<<"\n"; 
	}
	

	
      
      }


      if(LocEnCal){
	std::cout << "---- Reference Channel : " << RefChan << " ---------" << std::endl;
	//========================
	// Declare some root stuff:
	TSpectrum *FrontSpec[2];
	TSpectrum *BackSpec[2];
	TGraph *FrontGraph[2];
	TGraph *BackGraph[2];
	TCanvas *FitCanvas = new TCanvas("FitCanvas","FitCanvas",800,10,700,500);
	TCanvas *FitCanvas2 = new TCanvas("FitCanvas2","FitCanvas2",10,10,700,500);
	//TCanvas FitCanvas;
	// FitCanvas->Divide(1,2);
    
	for(int i = 0; i < 2; i++){
	  FrontSpec[i] = new TSpectrum();
	  BackSpec[i] = new TSpectrum();
	  FrontGraph[i] = new TGraph();
	  BackGraph[i] = new TGraph();
	}

	//================================
	// Declare some regular variables:
	Int_t N_InFG[2] = {0,0};
	Int_t N_InBG[2] = {0,0};
	char a;

      
	//===========================================
	// Get the values from the Reference Channel:
	TSpectrum *RefSpecF_A = new TSpectrum();
	TSpectrum *RefSpecB_A = new TSpectrum();
	TSpectrum *RefSpecF_B = new TSpectrum();
	TSpectrum *RefSpecB_B = new TSpectrum();
      
	Int_t Peaks_RefF_A = RefSpecF_A->Search(AlphaSpecRaw[0][0][RefChan][2],2,"",0.15);
	Int_t Peaks_RefB_A = RefSpecB_A->Search(AlphaSpecRaw[0][1][RefChan][2],2,"",0.15);
	Int_t Peaks_RefF_B = RefSpecF_B->Search(AlphaSpecRaw[1][0][RefChan][2],2,"",0.15);
	Int_t Peaks_RefB_B = RefSpecB_B->Search(AlphaSpecRaw[1][1][RefChan][2],2,"",0.15);

	Float_t *Peaks_RefSpecF_A = RefSpecF_A->GetPositionX();
	Float_t *Peaks_RefSpecB_A = RefSpecB_A->GetPositionX();
	Float_t *Peaks_RefSpecF_B = RefSpecF_B->GetPositionX();
	Float_t *Peaks_RefSpecB_B = RefSpecB_B->GetPositionX();
      
	std::vector<Float_t> RefSortedPeaksF_A;
	std::vector<Float_t> RefSortedPeaksF_B;
	std::vector<Float_t> RefSortedPeaksB_A;
	std::vector<Float_t> RefSortedPeaksB_B;
      
	std::vector<Float_t> RefSortedPeaksF_A_5;
	std::vector<Float_t> RefSortedPeaksF_B_5;
	std::vector<Float_t> RefSortedPeaksB_A_5;
	std::vector<Float_t> RefSortedPeaksB_B_5;
      


	for(Int_t j = 0; j < Peaks_RefF_A; j++){
	  RefSortedPeaksF_A.push_back(Peaks_RefSpecF_A[j]);
	}
	for(int j = 0; j < Peaks_RefF_B; j++){
	  RefSortedPeaksF_B.push_back(Peaks_RefSpecF_B[j]);
	}
	for(int j = 0; j < Peaks_RefB_A; j++){
	  RefSortedPeaksB_A.push_back(Peaks_RefSpecB_A[j]);
	}
	for(int j = 0; j < Peaks_RefB_B; j++){
	  RefSortedPeaksB_B.push_back(Peaks_RefSpecB_B[j]);
	}
      
	//================================
	// Sort the reference peak valuse:
	std::sort(RefSortedPeaksF_A.begin(),RefSortedPeaksF_A.end());
	std::sort(RefSortedPeaksB_A.begin(),RefSortedPeaksB_A.end());
	std::sort(RefSortedPeaksF_B.begin(),RefSortedPeaksF_B.end());
	std::sort(RefSortedPeaksB_B.begin(),RefSortedPeaksB_B.end());

	//std::cout << "Before abort stuff" << std::endl;

	if(RefSortedPeaksF_A.size()!=6 && RefSortedPeaksF_A.size()>0 ){
	  std::cout << "> WARNING!  Your reference spectrum does not have 6 peaks." << std::endl;
	  std::cout << "> No aborting.  Please change your reference peak. " << std::endl;
	  std::cout << "> Front Det A" << std::endl;
	  return 0;

	}
	else{
	  if(RefSortedPeaksF_A.size()!=0){
	    for(int j = 0; j < 6; j++){
	      if(j!=2)
		RefSortedPeaksF_A_5.push_back(RefSortedPeaksF_A[j]);
	    }
	  }
	}
     
	if(RefSortedPeaksB_A.size()!=6 && RefSortedPeaksB_A.size()>0){
	  std::cout << "> WARNING!  Your reference spectrum does not have 6 peaks." << std::endl;
	  std::cout << "> No aborting.  Please change your reference peak. " << std::endl;
	  std::cout << "> Back Det A" << std::endl;
	  return 0;
	}
	else{
	  if(RefSortedPeaksB_A.size()!=0){
	    for(int j = 0; j < 6; j++){
	      if(j!=2)
		RefSortedPeaksB_A_5.push_back(RefSortedPeaksB_A[j]);
	    }
	  }
	}

	if(RefSortedPeaksF_B.size()!=6 && RefSortedPeaksF_B.size()>0){
	  std::cout << "> WARNING!  Your reference spectrum does not have 6 peaks." << std::endl;
	  std::cout << "> No aborting.  Please change your reference peak. " << std::endl;
	  std::cout << "> Front Det B - Size : " << RefSortedPeaksF_B.size() << std::endl;
	  return 0;

	}
	else{
	  if(RefSortedPeaksF_B.size()!=0){
	    for(int j = 0; j < 6; j++){
	      if(j!=2)
		RefSortedPeaksF_B_5.push_back(RefSortedPeaksF_B[j]);
	    }
	  }
	}
      
	if(RefSortedPeaksB_B.size()!=6 && RefSortedPeaksB_B.size()>0){
	  std::cout << "> WARNING!  Your reference spectrum does not have 6 peaks." << std::endl;
	  std::cout << "> No aborting.  Please change your reference peak. " << std::endl;
	  std::cout << "> Back Det B - Size : " << RefSortedPeaksB_B.size() << std::endl;
	  return 0;

	}
	else{
	  if(RefSortedPeaksB_B.size()!=0){
	    for(int j = 0; j < 6; j++){
	      if(j!=2)
		RefSortedPeaksB_B_5.push_back(RefSortedPeaksB_B[j]);
	    }
	  }

	
	}
	//std::cout << "After abort stuff" << std::endl;
	//=============================
	// Begin loop over 16 channels:
	for(Int_t i = 0; i <16; i++){

	  //=====================================================
	  // At the beginning of the loop, initialize everything:
	  for(Int_t j = 0; j < 2; j++){
	
	    //=========================================
	    // Get the number of points in the TGraphs:
	    N_InFG[j] = FrontGraph[j]->GetN();
	    N_InBG[j] = BackGraph[j]->GetN();
	
	    //!!!!!!!
	    // Check:
	    /*std::cout << "> Points in Back Graph " << j << "  : " << BackGraph[j]->GetN() << std::endl;
	      std::cout << "> Points in Front Graph " << j << " : " << FrontGraph[j]->GetN() << std::endl;
	      std::cout << "> Points in Back Graph " << j << "  : " << N_InBG[j] << std::endl;
	      std::cout << "> Points in Front Graph " << j << " : " << N_InBG[j] << std::endl;
	    */

	    //===================================================================================
	    // If there are a non zero amount of points in the TGraphs, remove all of the points:
	    if(N_InFG[j]>0)
	      for(Int_t k = N_InFG[j]; k>=0; k--)
		FrontGraph[j]->RemovePoint(k);

	    if(N_InBG[j]>0)
	      for(Int_t k = N_InBG[j]; k>=0;  k--)
		BackGraph[j]->RemovePoint(k);
	
	    //!!!!!!!
	    // Check:
	    /*std::cout << "> Points in Back Graph " << j << "  : " << BackGraph[j]->GetN() << std::endl;
	      std::cout << "> Points in Front Graph " << j << " : " << FrontGraph[j]->GetN() << std::endl;
	      std::cout << "---------------------" << std::endl;*/
	  }

	  //==================================================
	  // At this point everything is initialized.  
	  // Now I want to get the stuff from the histograms:
	  N_InFG[0] = FrontSpec[0]->Search(AlphaSpecRaw[0][0][i][2],2,"",0.15);
	  N_InFG[1] = FrontSpec[1]->Search(AlphaSpecRaw[1][0][i][2],2,"",0.15);
      
	  N_InBG[0] = BackSpec[0]->Search(AlphaSpecRaw[0][1][i][2],2,"",0.15);
	  N_InBG[1] = BackSpec[1]->Search(AlphaSpecRaw[1][1][i][2],2,"",0.15);
      
	  Float_t *PeaksFront_X[2];
	  Float_t *PeaksBack_X[2];
	  for(Int_t j = 0; j <2; j++){
	    PeaksFront_X[j] = FrontSpec[j]->GetPositionX();
	    PeaksBack_X[j] = BackSpec[j]->GetPositionX();
	  }

	  std::vector<Float_t> SortedPeaksF_A;
	  std::vector<Float_t> SortedPeaksF_B;
	  std::vector<Float_t> SortedPeaksB_A;
	  std::vector<Float_t> SortedPeaksB_B;

	  for(Int_t j = 0; j < N_InFG[0]; j++){
	    SortedPeaksF_A.push_back(PeaksFront_X[0][j]);
	  }
	  for(int j = 0; j < N_InFG[1]; j++){
	    SortedPeaksF_B.push_back(PeaksFront_X[1][j]);
	  }
	  for(int j = 0; j < N_InBG[0]; j++){
	    SortedPeaksB_A.push_back(PeaksBack_X[0][j]);
	  }
	  for(int j = 0; j < N_InBG[1]; j++){
	    SortedPeaksB_B.push_back(PeaksBack_X[1][j]);
	  }

	  // Sort the peak valuse:
	  std::sort(SortedPeaksF_A.begin(),SortedPeaksF_A.end());
	  std::sort(SortedPeaksB_A.begin(),SortedPeaksB_A.end());
	  std::sort(SortedPeaksF_B.begin(),SortedPeaksF_B.end());
	  std::sort(SortedPeaksB_B.begin(),SortedPeaksB_B.end());

	  for(int j = 0; j < N_InFG[0]; j++){
	    if(N_InFG[0] == 6){
	      FrontGraph[0]->SetPoint(j,SortedPeaksF_A[j],RefSortedPeaksF_A[j]);
	    }
	    else if (N_InFG[0] == 5){
	      FrontGraph[0]->SetPoint(j,SortedPeaksF_A[j],RefSortedPeaksF_A_5[j]);
	    }
	    else{}
	  }

	  for(int j = 0; j < N_InFG[1]; j++){
	    if(N_InFG[1] == 6){
	      FrontGraph[1]->SetPoint(j,SortedPeaksF_B[j],RefSortedPeaksF_B[j]);
	    }
	    else if (N_InFG[1] == 5){
	      FrontGraph[1]->SetPoint(j,SortedPeaksF_B[j],RefSortedPeaksF_B_5[j]);
	    }
	    else{}
	  }

	  for(int j = 0; j < N_InBG[0]; j++){
	    if(N_InBG[0] == 6){
	      BackGraph[0]->SetPoint(j,SortedPeaksB_A[j],RefSortedPeaksB_A[j]);
	    }
	    else if (N_InBG[0] == 5){
	      BackGraph[0]->SetPoint(j,SortedPeaksB_A[j],RefSortedPeaksB_A_5[j]);
	    }
	    else{}
	  }

	  for(int j = 0; j < N_InBG[1]; j++){
	    if(N_InBG[1] == 6){
	      BackGraph[1]->SetPoint(j,SortedPeaksB_B[j],RefSortedPeaksB_B[j]);
	    }
	    else if (N_InBG[1] == 5){
	      BackGraph[1]->SetPoint(j,SortedPeaksB_B[j],RefSortedPeaksB_B_5[j]);
	    }
	    else{}
	  }

	  // Now all of the plots are made, we can go ahead and show them and fit them.

	  if(FrontGraph[0]->GetN()>0){
	    std::cout << "> Fitting Det A Front Channel " << i << " ------------" << std::endl;
	    FitCanvas->cd();
	    AlphaSpecRaw[0][0][i][2]->Draw();
	    FitCanvas->Update();
	    FitCanvas2->cd();
	    FrontGraph[0]->Draw("A*");
	    FrontGraph[0]->Fit(EnergyFit[0][0][i],"Q");
	    FitCanvas2->Update();
	    std::cout << "> Your values are : " << std::endl;
	    std::cout << ">  a0 = " << EnergyFit[0][0][i]->GetParameter(0) << std::endl;
	    std::cout << ">  a1 = " << EnergyFit[0][0][i]->GetParameter(1) << std::endl;
	    std::cout << "> Enter a character to move on: ";
	    std::cin >> a;
	    DetName = si_[0].GetName();
	    Output << DetName+".front.a0[" << i << "] " << EnergyFit[0][0][i]->GetParameter(0)<<"\n"; 
	    Output << DetName+".front.a1[" << i << "] " << EnergyFit[0][0][i]->GetParameter(1)<<"\n"; 
	  }
      
      
	  if(BackGraph[0]->GetN()>0){
	    std::cout << "> Fitting Det A Back Channel " << i << " ------------" << std::endl;
	    FitCanvas->cd();
	    AlphaSpecRaw[0][1][i][2]->Draw();
	    FitCanvas->Update();
	    FitCanvas2->cd();
	    BackGraph[0]->Draw("A*");
	    BackGraph[0]->Fit(EnergyFit[0][1][i],"Q");
	    FitCanvas2->Update();
	    std::cout << "> Your values are : " << std::endl;
	    std::cout << ">  a0 = " << EnergyFit[0][1][i]->GetParameter(0) << std::endl;
	    std::cout << ">  a1 = " << EnergyFit[0][1][i]->GetParameter(1) << std::endl;
	    std::cout << "> Enter a character to move on: ";
	    std::cin >> a;
	    DetName = si_[0].GetName();
	    Output << DetName+".back.a0[" << i << "] " << EnergyFit[0][1][i]->GetParameter(0)<<"\n"; 
	    Output << DetName+".back.a1[" << i << "] " << EnergyFit[0][1][i]->GetParameter(1)<<"\n"; 
	  }

	  if(FrontGraph[1]->GetN()>0){
	    std::cout << "> Fitting Det B Front Channel " << i << " ------------" << std::endl;
	    FitCanvas->cd();
	    AlphaSpecRaw[1][0][i][2]->Draw();
	    FitCanvas->Update();
	    FitCanvas2->cd();
	    FrontGraph[1]->Draw("A*");
	    FrontGraph[1]->Fit(EnergyFit[1][0][i],"Q");
	    FitCanvas2->Update();
	    std::cout << "> Your values are : " << std::endl;
	    std::cout << ">  a0 = " << EnergyFit[1][0][i]->GetParameter(0) << std::endl;
	    std::cout << ">  a1 = " << EnergyFit[1][0][i]->GetParameter(1) << std::endl;
	    std::cout << "> Enter a character to move on: ";
	    std::cin >> a;
	    DetName = si_[1].GetName();
	    Output << DetName+".front.a0[" << i << "] " << EnergyFit[1][0][i]->GetParameter(0)<<"\n"; 
	    Output << DetName+".front.a1[" << i << "] " << EnergyFit[1][0][i]->GetParameter(1)<<"\n"; 
	  }

	  if(BackGraph[1]->GetN()>0){
	    std::cout << "> Fitting Det B Back Channel " << i << " ------------" << std::endl;
	    FitCanvas->cd();
	    AlphaSpecRaw[1][1][i][2]->Draw();
	    FitCanvas->Update();
	    FitCanvas2->cd();
	    BackGraph[1]->Draw("A*");
	    BackGraph[1]->Fit(EnergyFit[1][1][i],"Q");
	    FitCanvas2->Update();
	    std::cout << "> Your values are : " << std::endl;
	    std::cout << ">  a0 = " << EnergyFit[1][1][i]->GetParameter(0) << std::endl;
	    std::cout << ">  a1 = " << EnergyFit[1][1][i]->GetParameter(1) << std::endl;
	    std::cout << "> Enter a character to move on: ";
	    std::cin >> a;
	    DetName = si_[1].GetName();
	    Output << DetName+".back.a0[" << i << "] " << EnergyFit[1][1][i]->GetParameter(0)<<"\n"; 
	    Output << DetName+".back.a1[" << i << "] " << EnergyFit[1][1][i]->GetParameter(1)<<"\n"; 
	  }


	}

      }
    }
    Output.close();

    return 1;
  }
  
  bool Check_Alpha_Calibration::TerminateIfLast(){
    fgRootFile->Write();
    fgRootFile->Close();
    
    return 1;
    
  }


  void LoadTemplateGates(){
    
  }
}