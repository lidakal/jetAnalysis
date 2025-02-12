#include "../draw_utils.h"

void model_unc_v2(TString observable="rg")
{
     // Setup 
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch}";
    TString ylabel = "1/N dN/d" + xlabel;

    TFile *fin_nom = new TFile("../unfolding/histos/pythia_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_nom = (TH2D *) fin_nom->Get("h_data_unfolded")->Clone("h_nom");
    TFile *fin_herwigFull = new TFile("../unfolding/histos/herwig_official_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_herwigFull = (TH2D *) fin_herwigFull->Get("h_data_unfolded")->Clone("h_herwigFull");
    TFile *fin_herwigPurity = new TFile("../unfolding/histos/herwig_official_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF_herwigPurity.root");
    TH2D *h_herwigPurity = (TH2D *) fin_herwigPurity->Get("h_data_unfolded")->Clone("h_herwigPurity");
    TFile *fin_herwigResponse = new TFile("../unfolding/histos/herwig_official_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF_herwigResponse.root");
    TH2D *h_herwigResponse = (TH2D *) fin_herwigResponse->Get("h_data_unfolded")->Clone("h_herwigResponse");
    TFile *fin_herwigEfficiency = new TFile("../unfolding/histos/herwig_official_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF_herwigEfficiency.root");
    TH2D *h_herwigEfficiency = (TH2D *) fin_herwigEfficiency->Get("h_data_unfolded")->Clone("h_herwigEfficiency");
    TFile *fin_herwigBtagEfficiency = new TFile("../unfolding/histos/herwig_official_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF_herwigBtagEfficiency.root");
    TH2D *h_herwigBtagEfficiency = (TH2D *) fin_herwigBtagEfficiency->Get("h_data_unfolded")->Clone("h_herwigBtagEfficiency");

    int nbins_x = h_nom->GetNbinsX();
    int nbins_pt = h_nom->GetNbinsY();

    TCanvas *c_model = new TCanvas("c_model", "", 800, 600);
    // TPad *pad11 = new TPad("pad11", "", 0., 0., 0.33, 0.35);
    TPad *pad12 = new TPad("pad12", "", 0., 0., 1., 0.33);
    // TPad *pad13 = new TPad("pad13", "", 0.66, 0., 0.99, 0.35);
    // TPad *pad21 = new TPad("pad21", "", 0., 0.35, 0.33, 1.);
    TPad *pad22 = new TPad("pad22", "", 0., 0.33, 1., 1.);
    // TPad *pad23 = new TPad("pad23", "", 0.66, 0.35, 0.99, 1.);

    TPad *pad11, *pad13, *pad21, *pad23;

    std::vector<TPad *> bottom_pads = {pad11, pad12, pad13};
    std::vector<TPad *> top_pads = {pad21, pad22, pad23};
    
    TFile *fout = new TFile("./histos/"+observable+"_model_unc_XXT.root", "recreate");

    int ibin_x_min = 1;
    int ibin_x_max = h_nom->GetNbinsX();
    // if (observable != "zpt") ibin_x_min = 2;
    // if (observable == "rg") ibin_x_max = h_nom->GetNbinsX() - 1;
    
    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        if (ibin_pt!=2) continue;
        int ipad = ibin_pt - 1;

        double pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);
        std::cout << ibin_pt << ": " << pt_min << ", " << pt_max << std::endl;

        TLegend *leg;
        if (observable=="rg") leg = new TLegend(0.25, 0.1, 0.65, 0.35);
        if (observable=="zg") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        if (observable=="zpt") leg = new TLegend(0.2, 0.4, 0.6, 0.85);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetMargin(0.15);
        leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));

        // Make projections 
        TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX(Form("h_nom_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_herwigFull_1d = (TH1D *) h_herwigFull->ProjectionX(Form("h_herwigFull_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_herwigPurity_1d = (TH1D *) h_herwigPurity->ProjectionX(Form("h_herwigPurity_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_herwigResponse_1d = (TH1D *) h_herwigResponse->ProjectionX(Form("h_herwigResponse_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_herwigEfficiency_1d = (TH1D *) h_herwigEfficiency->ProjectionX(Form("h_herwigEfficiency_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_herwigBtagEfficiency_1d = (TH1D *) h_herwigBtagEfficiency->ProjectionX(Form("h_herwigBtagEfficiency_1d_%d", ibin_pt), ibin_pt, ibin_pt);

        if (observable!="zpt") ibin_x_min = 2;
        if (observable=="rg") ibin_x_max = nbins_x - 1;

        h_nom_1d->SetMarkerStyle(kFullCircle); 
        h_nom_1d->SetMarkerColor(kBlack);
        h_nom_1d->SetLineColor(kBlack);
        leg->AddEntry(h_nom_1d, "pythia full (nominal)", "pe1");

        h_herwigPurity_1d->SetMarkerStyle(kFullTriangleUp); 
        h_herwigPurity_1d->SetMarkerColor(kRed);
        h_herwigPurity_1d->SetLineColor(kRed);
        leg->AddEntry(h_herwigPurity_1d, "herwig purity", "pe1");

        h_herwigResponse_1d->SetMarkerStyle(kFullSquare); 
        h_herwigResponse_1d->SetMarkerColor(kGreen);
        h_herwigResponse_1d->SetLineColor(kGreen);
        leg->AddEntry(h_herwigResponse_1d, "herwig response", "pe1");

        h_herwigEfficiency_1d->SetMarkerStyle(kFullTriangleDown); 
        h_herwigEfficiency_1d->SetMarkerColor(kBlue);
        h_herwigEfficiency_1d->SetLineColor(kBlue);
        leg->AddEntry(h_herwigEfficiency_1d, "herwig efficiency", "pe1");

        h_herwigBtagEfficiency_1d->SetMarkerStyle(kFullDiamond); 
        h_herwigBtagEfficiency_1d->SetMarkerColor(kMagenta);
        h_herwigBtagEfficiency_1d->SetLineColor(kMagenta);
        leg->AddEntry(h_herwigBtagEfficiency_1d, "herwig btag efficiency", "pe1");

        h_herwigFull_1d->SetMarkerStyle(kOpenCircle); 
        h_herwigFull_1d->SetMarkerColor(kOrange);
        h_herwigFull_1d->SetLineColor(kOrange);
        leg->AddEntry(h_herwigFull_1d, "herwig full", "pe1");

        top_pads[ipad]->cd();
        // top_pads[ipad]->SetRightMargin(0.05);
        // top_pads[ipad]->SetLeftMargin(0.2);
        top_pads[ipad]->SetBottomMargin(0.01);
        for (auto h : {h_nom_1d, h_herwigPurity_1d, h_herwigResponse_1d, h_herwigEfficiency_1d, h_herwigBtagEfficiency_1d, h_herwigFull_1d}) {
            h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
            h->Scale(1/h->Integral(), "width");
            h->GetYaxis()->SetTitle(ylabel);
            h->GetYaxis()->SetTitleOffset(2.);
        }
        h_nom_1d->Draw("pe1 same");
        h_herwigFull_1d->Draw("pe1 same");
        h_herwigPurity_1d->Draw("pe1 same");
        h_herwigResponse_1d->Draw("pe1 same");
        h_herwigEfficiency_1d->Draw("pe1 same");
        h_herwigBtagEfficiency_1d->Draw("pe1 same");
        h_nom_1d->Draw("pe1 same");
        leg->Draw();
        // drawHeader();

        // Extract uncertainties 
        TH1D * h_model_unc_herwigPurity = (TH1D *) h_herwigPurity_1d->Clone(Form("h_model_unc_%d", ibin_pt));
        h_model_unc_herwigPurity->Add(h_nom_1d, -1);
        h_model_unc_herwigPurity->GetYaxis()->SetTitle("var-nom");
        h_model_unc_herwigPurity->GetYaxis()->SetTitleOffset(2.);
        h_model_unc_herwigPurity->GetXaxis()->SetTitle(xlabel);
        h_model_unc_herwigPurity->GetXaxis()->SetTitleOffset(3.);

        TH1D * h_model_unc_herwigResponse = (TH1D *) h_herwigResponse_1d->Clone(Form("h_model_unc_herwigResponse_%d", ibin_pt));
        h_model_unc_herwigResponse->Add(h_nom_1d, -1);

        TH1D * h_model_unc_herwigEfficiency = (TH1D *) h_herwigEfficiency_1d->Clone(Form("h_model_unc_herwigEfficiency_%d", ibin_pt));
        h_model_unc_herwigEfficiency->Add(h_nom_1d, -1);

        TH1D * h_model_unc_herwigBtagEfficiency = (TH1D *) h_herwigBtagEfficiency_1d->Clone(Form("h_model_unc_herwigBtagEfficiency_%d", ibin_pt));
        h_model_unc_herwigBtagEfficiency->Add(h_nom_1d, -1);

        TH1D * h_model_unc_herwigFull = (TH1D *) h_herwigFull_1d->Clone(Form("h_model_unc_herwigFull_%d", ibin_pt));
        h_model_unc_herwigFull->Add(h_nom_1d, -1);

        fout->cd();
        h_model_unc_herwigFull->Write();
        h_model_unc_herwigPurity->Write();
        h_model_unc_herwigResponse->Write();
        h_model_unc_herwigEfficiency->Write();
        h_model_unc_herwigBtagEfficiency->Write();

        // save relative uncertainty 
        TH1D *h_model_unc_herwigPurity_rel = (TH1D *) h_model_unc_herwigPurity->Clone(Form("h_model_unc_rel_%d", ibin_pt));
        h_model_unc_herwigPurity_rel->Divide(h_nom_1d);
        h_model_unc_herwigPurity_rel->GetYaxis()->SetTitle("(var-nom)/nom");
        h_model_unc_herwigPurity_rel->GetYaxis()->SetTitleOffset(2.);
        h_model_unc_herwigPurity_rel->SetMinimum(-0.3);
        h_model_unc_herwigPurity_rel->SetMaximum(0.3);

        TH1D *h_model_unc_herwigResponse_rel = (TH1D *) h_model_unc_herwigResponse->Clone(Form("h_model_unc_herwigResponse_rel_%d", ibin_pt));
        h_model_unc_herwigResponse_rel->Divide(h_nom_1d);

        TH1D *h_model_unc_herwigEfficiency_rel = (TH1D *) h_model_unc_herwigEfficiency->Clone(Form("h_model_unc_herwigEfficiency_rel_%d", ibin_pt));
        h_model_unc_herwigEfficiency_rel->Divide(h_nom_1d);

        TH1D *h_model_unc_herwigBtagEfficiency_rel = (TH1D *) h_model_unc_herwigBtagEfficiency->Clone(Form("h_model_unc_herwigBtagEfficiency_rel_%d", ibin_pt));
        h_model_unc_herwigBtagEfficiency_rel->Divide(h_nom_1d);

        TH1D *h_model_unc_herwigFull_rel = (TH1D *) h_model_unc_herwigFull->Clone(Form("h_model_unc_herwigFull_rel_%d", ibin_pt));
        h_model_unc_herwigFull_rel->Divide(h_nom_1d);

        h_model_unc_herwigFull_rel->Write();
        h_model_unc_herwigPurity_rel->Write();
        h_model_unc_herwigResponse_rel->Write();
        h_model_unc_herwigEfficiency_rel->Write();
        h_model_unc_herwigBtagEfficiency_rel->Write();

        bottom_pads[ipad]->cd();
        bottom_pads[ipad]->SetTopMargin(0.01);
        bottom_pads[ipad]->SetBottomMargin(0.3);
        // bottom_pads[ipad]->SetRightMargin(0.05);
        // bottom_pads[ipad]->SetLeftMargin(0.2);
        
        // h_model_unc_herwigPurity->Draw("hist same");
        h_model_unc_herwigPurity_rel->Draw("pe1");
        h_model_unc_herwigResponse_rel->Draw("pe1 same");
        h_model_unc_herwigEfficiency_rel->Draw("pe1 same");
        h_model_unc_herwigBtagEfficiency_rel->Draw("pe1 same");
        h_model_unc_herwigFull_rel->Draw("pe1 same");

        TH1D *h_unc_rel = (TH1D *) h_model_unc_herwigFull_rel->Clone(Form("h_unc_rel_%d",ibin_pt));
        TH1D *h_band = (TH1D *) h_model_unc_herwigFull_rel->Clone("h_band");
        h_band->Reset();
        for (int ibin_x=1; ibin_x<=h_model_unc_herwigFull_rel->GetNbinsX(); ibin_x++) {
            double maxUnc = std::abs(h_model_unc_herwigFull_rel->GetBinContent(ibin_x));
            h_band->SetBinContent(ibin_x, 0.);
            h_band->SetBinError(ibin_x, maxUnc);
            h_unc_rel->SetBinContent(ibin_x, maxUnc);
        }
        h_unc_rel->Write();

        h_band->SetMarkerSize(0);
        h_band->SetFillStyle(1001);
        h_band->SetFillColorAlpha(kBlack, 0.05);
        // h_band->Draw("e2 same");

        TLine *line = new TLine(h_model_unc_herwigPurity->GetXaxis()->GetBinLowEdge(ibin_x_min), 0, h_model_unc_herwigPurity->GetXaxis()->GetBinUpEdge(ibin_x_max), 0);
        line->SetLineStyle(kDashed);
        line->Draw();

        c_model->cd();
        top_pads[ipad]->Draw();
        bottom_pads[ipad]->Draw();

        // add four independent variations in quadrature vs full
        TH1D *h_model_unc_herwigSum_rel = (TH1D *) h_model_unc_herwigPurity_rel->Clone("h_model_unc_herwigSum_rel");
        for (int ibin=1; ibin<h_model_unc_herwigSum_rel->GetNbinsX(); ibin++) {
            double uncPurity = h_model_unc_herwigPurity_rel->GetBinContent(ibin);
            double uncResponse = h_model_unc_herwigResponse_rel->GetBinContent(ibin);
            double uncEfficiency = h_model_unc_herwigEfficiency_rel->GetBinContent(ibin);
            double uncBtagEfficiency = h_model_unc_herwigBtagEfficiency_rel->GetBinContent(ibin);

            double uncSum = std::sqrt((uncPurity*uncPurity) 
                                    +(uncResponse*uncResponse)
                                    +(uncEfficiency*uncEfficiency)
                                    +(uncBtagEfficiency*uncBtagEfficiency)
                                    );
            h_model_unc_herwigSum_rel->SetBinContent(ibin, uncSum);
        }
        h_model_unc_herwigSum_rel->Write();

        TCanvas *c_unc = new TCanvas("c_unc","",800,600);
        h_model_unc_herwigSum_rel->GetYaxis()->SetRangeUser(0., 0.5);
        h_model_unc_herwigSum_rel->GetXaxis()->SetTitle(xlabel);
        h_model_unc_herwigSum_rel->GetXaxis()->SetTitleOffset(1.2);
        h_model_unc_herwigSum_rel->GetYaxis()->SetTitle("Relative uncertainty");
        h_model_unc_herwigSum_rel->Draw();
        h_unc_rel->Draw("same");

        c_unc->BuildLegend();
    }
    c_model->cd();
    // c_model->Print("plots_an/model_response_unc_XXT_"+observable+".png");
    // fout->Close();
    // delete fout;
}