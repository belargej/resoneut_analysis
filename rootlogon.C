{
  gSystem->Load("libRNeut.so");
  gSystem->Load("libSAK.so");
  gSystem->Load("libRNanalyzers.so");
  gSystem->Load("libRNsimulator.so");
 

  gStyle->SetPalette(1);
  cout<<"Hello "<<gSystem->Getenv("USER")<<"!"<<endl;
  gSystem->Exec("date");


}
