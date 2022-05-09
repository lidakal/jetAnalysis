#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "TreeAnalyzer.h"
#include "utils.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

void plot_svTagInfo()
{
    std::string finname = "/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root";
    TreeAnalyzer ta(finname, true);
    
    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "jteta", "jtHadFlav", "jtNbHad", "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", "nselIPtrk", "ipEta", "ipPhi", "ipMatchStatus", "svtxTrEta", "svtxTrPhi", "weight"};
    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);
    
    // Create new file to store histograms
	std::string outdir = "/home/llr/cms/kalipoliti/rootFiles/";
    std::string fname = "svTagInfo_histos";
	fname += "_cpp.root";

	std::string foutname = outdir + fname;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    Int_t nbins = 25;

    TH1D *hsv_trueB = new TH1D("hsv_trueB", "Percentage of tracks from B products associated to any SV per event", nbins, 0, 100);
    hsv_trueB->SetXTitle("% b-product tracks in SVs per event");
    hsv_trueB->SetYTitle("(Weighted) nb of events");

    TH1D *hsv_trueB_noGSP = new TH1D("hsv_trueB_noGSP", "Percentage of tracks from B products associated to any SV per event", nbins, 0, 100);
    hsv_trueB_noGSP->SetXTitle("% b-product tracks in SVs per event");
    hsv_trueB_noGSP->SetYTitle("(Weighted) nb of events");

    TH1D *hsv_wpB = new TH1D("hsv_wpB", "Percentage of tracks from B products associated to any SV per event", nbins, 0, 100);
    hsv_wpB->SetXTitle("% b-product tracks in SVs per event");
    hsv_wpB->SetYTitle("(Weighted) nb of events");

	TH1D *hsv_wpB_2prod = new TH1D("hsv_wpB_2prod", "Percentage of tracks from B products associated to any SV per event", nbins, 0, 100);
    hsv_wpB_2prod->SetXTitle("% b-product tracks in SVs per event");
    hsv_wpB_2prod->SetYTitle("(Weighted) nb of events");

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 33200) continue;
        //if (ient > 10000) break;
            
        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        ta.GetEntry(ient);
        
        Int_t bProducts_trueB = 0;
        Int_t bProductsInSV_trueB = 0;

        Int_t bProducts_trueB_noGSP = 0;
        Int_t bProductsInSV_trueB_noGSP = 0;

        Int_t bProducts_wpB = 0;
        Int_t bProductsInSV_wpB = 0;

        Int_t itrackOffset = 0;

	    //std::cout << "i = " << ient << std::endl;

        for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
		    // eta cut
		    if (std::abs(ta.jteta[ijet]) > 2) {
			   itrackOffset += ta.nselIPtrk[ijet];
               continue;
			}

            bool passWp = (ta.jtDiscDeepFlavourB[ijet] + ta.jtDiscDeepFlavourBB[ijet] + ta.jtDiscDeepFlavourLEPB[ijet]) > 0.9;

	        //std::cout << "ijet = " << ijet << std::endl;
	        // std::cout << "itrackOffset = " << itrackOffset << std::endl;
	        // std::cout << "jtHadFlav = " << ta.jtHadFlav[ijet] << std::endl;

            // Go over jet tracks
            for (Int_t itrack = 0; itrack < ta.nselIPtrk[ijet]; itrack++) {
                // Keep only B decay products
                Int_t sta = ta.ipMatchStatus[itrack + itrackOffset];
                if (sta < 100) continue;

                if (ta.jtHadFlav[ijet] == 5) {
                    bProducts_trueB += 1;
                    if (ta.jtNbHad[ijet] == 1) {
                        bProducts_trueB_noGSP += 1;
                    }
                }
                if (passWp) {
                    bProducts_wpB += 1;
                }
                
                // Look for track in SV
                Float_t eps = 0.001;
                
                Bool_t trackFoundInSV = false;
                std::vector<Float_t> svtxTrEta = flatten(*ta.svtxTrEta);
                std::vector<Float_t> svtxTrPhi = flatten(*ta.svtxTrPhi);

                for (size_t isvTrk = 0; isvTrk < svtxTrEta.size(); isvTrk++) {
                    if ((ta.ipEta[itrack + itrackOffset] - svtxTrEta[isvTrk]) > eps) continue;
                    if ((ta.ipPhi[itrack + itrackOffset] - svtxTrPhi[isvTrk]) > eps) continue;
                    trackFoundInSV = true;
                    break;
                }  
                if (trackFoundInSV) {
                    if (ta.jtHadFlav[ijet] == 5) {
                    bProductsInSV_trueB += 1;
                        if (ta.jtNbHad[ijet] == 1) {
                            bProductsInSV_trueB_noGSP += 1;
                        }
                    }
                    if (passWp) {
                        bProductsInSV_wpB += 1;
                    }
                }
            }
            itrackOffset += ta.nselIPtrk[ijet];
        }

        Float_t perc_trueB = -1.;
        if (bProducts_trueB > 0) {
            perc_trueB = (Float_t(bProductsInSV_trueB) / Float_t(bProducts_trueB)) * 100;
        }
        hsv_trueB->Fill(perc_trueB, ta.weight);  

        Float_t perc_trueB_noGSP = -1;
        if (bProducts_trueB_noGSP > 0) {
            perc_trueB_noGSP = (Float_t(bProductsInSV_trueB_noGSP) / Float_t(bProducts_trueB_noGSP)) * 100;
        }
        hsv_trueB_noGSP->Fill(perc_trueB_noGSP, ta.weight); 

        Float_t perc_wpB = -1;
        if (bProducts_wpB > 0) {
            perc_wpB = (Float_t(bProductsInSV_wpB) / Float_t(bProducts_wpB)) * 100;
        }
        hsv_wpB->Fill(perc_wpB, ta.weight); 

        Float_t perc_wpB_2prod = -1;
        if (bProducts_wpB > 1) {
    		perc_wpB_2prod = (Float_t(bProductsInSV_wpB) / Float_t(bProducts_wpB)) * 100;
		}
        hsv_wpB_2prod->Fill(perc_wpB_2prod, ta.weight);
    }
    std::cout << "Saving histograms in " << foutname << std::endl;
    hsv_trueB->Write();
    hsv_trueB_noGSP->Write();
    hsv_wpB->Write();
	hsv_wpB_2prod->Write();
    fout->Close();
}


