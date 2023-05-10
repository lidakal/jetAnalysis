#include "tTree.h"
#include "hist_utils.h"

void plot_rgkt()
{
    // ---- Load tree
    TString label = "aggrTMVA_lateKt";
    TString sample = "bjet";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/" + sample + "/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    TString foutname = "./histos/qcd_" + sample + "_" + label + "_rgkt.root"; 

    tTree t = tTree(fin);

    // Activate relevant branches
    std::vector<TString> activeBranchNames = {"nref", "refeta", "refpt", "jtHadFlav", "jtNbHad",
                                            "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", "rsjt1Y",
                                            "rsjt2Pt", "rsjt2Eta", "rsjt2Phi", "rsjt2Y",
                                            "jtpt", "jteta",
                                            "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt1Y",
                                            "sjt2Pt", "sjt2Eta", "sjt2Phi", "sjt2Y",
                                            "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                            "weight"
                                            };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranchNames, 1);
    
    // Create histograms

    // ln(0.4/rg)
    Int_t x1bins = 10;
    Float_t x1min = 0.;
    Float_t x1max = 2.5;

    // rg
    // Int_t x1bins = 10;
    // Float_t x1min = 0.;
    // Float_t x1max = 0.4;

    // ln(kt)
    Int_t y1bins = 40;
    Float_t y1min = -5.;
    Float_t y1max = 5.;

    // zg
    Int_t y2bins = 40;
    Float_t y2min = 0.1;
    Float_t y2max = 0.5;

    // refpt
    Int_t z1bins = 27*2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    // reco level
    TH3D *hB_rgkt = new TH3D("hB_rgkt", "rg, kt, pt, b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hBtag_rgkt = new TH3D("hBtag_rgkt", "rg, kt, pt, b tagged b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hL_rgkt = new TH3D("hL_rgkt", "rg, kt, pt, guds jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hLtag_rgkt = new TH3D("hLtag_rgkt", "rg, kt, pt, b tagged guds jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hC_rgkt = new TH3D("hC_rgkt", "rg, kt, pt, c jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hCtag_rgkt = new TH3D("hCtag_rgkt", "rg, kt, pt, b tagged c jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hSingleB_rgkt = new TH3D("hSingleB_rgkt", "rg, kt, pt, b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hSingleBtag_rgkt = new TH3D("hSingleBtag_rgkt", "rg, kt, pt, b tagged b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hBB_rgkt = new TH3D("hBB_rgkt", "rg, kt, pt, b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hBBtag_rgkt = new TH3D("hBBtag_rgkt", "rg, kt, pt, b tagged b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);


    // gen level
    TH3D *hB_rgkt_gen = new TH3D("hB_rgkt_gen", "rg, kt, pt, b jets gen", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hBtag_rgkt_gen = new TH3D("hBtag_rgkt_gen", "rg, kt, pt, b tagged b jets gen", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hL_rgkt_gen = new TH3D("hL_rgkt_gen", "rg, kt, pt, guds jets gen", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hLtag_rgkt_gen = new TH3D("hLtag_rgkt_gen", "rg, kt, pt, b tagged guds jets gen", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hC_rgkt_gen = new TH3D("hC_rgkt_gen", "rg, kt, pt, c jets gen", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hCtag_rgkt_gen = new TH3D("hCtag_rgkt_gen", "rg, kt, pt, b tagged c jets gen", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hSingleB_rgkt_gen = new TH3D("hSingleB_rgkt_gen", "rg, kt, pt, b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hSingleBtag_rgkt_gen = new TH3D("hSingleBtag_rgkt_gen", "rg, kt, pt, b tagged b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hBB_rgkt_gen = new TH3D("hBB_rgkt_gen", "rg, kt, pt, b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hBBtag_rgkt_gen = new TH3D("hBBtag_rgkt_gen", "rg, kt, pt, b tagged b jets reco", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    const Float_t jetR = 0.4;

    std::cout << "Creating histograms ..." << std::endl;
    Long64_t nentries = t.GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        // Choose nb of events
        const Long64_t total_events = 20000000;
        if (ient > total_events) break;
		// if (ient > 100) break;
 
        t.GetEntry(ient);

        for (Int_t ijet = 0; ijet < t.nref; ijet++) {
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

            // Fill histograms
            if (t.jtHadFlav[ijet] == 5) {
                hB_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                hB_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (t.jtNbHad[ijet] == 1) {
                    hSingleB_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                    hSingleB_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);
                } else {
                    hBB_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                    hBB_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);
                }

                if (passWP) {
                    hBtag_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                    hBtag_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);

                    if (t.jtNbHad[ijet] == 1) {
                        hSingleBtag_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                        hSingleBtag_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);
                    } else {
                        hBBtag_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                        hBBtag_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);
                    }
                }
            } 

            if (t.jtHadFlav[ijet] == 0) {
                hL_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                hL_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (passWP) {
                    hLtag_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                    hLtag_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);
                }
            }

            if (t.jtHadFlav[ijet] == 4) {
                hC_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                hC_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);

                if (passWP) {
                    hCtag_rgkt->Fill(logrg, logkt, t.refpt[ijet], t.weight);
                    hCtag_rgkt_gen->Fill(logrg_gen, logkt_gen, t.refpt[ijet], t.weight);
                }
            }
        } // jet loop
    } // entry loop

    // Save histograms
    std::cout << "\n(Re)creating file " << foutname << std::endl;
    TFile *fout = new TFile(foutname, "recreate");

    for (auto h : {hB_rgkt, hBtag_rgkt, 
                   hB_rgkt_gen, hBtag_rgkt_gen,
                   hL_rgkt, hLtag_rgkt, 
                   hL_rgkt_gen, hLtag_rgkt_gen,
                   hC_rgkt, hCtag_rgkt, 
                   hC_rgkt_gen, hCtag_rgkt_gen,
                   hSingleB_rgkt, hSingleBtag_rgkt, 
                   hSingleB_rgkt_gen, hSingleBtag_rgkt_gen, 
                   hBB_rgkt, hBBtag_rgkt, 
                   hBB_rgkt_gen, hBBtag_rgkt_gen, 
                   }) {
        h->Write();
    }

    fout->Close();
}