#include "sak_ReadBuffer.hpp"

namespace sak{
  
  const char* ReadLine(std::ifstream& a,std::vector<std::string>& b,const int& ent = 0){
    std::string c;
    std::getline(a,c);
    std::istringstream d(c);
    std::copy(std::istream_iterator<std::string>(d),
	      std::istream_iterator<std::string>(),
	      std::back_inserter<std::vector<std::string> >(b));
    if(ent!=0 && b.size()!=ent){
      std::cout<<"invalid number of entries at pos: "<<a.tellg()<<std::endl;
      b.clear();
      return "";//act accordingly
    }
    return (c.c_str());

  }
 
 
  double string_to_double( const std::string& s )
  {
    std::istringstream i(s);
    double x;
    if (!(i >> x))
      return 0;
    return x;
  } 

  int string_to_int( const std::string& s )
  {
    std::istringstream i(s);
    int x;
    if (!(i >> x))
      return 0;
    return x;
  } 



}
