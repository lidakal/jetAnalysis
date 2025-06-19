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

void trivial_test(TString observable="rg")
{
    // Dummy test: apply the unfolding to the MC used to fill the matrix
    //             unfolded/particle level should be 1

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
    TString sample = "dijet_PF40";
    TString label = "aggrTMVA_inclusive";
    bool is_inclusive = label.Contains("inclusive");
    TString fname = "./histos/" + sample + "_" + label + "_response_jer_nom_jec_nom.root";
    std::cout << "fin: " << fname << std::endl;
    TFile *fin = new TFile(fname);
    TH2D *h_training_true = (TH2D *) fin->Get("h_full_efficiency_denominator_" + observable + "pt");
    TH2D *h_training_reco = (TH2D *) fin->Get("h_full_purity_denominator_" + observable + "pt");
    TH2D *h_training_purity = (TH2D *) fin->Get("h_full_purity_" + observable + "pt");
    TH2D *h_training_efficiency = (TH2D *) fin->Get("h_full_efficiency_"+observable+"pt");
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin->Get("response_full_" + observable + "pt");

    // trivial test: testing = training
    TH2D *h_testing_reco = (TH2D *) h_training_reco->Clone("h_testing_reco_trivial");
    TH2D *h_testing_true = (TH2D *) h_training_true->Clone("h_testing_true_trivial");

    // Note: Result =  unfold(raw * purity) * 1 / (efficiency)
    //       fakes are negligible

    // ---- Purity correction 
    TH2D *h_testing_reco_purity_corrected = (TH2D *) h_testing_reco->Clone("h_testing_reco_purity_corrected");
    h_testing_reco_purity_corrected->Multiply(h_training_purity);

    TH2D *h_training_reco_purity_corrected = (TH2D *) h_training_reco->Clone("h_training_reco_purity_corrected");
    h_training_reco_purity_corrected->Multiply(h_training_purity);

    // std::cout << "content before unfolding= " << h_testing_reco_purity_corrected->GetBinContent(1,1) << std::endl;
    // std::cout << "error before unfolding= " << h_testing_reco_purity_corrected->GetBinError(1,1) << std::endl;

    // ---- Unfolding
    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
    // Int_t niter = 5;
    // RooUnfoldBayes unfold(response, h_testing_reco, niter);
    RooUnfoldInvert unfold(response, h_testing_reco_purity_corrected);
    // unfold.SetNToys(100000);
    // errorTreatment = RooUnfold::kCovToy;
    TH2D *h_testing_reco_unfolded = (TH2D *) unfold.Hreco(errorTreatment);
    TH2D *h_testing_reco_refolded = (TH2D *) response->ApplyToTruth(h_testing_reco_unfolded, "h_testing_reco_refolded"); // to be compared to purity corrected testing
    // std::cout << "content after unfolding= " << h_testing_reco_unfolded->GetBinContent(1,1) << std::endl;
    // std::cout << "error after unfolding= " << h_testing_reco_unfolded->GetBinError(1,1) << std::endl;
    
    // ---- Efficiency correction
    TH2D *h_testing_reco_efficiency_corrected = (TH2D *) h_testing_reco_unfolded->Clone("h_testing_reco_efficiency_corrected");
    h_testing_reco_efficiency_corrected->Divide(h_training_efficiency);

    // ---- Make projections
    Int_t ibin_pt = 2;
    Double_t pt_min = h_testing_reco->GetYaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_testing_reco->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_testing_reco_1d = (TH1D *) h_testing_reco->ProjectionX("h_testing_reco_1d", ibin_pt, ibin_pt);
    TH1D *h_testing_true_1d = (TH1D *) h_testing_true->ProjectionX("h_testing_true_1d", ibin_pt, ibin_pt);
    TH1D *h_testing_reco_purity_corrected_1d = (TH1D *) h_testing_reco_purity_corrected->ProjectionX("h_testing_reco_purity_corrected_1d", ibin_pt, ibin_pt);
    TH1D *h_testing_reco_unfolded_1d = (TH1D *) h_testing_reco_unfolded->ProjectionX("h_testing_reco_unfolded_1d", ibin_pt, ibin_pt);
    TH1D *h_testing_reco_refolded_1d = (TH1D *) h_testing_reco_refolded->ProjectionX("h_testing_reco_refolded_1d", ibin_pt, ibin_pt);
    TH1D *h_testing_reco_efficiency_corrected_1d = (TH1D *) h_testing_reco_efficiency_corrected->ProjectionX("h_testing_reco_efficiency_corrected_1d", ibin_pt, ibin_pt);
    TH1D *h_training_true_1d = (TH1D *) h_training_true->ProjectionX("h_training_true_1d", ibin_pt, ibin_pt);
    TH1D *h_training_reco_1d = (TH1D *) h_training_reco->ProjectionX("h_training_reco_1d", ibin_pt, ibin_pt);
    TH1D *h_training_reco_purity_corrected_1d = (TH1D *) h_training_reco_purity_corrected->ProjectionX("h_training_reco_purity_corrected_1d", ibin_pt, ibin_pt);

    // ---- Normalize
    Int_t nbins = h_testing_reco_1d->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins; 
    double x_min = h_testing_reco_1d->GetXaxis()->GetBinLowEdge(ibin_min);
    double rg_max = h_testing_reco_1d->GetXaxis()->GetBinUpEdge(ibin_max);
    for (auto h : {h_testing_reco_1d, h_testing_true_1d,
                   h_testing_reco_purity_corrected_1d, 
                   h_testing_reco_unfolded_1d,
                   h_testing_reco_refolded_1d,
                   h_testing_reco_efficiency_corrected_1d,
                   h_training_true_1d, h_training_reco_1d,
                   h_training_reco_purity_corrected_1d}) {
                    h->GetXaxis()->SetRange(ibin_min, ibin_max);
                    h->Scale(1/h->Integral(), "width");
                   }


    // ---- Format histos

    // pseudo data 
    h_testing_reco_purity_corrected_1d->SetMarkerColor(cmsBlue);
    h_testing_reco_purity_corrected_1d->SetLineColor(cmsBlue);
    h_testing_reco_purity_corrected_1d->SetMarkerStyle(kFullTriangleUp);
    h_testing_reco_purity_corrected_1d->SetMarkerSize(2);

    h_testing_true_1d->SetMarkerColor(cmsViolet);
    h_testing_true_1d->SetLineColor(cmsViolet);
    h_testing_true_1d->SetMarkerStyle(kFullTriangleDown);
    h_testing_true_1d->SetMarkerSize(2);

    h_testing_reco_efficiency_corrected_1d->SetMarkerColor(kBlack);
    h_testing_reco_efficiency_corrected_1d->SetLineColor(kBlack);
    h_testing_reco_efficiency_corrected_1d->SetMarkerStyle(kFullCrossX);
    h_testing_reco_efficiency_corrected_1d->SetMarkerSize(2);

    h_testing_reco_refolded_1d->SetMarkerColor(kBlue);
    h_testing_reco_refolded_1d->SetMarkerColor(kBlue);
    h_testing_reco_refolded_1d->SetMarkerStyle(kFullCross);
    h_testing_reco_refolded_1d->SetMarkerSize(1);

    // simulation
    h_training_reco_1d->SetMarkerColor(kRed);
    h_training_reco_1d->SetLineColor(kRed);
    h_training_reco_1d->SetMarkerStyle(kFullTriangleUp);
    h_training_reco_1d->SetMarkerSize(1);
    // h_training_reco_1d->SetLineWidth(0);

    h_training_true_1d->SetMarkerColor(kRed);
    h_training_true_1d->SetLineColor(kRed);
    h_training_true_1d->SetMarkerStyle(kOpenTriangleUp);
    h_training_true_1d->SetMarkerSize(1);
    // h_training_true_1d->SetLineWidth(0);

    // legend
    TLegend *leg = new TLegend(0.67, 0.2, 0.9, 0.5);
    if (is_inclusive) leg = new TLegend(0.18, 0.59, 0.41, 0.89);
    if (observable=="zpt") leg = new TLegend(0.2, 0.2, 0.43, 0.5);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->AddEntry(h_testing_reco_purity_corrected_1d, "Detector level", "pe1");
    leg->AddEntry(h_testing_true_1d, "Particle level", "pe1");
    // leg->AddEntry(h_training_reco_1d, "Detector level simulation", "pe1");
    // leg->AddEntry(h_training_true_1d, "Particle level simulation", "pe1");  
    leg->AddEntry(h_testing_reco_efficiency_corrected_1d, "Unfolded", "pe1");
    // leg->AddEntry(h_testing_reco_refolded_1d, "Refolded pseudo data", "p");

    // ------- RATIO PLOTS
        TH1D *h_refolded_smeared_ratio = (TH1D *) h_testing_reco_refolded_1d->Clone("h_refolded_smeared_ratio");
    h_refolded_smeared_ratio->Divide(h_training_reco_purity_corrected_1d);

    TH1D *h_efficiency_corrected_true_ratio = (TH1D *) h_testing_reco_efficiency_corrected_1d->Clone("h_efficiency_corrected_true_ratio");
    h_efficiency_corrected_true_ratio->Divide(h_training_true_1d);

    TLine *line = new TLine(x_min, 1., rg_max, 1.);
    line->SetLineWidth(2.); 
    line->SetLineStyle(kDashed);
    line->SetLineColor(kGray);

    // legend 
    TLegend *leg_ratio = new TLegend(0.4, 0.3, 0.85, 0.5);
    leg_ratio->SetBorderSize(1);
    leg_ratio->AddEntry(h_refolded_smeared_ratio, "reco pseudo data / reco mc", "pe1");
    leg_ratio->AddEntry(h_efficiency_corrected_true_ratio, "unfolded pseudo data / true mc", "pe1");

    // ---- Draw canvas

    TCanvas *c_trivial = new TCanvas("c_trivial", "", 700, 600);
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
    h_testing_reco_purity_corrected_1d->GetYaxis()->SetTitle(ylabel);
    h_testing_reco_purity_corrected_1d->GetYaxis()->SetTitleSize(text_size);
    h_testing_reco_purity_corrected_1d->GetYaxis()->SetLabelSize(text_size-4);
    h_testing_reco_purity_corrected_1d->GetYaxis()->SetTitleOffset(1.3);
    if (!is_inclusive&&observable=="rg") h_testing_reco_purity_corrected_1d->GetYaxis()->SetRangeUser(0., 0.8);
    else if (is_inclusive&&observable=="rg") h_testing_reco_purity_corrected_1d->GetYaxis()->SetRangeUser(0., 0.9);
    else if (!is_inclusive&&observable=="zg") h_testing_reco_purity_corrected_1d->GetYaxis()->SetRangeUser(0., 5.);
    else if (is_inclusive&&observable=="zg") h_testing_reco_purity_corrected_1d->GetYaxis()->SetRangeUser(0., 5.);
    else if (observable=="zpt") h_testing_reco_purity_corrected_1d->GetYaxis()->SetRangeUser(0., 4.5);
    h_testing_reco_purity_corrected_1d->GetXaxis()->SetLabelOffset(10.); // kick it out of existence
    h_testing_reco_purity_corrected_1d->GetXaxis()->SetTitleOffset(10.); // kick it out of existence
    h_testing_reco_purity_corrected_1d->Draw("pe1");
    h_testing_true_1d->Draw("pe1 same");
    h_testing_reco_efficiency_corrected_1d->Draw("pe1 same");
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
    h_efficiency_corrected_true_ratio->GetXaxis()->SetTitle(xlabel);
    h_efficiency_corrected_true_ratio->GetXaxis()->SetTitleSize(text_size);
    h_efficiency_corrected_true_ratio->GetXaxis()->SetLabelSize(text_size-4);
    h_efficiency_corrected_true_ratio->GetXaxis()->SetTitleOffset(3.);
    h_efficiency_corrected_true_ratio->GetYaxis()->SetTitle("#splitline{Ratio to}{particle level}");
    h_efficiency_corrected_true_ratio->GetYaxis()->SetTitleSize(text_size);
    h_efficiency_corrected_true_ratio->GetYaxis()->SetLabelSize(text_size-4);
    h_efficiency_corrected_true_ratio->GetYaxis()->SetTitleOffset(1.4);
    h_efficiency_corrected_true_ratio->GetYaxis()->SetRangeUser(0.5, 1.5);
    h_efficiency_corrected_true_ratio->GetYaxis()->SetNdivisions(-4);

    // h_testing_true_1d->Draw("pe1 same");
    h_efficiency_corrected_true_ratio->Draw("pe1");
    // line->Draw();
    // h_efficiency_corrected_true_ratio->Draw("pe1 same");



    c_trivial->cd();
    top_pad->Draw();
    bottom_pad->Draw();
    c_trivial->Draw();
    c_trivial->Print("../plots_thesis/"+sample+"_"+label+"_trivial_test_"+observable+".pdf");
}