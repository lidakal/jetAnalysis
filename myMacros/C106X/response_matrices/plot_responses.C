/* need an ntuple with aggregated gen and non aggregated reco 
   to compare with aggregated reco*/

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH2F.h"

#include "../tTree.h"
#include "calc_utils.h"

#include <iostream>
#include <cmath>

void plot_responses()
{
    // Setup 
    float ptMin = 50.;
    float ptMax = 80.;

    // Float_t ptMin = 30.;
    // Float_t ptMax = 700.;

    Float_t lnktMin = 0.;
    Float_t jetR = 0.4;

    // TString indir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    // TString label = "HiForestMiniAOD_HighPU_50000events_conmatch_newVars_truth";
    // TString label = "HiForestMiniAOD_HighPU_10000events_conmatch_bdt44_sig_vs_bkg";
    // TString label = "HiForestMiniAOD_HighPU_50000events_conmatch_aggrGenNoReco";
    // TString label = "HiForestMiniAOD_LowPU_10000events_conmatch_newVars_truth";

    TString label = "aggrGenNoReco";
    TString indir = "/data_CMS/cms/kalipoliti/ttbarMC/highPU/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";

    TString odir = "./histos_ttbar_highPU/";
    TString fout = odir + label + "_" + ptMin + "_pt_" + ptMax + "_responses.root";    

    // Load tree
    tTree t = tTree(fin);

    // Activate branches of interest
    std::vector<TString> activeBranches = {"nvtx",
                                           "nref", "jteta", "jtphi", "jtpt", "jtHadFlav",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb", 
                                           "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt2Pt", "sjt2Eta", "sjt2Phi", 
                                           "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", "rsjt2Pt", "rsjt2Eta", "rsjt2Phi",
                                           "jtmB", "refmB"
                                           };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches, 1);

    // Create new file to store signal and background trees
    TFile *foutPtr = new TFile(fout, "recreate");
    std::cout << "(Re)creating " << fout << " file." << std::endl;

    // ln(0.4/rg)
    Int_t x1bins = 16*2;
    Float_t x1min = 0.;
    Float_t x1max = 4.;

    // mB
    Int_t x2bins = 25;
    Float_t x2min = 0.;
    Float_t x2max = 5.;
    
    TH2F *h_response_rg = new TH2F("h_response_rg", "x=reco rg, y=ref rg", x1bins, x1min, x1max, x1bins, x1min, x1max);
    TH2F *h_response_rg_bjet = new TH2F("h_response_rg_bjet", "x=reco rg, y=ref rg", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *h_response_mb = new TH2F("h_response_mb", "x=reco mb, y=ref mb", x2bins, x2min, x2max, x2bins, x2min, x2max);
    TH2F *h_response_mb_bjet = new TH2F("h_response_mb_bjet", "x=reco mb, y=ref mb", x2bins, x2min, x2max, x2bins, x2min, x2max);

    int counts = 0;
    for (Long64_t ient = 0; ient < t.GetEntries(); ient++) {
        // for debugging purposes 
        //if (ient < 217316) continue;
        // if (ient > 0) break;
            
        // Show progress
        if (ient % 100000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        t.GetEntry(ient);
        for (Int_t ijet = 0; ijet < t.nref; ijet++) {
            Float_t jteta = t.jteta[ijet];
            Float_t jtpt = t.jtpt[ijet];
            Int_t jtHadFlav = t.jtHadFlav[ijet];
            Float_t deepFlavour_discr = t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_bb[ijet] + t.discr_deepFlavour_lepb[ijet];

            bool skipJet = false;
            skipJet |= (std::abs(jteta) > 2);
            skipJet |= (jtpt < ptMin || jtpt > ptMax);
            bool isBjet = (jtHadFlav == 5);
            skipJet |= (!isBjet);
            bool passBtag = (deepFlavour_discr > 0.9);
            skipJet |= (!passBtag);

            if (skipJet) continue;

            // Calculate quantities
            Float_t sjt1Eta = t.sjt1Eta[ijet];
            Float_t sjt1Phi = t.sjt1Phi[ijet];
            Float_t sjt1Pt = t.sjt1Pt[ijet];

            Float_t sjt2Eta = t.sjt2Eta[ijet];
            Float_t sjt2Phi = t.sjt2Phi[ijet];
            Float_t sjt2Pt = t.sjt2Pt[ijet];

            Float_t rsjt1Eta = t.rsjt1Eta[ijet];
            Float_t rsjt1Phi = t.rsjt1Phi[ijet];
            Float_t rsjt1Pt = t.rsjt1Pt[ijet];

            Float_t rsjt2Eta = t.rsjt2Eta[ijet];
            Float_t rsjt2Phi = t.rsjt2Phi[ijet];
            Float_t rsjt2Pt = t.rsjt2Pt[ijet];

            Float_t rg = -1.;
            Float_t kt = -1.;
            Float_t zg = -1.;

            Float_t logrg = -1.;
            Float_t logkt = -10.;

            Float_t rg_gen = -1.;
            Float_t kt_gen = -1.;
            Float_t zg_gen = -1.;

            Float_t logrg_gen = -1.;
            Float_t logkt_gen = -10.;

            // Calculate rg, kt only for 2 prong jets
            if (sjt2Pt > 0.) {
                rg = calc_rg(sjt1Eta, sjt1Phi, sjt1Pt, sjt2Eta, sjt2Phi, sjt2Pt);
                kt = sjt2Pt * rg;
                zg = sjt2Pt / (sjt2Pt + sjt2Pt);

                logrg = std::log(jetR/rg);
                logkt = std::log(kt);
            }

            if (rsjt2Pt > 0.) {
                rg_gen = calc_rg(rsjt1Eta, rsjt1Phi, rsjt1Pt, rsjt2Eta, rsjt2Phi, rsjt2Pt);
                kt_gen = rsjt2Pt * rg_gen;
                zg_gen = rsjt2Pt / (rsjt2Pt + rsjt2Pt);

                logrg_gen = std::log(jetR/rg_gen);
            }

            // if (logkt < 0.) continue;

            Float_t mB = t.jtmB[ijet];
            Float_t mB_gen = t.refmB[ijet];

            // Fill histos
            h_response_rg->Fill(logrg, logrg_gen);
            h_response_mb->Fill(mB, mB_gen);
            if (isBjet && passBtag) {
                h_response_rg_bjet->Fill(logrg, logrg_gen);
                h_response_mb_bjet->Fill(mB, mB_gen);
            }
            counts++;
        } // end jet loop
    } // end entry loop
    std::cout << " n selected jets " << counts << std::endl;
    for (auto h : {
                   h_response_rg, h_response_rg_bjet,
                //    h_response_mb, h_response_mb_bjet
                   }) {
        h->Write();
    }

    foutPtr->Close();
    delete foutPtr;         
}