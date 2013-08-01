{

  char * include=gSystem->ExpandPathName("${RNROOT}/include");
  gInterpreter->AddIncludePath(include);
  delete [] include;
  
  gSystem->Load("../lib/libRNeut.so");
  gInterpreter->ProcessLine(".L RNGates.hpp+");
  gInterpreter->ProcessLine(".L RNHistograms.hpp+");
  




}
