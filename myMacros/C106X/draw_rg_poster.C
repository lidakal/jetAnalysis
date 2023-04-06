#include "myPalette.h"

void draw_rg_poster()
{
    TFile *fin_aggrGenNoReco = new TFile("./histos_for_poster/aggrGenNoReco_withY.root");
    TH1F *hBtag_gen = (TH1F *) fin_aggrGenNoReco->Get("hBtag_gen");
    TH1F *hBtag_noAggr = (TH1F *) fin_aggrGenNoReco->Get("hBtag");
    TString temp = hBtag_noAggr->GetName() + (TString) "_noAggr";
    hBtag_noAggr->SetName(temp);
    TH1F *hL_gen = (TH1F *) fin_aggrGenNoReco->Get("hL_gen");
    

    TFile *fin_aggrTMVA = new TFile("./histos_for_poster/aggrTMVA_withY.root");
    TH1F *hBtag_aggrTMVA = (TH1F *) fin_aggrTMVA->Get("hBtag");

    THStack *h = new THStack("h", "");
    h->SetTitle("; ln(1/R_{g}); 1/N_{2-prong jets} dN / d(ln(1/R_{g}))");
    TLegend *leg = new TLegend(0.4, 0.75, 0.8, 0.85);
    leg->SetFillStyle(0);

    // hL_gen->Scale(1/hL_gen->Integral("width"));
    // hL_gen->SetLineColor(mykRed);
    // hL_gen->SetLineWidth(2);
    // h->Add(hL_gen);
    // leg->AddEntry(hL_gen, "gusd jets, particle level", "l");

    hBtag_gen->Scale(1/hBtag_gen->Integral("width"));
    hBtag_gen->SetLineColor(mykBlue);
    hBtag_gen->SetLineWidth(3);
    h->Add(hBtag_gen);
    leg->AddEntry(hBtag_gen, "b jets, particle level", "l");

    hBtag_noAggr->Scale(1/hBtag_noAggr->Integral("width"));
    hBtag_noAggr->SetLineColor(mykRedLight);
    hBtag_noAggr->SetLineWidth(3);
    hBtag_noAggr->SetLineStyle(2);
    h->Add(hBtag_noAggr);
    leg->AddEntry(hBtag_noAggr, "b jets, detector level, non-aggregated", "l");

    hBtag_aggrTMVA->Scale(1/hBtag_aggrTMVA->Integral("width"));
    hBtag_aggrTMVA->SetLineColor(mykRed);
    hBtag_aggrTMVA->SetLineWidth(3);
    h->Add(hBtag_aggrTMVA);
    leg->AddEntry(hBtag_aggrTMVA, "b jets, detector level, aggregated with MVA", "l");

    TPaveText *info_top_left = new TPaveText(0.9, 1.4, 1.8, 1.5, "nb ndc");
    info_top_left->SetTextSize(20);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Simulation Preliminary}");

    TPaveText *info_top_right = new TPaveText(3.1, 1.4, 4., 1.5, "nb ndc");
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

    TCanvas *c = new TCanvas("c", "", 1200, 1000);
    h->Draw("histo nostack");
    h->SetMaximum(1.4);
    leg->Draw();
    info_top_left->Draw();
    info_top_right->Draw();
    info_jets->Draw();
    c->Draw();
    c->SetGrid();
    c->Print("./plots_for_poster/rg.png");
}