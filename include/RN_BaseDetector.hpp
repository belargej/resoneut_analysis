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
  char* fName;
  int fNumOfCh;
  int sorted_by_channel;
public:
  RN_BaseDetector(){}
  RN_BaseDetector(char* name, int num):fName(name),
					     fNumOfCh(num),
					     sorted_by_channel(0)
					     
  {
    Init();
    Reset();
  }

  int fMult;
  std::vector<double>fChlist;
  std::vector<double> fE;
  std::vector<double> fT;
 
  void Init();
  void Reset();
  Int_t NumOfCh()const{return fNumOfCh;}
  char* Name()const {return fName;} 
  void InsertHit(const double&, const double&, const double&);
  void SetSortByChannel(){sorted_by_channel=1;}

  ClassDef(RN_BaseDetector,1);
};

#endif
