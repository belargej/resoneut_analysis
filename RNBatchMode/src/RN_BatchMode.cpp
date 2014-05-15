////////////////////////////////////////////////////////////////////
// RNBatchMode Binary:
// provides a main for a compilable version of RNRoot
// that runs in terminal given a few arguments.  Currently it serves
// to quickly perform evt 2 root conversion operation.
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

#include "RN_EventProcessor.hpp"
#include "RN_Root.hpp"

TRint *myRint;


int main(int argc, char*argv[])
{
  RNROOT::Initialize();
  if(argc==4){
    gEventProcessor.InitStack(argv[3]);
    std::string runinput = argv[1];
    
    if(runinput.find(".evt")!=std::string::npos){
      gEventProcessor.Convert2Root(runinput,argv[2]);
    }
    else{
      std::vector<std::string>run_numbers;
      run_numbers.reserve(10);
      std::string enter="";
      while(enter!="0"){
	std::cout<<"enter run number (0 to finish entering)"<<std::endl;
	std::cin>>enter;
	if(enter.size()>1)run_numbers.push_back(enter);
      }
      gEventProcessor.Convert2Root(run_numbers,argv[1],argv[2]);//1:data dir,2:output file    
    }
    
  }
  else{
    std::cout<<"To use Convert2Root: \n"<<"RNBatchMode dir output config"<<std::endl;
    std::cout<<"Instead, we are running a standard root session with RNROOT libraries loaded"<<std::endl;
    myRint=new TRint("myRint",&argc,argv);
    myRint->Run();
    return 0;
  }

}


