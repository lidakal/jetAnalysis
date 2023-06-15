// #if !(defined(__CINT__) || defined(__CLING__)) || defined(__ACLIC__)
// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
#include "../tTree.h"
#include "../hist_utils.h"
// #include "RooUnfoldResponse.h"
// #include "RooUnfoldBayes.h"
// #endif

void fill_response_2d()
{
    // Notes: In order to unfold using the underflow/overflow bins, 
    //        need to manually make them into actual histogram bins 

    // ---- Load MC

    TString label = "aggrTMVA_withNb";
    TString sample = "bjet";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/" + sample + "/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    TString rootf_out = "./histos/qcd_" + sample + "_" + label + "_unfold_2d.root";

    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref", "nvtx",
                                             "refeta", "refpt", "jtHadFlav", "jtNbHad",
                                             "jtpt",
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

    // ---- Setup 

    double kt_cut = 1.;
    double btag_cut = 0.7;
    double jetR = 0.4;

    // ---- Create the histograms 

    // jet pt bins : underflow = [60,80), overflow = [200, 1000)
    const Int_t z1bins = 8;
    Double_t z1min = 60.;
    Double_t z1max = 220.; // if jtpt >= 220 && jtpt < 1000 : jtpt = 210

    Double_t jtpt_max_overflow = 1000.;
    Double_t jtpt_overflow = 210.;

    // ln(0.4/rg) bins : underflow = [-0.4, 0), overflow = [2., +inf)
    const Int_t x1bins = 6;
    Double_t x1min = -0.4; // if untagged or kT < cut : ln(R/Rg) = -0.2
    Double_t x1max = 2.; // if ln(R/Rg) >= x1max : ln(R/Rg) = 2.3

    Double_t logrg_underflow = -0.2;
    Double_t logrg_overflow = 2.3;

    TH2D *h_data_reco_rgpt = new TH2D("h_data_reco_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_data_true_rgpt = new TH2D("h_data_true_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_data_fake_rgpt = new TH2D("h_data_fake_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_mc_true_rgpt = new TH2D("h_mc_true_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_mc_reco_rgpt = new TH2D("h_mc_reco_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_mc_fake_rgpt = new TH2D("h_mc_fake_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    RooUnfoldResponse response_rgpt(h_mc_reco_rgpt, h_mc_true_rgpt, "response_rgpt", "response for 2d: rg and jet pt"); 

    TH2D *h_mc_purity_rgpt = new TH2D("h_mc_purity_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_mc_efficiency_rgpt = new TH2D("h_mc_efficiency_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    // purity = h_mc_purity_rgpt / h_mc_reco_rgpt
    // efficiency = h_mc_efficiency_rgpt / h_mc_true_rgpt

    // ---- Fill histos
    Long64_t total_events = 13000000;
    Long64_t nentries = t.GetEntries();
    if (total_events > nentries) total_events = nentries;

    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        if (ient > total_events) break;

        t.GetEntry(ient);

        for (Int_t ijet = 0; ijet < t.nref; ijet++) {
            // event selection 
            if (std::abs(t.jteta[ijet]) > 2) continue;
            bool passWP = ((t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet]) > btag_cut);
            if (!passWP) continue;

            Float_t jtpt = t.jtpt[ijet];
            Float_t refpt = t.refpt[ijet];
            Float_t weight = t.weight;
            // weight = 5.; // debug

            // Initialize observables -- values for untagged jets
            Float_t rg = -1.;
            Float_t kt = -1.;

            Float_t logrg = logrg_underflow;
            Float_t logkt = -100.;

            Float_t rg_gen = -1.;
            Float_t kt_gen = -1.;

            Float_t logrg_gen = logrg_underflow;
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

            // kt cut 
            if (kt < kt_cut) logrg = logrg_underflow;
            if (kt_gen < kt_cut) logrg_gen = logrg_underflow;

            // overflow bins
            // if (jtpt >= z1max && jtpt < jtpt_max_overflow) jtpt = jtpt_overflow;
            // if (refpt >= z1max && refpt < jtpt_max_overflow) refpt = jtpt_overflow;
            
            // if (logrg >= x1max) logrg = logrg_overflow;
            // if (logrg_gen >= x1max) logrg_gen = logrg_overflow;

            // Fill histograms
            if (ient < total_events / 2) {
                // pseudo data
                h_data_reco_rgpt->Fill(logrg, jtpt, weight);
                if (refpt > 0)
                    h_data_true_rgpt->Fill(logrg_gen, refpt, weight);
                else
                    h_data_fake_rgpt->Fill(logrg, jtpt, weight);
            } else {
                // MC 
                bool has_gen_match = (refpt > 0);
                if (has_gen_match) {
                    // Signal = single b
                    if (!(t.jtNbHad[ijet] == 1)) continue;
                    response_rgpt.Fill(logrg, jtpt, logrg_gen, refpt, weight);
                    h_mc_reco_rgpt->Fill(logrg, jtpt, weight);
                    h_mc_true_rgpt->Fill(logrg_gen, refpt, weight);

                    // Fill purity
                    if (refpt > z1min && refpt < z1max && logrg_gen > x1min && logrg_gen < x1max)
                        h_mc_purity_rgpt->Fill(logrg, jtpt, weight);

                    // Fill efficiency
                    if (jtpt > z1min && jtpt < z1max && logrg > x1min && logrg < x1max)
                        h_mc_efficiency_rgpt->Fill(logrg_gen, refpt, weight);
                } else {
                    // Fakes 
                    response_rgpt.Fake(logrg, jtpt, weight);
                    h_mc_fake_rgpt->Fill(logrg, jtpt, weight); 
                }                
            }

            // Note: I do not have gen with no reco match in my ntuples

        } // end jet loop       
    } // end entries loop

    // ---- Save histos 

    h_mc_purity_rgpt->Divide(h_mc_reco_rgpt);
    h_mc_efficiency_rgpt->Divide(h_mc_true_rgpt);

    TFile *fout = new TFile(rootf_out, "recreate");
    for (auto h : {h_data_reco_rgpt, h_data_true_rgpt, h_data_fake_rgpt,
                   h_mc_true_rgpt, h_mc_reco_rgpt, h_mc_fake_rgpt,
                   h_mc_purity_rgpt, h_mc_efficiency_rgpt
                   }) {
                    h->Write();
                   }
    response_rgpt.Write();

    fout->Close();
    delete fout;
}