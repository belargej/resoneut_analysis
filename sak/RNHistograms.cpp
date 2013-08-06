#ifndef _RNHISTS_cxx
#define _RNHISTS_cxx
#include <TH2.h>

#include "RNHistograms.hpp"
namespace sak{

  Histogram2D::Histogram2D(const char* name,const char*xaxis, const char*yaxis,int binx,double xmin,double xmax,int biny,double ymin,double ymax):TH2D(name,name,binx,xmin,xmax,biny,ymin,ymax){
      SetXTitle(xaxis);
      SetYTitle(yaxis);
    }
    
  void Histogram2D::ApplyGate(const sak::Gate&newgate){gates.push_back(newgate);}
  bool Histogram2D::Check(){
    for(unsigned int i=0;i<gates.size();i++){
      if ((gates[i]).Check()==0)
	return 0;
    } 
    return 1;
  }
    
  bool Histogram2D::OrCheck(){
    
    for(unsigned int i=0;i<gates.size();i++){
      if ((gates[i]).Check()==1)
	return 1;
    } 
    return 0;
  }
    
  
  Histogram1D::Histogram1D(const char* name,const char*xaxis,int binx,double xmin,double xmax):TH1D(name,name,binx,xmin,xmax){
      SetXTitle(xaxis);
    }
    
   
  void Histogram1D::ApplyGate(const sak::Gate&newgate){gates.push_back(newgate);}
  bool Histogram1D::Check(){
    for(unsigned int i=0;i<gates.size();i++){
      if ((gates[i]).Check()==0)
	return 0;
    } 
      return 1;
  }
    
  bool Histogram1D::OrCheck(){
    
    for(unsigned int i=0;i<gates.size();i++){
      if ((gates[i]).Check()==1)
	return 1;
    } 
    return 0;
  }

  

  typedef Histogram2D Hist2D;
  typedef Histogram1D Hist1D;
  typedef Histogram2D H2D;
  typedef Histogram1D H1D;

}



#endif
