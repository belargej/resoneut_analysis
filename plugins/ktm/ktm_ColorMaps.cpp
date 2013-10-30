namespace ktm{
  const static int Number(6);

  void BlackBody(){
    //   UInt_t Number = 6;
    
    Double_t Red[Number]   = { 1.00, 1.00, 1.00, 1.00, .5416666666, 0.00001};
    Double_t Green[Number] = { 1.00,    1.00,   0.6041666666, 0.07291666666, 0.00,   0.00};
    Double_t Blue[Number]  = { 1.00,    0.203125,  0.00,    0.00,      0.00,   0.00};
    
    Double_t Length[Number] = { 0, 0.2, 0.4, 0.6, 0.8, 1.0};
    Int_t nb=200;
    TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,510);
  }
  
  void BlueGreen()
  {
    // UInt_t Number = 6;
    
    Double_t Red[Number] = {.94118, .4392,   .1961, .0667,  0.22, 0.00};
    Double_t Green[Number] = {.94901, .8118,   .6235, .0510,   0.01,   0.00};
    Double_t Blue[Number]  = {.79607, .4902,   .6431, .4314, 0.27, 0.00};
		
    //Double_t Length[Number] = {0.0, .166667, .333333, .5, .66667, .833333, 1.00};
    //Double_t Length[Number] = {0.0, 0.05, .2, .35, .4, .7, 1.0};
    //Double_t Length[Number] = {0.000, .125, .250, .375, .5, .625, .75, .875, 1.00};
    Double_t Length[Number] =  {0, .25, .5, .75, .9, 1.00};
    Int_t nb=200;
    TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
    
  }
}
