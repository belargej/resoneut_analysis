/***********************************************************
//Class: F17dn_Analyzer
//
//Author:Sean Kuvin
//
***********************************************************/

#ifndef _F17DNANALYZER_H
#define _F17DNAnalyzer_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"


namespace _F17{


class F17dn_Analyzer:public RN_Analyzer{
protected:
  Double32_t fErProton;
  Double32_t fErProtonReco;
  Double32_t fNeutTime;
  Double32_t fNeutKE_R;
  Double32_t fNeutKE;
  Double32_t fNeutTheta;
  Double32_t fHiKE;
  Double32_t fNeutQ;
  Double32_t fNeutQAngle;




public:  
  F17dn_Analyzer();
  virtual ~F17dn_Analyzer(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual void Reset();





  ClassDef(F17dn_Analyzer,0);
};

  void LoadF17AnalysisGates(const std::string&input);


}
#endif
