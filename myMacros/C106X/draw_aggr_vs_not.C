#include "draw_utils.h"
#include "cms_palette.h"

void draw_info()
{
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.14, 0.93, "#bf{CMS} #it{Simulation}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.97, 0.93, "PYTHIA8 CP5 (pp 5.02 TeV)");
}

void draw_aggr_vs_not(TString observable="rg")
{
    // Run with ROOT 6.30, take PDFs

    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetPalette(57);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R^{}/^{}R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";

    TString fin_aggr_name = "./histos/bjet_aggrTMVA_fixedMassBug_substructure.root"; 
    std::cout << "fin aggr: " << fin_aggr_name << std::endl;
    TFile *fin_aggr = new TFile(fin_aggr_name);
    TH2F *hSingleBtag_reco_aggr = (TH2F *) fin_aggr->Get("hSingleBtag_"+observable)->Clone("hSingleBtag_reco_aggr");
    TH2F *hSingleBtag_gen_aggr = (TH2F *) fin_aggr->Get("hSingleBtag_"+observable+"_gen")->Clone("hSingleBtag_gen_aggr");

    TString fin_noAggr_name = "./histos/bjet_aggrGenNoReco_fixedMassBug_substructure.root"; 
    std::cout << "fin no aggr: " << fin_noAggr_name << std::endl;
    TFile *fin_noAggr = new TFile(fin_noAggr_name);
    TH2F *hSingleBtag_reco_noAggr = (TH2F *) fin_noAggr->Get("hSingleBtag_"+observable)->Clone("hSingleBtag_reco_noAggr");

    // Make projections
    int min_kt_bin = hSingleBtag_reco_aggr->GetYaxis()->FindBin(0.);
    int max_kt_bin = hSingleBtag_reco_aggr->GetNbinsY();

    TH1F *hSingleBtag_gen_aggr_1d = (TH1F *) hSingleBtag_gen_aggr->ProjectionX("hSingleBtag_gen_aggr_1d", min_kt_bin, max_kt_bin);
    hSingleBtag_gen_aggr_1d->Scale(1/hSingleBtag_gen_aggr_1d->Integral());
    hSingleBtag_gen_aggr_1d->SetLineColor(cmsBlue);
    hSingleBtag_gen_aggr_1d->SetLineWidth(3);
    hSingleBtag_gen_aggr_1d->GetXaxis()->SetTitle(xlabel);
    hSingleBtag_gen_aggr_1d->GetYaxis()->SetTitle("1/^{}N dN/^{}d"+xlabel);
    hSingleBtag_gen_aggr_1d->GetXaxis()->SetRangeUser(0., 3.1);
    hSingleBtag_gen_aggr_1d->GetYaxis()->SetRangeUser(0., 0.35);
    hSingleBtag_gen_aggr_1d->GetYaxis()->SetTitleOffset(1.9);
    if (observable=="zg") hSingleBtag_gen_aggr_1d->GetYaxis()->SetRangeUser(0., 0.25);
    hSingleBtag_gen_aggr_1d->SetLineStyle(1);

    TH1F *hSingleBtag_reco_aggr_1d = (TH1F *) hSingleBtag_reco_aggr->ProjectionX("hSingleBtag_reco_aggr_1d", min_kt_bin, max_kt_bin);
    hSingleBtag_reco_aggr_1d->Scale(1/hSingleBtag_reco_aggr_1d->Integral());
    hSingleBtag_reco_aggr_1d->SetLineColor(cmsOrange);
    hSingleBtag_reco_aggr_1d->SetLineWidth(3);    
    hSingleBtag_reco_aggr_1d->SetLineStyle(8);

    TH1F *hSingleBtag_reco_noAggr_1d = (TH1F *) hSingleBtag_reco_noAggr->ProjectionX("hSingleBtag_reco_noAggr_1d", min_kt_bin, max_kt_bin);
    hSingleBtag_reco_noAggr_1d->Scale(1/hSingleBtag_reco_noAggr_1d->Integral());
    hSingleBtag_reco_noAggr_1d->SetLineColor(cmsViolet);
    hSingleBtag_reco_noAggr_1d->SetLineWidth(3); 
    hSingleBtag_reco_noAggr_1d->SetLineStyle(2);

    TCanvas *c_aggr_vs_noAggr = new TCanvas("c_aggr_vs_noAggr", "", 800, 700);
    c_aggr_vs_noAggr->SetRightMargin(0.03);
    c_aggr_vs_noAggr->SetLeftMargin(0.14);
    if (observable=="zg") {
        hSingleBtag_gen_aggr_1d->GetXaxis()->SetRangeUser(0.99,0.5);
        // hSingleBtag_reco_noAggr_1d->GetXaxis()->SetRangeUser(0.1,0.5);
        // hSingleBtag_reco_aggr_1d->GetXaxis()->SetRangeUser(0.1,0.5);
    }
    hSingleBtag_gen_aggr_1d->Draw("hist");
    hSingleBtag_reco_noAggr_1d->Draw("hist same");
    hSingleBtag_reco_aggr_1d->Draw("hist same");
    draw_info();

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    jet_info->SetTextAlign(32);
    // for png
    // jet_info->DrawLatex(0.52, 0.65, "anti-k_{T}, R=0.4 b-tagged b jets");
    // jet_info->DrawLatex(0.505, 0.6, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    // jet_info->DrawLatex(0.64, 0.55, "Soft drop (charged particles)");
    // jet_info->DrawLatex(0.575, 0.5, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");
    // for pdf
    jet_info->DrawLatex(0.94, 0.7, "anti-k_{T}, R=0.4 b-tagged b jets");
    jet_info->DrawLatex(0.94, 0.65, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info->DrawLatex(0.94, 0.6, "Soft drop (charged particles)");
    jet_info->DrawLatex(0.94, 0.55, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");

    TLegend *leg = new TLegend(0.2, 0.75, 0.85, 0.87);
    leg->SetFillStyle(0);
    leg->SetMargin(0.1);
    leg->AddEntry(hSingleBtag_gen_aggr_1d, "Particle level, clustered decay daughters", "l");
    leg->AddEntry(hSingleBtag_reco_noAggr_1d, "Detector level, unclustered decay daughters", "l");
    leg->AddEntry(hSingleBtag_reco_aggr_1d, "Detector level, clustered decay daughters", "l");
    leg->Draw();

    c_aggr_vs_noAggr->Print("plots_an/"+observable+"_aggr_vs_not.png");
    c_aggr_vs_noAggr->Print("plots_an/"+observable+"_aggr_vs_not.pdf");
    c_aggr_vs_noAggr->Print("plots_thesis/"+observable+"_aggr_vs_not.pdf");

}
