//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

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

using namespace std;

//Global////////////////////////////
////////Histograms//////////////////
//////////////////Cuts//////////////
//////////////////////etc///////////
TRint *myRint;


///////////////////////////////////////////////////////////////////////

int main(int argc, char*argv[]){  
  myRint=new TRint("myRint",&argc,argv);
  myRint->Run();
  return 0;
}

