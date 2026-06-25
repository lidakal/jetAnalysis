#include "../draw_utils.h"

std::vector<TString> variations = {
    "FSRup", "FSRdown",
    "ISRup", "ISRdown",
    "Bothup", "Bothdown",
};

void calculate_correction_pythia_var(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{  
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch}";

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;
    suffix += "_pythia_var";

    TString sample = "pythia_PF40";
    TString label = "aggrTMVA";

    bool sfDown = false;
    bool sfUp = false;
    suffix += "_withSF";
    if (sfUp) suffix += "Up";
    if (sfDown) suffix += "Down";

    // Load histograms
    TFile *fin = new TFile("histos/"+sample+"_"+label+"_inclusive_histograms"+suffix+".root");

    // One correction per variation 
    std::vector<TH2D *> histos_inclusive;
    std::vector<TH2D *> histos_btag;
    std::vector<TH2D *> histos_btag_withSF;
    std::vector<TH2D *> histos_eff;
    std::vector<TH2D *> histos_eff_withSF;

    for (int ivar=0; ivar<6; ivar++) {
        TString var = variations[ivar];

        TH2D *h_inclusive = (TH2D *) fin->Get(Form("h_%s_%spt_gen", var.Data(), observable.Data()));
        TH2D *h_btag = (TH2D *) fin->Get(Form("h_%s_%spt_gen_tagged", var.Data(), observable.Data()));
        TH2D *h_btag_withSF = (TH2D *) fin->Get(Form("h_%s_%spt_gen_tagged_withSF", var.Data(), observable.Data()));

        TH2D *h_eff = (TH2D *) h_btag->Clone(Form("h_%s_eff", var.Data()));
        h_eff->Divide(h_btag, h_inclusive, 1., 1., "b");

        TH2D *h_eff_withSF = (TH2D *) h_btag_withSF->Clone(Form("h_%s_eff_withSF", var.Data()));
        h_eff_withSF->Divide(h_btag_withSF, h_inclusive, 1., 1., "b");

        histos_inclusive.push_back(h_inclusive);
        histos_btag.push_back(h_btag);
        histos_btag_withSF.push_back(h_btag_withSF);
        histos_eff.push_back(h_eff);
        histos_eff_withSF.push_back(h_eff_withSF);
    }

    TString fout_name = "./histos/"+sample+"_"+label+"_XXT_" + observable + "_efficiency"+suffix+".root";
    std::cout << "fout : " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");


    for (int ivar=0; ivar<6; ivar++) {
        histos_inclusive[ivar]->Write();
        histos_btag_withSF[ivar]->Write();
        histos_eff_withSF[ivar]->Write();
        histos_btag[ivar]->Write();
        histos_eff[ivar]->Write();
    }

    fout->Close();
    delete fout;

    // gApplication -> Terminate(0);
}