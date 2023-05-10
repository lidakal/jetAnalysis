#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TH3.h>

#include <limits>

#include "tTree.h"
#include "hist_utils.h"

void plot_ptRatio()
{
    // ---- Load tree
    TString label = "aggrTMVA_withNb";
    TString sample = "dijet";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/" + sample + "/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    TString rootf_out = "./histos/qcd_" + sample + "_" + label + "_ptRatio.root";

    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref", "nvtx",
                                             "refeta", "refpt", "jtHadFlav", "jtNbHad",
                                             "jtpt", "jteta", "jtphi", 
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "jtBpt", "jtptCh",
                                             "refBpt", "refptCh",
                                             "weight"
                                             };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    // ---- Create the histograms 

    // ptRatio
    Int_t x2bins = 10;
    Float_t x2min = 0.;
    Float_t x2max = 1.;

    // ln(kt)
    Int_t y2bins = 40;
    Float_t y2min = -5.;
    Float_t y2max = 5.;

    // jet pt
    Int_t z1bins = 27 * 2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    // reco level
    TH3D *hB_ptRatio = new TH3D("hB_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBtag_ptRatio = new TH3D("hBtag_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hSingleB_ptRatio = new TH3D("hSingleB_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hSingleBtag_ptRatio = new TH3D("hSingleBtag_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hBB_ptRatio = new TH3D("hBB_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBBtag_ptRatio = new TH3D("hBBtag_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hL_ptRatio = new TH3D("hL_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hLtag_ptRatio = new TH3D("hLtag_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hC_ptRatio = new TH3D("hC_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hCtag_ptRatio = new TH3D("hCtag_ptRatio", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    // gen level
    TH3D *hB_ptRatio_gen = new TH3D("hB_ptRatio_gen", "x=ptRatio_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBtag_ptRatio_gen = new TH3D("hBtag_ptRatio_gen", "x=ptRatio_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hSingleB_ptRatio_gen = new TH3D("hSingleB_ptRatio_gen", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hSingleBtag_ptRatio_gen = new TH3D("hSingleBtag_ptRatio_gen", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hBB_ptRatio_gen = new TH3D("hBB_ptRatio_gen", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBBtag_ptRatio_gen = new TH3D("hBBtag_ptRatio_gen", "x=ptRatioreco, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hL_ptRatio_gen = new TH3D("hL_ptRatio_gen", "x=ptRatio_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hLtag_ptRatio_gen = new TH3D("hLtag_ptRatio_gen", "x=ptRatio_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

    TH3D *hC_ptRatio_gen = new TH3D("hC_ptRatio_gen", "x=ptRatio_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hCtag_ptRatio_gen = new TH3D("hCtag_ptRatio_gen", "x=ptRatio_gen, y=logkt, z=jtpt", x2bins, x2min, x2max, y2bins, y2min, y2max, z1bins, z1min, z1max);

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

            float ptRatio = t.jtBpt[ijet] / t.jtptCh[ijet];
            float ptRatio_gen = t.refBpt[ijet] / t.refptCh[ijet];

            // Fill histograms
            if (t.jtHadFlav[ijet] == 5) {
                hB_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                hB_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (t.jtNbHad[ijet] == 1) {
                    hSingleB_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                    hSingleB_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);
                } else {
                    hBB_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                    hBB_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);
                }

                if (passWP) {
                    hBtag_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                    hBtag_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);

                    if (t.jtNbHad[ijet] == 1) {
                        hSingleBtag_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                        hSingleBtag_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);
                    } else {
                        hBBtag_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                        hBBtag_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);
                    }
                }
            } 

            if (t.jtHadFlav[ijet] == 0) {
                hL_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                hL_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (passWP) {
                    hLtag_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                    hLtag_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);
                }
            }

            if (t.jtHadFlav[ijet] == 4) {
                hC_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                hC_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (passWP) {
                    hCtag_ptRatio->Fill(ptRatio, logkt, t.refpt[ijet], t.weight);
                    hCtag_ptRatio_gen->Fill(ptRatio_gen, logkt_gen, t.refpt[ijet], t.weight);
                }
            }
        } // end jet loop
    } // end entries loop

    // Save histograms 
    TFile *fout = new TFile(rootf_out, "recreate");
    std::cout << "(Re)creating " << rootf_out << " file." << std::endl;

    // for (TH3D *h : {hB_ptRatio_ratio, hBtag_ptRatio_ratio}) {
    //     h->Write();
    // }

    for (TH3D *h : {hB_ptRatio, hBtag_ptRatio,
                    hSingleB_ptRatio, hSingleBtag_ptRatio,
                    hBB_ptRatio, hBBtag_ptRatio,
                    hL_ptRatio, hLtag_ptRatio,
                    hC_ptRatio, hCtag_ptRatio,
                    hB_ptRatio_gen, hBtag_ptRatio_gen,
                    hSingleB_ptRatio_gen, hSingleBtag_ptRatio_gen,
                    hBB_ptRatio_gen, hBBtag_ptRatio_gen,
                    hL_ptRatio_gen, hLtag_ptRatio_gen,
                    hC_ptRatio_gen, hCtag_ptRatio_gen}) {
        h->Write();
    }

    fout->Close();
}

