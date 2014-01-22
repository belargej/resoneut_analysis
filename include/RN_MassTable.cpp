#ifndef RN_MASSTABLE_CXX
#define RN_MASSTABLE_CXX

#include "RN_MassTable.hpp"


RN_MassTable::RN_MassTable(){

  //  AddParam("proton",938.2720137); 
  //AddParam("p",938.2720137);      
  AddParam("proton",938.7830137); //+e
  AddParam("p",938.7830137); //+e      
  AddParam("neutron",939.5653451);
  AddParam("n",939.5653451);
  //AddParam("d",1875.61277858);	      
  //AddParam("deuterium",1875.61277858);
  AddParam("d",1876.12);//+e	      
  AddParam("deuterium",1876.12);//+e
  AddParam("12C",11174.862342);
  AddParam("13N",12111.190852);
  //AddParam("24Mg",22335.79);
  AddParam("24Mg",22341.93068);//+e
  //AddParam("25Al",23271.793);
  AddParam("25Al",23278.43487);//+e
  //AddParam("26Si",24204.44054);
  AddParam("26Si",24211.70053);//+e
  AddParam("16O",14895.03101);
  AddParam("17F",15832.751186);
  AddParam("18Ne",16767.09968);


}

#endif
