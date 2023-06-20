#include "../tTree.h"
#include "../hist_utils.h"

void create_ntuples_data()
{
    // Notes: create hdata with rg, mb, jtpt

    // ---- Load MC

    TString label = "aggrTMVA_v2";
    TString indir = "/data_CMS/cms/kalipoliti/bJet2017G/LowEGJet/bJet2017G_LowEGJet_ULv2_" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    TString rootf_out = "./histos/data_" + label + "_raw_full.root";

    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref",
                                             "jtpt", "jteta", "jtphi", "jtmB",
                                             "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt1Y",
                                             "sjt2Pt", "sjt2Eta", "sjt2Phi", "sjt2Y",
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
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
    const Int_t z1bins = 3;
    Double_t z1min = 80.;
    Double_t z1max = 140.; 

    // ln(0.4/rg) bins : 1st bin untagged (unphysical)
    const Int_t x1bins = 6;
    Double_t x1min = -0.4; // if untagged or kT < cut : ln(R/Rg) = -0.2
    Double_t x1max = 2.; 

    Double_t logrg_underflow = -0.2;

    // mBch bins : 
    const Int_t y1bins = 7;
    Double_t y1min = 0.;
    Double_t y1max = 7.;

    TH3D *hdata = new TH3D("hdata", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    // ---- Fill histos
    Long64_t total_events = 2000000000;
    Long64_t nentries = t.GetEntries();
    if (total_events > nentries) total_events = nentries;

    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 10000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        if (ient > total_events) break;

        t.GetEntry(ient);
        // event selection
        if (t.HLT_HIAK4CaloJet60_v1 != 1) continue;

        for (Int_t ijet = 0; ijet < t.nref; ijet++) {
            // jet selection 
            if (std::abs(t.jteta[ijet]) > 2) continue;
            bool passWP = ((t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet]) > btag_cut);
            if (!passWP) continue;

            Float_t jtpt = t.jtpt[ijet];
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

            // kt cut 
            if (kt < kt_cut) logrg = logrg_underflow;

            // Calculate if jet is within limits
            bool reco_pass_cuts = (jtpt < z1max && jtpt >= z1min && logrg < x1max && logrg >= x1min);

            // Fill histograms
            if (reco_pass_cuts)
                hdata->Fill(logrg, mBch, jtpt);
        } // end jet loop       
    } // end entries loop

    // ---- Save histos 
    TFile *fout = new TFile(rootf_out, "recreate");
    for (auto h : {hdata
                   }) {
                    h->Write();
                   }
    fout->Close();
    delete fout;
}