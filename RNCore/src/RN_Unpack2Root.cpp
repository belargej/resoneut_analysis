////////////////////////////////////////////////////////////////////////
///Originally Created by: Sean Kuvin- 2013                             
////////////////////////////////////////////////////////////////////////
#ifndef __RNUNPACKER_CXX
#define __RNUNPACKER_CXX

#include "RN_Unpack2Root.hpp"
#include "RN_Module.hpp"

namespace unpacker{
  TFile* RootFile;
  TTree* DataTree;
  TTree* ScalerTree;
  unsigned int BufferWords(0); //number of short length words, each 2 bytes
  unsigned int BufferBytes(0); //number of bytes to be stored
  unsigned int BufferType(0);
  unsigned int NBuffers(0); 
  int BufferPhysics(0);
  std::ifstream evtfile;
  std::ofstream logfile;
  unsigned short * buffer;
  int timer(0); 
  int adc_counter;
  int mes_counter;


  //Tree Parameters to be Unpacked into ROOTFile
  Int_t Event[3];//stores RunNum/flag/scaler
  ScalerNames scaler_names;
  ScalerValues scaler_values;
  RN_Module_Stack caen_stack;
  RN_Module_Stack mesy_stack;
  TBranch        *b_Event;  //!


  int ExtractRingBuffer(){
    BufferWords = 4; BufferBytes = BufferWords * sizeof(short);     
    buffer = new unsigned short[BufferWords];
    evtfile.read((char*)buffer,BufferBytes);
    if(evtfile.fail()||evtfile.eof())
      return 0;
    
    NBuffers++;
    if(NBuffers%30000==0)std::cout<<NBuffers<<std::endl;
    BufferBytes = buffer[0] - BufferWords*sizeof(short);
    BufferWords = BufferBytes/(sizeof(short)); 
    BufferType = buffer[2];
    delete [] buffer;
    
    buffer= new (std::nothrow) unsigned short[BufferWords];
    if (buffer==0){
      std::cout<<"memory could not be allocated, occurance: "<<std::endl;
      std::cout<<"Tried to allocated"<<2*BufferWords<<"bytes"<<std::endl;
      exit(EXIT_FAILURE);
    }
    
    evtfile.read((char*)buffer,BufferBytes);
    if(evtfile.fail()||evtfile.eof())
      return 0;
    
    switch(BufferType){
      
    case(BEGIN_RUN):
      UnpackBeginRun();
      break;
    case(END_RUN):
      UnpackEndRun();
      break;
    case(INCREMENTAL_SCALERS):
      UnpackScalers();
      break;
    case(PHYSICS_EVENT):
      UnpackPhysicsEvent();
      break;
    case(PHYSICS_EVENT_COUNT):
      UnpackPhysicsEventCounter();
      break;
    default:
      UnpackUndefined();
      break;
    } 
    
    delete [] buffer;
    
    return 1;
  }

  int UnpackBeginRun(){
    Event[0]=buffer[0];	  
    std::cout<<"Converting Run: "<<buffer[0]<<std::endl;
    logfile<<"Converting Run: "<<buffer[0]<<std::endl;
    unsigned short *tpointer;tpointer=buffer+4;
    int lowtime = *tpointer++ ;
    int hightime = *tpointer++ << 16 ;//read byte as high byte
    timer = (hightime + lowtime);
    std::cout<<"timer start :"<<timer<<std::endl;
    logfile<<"timer start :"<<timer<<std::endl;
    char title[TITLE_MAXSIZE+1];
    strcpy(title,(char*)tpointer);
    std::cout<<title<<std::endl;
    logfile<<title<<std::endl;
    return 1;
    
  }
  
  
  int UnpackEndRun(){
    logfile<<"End of Run :"<<buffer[0]<<std::endl;
    std::cout<<"End of Run :"<<buffer[0]<<std::endl;
    Event[0]=0;  
    unsigned short *tpointer;tpointer=buffer+4;
    int lowtime = *tpointer++ ;
    int hightime = *tpointer++ << 16 ;//read byte as high byte
    timer = (hightime + lowtime);
    logfile<<"timer stop :"<<timer<<std::endl;
    std::cout<<"timer stop :"<<timer<<std::endl;
    ResetTreeParameters();
    return 1;
  }
  
  int UnpackScalers(){ 
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
    if(ScalerTree)
      ScalerTree->Fill();
    Event[2]+=1; //increase scaler index to know which data goes with which scaler
    for(ScalerValueIterator it=scaler_values.begin();it!=scaler_values.end();it++){
      (*it)=0;
    }
    
    ResetTreeParameters();
    
    return 1;
    
  }
  
  
  int UnpackPhysicsEventCounter(){
    unsigned short * tpointer;
    tpointer = buffer; tpointer++;
    int lowtime = *tpointer++ ;
    int hightime = *tpointer << 16 ;//read byte as high byte
    // int timeoffset = (hightime + lowtime) - timer;
    timer = (hightime + lowtime);
    
  return 1;
  
  }
  
  int UnpackPhysicsEvent(){
    //check caen stack size, check mesy stack size
    //these are sorted in the order they are added 
    //to the stack, caen first then mesytec next.
    unsigned int caen_num=caen_stack.GetSize();
    unsigned int mesy_num=mesy_stack.GetSize();

    BufferPhysics++;
    ResetTreeParameters();	  
    unsigned short * gpointer,* endpointer;
    endpointer = buffer + BufferWords;
    gpointer = buffer; //copy pointer to beginning of buffer(outside of header) 
    gpointer++;//extra word after item type
    
  //first read in CAEN data, keep track of how many CAEN modules 
  //that are plugged in (not just the ones you are using.)
    
    for(unsigned int i=0; i<caen_num;i++){
      short dat;
      short chan;
      short ch_hits;
      short geoaddress;
      if(*gpointer==0xffff){
	gpointer=gpointer+2;
	caen_stack.NextModule(0);
      continue;}
      ch_hits = ( *gpointer++ & 0xff00 ) >> 8; //read high byte as low byte
      geoaddress = ( *gpointer++ & 0xf800) >> 11;
      
      if(!caen_stack.NextModule(geoaddress)){
	std::cout<<geoaddress<<" different from geoaddress in stack: "<< caen_stack.CurrentModule()->GeoAddress()<<" at file pos: "<< evtfile.tellg()<<std::endl;
	gpointer+=ch_hits*2;
      }
      else{
	for (short jj=0;jj<ch_hits;jj++){
	  dat =  *gpointer++ & 0xfff;
	  chan = *gpointer++ & 0x1f;
	  caen_stack.SortGeoChVal(geoaddress,chan,dat);
	  
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
	mesy_stack.NextModule(0);
	continue;}
      //this pointer gets number of 32 bit words so we must multiply by 2 for short words.
      shortwords = 2 * ( *zpointer++ & 0x0fff ); 
      ModuleID = *zpointer++ & 0x00ff;
      
      if(!mesy_stack.NextModule(ModuleID)){
	std::cout<<ModuleID<<" different from geoaddress in stack: "<< mesy_stack.CurrentModule()->GeoAddress()<<" at file pos: "<< evtfile.tellg()<<std::endl;
	gpointer+=shortwords;
      }
      else{
	while( zpointer < ( gpointer + shortwords)){
	  dat = *zpointer++ & 0xfff;
	  chan = *zpointer++ & 0x1f;
	
	  mesy_stack.SortGeoChVal(ModuleID,chan,dat);
	  
	  
	  
	}
      }
      mes_counter = *zpointer++;
      zpointer = zpointer + 3; //jump over EOB + ffff
      gpointer = zpointer; //move gpointer to end of this zpointer
    }
  
    while(gpointer < endpointer){
      if(*gpointer==0xffff)
	gpointer++;
      else if(*gpointer!=0xffff){
	Event[1]=1;
	gpointer++;
      }
    }
    
    if(gpointer!=endpointer){
      Event[1]=2;
      gpointer=endpointer;
    }
    
    if(DataTree)
      DataTree->Fill();
    return 1;
  }
  
int UnpackUndefined(){
  std::cout<<" An Item Type "<<BufferType<<" was intercepted meaning this is an unidentified buffer"<<std::endl;
  std::cout<<"Ending this, check for fault"<<std::endl;
  delete[]buffer;
  exit(EXIT_FAILURE);      
}


 
bool InitStack(const std::string & configfile){
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
	  // caen_stack.push_back(sak::string_to_int(input[i]));
	}
      else if(input[0]=="mesy_stack")
	for(unsigned int i=1;i<input.size();i++){
	  // mesy_stack.push_back(sak::string_to_int(input[i]));
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
  
  
  void ResetTreeParameters(){
    Event[1]=0;
    caen_stack.Reset();
    mesy_stack.Reset();  
  }

  
  
  ///////////////////////////////////////////////////////////////////////////////////
// File converter, from .evt to .root. Arguments:
// - files_list: used to specify the 
///////////////////////////////////////////////////////////////////////////////////

  int Convert2Root(std::vector<std::string>&run_number,std::string data_dir,std::string output_file){

    // ROOT output file
    RootFile = new TFile(output_file.c_str(),"RECREATE");  
    std::string logger = Form("%s.log",output_file.c_str());
    logfile.open(logger.c_str(),std::ios::out);

    // Data Tree
    DataTree = new TTree("DataTree","DataTree");
    ScalerTree = new TTree("ScalerTree","ScalerTree");
    
    //flag is used to notify the user of any issues seen during unpacking
    DataTree->Branch("Event",&Event,"RunNum/I:flag/I:ScalerIDX/I"); 

    caen_stack.AddBranches(DataTree);
    mesy_stack.AddBranches(DataTree);

    ScalerTree->Branch("Scaler",&scaler_values);
    
    //Loop over files in the data file list.
    for(unsigned int b=0;b<run_number.size();b++){
      //this section to properly format the evt number to buffer run number with zeroes.
      std::string name = data_dir + "/run-" + run_number[b] + ".evt";
      
      //Open evt file
      evtfile.open(name.c_str(),std::ios::binary);      
      if (!evtfile.is_open()){
	std::cout << "  Could not open " << name << std::endl;
	exit(EXIT_FAILURE);
      }
      else 
	std::cout << "  Converting " << name << " ..." << std::endl;
      
      while(ExtractRingBuffer()){}
      evtfile.close();
    }  
    std::cout << std::setprecision(3);
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


  int Convert2Root(const std::string& name,std::string output_file){

    // ROOT output file
    RootFile = new TFile(output_file.c_str(),"RECREATE");  
    std::string logger = Form("%s.log",output_file.c_str());
    logfile.open(logger.c_str(),std::ios::out);

    // Data Tree
    DataTree = new TTree("DataTree","DataTree");
    ScalerTree = new TTree("ScalerTree","ScalerTree");
    
    //flag is used to notify the user of any issues seen during unpacking
    DataTree->Branch("Event",&Event,"RunNum/I:flag/I:ScalerIDX/I"); 
    
    caen_stack.AddBranches(DataTree);
    mesy_stack.AddBranches(DataTree);

    ScalerTree->Branch("Scaler",&scaler_values);
    
    //Loop over files in the data file list.
    
    //this section to properly format the evt number to buffer run number with zeroes.
    
    
    //Open evt file
    evtfile.open(name.c_str(),std::ios::binary);      
    if (!evtfile.is_open()){
      std::cout << "  Could not open " << name << std::endl;
      exit(EXIT_FAILURE);
    }
    else 
      std::cout << "  Converting " << name << " ..." << std::endl;
    
    while(ExtractRingBuffer()){}
    evtfile.close();
    
    std::cout << std::setprecision(3);
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
}
    





#endif
