// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldInvert.h"
#include "../draw_utils.h"

void bottomline_test_inclusive(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{
    // ---- Setup 
    gSystem->Load("libRooUnfold.so");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    Double_t ptMin = 100.;
    Double_t ptMax = 120.;

    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab data 
    TString label = "aggrTMVA_inclusive";
    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt; 
    TString fname_data = "../template_fit/histos/data_" + label + "_histograms.root";
    TFile *fin_data = new TFile(fname_data);
    TH3D *h_data = (TH3D *) fin_data->Get("h_data_" + observable);
    h_data->GetYaxis()->SetRange(0, h_data->GetNbinsY() + 1);

    TH2D *h_raw = (TH2D *) h_data->Project3D("zx");
    h_raw->GetXaxis()->SetRange(1, h_raw->GetNbinsX());
    h_raw->GetYaxis()->SetRange(1, h_raw->GetNbinsY());

    // Note: Result = unfold(raw * purity) * 1 / (efficiency)
    //       fakes are negligible

    // ---- Grab response matrix + corrections
    TString sample = "dijet";
    TString fname = "./histos/" + sample + "_" + label + "_response_full" + suffix + ".root";

    TFile *fin = new TFile(fname);
    TH2D *h_training_true = (TH2D *) fin->Get("h_training_true_"+observable+"pt");
    TH2D *h_training_reco = (TH2D *) fin->Get("h_training_reco_"+observable+"pt");
    TH2D *h_training_purity_nominator = (TH2D *) fin->Get("h_training_purity_nominator_"+observable+"pt");
    TH2D *h_training_efficiency_nominator = (TH2D *) fin->Get("h_training_efficiency_nominator_"+observable+"pt");
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin->Get("response_"+observable+"pt");

    // ---- Condition number
    TDecompSVD *svd= new TDecompSVD(response->Mresponse());  // response is a RooUnfold response object, svd is the singular value decomposition (SVD) matrix. the response->Mresponse() returns the normalized migration matrix
    auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
    double cond_number = singular_values.Max() / singular_values.Min();
    std::cout << "\nCondition number = " << cond_number
              << std::endl;

    // ---- Purity correction
    TH2D *h_training_purity = (TH2D *) fin->Get("h_training_purity_"+observable+"pt");

    TH2D *h_raw_purity_corrected = (TH2D *) h_raw->Clone("h_raw_purity_corrected");
    h_raw_purity_corrected->Multiply(h_training_purity);

    TH2D *h_training_reco_purity_corrected = (TH2D *) h_training_reco->Clone("h_training_reco_purity_corrected");
    h_training_reco_purity_corrected->Multiply(h_training_purity);

    // ---- Unfold
    // Int_t niter = 5;
    // RooUnfoldBayes unfold(response, h_raw_purity_corrected, niter);
    RooUnfoldInvert unfold(response, h_raw_purity_corrected);
    TH2D *h_raw_purity_corrected_unfolded = (TH2D *) unfold.Hreco();
    h_raw_purity_corrected_unfolded->SetName("h_raw_purity_corrected_unfolded");

    // ---- Covariance
    // auto covariance = unfold.GetMeasuredCov(); // works

    // ---- Fold back
    // std::cout << "Refolding" << std::endl;
    TH2D *h_raw_purity_corrected_refolded = (TH2D *) response->ApplyToTruth(h_raw_purity_corrected_unfolded, "h_raw_refolded");
    // h_raw_refolded->Divide(h_purity);

    // ---- Efficiency correction 
    TH2D *h_training_efficiency = (TH2D *) fin->Get("h_training_efficiency_"+observable+"pt");

    TH2D *h_raw_purity_corrected_unfolded_efficiency_corrected = (TH2D *) h_raw_purity_corrected_unfolded->Clone("h_raw_purity_corrected_unfolded_efficiency_corrected");
    h_raw_purity_corrected_unfolded_efficiency_corrected->Divide(h_training_efficiency);

    // ---- Make projections
    Int_t iptmin = h_raw->GetYaxis()->FindBin(ptMin);
    Int_t iptmax = h_raw->GetYaxis()->FindBin(ptMax) - 1;
    Double_t ptMin_real = h_raw->GetYaxis()->GetBinLowEdge(iptmin);
    Double_t ptMax_real = h_raw->GetYaxis()->GetBinUpEdge(iptmax);

    TH1D *h_training_true_1d = (TH1D *) h_training_true->ProjectionX("h_training_true_1d", iptmin, iptmax);
    TH1D *h_training_reco_1d = (TH1D *) h_training_reco->ProjectionX("h_training_reco_1d", iptmin, iptmax);
    TH1D *h_training_reco_purity_corrected_1d = (TH1D *) h_training_reco_purity_corrected->ProjectionX("h_training_reco_purity_corrected_1d", iptmin, iptmax);
    TH1D *h_raw_1d = (TH1D *) h_raw->ProjectionX("h_raw_1d", iptmin, iptmax);
    TH1D *h_raw_purity_corrected_1d = (TH1D *) h_raw_purity_corrected->ProjectionX("h_raw_purity_corrected_1d", iptmin, iptmax);
    TH1D *h_raw_purity_corrected_unfolded_1d = (TH1D *) h_raw_purity_corrected_unfolded->ProjectionX("h_raw_purity_corrected_unfolded_1d", iptmin, iptmax);
    TH1D *h_raw_purity_corrected_refolded_1d = (TH1D *) h_raw_purity_corrected_refolded->ProjectionX("h_raw_purity_corrected_refolded_1d", iptmin, iptmax);
    TH1D *h_raw_purity_corrected_unfolded_efficiency_corrected_1d = (TH1D *) h_raw_purity_corrected_unfolded_efficiency_corrected->ProjectionX("h_raw_purity_corrected_unfolded_efficiency_corrected_1d", iptmin, iptmax);
    
    // ---- Normalize
    Int_t nbins_pt = h_raw->GetNbinsY();
    Int_t nbins_x = h_raw->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins_x; 

    double x_min = h_raw_1d->GetXaxis()->GetBinLowEdge(ibin_min);
    double x_max = h_raw_1d->GetXaxis()->GetBinUpEdge(ibin_max);
    for (auto h : {
                   h_training_true_1d, h_training_reco_1d,
                   h_training_reco_purity_corrected_1d,
                   h_raw_1d,
                   h_raw_purity_corrected_1d,
                   h_raw_purity_corrected_unfolded_1d,
                   h_raw_purity_corrected_refolded_1d,
                   h_raw_purity_corrected_unfolded_efficiency_corrected_1d
                   }) {
                    h->GetXaxis()->SetRange(ibin_min, ibin_max);
                    h->Scale(1/h->Integral(ibin_min, ibin_max), "width");
                   }


    // ---- Format histos
    THStack *hStack_data = new THStack("hStack_data", "");
    hStack_data->SetTitle("; "+xlabel+"; 1/N_{jets} dN/d("+xlabel+")");

    THStack *hStack_mc = new THStack("hStack_mc", "");
    hStack_mc->SetTitle("; "+xlabel+"; 1/N_{jets} dN/d("+xlabel+")");

    TLegend *leg = new TLegend(0.2, 0.1, 0.5, 0.4);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin_real, ptMax_real));

    h_raw_purity_corrected_1d->SetMarkerColor(kBlack);
    h_raw_purity_corrected_1d->SetMarkerStyle(kFullCircle);
    h_raw_purity_corrected_1d->SetMarkerSize(1);
    hStack_data->Add(h_raw_purity_corrected_1d, "pe1");
    leg->AddEntry(h_raw_purity_corrected_1d, "Detector level data", "pe1");

    h_training_reco_purity_corrected_1d->SetMarkerColor(kRed);
    h_training_reco_purity_corrected_1d->SetLineColor(kRed);
    h_training_reco_purity_corrected_1d->SetMarkerStyle(kFullTriangleUp);
    h_training_reco_purity_corrected_1d->SetMarkerSize(1);
    hStack_data->Add(h_training_reco_purity_corrected_1d, "pe1");
    leg->AddEntry(h_training_reco_purity_corrected_1d, "Detector level MC", "pe1");

    h_training_true_1d->SetMarkerColor(kRed);
    h_training_true_1d->SetLineColor(kRed);
    h_training_true_1d->SetMarkerStyle(kOpenTriangleUp);
    h_training_true_1d->SetMarkerSize(1);
    hStack_data->Add(h_training_true_1d, "pe1");
    leg->AddEntry(h_training_true_1d, "Particle level MC", "pe1");

    h_raw_purity_corrected_unfolded_efficiency_corrected_1d->SetMarkerColor(kBlue);
    h_raw_purity_corrected_unfolded_efficiency_corrected_1d->SetLineColor(kBlue);
    h_raw_purity_corrected_unfolded_efficiency_corrected_1d->SetMarkerStyle(kOpenCross);
    h_raw_purity_corrected_unfolded_efficiency_corrected_1d->SetMarkerSize(1);
    hStack_data->Add(h_raw_purity_corrected_unfolded_efficiency_corrected_1d, "pe1");
    leg->AddEntry(h_raw_purity_corrected_unfolded_efficiency_corrected_1d, "Unfolded data", "pe1");

    h_raw_purity_corrected_refolded_1d->SetMarkerColor(kBlue);
    h_raw_purity_corrected_refolded_1d->SetLineColor(kBlue);
    h_raw_purity_corrected_refolded_1d->SetMarkerStyle(kFullCross);
    h_raw_purity_corrected_refolded_1d->SetMarkerSize(1);
    hStack_data->Add(h_raw_purity_corrected_refolded_1d, "pe1");
    leg->AddEntry(h_raw_purity_corrected_refolded_1d, "Refolded data", "pe1");

    TCanvas *c_unfold = new TCanvas("c_unfold", "", 800, 600);
    TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.3);
    TPad *pad2 = new TPad("pad2", "", 0., 0.3, 1., 1.);
    pad1->SetTopMargin(0.01);
    pad1->SetBottomMargin(0.3);
    pad2->SetBottomMargin(0.01);

    pad2->cd();
    hStack_data->Draw("nostack");
    leg->Draw();
    TLatex *test_info_text = new TLatex;
    test_info_text->SetNDC();
    test_info_text->SetTextSize(text_size);
    test_info_text->DrawLatex(0.25, 0.45, "BOTTOMLINE TEST");
    test_info_text->Draw();
    drawHeader();


    // ---- RATIO PLOT
    THStack *h_ratio = new THStack("h_ratio", "");
    h_ratio->SetTitle("; "+xlabel+"; ratio");

    TLine *line = new TLine(x_min, 1., x_max, 1.);
    line->SetLineWidth(2.); 
    line->SetLineStyle(kDashed);
    line->SetLineColor(kGray);

    TLegend *leg_ratio = new TLegend(0.5, 0.3, 0.85, 0.5);
    leg_ratio->SetBorderSize(1);

    TH1D *h_raw_purity_corrected_refolded_smeared_ratio = (TH1D *) h_raw_purity_corrected_refolded_1d->Clone("h_raw_purity_corrected_refolded_smeared_ratio");
    h_raw_purity_corrected_refolded_smeared_ratio->Divide(h_training_reco_purity_corrected_1d);
    h_raw_purity_corrected_refolded_smeared_ratio->SetMarkerStyle(kFullCircle);
    h_raw_purity_corrected_refolded_smeared_ratio->SetMarkerColor(kBlack);
    h_raw_purity_corrected_refolded_smeared_ratio->SetLineColor(kBlack);
    h_raw_purity_corrected_refolded_smeared_ratio->SetMarkerSize(1);
    h_raw_purity_corrected_refolded_smeared_ratio->SetMinimum(0.5);
    h_raw_purity_corrected_refolded_smeared_ratio->SetMaximum(1.5);
    h_raw_purity_corrected_refolded_smeared_ratio->GetYaxis()->SetTitle("ratio");
    h_raw_purity_corrected_refolded_smeared_ratio->GetXaxis()->SetTitle(xlabel);
    h_raw_purity_corrected_refolded_smeared_ratio->GetXaxis()->SetTitleOffset(3.5);
    h_ratio->Add(h_raw_purity_corrected_refolded_smeared_ratio, "pe1");
    leg_ratio->AddEntry(h_raw_purity_corrected_refolded_smeared_ratio, "reco data / reco mc", "pe1");

    TH1D *h_raw_purity_corrected_unfolded_efficiency_corrected_ratio = (TH1D *) h_raw_purity_corrected_unfolded_efficiency_corrected_1d->Clone("h_raw_purity_corrected_unfolded_efficiency_corrected_ratio");
    h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->Divide(h_training_true_1d);
    h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->SetMarkerStyle(kOpenCross);
    h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->SetMarkerColor(kBlue);
    h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->SetLineColor(kBlue);
    h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->SetMarkerSize(1);
    h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->SetMinimum(0.8);
    h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->SetMaximum(1.2);
    h_ratio->Add(h_raw_purity_corrected_unfolded_efficiency_corrected_ratio, "pe1");
    leg_ratio->AddEntry(h_raw_purity_corrected_unfolded_efficiency_corrected_ratio, "unfolded data / true mc", "pe1");
    // std::cout << h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->GetBinContent(2) << std::endl;

    pad1->cd();
    h_raw_purity_corrected_refolded_smeared_ratio->Draw("pe1");
    h_raw_purity_corrected_unfolded_efficiency_corrected_ratio->Draw("pe1 same");
    leg_ratio->Draw();
    line->Draw();

    c_unfold->cd();
    pad1->Draw();
    pad2->Draw();
    c_unfold->Draw();
    c_unfold->Print("plots_an/"+sample+"_"+label+"_bottomline_test_"+observable+".png");

    // ---- Covariance after unfolding
    TMatrixD covariance_matrix = unfold.Ereco();
    TH2D *covariance_histogram = new TH2D(covariance_matrix);
    covariance_histogram->GetXaxis()->SetTitle("Detector level " + xlabel + " * p_{T}^{jet} bins");
    covariance_histogram->GetYaxis()->SetTitle("Particle level " + xlabel + " * p_{T}^{jet} bins");
    TCanvas *c_cov = new TCanvas("c_cov", "covariance", 800, 600);
    covariance_histogram->Draw("colz");

    // ---- Correlation after unfolding
    TH2D *correlation_histogram = (TH2D *) covariance_histogram->Clone("correlation_histogram");
    for (int ibinx=0; ibinx <= correlation_histogram->GetNbinsX()+1; ibinx++) {
        for (int ibiny=0; ibiny <= correlation_histogram->GetNbinsY()+1; ibiny++) {
            double covxy = covariance_histogram->GetBinContent(ibinx, ibiny);
            double covxx = covariance_histogram->GetBinContent(ibinx, ibinx);
            double covyy = covariance_histogram->GetBinContent(ibiny, ibiny);
            correlation_histogram->SetBinContent(ibinx, ibiny, covxy/std::sqrt(covxx*covyy));
        }
    }
    correlation_histogram->GetXaxis()->SetTitle("Detector level " + xlabel + " * p_{T}^{jet} bins");
    correlation_histogram->GetYaxis()->SetTitle("Particle level " + xlabel + " * p_{T}^{jet} bins");
    correlation_histogram->GetZaxis()->SetTitle("correlation");
    TCanvas *c_cor = new TCanvas("c_cor", "correlation", 800, 600);
    correlation_histogram->Draw("colz");
    if (sample.Contains("herwig")) drawHeaderHerwig();
    else drawHeaderSimulation();

    for (int ibin_pt=1; ibin_pt<=nbins_pt; ibin_pt++) {
        double y = ibin_pt*nbins_x;
        // std::cout << nbins_pt*nbins_x << std::endl;
        TLine *line_cory = new TLine(0, y, nbins_pt*nbins_x, y);
        TLine *line_corx = new TLine(y, 0, y, nbins_pt*nbins_x);

        c_cov->cd();
        line_cory->Draw();
        line_corx->Draw();

        c_cor->cd();
        line_cory->Draw();
        line_corx->Draw();
    }

    c_cor->Draw();
    c_cor->Print("plots_an/"+sample+"_"+label+"_correlation_"+observable+".png");

    TString fout_name = "./histos/"+sample+"_"+label+"_unfolded_histograms_"+observable+ suffix + ".root";
    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    h_raw_purity_corrected_unfolded_efficiency_corrected->SetName("h_data_unfolded");
    h_raw_purity_corrected_unfolded_efficiency_corrected->Write();

    fout->Close();
    delete fout;
}