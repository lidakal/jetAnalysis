#include "TreeAnalyzer.h"
#include "selections.h" // passSelection(int Selection, bool inSV, float ip3dSig)

#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"

void plot_aggregation_eff()
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_truthInfo_fixedBugs/merged_HiForestAOD.root";
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

    Int_t ptbins = 27*2;
    Float_t ptmin = 30.;
    Float_t ptmax = 300.;

    // X = track from B (0.2) / track not from B (0.9)
    // Y = track pass selection (0.2) / track does not pass selection (0.9)
    TH3D *heff_sel1 = new TH3D("heff_sel1", "histogram with b product tag efficiency, sel1", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel2 = new TH3D("heff_sel2", "histogram with b product tag efficiency, sel2", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel3 = new TH3D("heff_sel3", "histogram with b product tag efficiency, sel3", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel4 = new TH3D("heff_sel4", "histogram with b product tag efficiency, sel4", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel5 = new TH3D("heff_sel5", "histogram with b product tag efficiency, sel5", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel6 = new TH3D("heff_sel6", "histogram with b product tag efficiency, sel6", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel7 = new TH3D("heff_sel7", "histogram with b product tag efficiency, sel7", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel8 = new TH3D("heff_sel8", "histogram with b product tag efficiency, sel8", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel9 = new TH3D("heff_sel9", "histogram with b product tag efficiency, sel9", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel10 = new TH3D("heff_sel10", "histogram with b product tag efficiency, sel10", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel11 = new TH3D("heff_sel11", "histogram with b product tag efficiency, sel11", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel12 = new TH3D("heff_sel12", "histogram with b product tag efficiency, sel12", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel13 = new TH3D("heff_sel13", "histogram with b product tag efficiency, sel13", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel14 = new TH3D("heff_sel14", "histogram with b product tag efficiency, sel14", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel15 = new TH3D("heff_sel15", "histogram with b product tag efficiency, sel15", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    TH3D *heff_sel16 = new TH3D("heff_sel16", "histogram with b product tag efficiency, sel16", 2, 0, 1, 2, 0, 1, ptbins, ptmin, ptmax);
    
    //------------------ Calculate efficiency / purity ----------------------

    // Go over events
    for (Long64_t ient = 0; ient < ta.nentries; ient++) {

        // For debugging purposes
        //if (ient < 1887891) continue;
        //if (ient > 1887891) break;

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
            
            // bjet & eta cut 
            if ((!passWp) || (std::abs(ta.jteta[ijet]) > 2)) {
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
                bool passSelection6 = passSelection(6, inSV, ip3dSig);
                bool passSelection7 = passSelection(7, inSV, ip3dSig);
                bool passSelection8 = passSelection(8, inSV, ip3dSig);
                bool passSelection9 = passSelection(9, inSV, ip3dSig);
                bool passSelection10 = passSelection(10, inSV, ip3dSig);
                bool passSelection11 = passSelection(11, inSV, ip3dSig);
                bool passSelection12 = passSelection(12, inSV, ip3dSig);
                bool passSelection13 = passSelection(13, inSV, ip3dSig);
				bool passSelection14 = passSelection(14, inSV, ip3dSig);
                bool passSelection15 = passSelection(15, inSV, ip3dSig);
                bool passSelection16 = passSelection(16, inSV, ip3dSig);

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

                Float_t ySel6 = 0.9;
                if (passSelection6) ySel6 = 0.2;

                Float_t ySel7 = 0.9;
                if (passSelection7) ySel7 = 0.2;

                Float_t ySel8 = 0.9;
                if (passSelection8) ySel8 = 0.2;

                Float_t ySel9 = 0.9;
                if (passSelection9) ySel9 = 0.2;

                Float_t ySel10 = 0.9;
                if (passSelection10) ySel10 = 0.2;

                Float_t ySel11 = 0.9;
                if (passSelection11) ySel11 = 0.2;

                Float_t ySel12 = 0.9;
                if (passSelection12) ySel12 = 0.2;

                Float_t ySel13 = 0.9;
                if (passSelection13) ySel13 = 0.2;

                Float_t ySel14 = 0.9;
                if (passSelection14) ySel14 = 0.2;

                Float_t ySel15 = 0.9;
                if (passSelection15) ySel15 = 0.2;

                Float_t ySel16 = 0.9;
                if (passSelection16) ySel16 = 0.2;

                heff_sel1->Fill(xFromB, ySel1, ta.jtpt[ijet], weight);
                heff_sel2->Fill(xFromB, ySel2, ta.jtpt[ijet], weight);
                heff_sel3->Fill(xFromB, ySel3, ta.jtpt[ijet], weight);
                heff_sel4->Fill(xFromB, ySel4, ta.jtpt[ijet], weight);
                heff_sel5->Fill(xFromB, ySel5, ta.jtpt[ijet], weight);
                heff_sel6->Fill(xFromB, ySel6, ta.jtpt[ijet], weight);
                heff_sel7->Fill(xFromB, ySel7, ta.jtpt[ijet], weight);
                heff_sel8->Fill(xFromB, ySel8, ta.jtpt[ijet], weight);
                heff_sel9->Fill(xFromB, ySel9, ta.jtpt[ijet], weight);
                heff_sel10->Fill(xFromB, ySel10, ta.jtpt[ijet], weight);
                heff_sel11->Fill(xFromB, ySel11, ta.jtpt[ijet], weight);
                heff_sel12->Fill(xFromB, ySel12, ta.jtpt[ijet], weight);
                heff_sel13->Fill(xFromB, ySel13, ta.jtpt[ijet], weight);
                heff_sel14->Fill(xFromB, ySel14, ta.jtpt[ijet], weight);
                heff_sel15->Fill(xFromB, ySel15, ta.jtpt[ijet], weight);
                heff_sel16->Fill(xFromB, ySel16, ta.jtpt[ijet], weight);

            } // jet track loop
            itrackOffset += ta.nselIPtrk[ijet];
        } // jet loop
    } // entry loop

    std::cout << "Saving histograms in " << foutname << std::endl;
    for (auto h : {heff_sel1, 
                    heff_sel2, heff_sel3, heff_sel4, heff_sel5, 
                    heff_sel6, heff_sel7, heff_sel8, heff_sel9,
                    heff_sel10, heff_sel11, heff_sel12, heff_sel13,
                    heff_sel14, heff_sel15, heff_sel16}) 
	        
{
        h->Write();
    }

    fout->Close();
}
