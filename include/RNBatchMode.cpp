////////////////////////////////////////////////////////////////////
// RNBatchMode Program By: Sean Kuvin 2013
// provides a main for a compilable version of RNRoot
// that runs in terminal given a few arguments.  Currently it serves
// to quickly perform evt 2 root conversion operation.
// also, extracting rawpar and calpar (determined by whether config.in
// files are provided. 
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

#include "RNUnpack2Root.hpp"




int main(int argc, char*argv[]){
  if(argc==4){
    RNUnpack2Root* a=new RNUnpack2Root();
    a->init(argv[3]);
    std::vector<int>run_numbers;
    run_numbers.reserve(10);
    int enter=-1;
    while(enter!=0){
      std::cout<<"enter run number (0 to finish entering)"<<std::endl;
      std::cin>>enter;
      if(enter)run_numbers.push_back(enter);
    }
    a->Convert(run_numbers,argv[1],argv[2]);//1:data dir,2:output file    
  }
  else
    std::cout<<"invalid number of arguments: \n"<<"RNBatchMode dir output config"<<std::endl;

}
