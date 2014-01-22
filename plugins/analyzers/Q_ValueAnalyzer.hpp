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
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"

namespace physical{

  class Q_ValueAnalyzer:public RN_Analyzer{
  private:
    
  public:  
    Q_ValueAnalyzer();
    virtual ~Q_ValueAnalyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual void Clear(){};
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void ResetGlobals();
    

    ClassDef(Q_ValueAnalyzer,0);
  };
  
    extern Double32_t q_val_p;
    extern Double32_t q_val_n;
}


#endif
