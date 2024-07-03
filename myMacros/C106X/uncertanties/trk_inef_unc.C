#include "../draw_utils.h"

void trk_inef_unc(TString observable="rg")
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
    else if (observable=="zpt") xlabel = "z";
    TString ylabel = "1/N dN/d" + xlabel;

    TFile *fin_nom = new TFile("../unfolding/histos/pythia_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_nom = (TH2D *) fin_nom->Get("h_data_unfolded")->Clone("h_nom");
    TFile *fin_inef = new TFile("../unfolding/histos/pythia_0_03_trk_inef_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_inef = (TH2D *) fin_inef->Get("h_data_unfolded")->Clone("h_inef");

    int nbins_x = h_nom->GetNbinsX();
    int nbins_pt = h_nom->GetNbinsY();

    TCanvas *c_inef = new TCanvas("c_inef", "", 800, 600);
    // TPad *pad11 = new TPad("pad11", "", 0., 0., 0.33, 0.35);
    TPad *pad12 = new TPad("pad12", "", 0., 0., 1., 0.33);
    // TPad *pad13 = new TPad("pad13", "", 0.66, 0., 0.99, 0.35);
    // TPad *pad21 = new TPad("pad21", "", 0., 0.35, 0.33, 1.);
    TPad *pad22 = new TPad("pad22", "", 0., 0.33, 1., 1.);
    // TPad *pad23 = new TPad("pad23", "", 0.66, 0.35, 0.99, 1.);

    TPad *pad11, *pad13, *pad21, *pad23;

    std::vector<TPad *> bottom_pads = {pad11, pad12, pad13};
    std::vector<TPad *> top_pads = {pad21, pad22, pad23};
    
    TString fout_name = "./histos/"+observable+"_trk_inef_XXT.root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");
    
    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        if (ibin_pt!=2) continue;
        int ipad = ibin_pt - 1;

        double pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);
        std::cout << ibin_pt << ": " << pt_min << ", " << pt_max << std::endl;

        TLegend *leg;
        if (observable=="rg") leg = new TLegend(0.25, 0.1, 0.65, 0.35);
        if (observable=="zg") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        if (observable=="zpt") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetMargin(0.15);
        leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));

        // Make projections 
        TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX(Form("h_nom_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_inef_1d = (TH1D *) h_inef->ProjectionX(Form("h_inef_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        
        int ibin_x_min = 1;
        int ibin_x_max = nbins_x;

        if (observable!="zpt") ibin_x_min = 2;
        if (observable=="rg") ibin_x_max = nbins_x - 1;
        
        h_nom_1d->SetMarkerStyle(kFullCircle); 
        h_nom_1d->SetMarkerColor(kBlack);
        h_nom_1d->SetLineColor(kBlack);
        leg->AddEntry(h_nom_1d, "nominal", "pe1");

        h_inef_1d->SetMarkerStyle(kFullTriangleUp); 
        h_inef_1d->SetMarkerColor(kRed);
        h_inef_1d->SetLineColor(kRed);
        leg->AddEntry(h_inef_1d, "3% trk inef", "pe1");

        double ymax = 0;

        top_pads[ipad]->cd();
        // top_pads[ipad]->SetRightMargin(0.05);
        // top_pads[ipad]->SetLeftMargin(0.2);
        top_pads[ipad]->SetBottomMargin(0.01);
        for (auto h : {h_nom_1d, h_inef_1d}) {
            h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
            h->Scale(1/h->Integral(), "width");
            h->GetYaxis()->SetTitle(ylabel);
            h->GetYaxis()->SetTitleOffset(2);
            double maxh = h->GetBinContent(h->GetMaximumBin());
            if (maxh>ymax) ymax=maxh;
        }
        if (ymax<0) ymax*=0.9;
        else ymax*=1.1;
        h_nom_1d->GetYaxis()->SetRangeUser(0, ymax);
        h_inef_1d->GetYaxis()->SetRangeUser(0, ymax);
        h_nom_1d->Draw("pe1 same");
        h_inef_1d->Draw("pe1 same");
        leg->Draw();
        drawHeader();

        // Extract uncertainties 
        TH1D * h_inef_unc = (TH1D *) h_inef_1d->Clone(Form("h_inef_unc_%d", ibin_pt));
        h_inef_unc->Add(h_nom_1d, -1);
        h_inef_unc->GetYaxis()->SetTitle("uncertainty");
        h_inef_unc->GetYaxis()->SetTitleOffset(2);
        h_inef_unc->GetXaxis()->SetTitle(xlabel);
        h_inef_unc->GetXaxis()->SetTitleOffset(3.);

        // fout->cd();
        h_inef_unc->Write();

        // save relative uncertainty 
        TH1D *h_inef_unc_rel = (TH1D *) h_inef_unc->Clone(Form("h_inef_unc_rel_%d", ibin_pt));
        h_inef_unc_rel->Divide(h_nom_1d);
        h_inef_unc_rel->GetYaxis()->SetTitle("(var-nom)/nom");
        h_inef_unc_rel->GetYaxis()->SetTitleOffset(2);
        if (observable=="rg") h_inef_unc_rel->GetYaxis()->SetRangeUser(-0.1, 0.1);
        else if (observable=="zg") h_inef_unc_rel->GetYaxis()->SetRangeUser(-0.02, 0.02);
        else if (observable=="zpt") h_inef_unc_rel->GetYaxis()->SetRangeUser(-0.1, 0.1);
        h_inef_unc_rel->Write();

        bottom_pads[ipad]->cd();
        bottom_pads[ipad]->SetTopMargin(0.01);
        bottom_pads[ipad]->SetBottomMargin(0.3);
        // bottom_pads[ipad]->SetRightMargin(0.05);
        // bottom_pads[ipad]->SetLeftMargin(0.2);
        
        // h_inef_unc->Draw("hist same");
        h_inef_unc_rel->Draw("pe1");

        TH1D *h_band = (TH1D *) h_inef_unc_rel->Clone("h_band");
        h_band->Reset();
        for (int ibin_x=1; ibin_x<=h_inef_unc_rel->GetNbinsX(); ibin_x++) {
            double maxUnc = h_inef_unc_rel->GetBinContent(ibin_x);
            h_band->SetBinContent(ibin_x, 0.);
            h_band->SetBinError(ibin_x, maxUnc);
        }

        h_band->SetMarkerSize(0);
        h_band->SetFillStyle(1001);
        h_band->SetFillColorAlpha(kBlack, 0.05);
        h_band->Draw("e2 same");

        TLine *line = new TLine(h_inef_unc->GetXaxis()->GetBinLowEdge(ibin_x_min), 0, h_inef_unc->GetXaxis()->GetBinUpEdge(ibin_x_max), 0);
        line->Draw();

        c_inef->cd();
        top_pads[ipad]->Draw();
        bottom_pads[ipad]->Draw();
    }
    c_inef->Draw();
    c_inef->Print("plots_an/trk_inef_unc_XXT_"+observable+".png");
    // fout->Close();
    // delete fout;
}