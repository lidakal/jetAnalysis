#include "myPalette.h"
#include "response_matrices/draw_utils.h"

void draw_response_poster()
{
    Float_t font_size = 32.;
    gStyle->SetPalette(57);
    
    TFile *fin_aggrGenNoReco = new TFile("./histos_for_poster/aggrGenNoReco_withY.root");
    TH2F *hBtag_response_noAggr = (TH2F *) fin_aggrGenNoReco->Get("hBtag_2d");
    TString temp2 = hBtag_response_noAggr->GetName() + (TString) "_noAggr";
    hBtag_response_noAggr->SetName(temp2);
    format_probability(hBtag_response_noAggr, "ln(1/R_{g})");
    hBtag_response_noAggr->GetXaxis()->SetTitle("Detector level, non-aggregated, ln(1/R_{g})");
    hBtag_response_noAggr->GetXaxis()->SetTitleSize(font_size);
    hBtag_response_noAggr->GetXaxis()->SetLabelSize(font_size);
    hBtag_response_noAggr->GetYaxis()->SetTitleSize(font_size);
    hBtag_response_noAggr->GetYaxis()->SetLabelSize(font_size);
    hBtag_response_noAggr->GetZaxis()->SetLabelSize(font_size);
    hBtag_response_noAggr->GetZaxis()->SetRangeUser(0., 0.7);
    // hBtag_response_noAggr->GetXaxis()->CenterTitle(true);
    // hBtag_response_noAggr->GetYaxis()->CenterTitle(true);

    TFile *fin_aggrTMVA = new TFile("./histos_for_poster/aggrTMVA_withY.root");
    TH2F *hBtag_response_aggrTMVA = (TH2F *) fin_aggrTMVA->Get("hBtag_2d");
    format_probability(hBtag_response_aggrTMVA, "ln(1/R_{g})");
    hBtag_response_aggrTMVA->GetXaxis()->SetTitle("Detector level, aggregated with MVA, ln(1/R_{g})");
    hBtag_response_aggrTMVA->GetXaxis()->SetTitleSize(font_size);
    hBtag_response_aggrTMVA->GetXaxis()->SetLabelSize(font_size);
    hBtag_response_aggrTMVA->GetYaxis()->SetTitleSize(font_size);
    hBtag_response_aggrTMVA->GetYaxis()->SetLabelSize(font_size);
    hBtag_response_aggrTMVA->GetZaxis()->SetLabelSize(font_size);
    hBtag_response_aggrTMVA->GetZaxis()->SetRangeUser(0., 0.7);
    // hBtag_response_aggrTMVA->GetXaxis()->SetRangeUser(0.91, 3.8);
    // hBtag_response_aggrTMVA->GetXaxis()->CenterTitle(true);

    TPaveText *info_top_left = new TPaveText(0.9, 4.05, 2.25, 4.15, "nb ndc");
    info_top_left->SetTextSize(font_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Internal Simulation}");

    TPaveText *info_top_right = new TPaveText(2.55, 4.05, 4., 4.15, "nb ndc");
    info_top_right->SetTextSize(font_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 13 TeV #it{pp}");

    TPaveText *info_jets = new TPaveText(3., 0.6, 4., 0.9, "nb ndc");
    info_jets->SetTextSize(font_size);
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
    c_tmva->Print("./plots_for_poster/response_aggrTMVA.png");

    // TCanvas *c_both = new TCanvas("c_both", "", 2000, 1000);
    // c_both->Divide(2, 1, 0.0, 0.01);

    // c_both->cd(1);
    // // std::cout << gPad->GetWw() << std::endl;
    // hBtag_response_noAggr->Draw("col");
    // gPad->SetFixedAspectRatio();
    // // SetRealAspectRatio(c_both->cd(1));

    // c_both->cd(2);
    // hBtag_response_aggrTMVA->Draw("colz");
    // gPad->SetFixedAspectRatio();
    // // SetRealAspectRatio(c_both->cd(2));

    // c_both->Draw();


}