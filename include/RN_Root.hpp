#ifndef _RNROOT_CORE
#define _RNROOT_CORE
//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

//ROOT libraties
#include <TROOT.h>
#include <TRint.h>
#include <TH1.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TTree.h>
#include <TBranch.h>
#include <TBrowser.h>
#include <TCutG.h>

#include <map>
#include "RN_Unpack2Root.hpp"
#include "RN_Particle.hpp"
#include "RN_NeutDetectorArray.hpp"
#include "RN_SiliconDetectors.hpp"
#include "RN_Timing.hpp"
#include "RN_VariableMap.hpp"
#include "RN_IonChamber.hpp"
#include "RN_NaIArray.hpp"
#include "RN_MassTable.hpp"


//These are added here so that they will be added to rootcint
extern RN_ParticleCollection particle;
R__EXTERN RN_NeutDetectorArray Narray;	     
R__EXTERN RN_NeutCollection neut;	     
R__EXTERN RN_S2Collection si_;		     
R__EXTERN RN_S2ClusterCollection si_cluster_;
R__EXTERN RN_RFCollection rftime;	     
R__EXTERN RN_IonChamber ic;		     
R__EXTERN RN_NaICollection nai;		     
R__EXTERN RN_VariableMap DetVar;
R__EXTERN RN_MassTable MassTable;

R__EXTERN int RN_DetectorSet;

R__EXTERN TFile * rootfile;

void RN_RootInit();
void SetCalibrations();
void LoadVariableFile(const std::string& f);

namespace global{
  void LoadGlobalParams();
  extern double beam_e;
  extern double beam_eloss;
  extern double beam_est;
  extern double m_beam;
  extern double m_target;
  extern double m_frag;
  extern double m_recoil;
  extern double m_heavy_decay;
  extern double m_decay_product;
  extern double hi_ex_set;
  extern double d_ex_set;
  extern TRandom3 myRnd;

}



#endif
