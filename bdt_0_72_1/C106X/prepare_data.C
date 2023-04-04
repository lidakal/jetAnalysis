#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "tTree.h"

#include <iostream>

void prepare_data()
{
    // Setup 
    float ptMin = 30.;
    float ptMax = 700.;

    const float missing_value = -1000.;

    // TString indir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    // TString label = "HiForestMiniAOD_HighPU_50000events_conmatch_newVars_truth";

    // TString indir = "/data_CMS/cms/kalipoliti/ttbarMC/highPU/aggrGenNoReco/";
    // TString label = "ttbar_highPU";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/bjet/aggrGenNoReco/";
    TString label = "qcd_bjet";
    TString fin = indir + "merged_HiForestMiniAOD.root";

    TString odir = "./ntuples/";
    TString fout = odir + label + "_" + ptMin + "_pt_" + ptMax + ".root";    

    // Load tree
    tTree t = tTree(fin);

    // Activate branches of interest
    std::vector<TString> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb", 
                                           "ntrk", "trkJetId", "trkSvtxId", "trkPt", "trkEta", "trkPhi",
                                           "trkIp3d", "trkIp3dSig", "trkIp2d", "trkIp2dSig", "trkDistToAxis", "trkDistToAxisSig", "trkDz",
                                           "trkMatchSta", "trkPdgId",
                                           "nsvtx", "svtxJetId", "svtxNtrk", "svtxm", "svtxmcorr", "svtxpt",
                                           "svtxdl", "svtxdls", "svtxdl2d", "svtxdls2d", "svtxnormchi2",
                                           "weight"
                                           };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches, 1);

    // Create new file to store signal and background trees
    TFile *foutPtr = new TFile(fout, "recreate");
    std::cout << "(Re)creating " << fout << " file." << std::endl;

    TTree *TreeS = new TTree("TreeS", "Signal tree for MVA");
    TTree *TreeB = new TTree("TreeB", "Background tree for MVA");
    TTree *TreePU = new TTree("TreePU", "Pile-up tree for MVA");

    Float_t trkIp3dSig;
    Float_t trkIp2dSig;
    Float_t trkDistToAxis;
    Float_t trkDz;
    Float_t trkPtOverJet;
    Int_t trkPdgId;
    Int_t trkIsLepton;
    Int_t trkInSV;

    Float_t svtxdls;
    Float_t svtxdls2d;
    Float_t svtxm;
    Float_t svtxmcorr;
    Float_t svtxnormchi2;
    Float_t svtxNtrk;
    Float_t svtxTrkPtOverSv;

    Float_t jtpt;
    Float_t weight;

    TreeS->Branch("trkIp3dSig", &trkIp3dSig, "trkIp3dSig/F");
    TreeS->Branch("trkIp2dSig", &trkIp2dSig, "trkIp2dSig/F");
    TreeS->Branch("trkDistToAxis", &trkDistToAxis, "trkDistToAxis/F");
    TreeS->Branch("trkDz", &trkDz, "trkDz/F");
    TreeS->Branch("trkPtOverJet", &trkPtOverJet, "trkPtOverJet/F");
    TreeS->Branch("trkPdgId", &trkPdgId, "trkPdgId/I");
    TreeS->Branch("trkIsLepton", &trkIsLepton, "trkIsLepton/I");
    TreeS->Branch("trkInSV", &trkInSV, "trkInSV/I");
    TreeS->Branch("svtxdls", &svtxdls, "svtxdls/F");
    TreeS->Branch("svtxdls2d", &svtxdls2d, "svtxdls2d/F");
    TreeS->Branch("svtxm", &svtxm, "svtxm/F");
    TreeS->Branch("svtxmcorr", &svtxmcorr, "svtxmcorr/F");
    TreeS->Branch("svtxnormchi2", &svtxnormchi2, "svtxnormchi2/F");
    TreeS->Branch("svtxNtrk", &svtxNtrk, "svtxNtrk/F");
    TreeS->Branch("svtxTrkPtOverSv", &svtxTrkPtOverSv, "svtxTrkPtOverSv/F");
    TreeS->Branch("jtpt", &jtpt, "jtpt/F");
    TreeS->Branch("weight", &weight, "weight/F");

    TreeB->Branch("trkIp3dSig", &trkIp3dSig, "trkIp3dSig/F");
    TreeB->Branch("trkIp2dSig", &trkIp2dSig, "trkIp2dSig/F");
    TreeB->Branch("trkDistToAxis", &trkDistToAxis, "trkDistToAxis/F");
    TreeB->Branch("trkDz", &trkDz, "trkDz/F");
    TreeB->Branch("trkPtOverJet", &trkPtOverJet, "trkPtOverJet/F");
    TreeB->Branch("trkPdgId", &trkPdgId, "trkPdgId/I");
    TreeB->Branch("trkIsLepton", &trkIsLepton, "trkIsLepton/I");
    TreeB->Branch("trkInSV", &trkInSV, "trkInSV/I");
    TreeB->Branch("svtxdls", &svtxdls, "svtxdls/F");
    TreeB->Branch("svtxdls2d", &svtxdls2d, "svtxdls2d/F");
    TreeB->Branch("svtxm", &svtxm, "svtxm/F");
    TreeB->Branch("svtxmcorr", &svtxmcorr, "svtxmcorr/F");
    TreeB->Branch("svtxnormchi2", &svtxnormchi2, "svtxnormchi2/F");
    TreeB->Branch("svtxNtrk", &svtxNtrk, "svtxNtrk/F");
    TreeB->Branch("svtxTrkPtOverSv", &svtxTrkPtOverSv, "svtxTrkPtOverSv/F");
    TreeB->Branch("jtpt", &jtpt, "jtpt/F");
    TreeB->Branch("weight", &weight, "weight/F");

    TreePU->Branch("trkIp3dSig", &trkIp3dSig, "trkIp3dSig/F");
    TreePU->Branch("trkIp2dSig", &trkIp2dSig, "trkIp2dSig/F");
    TreePU->Branch("trkDistToAxis", &trkDistToAxis, "trkDistToAxis/F");
    TreePU->Branch("trkDz", &trkDz, "trkDz/F");
    TreePU->Branch("trkPtOverJet", &trkPtOverJet, "trkPtOverJet/F");
    TreePU->Branch("trkPdgId", &trkPdgId, "trkPdgId/I");
    TreePU->Branch("trkIsLepton", &trkIsLepton, "trkIsLepton/I");
    TreePU->Branch("trkInSV", &trkInSV, "trkInSV/I");
    TreePU->Branch("svtxdls", &svtxdls, "svtxdls/F");
    TreePU->Branch("svtxdls2d", &svtxdls2d, "svtxdls2d/F");
    TreePU->Branch("svtxm", &svtxm, "svtxm/F");
    TreePU->Branch("svtxmcorr", &svtxmcorr, "svtxmcorr/F");
    TreePU->Branch("svtxnormchi2", &svtxnormchi2, "svtxnormchi2/F");
    TreePU->Branch("svtxNtrk", &svtxNtrk, "svtxNtrk/F");
    TreePU->Branch("svtxTrkPtOverSv", &svtxTrkPtOverSv, "svtxTrkPtOverSv/F");
    TreePU->Branch("jtpt", &jtpt, "jtpt/F");
    TreePU->Branch("weight", &weight, "weight/F");
    
    int sig = 0;
    int bkg = 0;
    int pu = 0;

    for (Long64_t ient = 0; ient < t.GetEntries(); ient++) {
        // for debugging purposes 
        //if (ient < 217316) continue;
        // if (ient > 1) break;
            
        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        t.GetEntry(ient);
        weight = t.weight;
        for (Int_t itrk = 0; itrk < t.ntrk; itrk++) {
            int ijet = t.trkJetId[itrk];
            bool skipJet = false;
            skipJet |= (std::abs(t.jteta[ijet]) > 2);
            skipJet |= (t.jtpt[ijet] < ptMin || t.jtpt[ijet] > ptMax);
            bool isBjet = (t.jtHadFlav[ijet] == 5);
            skipJet |= (!isBjet);
            bool passBtag = (t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_bb[ijet] + t.discr_deepFlavour_lepb[ijet]) > 0.9;
            passBtag &= (t.discr_deepFlavour_bb[ijet] < 0.2);
            skipJet |= (!passBtag);

            if (skipJet) continue;

            jtpt = t.jtpt[ijet];

            Float_t trkPt = t.trkPt[itrk];
            trkIp3dSig = t.trkIp3dSig[itrk];
            if (trkIp3dSig != trkIp3dSig) trkIp3dSig = missing_value; // nan
            trkIp2dSig = t.trkIp2dSig[itrk];
            if (trkIp2dSig != trkIp2dSig) trkIp2dSig = missing_value; // nan
            trkDistToAxis = t.trkDistToAxis[itrk];
            if (trkDistToAxis != trkDistToAxis) trkDistToAxis = missing_value; // nan
            trkDz = t.trkDz[itrk];
            if (trkDz != trkDz) trkDz = missing_value; // nan

            if (std::abs(trkDz) > 0.6) continue;

            trkPtOverJet = trkPt / jtpt;
            trkPdgId = t.trkPdgId[itrk]; 
            if (std::abs(trkPdgId) == 11 || std::abs(trkPdgId) == 13) {
                trkIsLepton = 1;
            } else {
                trkIsLepton = 0;
            }
        
            int trkSvtxId = t.trkSvtxId[itrk];
            trkInSV = int(trkSvtxId >= 0);
            if (trkSvtxId >= 0) {
                svtxdls = t.svtxdls[trkSvtxId];
                svtxdls2d = t.svtxdls2d[trkSvtxId];
                svtxm = t.svtxm[trkSvtxId];
                svtxmcorr = t.svtxmcorr[trkSvtxId];
                svtxNtrk = t.svtxNtrk[trkSvtxId];
                svtxnormchi2 = t.svtxnormchi2[trkSvtxId];
                svtxTrkPtOverSv = trkPt / t.svtxpt[trkSvtxId];
            } else {
                svtxdls = missing_value;
                svtxdls2d = missing_value;
                svtxm = missing_value;
                svtxmcorr = missing_value;
                svtxNtrk = missing_value;
                svtxnormchi2 = missing_value;
                svtxTrkPtOverSv = missing_value;
            }

            Int_t sta = t.trkMatchSta[itrk];
            if (sta >= 100) {
                TreeS->Fill();
                sig++;
            } else {
                if (sta == 1) {
                    TreeB->Fill();
                    bkg++;
                } else {
                    // TreeB->Fill();
                    // TreeS->Fill();
                    TreePU->Fill();
                    pu++;
                }
            }

        } // end track loop
    } // end entry loop

    int total = sig + bkg + pu;

    std::cout << "sig / total : " << (float) sig / total << std::endl;
    std::cout << "bkg / total : " << (float) bkg / total << std::endl;
    std::cout << "pu / total : " << (float) pu / total << std::endl;

    std::cout << "bkg / sig : " << (float) bkg / sig << std::endl; 
    std::cout << "pu / sig : " << (float) pu / sig << std::endl; 

    TreeS->Write("", TObject::kOverwrite);
    TreeB->Write("", TObject::kOverwrite);
    TreePU->Write("", TObject::kOverwrite);

    foutPtr->Close();
    delete foutPtr;         
}