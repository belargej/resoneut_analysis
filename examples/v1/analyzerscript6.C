{

  SetRootOutputFile("10_30_timing2_requireS1.root");
  trigger::SetICdsOFF();
  trigger::RequireS1();
  trigger::SetTriggerLocs(724,1321,529,529);
  trigger::Trigger_Analyzer aa;
  psd::NeutAnalyzer a;
  si_cal::S2_Analyzer b;
  ionchamber::IC_Analyzer c;
  coinc::S2_IC_Analyzer d;
  Module_Analyzer e;
  NaI_NeutAnalyzer f;
  coinc::S2_Neut_IC_Analyzer g;

  AddAnalyzer(&aa);
  AddAnalyzer(&a);
  AddAnalyzer(&b);
  AddAnalyzer(&c);
  AddAnalyzer(&d);
  AddAnalyzer(&e);
  AddAnalyzer(&f);
  AddAnalyzer(&g);


  ////10_09
  
  MainAnalyzer.Init("/data0/skuvin/data/RN006/ROOT/run2238.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2239.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2240.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2241.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2242.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2243.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2244.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2245.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2246.root");
  ///

  ///10_10 MP51
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2252.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2253.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2254.root");

  ///10_11 MP51or58
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2255.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2256.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2257.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2258.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2259.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2260.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2261.root");
  
  ///10_11 mp58
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2262.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2263.root");
  

  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2270.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2271.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2272.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2273.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2274.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2275.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2276.root");

  //  10_12
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2280.root"); 
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2281.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2283.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2284.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2285.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2286.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2287.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2289.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2290.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2291.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2292.root");
  

  
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2294.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2295.root");
  
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2296.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2297.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2298.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2299.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2300.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2301.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2302.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2303.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2304.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2305.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2306.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2307.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2308.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2309.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2310.root");
 
  ///10_14 MP63
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2324.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2325.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2326.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2327.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2328.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2329.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2330.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2331.root");
  
  ///10_15
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2335.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2336.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2337.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2338.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2339.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2340.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2341.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2342.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2343.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2344.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2345.root");
  MainAnalyzer.AddTree("/data0/skuvin/data/RN006/ROOT/run2346.root");
  
    




  LoadVariableFile("config/DetConfig_1014.in");
  SetCalibrations();

  psd::LoadGates("cuts/cuts_1013.root");
  si_cal::LoadGates("cuts/cuts_1017.root");
  ionchamber::LoadGates("cuts/cuts_1013_ic.root");
  coinc::Load_Si_IC_Gates("cuts/cuts_1017.root");
  MainAnalyzer.Loop();


}
