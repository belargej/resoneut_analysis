/***************************************************************
Class: RN_NaIDetector, RN_NaIArray
Author: Sean A Kuvin
****************************************************************/

#ifndef __RNNAI__CXX
#define __RNNAI__CXX
#include "RN_NaIArray.hpp"


ClassImp(RN_NaIArray);
ClassImp(RN_NaIDetector);




void RN_NaIDetector::Reset(){

  for(int i=0;i<2;i++){
    fE[i]=0;
    fT[i]=0;
  }

}


Double32_t RN_NaIDetector::Position() const{
  if(E2()>0 && E1()>0)
    return ((1.0 / ( 2.0 * light_atten))*(log(E2()/E1())));
  
  else
    return 0xffff;
}


void RN_NaIDetector::SetCalibrations(RN_VariableMap& detvar){
  detvar.GetParam(Form("%s.elin[0]",GetName()),elin[0]);
  detvar.GetParam(Form("%s.eshift[0]",GetName()),eshift[0]);
  detvar.GetParam(Form("%s.tlin[0]",GetName()),tlin[0]);
  detvar.GetParam(Form("%s.tshift[0]",GetName()),tshift[0]);
  detvar.GetParam(Form("%s.elin[1]]",GetName()),elin[1]);
  detvar.GetParam(Form("%s.eshift[1]",GetName()),eshift[1]);
  detvar.GetParam(Form("%s.tlin[1]",GetName()),tlin[1]);
  detvar.GetParam(Form("%s.tshift[1]",GetName()),tshift[1]);
  detvar.GetParam(Form("%s.tzero[0]",GetName()),tzero[0]);
  detvar.GetParam(Form("%s.tzero[1]",GetName()),tzero[1]);


}




void RN_NaIArray::Reset(){
  //don't call BaseReset before using fMultx
  
  //std::cout << " Reseting, Mult = : " << fMult << std::endl;
  
  for(unsigned int i=0;i<fMult;i++){  
    fPosition[i]=0xffff;
  }

  tfirst=4096;
  efirst = -1;
  t_mult=0;
  detfirst=-1;
  //ESum=0;
  

  RN_BaseDetector::Reset();
  
 
}


void RN_NaIArray::SetCalibrations(Double32_t elin, Double32_t eshift, Double32_t tlin, Double32_t tshift){
  /*  this->elin=elin;
  this->eshift=eshift;
  this->tlin=tlin;
  this->tshift=tshift;
  */
}


void RN_NaIArray::SetCalibrations(RN_VariableMap& detvar){
  /*detvar.GetParam(Form("%s.elin",GetName()),elin);
  detvar.GetParam(Form("%s.eshift",GetName()),eshift);
  detvar.GetParam(Form("%s.tlin",GetName()),tlin);
  detvar.GetParam(Form("%s.tshift",GetName()),tshift);*/
  RN_BaseDetector::SetCalibrations(detvar);
  std::cout << " Name : " << GetName() << std::endl;

}


void RN_NaIArray::ReconstructHits(const RN_NaICollection& in){
    unsigned int idx(0);
    //std::cout << "=========== Start ===========" << std::endl;
    for(RN_NaICollectionCRef it=in.begin();it!=in.end();it++){
      FindTFirst((*it).T(),idx,(*it).E_Gamma());
      if((*it).E_Gamma()>0){

	//ESum+=(*it).E_Gamma();
	
	//std::cout << " EGamm > 0 " << std::endl;
      //std::cout << " NaI Time : "<< (*it).T() << std::endl;
      //if((*it).T()>0){
	//std::cout << " inserting hit, det number : " << idx << std::endl;
	InsertHit((*it).E_Gamma(),(*it).T(),(*it).Position(),idx);
      }
      idx++;

      //std::cout << " Name : " << (*it).Name() << std::endl;
      //std::cout << " a0   : " << (*it).fA1[(int)fChlist[idx]] << std::endl;
    }
    
    //std::cout << "=========== Stop ============" << std::endl;
  
}
int RN_NaIArray::FindTFirst(const double& t){
  if(t>0){
    t_mult++;
    if(t<tfirst){
      tfirst = t;
      return 1;
    }
    else return 0;
  }
  else
    return 0;
    
}

int RN_NaIArray::FindTFirst(const double& t,const int& det){
  if(t>0){
    t_mult++;
    if(t<tfirst){
      detfirst =det;
      tfirst = t;
      return 1;
    }
    else return 0;
  }
  else
    return 0;
  
}

int RN_NaIArray::FindTFirst(const double& t,const int& det,const double& e){
  if(t>0){
    t_mult++;
    if(t<tfirst){
      detfirst =det;
      tfirst = t;
      efirst = e;
      return 1;
    }
    else{
      //std::cout << "--------------------------" << std::endl;
      //std::cout << "This Time : " << t << std::endl;
      //std::cout << "First Time : " << tfirst << std::endl;
      return 0;
    }
  }
  else
    return 0;
  
}

int RN_NaIArray::InsertHit(const Double32_t&egamma,const Double32_t&tgamma,const Double32_t&pos_gamma,const Int_t&ch){
  if (!egamma>0)
    return -1;

  

  int i,j,k;

  //std::cout << " In Insert Hit " << std::endl;
  //std::cout << " fMult : " << fMult << std::endl;
  /* sorted by energy */
  //std::cout << "==========================" << std::endl;
  //std::cout << " Current Energy : " << egamma << std::endl;
  for (i=(int)fMult-1;i>=0;i--){
  //for(i=20;i>=0;i++){
    if (egamma<fE[i]){
      //std::cout << "> fE["<<i<<"] = " << fE[i] << std::endl;
      break;
    }
  }
  //std::cout << " i Val : " << i << std::endl;

  /* sorted by time */
  //std::cout << "------------------------" << std::endl;
  //std::cout << " Current Time : " << tgamma << std::endl;
  for(k=0;k<fMult;k++){
    
    //std::cout << "> fT["<<k<<"] = " << fT[k] << std::endl;
    if(tgamma<fT[k])
      break;
  }


  
  for(int k2 = 0; k2<k; k2++){}
  //std::cout << "> fT["<<k<<"] = " << fT[k] << std::endl;
  
  // element i+1 is at the position for ch 
  // so make room for it
  //std::cout << 
  for (j=(int)fMult-1;j>i;j--){
    //std::cout << " j : " << j << std::endl;
    //std::cout << " e[j] : " << fE[j] << std::endl;
    fChlist[j+1]=fChlist[j];
    fT[j+1]=fT[j];
    fE[j+1]=fE[j];
    fPosition[j+1]=fPosition[j];
  }
  // and shove it in
  fChlist[i+1]=ch;
  fE[i+1]=egamma;
  fPosition[i+1]=pos_gamma;
  fT[i+1]=tgamma;
  fMult += 1;

  //std::cout << " fMult : " << fMult << std::endl;
  
  //for(int jkl = 0; jkl<fMult; jkl++){
  // std::cout << " Energy : " << fE[jkl] << std::endl;
  // std::cout << " Time   : " << fT[jkl] << std::endl;
  //}

  //if(fMult>2){
  //std::cout << ">---------------------------------" << std::endl;
  //std::cout << "> Pos Gamma : " << pos_gamma << std::endl;
  //std::cout << "> fPosition : " << fPosition[i+1] << std::endl;
  //for(int i2=0; i2<i+2; i2++)
  //  std::cout << "> fE["<<i2<<"] = " << fE[i2] << std::endl;
  //}
  return (i+1);
}



#endif
