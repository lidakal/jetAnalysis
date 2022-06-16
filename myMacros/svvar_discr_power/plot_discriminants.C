#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH3D.h"
#include "TCanvas.h"

#include "TreeAnalyzer.h"
#include "utils.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

void plot_discriminants() 
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_truthInfo_fixedBugs/merged_HiForestAOD.root";
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
    std::string fname = "bDiscriminants_histos.root";

    std::string foutname = outdir + fname;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    // ip3dSig
    Int_t x1bins = 48;
    Float_t x1min = -45.;
    Float_t x1max = 99.;

    //TH1D *hsig_ip3dSig = new TH1D("hsig_ip3dSig", "histogram of ip3dSig, signal", x1bins, x1min, x1max);
    //TH1D *hbkg_ip3dSig = new TH1D("hbkg_ip3dSig", "histogram of ip3dSig, background", x1bins, x1min, x1max);

    // in SV
    Int_t x2bins = 2;
    Float_t x2min = 0.;
    Float_t x2max = 1.;

    //TH1D *hsig_inSV = new TH1D("hsig_inSV", "histogram of inSV, signal", x2bins, x2min, x2max);
    //TH1D *hbkg_inSV = new TH1D("hbkg_inSV", "histogram of inSV, background", x2bins, x2min, x2max);

    // jet pt
    Int_t z1bins = 27*3;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    // ip3dSig, inSV, jetpt
    TH3D *hsig_3d = new TH3D("hsig_3d", "3d histogram of ip3dSig, inSV, jetpt signal", x1bins, x1min, x1max, x2bins, x2min, x2max, z1bins, z1min, z1max);
    TH3D *hbkg_3d = new TH3D("hbkg_3d", "3d histogram of ip3dSig, inSV, jetpt background", x1bins, x1min, x1max, x2bins, x2min, x2max, z1bins, z1min, z1max);

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
		    // bjet & eta cut 
            bool isBjet = (ta.jtDiscDeepFlavourB[ijet] + ta.jtDiscDeepFlavourBB[ijet] + ta.jtDiscDeepFlavourLEPB[ijet]) > 0.9;

		    if ((!isBjet) || (std::abs(ta.jteta[ijet]) > 2)) {
			    itrackOffset += ta.nselIPtrk[ijet];
                continue;
			}

            // remove GSP 
            if (ta.jtNbHad[ijet] != 1) {
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

                // Check if track is in SV
                bool inSV = false;
                std::vector<Float_t> svtxTrEta = flatten(*ta.svtxTrEta);
                std::vector<Float_t> svtxTrPhi = flatten(*ta.svtxTrPhi);
				Float_t eps = 0.00001;
                for (size_t isvTrk = 0; isvTrk < svtxTrEta.size(); isvTrk++) {
				  if (std::abs(ipEta - svtxTrEta[isvTrk]) > eps) continue;
				  if (std::abs(ipPhi - svtxTrPhi[isvTrk]) > eps) continue;
                    inSV = true;
                    break;
                }  

                Float_t putInSV = 0.1;
                if (inSV) putInSV = 0.9;

                if (sta >= 100) {
                    //hsig_ip3dSig->Fill(ip3dSig, weight);
                    //hsig_inSV->Fill(putInSV, weight);
                    hsig_3d->Fill(ip3dSig, putInSV, ta.jtpt[ijet], weight);
                } else if (sta == 1) {
                    //hbkg_ip3dSig->Fill(ip3dSig, weight);
                    //hbkg_inSV->Fill(putInSV, weight);
                    hbkg_3d->Fill(ip3dSig, putInSV, ta.jtpt[ijet], weight);
                }
            } // track loop
            itrackOffset += ta.nselIPtrk[ijet];
        } // jet loop
    } // entry loop

    std::cout << "Saving histograms in " << foutname << std::endl;
    //for (auto h : {hsig_inSV, hsig_ip3dSig, hbkg_inSV, hbkg_ip3dSig}) {
    //    h->Write();
    //}
    hsig_3d->Write();
    hbkg_3d->Write();
    fout->Close();
}
