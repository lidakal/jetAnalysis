#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "TreeAnalyzer.h"
#include "utils.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

void plot_discriminants() 
{
    std::string finname = "/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root";
    TreeAnalyzer ta(finname, true);

    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav", "jtNbHad", 
                                               "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", 
                                               "nselIPtrk", "ipEta", "ipPhi", "ipMatchStatus", "ip3dSig", 
                                               "svtxTrEta", "svtxTrPhi", "svtxdls",
                                               "weight"};
    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);

    // Create new file to store histograms
	std::string outdir = "/home/llr/cms/kalipoliti/rootFiles/";
    std::string fname = "bDiscriminants_histos";

    std::string foutname = outdir + fname;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    // ip3dSig
    Int_t x1bins = 25;
    Float_t x1min = -100.;
    Float_t x1max = 150.;

    TH1D *hsig_ip3dSig = new TH1D("hsig_ip3dSig", "histogram of ip3dSig, signal", x1bins, x1min, x1max);
    TH1D *hbkg_ip3dSig = new TH1D("hbkg_ip3dSig", "histogram of ip3dSig, background", x1bins, x1min, x1max);

    // in SV
    Int_t x2bins = 2;
    Float_t x2min = 0.;
    Float_t x2max = 1.;

    TH1D *hsig_inSV = new TH1D("hsig_inSV", "histogram of inSV, signal", x2bins, x2min, x2max);
    TH1D *hbkg_inSV = new TH1D("hbkg_inSV", "histogram of inSV, background", x2bins, x2min, x2max);

    /*
    // svtxdls
    Int_t x3bins = 2;
    Float_t x3min = 0.;
    Float_t x3max = 1.;

    TH1D *hsig_svtxdls = new TH1D("hsig_svtxdls", "histogram of svtxdls, signal", x3bins, x3min, x3max);
    TH1D *hbkg_svtxdls = new TH1D("hbkg_svtxdls", "histogram of svtxdls, background", x3bins, x3min, x3max);
    */
    
    // Signal = from B decay
    // Background = not from B decay

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 33200) continue;
        //if (ient > 10000) break;
            
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
                Float_t ipEta = ta.ipEta[itrack + itrackOffset];
                Float_t ipPhi = ta.ipPhi[itrack + itrackOffset];
                Float_t ip3dSig = TAb.ip3dSig[itrackOffset + itrack];
                Int_t sta = ta.ipMatchStatus[itrack + itrackOffset];

                // Check if track is in SV
                bool inSV = false;
                std::vector<Float_t> svtxTrEta = flatten(*ta.svtxTrEta);
                std::vector<Float_t> svtxTrPhi = flatten(*ta.svtxTrPhi);
                for (size_t isvTrk = 0; isvTrk < svtxTrEta.size(); isvTrk++) {
                    if ((ipEta - svtxTrEta[isvTrk]) > eps) continue;
                    if ((ipPhi - svtxTrPhi[isvTrk]) > eps) continue;
                    inSV = true;
                    break;
                }  

                Float_t putInSV = 0.;
                if (inSV) putInSV = 1.;

                if (sta >= 100) {
                    hsig_ip3dSig->Fill(ip3dSig, weight);
                    hsig_inSV->Fill(putInSV, weight);
                } else if (sta == 1) {
                    hbkg_ip3dSig->Fill(ip3dSig, weight);
                    hbkg_inSV->Fill(putInSV, weight);
                }
            } // track loop
            itrackOffset += ta.nselIPtrk[ijet];
        } // jet loop
    } // entry loop

    std::cout << "Saving histograms in " << foutname << std::endl;
    for (auto h : {hsig_inSV, hsig_ip3dSig, hbkg_inSV, hbkg_ip3dSig}) {
        h->Write();
    }
    fout->Close();
}