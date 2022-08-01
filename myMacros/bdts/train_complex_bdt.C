#include "TFile.h"
#include "TTree.h"
#include "TCut.h"

void train_complex_bdt()
{
    TMVA::Tools::Instance(); 
    // open file and retrieve trees
    TFile *inputfile = new TFile("tmva_data_complex.root");
    //TFile *inputfile = new TFile("/home/llr/cms/kalipoliti/CMSSW_9_4_10/src/HeavyIonsAnalysis/JetAnalysis/test/HiForestAOD.root");
    TTree *signaltree = (TTree *) inputfile->Get("TreeS");
    TTree *backgroundtree = (TTree *) inputfile->Get("TreeB");

    TFile *outputFile = new TFile("tmva_results_complex.root", "RECREATE"); 

    // load trees into TMVA
    TMVA::DataLoader *loader = new TMVA::DataLoader("mydataloader");
    

    TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outputFile, "V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification"); 
    loader->AddVariable("ip3dSig", 'F');
    loader->AddVariable("ipSvtxdls", 'F');
    loader->AddVariable("ipSvtxdls2d", 'F');
    loader->AddVariable("ipSvtxm", 'F');
    loader->AddVariable("ipSvtxmcorr", 'F');
    //loader->AddVariable("ipInSV", 'I');

    Double_t signalWeight     = 1.0;
    Double_t backgroundWeight = 1.0;
    TCut mycuts = "";
    // using these weights brings down the efficiency
    //loader->SetSignalWeightExpression("weight"); 
    //loader->SetBackgroundWeightExpression("weight");
    loader->PrepareTrainingAndTestTree(mycuts, "random"); 

    loader->AddSignalTree(signaltree);
    loader->AddBackgroundTree(backgroundtree);
    
    TString opts = "!H:!V:NTrees=1000:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20";
    factory->BookMethod(loader, TMVA::Types::kBDT, "BDT", opts);
    TString opts_bdtg = "!H:!V:NTrees=1000:MinNodeSize=2.5%:MaxDepth=3:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20";
    factory->BookMethod(loader, TMVA::Types::kBDT, "BDTG", opts_bdtg);

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    outputFile->Close();

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;

    delete factory;
    delete loader;
}