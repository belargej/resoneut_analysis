/*************************************************************/
//Class: Q_ValueAnalyzer
//
//Author:Sean Kuvin
//
//Physics Analyzer class to be inserted in the analyzer list for 
//calculating Q_value parameters using the detector parameters
/**************************************************************/

#ifndef _QVANALYZER_H
#define _QVANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"



namespace physical{

  class Q_ValueAnalyzer:public RN_Analyzer{
  private:
    int fSiAngle; //which detector to use for silicon angle
    int QValIterations;
    bool UseInvMass;
    double Q0;
    
  public:  
    Q_ValueAnalyzer();
    virtual ~Q_ValueAnalyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual void Clear(){};
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Reset();
    void SetSiliconAngle(const int& index);
    void SetQValIterations(const int& Iter) { QValIterations = Iter; std::cout << " QVal Its : " << QValIterations << std::endl;};
    ClassDef(Q_ValueAnalyzer,0);
    void SetInvMass(const double& Q0_){UseInvMass=1;Q0=Q0_;};
  };
  
    extern Double32_t q_val_p;
    extern Double32_t q_val_n;
}


#endif
