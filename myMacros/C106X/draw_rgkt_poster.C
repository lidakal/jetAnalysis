#include "myPalette.h"

void draw_rgkt_poster()
{
    Float_t font_size = 32.;
    gStyle->SetPalette(57);

    TFile *fin_aggrGenNoReco = new TFile("./histos_for_poster/aggrGenNoReco_withY.root");
    TH2F *hBtag_aggr = (TH2F *) fin_aggrGenNoReco->Get("hBtag_rgkt_gen");
    TString temp = hBtag_aggr->GetName() + (TString) "_aggr";
    hBtag_aggr->SetName(temp);
    hBtag_aggr->Scale(1/hBtag_aggr->Integral());
    hBtag_aggr->GetXaxis()->SetTitle("ln(0.4/R_{g})");
    hBtag_aggr->GetXaxis()->SetTitleSize(font_size);
    hBtag_aggr->GetXaxis()->SetLabelSize(font_size);
    hBtag_aggr->GetYaxis()->SetTitle("ln(k_{T})");
    hBtag_aggr->GetYaxis()->SetTitleSize(font_size);    
    hBtag_aggr->GetYaxis()->SetLabelSize(font_size);
    hBtag_aggr->GetYaxis()->SetTitleOffset(1.);
    hBtag_aggr->GetZaxis()->SetLabelSize(font_size);
    hBtag_aggr->GetZaxis()->SetRangeUser(0, 0.015);

    TFile *fin_noAggr = new TFile("./histos_for_poster/noAggr_withY.root");
    TH2F *hBtag_noAggr = (TH2F *) fin_noAggr->Get("hBtag_rgkt_gen");
    hBtag_noAggr->Scale(1/hBtag_noAggr->Integral());
    hBtag_noAggr->GetXaxis()->SetTitle("ln(0.4/R_{g})");
    hBtag_noAggr->GetXaxis()->SetTitleSize(font_size);
    hBtag_noAggr->GetXaxis()->SetLabelSize(font_size);
    hBtag_noAggr->GetYaxis()->SetTitle("ln(k_{T})");
    hBtag_noAggr->GetYaxis()->SetTitleSize(font_size);
    hBtag_noAggr->GetYaxis()->SetLabelSize(font_size);
    hBtag_noAggr->GetYaxis()->SetTitleOffset(1.);
    hBtag_noAggr->GetZaxis()->SetLabelSize(font_size);
    hBtag_noAggr->GetZaxis()->SetRangeUser(0, 0.015);


    TPaveText *info_top_left = new TPaveText(0., 3., 1.6, 3.4, "nb ndc");
    info_top_left->SetTextSize(font_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Internal Simulation}");

    TPaveText *info_top_right = new TPaveText(2.2, 3., 4., 3.4, "nb ndc");
    info_top_right->SetTextSize(font_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");

    TPaveText *info_jets = new TPaveText(1.9, 1., 3.9, 2.5, "nb ndc");
    info_jets->SetTextSize(font_size);
    info_jets->SetFillStyle(0);
    info_jets->SetLineWidth(0);
    info_jets->AddText("Particle level b-tagged b-jets ");
    info_jets->AddText("80 < #it{p}_{T}^{jet} < 100 GeV");
    info_jets->AddText("-2 < #it{#eta}^{jet} < 2");
    info_jets->AddText("");

    double max_z = 0.03;

    TCanvas *c_aggr = new TCanvas("c_aggr", "", 1200, 1000);
    hBtag_aggr->Draw("colz");
    hBtag_aggr->SetMaximum(max_z);
    info_top_left->Draw();
    info_top_right->Draw();
    info_jets->Draw();
    c_aggr->Draw();
    c_aggr->SetGrid();
    c_aggr->Print("./plots_for_poster/rgkt_aggr.png");

    TCanvas *c_noAggr = new TCanvas("c_noAggr", "", 1200, 1000);
    hBtag_noAggr->Draw("colz");
    hBtag_noAggr->SetMaximum(max_z);
    info_top_left->Draw();
    info_top_right->Draw();
    info_jets->Draw();
    c_noAggr->Draw();
    c_noAggr->SetGrid();
    c_noAggr->Print("./plots_for_poster/rgkt_noAggr.png");
}