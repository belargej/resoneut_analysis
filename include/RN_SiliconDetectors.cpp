#ifndef __RN_SILICON_CXX
#define __RN_SILICON_CXX
#include "RN_SiliconDetectors.hpp"


ClassImp(RN_S2Detector);



////////////////////////////////////////////////////////////////////
///RN_S2Detector
////////////////////////////////////////////////////////////////////

void RN_S2Detector::Reset(){
  front.Reset();
  back.Reset();

}
void RN_S2Detector::SetCalibrations(double elin,double eshift,double tlin, double tshift){
  this->elin=elin;
  this->eshift=eshift;
  this->tlin=tlin;
  this->tshift=tshift;

}

RN_S2Detector::RN_S2Detector(std::string name,const int& fnum, const int& bnum):fName(name),
										elin(1),
										eshift(0),
										tlin(1),
										tshift(0),
										fronta0(fnum,double(0)),
										fronta1(fnum,double(1)),
										backa0(bnum,double(0)),
										backa1(bnum,double(1)),
										frontt0(fnum,double(0)),
										frontt1(fnum,double(1)),
										backt0(fnum,double(0)),
										backt1(bnum,double(1)),									
										normv_(0,0,0),
										shiftv_(0,0,0),
										posv_(0,0,0),
										rotv_(0,0,0),
										front("front",fnum),
										back("back",bnum)
								 
								 
  {
   

    outerrad=S2OUTERRAD;
    innerrad=S2INNERRAD;
    ring_pitch_ = (S2OUTERRAD - S2INNERRAD) / static_cast<double>(front.NumOfCh());
    delta_phi_ = 360. / static_cast<double>(back.NumOfCh());
    front.SetELimits(0,3500);
    back.SetELimits(0,3500);
  }

void RN_S2Detector::SetCalibrations(RN_VariableMap& detvar){
  for (int i=0;i<front.NumOfCh();i++){
    detvar.GetParam(Form("%s.front.a0[%d]",Name().c_str(),i),fronta0[i]);  
    detvar.GetParam(Form("%s.front.a1[%d]",Name().c_str(),i),fronta1[i]);
    detvar.GetParam(Form("%s.front.t0[%d]",Name().c_str(),i),frontt0[i]);  
    detvar.GetParam(Form("%s.front.t1[%d]",Name().c_str(),i),frontt1[i]);
  }
  for (int i=0;i<back.NumOfCh();i++){
    detvar.GetParam(Form("%s.back.a0[%d]",Name().c_str(),i),backa0[i]);  
    detvar.GetParam(Form("%s.back.a1[%d]",Name().c_str(),i),backa1[i]);
    detvar.GetParam(Form("%s.back.t0[%d]",Name().c_str(),i),backt0[i]);  
    detvar.GetParam(Form("%s.back.t1[%d]",Name().c_str(),i),backt1[i]);

  }
  detvar.GetParam(Form("%s.elin",Name().c_str()),elin);
  detvar.GetParam(Form("%s.eshift",Name().c_str()),eshift);
  detvar.GetParam(Form("%s.tlin",Name().c_str()),tlin);
  detvar.GetParam(Form("%s.tshift",Name().c_str()),tshift);
  double tempx=0,tempy=0,tempz=0,temp=0;
  detvar.GetParam(Form("%s.xpos",Name().c_str()),tempx);
  detvar.GetParam(Form("%s.ypos",Name().c_str()),tempy);
  detvar.GetParam(Form("%s.zpos",Name().c_str()),tempz);
  SetPosVect(TVector3(tempx,tempy,tempz));
  tempx=0,tempy=0,tempz=0,temp=0;
  detvar.GetParam(Form("%s.shiftx",Name().c_str()),tempx);
  detvar.GetParam(Form("%s.shifty",Name().c_str()),tempy);
  detvar.GetParam(Form("%s.shiftz",Name().c_str()),tempz);
  SetShiftVect(TVector3(tempx,tempy,tempz));
  tempx=0,tempy=0,tempz=0,temp=0;
  detvar.GetParam(Form("%s.rot_theta",Name().c_str()),tempx);
  detvar.GetParam(Form("%s.rot_phi",Name().c_str()),tempy);
  double flowlimit=0,fhighlimit=0,blowlimit=0,bhighlimit=0;
  detvar.GetParam(Form("%s.front.lowlimit",Name().c_str()),flowlimit);
  detvar.GetParam(Form("%s.front.highlimit",Name().c_str()),fhighlimit);
  detvar.GetParam(Form("%s.back.lowlimit",Name().c_str()),blowlimit);
  detvar.GetParam(Form("%s.back.highlimit",Name().c_str()),bhighlimit);
  if(flowlimit>0&&fhighlimit>0)
    front.SetELimits(flowlimit,fhighlimit);
  if(blowlimit>0&&bhighlimit>0)
    back.SetELimits(blowlimit,bhighlimit);
  
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

Double_t RN_S2Detector::Front_E(int i) const{
  if(i>front.fMult)
    return -1;
  if (!front.fE[i]>0)
    return 0;
  return (((fronta1[(int)front.fChlist[i]] * front.fE[i]) + fronta0[(int)front.fChlist[i]]) * elin + eshift);
}

Double_t RN_S2Detector::Front_T(int i) const{
  if(i>front.fMult)
    return -1;
  if (!front.fT[i]>0)
    return 0;
  return ((( front.fT[i] * frontt1[(int)front.fChlist[i]]) + frontt0[(int)front.fChlist[i]] ) * tlin  + tshift);
  
}

Double_t RN_S2Detector::Back_E(int i) const{
  if(i>back.fMult)
    return -1;
  if(!back.fE[i]>0)
    return 0;
  return (((backa1[(int)back.fChlist[i]] * back.fE[i] )+backa0[(int)back.fChlist[i]])*elin+eshift);   
}

Double_t RN_S2Detector::Back_T(int i) const{
  if (i>back.fMult)
    return -1;
  if (!back.fT[i]>0)
    return 0;
  return ((( back.fT[i] * backt1[(int)back.fChlist[i]]) + backt0[(int)back.fChlist[i]] ) * tlin  + tshift);
}

Double_t RN_S2Detector::InnerTheta()const{
  if(GetPosVect().Z()>0)
    return TMath::ATan(innerrad/GetPosVect().Z())*180/3.14;
  else return 0;
}
Double_t RN_S2Detector::OuterTheta()const{
  if(GetPosVect().Z()>0)
    return TMath::ATan(outerrad/GetPosVect().Z())*180/3.14;
  else return 0;
}



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
  TRandom3 myRnd(0);//!
  double s;
  double phi;
  if((cf < static_cast<double>(front.NumOfCh())) 
     && (cb < static_cast<double>(back.NumOfCh()))){
    s = innerrad + (cf + (double)myRnd.Rndm())*ring_pitch_;
    phi = (static_cast<double>(front.NumOfCh()) - cb 
	   - (double)myRnd.Rndm())*delta_phi_;
  }
  else{ 
    std::cerr << "InValid channels sent to S2DetectorPar::chVect" << std::endl;
    exit(EXIT_FAILURE);
  }

  TVector3 resultv;
  resultv.SetMagThetaPhi(s,M_PI/2.,phi*M_PI/180.);
  
  //then calibrate the vector 
  //add it to the detector position vector
  if(rotv_[0]){
    resultv.RotateX(rotv_[0]*M_PI/180.);
  }
  if(rotv_[1]){
    resultv.RotateY(rotv_[1]*M_PI/180.);
  }
  if(rotv_[2]){
    resultv.RotateZ(rotv_[2]*M_PI/180.);
  }
  resultv = resultv + shiftv_;
  resultv = resultv + posv_;
    
  return resultv;
}



RN_S2Cluster::RN_S2Cluster(std::string name,Int_t NumOfCh):RN_BaseDetector(name,NumOfCh),
							   fChlist_b(NumOfCh,-1.),
							   fPos(NumOfCh){
  
  Reset();
  match_enefromback=1.0;
  match_epsilon=0.0;
  match_delta=0.1;
  match_maxene=4096;
  addback_front=1.0; 
  addback_back=1.0;
  
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
  detvar.GetParam(Form("%s.match_enefromback",Name().c_str()),match_enefromback);
  detvar.GetParam(Form("%s.match_epsilon",Name().c_str()),match_epsilon);
  detvar.GetParam(Form("%s.match_delta",Name().c_str()),match_delta);
  detvar.GetParam(Form("%s.match_maxene",Name().c_str()),match_maxene);
  detvar.GetParam(Form("%s.addback_front",Name().c_str()),addback_front);
  detvar.GetParam(Form("%s.addback_back",Name().c_str()),addback_back);
}

int RN_S2Cluster::ReconstructClusters(RN_S2Detector& in){
  RNTempList FrontClusters(in.front.NumOfCh()), BackClusters(in.back.NumOfCh());
  if(!in.back.fMult) return 0;
  int i,front_match;
  unsigned int frontstatus=0;
  unsigned int backstatus=0;
  float efronttotal=0.;
  float ebacktotal=0.;

  // first add-back neighboring hits
  // in the back
  for (i=0;i<in.back.fMult;i++){
    backstatus |= (((unsigned int)in.back.fChlist[i]));
    ebacktotal = ebacktotal+in.Back_E(i);
    if (addback_back&&
	(i+1<in.back.fMult)&&(in.back.fChlist[i+1]==in.back.fChlist[i]+1)){
      float ecluster=in.Back_E(i)+in.Back_E(i+1);
      float tcluster=(in.Back_T(i+1)+in.Back_T(i))/2;
      float chnew=(in.back.fChlist[i+1]+in.back.fChlist[i])/2.0;
      BackClusters.InsertHit(ecluster,tcluster,chnew);
      // we analyzed the i+1 hit along with the i, so 
      i++;
    }
    else{
      int ch=in.back.fChlist[i];
      float ecluster=in.Back_E(i);
      float tcluster=in.Back_T(i);
      
      BackClusters.InsertHit(ecluster,tcluster,ch);
    }
  }
  //in the front
  for (i=0;i<in.front.fMult;i++){
    efronttotal = efronttotal+in.Front_E(i);
  }
  for (i=0;i<in.front.fMult;i++){
    frontstatus |= (((unsigned int)in.front.fChlist[i]));
    if (addback_front&&
	(i+1<in.front.fMult)&&(in.front.fChlist[i+1]==in.front.fChlist[i]+1)){
 
 
      float ecluster=in.Front_E(i+1)+in.Front_E(i);
      float tcluster=(in.Front_T(i+1)+in.Front_T(i))/2;
      float chnew=(in.front.fChlist[i+1]+in.front.fChlist[i])/2.0;
      FrontClusters.InsertHit(ecluster,tcluster,chnew);
      // we analyzed the i+1 hit along with the i, so 
      i++;
    }
    else{
      int ch=in.front.fChlist[i];
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
  for(int i=0;i<fMult;i++){
    fPos[i].SetXYZ(0,0,0);
    fChlist_b[i]=-1;
  }
   
  RN_BaseDetector::Reset();
 

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
