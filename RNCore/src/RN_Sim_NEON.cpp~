/**********************************************************
//Class: RN_Sim
//
//Author: Sean Kuvin
//
//RN_Sim is an abstract class which simply takes simulated kinematics
//of a transfer reaction and checks if any of the detectors
//defined in RN_Root.cpp are hit. The user needs to supply
//their own class which inherits from RN_Sim and defines
//the methods Print(), Begin(), Process(), ProcessFill(), and Terminate()   
*********************************************************/

#ifndef RNSIM_CXX
#define RNSIM_CXX
#include <numeric>
#include "RN_Sim.hpp"
#include "RN_Root.hpp"
using namespace global;
using namespace RNROOT;

RN_Sim::RN_Sim(const std::string& name):RN_BaseClass(name),
					fTotEvents(0),
					fEDeposited(0),
					fNTOF(0),			    
					fNCM(0),
					fSiliconResolution(0.1),
					fProtonThreshold(0),
					fBeamSpot(TVector3(0,0,0)),
					fNeutronIn(NEUTNUM,0),
					fNeutronDetected(NEUTNUM,0),
					fProtonIn(SI_NUM,0),
					fProtonIn_NeutDet(SI_NUM,0)
				 
{

}
  
  void RN_Sim::Init(){
    fNeutronIn.clear();
    fNeutronDetected.clear();
    fProtonIn.clear();
    for(unsigned int i=0;i<neut.size();i++){
      fNeutronIn.push_back(0);
      fNeutronDetected.push_back(0);
    }
    for(unsigned int i=0;i<si_.size();i++){
      fProtonIn.push_back(0);
      fProtonIn_NeutDet.push_back(0);
    }
       
    //JAB
    fIC_In.clear();
  }



  void RN_Sim::LoadConfig(std::string input){    
    std::string temp;
    ifstream cfg(input.c_str());
    if(!cfg){
      std::cout<<"could not cfg "<<input<<std::endl;
      std::cin>>temp;
      return ;
    }

    do{
      std::vector<std::string>input;
      sak::ReadLine(cfg,input);
      if(input.size()==0)
	continue;

      //if(input[0] == 'fSilicon
      if(input[0]=="fParam")
      {      
	gVariableMap.AddParam(input[1],sak::string_to_double(input[2]));
      }
      else if(input[0]=="fDWBA")
	{
	  gReactionInfo.SetAngularDistribution(input[1]);
	
	}
      else if(input[0]=="fReaction")
	{
	  std::string plist[6];
	  if (input.size()==7){
	    for(unsigned int i=0;i<6;i++){
	      plist[i] = input[i+1];
	    }
	    gReactionInfo.SetReaction(plist[0],
				      plist[1],
				      plist[2],
				      plist[3],
				      plist[4],
				      plist[5]);
	  }
	  else {
	    std::cout<<"incorrect number of entries to fReaction: "<< input.size()-1<<"but needed 6"<<std::endl;
	    exit(EXIT_FAILURE);
	  }
	}
      else if(input[0]=="fOutput")
	{
	  if(input.size()==3){
	    fSimFile=new TFile(input[1].c_str(),"RECREATE");
	    fSimLog.open(input[2].c_str());
	}
	else std::cout<<"invalid number of arguments for fOutput"<<std::endl;
      }
    else if(input[0]=="fEvents")
      {
	fTotEvents=sak::string_to_int(input[1]);
      }
      /*else if(input[0]=="fSRIMOption")
      {
	if(input.size()!=4){
	  std::cout<<"invalid number of arguments for fSRIMOption\n";
	}
	if(input[1]=="on"){
	  fUseSRIM=true;
	}
	fTargetMaterial = input[2];
	fTargetThick = sak::string_to_double(input[3]);
	
	//get ELossTables for the beam and the Heavy Recoil passing through the target make sure these exist!
	std::string beamname=(const char*)BeamName();
	std::string heavyrecoilname=(const char*)HeavyDecayName();
	fELossTableBeam.Init(beamname,(const char*)fTargetMaterial,"solid","",500,100);
	fELossTableHeavyRecoil.Init(heavyrecoilname,(const char*)fTargetMaterial,"solid","",500,100);
	
	}*/
    else if(input[0]=="fOption")
      {
	fOption=input[1];
      }

    else
      continue;
    } while(!cfg.eof());
    cfg.close();
    

    //gVariableMap.GetParam("SiliconSigma",fSiliconResolution)
    SetCalibrations();
  }
  
void RN_Sim::Loop(){
  Begin();
  Long64_t evcount=0;
  while(evcount<fTotEvents){
    if(GenerateEvents(evcount,fOption)){
      evcount++;
    }
    else
      continue;
    if(Process()){
      ProcessFill();
    }
    if(evcount%30000==0)std::cout<<evcount<<std::endl;
  }
  Terminate();
}  
  
  int RN_Sim::GenerateEvents(Long64_t evnum,TString options){
    Reset();
    //get cm angle and generate LorentzVectors for sim events(stored in gReaction Info)
    fNCM = gReactionInfo.GenerateSimEvent(); 


    double ReactionDepth = gReactionInfo.GetReactionDepth();
    //std::cout << " Reaction Depth 2 : " << ReactionDepth << std::endl;



    double phirand = 2*TMath::Pi()*myRnd.Rndm();
    double radiusrand = TMath::Sqrt(myRnd.Rndm()) * (gReactionInfo.BeamSpread() / 2);
    double xrand = TMath::Cos(phirand) * radiusrand;
    double yrand = TMath::Sin(phirand) * radiusrand;
    TVector3 nv = gReactionInfo.RecoilLV().Vect();
    TVector3 pv = gReactionInfo.DecayProductLV().Vect();
    fBeamSpot.SetXYZ(xrand,yrand,0);
    
    double decayke=global::myRnd.Gaus(gReactionInfo.DecayProductLV().E()-gReactionInfo.DecayProductLV().M(),fSiliconResolution);



    for(unsigned int i=0;i<neut.size();i++){
      if(neut[i].inDet(nv,fBeamSpot)){
	fNeutronIn[i]++;
	if(neut[i].NeutIn(gReactionInfo.RecoilLV(),fNTOF,fEDeposited,fBeamSpot)){
	  fNeutronDetected[i]++;
	  for(unsigned int s=0;s<si_.size();s++){
	    if(si_[s].inDet(pv,fBeamSpot) && decayke > fProtonThreshold ){
	      fProtonIn_NeutDet[s]++;
	    }
	  }
	}
      }
    }
    
    for(unsigned int s=0;s<si_.size();s++){
      if(si_[s].inDet(pv,fBeamSpot) && decayke > fProtonThreshold){
	fProtonIn[s]++;
	double chf=0;
	double chb=0;
	
	si_[s].Vect_to_ch(pv,chf,chb,fBeamSpot);
	si_[s].front.InsertHit(decayke,0,chf);
	si_[s].back.InsertHit(decayke,0,chb);
      }
      
      si_cluster_[s].ReconstructClusters(si_[s]);
    }
    
    
    //std::cout << " Number of Si Dets   : " << si_.size() << std::endl;
    //std::cout << " Number of Neut Dets : " << neut.size() << std::endl;
    //std::cout << " Number of IC Dets   : " << ic.size() << std::endl;

    return 1;
  }   
  

  
  void RN_Sim::Reset(){
    fEDeposited=0;
    fNTOF=0;

    gReactionInfo.Reset();
    fBeamSpot.SetXYZ(0,0,0);
    


    for(RN_NeutCollectionRef it=neut.begin();it!=neut.end();it++){ 
      (*it).Reset();  
    }
    
    for(RN_S2CollectionRef it=si_.begin();it!=si_.end();it++){
      (*it).Reset(); 
    }
    for(RN_S2ClusterCollectionRef it=si_cluster_.begin();it!=si_cluster_.end();it++){
      (*it).Reset(); 
    }

    // JAB
    ic.Reset();
 
    
  }




#endif
