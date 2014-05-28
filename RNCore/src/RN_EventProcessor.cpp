/************************************************************************
Class: RN_EventProcessor
Author: Sean A. Kuvin
************************************************************************/
#ifndef __RNEVENTPROCESSOR_CXX
#define __RNEVENTPROCESSOR_CXX

#include "RN_EventProcessor.hpp"
#include "RN_Module.hpp"
#include "sak_ReadBuffer.hpp"
#include "DataFormat.h"
#include "RN_Root.hpp"

ClassImp(RN_EventProcessor);

//default EventProcessor, ready to perform the Convert2Root Methods
//AFTER a module stack has been configured.
RN_EventProcessor gEventProcessor;



RN_EventProcessor::RN_EventProcessor(const std::string&a,const std::string&b):RN_BaseClass(a.c_str(),b.c_str()){
  for(int i=0;i<3;i++){
    fEvent[i]=0;
  }
  for(int i=0;i<32;i++){
    fScalerValues[i] = 0;
    fScalerSums[i] = 0;
  }
}

void RN_EventProcessor::InitRootFile(TString rootfilename)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed)

  if(!RNROOT::RN_RootSet)
    RNROOT::Initialize();
  
  fChain=new TChain("DataTree");
  fChain->Add(rootfilename);
  
  if(fChain->GetBranch("Event"))
    fChain->SetBranchAddress("Event",&fEvent, &b_Event);
  else
    std::cout<<"No Event Branch Present, check in sorting code"<<std::endl;
  
  RNROOT::gParameter_stack.SetBranches(fChain);
  RNROOT::gModule_stack.SetBranches(fChain);
}



bool RN_EventProcessor::Begin(){
  return 1;
}

void RN_EventProcessor::SetRootOutputFile(const std::string& filename){
  
  if(fgRootFile){
    std::cout<<"root output file already open, please write out and close first"<<std::endl;
    return ;
  }
  fgRootFile=new TFile(filename.c_str(),"RECREATE");
}

void RN_EventProcessor::SetRootOutputFileAndTree(const std::string& filename,const std::string& treename){
  if(fgDataTree){
    std::cout<<"output datatree already set"<<std::cout;
    return ;
  }
  
  if(fgRootFile){
    std::cout<<"root output file already open, please write out and close first"<<std::endl;
    return ;
  }
  
  fgRootFile=new TFile(filename.c_str(),"RECREATE");
  fgDataTree=new TTree(treename.c_str(),treename.c_str());
  fgScalerTree = new TTree("ScalerTree","ScalerTree");

  //flag is used to notify the user of any issues seen during unpacking
  fgDataTree->Branch("Event",&fEvent,"RunNum/I:flag/I:ScalerIDX/I"); 
  
  RNROOT::gModule_stack.AddBranches(fgDataTree);
  
  fgScalerTree->Branch("Scaler",&fScalerValues,"Scaler[32]/F");
  

}

void RN_EventProcessor::WriteOut(){
  if(fgRootFile){
    fgRootFile->Write();
    fgRootFile->Close();
    fgRootFile = NULL;
    fgDataTree = NULL;
    fgScalerTree = NULL;
  }
}


void RN_EventProcessor::Loop(Long64_t start, Long64_t evnum){
  if(!fChain){
    std::cout<<"fChain not initialized, no root files added to Analyzer\n";
    return;  	
  }
  
  std::cout<<"Total Entries in Chain: "<<TotEntries()<<std::endl;
  Long64_t totentries= TotEntries();    
  if (start!=0&&evnum!=0)
    if(start+evnum<totentries)
      totentries=start+evnum;

  
  Begin();
  RNROOT::gAnalyzer_stack.Begin();
  
  for (Long64_t i=start;i<totentries;i++){
    Reset();
    RNROOT::gAnalyzer_stack.Reset();
    RNROOT::gParameter_stack.Reset();
    //extract from the tree all of the module parameters
    if(!GetEntry(i)){
      continue;
    }
    if(i%30000==0)std::cout<<i<<std::endl;

    GetDetectorEntry();
    Process();
    if(RNROOT::gAnalyzer_stack.Process()){
      //only if Process returns true do we move to all of the ProcessFill
      ProcessFill();
      RNROOT::gAnalyzer_stack.ProcessFill();
    }
  }


  RNROOT::gAnalyzer_stack.Terminate();
  Terminate();

}

int RN_EventProcessor::ExtractRingBuffer(){
    fBufferWords = 4; fBufferBytes = fBufferWords * sizeof(short);     
    fBuffer = new unsigned short[fBufferWords];
    fEvtFile.read((char*)fBuffer,fBufferBytes);
    if(fEvtFile.fail()||fEvtFile.eof())
      return 0;
    
    fNBuffers++;
    if(fNBuffers%30000==0)std::cout<<fNBuffers<<std::endl;
    fBufferBytes = fBuffer[0] - fBufferWords*sizeof(short);
    fBufferWords = fBufferBytes/(sizeof(short)); 
    fBufferType = fBuffer[2];
    delete [] fBuffer;
    
    fBuffer= new (std::nothrow) unsigned short[fBufferWords];
    if (fBuffer==0){
      std::cout<<"memory could not be allocated, occurance: "<<std::endl;
      std::cout<<"Tried to allocated"<<2*fBufferWords<<"bytes"<<std::endl;
      exit(EXIT_FAILURE);
    }
    
    fEvtFile.read((char*)fBuffer,fBufferBytes);
    if(fEvtFile.fail()||fEvtFile.eof())
      return 0;
    
    switch(fBufferType){
      
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
    
    delete [] fBuffer;
    
    return 1;
  }

  //The following Unpack functions are not meant to be called on their own.
  //there are no checks if 'fBuffer' is allocated. 
  //These methods are only called during ExtractRingBuffer() 
  //which allocates fBuffer.


  int RN_EventProcessor::UnpackBeginRun(){
    fEvent[0]=fBuffer[0];	  
    std::cout<<"Converting Run: "<<fBuffer[0]<<std::endl;
    if(fLogFile)fLogFile<<"Converting Run: "<<fBuffer[0]<<std::endl;
    unsigned short *tpointer;tpointer=fBuffer+4;
    int lowtime = *tpointer++ ;
    int hightime = *tpointer++ << 16 ;//read byte as high byte
    fTimer = (hightime + lowtime);
    std::cout<<"fTimer start :"<<fTimer<<std::endl;
    if(fLogFile)fLogFile<<"fTimer start :"<<fTimer<<std::endl;
    char title[TITLE_MAXSIZE+1];
    strcpy(title,(char*)tpointer);
    std::cout<<title<<std::endl;
    if(fLogFile)fLogFile<<title<<std::endl;
    return 1;
    
  }
  
  
  int RN_EventProcessor::UnpackEndRun(){
    if(fLogFile)fLogFile<<"End of Run :"<<fBuffer[0]<<std::endl;
    std::cout<<"End of Run :"<<fBuffer[0]<<std::endl;
    fEvent[0]=0;  
    unsigned short *tpointer;tpointer=fBuffer+4;
    int lowtime = *tpointer++ ;
    int hightime = *tpointer++ << 16 ;//read byte as high byte
    fTimer = (hightime + lowtime);
    if(fLogFile)fLogFile<<"fTimer stop :"<<fTimer<<std::endl;
    std::cout<<"fTimer stop :"<<fTimer<<std::endl;
    ResetTreeParameters();
    return 1;
  }
  
  int RN_EventProcessor::UnpackScalers(){ 
    unsigned short* ipointer;
    ipointer = fBuffer;
    ipointer+=2;//time_start offset
    ipointer+=2;//time_end offset
    ipointer+=2;//time_stamp
    unsigned short scalercount = *ipointer;ipointer+=2;
    for(unsigned int i=0;i<scalercount;i++){
      fScalerValues[i] = (Double32_t)*ipointer;
      ipointer += 2;
      fScalerSums[i] += fScalerValues[i];
    }
    
    if(fgScalerTree)
      fgScalerTree->Fill();
    
    fEvent[2]+=1; //increase scaler index to know which data goes with which scaler
    //reset Scalers
    for(unsigned int i=0; i<scalercount; i++){
      fScalerValues[i] = 0;
    }
    
    ResetTreeParameters();
    
    return 1;
    
  }
  
  
  int RN_EventProcessor::UnpackPhysicsEventCounter(){
    unsigned short * tpointer;
    tpointer = fBuffer; tpointer++;
    int lowtime = *tpointer++ ;
    int hightime = *tpointer << 16 ;//read byte as high byte
    // int timeoffset = (hightime + lowtime) - fTimer;
    fTimer = (hightime + lowtime);
    
  return 1;
  
  }
  
  int RN_EventProcessor::UnpackPhysicsEvent(){
    //check caen stack size, check mesy stack size
    //these are sorted in the order they are added 
    //to the stack, caen first then mesytec next.

    fBufferPhysics++;
    ResetTreeParameters();	  

    unsigned short * gpointer,* endpointer;
    endpointer = fBuffer + fBufferWords;
    gpointer = fBuffer; //copy pointer to beginning of fBuffer(outside of header) 
    gpointer++;//extra word after item type

    //pass the pointer on to the stack of modules, each module will unpack the data using its own unpack method
    RNROOT::gModule_stack.UnpackModules(gpointer,(int)fEvtFile.tellg());
      
    while(gpointer < endpointer){
      if(*gpointer==0xffff)
	gpointer++;
      else if(*gpointer!=0xffff){
	fEvent[1]=1;
	gpointer++;
      }
    }
    
    if(gpointer!=endpointer){
      fEvent[1]=2;
      gpointer=endpointer;
    }
    
    if(fgDataTree)
      fgDataTree->Fill();
    return 1;
  }
  
  int RN_EventProcessor::UnpackUndefined(){
  std::cout<<" An Item Type "<<fBufferType<<" was intercepted meaning this is an unidentified fBuffer"<<std::endl;
  std::cout<<"Ending this, check for fault"<<std::endl;
  delete[]fBuffer;
  exit(EXIT_FAILURE);      
}


  
  void RN_EventProcessor::ResetTreeParameters(){
    fEvent[1] = 0;//flag reset
    
    RNROOT::gParameter_stack.Reset();
    RNROOT::gModule_stack.Reset();
    
  }

  
  
  ///////////////////////////////////////////////////////////////////////////////////
// File converter, from .evt to .root. Arguments:
// - files_list: used to specify the 
///////////////////////////////////////////////////////////////////////////////////

int RN_EventProcessor::Convert2Root(std::vector<std::string>&run_number,std::string data_dir,std::string output_file){
  
  // ROOT output file
  if(fgRootFile){
    std::cout<<"Output TFile already open, please close it, and try again"<<std::endl;
    return 1;
  }

  std::string logger = Form("%s.log",output_file.c_str());
  fLogFile.open(logger.c_str(),std::ios::out);
  SetRootOutputFileAndTree(output_file,"DataTree");


  //Loop over files in the data file list.
  for(unsigned int b=0;b<run_number.size();b++){
    //this section to properly format the evt number to fBuffer run number with zeroes.
    std::string name = data_dir + "/run-" + run_number[b] + ".evt";
    
    //Open evt file
    fEvtFile.open(name.c_str(),std::ios::binary);      
    if (!fEvtFile.is_open()){
      std::cout << "  Could not open " << name << std::endl;
      exit(EXIT_FAILURE);
    }
    else 
      std::cout << "  Converting " << name << " ..." << std::endl;
    
    while(ExtractRingBuffer()){}
    fEvtFile.close();
  }  
    std::cout << std::setprecision(3);
    std::cout << "Total buffers = " << fNBuffers << std::endl;
    std::cout << "Physics buffers = " << fBufferPhysics  << " (" 
	      << 100.0*fBufferPhysics/fNBuffers << "\% of total buffers)" << std::endl;
    std::cout << "Output file: " << output_file <<  std::endl;
    

    if(fBuffer!=0)fBuffer = NULL;
    fLogFile.close();
    fgDataTree->Write();
    fgScalerTree->Write();
    fgRootFile->Close(); 
    // The function Close() first writes to the ROOT file and then closes it.  
    
    return 0;
    
}


int RN_EventProcessor::Convert2Root(const std::string& name,std::string output_file){
  
  // ROOT output file
  if(fgRootFile){
    std::cout<<"Output TFile already open, please close it, and try again"<<std::endl;
    return 1;
  }

  std::string logger = Form("%s.log",output_file.c_str());
  fLogFile.open(logger.c_str(),std::ios::out);
  SetRootOutputFileAndTree(output_file,"DataTree");

  //Loop over files in the data file list.
  
  //this section to properly format the evt number to buffer run number with zeroes.
  
  
  //Open evt file
  fEvtFile.open(name.c_str(),std::ios::binary);      
  if (!fEvtFile.is_open()){
    std::cout << "  Could not open " << name << std::endl;
    exit(EXIT_FAILURE);
  }
  else 
    std::cout << "  Converting " << name << " ..." << std::endl;
  
  while(ExtractRingBuffer()){}
  fEvtFile.close();
  
  std::cout << std::setprecision(3);
  std::cout << "Total buffers = " << fNBuffers << std::endl;
  std::cout << "Physics buffers = " << fBufferPhysics  << " (" 
	    << 100.0*fBufferPhysics/fNBuffers << "\% of total buffers)" << std::endl;
  std::cout << "Output file: " << output_file <<  std::endl;
  int ScalerCount = fgScalerTree->GetEntries();

  fLogFile<<"Total Number of Scalers: "<<ScalerCount<<"\n";
  for(unsigned int i=0;i<32;i++){
    fLogFile<<"Scaler["<<i<<"] Sum = "<< fScalerSums[i] << ", Avg. = "<< fScalerSums[i] / ScalerCount<<"\n";
  }

  
  if(fBuffer!=0)fBuffer = NULL;
  fLogFile.close();
  fgDataTree->Write();
  fgScalerTree->Write();
  fgRootFile->Close(); 
  // The function Close() first writes to the ROOT file and then closes it.  
    
  return 0;
}

int RN_EventProcessor::AttachFromEVT(const TString& evtfilename,const std::string& outputfile){
  SetRootOutputFileAndTree(outputfile,"DataTree");
  return AttachFromEVT(evtfilename);
  
}
    
int RN_EventProcessor::AttachFromEVT(const TString& fEvtFilename){
  //Open evt file
  fEvtFile.open(fEvtFilename,std::ios::binary);      
  if (!fEvtFile.is_open()){
    std::cout << "  Could not open " << fEvtFilename << std::endl;
    exit(EXIT_FAILURE);
  }

  Begin();
  RNROOT::gAnalyzer_stack.Begin();

  Reset();
  RNROOT::gAnalyzer_stack.Reset();  
  
  while(ExtractRingBuffer()){
    GetDetectorEntry();
    Process();
    if(RNROOT::gAnalyzer_stack.Process()){
      //only if Process returns true do we move to all of the ProcessFill
      ProcessFill();
      RNROOT::gAnalyzer_stack.ProcessFill();
    }
    Reset();
    RNROOT::gParameter_stack.Reset();
    RNROOT::gAnalyzer_stack.Reset();
  }
  

  RNROOT::gAnalyzer_stack.Terminate();
  Terminate();
}




#endif
