#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

#include "TreeAnalyzer.h"
#include "utils.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

void plot_svtx_BvsL() 
{
    std::string finname_B = "/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root";
    std::string finname_L = "/data_CMS/cms/kalipoliti/qcdMC/chargedSJ/merged_HiForestAOD.root";
    
    TreeAnalyzer taB(finname_B, true);
    TreeAnalyzer taL(finname_L, true);

    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav", "jtNbHad", 
                                               "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", 
                                               "nselIPtrk", "ipEta", "ipPhi", "ipMatchStatus",  
                                               "svtxTrEta", "svtxTrPhi", "svtxdls", "nsvtx", "svtxntrk", "svtxm", "svtxmcorr",
                                               "weight"};
    taB.SetBranchStatus("*", 0);
    taB.SetBranchStatus(activeBranches, 1);

    taL.SetBranchStatus("*", 0);
    taL.SetBranchStatus(activeBranches, 1);

    // For debug purposes
	//taB.SetBranchStatus("svtxntrk",0);
	//taB.SetBranchStatus("svtxdls",1);
	//taB.SetBranchStatus("nsvtx",0);
    
    // Create new file to store histograms
	std::string outdir = "/home/llr/cms/kalipoliti/rootFiles/";
    std::string fname = "svtx_histos_BvsL.root";

    std::string foutname = outdir + fname;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    // svtxdls
    Int_t x1bins = 50;
    Float_t x1min = 0.;
    Float_t x1max = 400.;

    // svtxm
    Int_t x2bins = 20;
    Float_t x2min = 0.;
    Float_t x2max = 5.;

    // svtxmcorr
    Int_t x3bins = 20;
    Float_t x3min = 0.;
    Float_t x3max = 5.;

    // svtxdls 
    TH1D *hB_svtxdls = new TH1D("hB_svtxdls", "histogram of svtxdls, bjets", x1bins, x1min, x1max);
    TH1D *hL_svtxdls = new TH1D("hL_svtxdls", "histogram of svtxdls, ljets", x1bins, x1min, x1max);

    // svtxm 
    TH1D *hB_svtxm = new TH1D("hB_svtxm", "histogram of svtxm, bjets", x2bins, x2min, x2max);
    TH1D *hL_svtxm = new TH1D("hL_svtxm", "histogram of svtxm, ljets", x2bins, x2min, x2max);

    // svtxmcorr 
    TH1D *hB_svtxmcorr = new TH1D("hB_svtmcorr", "histogram of svtxmcorr, bjets", x3bins, x3min, x3max);
    TH1D *hL_svtxmcorr = new TH1D("hL_svtmcorr", "histogram of svtxmcorr, ljets", x3bins, x3min, x3max);

    // --------------------- B JETS ------------------------

    std::cout << "Creating b-jet histograms..." << std::endl;

    for (Long64_t ient = 0; ient < taB.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 30745) continue;
        //if (ient > 30745) break;

        // Show progress
		if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
		}

        taB.GetEntry(ient);
        Float_t weight = taB.weight;

        for (Int_t ijet = 0; ijet < taB.nref; ijet++) {
		    // bjet & eta cut & pt cut
            bool isBjet = (taB.jtDiscDeepFlavourB[ijet] + taB.jtDiscDeepFlavourBB[ijet] + taB.jtDiscDeepFlavourLEPB[ijet]) > 0.9;

		    if ((!isBjet) || (std::abs(taB.jteta[ijet]) > 2) || (taB.jtpt[ijet] < 100)) continue;

            // Go over SVs in jet
            Int_t nsv = taB.nsvtx[ijet];
            for (Int_t isv = 0; isv < nsv; isv++) {
                // Get SV properties
                Float_t svtxdls = (*taB.svtxdls)[ijet][isv];
                Float_t svtxm = (*taB.svtxm)[ijet][isv];
                Float_t svtxmcorr = (*taB.svtxmcorr)[ijet][isv];
				
                // Add to histograms
                hB_svtxdls->Fill(svtxdls, weight);
                hB_svtxm->Fill(svtxm, weight);
                hB_svtxmcorr->Fill(svtxmcorr, weight);
            } // jet SV loop
        } // jet loop
    } // B entry loop

    // ---------------- LIGHT JETS ------------------------

    std::cout << "Creating l-jet histograms..." << std::endl;

    for (Long64_t ient = 0; ient < taL.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 30745) continue;
        //if (ient > 30745) break;

        // Show progress
		if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
		}

        taL.GetEntry(ient);
        Float_t weight = taL.weight;

        for (Int_t ijet = 0; ijet < taL.nref; ijet++) {
		    // eta cut & pt cut
            if ((std::abs(taL.jteta[ijet]) > 2) || (taL.jtpt[ijet] < 100)) continue;

            // Go over SVs in jet
            Int_t nsv = taL.nsvtx[ijet];
            for (Int_t isv = 0; isv < nsv; isv++) {
                // Get SV properties
                Float_t svtxdls = (*taL.svtxdls)[ijet][isv];
                Float_t svtxm = (*taL.svtxm)[ijet][isv];
                Float_t svtxmcorr = (*taL.svtxmcorr)[ijet][isv];
				
                // Add to histograms
                hL_svtxdls->Fill(svtxdls, weight);
                hL_svtxm->Fill(svtxm, weight);
                hL_svtxmcorr->Fill(svtxmcorr, weight);
            } // jet SV loop
        } // jet loop
    } // L entry loop

    std::cout << "Saving histograms in " << foutname << std::endl;
    for (auto h : {hB_svtxdls, hB_svtxm, hB_svtxmcorr, hL_svtxdls, hL_svtxm, hL_svtxmcorr}) {
        h->Write();
    }
    fout->Close();
}
