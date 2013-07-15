#ifndef __BaseDetector__
#define __BaseDetector__
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include <TObject.h>
#include <TMath.h>

class RN_BaseDetector:public TObject{
private:
  std::string fName;
  int fNumOfCh;
  int sorted_by_channel;
  double lowlimit;
  double highlimit;
public:
  RN_BaseDetector(){
    /*fChlist=new double[32];
    fE=new double[32];
    fT=new double[32];*/
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
    /*fChlist=new double[fNumOfCh];
    fE=new double[fNumOfCh];
    fT=new double[fNumOfCh];*/
  }

  int fMult;
  std::vector<double>fChlist;//[fMult]
  std::vector<double>fE;//[fMult]
  std::vector<double>fT;//[fMult]
  /*
  double* fChlist;//[fMult]
  double* fE;//[fMult]
  double* fT;//[fMult]
  */
  void SetELimits(const double&,const double&);
  void Init(const double& num);
  void Reset();
  Int_t NumOfCh()const{return fNumOfCh;}
  std::string Name()const {return fName;} 
  void InsertHit(const double&, const double&, const double&);
  void SetSortByChannel(){sorted_by_channel=1;}

  ClassDef(RN_BaseDetector,1);
};

#endif
