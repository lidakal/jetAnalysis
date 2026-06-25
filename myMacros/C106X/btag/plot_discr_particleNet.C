#include "../tTree.h"

void plot_discr_particleNet()
{
    TString fin = "/data_CMS/cms/kalipoliti/qcdMC/dijet/aggrTMVA_particleNet/merged_HiForestMiniAOD.root";
    TString label = "particleNet_dijet";

    // TString fin = "/data_CMS/cms/kalipoliti/qcdMC/bjet/aggrTMVA_particleNet/merged_HiForestMiniAOD.root";
    // TString label = "particleNet";
    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref", 
                                             "refeta", "refpt", "jtNbHad", "jtNcHad",
                                             "jteta", "jtpt", "jtptCh",
                                             "discr_particleNet_b", "discr_particleNet_bb", "discr_particleNet_BvsAll",
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "weight",
                                             };
                                             
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);
    

    // ---- Create the histograms 

    // jet pt
    Int_t z1bins = 27 * 2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    // discr 
    Int_t x1bins = 40;
    Float_t x1min = 0.;
    Float_t x1max = 1.;

    Int_t nbins = 25;
    Double_t bins[26]={0.,0.02, 0.04, 0.06, 0.08, 0.1, 0.2, 0.3, 0.35, 0.4, 0.5, 0.6, 0.7, 0.8, 0.85, 0.9, 0.91, 0.92, 0.93, 0.94, 0.95, 0.96, 0.97, 0.98, 0.99, 1.0001};

    // for particleNet_b
    TH2F *hB_particleNet_b = new TH2F("hB_particleNet_b", "x=b, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hBB_particleNet_b = new TH2F("hBB_particleNet_b", "x=b, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hL_particleNet_b = new TH2F("hL_particleNet_b", "x=b, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hC_particleNet_b = new TH2F("hC_particleNet_b", "x=b, y=jtpt", nbins, bins, z1bins, z1min, z1max);

    // for particleNet_BvsAll
    TH2F *hB_particleNet_BvsAll = new TH2F("hB_particleNet_BvsAll", "x=BvsAll, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hBB_particleNet_BvsAll = new TH2F("hBB_particleNet_BvsAll", "x=BvsAll, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hL_particleNet_BvsAll = new TH2F("hL_particleNet_BvsAll", "x=BvsAll, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hC_particleNet_BvsAll = new TH2F("hC_particleNet_BvsAll", "x=BvsAll, y=jtpt", nbins, bins, z1bins, z1min, z1max);

    // for particleNet_bb
    TH2F *hB_particleNet_bb = new TH2F("hB_particleNet_bb", "x=bb, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hBB_particleNet_bb = new TH2F("hBB_particleNet_bb", "x=bb, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hL_particleNet_bb = new TH2F("hL_particleNet_bb", "x=bb, y=jtpt", nbins, bins, z1bins, z1min, z1max);
    TH2F *hC_particleNet_bb = new TH2F("hC_particleNet_bb", "x=bb, y=jtpt", nbins, bins, z1bins, z1min, z1max);

    // for deepFlavour_b
    TH2F *hB_deepFlavour_b = new TH2F("hB_deepFlavour_b", "x=b, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hBB_deepFlavour_b = new TH2F("hBB_deepFlavour_b", "x=b, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hL_deepFlavour_b = new TH2F("hL_deepFlavour_b", "x=b, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hC_deepFlavour_b = new TH2F("hC_deepFlavour_b", "x=b, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    // for deepFlavour_bb
    TH2F *hB_deepFlavour_bb = new TH2F("hB_deepFlavour_bb", "x=bb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hBB_deepFlavour_bb = new TH2F("hBB_deepFlavour_bb", "x=bb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hL_deepFlavour_bb = new TH2F("hL_deepFlavour_bb", "x=bb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hC_deepFlavour_bb = new TH2F("hC_deepFlavour_bb", "x=bb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    // for deepFlavour_blepb
    TH2F *hB_deepFlavour_blepb = new TH2F("hB_deepFlavour_blepb", "x=b+lepb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hBB_deepFlavour_blepb = new TH2F("hBB_deepFlavour_blepb", "x=b+lepb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hL_deepFlavour_blepb = new TH2F("hL_deepFlavour_blepb", "x=b+lepb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hC_deepFlavour_blepb = new TH2F("hC_deepFlavour_blepb", "x=b+lepb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    // for deepFlavour_BvsAll
    TH2F *hB_deepFlavour_BvsAll = new TH2F("hB_deepFlavour_BvsAll", "x=b+lepb+bb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hBB_deepFlavour_BvsAll = new TH2F("hBB_deepFlavour_BvsAll", "x=b+lepb+bb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hL_deepFlavour_BvsAll = new TH2F("hL_deepFlavour_BvsAll", "x=b+lepb+bb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2F *hC_deepFlavour_BvsAll = new TH2F("hC_deepFlavour_BvsAll", "x=b+lepb+bb, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    std::cout << "Creating histograms ..." << std::endl;

    Long64_t nentries = t.GetEntries();
    Long64_t total_events = 20000000;
    if (total_events > nentries) total_events = nentries;

    for (Long64_t ient = 0; ient < total_events; ient++) {
        t.GetEntry(ient);  
        double weight = t.weight;

        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }


        // debug: test efficiency with cut on weight
        if (weight > 0.005) continue;

        for (int ijet = 0; ijet < t.nref; ijet++) {
            // universal eta cut
            if (std::abs(t.jteta[ijet]) > 2) continue;
            if (std::abs(t.jtptCh[ijet]) < 1e-4) continue;

            double particleNet_b = t.discr_particleNet_b[ijet];
            double particleNet_BvsAll = t.discr_particleNet_BvsAll[ijet];
            double particleNet_bb = t.discr_particleNet_bb[ijet];

            double deepFlavour_b = t.discr_deepFlavour_b[ijet];
            double deepFlavour_lepb = t.discr_deepFlavour_lepb[ijet];
            double deepFlavour_bb = t.discr_deepFlavour_bb[ijet];

            double deepFlavour_blepb = deepFlavour_b + deepFlavour_lepb;
            double deepFlavour_BvsAll = deepFlavour_b + deepFlavour_lepb + deepFlavour_bb;

            Int_t jtNbHad = t.jtNbHad[ijet];
            Int_t jtNcHad = t.jtNcHad[ijet];
            Float_t jtpt = t.jtpt[ijet];

            if (jtNbHad > 1) {
                hBB_particleNet_b->Fill(particleNet_b, jtpt, weight);
                hBB_particleNet_bb->Fill(particleNet_bb, jtpt, weight);
                hBB_particleNet_BvsAll->Fill(particleNet_BvsAll, jtpt, weight);

                hBB_deepFlavour_b->Fill(deepFlavour_b, jtpt, weight);
                hBB_deepFlavour_bb->Fill(deepFlavour_bb, jtpt, weight);
                hBB_deepFlavour_blepb->Fill(deepFlavour_blepb, jtpt, weight);
                hBB_deepFlavour_BvsAll->Fill(deepFlavour_BvsAll, jtpt, weight);
            } else if (jtNbHad > 0) {
                hB_particleNet_b->Fill(particleNet_b, jtpt, weight);
                hB_particleNet_bb->Fill(particleNet_bb, jtpt, weight);
                hB_particleNet_BvsAll->Fill(particleNet_BvsAll, jtpt, weight);

                hB_deepFlavour_b->Fill(deepFlavour_b, jtpt, weight);
                hB_deepFlavour_bb->Fill(deepFlavour_bb, jtpt, weight);
                hB_deepFlavour_blepb->Fill(deepFlavour_blepb, jtpt, weight);
                hB_deepFlavour_BvsAll->Fill(deepFlavour_BvsAll, jtpt, weight);
            } else if (jtNcHad > 0) {
                hC_particleNet_b->Fill(particleNet_b, jtpt, weight);
                hC_particleNet_bb->Fill(particleNet_bb, jtpt, weight);
                hC_particleNet_BvsAll->Fill(particleNet_BvsAll, jtpt, weight);

                hC_deepFlavour_b->Fill(deepFlavour_b, jtpt, weight);
                hC_deepFlavour_bb->Fill(deepFlavour_bb, jtpt, weight);
                hC_deepFlavour_BvsAll->Fill(deepFlavour_BvsAll, jtpt, weight);
                hC_deepFlavour_blepb->Fill(deepFlavour_blepb, jtpt, weight);
            } else {
                hL_particleNet_b->Fill(particleNet_b, jtpt, weight);
                hL_particleNet_bb->Fill(particleNet_bb, jtpt, weight);
                hL_particleNet_BvsAll->Fill(particleNet_BvsAll, jtpt, weight);

                hL_deepFlavour_b->Fill(deepFlavour_b, jtpt, weight);
                hL_deepFlavour_bb->Fill(deepFlavour_bb, jtpt, weight);
                hL_deepFlavour_BvsAll->Fill(deepFlavour_BvsAll, jtpt, weight);
                hL_deepFlavour_blepb->Fill(deepFlavour_blepb, jtpt, weight);
            }       
        } // end jet loop
    } // end entry loop

    TString outName = "./histos/" + label + "_discr.root";
    TFile *fout = new TFile(outName, "recreate");
    for (auto h : {
                   hBB_particleNet_b, hBB_particleNet_bb, hBB_particleNet_BvsAll,
                   hB_particleNet_b, hB_particleNet_bb, hB_particleNet_BvsAll,
                   hC_particleNet_b, hC_particleNet_bb, hC_particleNet_BvsAll,
                   hL_particleNet_b, hL_particleNet_bb, hL_particleNet_BvsAll,

                   hBB_deepFlavour_b, hBB_deepFlavour_bb, hBB_deepFlavour_BvsAll, hBB_deepFlavour_blepb,
                   hB_deepFlavour_b, hB_deepFlavour_bb, hB_deepFlavour_BvsAll, hB_deepFlavour_blepb,
                   hC_deepFlavour_b, hC_deepFlavour_bb, hC_deepFlavour_BvsAll, hC_deepFlavour_blepb,
                   hL_deepFlavour_b, hL_deepFlavour_bb, hL_deepFlavour_BvsAll, hL_deepFlavour_blepb,
                   }) {
                    h->Write();
                   }

    fout->Close();
    delete fout;
}