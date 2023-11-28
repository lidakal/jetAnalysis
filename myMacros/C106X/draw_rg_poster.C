#include "myPalette.h"

void draw_rg_poster()
{
    Float_t font_size = 30.;

    // no aggr b jet
    TFile *fin_noAggr = new TFile("./histos_for_poster/bjet_noAggr_withY_rg.root");
    TH1F *hBtag_gen_noAggr = (TH1F *) fin_noAggr->Get("hBtag_gen")->Clone("hBtag_gen_noAggr");
    TH1F *hBtag_noAggr = (TH1F *) fin_noAggr->Get("hBtag")->Clone("hBtag_noAggr");
    
    // aggr b jet
    TFile *fin_aggrTMVA = new TFile("./histos_for_poster/bjet_aggrTMVA_particleNet_rg.root");
    TH1F *hBtag_gen_aggrTMVA = (TH1F *) fin_aggrTMVA->Get("hBtag_gen")->Clone("hBtag_gen_aggrTMVA");
    TH1F *hBtag_aggrTMVA = (TH1F *) fin_aggrTMVA->Get("hBtag")->Clone("hBtag_aggrTMVA");

    // inclusive jet
    TFile *fin_inclusive = new TFile("./histos_for_poster/dijet_aggrTMVA_particleNet_rg.root");
    TH1F *hB_gen_inclusive = (TH1F *) fin_inclusive->Get("hB_gen")->Clone("hB_gen_inclusive");
    TH1F *hC_gen_inclusive = (TH1F *) fin_inclusive->Get("hC_gen")->Clone("hC_gen_inclusive");
    TH1F *hL_gen_inclusive = (TH1F *) fin_inclusive->Get("hL_gen")->Clone("hL_gen_inclusive");

    TH1F *hIncl_gen = (TH1F *) hB_gen_inclusive->Clone("hIncl_gen");
    hIncl_gen->Add(hC_gen_inclusive);
    hIncl_gen->Add(hL_gen_inclusive);

    THStack *h = new THStack("h", "");
    h->SetTitle("; ln(0.4/R_{g}); 1/N_{2-prong jets} dN / d(ln(0.4/R_{g}))");

    TLegend *leg = new TLegend(0.2, 0.75, 0.8, 0.85);
    leg->SetFillStyle(0);
    gStyle->SetLegendTextSize(font_size);


    hIncl_gen->Scale(1/hIncl_gen->Integral(),"width");
    hIncl_gen->SetLineColor(mykGreen);
    hIncl_gen->SetLineWidth(4);
    h->Add(hIncl_gen);
    leg->AddEntry(hIncl_gen, "Particle level, all jets", "l");

    hBtag_gen_noAggr->Scale(1/hBtag_gen_noAggr->Integral(),"width");
    hBtag_gen_noAggr->SetLineColor(mykBlue);
    hBtag_gen_noAggr->SetLineWidth(4);
    h->Add(hBtag_gen_noAggr);
    leg->AddEntry(hBtag_gen_noAggr, "Particle level, b jets, non-aggregated", "l");

    // hBtag_noAggr->Scale(1/hBtag_noAggr->Integral("width"));
    // hBtag_noAggr->SetLineColor(mykGreen);
    // hBtag_noAggr->SetLineWidth(3);
    // hBtag_noAggr->SetLineStyle(1);
    // h->Add(hBtag_noAggr);
    // leg->AddEntry(hBtag_noAggr, "Detector level, non-aggregated", "l");

    hBtag_gen_aggrTMVA->Scale(1/hBtag_gen_aggrTMVA->Integral(),"width");
    hBtag_gen_aggrTMVA->SetLineColor(mykRed);
    hBtag_gen_aggrTMVA->SetLineWidth(4);
    h->Add(hBtag_gen_aggrTMVA);
    leg->AddEntry(hBtag_gen_aggrTMVA, "Particle level, b jets, aggregated with MVA", "l");

    // hBtag_aggrTMVA->Scale(1/hBtag_aggrTMVA->Integral("width"));
    // hBtag_aggrTMVA->SetLineColor(mykRed);
    // hBtag_aggrTMVA->SetLineWidth(3);
    // h->Add(hBtag_aggrTMVA);
    // leg->AddEntry(hBtag_aggrTMVA, "Detector level, aggregated with MVA", "l");

    TPaveText *info_top_left = new TPaveText(0., 1.2, 2.1, 1.3, "nb ndc");
    info_top_left->SetTextSize(font_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Work in Progress Simulation}");

    TPaveText *info_top_right = new TPaveText(2.25, 1.2, 4., 1.3, "nb ndc");
    info_top_right->SetTextSize(font_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");

    TPaveText *info_jets = new TPaveText(2., 0.2, 3.7, 0.6, "nb ndc");
    info_jets->SetTextSize(font_size);
    info_jets->SetFillStyle(0);
    info_jets->SetLineWidth(0);
    // info_jets->AddText("b-tagged b-jets ");
    info_jets->AddText("80 < #it{p}_{T}^{jet} < 100 GeV");
    info_jets->AddText("-2 < #it{#eta}^{jet} < 2");
    info_jets->AddText("");
    info_jets->AddText("#it{k}_{T} > 1 GeV");

    TCanvas *c = new TCanvas("c", "", 1200, 1000);
    h->Draw("histo nostack");
    h->SetMaximum(1.2);
    h->GetXaxis()->SetLabelSize(font_size);
    h->GetXaxis()->SetTitleSize(font_size);
    h->GetYaxis()->SetLabelSize(font_size);
    h->GetYaxis()->SetTitleSize(font_size);
    leg->Draw();
    info_top_left->Draw();
    info_top_right->Draw();
    info_jets->Draw();
    c->Draw();
    c->SetGrid();
    c->Print("./plots_for_poster/rg.png");
}