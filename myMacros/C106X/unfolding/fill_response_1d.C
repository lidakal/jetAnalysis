#include "../../../../RooUnfold/src/RooUnfoldResponse.h"
#include "../../../../RooUnfold/src/RooUnfoldBayes.h"
#include "../tTree.h"
#include "../hist_utils.h"

void fill_response_1d()
{
    // Notes: In order to unfold using the underflow/overflow bins, 
    //        need to manually make them into actual histogram bins 

    // ---- Load MC

    TString label = "aggrTMVA_withNb";
    TString sample = "bjet";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/" + sample + "/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    TString rootf_out = "./histos/qcd_" + sample + "_" + label + "_unfold_1d.root";

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

    double jtpt_min = 100.;
    double jtpt_max = 120.;

    // ---- Create the histograms 

    // ln(0.4/rg) bins : underflow = [-0.275, 0), overflow = [2.2, +inf)
    const Int_t x1bins = 10;
    Double_t x1min = -0.275; // if untagged or kT < cut : ln(R/Rg) = -0.2
    Double_t x1max = 2.475; // if ln(R/Rg) >= 2.475 : ln(R/Rg) = 2.3

    Double_t logrg_underflow = -0.2;
    Double_t logrg_overflow = 2.3;

    TH1D *h_data_reco_rg = new TH1D("h_data_reco_rg", "x=rg", x1bins, x1min, x1max);
    TH1D *h_data_true_rg = new TH1D("h_data_true_rg", "x=rg", x1bins, x1min, x1max);
    TH1D *h_data_fake_rg = new TH1D("h_data_fake_rg", "x=rg", x1bins, x1min, x1max);
    TH1D *h_mc_true_rg = new TH1D("h_mc_true_rg", "x=rg", x1bins, x1min, x1max);
    TH1D *h_mc_reco_rg = new TH1D("h_mc_reco_rg", "x=rg", x1bins, x1min, x1max);
    TH1D *h_mc_fake_rg = new TH1D("h_mc_fake_rg", "x=rg", x1bins, x1min, x1max);

    RooUnfoldResponse response_rg(h_mc_reco_rg, h_mc_true_rg, "response_rg", "response for 1d: rg"); 

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
            
            if (logrg >= x1max) logrg = logrg_overflow;
            if (logrg_gen >= x1max) logrg_gen = logrg_overflow;

            bool in_jtpt = (jtpt >= jtpt_min && jtpt < jtpt_max);
            bool in_refpt = (refpt >= jtpt_min && refpt < jtpt_max);

            // Fill histograms
            if (ient < total_events / 2) {
                // pseudo data
                if (in_jtpt) {
                    h_data_reco_rg->Fill(logrg, weight);
                    if (in_refpt)
                        h_data_true_rg->Fill(logrg_gen, weight);
                    else 
                        h_data_fake_rg->Fill(logrg, weight);
                }
            } else {
                // MC 
                bool has_gen_match = (refpt > 0);
                if (has_gen_match) {
                    // Signal = single b
                    if (!(t.jtNbHad[ijet] == 1)) continue;
                    if (in_jtpt && in_refpt) {
                        response_rg.Fill(logrg, logrg_gen, weight);
                        h_mc_reco_rg->Fill(logrg, weight);
                        h_mc_true_rg->Fill(logrg_gen, weight);
                    } else if (in_jtpt) {
                        response_rg.Fake(logrg, weight);
                        h_mc_fake_rg->Fill(logrg, weight);
                    } else if (in_refpt) {
                        response_rg.Miss(logrg_gen, weight);
                    }
                } else if (in_jtpt) {
                    // Fakes 
                    response_rg.Fake(logrg, weight);
                    h_mc_fake_rg->Fill(logrg, weight);
                }                
            }

            // Note: I do not have gen with no reco match in my ntuples

        } // end jet loop       
    } // end entries loop

    // ---- Save histos 

    TFile *fout = new TFile(rootf_out, "recreate");
    for (auto h : {h_data_reco_rg, h_data_true_rg, h_data_fake_rg,
                   h_mc_true_rg, h_mc_reco_rg, h_mc_fake_rg
                   }) {
                    h->Write();
                   }
    response_rg.Write();

    fout->Close();
    delete fout;
}