////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013                             
////////////////////////////////////////////////////////////////////////



#include "RNUnpack2Root.hpp"

using namespace std;


bool RNUnpack2Root::init(const std::string& config){
  ifstream cfg(config.c_str());
  if (!cfg.is_open()){
    cout<<"could not load init file: "<<config<<endl;
    return 0;
  }
 
  
  std::string temp;
  while(!cfg.eof()){
    std::vector<std::string>input;
    std::cout<<sak::ReadLine(cfg,input,4)<<std::endl;
    if (input.size()!=4){
      std::cout<<"config file has line with diff than 4 entries"<<std::endl;
      return 0;
    }
    if(input[0]=="caen"){
      caen_num++;
      caen_stack.push_back(RN_module(input[1],sak::string_to_int(input[2]),sak::string_to_int(input[3]),0));
   
    }
    else if(input[0]=="mesy"){
      mesy_num++;
      mesy_stack.push_back(RN_module(input[1],sak::string_to_int(input[2]),sak::string_to_int(input[3]),1));
    }
    else{
      std::cout<<"undefined entry in config file"<<std::endl;
      continue;
    }
   

    
    
    
  }
  
  
}


void RNUnpack2Root::Reset(){
  for(int i=0;i<caen_num;i++){
    caen_stack[i].Reset();
  }
  for(int i=0;i<mesy_num;i++){
    mesy_stack[i].Reset();
  }
}



///////////////////////////////////////////////////////////////////////////////////
// File converter, from .evt to .root. Arguments:
// - files_list: used to specify the 
///////////////////////////////////////////////////////////////////////////////////
int RNUnpack2Root::Convert(std::vector<int>&run_number,std::string data_dir,std::string output_file){
  
  TFile* RootFile;
  TTree* DataTree;
  unsigned int BufferWords; //number of short length words, each 2 bytes
  unsigned int BufferBytes; //number of bytes to be stored
  unsigned int BufferType;
  unsigned short OtherInfo;
  unsigned int ItemType;
  unsigned int NBuffers = 0; 
  unsigned short mem_counter=0,badcounter=0;
  int BufferPhysics = 0;
  ifstream ListEVT;
  ifstream evtfile;
  ofstream logfile;
  unsigned short * buffer;
  int timer=0;

  // ROOT output file
  RootFile = new TFile(output_file.c_str(),"RECREATE");  
  string logger = Form("%s.log",output_file.c_str());
  logfile.open(logger.c_str(),ios::out);

  // Data Tree
  DataTree = new TTree("DataTree","DataTree");
 
  for(unsigned int i=0;i<caen_stack.size();i++){
    DataTree->Branch(Form("%s.",caen_stack[i].Name().c_str()),"RN_module",&(caen_stack[i]));
  }
  for(unsigned int i=0;i<mesy_stack.size();i++){
    DataTree->Branch(Form("%s.",mesy_stack[i].Name().c_str()),"RN_module",&(mesy_stack[i]));
  } 
 
  //Loop over files in the data file list.
  for(int b=0;b<run_number.size();b++){
    //this section to properly format the evt number to buffer run number with zeroes.
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
	//first we read in "Event Buffers or Item" header
	//reading in 10 bytes reads the entire item header.

	BufferWords = 5; BufferBytes = BufferWords * sizeof(short); 
	buffer = new unsigned short[BufferWords];
	evtfile.read((char*)buffer,BufferBytes); 

	//remove from Buffer Bytes the amount of buffer already read.
	BufferBytes = buffer[0] - BufferWords*sizeof(short);
	//BufferWords is now the size of the buffer needed to read the event contents.
	BufferWords = BufferBytes/(sizeof(short)); 
	ItemType = buffer[2];
	OtherInfo=buffer[4];//important for non-event type construct
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
	  endpointer = buffer + BufferWords;


	  //first read in CAEN data, keep track of how many CAEN modules 
	  //that are plugged in (not just the ones you are using.)

	  for(int i=0; i<caen_num;i++){
	    short dat;
	    short chan;
	    short ch_hits;
	    short geoaddress;
	    if(*gpointer==0xffff){
	      gpointer=gpointer+2;
	   
	      continue;}
	    ch_hits = ( *gpointer++ & 0xff00 ) >> 8; //read high byte as low byte
	    geoaddress = ( *gpointer++ & 0xf800) >> 11;
	  
	    if(geoaddress!=caen_stack[i].GeoAddress()){
	      logfile<<geoaddress<<" different from geoaddress in stack: "<< caen_stack[i].GeoAddress()<<" at file pos: "<< evtfile.tellg()<<std::endl;
	      gpointer+=ch_hits*2;
	    }
	    else{
	      for (short jj=0;jj<ch_hits;jj++){
		dat =  *gpointer++ & 0xfff;
		chan = *gpointer++ & 0x1f;
		caen_stack[i].fCh[chan] = dat;
	      }
	    }
	    adc_counter= *gpointer++;
	    gpointer = gpointer + 3 ; //jump 3 words to skip rest of CAEN End of Block
	    
	  }
	  
	  //next comes MESYTEC modules.
	  
	  for(int i=0; i<mesy_num; i++){
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
	 
	    if(ModuleID!=mesy_stack[i].GeoAddress()){
	      logfile<<ModuleID<<" different from geoaddress in stack: "<< mesy_stack[i].GeoAddress()<<" at file pos: "<< evtfile.tellg()<<std::endl;
	      gpointer+=shortwords;
	    }
	    else{
	      while( zpointer < ( gpointer + shortwords)){
		dat = *zpointer++ & 0xfff;
		chan = *zpointer++ & 0x1f;
		
		mesy_stack[i].fCh[chan]=dat;
	      }
	    }
	    mes_counter = *zpointer++;
	    zpointer = zpointer + 3; //jump over EOB + ffff
	    gpointer = zpointer; //move gpointer to end of this zpointer
	  }


	  // this is a check to make sure the buffer is properly formatted.
	  // gpointer should, at this moment, point to the endpointer which has an
	  //an address dictated by buffer words at the beginning.
	  if (gpointer!=endpointer){
	    gpointer=endpointer;
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
	  logfile<<" An Item Type 31 was intercepted meaning this is a Physics Event Counter"<<std::endl;
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
	  logfile<<"time counter: "<<timer<<std::endl;
	  logfile<<"time since last event counter: "<<timeoffset <<std::endl;
	  delete [] buffer;
	  Reset();
	  continue;}




      /**********Other Item Type***************************************/
      //this is most likely a scaler increment event type.  Modify this later if you want
      //to save any information regarding scalers.

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
  RootFile->Close(); 
  // The function Close() first writes to the ROOT file and then closes it.  

  return 0;

}


