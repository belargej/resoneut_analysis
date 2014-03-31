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
#include "RN_TriggerBit.hpp"
#include "RN_Analyzer.hpp"

const static unsigned int NEUTNUM(16);

extern TFile *rootfile;
extern TTree *newtree;



//These are added here so that they will be added to rootcint
extern RN_ParticleCollection particle;
extern RN_NeutDetectorArray Narray;	     
extern RN_TriggerBitCollection triggerbit;	     
extern RN_NeutCollection neut;	     
extern RN_S2Collection si_;		     
extern RN_S2ClusterCollection si_cluster_;
extern RN_RFCollection rftime;	     
extern RN_IonChamber ic;		     
extern RN_NaICollection nai;		     
extern RN_NaIArray nai_array;

extern RN_VariableMap DetVar;
extern RN_MassTable MassTable;
extern int RN_RootSet;

extern TFile * rootfile;
extern TTree * newtree;
extern TList * analyzers;

extern RN_Analyzer MainAnalyzer;

namespace global{
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
  extern double E_fragment;
  extern TRandom3 myRnd;
  extern TVector3 target_pos;

  void SetReaction(std::string,std::string,std::string,
		   std::string,std::string,std::string);
}

		 
void RN_RootReset();
void RN_RootInit();
void SetCalibrations();
void LoadVariableFile(const std::string& f);
void SetRootOutputFile(std::string filename);
void SetRootOutputFileAndTree(std::string filename,std::string treename);
int AddAnalyzer(TObject *object);


////////////////////////////////////////////////////////////////////////////
//// 3/23: Under Construction RNROOT framework class
///////////////////////////////////////////////////////////////////////////

class RNROOT{

private:
  TList *fRNAnalyzers;
  TList *fRNDetectors;
  TList *fRNClasses;
  TList *fRNVariables;
  TList *fRNParameters;
  RN_VariableMap DetVar;
  //RN_MassTable MassTable;
  RN_Analyzer MainAnalyzer;
  int RN_RootSet;

public:
  TFile *rootfile;
  TTree *newtree;

  RNROOT(){};
  virtual ~RNROOT();
  TList * GetListOfRNClasses(){return fRNClasses;}
  TList * GetListOfRNAnalyzers(){return fRNAnalyzers;}
  TList * GetListOfRNDetectors(){return fRNDetectors;}
  TList * GetListOfRNParameters(){return fRNParameters;}
  TList * GetListOfRNVariables(){return fRNVariables;}
  //RN_ReactionInfo GetRN_ReactionInfo();
  int AddAnalyzer(TObject *object);
  int AddDetector(TObject *object);
  int AddParameter(TObject *object);
  int AddVariable(TObject *object);
  int AddClass(TObject *object);
  virtual void Bind(){};
  virtual void Execute(){};
  void LoadVariableFile(const std::string& f);
  void Reset();
  void Init();
  virtual void SetCalibrations();
  void SetRootOutputFile(std::string filename);
  void SetRootOutputFileAndTree(std::string filename,std::string treename);
  void SetReaction(std::string,std::string,std::string,
		   std::string,std::string,std::string);

};

extern RNROOT gRNROOT;

#endif
