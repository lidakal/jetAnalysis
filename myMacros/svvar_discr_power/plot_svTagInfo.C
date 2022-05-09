#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "TreeAnalyzer.h"
#include "utils.h"

#include <vector>

void plot_svTagInfo()
{
    std::string finname = "/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root";
    TreeAnalyzer ta(finname, true);
    
    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "nIP", "ipEta", "ipPhi", "ipMatchStatus", "svtxTrEta", "svtxTrPhi", "weight"};
    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);
    
    // Create new file to store histograms
    std::string foutname = "/home/llr/cms/kalipoliti/rootFiles/svTagInfo_histos_cpp.root";
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    TH1D *hsv = new TH1D("hsv", "Percentage of tracks from B products associated to any SV per event", 25, 0, 100);
    hsv->SetXTitle("% b-product tracks in SVs per event");
    hsv->SetYTitle("(Weighted) nb of events");

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 33200) continue;
        //if (ient > 1000) break;
            
        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        ta.GetEntry(ient);
        
        Int_t bProducts = 0;
        Int_t bProductsInSV = 0;

        for (Int_t itrack = 0; itrack < ta.nIP; itrack++) {
            // Keep only B decay products
            Int_t sta = ta.ipMatchStatus[itrack];
            if (sta < 100) continue;
            bProducts += 1;
            
            // Look for track in SV
            Float_t eps = 0.001;
            
            Bool_t trackFoundInSV = false;
            std::vector<Float_t> svtxTrEta = flatten(*ta.svtxTrEta);
            std::vector<Float_t> svtxTrPhi = flatten(*ta.svtxTrPhi);

            for (size_t isvTrk = 0; isvTrk < svtxTrEta.size(); isvTrk++) {
                if ((ta.ipEta[itrack] - svtxTrEta[isvTrk]) > eps) continue;
                if ((ta.ipPhi[itrack] - svtxTrPhi[isvTrk]) > eps) continue;
                trackFoundInSV = true;
                break;
            }  
            if (trackFoundInSV) bProductsInSV += 1;
        }
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


