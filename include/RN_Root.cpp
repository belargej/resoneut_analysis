#ifndef _RNROOT_CORE_CXX
#define _RNROOT_CORE_CXX
#include "RN_Root.hpp"

using namespace std;

//Global////////////////////////////
////////Histograms//////////////////
//////////////////Cuts//////////////
//////////////////////etc///////////

RN_NeutDetectorArray Narray;	     
RN_NeutCollection neut;	     
RN_S2Collection si_;		     
RN_S2ClusterCollection si_cluster_;
RN_RFCollection rftime;	     
RN_IonChamber ic("ic");		     
RN_NaICollection nai;		     
RN_VariableMap DetVar;
int RN_DetectorSet(0);




void RN_RootInit(){
  if(!RN_DetectorSet){
    neut.reserve(16);
    rftime.reserve(2);
    si_.reserve(2);
    si_cluster_.reserve(2);
    nai.reserve(20);
  }
  else{
    neut.clear();
    rftime.clear();
    si_.clear();
    si_cluster_.clear();
    nai.clear();
  }
    si_.push_back(RN_S2Detector("si_a",16,16));
    si_.push_back(RN_S2Detector("si_b",16,16));
    
    si_cluster_.push_back(RN_S2Cluster("si_cluster_a",16));
    si_cluster_.push_back(RN_S2Cluster("si_cluster_b",16));
    
    rftime.push_back(RN_RFTime("rftime"));
    
    neut.push_back(RN_NeutDetector("neut0",4,1));
    neut.push_back(RN_NeutDetector("neut1",4,4));
    neut.push_back(RN_NeutDetector("neut2",4,5));
    neut.push_back(RN_NeutDetector("neut3",4,6));
    neut.push_back(RN_NeutDetector("neut4",4,7));
    neut.push_back(RN_NeutDetector("neut5",4,11));
    neut.push_back(RN_NeutDetector("neut6",4,14));
    neut.push_back(RN_NeutDetector("neut7",4,10));
    neut.push_back(RN_NeutDetector("neut8",4,12));
    neut.push_back(RN_NeutDetector("neut9",4,15));
 
    nai.push_back(RN_NaIDetector("nai_l1"));
    nai.push_back(RN_NaIDetector("nai_l2"));
    nai.push_back(RN_NaIDetector("nai_l3"));
    nai.push_back(RN_NaIDetector("nai_l4"));
    nai.push_back(RN_NaIDetector("nai_l5"));
    nai.push_back(RN_NaIDetector("nai_l6"));
    nai.push_back(RN_NaIDetector("nai_l7"));
    nai.push_back(RN_NaIDetector("nai_l8"));
    nai.push_back(RN_NaIDetector("nai_l9"));
    nai.push_back(RN_NaIDetector("nai_l10"));
    nai.push_back(RN_NaIDetector("nai_r1"));
    nai.push_back(RN_NaIDetector("nai_r2"));
    nai.push_back(RN_NaIDetector("nai_r3"));
    nai.push_back(RN_NaIDetector("nai_r4"));
    nai.push_back(RN_NaIDetector("nai_r5"));
    nai.push_back(RN_NaIDetector("nai_r6"));
    nai.push_back(RN_NaIDetector("nai_r7"));
    nai.push_back(RN_NaIDetector("nai_r8"));
    nai.push_back(RN_NaIDetector("nai_r9"));
    nai.push_back(RN_NaIDetector("nai_r10"));
  
    RN_DetectorSet = 1;
  
  
}




#endif




///////////////////////////////////////////////////////////////////////
#ifdef MAKE_RN_EXE

TRint *myRint;

int main(int argc, char*argv[]){  
  myRint=new TRint("myRint",&argc,argv);
  myRint->Run();
  return 0;
}

#endif



