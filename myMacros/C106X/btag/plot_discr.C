#include "../tTree.h"

void plot_discr()
{
    TString fin = "/data_CMS/cms/kalipoliti/aod/qcdMC/chargedSJ/merged_HiForestAOD.root";
    // TString fin = "/data_CMS/cms/kalipoliti/qcdMC/dijet/aggrTMVA_withHLT_v2/merged_HiForestMiniAOD.root";
    // TString fin = "/data_CMS/cms/kalipoliti/qcdMC/bjet/aggrTMVA_withHLT/merged_HiForestMiniAOD.root";
    TString label = "aod";
    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref", 
                                             "refeta", "refpt", "jtNbHad", "jtNcHad",
                                             "jteta", "jtpt",
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB",
                                             "weight",
                                             "HLT_HIAK4CaloJet60_v1"
                                             };
                                             
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);
    

    // ---- Create the histograms 

    // jet pt
    Int_t z1bins = 27 * 2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    // discr 
    Int_t x1bins = 20;
    Float_t x1min = 0.;
    Float_t x1max = 1.;

    TH2F *hB_discr = new TH2F("hB_discr", "x=b+lepb, y=refpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hL_discr = new TH2F("hL_discr", "x=b+lepb, y=refpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hC_discr = new TH2F("hC_discr", "x=b+lepb, y=refpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    TH2F *hSingleB_discr = new TH2F("hSingleB_discr", "x=b+lepb, y=refpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hBB_discr = new TH2F("hBB_discr", "x=b+lepb, y=refpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    std::cout << "Creating histograms ..." << std::endl;

    Long64_t nentries = t.GetEntries();
    Long64_t total_events = 20000000;
    if (total_events > nentries) total_events = nentries;

    double sumWeight = 0;
    double sumWeightBtag = 0.;

    for (Long64_t ient = 0; ient < total_events; ient++) {
        t.GetEntry(ient);  
        double weight = t.weight;

        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        if (label == "mini" && t.HLT_HIAK4CaloJet60_v1 != 1) continue;

        // debug: test efficiency with cut on weight
        if (weight > 0.2) continue;

        for (int ijet = 0; ijet < t.nref; ijet++) {
            // universal eta cut
            if (std::abs(t.jteta[ijet]) > 2) continue;

            double discr_b = 0.;
            double discr_b_vs_bb = 0.;
            if (label == "aod") {
                discr_b = t.jtDiscDeepFlavourB[ijet] + t.jtDiscDeepFlavourLEPB[ijet] + t.jtDiscDeepFlavourBB[ijet];
                discr_b_vs_bb = t.jtDiscDeepFlavourB[ijet] + t.jtDiscDeepFlavourLEPB[ijet];
            } else if (label == "mini") {
                discr_b = t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet] + t.discr_deepFlavour_bb[ijet];
                discr_b_vs_bb = t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet];
            } 

            Int_t jtNbHad = t.jtNbHad[ijet];
            Int_t jtNcHad = t.jtNcHad[ijet];
            Float_t jtpt = t.jtpt[ijet];

            if (jtNbHad > 0) {
                // hB_discr->Fill(discr_b, jtpt, weight);
                if (jtNbHad == 1) {
                    hSingleB_discr->Fill(discr_b_vs_bb, jtpt, weight);
                    if (jtpt>=60 && jtpt < 140) sumWeight += weight;
                    if (discr_b_vs_bb >= 0.7 && jtpt>=60 && jtpt < 140) sumWeightBtag += weight;
                } else {
                    hBB_discr->Fill(discr_b_vs_bb, jtpt, weight);
                }
            } else if (jtNcHad > 0) {
                hC_discr->Fill(discr_b_vs_bb, jtpt, weight);
            } else {
                hL_discr->Fill(discr_b_vs_bb, jtpt, weight);
            }       
        } // end jet loop
    } // end entry loop

    std::cout << "efficiency = " << sumWeightBtag/sumWeight << std::endl;

    TString outName = "./histos/" + label + "_discr.root";
    TFile *fout = new TFile(outName, "recreate");
    for (auto h : {
                //    hB_discr, 
                   hSingleB_discr, 
                   hBB_discr,
                   hL_discr,
                   hC_discr}) {
                    h->Write();
                   }

    fout->Close();
    delete fout;
}