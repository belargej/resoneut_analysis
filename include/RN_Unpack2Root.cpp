////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013                             
////////////////////////////////////////////////////////////////////////
#ifndef __RNUNPACKER_CXX
#define __RNUNPACKER_CXX


#include "RN_Unpack2Root.hpp"

using namespace std;

namespace unpacker{
  Int_t Event[3];//stores RunNum/flag/scaler
  ScalerNames scaler_names;
  ScalerValues scaler_values;

  float ADC1[32];
  float ADC2[32];
  float ADC3[32];
  float ADC4[32];
  float ADC5[32];
  float ADC6[32];
  float ADC7[32];
  float TDC1[32];
  float TDC2[32];
  float TDC3[32];
  float TDC4[32];
  float QDC1[32];
  float QDC2[32];
  
  TBranch        *b_ADC1;   //!
  TBranch        *b_ADC2;   //!
  TBranch        *b_ADC3;   //!
  TBranch        *b_ADC4;   //!
  TBranch        *b_ADC5;   //!
  TBranch        *b_ADC6;   //!
  TBranch        *b_ADC7;   //!
  TBranch        *b_TDC1;   //!
  TBranch        *b_TDC2;   //!
  TBranch        *b_TDC3;   //!
  TBranch        *b_TDC4;   //!
  TBranch        *b_QDC1;   //!
  TBranch        *b_QDC2;   //!
 

  std::vector<short> caen_stack;
  std::vector<short> mesy_stack;

  int GetMesyNum(){return mesy_stack.size();}
  int GetCaenNum(){return caen_stack.size();}
}

using namespace unpacker;

RNUnpack2Root::RNUnpack2Root():Rnd(0)
{
  Event[0]=0;
  Event[1]=0;
  //Event[2]=0;
  for(int i=0;i<32;i++){
    ADC1[i]=0;
    ADC2[i]=0;
    ADC3[i]=0;
    ADC4[i]=0;
    ADC5[i]=0;
    ADC6[i]=0;
    ADC7[i]=0;
    TDC1[i]=0;
    TDC2[i]=0;
    TDC3[i]=0;
    TDC4[i]=0;
    QDC1[i]=0;
    QDC2[i]=0;

  }

}
 
bool RNUnpack2Root::init(const std::string & configfile){
  std::ifstream cfg;
  cfg.open(configfile.c_str());
  if (!cfg.is_open()){
    std::cout << "  Could not open " << configfile << std::endl;
    return 0;
  }
  do{
    std::vector<std::string>input;
    sak::ReadLine(cfg,input);
    if(input.size()>0){
      if(input[0]=="caen_stack")
	for(unsigned int i=1;i<input.size();i++){
	  caen_stack.push_back(sak::string_to_int(input[i]));
	}
      else if(input[0]=="mesy_stack")
	for(unsigned int i=1;i<input.size();i++){
	  mesy_stack.push_back(sak::string_to_int(input[i]));
	}
      else if(input[0]=="scaler_list"){
	for(unsigned int i=1;i<input.size();i++){
	  scaler_names.push_back(input[i]);
	  scaler_values.push_back(0);
	}
      }
    }
    
  }while(!cfg.eof());
  

  return 1;
}

bool RNUnpack2Root::init(){

  short caen_geo[]={3,4,5,6,7,8,10,11,12,13};
  short mesy_geo[]={14,16};
  
  caen_stack.insert(caen_stack.begin(),caen_geo,caen_geo+10);
  mesy_stack.insert(mesy_stack.begin(),mesy_geo,mesy_geo+2);  
  
  return 1;
} 

void RNUnpack2Root::Reset(){
  Event[1]=0;
  for(int i=0;i<32;i++){
    ADC1[i]=0;
    ADC2[i]=0;
    ADC3[i]=0;
    ADC4[i]=0;
    ADC5[i]=0;
    ADC6[i]=0;
    ADC7[i]=0;
    TDC1[i]=0;
    TDC2[i]=0;
    TDC3[i]=0;
    TDC4[i]=0;
    QDC1[i]=0;
    QDC2[i]=0;
  }
}


int RNUnpack2Root::SortGeoChan(short geoaddress,short chan, short val){
  
  if(geoaddress==2&&chan<32)ADC1[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==3&&chan<32)ADC2[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==4&&chan<32)ADC3[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==5&&chan<32)ADC4[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==6&&chan<32)ADC5[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==7&&chan<32)ADC6[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==8&&chan<32)ADC7[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==10&&chan<32)TDC1[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==11&&chan<32)TDC2[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==12&&chan<32)TDC3[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==13&&chan<32)TDC4[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==14&&chan<32)QDC1[chan]=(float)val+Rnd.Rndm();
  else if(geoaddress==16&&chan<32)QDC2[chan]=(float)val+Rnd.Rndm();
  else return 0;
  
  return 1;


}

///////////////////////////////////////////////////////////////////////////////////
// File converter, from .evt to .root. Arguments:
// - files_list: used to specify the 
///////////////////////////////////////////////////////////////////////////////////
int RNUnpack2Root::Convert(std::vector<int>&run_number,std::string data_dir,std::string output_file){
  
  TFile* RootFile;
  TTree* DataTree;
  TTree* ScalerTree;
  unsigned int BufferWords; //number of short length words, each 2 bytes
  unsigned int BufferBytes; //number of bytes to be stored
  unsigned int BufferType;
  unsigned short OtherInfo;
  unsigned int ItemType;
  unsigned int NBuffers = 0; 
  unsigned short mem_counter=0,badcounter=0;
  int BufferPhysics = 0;
  ifstream evtfile;
  ofstream logfile;
  unsigned short * buffer;
  int timer=0; 

  //stacks initialized in init.  Necessary to read out the modules in the right order
  unsigned int caen_num=caen_stack.size();
  unsigned int mesy_num=mesy_stack.size();


  // ROOT output file
  RootFile = new TFile(output_file.c_str(),"RECREATE");  
  string logger = Form("%s.log",output_file.c_str());
  logfile.open(logger.c_str(),ios::out);
 

  // Data Tree
  DataTree = new TTree("DataTree","DataTree");
  ScalerTree = new TTree("ScalerTree","ScalerTree");
 
  //flag is used to notify the user of any issues seen during unpacking
  DataTree->Branch("Event",&Event,"RunNum/I:flag/I:ScalerIDX/I"); 
  DataTree->Branch("ADC1",&ADC1,"ADC1[32]/F");
  DataTree->Branch("ADC2",&ADC2,"ADC2[32]/F");
  DataTree->Branch("ADC3",&ADC3,"ADC3[32]/F");
  DataTree->Branch("ADC4",&ADC4,"ADC4[32]/F");
  DataTree->Branch("ADC5",&ADC5,"ADC5[32]/F");
  DataTree->Branch("ADC6",&ADC6,"ADC6[32]/F");
  DataTree->Branch("ADC7",&ADC7,"ADC7[32]/F");
  DataTree->Branch("TDC1",&TDC1,"TDC1[32]/F");
  DataTree->Branch("TDC2",&TDC2,"TDC2[32]/F");
  DataTree->Branch("TDC3",&TDC3,"TDC3[32]/F");
  DataTree->Branch("TDC4",&TDC4,"TDC4[32]/F");
  DataTree->Branch("QDC1",&QDC1,"QDC1[32]/F");
  DataTree->Branch("QDC2",&QDC2,"QDC2[32]/F");
  ScalerTree->Branch("Scaler",&scaler_values);




  //Loop over files in the data file list.
  for(unsigned int b=0;b<run_number.size();b++){
    //this section to properly format the evt number to buffer run number with zeroes.
    Event[0]=run_number[b];
    stringstream ss;
    ss<<run_number[b];
    string run_string;
    ss>>run_string;
    int str_length = run_string.length();
    for (int i = 0; i < 4 - str_length; i++){
      run_string = "0" + run_string;}

    string name = data_dir + "/run-" + run_string + "-00.evt";
 
    //Open evt file
   evtfile.open(name.c_str(),ios::binary);      
   if (!evtfile.is_open()){
     std::cout << "  Could not open " << name << std::endl;
     break;
   }
   else 
     std::cout << "  Converting " << name << " ..." << std::endl;

   
    BufferWords = 4;  //read just enough to get words and type
    buffer = new unsigned short[BufferWords]; //create buffer to store header from evt file
    BufferBytes = BufferWords*sizeof(short); //to count number of bytes to be read from evt file
    evtfile.read((char*)buffer,BufferBytes); //read evt file
    BufferBytes = buffer[0]; //size is first 16 in a 32 bit block::number of bytes in file header
    BufferType = buffer[2]; //type is first 16 in a 32 bit block
   
    logfile<<"Ring Header Length(bytes), Type(01=Begin): "<<BufferBytes<<" "<<BufferType<<std::endl;
   
    //BufferBytes includes the BufferLength and BufferType, which is self-inclusive and 8bytes
    BufferBytes -= BufferWords*sizeof(short);
    BufferWords = BufferBytes/(sizeof(short));
    
    if (BufferType == 1) { //Type 1 means BEGIN_RUN.   
      // Now that we know the length we read the rest of the header
      delete [] buffer;

      //re-size header buffer using the remainder of the BufferBytes
      //gotten from previous step

      buffer = new unsigned short[BufferWords];
      evtfile.read((char*)buffer,BufferBytes);
      


      //Include below anything else wanted from the header
      //this can be run number, title, etc...
      //right now this is just getting the start time from the run

      unsigned short *tpointer;tpointer=buffer+4;
      int lowtime = *tpointer++ ;
      int hightime = *tpointer++ << 16 ;//read byte as high byte
      timer = (hightime + lowtime);
      logfile<<"timer start :"<<timer<<std::endl;

      /****************************************************/
    


      delete [] buffer;
      

      //looping over new buffer==event format
      //first we read in "Event Buffers or Item" header

      while(!evtfile.eof()){
	NBuffers++;
	if(NBuffers%30000==0)std::cout<<NBuffers<<std::endl;
	//first we read in "Event Buffers or Item" header
	//reading in 10 bytes reads the entire item header.

	BufferWords = 4; BufferBytes = BufferWords * sizeof(short); 
	buffer = new unsigned short[BufferWords];
	evtfile.read((char*)buffer,BufferBytes); 

	//remove from Buffer Bytes the amount of buffer already read.
	BufferBytes = buffer[0] - BufferWords*sizeof(short);
	//BufferWords is now the size of the buffer needed to read the event contents.
	BufferWords = BufferBytes/(sizeof(short)); 
	ItemType = buffer[2];
	delete [] buffer;

	if(ItemType==30){ //30 is PhysicsEvent
	  BufferPhysics++;
	  Reset();
	  buffer= new (nothrow) unsigned short[BufferWords];
	  if (buffer==0){
	    logfile<<"memory could not be allocated, occurance: "<<mem_counter++<<std::endl;
	    logfile<<"Tried to allocated"<<2*BufferWords<<"bytes"<<std::endl;
	    if(mem_counter==10){
	      logfile<<"event ended abruptly, check for cause"<<std::endl;
	      break;}
	    else{
	    evtfile.seekg(BufferBytes,ios::cur);
	    continue;}
	  }

	  evtfile.read((char*)buffer,BufferBytes);
	  unsigned short * gpointer,* endpointer;
	  gpointer = buffer; //copy pointer to beginning of buffer(outside of header) 
	  gpointer++;//extra word after item type
	  endpointer = buffer + BufferWords;


	  //first read in CAEN data, keep track of how many CAEN modules 
	  //that are plugged in (not just the ones you are using.)

	  for(unsigned int i=0; i<caen_num;i++){
	    short dat;
	    short chan;
	    short ch_hits;
	    short geoaddress;
	    if(*gpointer==0xffff){
	      gpointer=gpointer+2;
	      continue;}
	    ch_hits = ( *gpointer++ & 0xff00 ) >> 8; //read high byte as low byte
	    geoaddress = ( *gpointer++ & 0xf800) >> 11;
	  
	    if(geoaddress!=caen_stack[i]){
	      logfile<<geoaddress<<" different from geoaddress in stack: "<< caen_stack[i]<<" at file pos: "<< evtfile.tellg()<<std::endl;
	      gpointer+=ch_hits*2;
	    }
	    else{
	      for (short jj=0;jj<ch_hits;jj++){
		dat =  *gpointer++ & 0xfff;
		chan = *gpointer++ & 0x1f;
		SortGeoChan(geoaddress,chan,dat);
      
	      }
	    }
	    adc_counter= *gpointer++;
	    gpointer = gpointer + 3 ; //jump 3 words to skip rest of CAEN End of Block
	   
	  }
	  
	  //next comes MESYTEC modules.
	  
	  for(unsigned int i=0; i<mesy_num; i++){
	    short dat, chan;
	    short shortwords;
	    short ModuleID;
	    unsigned short * zpointer; zpointer = gpointer;
	   
	    if(*zpointer==0xffff){
	      zpointer = zpointer + 2;
	 
	      continue;}
	    //this pointer gets number of 32 bit words so we must multiply by 2 for short words.
	    shortwords = 2 * ( *zpointer++ & 0x0fff ); 
	    ModuleID = *zpointer++ & 0x00ff;
	 
	    if(ModuleID!=mesy_stack[i]){
	      logfile<<ModuleID<<" different from geoaddress in stack: "<< mesy_stack[i]<<" at file pos: "<< evtfile.tellg()<<std::endl;
	      gpointer+=shortwords;
	    }
	    else{
	      while( zpointer < ( gpointer + shortwords)){
		dat = *zpointer++ & 0xfff;
		chan = *zpointer++ & 0x1f;
		
		SortGeoChan(ModuleID,chan,dat);
		
		
  
	      }
	    }
	    mes_counter = *zpointer++;
	    zpointer = zpointer + 3; //jump over EOB + ffff
	    gpointer = zpointer; //move gpointer to end of this zpointer
	  }

	  if(*gpointer!=0xffff)
	    Event[1]=1;
	  gpointer = gpointer + 2; //end of buffer ffffs
	  // this is a check to make sure the buffer is properly formatted.
	  // gpointer should, at this moment, point to the endpointer which has an
	  //an address dictated by buffer words at the beginning.
	  if (gpointer!=endpointer){
	    gpointer=endpointer;
	    Event[1]+=2;//flag
	    // int diff = endpointer-gpointer;
	    //logfile<<"Item Header(Length, Type): "<< BufferBytes<<" "<<ItemType<<std::endl;
	    //logfile<<diff<<" words went unread out of "<<BufferWords<<std::endl;
	    // logfile<<"last adc counter number: "<<adc_counter<<" and last mesy counter number: "<<mes_counter<<std::endl;
	    //Reset();
	    //BufferPhysics--;
	    //  delete [] buffer;
	    //continue;
	  }
	 
	  DataTree->Fill();
	  Reset();
	  delete [] buffer;

	}//end if ItemType==30 (physics)

      /**********End Run Header Type************************************/
      // this means we are reading a file header and a 2 means END_RUN and we should 
      //break out of this event file

	else if(ItemType==02){
	  logfile<<" An Item Type 2 was intercepted meaning this is a RING_HEADER(2=End)"<<std::endl;
	  logfile<<"End of Run :"<<OtherInfo<<std::endl;
	  buffer = new unsigned short[BufferWords];
	  evtfile.read((char*)buffer,BufferBytes);
	  
	  //Include below anything else wanted from the header
	  //this can be run number, title, etc...
	  //right now this is just getting the start time from the run
	  
	  unsigned short *tpointer;tpointer=buffer+3;
	  int lowtime = *tpointer++ ;
	  int hightime = *tpointer++ << 16 ;//read byte as high byte
	  timer = (hightime + lowtime);
	  logfile<<"timer stop :"<<timer<<std::endl;
	  
	  delete [] buffer;
	  Reset();
	  break;}
	
	/**********Illegal Begin Run Header Type******************************/
	//this means we are reading a file header and a 1 means Begin_RUN
	//which should not happen since we are already inside a begin run
	//this is probably the result of readout hanging up and concatenating
	//the next run to the end of this one.
	
	else if(ItemType==01){
	  logfile<<" An Item Type 1 was intercepted meaning this is a RING_HEADER(1=Begin)"<<std::endl;
	  
	  logfile<<"Actually, the begin of Run: "<<OtherInfo<<std::endl;
      buffer = new unsigned short[BufferWords];
      evtfile.read((char*)buffer,BufferBytes);
     
      //Include below anything else wanted from the header
      //this can be run number, title, etc...
      //right now this is just getting the start time from the run
      
      unsigned short *tpointer;tpointer=buffer+3;
      int lowtime = *tpointer++ ;
      int hightime = *tpointer++ << 16 ;//read byte as high byte
      timer = (hightime + lowtime);
      logfile<<"timer stop :"<<timer<<std::endl;

      delete[]buffer;
      Reset();
      break;}
	

      /**********Physics Event Counter Item Type***********************/
      //right now this just prints to the log file how long its been since the previous 
      // "counter" type was encountered(since this has been coming in correspondance with errors 
      //related to buffer overloads).  This was to understand how frequently this happens.

	else if(ItemType==31){//31 is PhysicsEventCounter
	  //logfile<<" An Item Type 31 was intercepted meaning this is a Physics Event Counter"<<std::endl;
	  buffer= new (nothrow) unsigned short[BufferWords];
	  if (buffer==0){
	    logfile<<"memory could not be allocated, occurance: "<<mem_counter++<<std::endl;
	    logfile<<"Tried to allocated"<<BufferBytes<<"bytes"<<std::endl;
	    if(mem_counter==10){
	      logfile<<"event ended abruptly, check for cause"<<std::endl;
	      break;}
	    else{
	      evtfile.seekg(BufferBytes,ios::cur);
	      continue;}
	  }
	  evtfile.read((char*)buffer,BufferBytes); //skip to end of this type
	  unsigned short * tpointer;
	  tpointer = buffer; tpointer++;
	  int lowtime = *tpointer++ ;
	  int hightime = *tpointer << 16 ;//read byte as high byte
	  int timeoffset = (hightime + lowtime) - timer;
	  timer = (hightime + lowtime);
	  //logfile<<"time counter: "<<timer<<std::endl;
	  //logfile<<"time since last event counter: "<<timeoffset <<std::endl;
	  delete [] buffer;
	  Reset();
	  continue;}




      /**********Other Item Type***************************************/
      //this is most likely a scaler increment event type.  Modify this later if you want
      //to save any information regarding scalers.

	else if(ItemType==20){//20 is Scaler
	  //	  logfile<<" An Item Type 20 was intercepted meaning this is a Scaler"<<std::endl;
	  buffer= new (nothrow) unsigned short[BufferWords];
	  if (buffer==0){
	    logfile<<"memory could not be allocated, occurance: "<<mem_counter++<<std::endl;
	    logfile<<"Tried to allocated"<<BufferBytes<<"bytes"<<std::endl;
	    if(mem_counter==10){
	      logfile<<"event ended abruptly, check for cause"<<std::endl;
	      break;}
	    else{
	      evtfile.seekg(BufferBytes,ios::cur);
	      continue;}
	  }
	  evtfile.read((char*)buffer,BufferBytes); //skip to end of this type
	  unsigned short* ipointer;
	  ipointer = buffer;
	  ipointer+=2;//time_start offset
	  ipointer+=2;//time_end offset
	  ipointer+=2;//time_stamp
	  unsigned short scalercount = *ipointer;ipointer+=2;
	  if(scaler_values.size()!=0){//if scaler_list is even set
	    ScalerValueIterator it=scaler_values.begin();
	    for(unsigned short i=0;i<scalercount;i++){
	      if(it==scaler_values.end())
		break;
	      (*it) = (Double32_t)*ipointer;ipointer+=2;
	      it++;
	    }
	  }
	  ScalerTree->Fill();
	  Event[2]+=1; //increase scaler index to know which data goes with which scaler
	  for(ScalerValueIterator it=scaler_values.begin();it!=scaler_values.end();it++){
	    (*it)=0;
	  }
	  
	  
	  delete [] buffer;
	  Reset();
	  continue;
	}
      
	
	/**********Other Item Type***************************************/
	else{
	  badcounter++;
	  logfile<<" An Item Type "<<ItemType<<" was intercepted meaning this is an unidentified buffer"<<std::endl;
	  if(mem_counter!=0&&badcounter>10){
	    evtfile.seekg(0,ios::end);
	    logfile<<"Ending this, check for fault"<<std::endl;
	    break;}
	    
	  evtfile.seekg(2*BufferWords,ios::cur);
	  Reset();
	  continue;}

      }//end evtfile.eof
      
      evtfile.close();
      
    }//end if(BufferType==1)
  }

    

  
  std::cout << setprecision(3);
  std::cout << "Total buffers = " << NBuffers << std::endl;
  std::cout << "Physics buffers = " << BufferPhysics  << " (" 
       << 100.0*BufferPhysics/NBuffers << "\% of total buffers)" << std::endl;
  std::cout << "Output file: " << output_file <<  std::endl;
  
  if(buffer!=0)buffer = NULL;
  logfile.close();
  DataTree->Write();
  ScalerTree->Write();
  RootFile->Close(); 
  // The function Close() first writes to the ROOT file and then closes it.  

  return 0;

}

#endif
