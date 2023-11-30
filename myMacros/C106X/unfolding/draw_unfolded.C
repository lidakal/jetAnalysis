#include "../myPalette.h"

void draw_unfolded(TString observable="rg")
{
    Float_t font_size = 28.;
    gStyle->SetLegendTextSize(font_size);
    gStyle->SetTextSize(font_size);
    gStyle->SetLabelSize(font_size, "XYZ");
    gStyle->SetTitleSize(font_size, "XYZ");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(#frac{0.4}{R_{g}})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    // Load unfolded histograms
    TFile *fin_unfolded_btag = new TFile("./histos/aggrTMVA_XXT_unfolded_histograms_"+observable+".root");
    TH2D *h_unfolded_btag = (TH2D *) fin_unfolded_btag->Get("h_data_unfolded")->Clone("h_unfolded_btag");

    TFile *fin_unfolded_inclusive;
    TH2D *h_unfolded_inclusive;
    if (!observable.Contains("zpt")) {
        fin_unfolded_inclusive = new TFile("./histos/aggrTMVA_inclusive_unfolded_histograms_"+observable+".root");
        h_unfolded_inclusive = (TH2D *) fin_unfolded_inclusive->Get("h_data_unfolded")->Clone("h_unfolded_inclusive");
    }

    // Load b tag efficiency corrections
    TFile *fin_btag_correction = new TFile("../btag/histos/aggrTMVA_XXT_"+observable+"pt_correction.root");
    TH2D * h_btag_correction = (TH2D *) fin_btag_correction->Get("h_correction");

    h_unfolded_btag->Multiply(h_btag_correction);

    // Load gen level histograms 
    TFile *fin_bjet_btag = new TFile("./histos/bjet_aggrTMVA_XXT_response_full.root");
    TH2D *h_sig_true_bjet_btag = (TH2D *) fin_bjet_btag->Get("h_sig_training_true_"+observable+"pt")->Clone("h_sig_true_bjet_btag");

    TFile *fin_dijet_inclusive;
    TH2D *h_true_dijet_inclusive;
    if (!observable.Contains("zpt")) {
        fin_dijet_inclusive = new TFile("./histos/dijet_aggrTMVA_inclusive_response_full.root");
        h_true_dijet_inclusive = (TH2D *) fin_dijet_inclusive->Get("h_training_true_"+observable+"pt")->Clone("h_true_dijet_inclusive");
    }

    // Make projections  
    Int_t nbins_x = h_unfolded_btag->GetNbinsX();
    Int_t ibin_x_min = 2; 
    if (observable=="zpt") ibin_x_min = 1;
    Int_t ibin_x_max = nbins_x;
    if (observable=="rg") ibin_x_max=nbins_x-1; 
    double x_min = h_unfolded_btag->GetXaxis()->GetBinLowEdge(ibin_x_min);
    double x_max = h_unfolded_btag->GetXaxis()->GetBinUpEdge(ibin_x_max);

    Int_t nbins_pt = h_unfolded_btag->GetNbinsY();
    Int_t ibin_pt_min = 2; 
    Int_t ibin_pt_max = 2; 
    double pt_min = h_unfolded_btag->GetYaxis()->GetBinLowEdge(ibin_pt_min);
    double pt_max = h_unfolded_btag->GetYaxis()->GetBinUpEdge(ibin_pt_max);

    TH1D *h_unfolded_btag_1d = (TH1D *) h_unfolded_btag->ProjectionX("h_unfolded_btag_1d", ibin_pt_min, ibin_pt_max);
    TH1D *h_sig_true_bjet_btag_1d = (TH1D *) h_sig_true_bjet_btag->ProjectionX("h_sig_true_bjet_btag_1d", ibin_pt_min, ibin_pt_max);

    TH1D *h_unfolded_inclusive_1d;
    TH1D *h_true_dijet_inclusive_1d;

    if (!observable.Contains("zpt")) {
        h_unfolded_inclusive_1d = (TH1D *) h_unfolded_inclusive->ProjectionX("h_unfolded_inclusive_1d", ibin_pt_min, ibin_pt_max);
        h_true_dijet_inclusive_1d = (TH1D *) h_true_dijet_inclusive->ProjectionX("h_true_dijet_inclusive_1d", ibin_pt_min, ibin_pt_max);
    }

    for (auto h : {h_unfolded_btag_1d, 
                   h_sig_true_bjet_btag_1d,
                   }) {
                    h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
                    h->Scale(1/h->Integral(ibin_x_min, ibin_x_max), "width");
    }

    if (!observable.Contains("zpt")) {
        for (auto h : {h_unfolded_inclusive_1d, 
                       h_true_dijet_inclusive_1d,
                       }) {
                        h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
                        h->Scale(1/h->Integral(ibin_x_min, ibin_x_max), "width");
        }
    }

    
    std::cout << "done normalizing" << std::endl;

    // Create stack + legend
    THStack *h_stack = new THStack("h_stack", "");
    h_stack->SetTitle("; "+xlabel+"; 1/N dN/d("+xlabel+")");

    TLegend *leg = new TLegend(0.5, 0.65, 0.85, 0.85);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);

    // Format histograms
    if (!observable.Contains("zpt")) {
        h_unfolded_inclusive_1d->SetMarkerStyle(kOpenCircle);
        h_unfolded_inclusive_1d->SetMarkerSize(2);
        h_unfolded_inclusive_1d->SetMarkerColor(mykBlue);
        h_unfolded_inclusive_1d->SetLineColor(mykBlue);
        h_stack->Add(h_unfolded_inclusive_1d, "pe1");
        leg->AddEntry(h_unfolded_inclusive_1d, "all jets unfolded data", "pe1");
    }

    h_unfolded_btag_1d->SetMarkerStyle(kFullCircle);
    h_unfolded_btag_1d->SetMarkerSize(2);
    h_unfolded_btag_1d->SetMarkerColor(mykBlue);
    h_unfolded_btag_1d->SetLineColor(mykBlue);
    h_stack->Add(h_unfolded_btag_1d, "pe1");
    leg->AddEntry(h_unfolded_btag_1d, "b jets unfolded data", "pe1");

    if (!observable.Contains("zpt")) {
        h_true_dijet_inclusive_1d->SetMarkerStyle(kOpenCross);
        h_true_dijet_inclusive_1d->SetMarkerSize(2);
        h_true_dijet_inclusive_1d->SetMarkerColor(mykRed);
        h_true_dijet_inclusive_1d->SetLineColor(mykRed);
        h_stack->Add(h_true_dijet_inclusive_1d, "pe1");
        leg->AddEntry(h_true_dijet_inclusive_1d, "all jets true simulation", "pe1");
    }

    h_sig_true_bjet_btag_1d->SetMarkerStyle(kFullCross);
    h_sig_true_bjet_btag_1d->SetMarkerSize(2);
    h_sig_true_bjet_btag_1d->SetMarkerColor(mykRed);
    h_sig_true_bjet_btag_1d->SetLineColor(mykRed);
    h_stack->Add(h_sig_true_bjet_btag_1d, "pe1");
    leg->AddEntry(h_sig_true_bjet_btag_1d, "b jets true simulation", "pe1");

    TCanvas *c_unfolded = new TCanvas("c_unfolded", "", 1200, 1000);
    TPad *full_pad = new TPad("full_pad", "", 0., 0., 1., 1.);
    TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.3);
    TPad *pad2 = new TPad("pad2", "", 0., 0.3, 1., 1.);
    if (observable.Contains("zpt")) pad2=full_pad;
    else pad2->SetBottomMargin(0.01);
    pad1->SetTopMargin(0.01);
    pad1->SetBottomMargin(0.3);
    

    pad2->cd();
    h_stack->Draw("nostack");
    leg->Draw();

    c_unfolded->cd();
    pad2->Draw();

    h_stack->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
    // h_stack->SetMaximum(2.2);    

    if (!observable.Contains("zpt")) {
        // Ratio histograms
        THStack *h_ratios = new THStack("h_ratios", "");
        h_ratios->SetTitle("; "+xlabel+"; b-tagged / inclusive");

        TH1D *h_unfolded_btag_inclusive_ratio = (TH1D *) h_unfolded_btag_1d->Clone("h_unfolded_btag_inclusive_ratio");
        h_unfolded_btag_inclusive_ratio->Divide(h_true_dijet_inclusive_1d);
        h_unfolded_btag_inclusive_ratio->GetYaxis()->SetTitle("b jets / all jets");
        h_unfolded_btag_inclusive_ratio->GetXaxis()->SetTitle(xlabel);
        h_unfolded_btag_inclusive_ratio->GetXaxis()->SetTitleOffset(4.);
        h_ratios->Add(h_unfolded_btag_inclusive_ratio, "pe1");

        TH1D *h_true_btag_inclusive_ratio = (TH1D *) h_sig_true_bjet_btag_1d->Clone("h_true_btag_inclusive_ratio");
        h_true_btag_inclusive_ratio->Divide(h_true_dijet_inclusive_1d);
        h_true_btag_inclusive_ratio->GetYaxis()->SetTitle("b jets / all jets");
        h_true_btag_inclusive_ratio->GetXaxis()->SetTitle(xlabel);
        h_true_btag_inclusive_ratio->GetXaxis()->SetTitleOffset(4.);
        h_ratios->Add(h_true_btag_inclusive_ratio, "pe1");

        TLine *line = new TLine(x_min, 1., x_max, 1.);
        line->SetLineWidth(2.); 
        line->SetLineStyle(kDashed);
        line->SetLineColor(kGray);

        pad1->cd();
        // h_ratios->Draw("nostack");
        h_unfolded_btag_inclusive_ratio->Draw("pe1");
        h_true_btag_inclusive_ratio->Draw("pe1 same");
        line->Draw();

        c_unfolded->cd();
        pad1->Draw();

        // h_ratios->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        // h_ratios->SetMinimum(0.2); 
        // h_ratios->SetMaximum(1.8);
    } 

    TPaveText *info_top_left = new TPaveText(0., 1., 1., 1.1, "nb ndc");
    info_top_left->SetTextSize(font_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Work in Progress Simulation}");

    TPaveText *info_top_right = new TPaveText(1.2, 1., 2.1, 1.1, "nb ndc");
    info_top_right->SetTextSize(font_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");

    TPaveText *info_jets = new TPaveText(0.2, 0.1, 0.8, 0.35, "nb ndc");
    info_jets->SetTextSize(font_size);
    info_jets->SetFillStyle(0);
    info_jets->SetLineWidth(0);
    // info_jets->AddText("b-tagged b-jets ");
    info_jets->AddText(Form("%.0f < #it{p}_{T}^{jet} < %.0f GeV", pt_min, pt_max));
    info_jets->AddText("-2 < #it{#eta}^{jet} < 2");
    info_jets->AddText("");
    info_jets->AddText("#it{k}_{T} > 1 GeV");

    pad2->cd();
    info_top_left->Draw();
    info_top_right->Draw();
    info_jets->Draw();
    c_unfolded->Draw();

    c_unfolded->Print("plots/unfolded_"+observable+".png");
}