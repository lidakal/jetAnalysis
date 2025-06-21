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
    lumi->DrawLatex(0.95, 0.98, "Simulation (pp 5.02 TeV)");
}

void cross_folding(TString observable="rg")
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
    if (observable=="rg") ylabel = "1/N dN/dln(R/R_{g})";
    else if (observable=="zg") ylabel = "1/N dN/dz_{g}";
    else if (observable=="zpt") ylabel = "1/N dN/dz_{b,ch}";    

    TString pythia_sample = "dijet_PF40";
    TString herwig_sample = "herwig_dijet_official_PF40";
    TString label = "aggrTMVA_inclusive";
    bool is_inclusive = label.Contains("inclusive");

    TFile *fin_pythia = new TFile("histos/"+pythia_sample+"_"+label+"_response_jer_nom_jec_nom.root");
    TH2D *h_pythia_reco = (TH2D *) fin_pythia->Get("h_full_purity_denominator_"+observable+"pt")->Clone("h_pythia_reco");
    TH2D *h_pythia_true = (TH2D *) fin_pythia->Get("h_full_efficiency_denominator_"+observable+"pt")->Clone("h_pythia_true");
    TH2D *h_pythia_purity = (TH2D *) fin_pythia->Get("h_full_purity_"+observable+"pt")->Clone("h_pythia_purity");
    TH2D *h_pythia_efficiency = (TH2D *) fin_pythia->Get("h_full_efficiency_"+observable+"pt")->Clone("h_pythia_efficiency");
    RooUnfoldResponse *response_pythia = (RooUnfoldResponse *) fin_pythia->Get("response_full_" + observable + "pt")->Clone("response_pythia");

    TFile *fin_herwig = new TFile("histos/"+herwig_sample+"_"+label+"_response_jer_nom_jec_nom.root");
    TH2D *h_herwig_reco = (TH2D *) fin_herwig->Get("h_full_purity_denominator_"+observable+"pt")->Clone("h_herwig_reco");
    TH2D *h_herwig_true = (TH2D *) fin_herwig->Get("h_full_efficiency_denominator_"+observable+"pt")->Clone("h_herwig_true");
    TH2D *h_herwig_purity = (TH2D *) fin_herwig->Get("h_full_purity_"+observable+"pt")->Clone("h_herwig_purity");
    TH2D *h_herwig_efficiency = (TH2D *) fin_herwig->Get("h_full_efficiency_"+observable+"pt")->Clone("h_herwig_efficiency");
    RooUnfoldResponse *response_herwig = (RooUnfoldResponse *) fin_herwig->Get("response_full_" + observable + "pt")->Clone("response_herwig");

    // Unfold pythia with herwig response
    TH2D *h_pythia_purity_corrected = (TH2D *) h_pythia_reco->Clone("h_pythia_purity_corrected");
    h_pythia_purity_corrected->Multiply(h_herwig_purity);

    // RooUnfoldBayes unfold_herwig(response_herwig, h_pythia_purity_corrected, 5);
    RooUnfoldInvert unfold_herwig(response_herwig, h_pythia_purity_corrected);
    TH2D *h_pythia_unfolded = (TH2D *) unfold_herwig.Hreco();
    h_pythia_unfolded->SetName("h_pythia_unfolded");

    TH2D *h_pythia_efficiency_corrected = (TH2D *) h_pythia_unfolded->Clone("h_pythia_efficiency_corrected");
    h_pythia_efficiency_corrected->Divide(h_pythia_unfolded, h_herwig_efficiency, 1, 1);

    // Unfold herwig with pythia response
    TH2D *h_herwig_purity_corrected = (TH2D *) h_herwig_reco->Clone("h_herwig_purity_corrected");
    h_herwig_purity_corrected->Multiply(h_pythia_purity);

    // RooUnfoldBayes unfold_pythia(response_pythia, h_herwig_purity_corrected, 5);
    RooUnfoldInvert unfold_pythia(response_pythia, h_herwig_purity_corrected);
    TH2D *h_herwig_unfolded = (TH2D *) unfold_pythia.Hreco();
    h_herwig_unfolded->SetName("h_herwig_unfolded");

    TH2D *h_herwig_efficiency_corrected = (TH2D *) h_herwig_unfolded->Clone("h_herwig_efficiency_corrected");
    h_herwig_efficiency_corrected->Divide(h_herwig_unfolded, h_pythia_efficiency, 1, 1);

    // Make projections
    int ibin_pt = 2;
    Double_t pt_min = h_pythia_reco->GetYaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_pythia_reco->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_pythia_reco_1d = (TH1D *) h_pythia_reco->ProjectionX("h_pythia_reco_1d", ibin_pt, ibin_pt);
    TH1D *h_pythia_true_1d = (TH1D *) h_pythia_true->ProjectionX("h_pythia_true_1d", ibin_pt, ibin_pt);
    TH1D *h_pythia_efficiency_corrected_1d = (TH1D *) h_pythia_efficiency_corrected->ProjectionX("h_pythia_efficiency_corrected_1d", ibin_pt, ibin_pt);

    TH1D *h_herwig_reco_1d = (TH1D *) h_herwig_reco->ProjectionX("h_herwig_reco_1d", ibin_pt, ibin_pt);
    TH1D *h_herwig_true_1d = (TH1D *) h_herwig_true->ProjectionX("h_herwig_true_1d", ibin_pt, ibin_pt);
    TH1D *h_herwig_efficiency_corrected_1d = (TH1D *) h_herwig_efficiency_corrected->ProjectionX("h_herwig_efficiency_corrected_1d", ibin_pt, ibin_pt);

    // Normalize
    Int_t nbins = h_pythia_reco_1d->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins; 
    for (auto h : {
        h_pythia_reco_1d, h_pythia_true_1d, h_pythia_efficiency_corrected_1d,
        h_herwig_reco_1d, h_herwig_true_1d, h_herwig_efficiency_corrected_1d
    }) {
        h->GetXaxis()->SetRange(ibin_min, ibin_max);
        h->Scale(1/h->Integral(), "width");
    }

    // Format histos
    h_pythia_reco_1d->SetMarkerStyle(kFullTriangleUp);
    h_pythia_reco_1d->SetMarkerSize(2);
    h_pythia_reco_1d->SetMarkerColor(cmsBlue);
    h_pythia_reco_1d->SetLineColor(cmsBlue);

    h_pythia_true_1d->SetMarkerStyle(kFullTriangleDown);
    h_pythia_true_1d->SetMarkerSize(2);
    h_pythia_true_1d->SetMarkerColor(cmsViolet);
    h_pythia_true_1d->SetLineColor(cmsViolet);

    h_pythia_efficiency_corrected_1d->SetMarkerStyle(kFullCrossX);
    h_pythia_efficiency_corrected_1d->SetMarkerSize(2);
    h_pythia_efficiency_corrected_1d->SetMarkerColor(kBlack);
    h_pythia_efficiency_corrected_1d->SetLineColor(kBlack);
    

    h_herwig_reco_1d->SetMarkerStyle(kOpenTriangleUp);
    h_herwig_reco_1d->SetMarkerSize(2);
    h_herwig_reco_1d->SetMarkerColor(cmsBlue);
    h_herwig_reco_1d->SetLineColor(cmsBlue);

    h_herwig_true_1d->SetMarkerStyle(kOpenTriangleDown);
    h_herwig_true_1d->SetMarkerSize(2);
    h_herwig_true_1d->SetMarkerColor(cmsViolet);
    h_herwig_true_1d->SetLineColor(cmsViolet);
   
    h_herwig_efficiency_corrected_1d->SetMarkerStyle(kOpenCrossX);
    h_herwig_efficiency_corrected_1d->SetMarkerSize(2);
    h_herwig_efficiency_corrected_1d->SetMarkerColor(kBlack);
    h_herwig_efficiency_corrected_1d->SetLineColor(kBlack);
  
    // Make ratios
    TH1D *h_ratio_pythia = (TH1D *) h_pythia_efficiency_corrected_1d->Clone("h_ratio_pythia");
    h_ratio_pythia->Divide(h_pythia_true_1d);

    TH1D *h_ratio_herwig = (TH1D *) h_herwig_efficiency_corrected_1d->Clone("h_ratio_herwig");
    h_ratio_herwig->Divide(h_herwig_true_1d);

    // Make legend
    TLegend *leg = new TLegend(0.58, 0.23, 0.9, 0.53);
    if (observable=="zpt") leg = new TLegend(0.35, 0.4, 0.67, 0.7);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(1.1);
    leg->SetHeader("PYTHIA8  HERWIG7");
    leg->SetNColumns(2);
    leg->AddEntry(h_pythia_reco_1d, " ", "pe1");
    leg->AddEntry(h_herwig_reco_1d, " ", "pe1");
    leg->AddEntry(h_pythia_true_1d, " ", "pe1");
    leg->AddEntry(h_herwig_true_1d, " ", "pe1");
    leg->AddEntry(h_pythia_efficiency_corrected_1d, " ", "pe1");
    leg->AddEntry(h_herwig_efficiency_corrected_1d, " ", "pe1");

    // Draw
    TCanvas *c_split = new TCanvas("c_split", "", 700, 600);
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
    h_pythia_reco_1d->GetYaxis()->SetTitle(ylabel);
    h_pythia_reco_1d->GetYaxis()->SetTitleSize(text_size);
    h_pythia_reco_1d->GetYaxis()->SetLabelSize(text_size-4);
    h_pythia_reco_1d->GetYaxis()->SetTitleOffset(1.3);
    if (!is_inclusive&&observable=="rg") h_pythia_reco_1d->GetYaxis()->SetRangeUser(0., 1.5);
    else if (is_inclusive&&observable=="rg") h_pythia_reco_1d->GetYaxis()->SetRangeUser(0., 2.5);
    else if (!is_inclusive&&observable=="zg") h_pythia_reco_1d->GetYaxis()->SetRangeUser(0., 8.);
    else if (is_inclusive&&observable=="zg") h_pythia_reco_1d->GetYaxis()->SetRangeUser(0., 12.);
    else if (observable=="zpt") h_pythia_reco_1d->GetYaxis()->SetRangeUser(0., 4.5);
    h_pythia_reco_1d->GetXaxis()->SetLabelOffset(10.); // kick it out of existence
    h_pythia_reco_1d->GetXaxis()->SetTitleOffset(10.); // kick it out of existence  

    h_pythia_reco_1d->Draw("pe1");
    h_pythia_true_1d->Draw("pe1 same");
    h_pythia_efficiency_corrected_1d->Draw("pe1 same");
    h_herwig_reco_1d->Draw("pe1 same");
    h_herwig_true_1d->Draw("pe1 same");
    h_herwig_efficiency_corrected_1d->Draw("pe1 same");

    leg->Draw();
    drawHeader();

    if (observable!="zpt") {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(32);
        if (is_inclusive) jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 inclusive jets"); 
        else jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 b-tagged b jets");
        jet_info->DrawLatex(0.9, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
        jet_info->DrawLatex(0.9, 0.67, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.9, 0.59, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV/c");

        jet_info->DrawLatex(0.61, 0.42, "Detector level");
        jet_info->DrawLatex(0.61, 0.34, "Particle level");
        jet_info->DrawLatex(0.61, 0.26, "Unfolded");
    } else {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(12);
        jet_info->DrawLatex(0.2, 0.83, "anti-k_{T}, R = 0.4 b-tagged b jets");
        jet_info->DrawLatex(0.2, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));

        jet_info->DrawLatex(0.2, 0.58, "Detector level");
        jet_info->DrawLatex(0.2, 0.5, "Particle level");
        jet_info->DrawLatex(0.2, 0.42, "Unfolded");
    }

    bottom_pad->cd();
    h_ratio_pythia->GetXaxis()->SetTitle(xlabel);
    h_ratio_pythia->GetXaxis()->SetTitleSize(text_size);
    h_ratio_pythia->GetXaxis()->SetLabelSize(text_size-4);
    h_ratio_pythia->GetXaxis()->SetTitleOffset(3.);
    h_ratio_pythia->GetYaxis()->SetTitle("#splitline{Ratio to}{particle level}");
    h_ratio_pythia->GetYaxis()->SetTitleSize(text_size);
    h_ratio_pythia->GetYaxis()->SetLabelSize(text_size-4);
    h_ratio_pythia->GetYaxis()->SetTitleOffset(1.4);
    if (is_inclusive) h_ratio_pythia->GetYaxis()->SetRangeUser(0.5, 1.5);
    else h_ratio_pythia->GetYaxis()->SetRangeUser(0., 2.);
    h_ratio_pythia->GetYaxis()->SetNdivisions(-4);

    h_ratio_pythia->Draw("pe1");
    h_ratio_herwig->Draw("pe1 same");

    if (observable!="zpt"&&ibin_min==1) {
        TPaveText *untagged_text = new TPaveText(0.17, 0.02, 0.375, 0.325, "NDC");
        untagged_text->SetFillColor(0);
        untagged_text->SetBorderSize(0);
        untagged_text->SetTextAlign(22);
        untagged_text->SetTextSize(text_size-4);
        untagged_text->AddText("SD-untagged");
        untagged_text->AddText("or k_{T} < 1 GeV/c");
        untagged_text->Draw();
        h_ratio_pythia->GetXaxis()->ChangeLabel(1, -1, 0.0, -1, -1, -1, " ");
    }

    c_split->cd();
    top_pad->Draw();
    bottom_pad->Draw();
    
    c_split->Draw();
    c_split->Print("../plots_thesis/"+label+"_xfolding_test_"+observable+".pdf");


}