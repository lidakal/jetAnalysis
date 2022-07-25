#include "TFile.h"
#include "TTree.h"
#include "TCut.h"

void train_simple_bdt()
{
    TMVA::Tools::Instance(); 
    // open file and retrieve trees
    TFile *inputfile = new TFile("tmva_data.root");
    //TFile *inputfile = new TFile("/home/llr/cms/kalipoliti/CMSSW_9_4_10/src/HeavyIonsAnalysis/JetAnalysis/test/HiForestAOD.root");
    TTree *signaltree = (TTree *) inputfile->Get("TreeS");
    TTree *backgroundtree = (TTree *) inputfile->Get("TreeB");

    TFile *outputFile = new TFile("tmva_results.root", "RECREATE"); 

    // load trees into TMVA
    TMVA::DataLoader *loader = new TMVA::DataLoader("mydataloader");
    

    TMVA::Factory *factory = new TMVA::Factory("TMVAClassification",outputFile,"V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification"); 
    loader->AddVariable("ip3dSig", 'F');
    loader->AddVariable("inSV", 'I');

    Double_t signalWeight     = 1.0;
    Double_t backgroundWeight = 1.0;
    TCut mycuts = "";
    // using these weights brings down the efficiency
    //loader->SetSignalWeightExpression("weight"); 
    //loader->SetBackgroundWeightExpression("weight");
    loader->PrepareTrainingAndTestTree(mycuts, "random"); 

    loader->AddSignalTree(signaltree);
    loader->AddBackgroundTree(backgroundtree);
    

    factory->BookMethod(loader, TMVA::Types::kBDT, "BDT",
                        "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    factory->BookMethod( loader, TMVA::Types::kBDT, "BDTG",
                        "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    outputFile->Close();

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;

    delete factory;
    delete loader;
}