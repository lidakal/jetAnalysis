#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TString.h>

void plot_track_gen_association()
{
    // Load the tree
    TString fdir = "./rootf/";
    TString label = "HiForestMiniAOD_LowPU_10000events";
    TString fname = label + "_matched.root";
    TString rootf = fdir + fname;

    TFile *fin = new TFile(rootf);
    TTree *matches = (TTree *)fin->Get("matches");

    Long64_t evt = 0;

    Int_t jtId = 0;
    Int_t jtFlav = 0;
    Float_t jtPt = 0.;
    Float_t jtEta = 0.;
    Float_t jtPhi = 0.;

    Float_t trkPt = 0.;
    Float_t trkEta = 0.;
    Float_t trkPhi = 0.;

    Int_t matchId = -1;
    Float_t matchPt = 0.;
    Float_t matchEta = 0.;
    Float_t matchPhi = 0.;
    Float_t matchDR = 0.;
    Float_t matchRelPt = 0.;
    Int_t matchSta = 0;

    Int_t matchIdCut = -1;
    Float_t matchPtCut = 0.;
    Float_t matchEtaCut = 0.;
    Float_t matchPhiCut = 0.;
    Float_t matchDRCut = 0.;
    Float_t matchRelPtCut = 0.;
    Int_t matchStaCut = 0;

    matches->SetBranchAddress("evt", &evt);
    
    matches->SetBranchAddress("jtId", &jtId);
    matches->SetBranchAddress("jtFlav", &jtFlav);
    matches->SetBranchAddress("jtPt", &jtPt);
    matches->SetBranchAddress("jtEta", &jtEta);
    matches->SetBranchAddress("jtPhi", &jtPhi);

    matches->SetBranchAddress("trkPt", &trkPt);
    matches->SetBranchAddress("trkEta", &trkEta);
    matches->SetBranchAddress("trkPhi", &trkPhi);

    matches->SetBranchAddress("matchId", &matchId);
    matches->SetBranchAddress("matchPt", &matchPt);
    matches->SetBranchAddress("matchEta", &matchEta);
    matches->SetBranchAddress("matchPhi", &matchPhi);
    matches->SetBranchAddress("matchDR", &matchDR);
    matches->SetBranchAddress("matchRelPt", &matchRelPt);
    matches->SetBranchAddress("matchSta", &matchSta);

    matches->SetBranchAddress("matchIdCut", &matchIdCut);
    matches->SetBranchAddress("matchPtCut", &matchPtCut);
    matches->SetBranchAddress("matchEtaCut", &matchEtaCut);
    matches->SetBranchAddress("matchPhiCut", &matchPhiCut);
    matches->SetBranchAddress("matchDRCut", &matchDRCut);
    matches->SetBranchAddress("matchRelPtCut", &matchRelPtCut);
    matches->SetBranchAddress("matchStaCut", &matchStaCut);

    // Create the histograms 
    Int_t x1bins = 50;
    Float_t x1min = 0.;
    Float_t x1max = 0.04;

    TH1F *hdr = new TH1F("hdr", "dr for matches", x1bins, x1min, x1max);
    TH1F *hdrCut = new TH1F("hdrCut", "dr for matches with cuts", x1bins, x1min, x1max);

    Int_t x2bins = 50;
    Float_t x2min = 0.6;
    Float_t x2max = 1.4;

    TH1F *hpt = new TH1F("hpt", "relpt for matches", x2bins, x2min, x2max);
    TH1F *hptCut = new TH1F("hptCut", "relpt for matches with cuts", x2bins, x2min, x2max);

    Long64_t nentries = matches->GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 10000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        matches->GetEntry(ient);

        hdr->Fill(matchDR);
        hdrCut->Fill(matchDRCut);

        hpt->Fill(matchRelPt);
        hptCut->Fill(matchRelPtCut);
    } // end entries (track) loop 

    // Save histograms 
    TString rootf_out = "./histos/" + label + "_histos.root";
    TFile *fout = new TFile(rootf_out, "recreate");
    std::cout << "(Re)creating " << rootf_out << " file." << std::endl;

    for (TH1F *h : {hdr, hdrCut, hpt, hptCut}) {
        h->Write();
    }

    fout->Close();
}