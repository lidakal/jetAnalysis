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

void split_test(TString observable="rg")
{
    // Split test: fill response w/ half MC, test on other half

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

    // ---- Grab raw histos
    TString sample = "pythia_PF40";
    TString label = "aggrTMVA_XXT";
    TString fname = "./histos/" + sample + "_" + label + "_response_jer_nom_jec_nom.root";
    std::cout << "File in : " << fname << std::endl;
    bool is_inclusive = label.Contains("inclusive");

    // half0: response, purity, efficiency
    // half1: pseudo data

    TFile *fin = new TFile(fname);
    TH2D *h_half1_efficiency_denominator = (TH2D *) fin->Get("h_half1_efficiency_denominator_" + observable + "pt");
    TH2D *h_half1_purity_denominator = (TH2D *) fin->Get("h_half1_purity_denominator_" + observable + "pt");
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin->Get("response_" + observable + "pt_half0");
    TH2D *h_purity = (TH2D *) fin->Get("h_half1_purity_" + observable + "pt");
    TH2D *h_efficiency = (TH2D *) fin->Get("h_half1_efficiency_"+observable+"pt");

    // Note: Result =  unfold(raw * purity * signal yield) * 1 / (efficiency)
    //       fakes are negligible

    // ---- Purity correction 
    TH2D *h_purity_corrected = (TH2D *) h_half1_purity_denominator->Clone("h_purity_corrected");
    h_purity_corrected->Multiply(h_purity);

    // ---- Unfolding
    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
    Int_t niter = 5;
    RooUnfoldBayes unfold(response, h_half1_purity_denominator, niter);
    // RooUnfoldInvert unfold(response, h_purity_corrected);
    TH2D *h_unfolded = (TH2D *) unfold.Hreco(errorTreatment);
    TH2D *h_refolded = (TH2D *) response->ApplyToTruth(h_unfolded, "h_refolded"); 
    // ---- Efficiency correction
    TH2D *h_efficiency_corrected = (TH2D *) h_unfolded->Clone("h_efficiency_corrected");
    h_efficiency_corrected->Divide(h_efficiency);

    // ---- Make projections
    Int_t ibin_pt = 2;
    Double_t pt_min = h_half1_purity_denominator->GetYaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_half1_purity_denominator->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_half1_purity_denominator_1d = (TH1D *) h_half1_purity_denominator->ProjectionX("h_half1_purity_denominator_1d", ibin_pt, ibin_pt);
    TH1D *h_half1_efficiency_denominator_1d = (TH1D *) h_half1_efficiency_denominator->ProjectionX("h_half1_efficiency_denominator_1d", ibin_pt, ibin_pt);
    // TH1D *h_purity_corrected_1d = (TH1D *) h_purity_corrected->ProjectionX("h_purity_corrected_1d", ibin_pt, ibin_pt);
    // TH1D *h_unfolded_1d = (TH1D *) h_unfolded->ProjectionX("h_unfolded_1d", ibin_pt, ibin_pt);
    // TH1D *h_refolded_1d = (TH1D *) h_refolded->ProjectionX("h_refolded_1d", ibin_pt, ibin_pt);
    TH1D *h_efficiency_corrected_1d = (TH1D *) h_efficiency_corrected->ProjectionX("h_efficiency_corrected_1d", ibin_pt, ibin_pt);

    // ---- Normalize
    Int_t nbins = h_half1_purity_denominator_1d->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins; 
    // if (observable=="rg") ibin_max = nbins-1;
    double x_min = h_half1_purity_denominator_1d->GetXaxis()->GetBinLowEdge(ibin_min);
    double rg_max = h_half1_purity_denominator_1d->GetXaxis()->GetBinUpEdge(ibin_max);
    for (auto h : {
                   h_half1_purity_denominator_1d, h_half1_efficiency_denominator_1d,
                //    h_purity_corrected_1d, 
                //    h_unfolded_1d,
                //    h_refolded_1d,
                   h_efficiency_corrected_1d,
                  //  h_half0_efficiency_denominator_1d, h_half0_purity_denominator_1d,
                  //  h_half0_purity_denominator_purity_corrected_1d,
                  }) {
                    h->GetXaxis()->SetRange(ibin_min, ibin_max);
                    h->Scale(1/h->Integral(), "width");
                   }


    // ---- Format histos

    // pseudo data
    h_half1_purity_denominator_1d->SetMarkerColor(cmsBlue);
    h_half1_purity_denominator_1d->SetLineColor(cmsBlue);
    h_half1_purity_denominator_1d->SetMarkerStyle(kFullTriangleUp);
    h_half1_purity_denominator_1d->SetMarkerSize(2);

    h_half1_efficiency_denominator_1d->SetMarkerColor(cmsViolet);
    h_half1_efficiency_denominator_1d->SetLineColor(cmsViolet);
    h_half1_efficiency_denominator_1d->SetMarkerStyle(kFullTriangleDown);
    h_half1_efficiency_denominator_1d->SetMarkerSize(2);
    
    h_efficiency_corrected_1d->SetMarkerColor(kBlack);
    h_efficiency_corrected_1d->SetLineColor(kBlack);
    h_efficiency_corrected_1d->SetMarkerStyle(kFullCrossX);
    h_efficiency_corrected_1d->SetMarkerSize(2);

    // legend
    TLegend *leg = new TLegend(0.67, 0.2, 0.9, 0.5);
    if (is_inclusive) leg = new TLegend(0.18, 0.59, 0.41, 0.89);
    if (observable=="zpt") leg = new TLegend(0.2, 0.2, 0.43, 0.5);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->AddEntry(h_half1_purity_denominator_1d, "Detector level", "pe1");
    leg->AddEntry(h_half1_efficiency_denominator_1d, "Particle level", "pe1");
    leg->AddEntry(h_efficiency_corrected_1d, "Unfolded", "pe1");
    
    // ------- RATIO PLOTS

    // THStack *hStack_ratio = new THStack("hStack_ratio", "");
    // hStack_ratio->SetTitle("; " + xlabel + "; ratio");

    // TLegend *leg_ratio = new TLegend(0.4, 0.3, 0.85, 0.5);
    // leg_ratio->SetBorderSize(1);

    // TH1D *h_smeared_smeared_ratio = (TH1D *) h_half1_purity_denominator_1d->Clone("h_smeared_smeared_ratio");
    // h_smeared_smeared_ratio->Divide(h_half0_purity_denominator_1d);
    // h_smeared_smeared_ratio->SetMarkerStyle(kFullCircle);
    // h_smeared_smeared_ratio->SetMarkerColor(kBlack);
    // h_smeared_smeared_ratio->SetLineColor(kBlack);
    // h_smeared_smeared_ratio->SetMarkerSize(1);
    // h_smeared_smeared_ratio->SetMinimum(0.);
    // h_smeared_smeared_ratio->SetMaximum(2.5);
    // h_smeared_smeared_ratio->GetYaxis()->SetTitle("ratio");
    // h_smeared_smeared_ratio->GetXaxis()->SetTitle(xlabel);
    // h_smeared_smeared_ratio->GetXaxis()->SetTitleOffset(3.5);
    // hStack_ratio->Add(h_smeared_smeared_ratio, "pe1");
    // leg_ratio->AddEntry(h_smeared_smeared_ratio, "reco pseudo data / reco mc", "pe1");

    TH1D *h_true_ratio = (TH1D *) h_efficiency_corrected_1d->Clone("h_true_ratio");
    h_true_ratio->Divide(h_half1_efficiency_denominator_1d);
    
    // ---- Draw canvas
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
    h_half1_purity_denominator_1d->GetYaxis()->SetTitle(ylabel);
    h_half1_purity_denominator_1d->GetYaxis()->SetTitleSize(text_size);
    h_half1_purity_denominator_1d->GetYaxis()->SetLabelSize(text_size-4);
    h_half1_purity_denominator_1d->GetYaxis()->SetTitleOffset(1.3);
    if (!is_inclusive&&observable=="rg") h_half1_purity_denominator_1d->GetYaxis()->SetRangeUser(0., 0.8);
    else if (is_inclusive&&observable=="rg") h_half1_purity_denominator_1d->GetYaxis()->SetRangeUser(0., 0.9);
    else if (!is_inclusive&&observable=="zg") h_half1_purity_denominator_1d->GetYaxis()->SetRangeUser(0., 5.);
    else if (is_inclusive&&observable=="zg") h_half1_purity_denominator_1d->GetYaxis()->SetRangeUser(0., 5.);
    else if (observable=="zpt") h_half1_purity_denominator_1d->GetYaxis()->SetRangeUser(0., 4.5);
    h_half1_purity_denominator_1d->GetXaxis()->SetLabelOffset(10.); // kick it out of existence
    h_half1_purity_denominator_1d->GetXaxis()->SetTitleOffset(10.); // kick it out of existence
    h_half1_purity_denominator_1d->Draw("pe1");
    h_half1_efficiency_denominator_1d->Draw("pe1 same");
    h_efficiency_corrected_1d->Draw("pe1 same");
    leg->Draw();
    drawHeader();
    // Jets text
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
    } else {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(12);
        jet_info->DrawLatex(0.2, 0.83, "anti-k_{T}, R = 0.4 b-tagged b jets");
        jet_info->DrawLatex(0.2, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
    }

    bottom_pad->cd();
    h_true_ratio->GetXaxis()->SetTitle(xlabel);
    h_true_ratio->GetXaxis()->SetTitleSize(text_size);
    h_true_ratio->GetXaxis()->SetLabelSize(text_size-4);
    h_true_ratio->GetXaxis()->SetTitleOffset(3.);
    h_true_ratio->GetYaxis()->SetTitle("#splitline{Ratio to}{particle level}");
    h_true_ratio->GetYaxis()->SetTitleSize(text_size);
    h_true_ratio->GetYaxis()->SetLabelSize(text_size-4);
    h_true_ratio->GetYaxis()->SetTitleOffset(1.4);
    if (is_inclusive) h_true_ratio->GetYaxis()->SetRangeUser(0.8, 1.2);
    else h_true_ratio->GetYaxis()->SetRangeUser(0.5, 1.5);
    h_true_ratio->GetYaxis()->SetNdivisions(-4);
    h_true_ratio->Draw("pe1 same");

    if (observable!="zpt"&&ibin_min==1) {
        TPaveText *untagged_text = new TPaveText(0.17, 0.02, 0.375, 0.325, "NDC");
        untagged_text->SetFillColor(0);
        untagged_text->SetBorderSize(0);
        untagged_text->SetTextAlign(22);
        untagged_text->SetTextSize(text_size-4);
        untagged_text->AddText("SD-untagged");
        untagged_text->AddText("or k_{T} < 1 GeV/c");
        untagged_text->Draw();
        h_true_ratio->GetXaxis()->ChangeLabel(1, -1, 0.0, -1, -1, -1, " ");
    }


    c_split->cd();
    top_pad->Draw();
    bottom_pad->Draw();

    // c_split->Draw();    
    c_split->Print("../plots_thesis/"+sample+"_"+label+"_split_test_"+observable+".pdf");
    
}