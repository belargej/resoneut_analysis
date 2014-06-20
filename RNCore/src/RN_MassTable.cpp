#ifndef RN_MASSTABLE_CXX
#define RN_MASSTABLE_CXX

#include "RN_MassTable.hpp"
#include <cstdlib>
#include "sak_ReadBuffer.hpp"


RN_MassTable::RN_MassTable(const std::string &name):RN_VariableMap(name){
  /*
  //AddParam("proton",938.2720137); 
  //AddParam("p",938.2720137);      
  AddParam("proton",938.7830137); //+e
  AddParam("p",938.7830137); //+e      

  AddParam("neutron",939.5653451);
  AddParam("n",939.5653451);

  //AddParam("d",1875.61277858);	      
  //AddParam("deuterium",1875.61277858);
  AddParam("d",1876.12);//+e	      
  AddParam("deuterium",1876.12);//+e

  AddParam("9B",8395.8626);//+e
  AddParam("10B",9326.9607);//+e

  //AddParam("12C",11174.862342);
  AddParam("12C",11177.928342); //+e

  //AddParam("13N",12111.190852);
  AddParam("13N",12114.767852); //+e

  //AddParam("24Mg",22335.79);
  AddParam("24Mg",22341.93068);//+e

  //AddParam("25Al",23271.793);
  AddParam("25Al",23278.43487);//+e

  //AddParam("26Si",24204.44054);
  AddParam("26Si",24211.70053);//+e
  
  AddParam("14O",13048.92336);//+e
  //AddParam("16O",14895.03101);
  AddParam("16O",14899.16699);//+e
 
  //  AddParam("17F",15832.751186);
  AddParam("17F",15837.35018);//+e
  
//AddParam("18Ne",16767.09968);
  AddParam("18Ne",16772.20968);//+e

  AddParam("a",3727.37924);
  AddParam("alpha",3727.37924);
  AddParam("4He",3727.37924);
  */
  std::string key;double value;
  std::ifstream masses;
  char* pPath;
  pPath = getenv ("RN_ANALYSIS");
  if (pPath!=NULL){
    printf ("The current path is: %s\n",pPath);
    masses.open(Form("%s/input/masses.in",pPath));
    if (!masses.is_open()){
      std::cout << "masses.in file missing from input folder" << std::endl;
      return;
    }
    else{
      std::cout<<"Loading masses from"<<Form("%s/input/masses.in\n",pPath);
    }
  }
  else{
    std::cout<<"environment variable RN_ANALYSIS not set\n";
    std::cout<<"mass table not set"<<std::endl;
  }
  do{
    std::vector<std::string>input;
    sak::ReadLine(masses,input,5);
    if (input.size()!=5){
      std::cout<<"masses file has line with diff than 5 entries"<<std::endl;
      continue;
    }
    key=input[2];
    value=sak::string_to_double(input[4]);
    AddParam(key,value);
  }while(!masses.eof());
}




#endif
