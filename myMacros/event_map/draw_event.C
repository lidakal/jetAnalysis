#include "../aggregation/TreeAnalyzer.h"
#include "../aggregation/selections.h" // passSelection(int Selection, bool inSV, float ip3dSig)
#include "hiTreeAnalyzer.h"

#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TCanvas.h"

void draw_event(Long64_t ient)
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

    TMultiGraph *mg;
    TLegend *leg = new TLegend(0.2, 0.6, 0.4, 0.8);

    ta.GetEntry(ient);
    hita.GetEntry(ient);

    // Draw gen particles
    std::vector<Float_t> phi_gen;
    std::vector<Float_t> eta_gen;

    std::vector<Float_t> phi_genb;
    std::vector<Float_t> eta_genb;

    for (Int_t ipar = 0; ipar < hita.mult; ipar++) {
        Float_t phi = (*hita.phi)[ipar];
        Float_t eta = (*hita.eta)[ipar];
        Int_t sta = (*hita.sta)[ipar];

        phi_gen.push_back(phi);
        eta_gen.push_back(eta);

        if (sta >= 100) {
            phi_genb.push_back(phi);
            eta_genb.push_back(eta);
        }
    } // gen particle loop

    TGraph *gr_gen = new TGraph(phi_gen.size(), &phi_gen[0], &eta_gen[0]); 
    gr_gen->SetMarkerStyle(kOpenCircle);
    gr_gen->SetMarkerColorAlpha(kBlack, 0.5);
    mg->Add(gr_gen);

    TGraph *gr_genb = new TGraph(phi_genb.size(), &phi_genb[0], &eta_genb[0]); 
    gr_genb->SetMarkerStyle(kFullCircle);
    gr_genb->SetMarkerColorAlpha(kBlack, 0.5);
    mg->Add(gr_genb);




    // Go over jets in event
    /*
    Int_t itrackOffset = 0;
    for (Int_t ijet = 0; ijet < ta)
    */
    

    TCanvas *c = new TCanvas("c", "", 1000, 800);
    mg->Draw("pa");
    c->Draw();

}

