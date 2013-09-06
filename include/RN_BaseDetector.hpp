//////////////////////////////////////////////////////////////////////////////////
//** RN_BaseDetector class: Inserting a hit (energy, time, channel) 
//** is sorted by energy(default) and mult is counted. 
//** Adapted from SpecTcl DetectorParameters.cpp
//                 Author: Sean Kuvin                             
//***********************************************************************************/




#ifndef __BaseDetector__
#define __BaseDetector__
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include <TObject.h>
#include <TMath.h>
#include <TString.h>
#include "RN_VariableMap.hpp"



class RN_BaseDetector:public TObject{
private:
  std::string fName;//!
  int fNumOfCh;//!
  int sorted_by_channel;//!
  double lowlimit;//!
  double highlimit;//!
public:
  RN_BaseDetector(){
  }
  RN_BaseDetector(std::string name, int num):fName(name),
					     fNumOfCh(num),
					     sorted_by_channel(0),
					     lowlimit(0),
					     highlimit(4096),
					     fMult(0),
					     fChlist(num,double(0)),
					     fE(num,double(0)),
					     fT(num,double(0))
				       
				       
				       
  {
  }

  int fMult;
  std::vector<Double32_t>fChlist;//[fMult]
  std::vector<Double32_t>fE;//[fMult]
  std::vector<Double32_t>fT;//[fMult]

  void SetELimits(const double&,const double&);
  void Init(const double& num);
  void Reset();
  Int_t NumOfCh()const{return fNumOfCh;}
  std::string Name()const {return fName;} 
  int InsertHit(const double&, const double&, const double&);
  void SetSortByChannel(){sorted_by_channel=1;}

  ClassDef(RN_BaseDetector,1);
};


typedef std::vector<RN_BaseDetector> RN_BaseCollection;
typedef std::vector<RN_BaseDetector>::iterator RN_BaseCollectionRef;




#endif
