#include "../cms_palette.h"

void drawHeader(void) {
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(13);
    prelim->DrawLatex(0.15, 0.98, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(33);
    lumi->DrawLatex(0.95, 0.98, "PYTHIA8 (pp 5.02 TeV)");
}

void draw_bias(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{
    // Setup plot
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch}";
    TString ylabel;
    if (observable=="rg") ylabel = "dN/dln(R/R_{g})";
    else if (observable=="zg") ylabel = "dN/dz_{g}";
    else if (observable=="zpt") ylabel = "dN/dz_{b,ch}";    

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;

    // Load histograms
    TString sample = "pythia_PF40";
    TString label = "aggrTMVA";
    TString fin_name = "histos/"+sample+"_"+label+"_inclusive_histograms"+suffix+"_withSF.root";
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);
    TH2D *h_inclusive = (TH2D *) fin->Get("h_"+observable+"pt_gen");
    TH2D *h_btag = (TH2D *) fin->Get("h_"+observable+"pt_gen_tagged");

    // Create efficiency histograms 
    TH2D *h_eff = (TH2D *) h_btag->Clone("h_eff");
    h_eff->Divide(h_btag, h_inclusive, 1., 1., "b");

    // Make projections 
    Int_t ibin_pt = 2;
    Double_t pt_min = h_inclusive->GetYaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_inclusive->GetYaxis()->GetBinUpEdge(ibin_pt);
    TH1D *h_inclusive_1d = (TH1D *) h_inclusive->ProjectionX("h_inclusive_1d", ibin_pt, ibin_pt);
    TH1D *h_btag_1d = (TH1D *) h_btag->ProjectionX("h_btag_1d", ibin_pt, ibin_pt);
    TH1D *h_eff_1d = (TH1D *) h_eff->ProjectionX("h_eff_1d", ibin_pt, ibin_pt);

    // Normalize ONLY BY BIN WIDTH
    Int_t nbins = h_inclusive_1d->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins; 
    for (auto h : {h_inclusive_1d, h_btag_1d}) {
        h->GetXaxis()->SetRange(ibin_min, ibin_max);
        h->Scale(1, "width"); 
    }

    // Format histos 
    h_btag_1d->SetMarkerColor(cmsBlue);
    h_btag_1d->SetLineColor(cmsBlue);
    h_btag_1d->SetMarkerStyle(kFullTriangleUp);
    h_btag_1d->SetMarkerSize(2);

    h_inclusive_1d->SetMarkerColor(cmsViolet);
    h_inclusive_1d->SetLineColor(cmsViolet);
    h_inclusive_1d->SetMarkerStyle(kFullTriangleDown);
    h_inclusive_1d->SetMarkerSize(2);

    // legend
    TLegend *leg = new TLegend(0.65, 0.35, 0.9, 0.55);
    if (observable=="zpt") leg = new TLegend(0.2, 0.5, 0.43, 0.7);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->AddEntry(h_btag_1d, "b-tagged b jets", "pe1");
    leg->AddEntry(h_inclusive_1d, "All b jets", "pe1");

    // Make ratio -> compare with efficiency
    TH1D *h_ratio = (TH1D *) h_btag_1d->Clone("h_ratio");
    h_ratio->Divide(h_inclusive_1d);

    TCanvas *c_eff = new TCanvas("c_eff", "", 700, 600);
    TPad *top_pad = new TPad("top_pad", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad = new TPad("top_pad", "", 0., 0., 1., 0.33);

    top_pad->SetLeftMargin(0.15);
    top_pad->SetRightMargin(0.05);
    top_pad->SetTopMargin(0.09);
    top_pad->SetBottomMargin(0.03);
    bottom_pad->SetLeftMargin(0.15);
    bottom_pad->SetRightMargin(0.05);
    bottom_pad->SetTopMargin(0.04);
    bottom_pad->SetBottomMargin(0.35);

    top_pad->cd();
    h_btag_1d->GetYaxis()->SetTitle(ylabel);
    h_btag_1d->GetYaxis()->SetTitleSize(text_size);
    h_btag_1d->GetYaxis()->SetLabelSize(text_size-4);
    h_btag_1d->GetYaxis()->SetTitleOffset(1.3);
    h_btag_1d->GetYaxis()->SetRangeUser(0., 60);
    if (observable=="zg") h_btag_1d->GetYaxis()->SetRangeUser(0., 300.);
    else if (observable=="zpt") h_btag_1d->GetYaxis()->SetRangeUser(0., 300.);
    h_btag_1d->GetXaxis()->SetLabelOffset(10.); // kick it out of existence
    h_btag_1d->GetXaxis()->SetTitleOffset(10.); // kick it out of existence
    h_btag_1d->Draw("pe1");
    h_inclusive_1d->Draw("pe1 same");
    leg->Draw();
    drawHeader();
    // Jets text
    if (observable!="zpt") {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(32);
        jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 b jets"); 
        jet_info->DrawLatex(0.9, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
        jet_info->DrawLatex(0.9, 0.67, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.9, 0.59, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV/c");
    } else {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(12);
        jet_info->DrawLatex(0.2, 0.83, "anti-k_{T}, R = 0.4 b jets");
        jet_info->DrawLatex(0.2, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
    }

    bottom_pad->cd();
    h_ratio->GetXaxis()->SetTitle(xlabel);
    h_ratio->GetXaxis()->SetTitleSize(text_size);
    h_ratio->GetXaxis()->SetLabelSize(text_size-4);
    h_ratio->GetXaxis()->SetTitleOffset(3.);
    h_ratio->GetYaxis()->SetTitle("Efficiency");
    h_ratio->GetYaxis()->SetTitleSize(text_size);
    h_ratio->GetYaxis()->SetLabelSize(text_size-4);
    h_ratio->GetYaxis()->SetTitleOffset(1.4);
    h_ratio->GetYaxis()->SetRangeUser(0., 1.);
    h_ratio->GetYaxis()->SetNdivisions(-4);
    h_ratio->Draw("pe1");
    // h_eff_1d->Draw("pe1 same");

    c_eff->cd();
    top_pad->Draw();
    bottom_pad->Draw();

    c_eff->Print("../plots_thesis/btag_bias_"+observable+suffix+".pdf");
}