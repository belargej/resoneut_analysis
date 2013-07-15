{
  char * include=gSystem->ExpandPathName("${RNROOT}/include");
  gInterpreter->AddIncludePath(include);
  delete [] include;

  //Main RN Library
  gSystem->Load("libRNeut.so");
  
  //PLUGINS
  gSystem->Load("libSAK.so");
  //  gSystem->Load("RNHistograms_hpp.so");
  // gSystem->Load("RNGates_hpp.so");
  //gSystem->Load("RNDetHistograms_hpp.so");

  gStyle->SetPalette(1);
  cout<<"Hello "<<gSystem->Getenv("USER")<<"!"<<endl;
  gSystem->Exec("date");


}
