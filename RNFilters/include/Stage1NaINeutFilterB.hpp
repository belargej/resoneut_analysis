/***********************************************************/
//Class: Stage1NaINeutFilterB
//
//Author:Sean Kuvin
//
//Filter code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
/***********************************************************/

#ifndef _Stage1_NaINeutFilterB_H
#define _Stage1_NaINeutFilterB_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class Stage1NaINeutFilterB:public RN_Analyzer{
  private:    
  public:
    
    Stage1NaINeutFilterB();
    virtual ~Stage1NaINeutFilterB(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    
    ClassDef(Stage1NaINeutFilterB,0);
  };
  
}


#endif
