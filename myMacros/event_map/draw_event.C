#include "../aggregation/TreeAnalyzer.h"
#include "../aggregation/selections.h" // passSelection(int Selection, bool inSV, float ip3dSig)
#include "hiTreeAnalyzer.h"

#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "stdlib.h"

void draw_event(Long64_t ient = 0)
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSig_looserCut_fixedBugs/merged_HiForestAOD.root";
    TreeAnalyzer ta(finname, true);
    hiTreeAnalyzer hita(finname, true);

    // Activate branches of interest
    std::vector<std::string> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav", "jtNbHad", 
                                               "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", 
                                               "nselIPtrk", "ipEta", "ipPhi", "ipMatchStatus", "ip3dSig", 
                                               "svtxTrEta", "svtxTrPhi", "svtxdls", "svtxm", "svtxmcorr", "nsvtx", "svtxntrk",
                                               "weight"};
    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);

    TMultiGraph *mg = new TMultiGraph();
    TLegend *leg = new TLegend(0.2, 0.6, 0.4, 0.8);

    ta.GetEntry(ient);
    hita.GetEntry(ient);

    // Draw gen particles
    std::cout << "Drawing gen particles..." << std::endl;

	TGraph *gr_gen = new TGraph();
	TGraph *gr_genb = new TGraph();

    for (Int_t ipar = 0; ipar < hita.mult; ipar++) {
        Float_t phi = (*hita.phi)[ipar];
        Float_t eta = (*hita.eta)[ipar];
        Int_t sta = (*hita.sta)[ipar];

		gr_gen->SetPoint(gr_gen->GetN(), phi, eta);

        if (sta >= 100) {
		    gr_genb->SetPoint(gr_genb->GetN(), phi, eta);
        }
    } // gen particle loop

	gr_gen->SetMarkerStyle(kOpenCircle);
	gr_gen->SetMarkerColorAlpha(kBlack, 0.5);
	gr_gen->SetMarkerSize(1);
	mg->Add(gr_gen);

    gr_genb->SetMarkerStyle(kFullCircle);
    gr_genb->SetMarkerColorAlpha(kBlack, 0.5);
	gr_genb->SetMarkerSize(1);
    mg->Add(gr_genb);

    // Go over jets in event
    std::cout << "Drawing the jets..." << std::endl;

    Int_t itrackOffset = 0;
    Int_t jetColor = 0;
    for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
        Int_t ntracks = ta.nselIPtrk[ijet];

        // universal eta cut
        if (std::abs(ta.jteta) > 2) {
            itrackOffset += ntracks;
            continue;
        }

        // b-jet cut -> TODO: Switch to btag
        if (ta.jtHadFlav[ijet] != 5) {
                itrackOffset += ntracks;
                continue;
        }

        TGraph *gr_jet = new TGraph();
        TGraph *gr_match = new TGraph();

        // Go over tracks
        for (Int_t itrack = 0; itrack < ntracks; itrack++) {
            gr_jet->SetPoint(gr_jet->GetN(), ta.ipPhi[itrackOffset + itrack], ta.ipPhi[itrackOffset + itrack]);

            if (ta.ipMatchStatus[itrackOffset + itrack] >= 100) {
                gr_match->SetPoint(gr_match->GetN(), ta.ipPhiMatch[itrackOffset + itrack], ta.ipPhiMatch[itrackOffset + itrack]);
            }
        } // track loop

        gr_jet->SetMarkerStyle(kOpenDiamond);
        gr_jet->SetMarkerColor(jetColor);
        gr_jet->SetMarkerSize(4);
        mg->Add(gr_jet);

        gr_match->SetMarkerStyle(kOpenCircle);
        gr_match->SetMarkerColor(jetColor);
        gr_jet->SetMarkerSize(1);
        mg->Add(gr_match);

        jetColor++;
        itrackOffset += ntracks;
    } // jet loop
    
    

	TCanvas *c = new TCanvas("c", "", 1000, 800);
    mg->Draw("pa");
	c->Draw();

}

