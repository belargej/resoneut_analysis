
// C++ includes
#include <iostream>
#include <fstream>

// C includes
#include <unistd.h>

// ROOT includes
#include <TH1.h>
#include <TH2F.h>
#include <TCutG.h>
#include <TDirectory.h>
#include <TRint.h>
#include <TBrowser.h>
#include <TThread.h>
#include <TStyle.h>
#include <math.h>
#include <ChannelMap.h>

// project includes
#include "configfile.h"
#include "SimpleInPipe.h"
#include "ExtraHistos.h"

// making STD available
using namespace std;


// global variables
//ExtraHistos* EH;
float CalParamF[128][3];
float CalParamB[128][3];

  TH1I* hE1[9][32];
  TH1I* hT1[9][32];

  TH1I* hE2[9][32];
  TH1I* hT2[9][32];

  TH1I* hCaenADC[5][32];
  TH1I* hCaenTDC[5][32];

  TH1I* HitPattern_MB1;
  TH1I* HitPattern_MB2;
  TH2I* ChanEn_MB1;
  TH2I* ChanEn_MB2;
  TH2I* ChanT_MB1;
  TH2I* ChanT_MB2;
  TH2I* IC;
  TH2I *RFTimeSiPM_VsSi16;

  TH1I* RFTime;
  TH1I* PulserTime;
  TH1I* ION0time;
  TH1I* ION1time;
  TH1I* ION2time;
  TH1I* ION0;
  TH1I* ION1;
  TH1I* ION2;
  TH2I* ION0vsION1;
  TH2I* ION0vsION012;
  TH2I* ION01vsION012;
  TH2I* ION01vsION2;
  TH2I* IONvsSiIon;
  TH2F* EdEcal;
  TH1F* IONcalsum;

  TH2F* SiIon_Pos;
  TH1F* SiIon_Energy;

  TH1I* SiIonSiPMTime[16];
  TH1I* SiIonRFTime[16];
  TH1I* RFvsSiPMTime;
  TH2I* SiIonEvsSiPMT[32];
  TH2I* SiIonEvsRF[32];

  TH1I* SiIonE_withSiPM[32];
  TH1I* Si16En_withSiPM_All;
  TH2I* Si16En_vsSiPM_All;
  
  TH1I* Si16En_withMCP_All;
  TH2I* Si16En_vsMCP_All;

  ChannelMap *CMAP;

//  TH2I* TvsE[16];
//  TH2I* E32vsICE;
      
//int DefaultBins;
//vector<double> DefaultRange;

/***************************************************************
 * a little construct to have an array initialized with zeroes *
 ***************************************************************/
template<class T,int n>
class ZeroedArray {
  T histo[n];
public:
  ZeroedArray()
  {
    for(int i = 0; i < n; i++)
      histo[i] = 0;
  }
  T& operator[](int idx)
    { return histo[idx]; }
};



/****************************************
 * function for the reading data thread *
 ****************************************/
void* ReadBuffers(void* intobj)
{
  int const BufferWords = 13328;
  int const BufferBytes = BufferWords*2;
   unsigned short buffer[13328];
   unsigned short *point,*epoint;
  int unsigned words;
  int unsigned type;
  int unsigned Nevents;
  int unsigned TotEvents=0;
  int unsigned Nbuffers=0;
  int unsigned counter =0;
  int BufferPhysics = 0;
  int BufferScaler = 0;
  int runNum;
  string target;
  // define buffer
  int unsigned buflen = 26656;
  //char buffer[buflen];

  // open pipe to read data
  char* pipecommand[] = { "spectcldaq","tcp://128.186.111.135:2602", 0 };
  SimpleInPipe evtpipe(pipecommand);

  // loop until we get killed
  for(int run;;)
    if(evtpipe.read((char*)buffer,buflen)) // read a buffer from the pipe
    {
      point = buffer;
            point = buffer;
	    //cout << "buffer = " << buffer << endl; //debug

	    Nbuffers++;
            if (Nbuffers%30000 == 0) cout << Nbuffers << endl;
            epoint = point;
            words = *epoint++;
            type = *epoint++;

            if (type == 11) // begin run buffer
	      {
                runNum = *(epoint+1);
                cout << "run number="<< runNum << endl;	
              }
            else if (type == 12) //end run buffer
	      {
               // cout << "end of run buffer"<< endl;
		// endOfRun = 1;
     // ?????? Milan           // break;
	      }
	    else if (type == 2)  // scaler buffer
	      {
                BufferScaler++;
              }
	    else if (type == 1)
	      {
		// cout << " physics buffer" << endl;
		BufferPhysics++;
		Nevents = *(epoint+4);
		TotEvents += Nevents;
		epoint = epoint + 14;
		//cout << "Nevents = "<<Nevents<<endl;
		for (int ievent=0;ievent<Nevents;ievent++)
		  {
		    //create pointer inside of each  event
		    unsigned short * fpoint = epoint;
		    
		    words = *fpoint++;
                    int XLMdata1 = *fpoint++;
		    
		    if (XLMdata1==0xaaaa) {

		      fpoint +=2 ;
		      unsigned short Nstrips = *fpoint;

// 			cout << Nstrips << endl ;

		      fpoint+=5;

		      Int_t ESeg = 0;

		      for (int istrip=0;istrip<Nstrips;istrip++)
			{
			  unsigned short *gpoint = fpoint;
			  unsigned short id = *gpoint;
			  unsigned short chipNum = (id&0x1FE0)>>5;
			  unsigned short chanNum = id& 0x1F;
			  gpoint++;
			  unsigned short energy = *gpoint;
			  gpoint++;
			  unsigned short time = *gpoint;
			  time = 16384 - time;
			  if(chanNum<16){ //--ddc add back test for chanNum in range
			    if (chipNum == 3 || chipNum == 4 ) energy = 16384 - energy;
			    if (chipNum == 9 || chipNum == 10 ) energy = 16384 - energy;
			    if (chipNum == 13 || chipNum == 14) energy = 16384 - energy;
			    if (chipNum == 15 || chipNum == 16) energy = 16384 - energy;
			    if (chipNum == 17 || chipNum == 18) energy = 16384 - energy;

			    HitPattern_MB1->Fill(chipNum*16-16+chanNum);
			    ChanEn_MB1->Fill(chipNum*16-16+chanNum,energy);
			    ChanT_MB1->Fill(chipNum*16-16+chanNum,time);
                            
                            if (chipNum == 0) 
				{
				   //cout ....;
                                }	

			    else if (chipNum < 19 ){
 
			    if(chipNum % 2 == 0){ //even chip means second chip on board
			      hE1[chipNum/2-1][chanNum+16]->Fill(energy);
			      hT1[chipNum/2-1][chanNum+16]->Fill(time);            
			    }
			    else {     //odd chip means first chip on board
			      hE1[(chipNum-1)/2][chanNum]->Fill(energy);
			      hT1[(chipNum-1)/2][chanNum]->Fill(time);
			    }
 
			  }
                          else
                          {
                              //cout ... ;
                          } 
			}
		      
		      fpoint +=3;
                      }
		    }	

		    // Second XLM readout if present
		     	
                    int XLMdata2 = *fpoint++;
		    int counter;

		    counter = 0;
		    while (XLMdata2 != 0xbbbb) {
			XLMdata2 = *fpoint++;
                        counter++;
		        if (counter>10) break;
                    }

		    if (XLMdata2==0xbbbb) {

//                      words = *fpoint++;
                      
		      fpoint += 2;
		      unsigned short Nstrips = *fpoint;
//                      cout << Nstrips <<endl;
 
		      fpoint+=5;

		      Int_t ESeg = 0;

		      for (int istrip=0;istrip<Nstrips;istrip++)
			{
			  unsigned short *gpoint = fpoint;
			  unsigned short id = *gpoint;
			  unsigned short chipNum = (id&0x1FE0)>>5;
			  unsigned short chanNum = id& 0x1F;
			  gpoint++;
			  unsigned short energy = *gpoint;
			  gpoint++;
			  unsigned short time = *gpoint;
			  time = 16384 - time;
			  if(chanNum<16){ //--ddc add back test for chanNum in range
      //                      if (chipNum == 1 || chipNum == 2 ) energy = 16384 - energy;
      //      			    if (chipNum == 3 || chipNum == 4 ) energy = 16384 - energy;
      			    if (chipNum == 5 || chipNum == 6 ) energy = 16384 - energy;
      		            if (chipNum == 7 || chipNum == 8 ) energy = 16384 - energy;
      //			    if (chipNum == 15 || chipNum == 16) energy = 16384 - energy;
      //			    if (chipNum == 17 || chipNum == 18) energy = 16384 - energy;

			    HitPattern_MB2->Fill(chipNum*16-16+chanNum);
			    ChanEn_MB2->Fill(chipNum*16-16+chanNum,energy);
			    ChanT_MB2->Fill(chipNum*16-16+chanNum,time);
                            
                            if (chipNum == 0) 
				{
				   //cout ....;
                                }	

			    else if (chipNum < 19 ){
 
			    if(chipNum % 2 == 0){ //even chip means second chip on board
			      hE2[chipNum/2-1][chanNum+16]->Fill(energy);
			      hT2[chipNum/2-1][chanNum+16]->Fill(time);            
			    }
			    else {     //odd chip means first chip on board
			      hE2[(chipNum-1)/2][chanNum]->Fill(energy);
			      hT2[(chipNum-1)/2][chanNum]->Fill(time);
			    }
 
			  }
                          else
                          {
                              //cout ... ;
                          } 
			}
		      
		      fpoint +=3;
                      }
		    }

		    int CAEN = *fpoint++;
	
		    counter = 0;
		    while (CAEN != 0xcccc) {
			CAEN = *fpoint++;
                        counter++;
		        if (counter>10) break;
                    }

		    // CAEN TDC / ADC readout

			unsigned short SiPMTime = 0; 
			unsigned short RFTime = 0;
                        unsigned short MCPTime = 0;
                        unsigned short data[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		    while (fpoint < epoint + words){

		      if(*fpoint == 0xffff ){
			fpoint++;
			continue;
		      }
		  
		      unsigned short *gpoint = fpoint;
		      unsigned short chanCount = (*(gpoint++) & 0xff00)>>8;
		      unsigned short GEOaddress = (*(gpoint++) & 0xf800)>>11;
                      float gain[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0.98,1.008,0.981,0.98,1.008,0.991,0.993,0.985,0.9965,0.979,1.0075,1.002,1.004,0.985};

		      int i;
		      float SiIon_x = -1.;
                      float SiIon_y = -1.;
                      float SiIon_E = 0.0;



		      for (i=0;i<chanCount;i++){
			if(i>31) continue;
			unsigned short dat = (*(gpoint++)&0xfff);
			unsigned short chn = (*(gpoint++)&0x1f);
			if (GEOaddress==3 && chn<32) hCaenADC[0][chn]->Fill(dat);
			if (GEOaddress==4 && chn<32) hCaenADC[1][chn]->Fill(dat);
			if (GEOaddress==5 && chn<32) hCaenADC[2][chn]->Fill(dat);
			if (GEOaddress==6 && chn<32) hCaenADC[3][chn]->Fill(dat);
                        if (GEOaddress==7 && chn<32) hCaenADC[4][chn]->Fill(dat);
			if (GEOaddress==11 && chn<32) hCaenTDC[0][chn]->Fill(dat);
			if (GEOaddress==12 && chn<32) hCaenTDC[1][chn]->Fill(dat);

			//Now we get time from Si-PM and RF timing for separate time spectra of different channels in the Si-Ion Chamber

                 	if (GEOaddress==11 && chn==0) SiPMTime = dat;
			if (GEOaddress==11 && chn==1) {
				RFTime = dat;
				RFvsSiPMTime->Fill(RFTime-SiPMTime);
				
			}
   
                        if (GEOaddress==11 && chn==16) MCPTime=dat;

			if (GEOaddress==12 && chn>-1 && chn<16 && SiPMTime>0) SiIonSiPMTime[chn]->Fill(SiPMTime-dat);
			if (GEOaddress==12 && chn>-1 && chn<16 && RFTime>0) SiIonRFTime[chn]->Fill(RFTime-dat+2000);
              
	


                        if (GEOaddress == 6 && chn>-1 && chn<8 && dat>250 ) SiIon_y = (7.0-float(chn)+float(rand())/float(RAND_MAX))*3.1;
//                        if (GEOaddress == 6 && chn>-1 && chn<8) SiIon_y = (7-float(chn))*3.1+24.8;
                        if (GEOaddress == 6 && chn>7 && chn<16 && dat>250) SiIon_y = (float(chn)+float(rand())/float(RAND_MAX))*3.1;
//                        if (GEOaddress == 6 && chn>7 && chn<16) SiIon_y = (float(chn))*3.1-24.8;
                        if (GEOaddress == 6 && chn>15 && chn<32 && dat>250) SiIon_x = (float(chn)+float(rand())/float(RAND_MAX))*3.1-49.6;
                        if (GEOaddress == 6 && chn>15 && chn<32 && dat>250) SiIon_E = float(dat)*gain[chn];  

                        if (GEOaddress == 6 && chn>-1 && chn<32) data[chn] = dat;
			
		      }

		      // Do custom analysis here:

                        if (data[0]>0 && data[1]>0) ION0vsION1->Fill(data[1],data[0]);
                        if (data[0]>0 && data[1]>0 && data[2]>0) ION01vsION2->Fill(data[2],data[1]+data[0]);
                        if (data[0]>0 && data[1]>0 && data[2]>0) ION0vsION012->Fill(data[2]+data[1]+data[0],data[0]);
                        if (data[0]>0 && data[1]>0 && data[2]>0) ION01vsION012->Fill(data[2]+data[1]+data[0],data[1]+data[0]);
                        if (data[0]>0 && data[1]>0 && data[2]>0 && SiIon_E>100) IONvsSiIon->Fill(SiIon_E,data[2]+data[1]+data[0]);

                      if (SiIon_x > 0 && SiIon_y > 0) SiIon_Pos->Fill(SiIon_x,SiIon_y);
//                      cout << SiIon_x << "  " << SiIon_y <<endl ;
//                      cout << rand() << "   "<< RAND_MAX << "  " << float(rand())/float(RAND_MAX) << endl;
                      if (SiIon_E > 100) SiIon_Energy->Fill(SiIon_E);

		      // End of custom analysis
		      

		      unsigned short trailer_l = *(gpoint++);
		      unsigned short trailer_h = *(gpoint++);
		      
		      while ((gpoint < epoint + words )&&(*gpoint==0xffff)){
			gpoint ++;
		      }
		      // go to next CAEN data
		      fpoint = gpoint;
		      
		    }
		    
		    epoint += words+1; // This skips the rest of the event
                    for (Int_t i=0;i<32;i++){ 
		       if (data[i]>0 && SiPMTime>0) SiIonEvsSiPMT[i]->Fill(data[i],SiPMTime);
                       if (data[i]>0 && SiPMTime>0) SiIonE_withSiPM[i]->Fill(data[i]);
                       if (data[i]>0 && RFTime>0) {
			 SiIonEvsRF[i]->Fill(data[i],RFTime);
			 if(SiPMTime>0) RFTimeSiPM_VsSi16->Fill(data[i],RFTime-SiPMTime+2000);
		       }
                    } 
		    //  eventNo++;
		    for(Int_t i = 0; i < 16; i++) {

		      if(data[i]>0&& SiPMTime>0) Si16En_vsSiPM_All->Fill(data[i],SiPMTime);
		      if(data[i]>0&& SiPMTime>0) Si16En_withSiPM_All->Fill(data[i]);
		      if(data[i]>0&& MCPTime>0) Si16En_vsMCP_All->Fill(data[i],MCPTime);
		      if(data[i]>0&& MCPTime>0) Si16En_withMCP_All->Fill(data[i]);

		    }

		    counter++;
		  }//end for over events
		
	      } //end if physics buffer
    } 
    else 
      sleep(1); // wait a moment before the next attempt to read data
}

/****************
 * main program *
 ****************/
int main(int argc, char* argv[])
{


  gStyle->SetPalette(1);
 //  cout << RAND_MAX << endl;

  //read calibration
/*
  ifstream fCalIn;
  fCalIn.open("CalibrationParameters.txt");

  for(int i=0;i<128;i++){
    for(int j=0;j<3;j++){
      fCalIn >> CalParamF[i][j];
      cout << CalParamF[i][j] << " ";
      }
    cout << endl;
  }
  
  for(int i=0;i<128;i++){
    for(int j=0;j<3;j++){
      fCalIn >> CalParamB[i][j];
      cout << CalParamB[i][j] << " ";
      }
    cout << endl;
  }
  


  fCalIn.close();
*/
  // read the histogram binning from a configuration file
  const char* conffile = "Read_NSCLDAQ_Online.cfg";
  SymbolTable ST = get_symbol_table(conffile,"main");
  //  DefaultBins = ST["DefaultBins"];
  //  copyvector(DefaultRange,ST["DefaultRange"]);
  
  // Create objects for root shell and data reading
  TRint intobj("intobj",&argc,argv);
  TThread ReadThread("ReadThread",ReadBuffers,(void*)&intobj);


  TDirectory* Hits = gDirectory->mkdir("Hits");
  TDirectory* MB1 = gDirectory->mkdir("MB1");
  TDirectory* MB2 = gDirectory->mkdir("MB2");
  TDirectory* CAEN = gDirectory->mkdir("CAEN");
  TDirectory* SiIonTiming = gDirectory->mkdir("SiIonTiming");

  MB1->cd();
  TDirectory* CB1[9];
  char name[20];
  for(int j=0;j<9;j++){
    sprintf(name,"ChipBoard_%d",j+1);
    CB1[j] = gDirectory->mkdir(name);        
  }     

  MB2->cd();
  TDirectory* CB2[9];
  for(int j=0;j<9;j++){
    sprintf(name,"ChipBoard_%d",j+1);
    CB2[j] = gDirectory->mkdir(name);        
  }     
   
  CAEN->cd(); 
  TDirectory* CaenE[5];
  char nameCaenE[20];
  for(int j=0;j<5;j++){
    sprintf(nameCaenE,"CaenADC_%d",j+1);
    CaenE[j] = gDirectory->mkdir(nameCaenE);        
  }
 
  TDirectory* CaenT[5];
  char nameCaenT[20];
  for(int j=0;j<5;j++){
    sprintf(nameCaenT,"CaenTDC_%d",j+1);
    CaenT[j] = gDirectory->mkdir(nameCaenT);        
  }
 
  TBrowser * mybrowser = new TBrowser("OnlineBrowser",Hits);

  mybrowser->Add(MB1);
  mybrowser->Add(MB2);
  mybrowser->Add(CAEN);
  mybrowser->Add(SiIonTiming);

  Hits->cd();

  HitPattern_MB1 = new TH1I("HitPattern_MB1","",288,0,288);
  HitPattern_MB2 = new TH1I("HitPattern_MB2","",288,0,288);

  ChanEn_MB1 = new TH2I("EnVsCh_MB1","",288,0,288,4096,0,16384);
  ChanEn_MB2 = new TH2I("EnVsCh_MB2","",288,0,288,4096,0,16384);

  ChanT_MB1 = new TH2I("TiVsCh_MB1","",288,0,288,4096,0,16384);
  ChanT_MB2 = new TH2I("TiVsCh_MB2","",288,0,288,4096,0,16384);

  IC = new TH2I("IC","",1024,0,16384,1024,0,16384);

  RFTime = new TH1I("RFTIME","",4096,0,4095);
  PulserTime = new TH1I("PULSRETIME","",4096,0,4095);

  ION0 = new TH1I("ION0","ION0",4096,0,4096);
  ION1 = new TH1I("ION1","ION1",4096,0,4096);
  ION2 = new TH1I("ION2","ION2",4096,0,4096);
  IONcalsum = new TH1F("IONcalsum","Summed energy",1024,0,100);

  ION0time = new TH1I("ION0time","ION0time",4096,0,4096);
  ION1time = new TH1I("ION1time","ION1time",4096,0,4096);
  ION2time = new TH1I("ION2time","ION2time",4096,0,4096);

  ION0vsION1 = new TH2I("ION0vsION1","ION0vsION1",512,0,4096,512,0,4096);
  ION0vsION012 = new TH2I("ION0vsION012","ION0vsION012",512,0,12288,512,0,4096);
  ION01vsION012 = new TH2I("ION01vsION012","ION01vsION012",512,0,12288,512,0,8182);
  ION01vsION2 = new TH2I("ION01vsION2","ION01vsION2",512,0,4096,512,0,8182);
  IONvsSiIon = new TH2I("IONvsSiIon","IONvsSiIon",512,0,4096,512,0,8182);

  EdEcal = new TH2F("EdEcal","Ion chamber E v. dE",1000,40,90,200,15,25);

  SiIon_Pos = new TH2F("SiIon_Pos","SiIon Position",50,0,50,50,0,50);
  SiIon_Energy = new TH1F("SiIon_Energy","SiIon Energy",4096.,0,4096.);

  

  SiIonTiming->cd();
	for (Int_t j=0;j<16;j++) {
		SiIonSiPMTime[j] = new TH1I(Form("SiPMTime%02d",j),"",4096, 0, 4096);
		SiIonRFTime[j] = new TH1I(Form("RFTime%02d",j),"",4096, 0, 4096 );
}
	RFvsSiPMTime = new TH1I("RF_SiPMTime", "", 4096, 0, 4096);


  for (Int_t j=0;j<32;j++) {
     SiIonEvsSiPMT[j]= new TH2I(Form("SiIonEvsSiPMT%0d",j),"",512,0,4096,512,0,4096);
     SiIonE_withSiPM[j] = new TH1I(Form("SiIonE_withSiPM%0d",j),"",512,0,4096);
     SiIonEvsRF[j] = new TH2I(Form("SiIonEvsRF%0d",j),"",512,0,4096,512,0,4096);
  }
  
  Si16En_withSiPM_All = new TH1I("Si16En_withSiPM_ALL","",512,0,4096);
  Si16En_vsSiPM_All = new TH2I("Si16EN_vsSiPM_ALL","",512,0,4096,512,0,4096);
  Si16En_withMCP_All = new TH1I("Si16En_withMCP_All","",512,0,4096);
  Si16En_vsMCP_All = new TH2I("Si16EN_vsMCP_All","",512,0,4096,512,0,4096);
  RFTimeSiPM_VsSi16 = new TH2I("RFTimeSiPM_VsSi16","",512,0,4096,512,0,4096);
  
//  E32vsICE = new TH2I("ICEvsSegE32","",512,0,16384,512,0,16384);


  for(int j=0;j<9;j++){
    CB1[j]->cd();
    for(int i=0;i<32;i++){
      hE1[j][i] = new TH1I(Form("Energy_Board%d_strip%02d",j+1,i),"",16384,0,16384);
      hT1[j][i] = new TH1I(Form("Time_Board%d_strip%02d",j+1,i),"",16384,0,16384);
    }
  }
  
  for(int j=0;j<9;j++){
    CB2[j]->cd();
    for(int i=0;i<32;i++){
      hE2[j][i] = new TH1I(Form("Energy_Board%d_strip%02d",j+1,i),"",16384,0,16384);
      hT2[j][i] = new TH1I(Form("Time_Board%d_strip%02d",j+1,i),"",16384,0,16384);
    }
  }
  
  for(int j=0;j<5;j++){
     CaenE[j]->cd();
     for(int i=0;i<32;i++){
       hCaenADC[j][i] = new TH1I(Form("ADC%d_ch%02d",j+1,i),"",4096,0,4096);
     }
     CaenT[j]->cd();	
     for(int i=0;i<32;i++){
       hCaenTDC[j][i] = new TH1I(Form("TDC%d_ch%02d",j+1,i),"",4096,0,4096);
     }
   }

   CMAP = new ChannelMap();
   
   //Initialization of the main channel map
   CMAP->Init("channelmap.042612","alignchannels.063012","AlphaCalibration_063012.txt","X3Slope063012-refchan0.out");
   CMAP->FinalInit("FinalFix.063012","X3geometry.061812");
   CMAP->InitWorldCoordinates("WorldCoordinates.061412_ver3");
   
   CMAP->InitPCADC("PropCounterMap.061512");
   CMAP->InitPCCalibration("PCpulser_071312.dat");
   CMAP->InitPCWireCal("PCWireCal.062712");

  // start reading data
  ReadThread.Run();

  // start root shell
  intobj.Run(kTRUE);
  return 0;
}


