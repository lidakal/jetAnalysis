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

void plot_svtx() 
{
    std::string finname = "/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root";
    TreeAnalyzer ta(finname, true);

    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav", "jtNbHad", 
                                               "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", 
                                               "nselIPtrk", "ipEta", "ipPhi", "ipMatchStatus",  
                                               "svtxTrEta", "svtxTrPhi", "svtxdls", "nsvtx", "svtxntrk",
                                               "weight"};
    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);

    // Create new file to store histograms
	std::string outdir = "/home/llr/cms/kalipoliti/rootFiles/";
    std::string fname = "svtx_histos.root";

    std::string foutname = outdir + fname;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    // svtxdls
    Int_t x1bins = 50;
    Float_t x1min = 0.;
    Float_t x1max = 200.;

    // svtxm
    Int_t x2bins = 20;
    Float_t x2min = 0.;
    Float_t x2max = 5.;

    // svtxmcorr
    Int_t x3bins = 20;
    Float_t x3min = 0.;
    Float_t x3max = 5.;

    // % of tracks from b products
    Int_t y1bins = 10;
    Float_t y1min = 0.;
    Float_t y1max = 100.;

    // svtxdls and % of tracks from b products
    TH2D *h_svtxdls = new TH2D("h_svtxdls", "2d histogram of svtxdls and % of tracks from b products", x1bins, x1min, x1max, y1bins, y1min, y1max);

    // svtxm and % of tracks from b products
    TH2D *h_svtxm = new TH2D("h_svtxm", "2d histogram of svtxm and % of tracks from b products", x2bins, x2min, x2max, y1bins, y1min, y1max);

    // svtxmcorr and % of tracks from b products
    TH2D *h_svtxmcorr = new TH2D("h_svtmcorr", "2d histogram of svtxmcorr and % of tracks from b products", x3bins, x3min, x3max, y1bins, y1min, y1max);
    
    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 2) continue;
        if (ient > 0) break;

            
        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
		}

        ta.GetEntry(ient);
        Float_t weight = ta.weight;

        Int_t itrackOffset = 0;

        for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
            std::cout << "jet = " << ijet << std::endl;
            std::cout << "itrackOffset = " << itrackOffset << std::endl;

		    // bjet & eta cut & pt cut
            bool isBjet = (ta.jtDiscDeepFlavourB[ijet] + ta.jtDiscDeepFlavourBB[ijet] + ta.jtDiscDeepFlavourLEPB[ijet]) > 0.9;

		    if ((!isBjet) || (std::abs(ta.jteta[ijet]) > 2) || (ta.jtpt[ijet] < 100)) {
			   itrackOffset += ta.nselIPtrk[ijet];
               continue;
			}

            // Go over SVs in jet
            Int_t nsv = ta.nsvtx[ijet];
            Int_t iSVtrackOffset = 0;
            for (Int_t isv = 0; isv < nsv; isv++) {
                std::cout << "sv = " << isv << std::endl;
                std::cout << "isvtrackOffset = " << iSVtrackOffset << std::endl;
                Float_t svtxdls = (*ta.svtxdls)[ijet][isv];
                Float_t svtxm = (*ta.svtxm)[ijet][isv];
                Float_t svtxmcorr = (*ta.svtxmcorr)[ijet][isv];

                // Go over tracks in SV and count how many come from b decay products
                Int_t bProductsInSV = 0;
                Int_t tracksInSV = (*ta.svtxntrk)[ijet][isv];
                for (Int_t iSVtrack = 0; iSVtrack < tracksInSV; iSVtrack++) { 
                    std::cout << "isvtrack = " << iSVtrack << std::endl;
                    // Get track properties
                    Float_t trackEta = (*ta.svtxTrEta)[ijet][iSVtrackOffset + iSVtrack];
                    Float_t trackPhi = (*ta.svtxTrPhi)[ijet][iSVtrackOffset + iSVtrack];

                    // Look for track in jet tracks to get its status
                    Int_t trackID = -1;
                    Float_t eps = 0.001;
                    for (Int_t itrack = 0; itrack < ta.nselIPtrk[ijet]; itrack++) {
                        Float_t ipEta = ta.ipEta[itrackOffset + itrack];
                        Float_t ipPhi = ta.ipPhi[itrackOffset + itrack];
                        if (trackEta - ipEta > eps) continue;
                        if (trackPhi - ipPhi > eps) continue;
                        trackID = itrack;
                        break;
                    } // jet track loop
                    Int_t sta = ta.ipMatchStatus[itrackOffset + trackID];
                    std::cout << "track status = " << sta << std::endl;
                    if (sta >= 100) bProductsInSV += 1;
                } // SV track loop
                // Get percentage of b products in SV 
                Float_t perc = -1;
                std::cout << "bproducts in sv = " << bProductsInSV << std::endl;
                std::cout << "tracks in sv = " << tracksInSV << std::endl;
                if (tracksInSV > 0) perc = (Float_t(bProductsInSV) / Float_t(tracksInSV)) * 100.;

                // Add the SV in the histograms 
                h_svtxdls->Fill(svtxdls, perc, weight);
                h_svtxm->Fill(svtxm, perc, weight);
                h_svtxmcorr->Fill(svtxmcorr, perc, weight);

                iSVtrackOffset += tracksInSV;
            } // jet SV loop
            itrackOffset += ta.nselIPtrk[ijet];
        } // jet loop
    } // entry loop

    std::cout << "Saving histograms in " << foutname << std::endl;
    for (auto h : {h_svtxdls, h_svtxm, h_svtxmcorr}) {
        h->Write();
    }
    fout->Close();
}
