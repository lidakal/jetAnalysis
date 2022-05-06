#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "TreeAnalyzer.h"

#include <vector>

void plot_svTagInfo()
{
    TreeAnalyzer ta("/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root", true);
    
    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "nIP", "ipMatchStatus", "svtxTrEta", "svtxTrPhi", "weight"};
    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);
    
    // Create new file to store histograms
    fout = TFile("/home/llr/cms/kalipoliti/rootFiles/svTagInfo_histos.root", "recreate")

    TH1D *hsv = new TH1D("hsv", "Percentage of tracks from B products associated to any SV per event", 25, 0, 100);
    hsv.SetXTitle("% b-product tracks in SVs per event");
    hsv.SetYTitle("(Weighted) nb of events");

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
        
        // for debugging purposes 
        //if (ient < 33200) continue;
        if (ient > 1000) break;
            
        // Show progress
        if (ient % 10000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        ta.GetEntry(ient);
        
        Int_t bProducts = 0;
        Int_t bProductsInSV = 0;

        for (Int_t itrack = 0; itrack < ta.nIP; itrack++) {
            // Keep only B decay products
            Int_t sta = ta.ipMatchStatus[itrack];
            if (sta < 100) continue;
            bProducts += 1
            
            // Look for track in SV
            Float_t eps = 0.001;
            
            Bool_t trackFoundInSV = false;
            std::vector<Float_t> svtxTrEta = flatten(*ta.svtTrEta);
            std::vector<Float_t> svtxTrPhi = flatten(*ta.svtTrPhi);

            for (size_t isvTrk = 0; isvTrk < svtxTrEta.size(); isvTrk++) {
                if ((ta.ipEta[itrack] - svtxTrEta[isvTrk]) > eps) continue;
                if ((ta.ipPhi[itrack] - svtxTrPhi[isvTrk]) > eps) continue;
                trackFoundInSV = true;
                break;
            }  
            if (trackFoundInSV) bProductsInSV += 1;
        Float_t perc = -1.;
        if (bProducts > 0) {
            perc = (Float_t(bProductsInSV) / Float_t(bProducts)) * 100;
        }
        hsv.Fill(perc, t.weight);     
        
    hsv.Write();

    TCanvas *c = new TCanvas("c", "c", 800, 800);
    hsv.Draw("hist");
    c.Draw();

    fout.Close();
}

template<typename T>
std::vector<T> flatten(std::vector<std::vector<T>> const &vec)
{
    std::vector<T> flattened;
    for (auto const &v: vec) {
        flattened.insert(flattened.end(), v.begin(), v.end());
    }
    return flattened;
}