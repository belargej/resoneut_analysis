#ifndef _CHECKALPHACAL_CXX
#define _CHECKALPHACAL_CXX

#include "CheckAlphaCal.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;

namespace CheckAlphaCal{

  // Define Histograms Here:
  /* TH1D *AlphaSpecRaw[2][2][16];
  TH1D *AlphaSpecOffset[2][2][16];
  TH1D *AlphaSpecLocal[2][2][16];
  TH1D *AlphaSpecE[2][2][16];
  */


  CheckAlphaCal::CheckAlphaCal()
  {
    
  }
  
  void CheckAlphaCal::Reset(){

  }

  bool CheckAlphaCal::Begin(){
    /*if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      //ClearGates();
      exit(EXIT_FAILURE);
      }*/
    /*
    fgRootFile->mkdir("AlphaCalCheck");
    fgRootFile->cd("AlphaCalCheck");
    gDirectory->mkdir("Si_A");
    gDirectory->mkdir("Si_B");
    
    gDirectory->cd("Si_A");
    gDirectory->mkdir("Front");
    gDirectory->mkdir("Back");

    gDirectory->cd("Front");
    gDirectory->mkdir("Raw_Energy");
    gDirectory->mkdir("Offset");
    gDirectory->mkdir("Local");
    gDirectory->mkdir("Calibrated_Energy");

    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_A");
    gDirectory->cd("Back");
    gDirectory->mkdir("Raw_Energy");
    gDirectory->mkdir("Offset");
    gDirectory->mkdir("Local");
    gDirectory->mkdir("Calibrated_Energy");

    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Front");
    gDirectory->mkdir("Raw_Energy");
    gDirectory->mkdir("Offset");
    gDirectory->mkdir("Local");
    gDirectory->mkdir("Calibrated_Energy");

    fgRootFile->cd("AlphaCalCheck");
    gDirectory->cd("Si_B");
    gDirectory->cd("Back");
    gDirectory->mkdir("Raw_Energy");
    gDirectory->mkdir("Offset");
    gDirectory->mkdir("Local");
    gDirectory->mkdir("Calibrated_Energy");
    */

   

    

    return 1;
  }

  bool CheckAlphaCal::Process(){

    return 1;
  }

  bool CheckAlphaCal::ProcessFill(){

    return 1;

  }

  bool CheckAlphaCal::Terminate(){
    
    return 1;
  }
  
  bool CheckAlphaCal::TerminateIfLast(){

    return 1;
  }

  void CheckAlphaCal::Clear(){


  }
  


  void LoadGates(){
    
  }

  void ClearGates(){

  }

}

#endif
