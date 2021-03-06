///////////////////////////////////////////////////////////
///These are the classes to be packaged into RNCore.so
//////////////////////////////////////////////////////////


#ifdef __CINT__
#pragma link C++ defined_in "RN_BaseClass.hpp";
#pragma link C++ defined_in "sak_ReadBuffer.hpp";
//#pragma link C++ defined_in "RN_Unpack2Root.hpp";
#pragma link C++ defined_in "RN_EventProcessor.hpp";
#pragma link C++ defined_in "RN_BaseDetector.hpp";
#pragma link C++ defined_in "RN_NeutDetectorArray.hpp";
#pragma link C++ defined_in "RN_NaIArray.hpp";
#pragma link C++ defined_in "RN_VariableMap.hpp";
#pragma link C++ defined_in "RN_SiliconDetectors.hpp";
#pragma link C++ defined_in "RN_IonChamber.hpp";
#pragma link C++ defined_in "RN_Calibrator.hpp";
#pragma link C++ defined_in "RN_MassTable.hpp";
#pragma link C++ defined_in "RN_Particle.hpp";
#pragma link C++ defined_in "RN_Timing.hpp";
#pragma link C++ defined_in "RN_Module.hpp";
#pragma link C++ defined_in "RN_TriggerBit.hpp";
#pragma link C++ defined_in "RN_Analyzer.hpp";
#pragma link C++ defined_in "RN_Root.hpp";
#pragma link C++ defined_in "RN_Sim.hpp";
#pragma link C++ defined_in "RN_Sim_NEON.hpp";
#pragma link C++ defined_in "RN_SimPhysics.hpp";
#pragma link C++ defined_in "RN_Parameters.hpp";
#pragma link C++ defined_in "RN_ReactionInfo.hpp";
#pragma link C++ defined_in "RN_EnergyLoss.hpp";



#pragma link C++ class std::vector<RN_NeutDetector>;
#pragma link C++ class std::vector<RN_BaseDetector>;
#pragma link C++ class std::vector<RN_IonChamber>;
#pragma link C++ class std::vector<RN_S2Detector>;
#pragma link C++ class std::vector<RN_RFTime>;
#pragma link C++ class std::vector<RN_NaIDetector>;
#pragma link C++ class std::vector<RN_TriggerBit>;
#pragma link C++ class std::vector<RN_Particle>;
#pragma link C++ class std::vector<Double32_t>;


#endif
