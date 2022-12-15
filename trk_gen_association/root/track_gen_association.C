#include <TTree.h>
#include <TFile.h>
#include <TString.h>

#include <limits>

#include "hiTree.h"
#include "tTree.h"

void track_gen_association()
{
    // Load trees with tracks and gen particles 
    TString fdir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_LowPU_10000events";
    TString fname = label + ".root";
    TString rootf = fdir + fname;

    tTree t = tTree(rootf);
    hiTree hi = hiTree(rootf);

    std::vector<TString> activeBranches_t = {"nref", "jtHadFlav", "jtpt", "jteta", "jtphi",
                                             "ntrk", "trkJetId", "trkPt", "trkEta", "trkPhi"};
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    std::vector<TString> activeBranches_hi = {"mult", "pt", "eta", "phi", "sta"};
    hi.SetBranchStatus("*", 0);
    hi.SetBranchStatus(activeBranches_hi, 1);

    // Create new tree
    TString rootf_out = "./rootf/" + label + "_matched.root";
    TFile *fout = new TFile(rootf_out, "recreate");
    std::cout << "(Re)creating " << rootf_out << " file." << std::endl;

    TTree *matches = new TTree("matches", "tree with track-gen matches");

    Long64_t evt = 0;

    Int_t jtId = 0;
    Int_t jtFlav = 0;
    Float_t jtPt = 0.;
    Float_t jtEta = 0.;
    Float_t jtPhi = 0.;

    Float_t trkPt = 0.;
    Float_t trkEta = 0.;
    Float_t trkPhi = 0.;

    Int_t matchId = 0;
    Float_t matchPt = 0.;
    Float_t matchEta = 0.;
    Float_t matchPhi = 0.;
    Float_t matchDR = 0.;
    Float_t matchRelPt = 0.;
    Int_t matchSta = 0;

    Int_t matchIdCut = 0;
    Float_t matchPtCut = 0.;
    Float_t matchEtaCut = 0.;
    Float_t matchPhiCut = 0.;
    Float_t matchDRCut = 0.;
    Float_t matchRelPtCut = 0.;
    Int_t matchStaCut = 0;

    matches->Branch("evt", &evt, "evt/L");
    
    matches->Branch("jtId", &jtId, "jtId/I");
    matches->Branch("jtFlav", &jtFlav, "jtFlav/I");
    matches->Branch("jtPt", &jtPt, "jtPt/F");
    matches->Branch("jtEta", &jtEta, "jtEta/F");
    matches->Branch("jtPhi", &jtPhi, "jtPhi/F");

    matches->Branch("trkPt", &trkPt,"trkPt/F");
    matches->Branch("trkEta", &trkEta,"trkEta/F");
    matches->Branch("trkPhi", &trkPhi,"trkPhi/F");

    matches->Branch("matchId", &matchId,"matchId/I");
    matches->Branch("matchPt", &matchPt,"matchPt/F");
    matches->Branch("matchEta", &matchEta,"matchEta/F");
    matches->Branch("matchPhi", &matchPhi,"matchPhi/F");
    matches->Branch("matchDR", &matchDR,"matchDR/F");
    matches->Branch("matchRelPt", &matchRelPt,"matchRelPt/F");
    matches->Branch("matchSta", &matchSta,"matchSta/I");

    matches->Branch("matchIdCut", &matchIdCut,"matchIdCut/I");
    matches->Branch("matchPtCut", &matchPtCut,"matchPtCut/F");
    matches->Branch("matchEtaCut", &matchEtaCut,"matchEtaCut/F");
    matches->Branch("matchPhiCut", &matchPhiCut,"matchPhiCut/F");
    matches->Branch("matchDRCut", &matchDRCut,"matchDRCut/F");
    matches->Branch("matchRelPtCut", &matchRelPtCut,"matchRelPtCut/F");
    matches->Branch("matchStaCut", &matchStaCut,"matchStaCut/I");

    Long64_t nentries = t.GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        evt = ient;

        // Print progress
        if (ient % 1000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        t.GetEntry(ient);
        hi.GetEntry(ient);

        for (Int_t itrk = 0; itrk < t.ntrk; itrk++) {
            // Initialize the matched variables
            matchId = -1;
            matchPt = 0.;
            matchEta = 0.;
            matchPhi = 0.;
            matchDR = -1.;
            matchRelPt = -1.;
            matchSta = -1;

            matchIdCut = -1;
            matchPtCut = 0.;
            matchEtaCut = 0.;
            matchPhiCut = 0.;
            matchDRCut = -1.;
            matchRelPtCut = -1.;
            matchStaCut = -1;

            jtId = t.trkJetId[itrk];
            jtFlav = t.jtHadFlav[jtId];
            jtPt = t.jtpt[jtId];
            jtEta = t.jteta[jtId];
            jtPhi = t.jtphi[jtId];

            trkPt = t.trkPt[itrk];
            trkEta = t.trkEta[itrk];
            trkPhi = t.trkPhi[itrk];

            ROOT::Math::PtEtaPhiMVector vTrack;
            vTrack.SetPt(trkPt);
            vTrack.SetEta(trkEta);
            vTrack.SetPhi(trkPhi);
            
            Float_t dRMin = std::numeric_limits<float>::max();
            Float_t dRMinCut = std::numeric_limits<float>::max();

            for (Int_t ipart = 0; ipart < hi.mult; ipart++) {
                Float_t genPt = (*hi.pt)[ipart];
                if (genPt < 0.5) continue;

                Float_t genEta = (*hi.eta)[ipart];
                Float_t genPhi = (*hi.phi)[ipart];
                Int_t genSta = (*hi.sta)[ipart];

                ROOT::Math::PtEtaPhiMVector vGen;
                vGen.SetPt(genPt);
                vGen.SetEta(genEta);
                vGen.SetPhi(genPhi);

                Float_t dR = ROOT::Math::VectorUtil::DeltaR(vTrack, vGen);
                Float_t relPt = trkPt / genPt;

                if (dR < dRMin) {
                    dRMin = dR;

                    matchId = ipart;
                    matchPt = genPt;
                    matchEta = genEta;
                    matchPhi = genPhi;
                    matchDR = dR;
                    matchRelPt = relPt;
                    matchSta = genSta;
                }

                if ((dR < 0.02) && (relPt > 0.8) && (relPt < 1.2)) {
                    if (dR < dRMinCut) {
                        dRMinCut = dR;

                        matchIdCut = ipart;
                        matchPtCut = genPt;
                        matchEtaCut = genEta;
                        matchPhiCut = genPhi;
                        matchDRCut = dR;
                        matchRelPtCut = relPt;
                        matchStaCut = genSta;
                    }
                }
            } // end gen particle loop
            matches->Fill();
        } // end track loop        
    } // end entries loop
    matches->Write("", TObject::kOverwrite);
    fout->Close();
    delete fout;
}

