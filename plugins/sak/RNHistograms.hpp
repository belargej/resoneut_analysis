#ifndef _RNHISTS_
#define _RNHISTS_
#include <TH2.h>
#include "RNGates.hpp"

namespace sak{

class Histogram2D:public TH2D{
private:
public:
  std::vector<sak::Gate> gates;//!

  Histogram2D(){};

  Histogram2D(const char* name,const char*xaxis, const char*yaxis,int binx,double xmin,double xmax,int biny,double ymin,double ymax);


  void ApplyGate(const sak::Gate&newgate);
  bool Check();

  bool OrCheck();

  ClassDef(Histogram2D,1);
};
  
class Histogram1D: public TH1D{
private:

public:
  std::vector<sak::Gate> gates;

  Histogram1D(){};
  Histogram1D(const char* name,const char*xaxis,int binx,double xmin,double xmax);

  void ApplyGate(const sak::Gate&newgate);
  bool Check();
  bool OrCheck();

  ClassDef(Histogram1D,1);
};


  typedef Histogram2D Hist2D;
  typedef Histogram1D Hist1D;
  typedef Histogram2D H2D;
  typedef Histogram1D H1D;

}



#endif
