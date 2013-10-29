////////////////////////////////////////////////////////////////////
// RN_Sim Binary:
// provides a main for a compilable version of RN_Sim
// that runs in terminal given a few arguments.  Currently it serves
// to quickly perform simulation given a single config file.
// 
// Author: Sean Kuvin 
////////////////////////////////////////////////////////////////////

//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

//ROOT libraties
#include <TROOT.h>
#include <TRint.h>
#include <TH1.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TTree.h>
#include <TBranch.h>
#include <TBrowser.h>
#include <TCutG.h>

#include "RN_Root.hpp"
#include "RN_Sim.hpp"

int main(int argc, char*argv[])
{
  if(argc==2){
    RN_RootInit();//initialize default detector setup
    sim::RN_Sim a;
    a.StartRun(argv[1]);
    return 1;
  }
  else{
    std::cout<<"RN_Sim expects one additional argument only for the configuration file"<<std::endl;
    exit(EXIT_FAILURE);
  }
}
