
void train_TMVA() 
{
    TMVA::Tools::Instance();

    // Choose methods
    std::map<std::string,int> Use;
    Use["BDTG"] = 1; // uses Gradient Boost

    std::cout << std::endl;
    std::cout << "==> Start TMVAClassification" << std::endl;

    TFile *input(0);
    // TString label = "ttbar_highPU";
    TString label = "qcd_bjet";
    TString fname = "./data_root_" + label + "_30_pt_700/data.root";
    if (!gSystem->AccessPathName( fname )) {
        input = TFile::Open( fname ); // check if file in local directory exists
    }
    if (!input) {
        std::cout << "ERROR: could not open data file" << std::endl;
        exit(1);
    }
    std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;


    TTree *signalTree     = (TTree*)input->Get("train_sig");
    TTree *background     = (TTree*)input->Get("train_bkg");

    TString outfileName( "./saved_models/" + label + "_TMVA.root" );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                            "!V:!Silent:Color:!DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

    TMVA::DataLoader *dataloader=new TMVA::DataLoader(label + "_dataloader");
    dataloader->AddVariable( "trkIp3dSig", 'F' );
    dataloader->AddVariable( "trkIp2dSig", 'F' );
    dataloader->AddVariable( "trkDistToAxis", 'F' );
    // dataloader->AddVariable( "trkDz", 'F' );
    // dataloader->AddVariable( "trkIsLepton", 'I' );
    // dataloader->AddVariable( "trkInSV" , 'I' );
    dataloader->AddVariable( "svtxdls", 'F' );
    dataloader->AddVariable( "svtxdls2d", 'F' );
    dataloader->AddVariable( "svtxm", 'F' );
    dataloader->AddVariable( "svtxmcorr", 'F' );
    dataloader->AddVariable( "svtxnormchi2", 'F' );
    dataloader->AddVariable( "svtxNtrk", 'F' );
    dataloader->AddVariable( "svtxTrkPtOverSv", 'F' );
    dataloader->AddVariable( "jtpt", 'F' );

    // dataloader->AddSpectator( "trkIsLepton", 'I' );
    // dataloader->AddSpectator( "trkInSV", 'I' );
    // dataloader->AddSpectator( "jtpt", 'F' );

    Double_t signalWeight = 1.0;
    Double_t backgroundWeight = 1.0;

    dataloader->AddSignalTree    ( signalTree,     signalWeight );
    dataloader->AddBackgroundTree( background, backgroundWeight );

    // TCut mycuts = "trkInSV==1"; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
    // TCut mycutb = "trkInSV==1"; // for example: TCut mycutb = "abs(var1)<0.5";

    TCut mycuts = "";
    TCut mycutb = mycuts;

    dataloader->PrepareTrainingAndTestTree( mycuts, mycutb,
        "nTrain_Signal=150000:nTrain_Background=150000:SplitMode=Random:NormMode=NumEvents:!V" );

    // Book methods
    if (Use["BDTG"]) // Gradient Boost
        factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG",
            "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=8" );

    // Train
    factory->TrainAllMethods();

    // Test
    factory->TestAllMethods();

    // Evaluate
    factory->EvaluateAllMethods();

    outputFile->Close();

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;

    delete factory;
    delete dataloader;

}