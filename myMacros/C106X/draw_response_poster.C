#include "myPalette.h"
#include "response_matrices/draw_utils.h"

void draw_response_poster()
{
    gStyle->SetPalette(57);
    
    TFile *fin_aggrGenNoReco = new TFile("./histos_for_poster/aggrGenNoReco_withY.root");
    TH2F *hBtag_response_noAggr = (TH2F *) fin_aggrGenNoReco->Get("hBtag_2d");
    TString temp2 = hBtag_response_noAggr->GetName() + (TString) "_noAggr";
    hBtag_response_noAggr->SetName(temp2);
    format_probability(hBtag_response_noAggr, "ln(1/R_{g})");
    hBtag_response_noAggr->GetXaxis()->SetTitle("Detector level, non-aggregated, ln(1/R_{g})");

    TFile *fin_aggrTMVA = new TFile("./histos_for_poster/aggrTMVA_withY.root");
    TH2F *hBtag_response_aggrTMVA = (TH2F *) fin_aggrTMVA->Get("hBtag_2d");
    format_probability(hBtag_response_aggrTMVA, "ln(1/R_{g})");
    hBtag_response_aggrTMVA->GetXaxis()->SetTitle("Detector level, aggregated with MVA, ln(1/R_{g})");

    TPaveText *info_top_left = new TPaveText(0.9, 4., 2., 4.1, "nb ndc");
    info_top_left->SetTextSize(20);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Simulation Preliminary}");

    TPaveText *info_top_right = new TPaveText(2.9, 4., 4., 4.1, "nb ndc");
    info_top_right->SetTextSize(20);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 13 TeV #it{pp}");

    TPaveText *info_jets = new TPaveText(3., 0.6, 4., 0.9, "nb ndc");
    info_jets->SetTextSize(20);
    info_jets->SetFillStyle(0);
    info_jets->SetLineWidth(0);
    info_jets->AddText("b-tagged b-jets ");
    info_jets->AddText("50 < #it{p}_{T}^{jet} < 80 GeV");
    info_jets->AddText("-2 < #it{#eta}^{jet} < 2");
    info_jets->AddText("");
    info_jets->AddText("#it{k}_{T} > 1 GeV");

    TCanvas *c = new TCanvas("c", "", 1000, 1000);
    hBtag_response_noAggr->Draw("colz");
    info_top_left->Draw();
    info_top_right->Draw();
    // info_jets->Draw();
    c->Draw();
    SetRealAspectRatio(c);
    // c->SetGrid();
    c->Print("./plots_for_poster/response_noAggr.png");

    TCanvas *c_tmva = new TCanvas("c_tmva", "", 1000, 1000);
    hBtag_response_aggrTMVA->Draw("colz");
    info_top_left->Draw();
    info_top_right->Draw();
    // info_jets->Draw();
    c_tmva->Draw();
    // c_tmva->SetLogz();
    SetRealAspectRatio(c_tmva);
    // c->SetGrid();
    c->Print("./plots_for_poster/response_aggrTMVA.png");


}