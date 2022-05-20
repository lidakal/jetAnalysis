#include "TreeAnalyzer.h"
#include "selections.h" // passSelection(int Selection, bool inSV, float ip3dSig)

#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"

void plot_aggregation_eff()
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

    // Create new file to store efficiencies 
	std::string outdir = "/home/llr/cms/kalipoliti/rootFiles/";
    std::string fname = "aggregation_eff.root";

    std::string foutname = outdir + fname;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    // X = track from B (0.2) / track not from B (0.9)
    // Y = track pass selection (0.2) / track does not pass selection (0.9)
    TH2D *heff_sel1 = new TH2D("heff_sel1", "histogram with b product tag efficiency", 2, 0, 1, 2, 0, 1);
    TH2D *heff_sel2 = new TH2D("heff_sel2", "histogram with b product tag efficiency", 2, 0, 1, 2, 0, 1);
    TH2D *heff_sel3 = new TH2D("heff_sel3", "histogram with b product tag efficiency", 2, 0, 1, 2, 0, 1);
    TH2D *heff_sel4 = new TH2D("heff_sel4", "histogram with b product tag efficiency", 2, 0, 1, 2, 0, 1);
    TH2D *heff_sel5 = new TH2D("heff_sel5", "histogram with b product tag efficiency", 2, 0, 1, 2, 0, 1);
    
    //------------------ Calculate efficiency / purity ----------------------

    // Go over events
    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
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

            // bjet & eta cut & pt cut
            if ((!passWp) || (std::abs(ta.jteta[ijet]) > 2) || (ta.jtpt[ijet] < 100)) {
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

                bool isFromB = (sta >= 100);

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
                    if (whichSV >= 0) break;
                    iSVtrackOffset += tracksInSV;
                } // SV loop

                bool inSV = (whichSV >= 0);

                // Initialize variables
                Float_t svtxdls = -100.;
                Float_t svtxm = -100.;
                Float_t svtxmcorr = -100.;

                if (inSV) {
                    svtxdls = (*ta.svtxdls)[ijet][whichSV];
                    svtxm = (*ta.svtxm)[ijet][whichSV];
                    svtxmcorr = (*ta.svtxmcorr)[ijet][whichSV];
                }

                bool passSelection1 = passSelection(1, inSV, ip3dSig);
                bool passSelection2 = passSelection(2, inSV, ip3dSig);
                bool passSelection3 = passSelection(3, inSV, ip3dSig);
                bool passSelection4 = passSelection(4, inSV, ip3dSig);
                bool passSelection5 = passSelection(5, inSV, ip3dSig);

                // Fill histograms
                Float_t xFromB = 0.9;
                if (isFromB) xFromB = 0.2;

                Float_t ySel1 = 0.9;
                if (passSelection1) ySel1 = 0.2;

                Float_t ySel2 = 0.9;
                if (passSelection2) ySel2 = 0.2;

                Float_t ySel3 = 0.9;
                if (passSelection3) ySel3 = 0.2;

                Float_t ySel4 = 0.9;
                if (passSelection4) ySel4 = 0.2;

                Float_t ySel5 = 0.9;
                if (passSelection5) ySel5 = 0.2;

                heff_sel1->Fill(xFromB, ySel1, weight);
                heff_sel2->Fill(xFromB, ySel2, weight);
                heff_sel3->Fill(xFromB, ySel3, weight);
                heff_sel4->Fill(xFromB, ySel4, weight);
                heff_sel5->Fill(xFromB, ySel5, weight);
            } // jet track loop
            itrackOffset += ta.nselIPtrk[ijet];
        } // jet loop
    } // entry loop

    std::cout << "Saving histograms in " << foutname << std::endl;
    for (auto h : {heff_sel1, heff_sel2, heff_sel3, heff_sel4, heff_sel5}) {
        h->Write();
    }

    fout->Close();
}
