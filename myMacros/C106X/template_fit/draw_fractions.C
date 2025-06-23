#include "../cms_palette.h"

void draw_fractions(TString observable="rg")
{
    gStyle->SetErrorX(0.5);
    gStyle->SetCanvasPreferGL(kTRUE);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R^{}/^{}R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch} #equiv p_{T}^{b,ch}/^{}p_{T}^{jet,ch}";

    // Setup plot
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");


    TString fin_nom_name = "histos/fitted_parameters_RooFit_data_aggrTMVA_XXT_"+observable+"_glued_jer_nom_jec_nom.root";
    TString fin_herwig_name = "histos/fitted_parameters_RooFit_data_herwig_aggrTMVA_XXT_"+observable+"_glued_jer_nom_jec_nom.root";

    TFile *fin_nom = new TFile(fin_nom_name);
    TH2D *h_nom = (TH2D *) fin_nom->Get("h_sig_fraction")->Clone("h_nom");
    TFile *fin_herwig = new TFile(fin_herwig_name);
    TH2D *h_herwig = (TH2D *) fin_herwig->Get("h_sig_fraction")->Clone("h_herwig");


    // Make projections
    int ibin_pt = 2;
    Double_t pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);
    TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX("h_nom_1d", ibin_pt, ibin_pt);
    TH1D *h_herwig_1d = (TH1D *) h_herwig->ProjectionX("h_herwig_1d", ibin_pt, ibin_pt);

    // Format histograms
    h_nom_1d->SetMarkerStyle(kFullTriangleUp);
    h_nom_1d->SetMarkerColor(cmsBlue);
    h_nom_1d->SetLineColor(cmsBlue);
    h_nom_1d->SetMarkerSize(2);

    h_herwig_1d->SetMarkerStyle(kFullTriangleDown);
    h_herwig_1d->SetMarkerColor(cmsViolet);
    h_herwig_1d->SetLineColor(cmsViolet);
    h_herwig_1d->SetMarkerSize(2);

    // Legend
    TLegend *leg = new TLegend(0.15, 0.45, 0.6, 0.55);
    // if (observable=="zpt") leg = new TLegend(0.45, 0.5, 0.9, 0.65);
    leg->SetMargin(0.3);
    leg->SetFillStyle(0);
    leg->AddEntry(h_nom_1d, "Templates from PYTHIA8", "pe1");
    leg->AddEntry(h_herwig_1d, "Templates from HERWIG7", "pe1");

    TCanvas *c_fit = new TCanvas("c_fit", "", 700, 600);
    c_fit->SetRightMargin(0.05);
    c_fit->SetLeftMargin(0.12);

    h_nom_1d->GetYaxis()->SetRangeUser(0., 1.);
    h_nom_1d->GetXaxis()->SetTitle(xlabel);
    h_nom_1d->GetXaxis()->SetTitleOffset(0.5);
    h_nom_1d->GetYaxis()->SetTitle("Single-b jet fraction");
    h_nom_1d->GetYaxis()->SetTitleOffset(1.2);
    h_nom_1d->GetXaxis()->SetTitle(xlabel);
    h_nom_1d->GetXaxis()->SetTitleOffset(1.2);

    h_nom_1d->Draw("pe1");
    h_herwig_1d->Draw("pe1 same");
    leg->Draw();

    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.12, 0.93, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.95, 0.93, "pp 301^{} pb^{-1} (5.02 TeV)");

    // Jets text
    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size-4);
    jet_info->SetTextAlign(32);
    jet_info->DrawLatex(0.9, 0.38, "anti-k_{T}, R = 0.4 b-tagged jets");
    jet_info->DrawLatex(0.9, 0.32, Form("%.0f < p_{T}^{jet,reco} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
    if (observable!="zpt") {
        jet_info->DrawLatex(0.9, 0.26, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.9, 0.2, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV/c");
    }

    if (observable!="zpt") {
        TPaveText *untagged_text = new TPaveText(0.09, 0.02, 0.37, 0.137, "NDC");
        untagged_text->SetFillColor(0);
        untagged_text->SetBorderSize(0);
        untagged_text->SetTextAlign(22);
        untagged_text->SetTextSize(text_size-4);
        untagged_text->AddText("SD-untagged");
        untagged_text->AddText("or k_{T} < 1 GeV/c");
        untagged_text->Draw();
    }

    c_fit->Print("../plots_thesis/"+observable+"_fraction_comparison.pdf");
}