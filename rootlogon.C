{
  gSystem->Load("libRNCore.so");
  gSystem->Load("libSAK.so");
  gSystem->Load("libRNanalyzers.so");
  gSystem->Load("libRNfilters.so");
  gSystem->Load("libRNfitting.so");

 

  gStyle->SetPalette(1);
  cout<<"Hello "<<gSystem->Getenv("USER")<<"!"<<endl;
  gSystem->Exec("date");


}
