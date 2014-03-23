#ifndef __RNVARIABLEMAP_CXX
#define __RNVARIABLEMAP_CXX


#include "RN_VariableMap.hpp"
#include "sak_ReadBuffer.hpp"
void RN_VariableMap::LoadParams(std::string filename){

  std::ifstream cal;
  std::string key;double value;
  cal.open(filename.c_str());
  if (!cal.is_open()){
    std::cout << "  Could not open " << filename << std::endl;
    return;
  }
  do{
    std::vector<std::string>input;
    sak::ReadLine(cal,input,2);
    if (input.size()!=2){
      std::cout<<"Variable map file has line with diff than 2 entries"<<std::endl;
      return;
    }
    key=input[0];
    value=sak::string_to_double(input[1]);

    vmap.insert(std::pair<std::string,double>(key,value));
  }while(!cal.eof());

}

int RN_VariableMap::GetParam(std::string param,float& var){

  it=vmap.begin();
  it=vmap.find(param);
  if (it!=vmap.end()){
    var=(float)it->second;
    return 1;
  }
  else return 0;
  
}

int RN_VariableMap::GetParam(std::string param,int& var){

  it=vmap.begin();
  it=vmap.find(param);
  if (it!=vmap.end()){
    var=(int)it->second;
    return 1;
  }
  else return 0;
  
}


int RN_VariableMap::GetParam(std::string param,double& var){

  it=vmap.begin();
  it=vmap.find(param);
  if (it!=vmap.end()){
    var=it->second;
    return 1;
  }
  else return 0;
  
}



int RN_VariableMap::AddParam(std::string param,double var){
  vmap.insert(std::pair<std::string,double>(param,var));
  return 1;
}

void RN_VariableMap::ClearParams(){
  vmap.clear();
}


#endif
