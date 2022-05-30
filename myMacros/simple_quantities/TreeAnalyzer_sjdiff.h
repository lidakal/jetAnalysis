#include "../pt2dscan/TreeAnalyzer_pt2dscan.h"

#include "TH3D.h"

#include "stdlib.h"
#include <cmath>

class TreeAnalyzer_sjdiff : public TreeAnalyzer_pt2dscan
{
    public:
        void plot_sjdiff(std::string foutname, bool GSPincl, float bTagWP = 0.9);
};

void TreeAnalyzer_sjdiff::plot_sjdiff(std::string foutname, bool GSPincl, float bTagWP = 0.9)
{   
    std::string noGSP = "";
    if (!GSPincl) {
        noGSP = "_noGSP";
    }

    // Fix foutname 
    foutname = "~/rootFiles/" + foutname + noGSP + ".root";
    
    // Activate relevant branches
    t->SetBranchStatus("*", 0);
    std::vector<std::string> activeBranches = {"nref", "jtHadFlav", "jtNbHad", "jteta", "jtpt", "jtIsHardest",
                                                "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt2Pt", "sjt2Eta", "sjt2Phi",
                                                "rstj1Pt", "rsjt1Eta", "rsjt1Phi", "rstj2Pt", "rsjt2Eta", "rsjt2Phi",
                                                "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB",
                                                "weight"};
    for (auto activeBranchName : activeBranches) {
        t->SetBranchStatus(activeBranchName.c_str(), 1);
    }

    // Create histograms

    std::cout << "Creating B histograms ... " << std::endl;

    // dpt
    Int_t xbins = 40;
    Float_t xmin = -100.;
    Float_t xmax = 60.;

    // dR
    Int_t ybins = 40.;
    Float_t ymin = -1.;
    Float_t ymax = 1.;

    // jetpt 
    Int_t zbins = 27 * 2;
    Float_t zmin = 30.;
    Float_t zmax = 300.;

    TH3D *hB_sjt1 = new TH3D("hB_sjt1", "Dpt, DR, jetpt for sjt1", xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);
    TH3D *hB_sjt1_dynKt = new TH3D("hB_sjt1_dynKt", "Dpt, DR, jetpt for sjt1", xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);
    TH3D *hBtag_sjt1 = new TH3D("hBtag_sjt1", "Dpt, DR, jetpt for sjt1, that pass btag", xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);
    TH3D *hBtag_sjt1_dynKt = new TH3D("hBtag_sjt1_dynKt", "Dpt, DR, jetpt for sjt1, that pass btag", xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);

    TH3D *hB_sjt2 = new TH3D("hB_sjt2", "Dpt, DR, jetpt for sjt2", xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);
    TH3D *hB_sjt2_dynKt = new TH3D("hB_sjt2_dynKt", "Dpt, DR, jetpt for sjt2", xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);
    TH3D *hBtag_sjt2 = new TH3D("hBtag_sjt2", "Dpt, DR, jetpt for sjt2, that pass btag", xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);
    TH3D *hBtag_sjt2_dynKt = new TH3D("hBtag_sjt2_dynKt", "Dpt, DR, jetpt for sjt2, that pass btag", xbins, xmin, xmax, ybins, ymin, ymax, zbins, zmin, zmax);

    // Go over events
    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        t->GetEntry(ient);

        for (Int_t ijet = 0; ijet < nref; ijet++) {
            // Universal eta cut
            if (std::abs(jteta[ijet]) > 2) continue;

            // Check if it passes b-tagging WP
            bool passWP = ((jtDiscDeepFlavourB[ijet] + jtDiscDeepFlavourBB[ijet] + jtDiscDeepFlavourLEPB[ijet]) > bTagWP);

            // Initialize observables
            Float_t DPt1 = -1000.;
            Float_t DPt2 = -1000.;

            Float_t DR1 = -1000.;
            Float_t DR2 = -1000.;

            // Calculate observables if valid substructure 
            if ((sjt2Pt[ijet] > 0.) && (rsjt2Pt[ijet] > 0.)) {
                DPt1 = sjt1Pt[ijet] - rsjt1Pt[ijet];
                DPt2 = sjt2Pt[ijet] - rsjt2Pt[ijet];

                Float_t DEta1 = sjt1Eta[ijet] - rsjt1Eta[ijet];
                Float_t DPhi1 = std::acos(std::cos(sjt1Phi[ijet] - rsjt1Phi[ijet]));
                DR1 = std::sqrt((DEta1 * DEta1) + (DPhi1 * DPhi1));

                Float_t DEta2 = sjt2Eta[ijet] - rsjt2Eta[ijet];
                Float_t DPhi2 = std::acos(std::cos(sjt2Phi[ijet] - rsjt2Phi[ijet]));
                DR2 = std::sqrt((DEta2 * DEta2) + (DPhi2 * DPhi2));
            }

            // Fill histograms
            if (jtHadFlav[ijet] == 5) {
                if (GSPincl || (jtNbHad[ijet] == 1)) {
                    hB_sjt1->Fill(DPt1, DR1, jtpt[ijet], weight);
                    hB_sjt2->Fill(DPt2, DR2, jtpt[ijet], weight);
                }
            }

            if (passWP) { 
                if (GSPincl || (jtNbHad[ijet] == 1)) {
                    hBtag_sjt1->Fill(DPt1, DR1, jtpt[ijet], weight);
                    hBtag_sjt2->Fill(DPt2, DR2, jtpt[ijet], weight);
                }
            }

            // Same for dynKt histograms
            if (!jtIsHardest) { 
                DPt1 = -1000.;
                DPt2 = -1000.;
                DR1 = -1000.;
                DR2 = -1000.;
            }

            if (jtHadFlav[ijet] == 5) {
                if (GSPincl || (jtNbHad[ijet] == 1)) {
                    hB_sjt1_dynKt->Fill(DPt1, DR1, jtpt[ijet], weight);
                    hB_sjt2_dynKt->Fill(DPt2, DR2, jtpt[ijet], weight);
                }
            }

            if (passWP) { 
                if (GSPincl || (jtNbHad[ijet] == 1)) {
                    hBtag_sjt1_dynKt->Fill(DPt1, DR1, jtpt[ijet], weight);
                    hBtag_sjt2_dynKt->Fill(DPt2, DR2, jtpt[ijet], weight);
                }
            } 
        } // jet loop 
    } // entry loop

    // Create output file
    std::cout << "Saving histograms in " << foutname << std::endl;
    TFile *fout = new TFile(foutname.c_str(), "recreate");
    for (auto h : {hB_sjt1, hB_sjt1_dynKt, hB_sjt2, hB_sjt2_dynKt,
                   hBtag_sjt1, hBtag_sjt1_dynKt, hBtag_sjt2, hBtag_sjt2_dynKt}) {
                       h->Write();
                   }
    fout->Close();
}