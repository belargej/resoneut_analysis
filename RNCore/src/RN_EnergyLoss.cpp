///////////////////////////////////////////////////////////////////////
//Class: RN_EnergyLoss
//
// Adapted from ANASEN code method by Joe Belarge and Dr. Rogachev
// This class reads a SRIM Table to calculate energy loss values
//
////////////////////////////////////////////////////////////////////////
#include "RN_EnergyLoss.hpp"
#include "sak_ReadBuffer.hpp"
#include "RN_Root.hpp"

#ifndef RN_ENLOSS_CXX
#define RN_ENLOSS_CXX


using namespace eneloss;
using namespace std;

RN_EnergyLoss::RN_EnergyLoss(const std::string&name):RN_BaseClass(name),
						     fNpoints(0){

  for(int i=0;i<gNumLossMax;i++){
    fEnergy[i]=0;
    fdEdXE[i]=0;
    fdEdXN[i]=0;
  }

}

void RN_EnergyLoss::Init(const std::string& projectile,const std::string& target,const std::string&option,const std::string& pressure,const double& EMax,Int_t steps){
  
  std::ifstream masses;
  std::string filename="";
  char* pPath;
  pPath = getenv ("RN_ANALYSIS");
  if (pPath!=NULL){
    //    printf ("The current path is: %s\n",pPath);
    filename = (std::string)pPath + "/input/SRIMTables/"+ projectile + "_" + target+"_" + option;
    if(option == "gas"){
      filename += "_" + pressure;
    }
    filename += ".in";
    
    Init(filename.c_str(),EMax,steps);
  }
}


void RN_EnergyLoss::Init(const char* LossFileName, double EMax, Int_t steps)
{
  fNPSteps = steps;

  // Open the SRIM file
 ifstream lossfile;
 lossfile.open(LossFileName);
 string dashline;
 std::vector<std::string>input;
 bool notfound;
 

 if (!lossfile.is_open()) 
   {
     cout << "Cannot open energy loss file " << LossFileName << endl; 
     return;
   }

 std::cout<<"Energy Loss File loading: "<<LossFileName<<"\n";


 do{
   input.clear();
   sak::ReadLine(lossfile,input);
   if(input.size()!=0){
     notfound = input[0].find("--------") == std::string::npos;
   }
   else {notfound=1;}

 }while(notfound);
 
 int i;
 string units;
 units="keV";
 i=0;

 fEnergy[0]  = 0;
 fdEdXE[0]   = 0;
 fdEdXN[0]   = 0;
 
 ////////////////////////////////
 // Read in the relevant numbers
 // from the SRIM file.
 do{
   if (fEnergy[i++]>EMax){
     break;
   }
   input.clear();
   sak::ReadLine(lossfile,input,10,sak::kNODEBUG);
   if(input.size()!=10){
     //     std::cout<<"Finished loading EnergyLoss Table,  You can check that the EnergyLoss Table was loaded properly by executing Print()\n\n";
     break;
   }
   fEnergy[i] = sak::string_to_double(input[0]);
   units = input[1];
   fdEdXE[i] = sak::string_to_double(input[2]);
   fdEdXN[i] = sak::string_to_double(input[3]);
   if (units=="keV") fEnergy[i] = 0.001*fEnergy[i];
   notfound = input[0].find("-----------------") == std::string::npos;
 }while(notfound || !lossfile.eof());
 fNpoints = i-1;
}


//////////////////////////////////////////////////
// Calculate Energy loss using the linear method:
double RN_EnergyLoss::GetLossLinear(double Path, double FinalEnergy, double density)
{
  int k;
  double IntEnergy,DE;
  IntEnergy = FinalEnergy;
  
  for (int i=0;i<fNPSteps;i++)  
    {
      k = 0;	
      
      //k is only incremented when IntEnergy>fEnergy[k] fails
      while (IntEnergy>fEnergy[k++] && k<fNpoints);
      k--;
      DE = ((fdEdXN[k]+fdEdXE[k])-(fdEdXN[k-1]+fdEdXE[k-1]))*(IntEnergy-fEnergy[k-1])/
	(fEnergy[k]-fEnergy[k-1]) + fdEdXN[k-1]+fdEdXE[k-1];
      
      DE = DE*Path/double(fNPSteps)*density;
      
      IntEnergy += DE; 
    }
  
  return IntEnergy - FinalEnergy;
}

//////////////////////////////////////////////////
// Calculate Energy loss using the linear method:
double RN_EnergyLoss::GetLossLinear(double Path, double InitialEnergy, double density, double&FinalEnergy)
{
  int k;
  double DE;
  FinalEnergy = InitialEnergy;
  
  for (int i=0;i<fNPSteps;i++)  
    {
      k = 0;	
      
      
      while (FinalEnergy>fEnergy[k++] && k<fNpoints);
      k--;
      DE = ((fdEdXN[k]+fdEdXE[k])-(fdEdXN[k-1]+fdEdXE[k-1]))*(FinalEnergy-fEnergy[k-1])/
	(fEnergy[k]-fEnergy[k-1]) + fdEdXN[k-1]+fdEdXE[k-1];
      
      DE = DE*Path/float(fNPSteps)*density;
      
      FinalEnergy -= DE; 
    }
  
  return InitialEnergy - FinalEnergy;
}

void RN_EnergyLoss::Print(){
  std::cout<<"Energy Loss Table for "<<GetName()<<std::endl;
  std::cout<<"Energy/dEdXE/dEdXN\n";
  for(int i=0;i<fNpoints;i++){
    std::cout<<fEnergy[i]<<" "<<fdEdXE[i]<<" "<<fdEdXN[i]<<"\n";
  }

}




#endif
