#include "tTree.h"

void plot_hlt()
{
    TString sample = "HighEGJet";
    // TString label = "aggrTMVA_withHighPtHLT";
    TString label = "aggrTMVA_particleNet";

    TString fin = "/data_CMS/cms/kalipoliti/bJet2017G/" + sample + "/" + label + "/merged_HiForestMiniAOD.root";
    tTree t(fin);

    std::vector<TString> activeBranches = {"nref",
                                           "jtpt", "jteta", "jtHadFlav",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                           "rsjt1Phi", "rsjt1Y", "rsjt1Pt", "rsjt1Eta",
                                           "rsjt2Phi", "rsjt2Y", "rsjt2Pt", "rsjt2Eta",
                                           "sjt1Phi", "sjt1Y", "sjt1Pt", "sjt1Eta",
                                           "sjt2Phi", "sjt2Y", "sjt2Pt", "sjt2Eta",
                                           "HLT_HIAK4CaloJet30_v1", "HLT_HIAK4CaloJet40_v1", "HLT_HIAK4CaloJet60_v1",
                                           "HLT_HIAK4CaloJet80_v1", "HLT_HIAK4CaloJet100_v1"
                                           };

    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches, 1);

    // Int_t x1bins = 20;
    // Float_t x1min = 60.;
    // Float_t x1max = 160.;

    // TH1D *hPass80 = new TH1D("hPass80", "", x1bins, x1min, x1max);
    // TH1D *hPass60 = new TH1D("hPass60", "", x1bins, x1min, x1max);
    // TH1D *hPass60butNot80 = new TH1D("hPass60butNot80", "", x1bins, x1min, x1max);
    // TH1D *hPass60butNot80 = new TH1D("hPass60butNot80", "", x1bins, x1min, x1max);

    TH2D *hHLT60 = new TH2D("hHLT60", "x=pass60,y=pass80", 2, 0, 2, 2, 0, 2);
    TH2D *hHLT40 = new TH2D("hHLT40", "x=pass40,y=pass80", 2, 0, 2, 2, 0, 2);

    // double pass80 = 0;
    // double pass60 = 0;
    // double pass60butNot80 = 0;
    // double pass60and80 = 0;

    Long64_t total_events = 1018211727;
    Long64_t nentries = t.GetEntries();
    if (total_events > nentries) total_events = nentries;

    for (Long64_t ient = 0; ient < total_events; ient++) {
        // Print progress
        if (ient % 10000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        t.GetEntry(ient);

        // double leadPt = 0.;
        // if (t.nref > 0) leadPt = t.jtpt[0]; else continue;
        // if (leadPt < 60.) continue;

        // if (t.HLT_HIAK4CaloJet60_v1 == 1 && leadPt > 80) {
        //     pass60 += 1.;
        //     hPass60->Fill(leadPt);

        //     if (t.HLT_HIAK4CaloJet80_v1 != 1) {
        //         pass60butNot80 += 1.;
        //         hPass60butNot80->Fill(leadPt);
        //     }
        // }

        // if (t.HLT_HIAK4CaloJet80_v1 == 1 && leadPt > 100) {
        //     pass80 += 1.;
        //     hPass80->Fill(leadPt);
        // }

        double pass40 = 0.1;
        double pass60 = 0.1;
        double pass80 = 0.1;

        if (t.HLT_HIAK4CaloJet40_v1 == 1) pass40 = 1.1;
        if (t.HLT_HIAK4CaloJet60_v1 == 1) pass60 = 1.1;
        if (t.HLT_HIAK4CaloJet80_v1 == 1) pass80 = 1.1;

        hHLT60->Fill(pass60, pass80);
        hHLT40->Fill(pass40, pass80);
    }

    // double prescale_factor = pass80 / pass60;
    // std::cout << "prescale factor " << prescale_factor << std::endl;

    TString fout_name = "./histos/" + sample + "_prescale.root";
    TFile *fout = new TFile(fout_name, "recreate");
    // for (auto h : {hPass60, hPass80, hPass60butNot80}) {
    //     h->Write();
    // }

    hHLT60->Write();
    hHLT40->Write();

    fout->Close();
    delete fout;
}