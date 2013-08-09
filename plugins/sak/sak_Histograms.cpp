#ifndef _RNHISTS_cxx
#define _RNHISTS_cxx
#include <TH2.h>

#include "sak_Histograms.hpp"
namespace sak{

  Histogram2D::Histogram2D(const char* name,const char*xaxis, const char*yaxis,int binx,double xmin,double xmax,int biny,double ymin,double ymax):TH2D(name,name,binx,xmin,xmax,biny,ymin,ymax){
      SetXTitle(xaxis);
      SetYTitle(yaxis);
    }
  
  Histogram1D::Histogram1D(const char* name,const char*xaxis,int binx,double xmin,double xmax):TH1D(name,name,binx,xmin,xmax){
      SetXTitle(xaxis);
    }

}



#endif
