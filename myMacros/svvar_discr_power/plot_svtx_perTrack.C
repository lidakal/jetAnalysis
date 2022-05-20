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

void plot_svtx_perTrack() 
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSig_looserCut_fixedBugs/merged_HiForestAOD.root";
    TreeAnalyzer ta(finname, true);

    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav", "jtNbHad", 
                                               "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", 
                                               "nselIPtrk", "ipEta", "ipPhi", "ipMatchStatus", "ip3dSig", 
                                               "svtxTrEta", "svtxTrPhi", "svtxdls", "svtxm", "svtxmcorr", "nsvtx", "svtxntrk",
                                               "weight"};
    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);

    // Create new file to store histograms
	std::string outdir = "/home/llr/cms/kalipoliti/rootFiles/";
    std::string fname = "svtx_histos_perTrack.root";

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
    Float_t x3max = 10.;

    // svtxdls 
    TH1D *hsig_svtxdls = new TH1D("hsig_svtxdls", "histogram of svtxdls, signal", x1bins, x1min, x1max);
    TH1D *hbkg_svtxdls = new TH1D("hbkg_svtxdls", "histogram of svtxdls, background", x1bins, x1min, x1max);

    // svtxm 
    TH1D *hsig_svtxm = new TH1D("hsig_svtxm", "histogram of svtxm, signal", x2bins, x2min, x2max);
    TH1D *hbkg_svtxm = new TH1D("hbkg_svtxm", "histogram of svtxm, background", x2bins, x2min, x2max);

    // svtxmcorr 
    TH1D *hsig_svtxmcorr = new TH1D("hsig_svtxmcorr", "histogram of svtxmcorr, signal", x3bins, x3min, x3max);
    TH1D *hbkg_svtxmcorr = new TH1D("hbkg_svtxmcorr", "histogram of svtxmcorr, background", x3bins, x3min, x3max);
    
    // Signal = from B decay
    // Background = not from B decay

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 2) continue;
        //if (ient > 10) break;

            
        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
		}

        ta.GetEntry(ient);
        Float_t weight = ta.weight;

        Int_t itrackOffset = 0;

        for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
		    // bjet & eta cut & pt cut
            bool isBjet = (ta.jtDiscDeepFlavourB[ijet] + ta.jtDiscDeepFlavourBB[ijet] + ta.jtDiscDeepFlavourLEPB[ijet]) > 0.9;

		    if ((!isBjet) || (std::abs(ta.jteta[ijet]) > 2) || (ta.jtpt[ijet] < 100)) {
			   itrackOffset += ta.nselIPtrk[ijet];
               continue;
			}

            // Go over jet tracks
            for (Int_t itrack = 0; itrack < ta.nselIPtrk[ijet]; itrack++) {
                // Get track properties
                Float_t ipEta = ta.ipEta[itrackOffset + itrack];
                Float_t ipPhi = ta.ipPhi[itrackOffset + itrack];
                Float_t ip3dSig = ta.ip3dSig[itrackOffset + itrack];
                Int_t sta = ta.ipMatchStatus[itrackOffset + itrack];

                // Find track in SV and get the SV properties
                Float_t eps = 0.00001;
                Int_t whichSV = -1;
                Int_t iSVtrackOffset = 0;
                for (Int_t isv = 0; isv < ta.nsvtx[ijet]; isv++) {
                    Float_t tracksInSV = (*ta.svtxntrk)[ijet][isv];
                    for (Int_t iSVtrack = 0; iSVtrack < tracksInSV; iSVtrack++) { 
                        // Get SV track properties
                        Float_t trackEta = (*ta.svtxTrEta)[ijet][iSVtrackOffset + iSVtrack];
                        Float_t trackPhi = (*ta.svtxTrPhi)[ijet][iSVtrackOffset + iSVtrack];

                        if (std::abs(trackEta - ipEta) > eps) continue;
                        if (std::abs(trackPhi - ipPhi) > eps) continue;

                        whichSV = isv;
                        break;
                    } // SV track loop
                    if (whichSV > 0) break;
                    iSVtrackOffset += tracksInSV;
                } // SV loop

                // Initialize variables
                Float_t svtxdls = -100.;
                Float_t svtxm = -100.;
                Float_t svtxmcorr = -100.;

                if (whichSV > 0) {
                    svtxdls = (*ta.svtxdls)[ijet][whichSV];
                    svtxm = (*ta.svtxm)[ijet][whichSV];
                    svtxmcorr = (*ta.svtxmcorr)[ijet][whichSV];
                }

                // Add to histograms
                if (sta >= 100) {
                    hsig_svtxdls->Fill(svtxdls, weight);
                    hsig_svtxm->Fill(svtxm, weight);
                    hsig_svtxmcorr->Fill(svtxmcorr, weight);
                } else if (sta == 1) {
                    hbkg_svtxdls->Fill(svtxdls, weight);
                    hbkg_svtxm->Fill(svtxm, weight);
                    hbkg_svtxmcorr->Fill(svtxmcorr, weight);
                }
            } // track loop
            itrackOffset += ta.nselIPtrk[ijet];
        } // jet loop
    } // entry loop

    std::cout << "Saving histograms in " << foutname << std::endl;
    for (auto h : {hsig_svtxdls, hsig_svtxm, hsig_svtxmcorr, hbkg_svtxdls, hbkg_svtxm, hbkg_svtxmcorr}) {
        h->Write();
    }
    fout->Close();
}
