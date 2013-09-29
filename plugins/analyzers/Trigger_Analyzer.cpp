#include "Trigger_Analyzer.hpp"
#include "../../include/RN_Root.hpp"
#include "../../include/RN_Unpack2Root.hpp"

int Q1T1[16];
int Q1T0[16];
int T1Q1[16];
int T1Q0[16];
int Q1T1a(0);
int Q1T0a(0);
int T1Q1a(0);
int T1Q0a(0);

using namespace unpacker;

Trigger_Analyzer::Trigger_Analyzer()
{

}

void Trigger_Analyzer::Begin(){
  for (int i=0;i<16;i++){
    Q1T1[i]=0;
    Q1T0[i]=0;
    T1Q1[i]=0;
    T1Q0[i]=0;
  }
  Q1T1a=0;
  Q1T1a=0;
  T1Q1a=0;
  T1Q0a=0;

}


void Trigger_Analyzer::Process(){
  for (int i=1;i<16;i++){
    
    if(QDC1[i]){
      if(TDC1[i+16]){
	Q1T1[i]++;
      }
      else{
	Q1T0[i]++;
      }
    }

    if(TDC1[i+16]){
      if(QDC1[i]){
	T1Q1[i]++;
      }
      else{
	T1Q0[i]++;
      }
    }    

    
  }
 




}

  

void Trigger_Analyzer::Terminate(){
  std::cout<<"Total Entries: "<<TotEntries()<<std::endl;
  for(int i=0;i<16;i++){

    std::cout<<"Q1T1["<<i<<"]"<<Q1T1[i]<<std::endl;
    std::cout<<"Q1T0["<<i<<"]"<<Q1T0[i]<<std::endl;
    std::cout<<"T1Q1["<<i<<"]"<<T1Q1[i]<<std::endl;
    std::cout<<"T1Q0["<<i<<"]"<<T1Q0[i]<<std::endl;
    
  
  }
  
  std::cout<<"Q1T1a"<<Q1T1a<<std::endl;
  std::cout<<"Q1T0a"<<Q1T0a<<std::endl;
  std::cout<<"T1Q1a"<<T1Q1a<<std::endl;
  std::cout<<"T1Q0a"<<T1Q0a<<std::endl;
  
  
}
