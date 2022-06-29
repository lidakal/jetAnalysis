#include "TreeAnalyzer.h"
#include "selections.h" // passSelection(int Selection, bool inSV, float ip3dSig)

#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"

void plot_Bfrag_function()
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_truthInfo_fixedBugs/merged_HiForestAOD.root";
    TreeAnalyzer ta(finname, true);

    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav", "jtNbHad",
                                               "refeta", "refpt",
                                               "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", 
                                               "nselIPtrk", "ipPt", "ipEta", "ipPhi", "ipMatchStatus", "ip3dSig", 
                                               "svtxTrEta", "svtxTrPhi", "svtxdls", "svtxm", "svtxmcorr", "nsvtx", "svtxntrk",
                                               "weight"};
    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);

    // Create new file to store histograms
	std::string outdir = "/home/llr/cms/kalipoliti/rootFiles/";
    std::string fname = "bFF.root";

    std::string foutname = outdir + fname;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    Int_t ffbins = 40;
    Float_t ffmin = 0.000001;
    Float_t ffmax = 1.;

    Int_t ptbins = 27*2;
    Float_t ptmin = 30.;
    Float_t ptmax = 300.;

    TH2D *hBtag_reco_truth = new TH2D("hBtag_reco_truth", "B FF at reco level, truth info", ffbins, ffmin, ffmax, ptbins, ptmin, ptmax);
    TH2D *hBtag_reco_highEff = new TH2D("hBtag_reco_highEff", "B FF at reco level, highest eff", ffbins, ffmin, ffmax, ptbins, ptmin, ptmax);

    //------------------ Calculate B FF ----------------------

    // Go over events
    for (Long64_t ient = 0; ient < ta.nentries; ient++) {

        // For debugging purposes
        //if (ient < 1887891) continue;
        //if (ient > 10000) break;

        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
		}

        ta.GetEntry(ient);
        Float_t weight = ta.weight;
        Int_t itrackOffset = 0;

        // Go over jets
        for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
            bool passWp = (ta.jtDiscDeepFlavourB[ijet] + ta.jtDiscDeepFlavourBB[ijet] + ta.jtDiscDeepFlavourLEPB[ijet]) > 0.9;
            
            // b-jet & eta cut 
            if ((ta.jtHadFlav[ijet] != 5) || (!passWp) || (std::abs(ta.jteta[ijet]) > 2)) {
			   itrackOffset += ta.nselIPtrk[ijet];
               continue;
			}

            // remove GSP 
            if (ta.jtNbHad[ijet] != 1) {
                itrackOffset += ta.nselIPtrk[ijet];
                continue;
            }

            // Initialize B pt's
            Float_t bPt_Btag_reco_truth = 0.;
            Float_t bPt_Btag_reco_highEff = 0.;
            
            // Initialize (charged) jet pt
            Float_t jetpt = 0.;
            // Go over jet tracks
            for (Int_t itrack = 0; itrack < ta.nselIPtrk[ijet]; itrack++) {
                
                // Get track properties
                Float_t ipPt = ta.ipPt[itrackOffset + itrack];
                Float_t ipEta = ta.ipEta[itrackOffset + itrack];
                Float_t ipPhi = ta.ipPhi[itrackOffset + itrack];
                Float_t ip3dSig = ta.ip3dSig[itrackOffset + itrack];
                Int_t sta = ta.ipMatchStatus[itrackOffset + itrack];

                bool isFromB = (sta >= 100);

                // Find track in SV and get the SV properties
                Float_t eps = 0.00001;
                Int_t whichSV = -1;
                Int_t iSVtrackOffset = 0;

                //std::cout << "jet: " << ijet << ", nsv: " << ta.nsvtx[ijet] << std::endl;

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
                    if (whichSV >= 0) break;
                    iSVtrackOffset += tracksInSV;
                } // SV loop

                bool inSV = (whichSV >= 0);
                bool passHighEffSel = passSelection(2, inSV, ip3dSig);
                
                // Add to B pt
                if (isFromB) bPt_Btag_reco_truth += ipPt;
                if (passHighEffSel) bPt_Btag_reco_highEff += ipPt;
                jetpt += ipPt;

            } // jet track loop
            itrackOffset += ta.nselIPtrk[ijet];

            // Fill histograms
            Float_t jtpt = ta.jtpt[ijet];
            hBtag_reco_truth->Fill(bPt_Btag_reco_truth / jetpt, jtpt, weight);
            hBtag_reco_highEff->Fill(bPt_Btag_reco_highEff / jetpt, jtpt, weight);
        } // jet loop
    } // entry loop

    std::cout << "Saving histograms in " << foutname << std::endl;
    for (auto h : {hBtag_reco_truth, hBtag_reco_highEff}) {
        h->Write();
    }
    fout->Close();
}
