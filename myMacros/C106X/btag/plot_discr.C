#include "../tTree.h"

void plot_discr()
{
    // TString fin = "/data_CMS/cms/kalipoliti/aod/qcdMC/chargedSJ/merged_HiForestAOD.root";
    TString fin = "/data_CMS/cms/kalipoliti/qcdMC/dijet/aggrTMVA_withNb/merged_HiForestMiniAOD.root";
    TString label = "mini";
    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref", 
                                             "refeta", "refpt", "jtHadFlav", "jtNbHad",
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB",
                                             "weight"
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
    for (Long64_t ient = 0; ient < nentries; ient++) {
        t.GetEntry(ient);  

        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
            // std::cout << "nref = " << t.nref << std::endl;
            // std::cout << "weight = " << t.weight << std::endl;
        }

        // Choose nb of events
        const Long64_t total_events = 10000000;
        // const Long64_t total_events = 1;
        if (ient > total_events) break;
		// if (ient > 100) break;

        for (int ijet = 0; ijet < t.nref; ijet++) {
            // universal eta cut
            if (std::abs(t.refeta[ijet]) > 2) continue;

            double discr_b = 0.;
            double discr_b_vs_bb = 0.;
            if (label == "aod") {
                discr_b = t.jtDiscDeepFlavourB[ijet] + t.jtDiscDeepFlavourLEPB[ijet] + t.jtDiscDeepFlavourBB[ijet];
                discr_b_vs_bb = t.jtDiscDeepFlavourB[ijet] + t.jtDiscDeepFlavourLEPB[ijet];
            } else if (label == "mini") {
                discr_b = t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet] + t.discr_deepFlavour_bb[ijet];
                discr_b_vs_bb = t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet];
            } 

            int flav = (int) t.jtHadFlav[ijet];

            if (flav == 5) {
                hB_discr->Fill(discr_b, t.refpt[ijet], t.weight);
                if (t.jtNbHad[ijet] == 1) {
                    hSingleB_discr->Fill(discr_b_vs_bb, t.refpt[ijet], t.weight);
                } else {
                    hBB_discr->Fill(discr_b_vs_bb, t.refpt[ijet], t.weight);
                }
            } else if (flav == 4) {
                hC_discr->Fill(discr_b, t.refpt[ijet], t.weight);
            } else if (flav == 0) {
                hL_discr->Fill(discr_b, t.refpt[ijet], t.weight);
            }       
        } // end jet loop
    } // end entry loop

    TString outName = "./histos/" + label + "_discr.root";
    TFile *fout = new TFile(outName, "recreate");
    for (auto h : {hB_discr, hSingleB_discr, hBB_discr,
                   hL_discr,
                   hC_discr}) {
                    h->Write();
                   }

    fout->Close();
    delete fout;
}