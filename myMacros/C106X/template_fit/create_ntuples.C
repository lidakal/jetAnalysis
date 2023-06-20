#include "../tTree.h"
#include "../hist_utils.h"

void create_ntuples()
{
    // Notes: create hdata, hbkg, hsig from MC
    //        at reco level to do a template fit

    // ---- Load MC

    TString label = "aggrTMVA_withHLT";
    TString sample = "dijet";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/" + sample + "/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    TString rootf_out = "./histos/qcd_" + sample + "_" + label + "_data_for_template_fit_noWeight.root";

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
                                             "weight",
                                             "HLT_HIAK4CaloJet30_v1", "HLT_HIAK4CaloJet40_v1", "HLT_HIAK4CaloJet60_v1"
                                             };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    // ---- Setup 

    double kt_cut = 1.;
    double btag_cut = 0.7;
    double jetR = 0.4;

    // ---- Create the histograms 

    // jet pt bins : no underflow or overflow
    const Int_t z1bins = 5;
    Double_t z1min = 60.;
    Double_t z1max = 160.; 

    // ln(0.4/rg) bins : 1st bin untagged (unphysical)
    const Int_t x1bins = 5;
    Double_t x1min = -0.6; // if untagged or kT < cut : ln(R/Rg) = -0.2
    Double_t x1max = 2.4; 

    Double_t logrg_underflow = -0.2;

    // mBch bins : 
    const Int_t y1bins = 7;
    Double_t y1min = 0.;
    Double_t y1max = 7.;

    TH3D *hdata_mc = new TH3D("hdata_mc", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hdata_sig = new TH3D("hdata_sig", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hdata_bkg_bb = new TH3D("hdata_bkg_bb", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hdata_bkg_rest = new TH3D("hdata_bkg_rest", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hsig = new TH3D("hsig", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hbkg_bb = new TH3D("hbkg_bb", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hbkg_rest = new TH3D("hbkg_rest", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    // ---- Fill histos
    Long64_t total_events = 100000000;
    Long64_t nentries = t.GetEntries();
    if (total_events > nentries) total_events = nentries;

    std::cout << "Going to run on " << total_events << " events" << std::endl;

    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        if (ient > total_events) break;

        t.GetEntry(ient);
        if (t.HLT_HIAK4CaloJet60_v1 != 1) continue;

        for (Int_t ijet = 0; ijet < t.nref; ijet++) {
            // event selection 
            if (std::abs(t.jteta[ijet]) > 2) continue;
            bool passWP = ((t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet]) > btag_cut);
            if (!passWP) continue;

            Float_t jtpt = t.jtpt[ijet];
            Float_t refpt = t.refpt[ijet];
            Float_t weight = t.weight;
            weight = 1.;

            Float_t mBch = t.jtmB[ijet];

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

            // Calculate if jet is within limits
            bool reco_pass_cuts = (jtpt < z1max && jtpt >= z1min && logrg < x1max && logrg >= x1min);
            bool true_pass_cuts = (refpt < z1max && refpt >= z1min && logrg_gen < x1max && logrg_gen >= x1min);

            // Fill histograms
            if (ient < total_events / 2) {
                // pseudo data
                hdata_mc->Fill(logrg, mBch, jtpt, weight);
                if (t.jtNbHad[ijet] == 1) {
                    hdata_sig->Fill(logrg, mBch, jtpt, weight);         
                } else if (t.jtNbHad[ijet] == 2) {
                    hdata_bkg_bb->Fill(logrg, mBch, jtpt, weight);
                } else {
                    hdata_bkg_rest->Fill(logrg, mBch, jtpt, weight);
                }   
            } else {
                // MC 
                if (t.jtNbHad[ijet] == 1) {
                    hsig->Fill(logrg, mBch, jtpt, weight);         
                } else if (t.jtNbHad[ijet] == 2) {
                    hbkg_bb->Fill(logrg, mBch, jtpt, weight);
                } else {
                    hbkg_rest->Fill(logrg, mBch, jtpt, weight);
                }              
            }
        } // end jet loop       
    } // end entries loop

    // ---- Save histos 
    TFile *fout = new TFile(rootf_out, "recreate");
    for (auto h : {hdata_mc, hdata_sig, hdata_bkg_bb, hdata_bkg_rest,
                   hsig, hbkg_bb, hbkg_rest
                   }) {
                    h->Write();
                   }
    fout->Close();
    delete fout;
}