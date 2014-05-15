#ifndef __RFTIME_H
#define __RFTIME_H

//C and C++ libraries.
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>

//ROOT libraties
#include <TString.h>
#include <TFile.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TChain.h>
#include <TObject.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH2.h>
#include <TF1.h>
#include <TRandom3.h>
#include <TPolyLine3D.h>
#include <TVector3.h>

#include "RN_VariableMap.hpp"


class RN_RFTime:public RN_BaseClass{
protected:
  double tlin;//!
  double tshift;//!
  double fTo;//!

public:
  RN_RFTime(int i = 0):tlin(1),
		       tshift(0),
		       fTo(0)
  {
  }
  RN_RFTime(std::string name);
  ~RN_RFTime(){};
  /*
  RN_RFTime(const RN_RFTime& old):TObject(old){
    fName=old.Name();
    fT=old.fT;
    tlin=old.tlin;
    tshift=old.tshift;
    fTo=old.fTo;
  }
  */
  /*
  RN_RFTime& operator=(const RN_RFTime& other){
    if(this !=&other){
      fName=other.Name();
      fT=other.fT;
      tlin=other.tlin;
      tshift=other.tshift;
      fTo=other.fTo;
    }
    return *this;
  }
  */

 //data variables 
  double fT;

  Double_t T()const;
  Double_t T_Wrapped()const;



  void SetCalibrations(RN_VariableMap&);
  void InsertHit(const double&);
  void Reset();
 
 
  
 
  ClassDef(RN_RFTime,1);  
};


typedef std::vector<RN_RFTime> RN_RFCollection;
typedef std::vector<RN_RFTime>::iterator RN_RFCollectionRef;




#endif
