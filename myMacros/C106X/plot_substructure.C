#include "tTree.h"

void plot_substructure()
{
    // ---- Load tree
    TString label = "noAggr_withPNET";
    TString sample = "bjet";
    TString fin_name = "/data_CMS/cms/kalipoliti/qcdMC/" + sample + "/" + label + "/" + "merged_HiForestMiniAOD.root";
    TString fout_name = "./histos/" + sample + "_" + label + "_substructure.root"; 

    std::cout << "fin: " << fin_name << std::endl;
    tTree tree(fin_name);
    std::vector<TString> activeBranches = {"nref",
                                           "jtpt", "jteta", 
                                           "refpt", "jtHadFlav", "jtNbHad", "jtNcHad",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                           "discr_particleNet_BvsAll",
                                           "rsjt1Phi", "rsjt1Y", "rsjt1Pt", "rsjt1Eta",
                                           "rsjt2Phi", "rsjt2Y", "rsjt2Pt", "rsjt2Eta",
                                           "sjt1Phi", "sjt1Y", "sjt1Pt", "sjt1Eta",
                                           "sjt2Phi", "sjt2Y", "sjt2Pt", "sjt2Eta",
                                           "weight"
                                           };

    tree.SetBranchStatus("*", 0);
    tree.SetBranchStatus(activeBranches, 1);

    double ptMin = 100.;
    double ptMax = 120.;

    double jetR = 0.4;

    // ln(R/Rg) bins
    Int_t x1bins = 16;
    Float_t x1min = 0.;
    Float_t x1max = 4.;

    // ln(kT) bins
    Int_t x2bins = 24;
    Float_t x2min = -3.;
    Float_t x2max = 3.;

    // zg bins
    Int_t x3bins = 16;
    Float_t x3min = 0.1;
    Float_t x3max = 0.5;

    // --------- single b histograms ------------------
    TH2F *hSingleB_rg = new TH2F("hSingleB_rg", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hSingleB_rg_gen = new TH2F("hSingleB_rg_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hSingleB_rg_reco_vs_gen = new TH2F("hSingleB_rg_reco_vs_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *hSingleBtag_rg = new TH2F("hSingleBtag_rg", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hSingleBtag_rg_gen = new TH2F("hSingleBtag_rg_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hSingleBtag_rg_reco_vs_gen = new TH2F("hSingleBtag_rg_reco_vs_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *hSingleB_zg = new TH2F("hSingleB_zg", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hSingleB_zg_gen = new TH2F("hSingleB_zg_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hSingleB_zg_reco_vs_gen = new TH2F("hSingleB_zg_reco_vs_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x3bins, x3min, x3max);

    TH2F *hSingleBtag_zg = new TH2F("hSingleBtag_zg", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hSingleBtag_zg_gen = new TH2F("hSingleBtag_zg_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hSingleBtag_zg_reco_vs_gen = new TH2F("hSingleBtag_zg_reco_vs_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x3bins, x3min, x3max);

    // --------- bb histograms ------------------
    TH2F *hBB_rg = new TH2F("hBB_rg", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hBB_rg_gen = new TH2F("hBB_rg_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hBB_rg_reco_vs_gen = new TH2F("hBB_rg_reco_vs_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *hBBtag_rg = new TH2F("hBBtag_rg", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hBBtag_rg_gen = new TH2F("hBBtag_rg_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hBBtag_rg_reco_vs_gen = new TH2F("hBBtag_rg_reco_vs_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *hBB_zg = new TH2F("hBB_zg", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hBB_zg_gen = new TH2F("hBB_zg_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hBB_zg_reco_vs_gen = new TH2F("hBB_zg_reco_vs_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x3bins, x3min, x3max);

    TH2F *hBBtag_zg = new TH2F("hBBtag_zg", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hBBtag_zg_gen = new TH2F("hBBtag_zg_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hBBtag_zg_reco_vs_gen = new TH2F("hBBtag_zg_reco_vs_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x3bins, x3min, x3max);

    // --------- c histograms ------------------
    TH2F *hC_rg = new TH2F("hC_rg", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hC_rg_gen = new TH2F("hC_rg_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hC_rg_reco_vs_gen = new TH2F("hC_rg_reco_vs_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *hCtag_rg = new TH2F("hCtag_rg", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hCtag_rg_gen = new TH2F("hCtag_rg_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hCtag_rg_reco_vs_gen = new TH2F("hCtag_rg_reco_vs_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *hC_zg = new TH2F("hC_zg", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hC_zg_gen = new TH2F("hC_zg_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hC_zg_reco_vs_gen = new TH2F("hC_zg_reco_vs_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x3bins, x3min, x3max);

    TH2F *hCtag_zg = new TH2F("hCtag_zg", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hCtag_zg_gen = new TH2F("hCtag_zg_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hCtag_zg_reco_vs_gen = new TH2F("hCtag_zg_reco_vs_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x3bins, x3min, x3max);

    // --------- light histograms ------------------
    TH2F *hL_rg = new TH2F("hL_rg", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hL_rg_gen = new TH2F("hL_rg_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hL_rg_reco_vs_gen = new TH2F("hL_rg_reco_vs_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *hLtag_rg = new TH2F("hLtag_rg", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hLtag_rg_gen = new TH2F("hLtag_rg_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hLtag_rg_reco_vs_gen = new TH2F("hLtag_rg_reco_vs_gen", "x=log(r/rg), y=ln(kt)", x1bins, x1min, x1max, x1bins, x1min, x1max);

    TH2F *hL_zg = new TH2F("hL_zg", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hL_zg_gen = new TH2F("hL_zg_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hL_zg_reco_vs_gen = new TH2F("hL_zg_reco_vs_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x3bins, x3min, x3max);

    TH2F *hLtag_zg = new TH2F("hLtag_zg", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hLtag_zg_gen = new TH2F("hLtag_zg_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x2bins, x2min, x2max);
    TH2F *hLtag_zg_reco_vs_gen = new TH2F("hLtag_zg_reco_vs_gen", "x=zg, y=ln(kt)", x3bins, x3min, x3max, x3bins, x3min, x3max);

    // ===> 12 x 4 = 48 histograms to fill

    for (Long64_t ient = 0; ient < tree.GetEntries(); ient++) {
        // if (ient > 1000000) break;

        if ((ient % 1000000) == 0) {
                std::cout << "ient = " << ient << std::endl;
            }

        tree.GetEntry(ient);
        for (Int_t ijet = 0; ijet < tree.nref; ijet++) {
            if (std::abs(tree.jteta[ijet]) > 2) continue;
            if (tree.jtpt[ijet]<ptMin||tree.jtpt[ijet]>ptMax) continue;
            bool passTag = (tree.discr_particleNet_BvsAll[ijet] > 0.99);

            Float_t dphi, dphi_gen;
            Float_t dy, dy_gen;
            Float_t rg = -999.;
            Float_t rg_gen = -999.;
            Float_t logrg = -999.;
            Float_t logrg_gen = -999.;
            Float_t kt = -999.;
            Float_t kt_gen = -999.;
            Float_t logkt = -999.;
            Float_t logkt_gen = -999.;
            Float_t zg = -999.;
            Float_t zg_gen = -999.;

            Float_t weight = tree.weight;
            
            // Calculate gen variables
            if (tree.rsjt2Pt[ijet] > 0) {
                dphi_gen = std::acos(std::cos(tree.rsjt1Phi[ijet] - tree.rsjt2Phi[ijet]));
                dy_gen = tree.rsjt1Y[ijet] - tree.rsjt2Y[ijet];

                rg_gen = std::sqrt((dy_gen*dy_gen) + (dphi_gen*dphi_gen));
                kt_gen = tree.rsjt2Pt[ijet] * rg_gen;

                logrg_gen = std::log(jetR / rg_gen);
                logkt_gen = std::log(kt_gen);

                zg_gen = tree.rsjt2Pt[ijet] / (tree.rsjt2Pt[ijet] + tree.rsjt1Pt[ijet]);
            } 

            // Calculate reco variables
            if (tree.sjt2Pt[ijet] > 0) {
                dphi = std::acos(std::cos(tree.sjt1Phi[ijet] - tree.sjt2Phi[ijet]));
                dy = tree.sjt1Y[ijet] - tree.sjt2Y[ijet];

                rg = std::sqrt((dy*dy) + (dphi*dphi));
                kt = tree.sjt2Pt[ijet] * rg;

                logrg = std::log(jetR / rg);
                logkt = std::log(kt);

                zg = tree.sjt2Pt[ijet] / (tree.sjt2Pt[ijet] + tree.sjt1Pt[ijet]);
            }
            
            // Fill histograms 
            if (tree.jtNbHad[ijet]==1) {
                hSingleB_rg->Fill(logrg, logkt, weight);
                hSingleB_rg_gen->Fill(logrg_gen, logkt_gen, weight);
                hSingleB_rg_reco_vs_gen->Fill(logrg, logrg_gen, weight);

                hSingleB_zg->Fill(zg, logkt, weight);
                hSingleB_zg_gen->Fill(zg_gen, logkt_gen, weight);
                hSingleB_zg_reco_vs_gen->Fill(zg, zg_gen, weight);

                if (passTag) {
                    hSingleBtag_rg->Fill(logrg, logkt, weight);
                    hSingleBtag_rg_gen->Fill(logrg_gen, logkt_gen, weight);
                    hSingleBtag_rg_reco_vs_gen->Fill(logrg, logrg_gen, weight);

                    hSingleBtag_zg->Fill(zg, logkt, weight);
                    hSingleBtag_zg_gen->Fill(zg_gen, logkt_gen, weight);
                    hSingleBtag_zg_reco_vs_gen->Fill(zg, zg_gen, weight);
                }
            } else if (tree.jtNbHad[ijet]>1) {
                hBB_rg->Fill(logrg, logkt, weight);
                hBB_rg_gen->Fill(logrg_gen, logkt_gen, weight);
                hBB_rg_reco_vs_gen->Fill(logrg, logrg_gen, weight);

                hBB_zg->Fill(zg, logkt, weight);
                hBB_zg_gen->Fill(zg_gen, logkt_gen, weight);
                hBB_zg_reco_vs_gen->Fill(zg, zg_gen, weight);

                if (passTag) {
                    hBBtag_rg->Fill(logrg, logkt, weight);
                    hBBtag_rg_gen->Fill(logrg_gen, logkt_gen, weight);
                    hBBtag_rg_reco_vs_gen->Fill(logrg, logrg_gen, weight);

                    hBBtag_zg->Fill(zg, logkt, weight);
                    hBBtag_zg_gen->Fill(zg_gen, logkt_gen, weight);
                    hBBtag_zg_reco_vs_gen->Fill(zg, zg_gen, weight);
                }
            } else if (tree.jtNcHad[ijet]>=1) {
                hC_rg->Fill(logrg, logkt, weight);
                hC_rg_gen->Fill(logrg_gen, logkt_gen, weight);
                hC_rg_reco_vs_gen->Fill(logrg, logrg_gen, weight);

                hC_zg->Fill(zg, logkt, weight);
                hC_zg_gen->Fill(zg_gen, logkt_gen, weight);
                hC_zg_reco_vs_gen->Fill(zg, zg_gen, weight);

                if (passTag) {
                    hCtag_rg->Fill(logrg, logkt, weight);
                    hCtag_rg_gen->Fill(logrg_gen, logkt_gen, weight);
                    hCtag_rg_reco_vs_gen->Fill(logrg, logrg_gen, weight);

                    hCtag_zg->Fill(zg, logkt, weight);
                    hCtag_zg_gen->Fill(zg_gen, logkt_gen, weight);
                    hCtag_zg_reco_vs_gen->Fill(zg, zg_gen, weight);
                }
            } else {
                hL_rg->Fill(logrg, logkt, weight);
                hL_rg_gen->Fill(logrg_gen, logkt_gen, weight);
                hL_rg_reco_vs_gen->Fill(logrg, logrg_gen, weight);

                hL_zg->Fill(zg, logkt, weight);
                hL_zg_gen->Fill(zg_gen, logkt_gen, weight);
                hL_zg_reco_vs_gen->Fill(zg, zg_gen, weight);

                if (passTag) {
                    hLtag_rg->Fill(logrg, logkt, weight);
                    hLtag_rg_gen->Fill(logrg_gen, logkt_gen, weight);
                    hLtag_rg_reco_vs_gen->Fill(logrg, logrg_gen, weight);

                    hLtag_zg->Fill(zg, logkt, weight);
                    hLtag_zg_gen->Fill(zg_gen, logkt_gen, weight);
                    hLtag_zg_reco_vs_gen->Fill(zg, zg_gen, weight);
                }
            }
        } // jet loop
    } // event loop

    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {
        hSingleB_rg, hSingleB_rg_gen, hSingleB_rg_reco_vs_gen,
        hSingleBtag_rg, hSingleBtag_rg_gen, hSingleBtag_rg_reco_vs_gen,
        hSingleB_zg, hSingleB_zg_gen, hSingleB_zg_reco_vs_gen,
        hSingleBtag_zg, hSingleBtag_zg_gen, hSingleBtag_zg_reco_vs_gen,

        hBB_rg, hBB_rg_gen, hBB_rg_reco_vs_gen,
        hBBtag_rg, hBBtag_rg_gen, hBBtag_rg_reco_vs_gen,
        hBB_zg, hBB_zg_gen, hBB_zg_reco_vs_gen,
        hBBtag_zg, hBBtag_zg_gen, hBBtag_zg_reco_vs_gen,

        hC_rg, hC_rg_gen, hC_rg_reco_vs_gen,
        hCtag_rg, hCtag_rg_gen, hCtag_rg_reco_vs_gen,
        hC_zg, hC_zg_gen, hC_zg_reco_vs_gen,
        hCtag_zg, hCtag_zg_gen, hCtag_zg_reco_vs_gen,

        hL_rg, hL_rg_gen, hL_rg_reco_vs_gen,
        hLtag_rg, hLtag_rg_gen, hLtag_rg_reco_vs_gen,
        hL_zg, hL_zg_gen, hL_zg_reco_vs_gen,
        hLtag_zg, hLtag_zg_gen, hLtag_zg_reco_vs_gen,
    }) {
        h->Write();
    }
    fout->Close();
    delete fout;    
}