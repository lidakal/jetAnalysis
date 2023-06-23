#include "tTree.h"

void plot_rg_poster()
{
    // TString label = "aggrGenNoReco_withY";
    // TString label = "noAggr_withY";
    TString label = "aggrTMVA_withY";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/bjet/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    tTree tree(fin);
    std::vector<TString> activeBranches = {"nref",
                                           "jtpt", "jteta", "jtHadFlav",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                           "rsjt1Phi", "rsjt1Y", "rsjt1Pt", "rsjt1Eta",
                                           "rsjt2Phi", "rsjt2Y", "rsjt2Pt", "rsjt2Eta",
                                           "sjt1Phi", "sjt1Y", "sjt1Pt", "sjt1Eta",
                                           "sjt2Phi", "sjt2Y", "sjt2Pt", "sjt2Eta",
                                           };

    tree.SetBranchStatus("*", 0);
    tree.SetBranchStatus(activeBranches, 1);

    double ptMin = 80.;
    double ptMax = 100.;

    double jetR = 0.4;

    Int_t x1bins = 16;
    Float_t x1min = 0.;
    Float_t x1max = 4.;

    Int_t x2bins = 24;
    Float_t x2min = -3.;
    Float_t x2max = 3.;

    // Int_t x1bins = 10;
    // Float_t x1min = 0.;
    // Float_t x1max = 0.5;

    TH1F *hBtag_gen = new TH1F("hBtag_gen", "x=log(1/rg)", x1bins, x1min, x1max);
    TH1F *hL_gen = new TH1F("hL_gen", "x=log(1/rg)", x1bins, x1min, x1max);

    TH1F *hBtag = new TH1F("hBtag", "x=log(1/rg)", x1bins, x1min, x1max);
    // TH1F *hL = new TH1F("hL", "x=log(1/rg)", x1bins, x1min, x1max);
    TH2F *hBtag_rgkt = new TH2F("hBtag_rgkt", "x=log(1/rg), y=log(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);
    TH2F *hBtag_rgkt_gen = new TH2F("hBtag_rgkt_gen", "x=log(1/rg), y=log(kt)", x1bins, x1min, x1max, x2bins, x2min, x2max);

    TH2F *hBtag_2d = new TH2F("hBtag_2d", "x=reco, y=gen", x1bins, x1min, x1max, x1bins, x1min, x1max);

    for (Long64_t ient = 0; ient < tree.GetEntries(); ient++) {
        if (ient > 1000000) break;

        if ((ient % 1000000) == 0) {
                std::cout << "ient = " << ient << std::endl;
            }

        tree.GetEntry(ient);
        for (Int_t ijet = 0; ijet < tree.nref; ijet++) {
            if (std::abs(tree.jteta[ijet]) > 2) continue;

            Float_t dphi_gen = std::acos(std::cos(tree.rsjt1Phi[ijet] - tree.rsjt2Phi[ijet]));
            Float_t dy_gen = tree.rsjt1Y[ijet] - tree.rsjt2Y[ijet];

            Float_t rg_gen = std::sqrt((dy_gen*dy_gen) + (dphi_gen*dphi_gen));
            Float_t kt_gen = tree.rsjt2Pt[ijet] * rg_gen;

            Float_t logrg_gen = std::log(jetR / rg_gen);
            Float_t logkt_gen = std::log(kt_gen);

            Float_t dphi = std::acos(std::cos(tree.sjt1Phi[ijet] - tree.sjt2Phi[ijet]));
            Float_t dy = tree.sjt1Y[ijet] - tree.sjt2Y[ijet];

            Float_t rg = std::sqrt((dy*dy) + (dphi*dphi));
            Float_t kt = tree.sjt2Pt[ijet] * rg;

            Float_t logrg = std::log(jetR / rg);
            Float_t logkt = std::log(kt);

            bool passTag = (tree.discr_deepFlavour_b[ijet] + tree.discr_deepFlavour_bb[ijet] + tree.discr_deepFlavour_lepb[ijet]) > 0.9;
            
            if (tree.rsjt2Pt[ijet] > 0) {
                if (tree.jtpt[ijet] > ptMin && tree.jtpt[ijet] < ptMax && kt_gen>1) {
                    if (tree.jtHadFlav[ijet] == 0) {
                        hL_gen->Fill(logrg_gen);
                    } else if (tree.jtHadFlav[ijet] == 5 && passTag) {
                        hBtag_gen->Fill(logrg_gen);
                    }
                }
                if (tree.jtpt[ijet] > ptMin && tree.jtpt[ijet] < ptMax && tree.jtHadFlav[ijet] == 5 && passTag) {
                    hBtag_rgkt_gen->Fill(logrg_gen, logkt_gen);
                }
            }

            if (tree.sjt2Pt[ijet] > 0) {
                if (tree.jtpt[ijet] > ptMin && tree.jtpt[ijet] < ptMax && kt>1) {
                    if (tree.jtHadFlav[ijet] == 0) {
                        // hL->Fill(rg);
                    } else if (tree.jtHadFlav[ijet] == 5 && passTag) {
                        hBtag->Fill(logrg);
                    }
                }
                if (tree.jtpt[ijet] > ptMin && tree.jtpt[ijet] < ptMax && tree.jtHadFlav[ijet] == 5 && passTag) {
                    hBtag_rgkt->Fill(logrg, logkt);
                }
            }

            if (tree.sjt2Pt[ijet] > 0 && tree.rsjt2Pt[ijet] > 0 && tree.jtHadFlav[ijet] == 5 && passTag) {
                hBtag_2d->Fill(logrg, logrg_gen);
            }
        } // end jet loop
    } // end entry loop

    TString foutName = "./histos_for_poster/" + label + ".root";
    TFile *fout = new TFile(foutName, "recreate");

    for (auto h : {hBtag, hBtag_gen, hL_gen}) {
        h->Write();
    }
    for (auto h : {hBtag_2d, hBtag_rgkt, hBtag_rgkt_gen}) {
        h->Write();
    }

    fout->Close();
    delete fout;
}