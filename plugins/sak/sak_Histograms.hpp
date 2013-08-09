#ifndef _SAKHISTS_
#define _SAKHISTS_
#include <TH2.h>


namespace sak{

class Histogram2D:public TH2D{
private:
public:
  Histogram2D(){};
  Histogram2D(const char* name,const char*xaxis, const char*yaxis,int binx,double xmin,double xmax,int biny,double ymin,double ymax);

  ClassDef(Histogram2D,1);
};
  
class Histogram1D: public TH1D{
private:

public:
  Histogram1D(){};
  Histogram1D(const char* name,const char*xaxis,int binx,double xmin,double xmax);

  
  
  

  ClassDef(Histogram1D,1);
};


  typedef Histogram2D Hist2D;
  typedef Histogram1D Hist1D;
  typedef Histogram2D H2D;
  typedef Histogram1D H1D;

}



#endif
