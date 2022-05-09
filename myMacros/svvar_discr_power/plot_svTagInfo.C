#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "TreeAnalyzer.h"
#include "utils.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

void plot_svTagInfo(bool GSPincl = true, bool useBtag = true)
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
	if (!GSPincl) {
	    fname += "_noGSP";
    }
    if (useBtag) {
	   fname += "_bTag";
	}
	fname += "_cpp.root";

	std::string foutname = outdir + fname;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    TH1D *hsv = new TH1D("hsv", "Percentage of tracks from B products associated to any SV per event", 25, 0, 100);
    hsv->SetXTitle("% b-product tracks in SVs per event");
    hsv->SetYTitle("(Weighted) nb of events");

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 33200) continue;
        if (ient > 10000) break;
            
        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        ta.GetEntry(ient);
        
        Int_t bProducts = 0;
        Int_t bProductsInSV = 0;

        Int_t itrackOffset = 0;

	//std::cout << "i = " << ient << std::endl;

        for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
		    // eta cut
		    if (std::abs(ta.jteta[ijet]) > 2) {
			   itrackOffset += ta.nselIPtrk[ijet];
               continue;
			}
	        //std::cout << "ijet = " << ijet << std::endl;
	        // std::cout << "itrackOffset = " << itrackOffset << std::endl;
	        // std::cout << "jtHadFlav = " << ta.jtHadFlav[ijet] << std::endl;
            
            // Discard non b-jets
		    if ((!useBtag) && (ta.jtHadFlav[ijet] != 5)) {
                itrackOffset += ta.nselIPtrk[ijet];
                continue;
            }

			bool passWp = (ta.jtDiscDeepFlavourB[ijet] + ta.jtDiscDeepFlavourBB[ijet] + ta.jtDiscDeepFlavourLEPB[ijet]) > 0.9;
			if (useBtag && (!passWp)) {
			    itrackOffset += ta.nselIPtrk[ijet];
                continue;
            }


            // Discard GSP jets if !useBtag
	        if ((!useBtag) && (!GSPincl) && (ta.jtNbHad[ijet] > 1)) {
                itrackOffset += ta.nselIPtrk[ijet];
                continue;
			}

            // Go over jet tracks
            for (Int_t itrack = 0; itrack < ta.nselIPtrk[ijet]; itrack++) {
                // Keep only B decay products
                Int_t sta = ta.ipMatchStatus[itrack + itrackOffset];
                if (sta < 100) continue;
                bProducts += 1;
                
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
                if (trackFoundInSV) bProductsInSV += 1;
            }
            itrackOffset += ta.nselIPtrk[ijet];
        }
	//std::cout << "bProducts: " << bProducts << std::endl;
	//std::cout << "bProductsInSV: " << bProductsInSV << std::endl;
	Float_t perc = -1.;
        if (bProducts > 0) {
            perc = (Float_t(bProductsInSV) / Float_t(bProducts)) * 100;
        }
        hsv->Fill(perc, ta.weight);     
    }
    std::cout << "Saving histograms in " << foutname << std::endl;
    hsv->Write();
    fout->Close();
}


