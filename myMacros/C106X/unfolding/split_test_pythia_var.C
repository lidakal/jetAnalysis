#include "../draw_utils.h"

void split_test_pythia_var(TString observable="rg")
{
    gStyle->SetErrorX(0.5);
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch}";
    TString ylabel = "1/N dN/d" + xlabel;

    TString sample = "dijet_PF40";
    TString label = "aggrTMVA_inclusive";
    TString var = "FSRdown";
    bool unfold_0 = true;
    bool inclusive = sample.Contains("dijet");

    TFile *fin_0 = new TFile("histos/"+sample+"_"+label+"_response_jer_nom_jec_nom_pythia_variations_1sthalf.root");
    TH2D *h_reco_0 = (TH2D *) fin_0->Get("h_"+var+"_purity_denominator_"+observable+"pt")->Clone("h_reco_0");
    TH2D *h_true_0 = (TH2D *) fin_0->Get("h_"+var+"_efficiency_denominator_"+observable+"pt")->Clone("h_true_0");
    TH2D *h_purity_0 = (TH2D *) fin_0->Get("h_"+var+"_purity_"+observable+"pt")->Clone("h_purity_0");
    TH2D *h_efficiency_0 = (TH2D *) fin_0->Get("h_"+var+"_efficiency_"+observable+"pt")->Clone("h_efficiency_0");
    RooUnfoldResponse *response_0 = (RooUnfoldResponse *) fin_0->Get("response_"+var+"_"+observable+"pt")->Clone("response_0");

    TFile *fin_1 = new TFile("histos/"+sample+"_"+label+"_response_jer_nom_jec_nom_pythia_variations_2ndhalf.root");
    TH2D *h_reco_1 = (TH2D *) fin_1->Get("h_"+var+"_purity_denominator_"+observable+"pt")->Clone("h_reco_1");
    TH2D *h_true_1 = (TH2D *) fin_1->Get("h_"+var+"_efficiency_denominator_"+observable+"pt")->Clone("h_true_1");
    TH2D *h_purity_1 = (TH2D *) fin_1->Get("h_"+var+"_purity_"+observable+"pt")->Clone("h_purity_1");
    TH2D *h_efficiency_1 = (TH2D *) fin_1->Get("h_"+var+"_efficiency_"+observable+"pt")->Clone("h_efficiency_1");
    RooUnfoldResponse *response_1 = (RooUnfoldResponse *) fin_1->Get("response_"+var+"_"+observable+"pt")->Clone("response_1");

    TH2D *h_reco, *h_true, *h_purity, *h_efficiency;
    RooUnfoldResponse *response;

    if (unfold_0) {
        h_reco = (TH2D *) h_reco_0->Clone("h_reco");
        h_true = (TH2D *) h_true_0->Clone("h_true");

        h_purity = (TH2D *) h_purity_1->Clone("h_purity");
        h_efficiency = (TH2D *) h_efficiency_1->Clone("h_efficiency");
        response = (RooUnfoldResponse *) response_1->Clone("response");
    } else {
        h_reco = (TH2D *) h_reco_1->Clone("h_reco");
        h_true = (TH2D *) h_true_1->Clone("h_true");

        h_purity = (TH2D *) h_purity_0->Clone("h_purity");
        h_efficiency = (TH2D *) h_efficiency_0->Clone("h_efficiency");
        response = (RooUnfoldResponse *) response_0->Clone("response");
    }

    TH2D *h_purity_corrected = (TH2D *) h_reco->Clone("h_purity_corrected");
    h_purity_corrected->Multiply(h_purity);

    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
    RooUnfoldInvert unfold(response, h_purity_corrected);
    TH2D *h_unfolded = (TH2D *) unfold.Hreco(errorTreatment);

    TH2D *h_efficiency_corrected = (TH2D *) h_unfolded->Clone("h_efficiency_corrected");
    h_efficiency_corrected->Divide(h_efficiency);

    // Make projections 
    int ibin_pt = 2;
    TH1D *h_reco_1d = (TH1D *) h_reco->ProjectionX("h_reco_1d", ibin_pt, ibin_pt);
    TH1D *h_true_1d = (TH1D *) h_true->ProjectionX("h_true_1d", ibin_pt, ibin_pt);
    TH1D *h_efficiency_corrected_1d = (TH1D *) h_efficiency_corrected->ProjectionX("h_efficiency_corrected_1d", ibin_pt, ibin_pt);

    // Normalize
    for (auto h : {h_reco_1d, h_true_1d, h_efficiency_corrected_1d}) {
        h->Scale(1/h->Integral(), "width");
    }

    // Make ratio
    TH2D *h_true_ratio = (TH2D *) h_efficiency_corrected_1d->Clone("h_true_ratio");
    h_true_ratio->Divide(h_true_1d);
    h_true_ratio->GetYaxis()->SetTitle("Unfolded / true");
    h_true_ratio->GetXaxis()->SetTitle(xlabel);
    h_true_ratio->GetXaxis()->SetTitleOffset(3.);
    if (!inclusive) h_true_ratio->GetYaxis()->SetRangeUser(0., 2.);
    else h_true_ratio->GetYaxis()->SetRangeUser(0.93, 1.05);

    h_reco_1d->SetMarkerStyle(kFullTriangleUp);
    h_reco_1d->SetMarkerColor(kRed);
    h_reco_1d->SetLineColor(kRed);
    h_reco_1d->GetXaxis()->SetTitle(xlabel);
    h_reco_1d->GetYaxis()->SetTitle(ylabel);
    if (!inclusive) h_reco_1d->GetYaxis()->SetRangeUser(-0.05, 0.7);
    else h_reco_1d->GetYaxis()->SetRangeUser(0., 0.5);

    h_true_1d->SetMarkerStyle(kFullTriangleDown);
    h_true_1d->SetMarkerColor(kBlue);
    h_true_1d->SetLineColor(kBlue);

    h_efficiency_corrected_1d->SetMarkerStyle(kFullCircle);
    h_efficiency_corrected_1d->SetMarkerColor(kBlack);
    h_efficiency_corrected_1d->SetLineColor(kBlack);

    TLegend *leg_split = new TLegend(0.45, 0.6, 0.8, 0.85);
    double ptMin_real = h_purity->GetYaxis()->GetBinLowEdge(ibin_pt);
    double ptMax_real = h_purity->GetYaxis()->GetBinUpEdge(ibin_pt);
    leg_split->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin_real, ptMax_real));
    leg_split->AddEntry(h_reco_1d, "Detector level pseudo-data", "pe1");
    leg_split->AddEntry(h_true_1d, "Particle level pseudo-data", "pe1");
    leg_split->AddEntry(h_efficiency_corrected_1d, "Unfolded pseudo-data", "pe1");

    TCanvas *c_split = new TCanvas("c_split", "", 800, 600);
    TPad *top_pad = new TPad("top_pad", "", 0., 0.33, 1., 1.);
    TPad *bot_pad = new TPad("bot_pad", "", 0., 0., 1., 0.33);
    bot_pad->SetTopMargin(0.01);
    bot_pad->SetBottomMargin(0.3);
    top_pad->SetBottomMargin(0.01);
    
    top_pad->cd();
    h_reco_1d->Draw("pe1");
    h_true_1d->Draw("pe1 same");
    h_efficiency_corrected_1d->Draw("pe1 same");
    leg_split->Draw();

    TLatex *test_info_text = new TLatex;
    test_info_text->SetNDC();
    // test_info_text->SetTextSize(text_size);
    test_info_text->DrawLatex(0.2, 0.2, "SPLIT TEST");
    test_info_text->DrawLatex(0.2, 0.15, var);
    test_info_text->DrawLatex(0.2, 0.1, inclusive ? "inclusive jets" : "b jets");
    test_info_text->Draw();
    if (sample.Contains("herwig")) drawHeaderHerwig();
    else drawHeaderSimulation();

    bot_pad->cd();
    h_true_ratio->Draw("pe1");

    c_split->cd();
    top_pad->Draw();
    bot_pad->Draw();

}