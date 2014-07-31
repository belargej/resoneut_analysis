/***********************************************************/
//Class: SiDeadLayers
//
//Author:Sean Kuvin
//
//This class calculates any angular dependence on the phi segment energy
//The dependence is used to determine the dead layer thickness for the detector.
//The only input that is necessary is a root file which contains data
//from a Th228 alpha source.  
/***********************************************************/

#ifndef _SIDEADLAYER_CXX
#define _SIDEADLAYER_CXX

#include "SiDeadLayers.hpp"
#include "RN_Root.hpp"

using namespace RNROOT;
using namespace TMath;

namespace si_cal{
  TH2D * hSiEvTheta[SI_NUM][16];
  TH2D * hSiClusterEvTheta[SI_NUM][16];
  TH2D * hSibackEvfCh[SI_NUM][16];
  TH2D * hSifrontEvbCh[SI_NUM][16];
  TH2D * hSibackRawEvfCh[SI_NUM][16];

  
  SiDeadLayers::SiDeadLayers()
  {
    
  }
  
  void SiDeadLayers::Reset(){
  
  }

  
  bool SiDeadLayers::Begin(){   
    
    if(!fgRootFile){
      std::cout<<"output file has not been created"<<std::endl;
      exit(EXIT_FAILURE);
    }
    
    //make directory structure
    fgRootFile->mkdir("SiDeadLayers");
    for(unsigned int i=0;i<si_.size();i++){
      fgRootFile->cd("SiDeadLayers");
      gDirectory->mkdir(si_[i].GetName());
      gDirectory->cd(si_[i].GetName());
      for(unsigned int j=0;j<16;j++){
	hSiEvTheta[i][j]=new TH2D(Form("%s_EvTheta_seg%d",si_[i].GetName(),j),Form("%s_EvTheta_seg%d",si_[i].GetName(),j),128,0,31,128,8,10);	
	hSibackEvfCh[i][j]=new TH2D(Form("%s_backEvfCh_seg%d",si_[i].GetName(),j),Form("%s_backEvfCh_seg%d",si_[i].GetName(),j),16,0,15,128,8,10);	
	hSibackRawEvfCh[i][j]=new TH2D(Form("%s_backRawEvfCh_seg%d",si_[i].GetName(),j),Form("%s_backRawEvfCh_seg%d",si_[i].GetName(),j),16,0,15,512,0,4095);	
	hSifrontEvbCh[i][j]=new TH2D(Form("%s_frontEvbCh_ring%d",si_[i].GetName(),j),Form("%s_frontEvbCh_ring%d",si_[i].GetName(),j),16,0,15,128,8,10);	
	hSiClusterEvTheta[i][j]=new TH2D(Form("%s_ClusterEvTheta_seg%d",si_[i].GetName(),j),Form("%s_ClusterEvTheta_seg%d",si_[i].GetName(),j),128,0,31,128,0,10);

      }

   }
    

    return 1;
  }  
  
  

  bool SiDeadLayers::Process(){
    
    return 1;
  }
  bool SiDeadLayers::ProcessFill(){
    
    for(unsigned int i=0;i<SI_NUM;i++){
      int bch = (int)si_[i].back.Ch();
      int fch = (int)si_[i].front.Ch();
      double e = si_[i].Back_E();
      double theta = (si_[i].chVect(fch,bch)).Theta() * RadToDeg();
      double ec = si_cluster_[i].ERaw();

      if(bch>=0){
	if(theta>0){
	  hSiEvTheta[i][bch]->Fill(theta,e);
	  hSiClusterEvTheta[i][bch]->Fill(theta,ec);
	}
	hSibackEvfCh[i][bch]->Fill(fch,e);
	hSibackRawEvfCh[i][bch]->Fill(fch,si_[i].back.ERaw());
      }
      if(fch>=0){
	hSifrontEvbCh[i][fch]->Fill(bch,e);
      }
      

      
    }
    

    
 
    
    return 1;
    
  }


  bool SiDeadLayers::Terminate(){
    return 1;
  }
  void SiDeadLayers::Clear(){
    
  }
  
  
  
}


#endif
