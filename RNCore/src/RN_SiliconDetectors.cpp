/***************************************************************
Class: RN_S2Detector, RN_S2Cluster, RN_SiArray, RN_S1Detector
Author: Sean A Kuvin
****************************************************************/
#ifndef __RN_SILICON_CXX
#define __RN_SILICON_CXX
#include "RN_SiliconDetectors.hpp"
#include "RN_Root.hpp"

ClassImp(RN_S2Detector);
ClassImp(RN_S2Cluster);
ClassImp(RN_SiArray);
ClassImp(RN_S1Detector);

using global::myRnd;

//S1FIXENABLE was needed for some data sets where the S1 Detector was incorrectly plugged in
#define S1FIXENABLE 0

//S2FIXENABLE to correct energy discrepency when front.ch = back.ch?
#define S2FIXENABLE 1

//To enable/disable randomization of the angle during cluster reconstruction
#if 1
#define RNDMNUM global::myRnd.Rndm()
#else
#define RNDMNUM 0
#endif



////////////////////////////////////////////////////////////////////
///RN_S2Detector
////////////////////////////////////////////////////////////////////

void RN_S2Detector::Reset(){
  front.Reset();
  back.Reset();
}

void RN_S2Detector::SetCalibrations(double elin,double eshift,double tlin, double tshift){
  this->fELin=elin;
  this->fEShift=eshift;
  this->fTLin=tlin;
  this->fTShift=tshift;
}


RN_S2Detector::RN_S2Detector(std::string name,const int& fnum, const int& bnum):RN_BaseClass(name,name),
  fELin(1),
  fEShift(0),
  fTLin(1),
  fTShift(0),
  normv_(0,0,0),
  shiftv_(0,0,0),
  posv_(0,0,0),
  rotv_(0,0,0),
  _s1switch(0),
  front(name+".front",fnum),
  back(name+".back",bnum)
{
  
  
  outerrad=S2OUTERRAD;
  innerrad=S2INNERRAD;
  ring_pitch_ = (S2OUTERRAD - S2INNERRAD) / static_cast<double>(front.NumOfCh());
  delta_phi_ = 360. / static_cast<double>(back.NumOfCh());
  front.SetELimits(0,4095);
  back.SetELimits(0,4095);

  //  Build();
}

//void RN_S2Detector::Build(){
//  gRNROOT.AddDetector(this);
//}

void RN_S2Detector::SetCalibrations(RN_VariableMap& detvar){
  //pass the calibrations to the RN_BaseDetectors
  front.SetCalibrations(detvar);
  back.SetCalibrations(detvar);

  //get the calibrations specific to this class
  detvar.GetParam(Form("%s.elin",GetName()),fELin);
  detvar.GetParam(Form("%s.eshift",GetName()),fEShift);
  detvar.GetParam(Form("%s.tlin",GetName()),fTLin);
  detvar.GetParam(Form("%s.tshift",GetName()),fTShift);
  double tempx=0,tempy=0,tempz=0;
  detvar.GetParam(Form("%s.xpos",GetName()),tempx);
  detvar.GetParam(Form("%s.ypos",GetName()),tempy);
  detvar.GetParam(Form("%s.zpos",GetName()),tempz);
  SetPosVect(TVector3(tempx,tempy,tempz));
  tempx=0,tempy=0,tempz=0;
  detvar.GetParam(Form("%s.shiftx",GetName()),tempx);
  detvar.GetParam(Form("%s.shifty",GetName()),tempy);
  detvar.GetParam(Form("%s.shiftz",GetName()),tempz);
  SetShiftVect(TVector3(tempx,tempy,tempz));
  tempx=0,tempy=0,tempz=0;
  detvar.GetParam(Form("%s.rotx",GetName()),tempx);
  detvar.GetParam(Form("%s.roty",GetName()),tempy);
  detvar.GetParam(Form("%s.rotz",GetName()),tempz);
  SetRotVect(TVector3(tempx,tempy,tempz));
  
  Calcnormv();

}


void RN_S2Detector::Calcnormv(){
  TVector3 resultv = (shiftv_ + posv_).Unit();
  if(rotv_[0]){
    resultv.RotateX(rotv_[0]*M_PI/180.);
  }
  if(rotv_[1]){
    resultv.RotateY(rotv_[1]*M_PI/180.);
  }
  if(rotv_[2]){
    resultv.RotateZ(rotv_[2]*M_PI/180.);
  }
  
  normv_ = resultv;
}

//apply global calibrations elin and eshift to calibrated basedetectors front and back

Double_t RN_S2Detector::Front_E(unsigned int i) const{
  if(i>=front.Mult())
    return 0;
  if (!(front.ERaw(i)>0))
    return 0;
  return ((front.E(i) * fELin) + fEShift);
}

Double_t RN_S2Detector::Front_T(unsigned int i) const{
  if(i>=front.Mult())
    return 0;
  if (!(front.TRaw(i)>0))
    return 0;
  return (( front.T(i) * fTLin ) + fTShift);
  
}

Double_t RN_S2Detector::Back_E(unsigned int i) const{
  if(i>=back.Mult())
    return 0;
  if(!(back.ERaw(i)>0))
    return 0;
  return (( back.E(i) * fELin) + fEShift);   
}

Double_t RN_S2Detector::Back_T(unsigned int i) const{
  if (i>=back.Mult())
    return 0;
  if (!(back.TRaw(i)>0))
    return 0;
  return ((back.T(i) * fTLin)  + fTShift);
}




Double_t RN_S2Detector::InnerTheta()const{
  if(GetPosVect().Z()>0)
    return TMath::ATan(innerrad/GetPosVect().Z())*180.0/3.14;
  else return 0;
}
Double_t RN_S2Detector::OuterTheta()const{
  if(GetPosVect().Z()>0)
    return TMath::ATan(outerrad/GetPosVect().Z())*180.0/3.14;
  else return 0;
}

//these functions were added when I was trying to add an S1-fix for when
//S1 detector was incorrectly plugged in. This fix was eventually added
//just to the ReconstructCluster method
/*
int RN_S2Detector::Quadrant(unsigned int i)const{
  return floor(back.Ch(i)/4);
}
int RN_S2Detector::Side(unsigned int i)const{
  return floor(back.Ch(i)/2);
}

Double_t RN_S2Detector::Ring_Ch(unsigned int i)const{
  if(!_s1switch){ //If not an S1Detector just return the front.Ch() as usual
    return front.Ch(i);
  }
  else{
    //first 8 don't change anything
    if(Side()==1){
      return front.Ch(i);
    }
    //second 8 flip odd/even
    else{
      if((int)front.Ch(i)%2!=0){
	return front.Ch(i)-1;
      }
      else
	return front.Ch(i)+1;
    }
  }
    

}
*/


bool RN_S2Detector::inDet(const TVector3& v){
  //first see if it intersects the plane of the detector
  double udotnormv = (v.Unit()).Dot(normv_);
  if(udotnormv <= 0.)
    return false;

  //Find distance from target to interesection point of detector plane 
  //use line-plane intersection formula
  TVector3 posvect = GetPosVect();
  double vdist = normv_.Dot(posvect)/udotnormv;
  if(vdist <= 0.)
    return false;
  
  //vector from target to interesection point of detector plane with
  //magnitude equal to distance
  TVector3 v_to_det(v);
  v_to_det.SetMag(vdist);

  //create vector from detector origin to interaction point on plane of detector
  TVector3 ch_vect = v_to_det - posvect;

  //radial component from center of detector
  double ch_vect_mag = ch_vect.Mag();

  //see if it falls in the detector window
  if((ch_vect_mag > outerrad)||(ch_vect_mag < innerrad)){
    return false;
  }
  return true;
}

bool RN_S2Detector::Vect_to_ch(const TVector3& v,double& cf,double& cb){
  //first see if it intersects the plane of the detector
  double udotnormv = (v.Unit()).Dot(normv_);
  if(udotnormv <= 0.)
    return false;

  //Find distance from target to interesection point of detector plane 
  //use line-plane intersection formula
  TVector3 posvect = GetPosVect();
  double vdist = normv_.Dot(posvect)/udotnormv;
  if(vdist <= 0.)
    return false;
  
  //vector from target to interesection point of detector plane with
  //magnitude equal to distance
  TVector3 v_to_det(v);
  v_to_det.SetMag(vdist);

  //create vector from detector origin to interaction point on plane of detector
  TVector3 ch_vect = v_to_det - posvect;

  //radial component from center of detector
  double ch_vect_mag = ch_vect.Mag();

  //see if it falls in the detector window
  if((ch_vect_mag > outerrad)||(ch_vect_mag < innerrad)){
    return false;
  }
  
  //find the ring
  cf = (ch_vect_mag-innerrad)/ring_pitch_;
  
  double phi = ch_vect.Phi()*180./M_PI;
  //switch range of phi from (-180,180) to (0,360)
  phi += 360.;
  if(phi > 360.)
    phi = phi - 360.;
  
  //find the segment
  cb = static_cast<double>(back.NumOfCh()) - (phi/delta_phi_);
  
  return true;
}


TVector3 RN_S2Detector::chVect(const double& cf,const double& cb) const{
  //First make a vector to the channel for a detector at the origin
  double s;
  double phi;

  if((cf < static_cast<double>(front.NumOfCh())) 
     && (cb < static_cast<double>(back.NumOfCh()))){
    s = innerrad + (cf + (double)RNDMNUM)*ring_pitch_;
    phi = (static_cast<double>(front.NumOfCh()) - cb 
	   - (double)RNDMNUM) * delta_phi_;
  }
  else{ 
    std::cerr << "InValid channels sent to S2DetectorPar::chVect" << std::endl;
    exit(EXIT_FAILURE);
  }

  TVector3 resultv;
  resultv.SetMagThetaPhi(s,M_PI/2.,phi*M_PI/180.);
  
  //then calibrate the vector 
  //add it to the detector position vector
  if(rotv_[2]){
    resultv.RotateZ(rotv_[2]*M_PI/180.);
  }
  if(rotv_[0]){
    resultv.RotateX(rotv_[0]*M_PI/180.);
  }
  if(rotv_[1]){
    resultv.RotateY(rotv_[1]*M_PI/180.);
  }

  resultv = resultv + shiftv_;
  resultv = resultv + posv_;
    
  return resultv;
}



RN_S2Cluster::RN_S2Cluster(std::string name,Int_t NumOfCh):RN_BaseDetector(name,NumOfCh),
		 efrontmatch(0),
		 ebackmatch(0),
		 frontmatchstat(0),
		 backmatchstat(0),
		 fChlist_b(NumOfCh,-1.),
		 fPos(NumOfCh)
{
  
  Reset();
  match_enefromback=1.0;
  match_epsilon=0.0;
  match_delta=0.1;
  match_maxene=4096;
  addback_front=0.0; 
  addback_back=0.0;
  
}

int RN_S2Cluster::SetMatchParameters(float match_enefromback,
				     float match_epsilon,
				     float match_delta,
				     float match_maxene,
				     float addback_front, 
				     float addback_back){
  this->match_enefromback=match_enefromback;
  this->match_epsilon=match_epsilon;
  this->match_delta=match_delta;
  this->match_maxene=match_maxene;
  this->addback_front=addback_front;
  this->addback_back=addback_back;

  return 1;

}
void RN_S2Cluster::SetCalibrations(RN_VariableMap& detvar){
  detvar.GetParam(Form("%s.match_enefromback",GetName()),match_enefromback);
  detvar.GetParam(Form("%s.match_epsilon",GetName()),match_epsilon);
  detvar.GetParam(Form("%s.match_delta",GetName()),match_delta);
  detvar.GetParam(Form("%s.match_maxene",GetName()),match_maxene);
  detvar.GetParam(Form("%s.addback_front",GetName()),addback_front);
  detvar.GetParam(Form("%s.addback_back",GetName()),addback_back);
}

int RN_S2Cluster::ReconstructClusters(RN_S2Detector& in){
  RNTempList FrontClusters(in.front.NumOfCh()), BackClusters(in.back.NumOfCh());
  if(!in.back.Mult()) return 0;
  unsigned int i,front_match;
  unsigned int frontstatus=0;
  unsigned int backstatus=0;
  float efronttotal=0.;
  float ebacktotal=0.;

  // first add-back neighboring hits
  // in the back
  for (i=0;i<in.back.Mult();i++){
    backstatus |= (((unsigned int)in.back.Ch(i)));
    ebacktotal = ebacktotal+in.Back_E(i);
    if (addback_back&&
	(i+1<in.back.Mult())&&(in.back.Ch(i+1)==in.back.Ch(i)+1)){
      float ecluster=in.Back_E(i)+in.Back_E(i+1);
      float tcluster=(in.Back_T(i+1)+in.Back_T(i))/2.0;
      float chnew=(in.back.Ch(i+1)+in.back.Ch(i))/2.0;
      //float chnew = in.back.Ch(i);
      BackClusters.InsertHit(ecluster,tcluster,chnew);
      // we analyzed the i+1 hit along with the i, so 
      i++;
    }
    else{
      float ch=in.back.Ch(i);
      float ecluster=in.Back_E(i);
      float tcluster=in.Back_T(i);
      
      BackClusters.InsertHit(ecluster,tcluster,ch);
    }
  }
  //in the front
  for (i=0;i<in.front.Mult();i++){
    efronttotal = efronttotal+in.Front_E(i);
  }
  for (i=0;i<in.front.Mult();i++){
    frontstatus |= (((unsigned int)in.front.Ch(i)));
    if (addback_front&&
	(i+1<in.front.Mult())&&(in.front.Ch(i+1)==in.front.Ch(i)+1)){
      float ecluster=in.Front_E(i+1)+in.Front_E(i);
      float tcluster=(in.Front_T(i+1)+in.Front_T(i))/2.0;
      float chnew=(in.front.Ch(i+1)+in.front.Ch(i))/2.0;
      //float chnew = in.front.Ch(i);
      FrontClusters.InsertHit(ecluster,tcluster,chnew);
      // we analyzed the i+1 hit along with the i, so 
      i++;
    }
    else{
      float ch=in.front.Ch(i);
      float ecluster=in.Front_E(i);
      float tcluster=in.Front_T(i);
      FrontClusters.InsertHit(ecluster,tcluster,ch);
    }
  }
  front_match=0;i=0;
  efrontmatch=0.;ebackmatch=0.;
  // Front and Backclusters are sorted by energy, high first
  while ((i<BackClusters.mult)&&(front_match<FrontClusters.mult)){
    float eps = FrontClusters.elist[front_match]-BackClusters.elist[i];
    float delta = match_epsilon+match_delta*BackClusters.elist[i];
    float back_e=BackClusters.elist[i];
    if (back_e>match_maxene || (fabs(eps)<=delta)){
      // we have a match
      float match_e= FrontClusters.elist[front_match];
      float match_ch= FrontClusters.chlist[front_match];
      float cb=BackClusters.chlist[i];
      float back_t=BackClusters.tlist[i];

      //temporary fix for the S1 mistake made during 2013 runs
      //swapping odd-even ring pairs depending on what half
      //of the detector was hit.
#if S1FIXENABLE
      if(in.IsS1()){
	if( floor((int)cb / 8) == 0){
	  if((int)match_ch % 2 !=0 ){
	    match_ch = match_ch - 1;
	  }
	  else
	    match_ch = match_ch + 1;
	}
      }
#endif
      
#if S2FIXENABLE
      if(match_ch == cb){
	//back_e = (back_e * 1.03529411);
	//match_e = (match_e * 1.035294118);
	return 0;
      }
      if((match_ch + 1) == cb){
	//back_e = (back_e * 1.023255814);
	//match_e = (match_e * 1.023255814);
	return 0;
      }

#endif      
      
      
      fPos[fMult] = in.chVect(match_ch,cb);
      fChlist[fMult]=match_ch;
      fChlist_b[fMult]=cb;
      if (match_enefromback){
	fE[fMult]=back_e;
	fT[fMult]=back_t;
       }
      else{
	fE[fMult]=match_e;
	fT[fMult]=back_t;

      }// end if "match_enefromback"
      (fMult)++;
      {	
	// sum up matched energies front and back
	efrontmatch += match_e;
	ebackmatch += back_e;
      }
    }// end found match
    else if (eps>delta){
      // front energy is larger than back energy+delta , 
      // so we won't ever match 
      // this one in the future, drop it, pick next front, keep the back
      frontstatus|=128; // flags "unmatched" 
      front_match++;
      continue;
    }
    else if (eps< (-delta)){
      // back energy is larger than front energy-delta , 
      // so we won't ever match 
      // this one in the future, drop it, pick next back, keep the front
      backstatus|=128; // flags "unmatched" 
      i++;
      continue;
    }

    front_match++;
    i++;
  }
  frontmatchstat=frontstatus;
  backmatchstat=backstatus;
  return(0);
}



void RN_S2Cluster::Reset(){
  
  for(unsigned int i=0;i<fMult;i++){
    fPos[i].SetXYZ(0.,0.,0.);
    fChlist_b[i]=-1;
  }
   
  RN_BaseDetector::Reset();
 
}

RN_SiArray::RN_SiArray(const std::string& name, int num):RN_BaseClass(name),
							 fNumOfSi(num),
							 fE_(num,0),
							 fPos_(num,TVector3(0,0,0)),
							 fT_(num,0)
{
  for(unsigned int i=0;i<4;i++){
    fP[i] = 0;
  }
  
}

void RN_SiArray::SetCalibrations(RN_VariableMap& detvar){
  detvar.GetParam(Form("%s.p0",GetName()),fP[0]);
  detvar.GetParam(Form("%s.p1",GetName()),fP[1]);
  detvar.GetParam(Form("%s.p2",GetName()),fP[2]);
  detvar.GetParam(Form("%s.p3",GetName()),fP[3]);

}


void RN_SiArray::Reset(){
  for(int i=0;i<fNumOfSi;i++){
    fE_[i] = 0;
    fT_[i] = 0;
    fPos_[i].SetXYZ(0,0,0);
  }
  
}

void RN_SiArray::ReconstructHits(RN_S2ClusterCollection& si_c_)
{
  for(unsigned int i=0;i<si_c_.size();i++){
    fE_[i] = si_c_[i].ERaw(0); 
    fPos_[i] = si_c_[i].fPos[0]; 
    fT_[i] = si_c_[i].TRaw(0); 
  }
  
}



/////////////////////////////////////////////////////////////////
///RNTempList functions
////////////////////////////////////////////////////////////////


RNTempList::RNTempList(const unsigned short no_channels){
  mult=0;
  no_channels_=no_channels;
  chlist = new float[no_channels];
  elist = new float[no_channels];
  tlist = new float[no_channels];
}


RNTempList::RNTempList(){
}

RNTempList::~RNTempList(){
  delete[] chlist;
  delete[] elist;
  delete[] tlist;
}
void RNTempList::InsertHit(float e,float t, float ch){
 
  int i,j;
  /* enough space ? */
  if (mult>=no_channels_){
    return;
  }
  /* insert into list sorted by energy */
  for (i=(int)mult-1;i>=0;i--){
    if (e<elist[i])
      break;
  }
  // element i+1 is at the position for ch 
  // so make room for it
  for (j=(int)mult-1;j>i;j--){
    chlist[j+1]=(float)chlist[j];
    elist[j+1]=(float)elist[j];
    tlist[j+1]=(float)tlist[j];
  }
  // and shove it in
  chlist[i+1]=ch;
  elist[i+1]=e;
  tlist[i+1]=t;
  mult++;
}

#endif
