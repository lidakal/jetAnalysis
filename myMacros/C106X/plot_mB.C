#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TH3.h>

#include <limits>

#include "tTree.h"
#include "hist_utils.h"

void plot_mB()
{
    // ---- Load tree
    TString label = "aggrTMVA_withNb";
    TString sample = "dijet";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/" + sample + "/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    TString rootf_out = "./histos/qcd_" + sample + "_" + label + "_mB.root";

    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref", "nvtx",
                                             "refeta", "refpt", "jtHadFlav", "jtNbHad",
                                             "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", "rsjt1Y",
                                             "rsjt2Pt", "rsjt2Eta", "rsjt2Phi", "rsjt2Y",
                                             "jtpt", "jteta", "jtphi", 
                                             "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt1Y",
                                             "sjt2Pt", "sjt2Eta", "sjt2Phi", "sjt2Y",
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "jtmB", "refmB",
                                             "weight"
                                             };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    // ---- Create the histograms 

    // mB ratio
    Int_t x1bins = 20;
    Float_t x1min = 0.;
    Float_t x1max = 2.;

    // mB
    Int_t x2bins = 10;
    Float_t x2min = 0.;
    Float_t x2max = 10.;

    // nvtx
    Int_t y1bins = 20;
    Float_t y1min = 0.;
    Float_t y1max = 100.;

    // ln(kt)
    Int_t y2bins = 40;
    Float_t y2min = -5.;
    Float_t y2max = 5.;

    // jet pt
    Int_t z1bins = 27 * 2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    // reco level
    TH3D *hB_mB = new TH3D("hB_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBtag_mB = new TH3D("hBtag_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hSingleB_mB = new TH3D("hSingleB_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hSingleBtag_mB = new TH3D("hSingleBtag_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hBB_mB = new TH3D("hBB_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBBtag_mB = new TH3D("hBBtag_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hL_mB = new TH3D("hL_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hLtag_mB = new TH3D("hLtag_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hC_mB = new TH3D("hC_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hCtag_mB = new TH3D("hCtag_mB", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hB_mB_ratio = new TH3D("hB_mB_ratio", "x=mBreco/mBgen, y=logkt, z=jtpt", x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBtag_mB_ratio = new TH3D("hBtag_mB_ratio", "x=mBreco/mBgen, y=logkt, z=jtpt", x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    // gen level
    TH3D *hB_mB_gen = new TH3D("hB_mB_gen", "x=mB_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBtag_mB_gen = new TH3D("hBtag_mB_gen", "x=mB_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hSingleB_mB_gen = new TH3D("hSingleB_mB_gen", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hSingleBtag_mB_gen = new TH3D("hSingleBtag_mB_gen", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hBB_mB_gen = new TH3D("hBB_mB_gen", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBBtag_mB_gen = new TH3D("hBBtag_mB_gen", "x=mBreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hL_mB_gen = new TH3D("hL_mB_gen", "x=mB_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hLtag_mB_gen = new TH3D("hLtag_mB_gen", "x=mB_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hC_mB_gen = new TH3D("hC_mB_gen", "x=mB_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hCtag_mB_gen = new TH3D("hCtag_mB_gen", "x=mB_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    std::cout << "Creating histograms ..." << std::endl;
    Long64_t nentries = t.GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        t.GetEntry(ient);  

        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
            // std::cout << "nref = " << t.nref << std::endl;
            // std::cout << "weight = " << t.weight << std::endl;
        }

        // Choose nb of events
        const Long64_t total_events = 20000000;
        if (ient > total_events) break;
		// if (ient > 100) break;

        const Float_t jetR = 0.4;

        for (int ijet = 0; ijet < t.nref; ijet++) {
            // universal eta cut
            if (std::abs(t.refeta[ijet]) > 2) continue;

            Float_t rg = -1.;
            Float_t kt = -1.;

            Float_t logrg = -100.;
            Float_t logkt = -100.;

            Float_t rg_gen = -1.;
            Float_t kt_gen = -1.;

            Float_t logrg_gen = -100.;
            Float_t logkt_gen = -100.;

            // Calculate rg, kt only for 2 prong jets
            if (t.sjt2Pt[ijet] > 0.) {
                rg = calc_rg(t.sjt1Y[ijet], t.sjt1Phi[ijet], t.sjt2Y[ijet], t.sjt2Phi[ijet]);
                kt = t.sjt2Pt[ijet] * rg;
                
                // calculate logs
                logrg = std::log(jetR/rg);
                logkt = std::log(kt);
            }

            if (t.rsjt2Pt[ijet] > 0.) {
                rg_gen = calc_rg(t.rsjt1Y[ijet], t.rsjt1Phi[ijet], t.rsjt2Y[ijet], t.rsjt2Phi[ijet]);
                kt_gen = t.rsjt2Pt[ijet] * rg_gen;
                
                // calculate logs
                logrg_gen = std::log(jetR/rg_gen);
                logkt_gen = std::log(kt_gen);
            }

            bool passWP = ((t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet]) > 0.7);

            float mB = t.jtmB[ijet];
            float mB_gen = t.refmB[ijet];

            float ratio = -1.;
            if (mB_gen > 0) {
                ratio = mB / mB_gen;
            } else {
                if (mB > 0) {
                    ratio = -1.;
                } else {
                    ratio = 1.;
                }
            }

            // Fill histograms
            if (t.jtHadFlav[ijet] == 5) {
                hB_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                hB_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (t.jtNbHad[ijet] == 1) {
                    hSingleB_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                    hSingleB_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);
                } else {
                    hBB_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                    hBB_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);
                }

                if (passWP) {
                    hBtag_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                    hBtag_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);

                    if (t.jtNbHad[ijet] == 1) {
                        hSingleBtag_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                        hSingleBtag_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);
                    } else {
                        hBBtag_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                        hBBtag_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);
                    }
                }
            } 

            if (t.jtHadFlav[ijet] == 0) {
                hL_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                hL_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (passWP) {
                    hLtag_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                    hLtag_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);
                }
            }

            if (t.jtHadFlav[ijet] == 4) {
                hC_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                hC_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (passWP) {
                    hCtag_mB->Fill(mB, logkt, t.refpt[ijet], t.weight);
                    hCtag_mB_gen->Fill(mB_gen, logkt_gen, t.refpt[ijet], t.weight);
                }
            }

            

            // Int_t nvtx = t.nvtx;
            // Float_t jtpt = t.jtpt[ijet];            

            // hB_mB_ratio->Fill(ratio, nvtx, jtpt);
            // hB_mB->Fill(mB, jtpt);
            // hB_mB_gen->Fill(mB_gen, jtpt);
            // if (passWP) {
            //     hBtag_mB_ratio->Fill(ratio, nvtx, jtpt);
            //     hBtag_mB->Fill(mB, jtpt);
            //     hBtag_mB_gen->Fill(mB_gen, jtpt);
            // }
        } // end jet loop
    } // end entries loop

    // Save histograms 
    TFile *fout = new TFile(rootf_out, "recreate");
    std::cout << "(Re)creating " << rootf_out << " file." << std::endl;

    // for (TH3D *h : {hB_mB_ratio, hBtag_mB_ratio}) {
    //     h->Write();
    // }

    for (TH3D *h : {hB_mB, hBtag_mB,
                    hSingleB_mB, hSingleBtag_mB,
                    hBB_mB, hBBtag_mB,
                    hL_mB, hLtag_mB,
                    hC_mB, hCtag_mB,
                    hB_mB_gen, hBtag_mB_gen,
                    hSingleB_mB_gen, hSingleBtag_mB_gen,
                    hBB_mB_gen, hBBtag_mB_gen,
                    hL_mB_gen, hLtag_mB_gen,
                    hC_mB_gen, hCtag_mB_gen}) {
        h->Write();
    }

    fout->Close();
}

