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
#define S2FIXENABLE 0

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
  //std::cout << "Name  : "<< GetName() <<"fELin : " << fELin << std::endl;
  double tempx=0,tempy=0,tempz=0;
  detvar.GetParam(Form("%s.xpos",GetName()),tempx);
  detvar.GetParam(Form("%s.ypos",GetName()),tempy);
  detvar.GetParam(Form("%s.zpos",GetName()),tempz);
  std::cout << "Name  : "<< GetName() << "(x,y,z) pos: (" << tempx << "," << tempy << "," << tempz << ")" << std::endl;
  SetPosVect(TVector3(tempx,tempy,tempz));
  tempx=0,tempy=0,tempz=0;
  detvar.GetParam(Form("%s.shiftx",GetName()),tempx);
  detvar.GetParam(Form("%s.shifty",GetName()),tempy);
  detvar.GetParam(Form("%s.shiftz",GetName()),tempz);
  //std::cout << "Name  : "<< GetName() << "(x,y,z) shift: (" << tempx << "," << tempy << "," << tempz << ")" << std::endl;
  SetShiftVect(TVector3(tempx,tempy,tempz));
  tempx=0,tempy=0,tempz=0;
  detvar.GetParam(Form("%s.rotx",GetName()),tempx);
  detvar.GetParam(Form("%s.roty",GetName()),tempy);
  detvar.GetParam(Form("%s.rotz",GetName()),tempz);
  //  std::cout << "Name  : "<< GetName() << "(x,y,z) rot: (" << tempx << "," << tempy << "," << tempz << ")" << std::endl;
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


bool RN_S2Detector::inDet(const TVector3& v, const TVector3& beamspot){
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
  TVector3 ch_vect = v_to_det + beamspot - posvect;

  //radial component from center of detector
  double ch_vect_mag = ch_vect.Mag();

  //see if it falls in the detector window
  if((ch_vect_mag > outerrad)||(ch_vect_mag < innerrad)){
    return false;
  }
  return true;
}

bool RN_S2Detector::Vect_to_ch(const TVector3& v,double& cf,double& cb, const TVector3& beamspot){
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
  TVector3 ch_vect = v_to_det + beamspot- posvect;

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
  
  //=========================
  // Declare these variables:
  double s;
  double phi;

  //==============================================================
  // If the front and back channels are less than the total number 
  // of channels go in here:
  if((cf < static_cast<double>(front.NumOfCh())) 
     && (cb < static_cast<double>(back.NumOfCh()))){

    // s is the radius from the beam axis to the spot where
    // the particle hit.
    //double RandNumb1 = global::myRnd.Rndm();
    //double RandNumb2 = global::myRnd.Rndm();
    s = innerrad + (cf + (double)RNDMNUM)*ring_pitch_;
    //s = innerrad + (cf + RandNumb1)*ring_pitch_;
    //std::cout << " Random Number 1 : " << RNDMNUM << std::endl;  
    //std::cout << " Random Number 2 : " << (double)RNDMNUM << std::endl;
    // phi is the angle.  It is calculated by taking the 
    // number of front channels (16) subtracting th back
    // channel and then a random number, then multiplying by
    // the amount of phi per section.
    phi = (static_cast<double>(front.NumOfCh()) - cb 
       - (double)RNDMNUM) * delta_phi_;
    //phi = (static_cast<double>(front.NumOfCh())-cb-RandNumb2) * delta_phi_;

    //std::cout << " Random Number 3 : " << RNDMNUM << std::endl;
    //std::cout << " Random Number 4 : " << (double)RNDMNUM << std::endl;
    //std::cout << " RandNumb1 : " << RandNumb1 << std::endl;
    //std::cout << " RandNumb2 : " << RandNumb2 << std::endl;
  }
  else{ 
    std::cerr << "InValid channels sent to S2DetectorPar::chVect" << std::endl;
    exit(EXIT_FAILURE);
  }

  TVector3 resultv;
  
  // Set the magnitude, theta, and phi of the vector.
  // here the magnitude is s, theta is pi/2, and phi is from 
  // above.  
  //
  // Why take theta to be 90 degrees initially?
  resultv.SetMagThetaPhi(s,M_PI/2.,phi*M_PI/180.);
  
  //std::cout << "> Hit Vector Stuff ------" << std::endl;
  //std::cout << "> Before XYZ : (" << resultv.X() << ","<< resultv.Y() << ","<< resultv.Z() << ")" << std::endl;
  
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

  //std::cout << "> Aft Rot XYZ : (" << resultv.X() << ","<< resultv.Y() << ","<< resultv.Z() << ")" << std::endl;
  
  resultv = resultv + shiftv_;
  //std::cout << "> Aft shift XYZ : (" << resultv.X() << ","<< resultv.Y() << ","<< resultv.Z() << ")" << std::endl;

  resultv = resultv + posv_;
  //std::cout << "> Aft pos XYZ : (" << resultv.X() << ","<< resultv.Y() << ","<< resultv.Z() << ")" << std::endl;
  return resultv;
}


//ChVect2 takes out the randomization of angle in between strips
TVector3 RN_S2Detector::ChVect2(const double& cf,const double& cb) const{
  //First make a vector to the channel for a detector at the origin
  double s;
  double phi;

  if((cf < static_cast<double>(front.NumOfCh())) 
     && (cb < static_cast<double>(back.NumOfCh()))){
    s = innerrad + (cf*ring_pitch_);
    phi = (static_cast<double>(front.NumOfCh()) - cb) * delta_phi_;
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

  // Create these temporary things:
  RNTempList FrontClusters(in.front.NumOfCh()), BackClusters(in.back.NumOfCh());
  
  /*std::cout << "--------- In Reconstruct Clusters ---------- " << std::endl;

  std::cout << "> Back Mult  : " << in.back.Mult() << std::endl;
  std::cout << "> Front Mult : " << in.front.Mult() << std::endl;
  */
  // If  you have no hits get out of here:
  if(!in.back.Mult()) return 0;

  //========================
  // Declare some variables:
  unsigned int i,front_match;
  unsigned int frontstatus=0;
  unsigned int backstatus=0;
  float efronttotal=0.;
  float ebacktotal=0.;

  // first add-back neighboring hits
  // in the back

  //std::cout << "--- Checking Back Status ---" << std::endl;
  
  //===================================
  // Loop over the number of back hits:
  for (i=0;i<in.back.Mult();i++){

    //**************************************************************
    // It is not clear to me what this back channel backstatus does.
    /*std::cout << "------------------------" << std::endl;
    std::cout << "> Back Before  : " << backstatus << std::endl;
    std::cout << "> Back Channel : " << in.back.Ch(i) << std::endl;*/
    //Set the back status based on the channel.
    backstatus |= (((unsigned int)in.back.Ch(i)));
    //    std::cout << "> Back After  : " << backstatus << std::endl;



    // the energy back total equals the sum here.  Note, in.Back_E calls
    // back in.back.E(i)*RN_S2Detector.fELin + RN_S2Detector.fEShift
    // and in.back.E(i) = in.back.ELocal(i)*in.back.fELin + in.back.fEShift
    // and in.back.ELocal(i) = in.back.EOffset(i)*in.back.fA1[in.back.fchlist(i)] + in.back.fA0[in.back.fchlist(i)]
    // and in.back.EOffset(i) = in.back.fE[i] + in.back.fQOffset[in.back.fchlist(i)]

    //=======================================================
    // NOTE --> This "ebacktotal" does not appear to be used.
    //    std::cout << "> Back En Before Add : " << ebacktotal << std::endl;
    ebacktotal = ebacktotal+in.Back_E(i);
    //std::cout << "> Back En After Add : " << ebacktotal << std::endl;

    //=======================================================================
    // If addback_back && other stuff.  "addback_back" is a variable that is
    // set in your parameters file.  As of now, It is not set for me, and
    // therfore this part is not being used for my analysis right now
    // - JAB 9/26/14 19Ne(d,n)20Na
    if (addback_back&&
	(i+1<in.back.Mult())&&(in.back.Ch(i+1)==in.back.Ch(i)+1)){

      // however, if this "addback_back" == 1 then we go in here if
      // we also have good multiplicity, and the back channels are 
      // adjacent.

      //===============================================================
      // In this case the ecluster is the sum of the two back energies:
      float ecluster=in.Back_E(i)+in.Back_E(i+1);

      //=====================================================
      // average the time, and average the channel...strange:
      float tcluster=(in.Back_T(i+1)+in.Back_T(i))/2.0;
      float chnew=(in.back.Ch(i+1)+in.back.Ch(i))/2.0;
      //float chnew = in.back.Ch(i);

      // insert this hit into the cluster.
      BackClusters.InsertHit(ecluster,tcluster,chnew);
      // we analyzed the i+1 hit along with the i, so 
      i++;
    }
    else{

      // If we do not have "addback_back" turned on, or we do not have
      // adjacent hits, then go in here and act accordingly.
      float ch=in.back.Ch(i);
      float ecluster=in.Back_E(i);
      float tcluster=in.Back_T(i);
      
      BackClusters.InsertHit(ecluster,tcluster,ch);
    }
  }// end loop over back multiplicity.
  
  //===============================
  // Now do the same for the front:
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
  } // end loop over front multiplicity.


  //==================================
  //Set these variables equal to zero.
  front_match=0;i=0;
  efrontmatch=0.;ebackmatch=0.;


  // Output the values for the detectors:
  /*
  std::cout << " --- Start --------------------------------------" << std::endl;
  std::cout << " Det " << in.Name() << " Back ------------------" << std::endl;
  for(int i_Test = 0; i_Test<BackClusters.mult; i_Test++){
    std::cout << " Back Channel : " << BackClusters.chlist[i_Test] << " & Energy : " << BackClusters.elist[i_Test] << std::endl;
    
  }
  std::cout << " Det " << in.Name() << " Front ------------------" << std::endl;
  for(int i_Test = 0; i_Test<FrontClusters.mult; i_Test++){
    std::cout << " Front Channel : " << FrontClusters.chlist[i_Test] << " & Energy : " << FrontClusters.elist[i_Test] << std::endl;
    
  }
  */

  //========================================================
  // Front and Backclusters are sorted by energy, high first
  while ((i<BackClusters.mult)&&(front_match<FrontClusters.mult)){

    /*std::cout << " -- In the front back matching section --" << std::endl;
    std::cout << "> Front En : " << FrontClusters.elist[front_match] << std::endl;
    std::cout << "> Back En  : " << BackClusters.elist[i] << std::endl;
    */

    //============================================================
    // Get the difference between front and back energy, with the
    // highest energy events first.
    float eps = FrontClusters.elist[front_match]-BackClusters.elist[i];
    
    //std::cout << "> En Diff  : " << eps << std::endl;

    //================================================================
    // Calculate delta, this is the match_epsilon +match_delta*Energy.
    // the "match_epsilon" and "match_delta" are set in the config 
    // and calibration file.  
    float delta = match_epsilon+match_delta*BackClusters.elist[i];

    //std::cout << "> Delta    : " << delta << std::endl;
    

    //=====================
    // Get the back energy:
    float back_e=BackClusters.elist[i];

    //std::cout << "> Back En  : " << back_e << std::endl;
    
    //===============================================================
    // If the back energy is greater than the "match_maxene" which is
    // initilized to 4096 (the number of channels in the det) and set
    // manually in the config/calibration file.  OR if the value of
    // the difference between the front and the back is less than
    // or equal to the calculated delta, then go in here.
    if (back_e>match_maxene || (fabs(eps)<=delta)){
      // we have a match

      //std::cout << "> We are in the match if statement" << std::endl;
      
      //================================================
      // Set the "match_e" and "match_ch" from the FRONT.
      float match_e= FrontClusters.elist[front_match];
      float match_ch= FrontClusters.chlist[front_match];

      //======================================================
      // get the back channel, and get the time from the BACK.
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
      
      // Get the position.  The position is obtained via this
      // chVect() function.  The inputs of this are the front
      // channel (match_ch) and the back channel (cb).
      /*  std::cout << "> About to set position ** " << std::endl;
      std::cout << "> Front Channel : " << match_ch << std::endl;
      std::cout << "> Back Channel  : " << cb << std::endl;*/
      fPos[fMult] = in.chVect(match_ch,cb);
      fChlist[fMult]=match_ch;
      fChlist_b[fMult]=cb;

      //===================================================
      // If you want the energy from the back, get it here:
      if (match_enefromback){
	fE[fMult]=back_e;
	fT[fMult]=back_t;
       }
      else{
	fE[fMult]=match_e;
	fT[fMult]=back_t;

      }// end else "match_enefromback"
      // Increase the multiplicity 
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
  /*
  std::cout << " Energy Used      : " << fE[0] << std::endl;
  std::cout << " Back Chan Used  : " << fChlist_b[0] << std::endl;
  std::cout << " Front Chan Used : " << fChlist[0] << std::endl;
  std::cout << " --- end ----------------------------------------" << std::endl;
  */
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
