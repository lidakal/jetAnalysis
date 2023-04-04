#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TMVA/Reader.h"

void test_TMVA()
{
    // Load data
    // TString label = "qcd_bjet";
    TString label = "ttbar_highPU";
    // TString fname = "./saved_models/TMVA.root";
    // TString fname = "./data_root_ttbar_highPU_30_pt_700/data.root";
    TString fname = "./data_root_" + label + "_30_pt_700/data.root";
    TFile *fin = new TFile(fname);
    // TTree *testTree = (TTree *) fin->Get("dataset/TestTree");
    TTree *testTree = (TTree *) fin->Get("val");

    // Load the TMVA model
    TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

    Float_t trkIp3dSig_bdt;
    Float_t trkIp2dSig_bdt;
    Float_t trkDistToAxis_bdt;
    Float_t svtxdls_bdt;
    Float_t svtxdls2d_bdt;
    Float_t svtxm_bdt;
    Float_t svtxmcorr_bdt;
    Float_t svtxnormchi2_bdt;
    Float_t svtxNtrk_bdt;
    Float_t svtxTrkPtOverSv_bdt;
    Float_t jtpt_bdt;

    Int_t classID_bdt;

    Double_t trkIp3dSig_tree;
    Double_t trkIp2dSig_tree;
    Double_t trkDistToAxis_tree;
    Double_t svtxdls_tree;
    Double_t svtxdls2d_tree;
    Double_t svtxm_tree;
    Double_t svtxmcorr_tree;
    Double_t svtxnormchi2_tree;
    Double_t svtxNtrk_tree;
    Double_t svtxTrkPtOverSv_tree;
    Double_t jtpt_tree;

    Long64_t classID_tree;
    Float_t weight_tree;

    std::vector<TString> variable_names_ = {"trkIp3dSig", "trkIp2dSig", "trkDistToAxis",
                                            "svtxdls", "svtxdls2d", "svtxm", "svtxmcorr",
                                            "svtxnormchi2", "svtxNtrk", "svtxTrkPtOverSv",
                                            // "jtpt"
                                            };

    std::map<TString, float *> variables_bdt;
    variables_bdt["trkIp3dSig"] = &trkIp3dSig_bdt;
    variables_bdt["trkIp2dSig"] = &trkIp2dSig_bdt;
    variables_bdt["trkDistToAxis"] = &trkDistToAxis_bdt;
    variables_bdt["svtxdls"] = &svtxdls_bdt;
    variables_bdt["svtxdls2d"] = &svtxdls2d_bdt;
    variables_bdt["svtxm"] = &svtxm_bdt;
    variables_bdt["svtxmcorr"] = &svtxmcorr_bdt;
    variables_bdt["svtxnormchi2"] = &svtxnormchi2_bdt;
    variables_bdt["svtxNtrk"] = &svtxNtrk_bdt;
    variables_bdt["svtxTrkPtOverSv"] = &svtxTrkPtOverSv_bdt;
    variables_bdt["jtpt"] = &jtpt_bdt;

    std::map<TString, double *> variables_tree;
    variables_tree["trkIp3dSig"] = &trkIp3dSig_tree;
    variables_tree["trkIp2dSig"] = &trkIp2dSig_tree;
    variables_tree["trkDistToAxis"] = &trkDistToAxis_tree;
    variables_tree["svtxdls"] = &svtxdls_tree;
    variables_tree["svtxdls2d"] = &svtxdls2d_tree;
    variables_tree["svtxm"] = &svtxm_tree;
    variables_tree["svtxmcorr"] = &svtxmcorr_tree;
    variables_tree["svtxnormchi2"] = &svtxnormchi2_tree;
    variables_tree["svtxNtrk"] = &svtxNtrk_tree;
    variables_tree["svtxTrkPtOverSv"] = &svtxTrkPtOverSv_tree;
    variables_tree["jtpt"] = &jtpt_tree;

    for (TString var : variable_names_) {
        // std::cout << "var: " << var << std::endl;
        reader->AddVariable(var, variables_bdt[var]);
        testTree->SetBranchAddress(var, variables_tree[var]);
    }
    testTree->SetBranchAddress("class", &classID_tree);
    // testTree->SetBranchAddress("weight", &weight_tree);

    TString weightfile = label + "_dataloader/weights/TMVAClassification_BDTG.weights.xml";
    reader->BookMVA("BDTG", weightfile);

    // Decalre WP
    float wp = -0.3;
    // float wp = 0.;

    // Get WP effS and effB
    float totalS = 0;
    float totalB = 0;
    float passS = 0;
    float passB = 0;

    // Fill histo for roc curve
    TH1F *h_discr_s = new TH1F("h_discr_s", "x=discr value, y=events with that value", 40, -1., 1.);
    TH1F *h_discr_b = new TH1F("h_discr_b", "x=discr value, y=events with that value", 40, -1., 1.);

    for (Long64_t ient = 0; ient < testTree->GetEntries(); ient++) {
        testTree->GetEntry(ient);

        weight_tree = 1.;

        // [DEBUG]
        // if (ient > 0) break;

        trkIp3dSig_bdt = trkIp3dSig_tree;
        trkIp2dSig_bdt = trkIp2dSig_tree;
        trkDistToAxis_bdt = trkDistToAxis_tree;
        svtxdls_bdt = svtxdls_tree;
        svtxdls2d_bdt = svtxdls2d_tree;
        svtxm_bdt = svtxm_tree;
        svtxmcorr_bdt = svtxmcorr_tree;
        svtxnormchi2_bdt = svtxnormchi2_tree;
        svtxNtrk_bdt = svtxNtrk_tree;
        svtxTrkPtOverSv_bdt = svtxTrkPtOverSv_tree;
        jtpt_bdt = jtpt_tree;

        classID_bdt = classID_tree;
        
        bool pass = false;
        float proba = reader->EvaluateMVA("BDTG");
        // std::cout << proba << std::endl;

        if (proba > wp) {
            pass = true;
        }

        if (classID_bdt == 1) {
            totalS += weight_tree;
            if (pass) passS += weight_tree;
            h_discr_s->Fill(proba, weight_tree);
        } else {
            totalB += weight_tree;
            if (pass) passB += weight_tree;
            h_discr_b->Fill(proba, weight_tree);
        }
    } // end signal tree loop

    // Save the histos
    TString foutName = "./saved_models/" + label + "_roc.root";
    TFile *fout = new TFile(foutName, "recreate");
    
    for (auto h : {h_discr_s, h_discr_b}) {
        h->Write();
    }
    fout->Close();
    delete fout;

    std::cout << "for wp : " << wp 
              << "\n\tsignal efficiency = " << (float) passS / totalS
              << "\n\tbkg rejection = " << 1. - ((float) passB / totalB)
              << std::endl;
}