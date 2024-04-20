#include "draw_utils.h"

void draw_aggr_vs_not(TString observable="rg")
{
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetPalette(57);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";

    TString fin_aggr_name = "./histos/bjet_aggrTMVA_newJP_substructure.root"; 
    std::cout << "fin aggr: " << fin_aggr_name << std::endl;
    TFile *fin_aggr = new TFile(fin_aggr_name);
    TH2F *hSingleBtag_reco_aggr = (TH2F *) fin_aggr->Get("hSingleBtag_"+observable)->Clone("hSingleBtag_reco_aggr");
    TH2F *hSingleBtag_gen_aggr = (TH2F *) fin_aggr->Get("hSingleBtag_"+observable+"_gen")->Clone("hSingleBtag_gen_aggr");

    TString fin_noAggr_name = "./histos/bjet_noAggr_withPNET_substructure.root"; 
    std::cout << "fin no aggr: " << fin_noAggr_name << std::endl;
    TFile *fin_noAggr = new TFile(fin_noAggr_name);
    TH2F *hSingleBtag_reco_noAggr = (TH2F *) fin_noAggr->Get("hSingleBtag_"+observable)->Clone("hSingleBtag_reco_noAggr");

    // Make projections
    int min_kt_bin = hSingleBtag_reco_aggr->GetYaxis()->FindBin(0.);
    int max_kt_bin = hSingleBtag_reco_aggr->GetNbinsY();

    TH1F *hSingleBtag_gen_aggr_1d = (TH1F *) hSingleBtag_gen_aggr->ProjectionX("hSingleBtag_gen_aggr_1d", min_kt_bin, max_kt_bin);
    hSingleBtag_gen_aggr_1d->Scale(1/hSingleBtag_gen_aggr_1d->Integral());
    hSingleBtag_gen_aggr_1d->SetLineColor(kBlue-2);
    hSingleBtag_gen_aggr_1d->SetLineWidth(2);
    hSingleBtag_gen_aggr_1d->GetXaxis()->SetTitle(xlabel);
    hSingleBtag_gen_aggr_1d->GetYaxis()->SetTitle("normalized per jet");
    hSingleBtag_gen_aggr_1d->GetYaxis()->SetRangeUser(0., 0.25);
    hSingleBtag_gen_aggr_1d->SetLineStyle(1);

    TH1F *hSingleBtag_reco_aggr_1d = (TH1F *) hSingleBtag_reco_aggr->ProjectionX("hSingleBtag_reco_aggr_1d", min_kt_bin, max_kt_bin);
    hSingleBtag_reco_aggr_1d->Scale(1/hSingleBtag_reco_aggr_1d->Integral());
    hSingleBtag_reco_aggr_1d->SetLineColor(kGreen+2);
    hSingleBtag_reco_aggr_1d->SetLineWidth(2);    
    hSingleBtag_reco_aggr_1d->SetLineStyle(9);

    TH1F *hSingleBtag_reco_noAggr_1d = (TH1F *) hSingleBtag_reco_noAggr->ProjectionX("hSingleBtag_reco_noAggr_1d", min_kt_bin, max_kt_bin);
    hSingleBtag_reco_noAggr_1d->Scale(1/hSingleBtag_reco_noAggr_1d->Integral());
    hSingleBtag_reco_noAggr_1d->SetLineColor(kRed+1);
    hSingleBtag_reco_noAggr_1d->SetLineWidth(2); 
    hSingleBtag_reco_noAggr_1d->SetLineStyle(2);

    TCanvas *c_aggr_vs_noAggr = new TCanvas("c_aggr_vs_noAggr", "", 800, 600);
    c_aggr_vs_noAggr->SetRightMargin(0.16);
    hSingleBtag_gen_aggr_1d->Draw("hist");
    hSingleBtag_reco_noAggr_1d->Draw("hist same");
    hSingleBtag_reco_aggr_1d->Draw("hist same");
    drawHeaderSimulation();

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    jet_info->DrawLatex(0.42, 0.83, "anti-k_{T}, R=0.4 b-tagged single b jets");
    jet_info->DrawLatex(0.48, 0.78, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info->DrawLatex(0.56, 0.73, "Charged particles only");
    jet_info->DrawLatex(0.68, 0.68, "k_{T} > 1 GeV");

    TLegend *leg = new TLegend(0.48, 0.44, 0.85, 0.62);
    leg->SetFillStyle(0);
    leg->SetMargin(0.1);
    leg->AddEntry(hSingleBtag_gen_aggr_1d, "Particle level, aggregated", "l");
    leg->AddEntry(hSingleBtag_reco_noAggr_1d, "Detector level, not aggregated", "l");
    leg->AddEntry(hSingleBtag_reco_aggr_1d, "Detector level, aggregated", "l");
    leg->Draw();

    c_aggr_vs_noAggr->Print("plots_an/"+observable+"_aggr_vs_not.png");
    c_aggr_vs_noAggr->Print("plots_an/"+observable+"_aggr_vs_not.pdf");

}
