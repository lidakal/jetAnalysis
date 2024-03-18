#include "../draw_utils.h"

void cl_frac_unc(TString observable="rg")
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

    TFile *fin_nom = new TFile("../unfolding/histos/bjet_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_nom = (TH2D *) fin_nom->Get("h_data_unfolded")->Clone("h_nom");
    TFile *fin_up = new TFile("../unfolding/histos/bjet_aggrTMVA_XXT_CLfracUp_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_up = (TH2D *) fin_up->Get("h_data_unfolded")->Clone("h_up");
    TFile *fin_down = new TFile("../unfolding/histos/bjet_aggrTMVA_XXT_CLfracDown_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_down = (TH2D *) fin_down->Get("h_data_unfolded")->Clone("h_down");

    int nbins_x = h_nom->GetNbinsX();
    int nbins_pt = h_nom->GetNbinsY();

    TCanvas *c_cl_frac = new TCanvas("c_cl_frac", "", 800, 600);
    // TPad *pad11 = new TPad("pad11", "", 0., 0., 0.33, 0.35);
    TPad *pad12 = new TPad("pad12", "", 0., 0., 1., 0.33);
    // TPad *pad13 = new TPad("pad13", "", 0.66, 0., 0.99, 0.35);
    // TPad *pad21 = new TPad("pad21", "", 0., 0.35, 0.33, 1.);
    TPad *pad22 = new TPad("pad22", "", 0., 0.33, 1., 1.);
    // TPad *pad23 = new TPad("pad23", "", 0.66, 0.35, 0.99, 1.);

    TPad *pad11, *pad13, *pad21, *pad23;

    std::vector<TPad *> bottom_pads = {pad11, pad12, pad13};
    std::vector<TPad *> top_pads = {pad21, pad22, pad23};
    
    TFile *fout = new TFile("./histos/"+observable+"_cl_frac_unc_XXT.root", "recreate");
    
    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        if (ibin_pt!=2) continue;
        int ipad = ibin_pt - 1;

        double pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);
        std::cout << ibin_pt << ": " << pt_min << ", " << pt_max << std::endl;

        TLegend *leg;
        if (observable=="rg") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        if (observable=="zg") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        if (observable=="zpt") leg = new TLegend(0.3, 0.55, 0.6, 0.85);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetMargin(0.15);
        leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));

        // Make projections 
        TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX(Form("h_nom_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_up_1d = (TH1D *) h_up->ProjectionX(Form("h_up_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_down_1d = (TH1D *) h_down->ProjectionX(Form("h_down_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        
        int ibin_x_min = 1;
        int ibin_x_max = nbins_x;

        // if (observable=="rg") {
        //     ibin_x_min = 2;
        //     ibin_x_max = nbins_x - 1;
        // }

        h_nom_1d->SetMarkerStyle(kFullCircle); 
        h_nom_1d->SetMarkerColor(kBlack);
        h_nom_1d->SetLineColor(kBlack);
        leg->AddEntry(h_nom_1d, "nominal", "pe1");

        h_up_1d->SetMarkerStyle(kFullTriangleUp); 
        h_up_1d->SetMarkerColor(kRed);
        h_up_1d->SetLineColor(kRed);
        leg->AddEntry(h_up_1d, "cl fraction up", "pe1");

        h_down_1d->SetMarkerStyle(kFullTriangleDown); 
        h_down_1d->SetMarkerColor(kGreen);
        h_down_1d->SetLineColor(kGreen);
        leg->AddEntry(h_down_1d, "cl fraction down", "pe1");

        top_pads[ipad]->cd();
        // top_pads[ipad]->SetRightMargin(0.05);
        // top_pads[ipad]->SetLeftMargin(0.2);
        top_pads[ipad]->SetBottomMargin(0.01);
        for (auto h : {h_nom_1d, h_up_1d, h_down_1d}) {
            h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
            h->Scale(1/h->Integral(), "width");
            h->GetYaxis()->SetTitle(ylabel);
            h->GetYaxis()->SetTitleOffset(2.);
            h->Draw("pe1 same");
        }
        leg->Draw();
        drawHeader();

        // Extract uncertainties 
        TH1D * h_unc_up = (TH1D *) h_up_1d->Clone(Form("h_unc_up_%d", ibin_pt));
        h_unc_up->Add(h_nom_1d, -1);
        h_unc_up->GetYaxis()->SetTitle("uncertainty");
        h_unc_up->GetYaxis()->SetTitleOffset(2.);
        h_unc_up->GetXaxis()->SetTitle(xlabel);
        h_unc_up->GetXaxis()->SetTitleOffset(3.);

        TH1D * h_unc_down = (TH1D *) h_down_1d->Clone(Form("h_unc_down_%d", ibin_pt));
        h_unc_down->Add(h_nom_1d, -1);
        h_unc_down->GetYaxis()->SetTitle("uncertainty");
        h_unc_down->GetYaxis()->SetTitleOffset(2.);
        h_unc_down->GetXaxis()->SetTitle(xlabel);
        h_unc_down->GetXaxis()->SetTitleOffset(3.);

        // fout->cd();
        h_unc_up->Write();
        h_unc_down->Write();

        // save relative uncertainty 
        TH1D *h_unc_up_rel = (TH1D *) h_unc_up->Clone(Form("h_unc_up_rel_%d", ibin_pt));
        h_unc_up_rel->Divide(h_nom_1d);
        h_unc_up_rel->GetYaxis()->SetTitle("(var-nom)/nom");
        h_unc_up_rel->GetYaxis()->SetTitleOffset(2.);
        h_unc_up_rel->GetYaxis()->SetRangeUser(-0.25,0.25);
        h_unc_up_rel->Write();

        TH1D *h_unc_down_rel = (TH1D *) h_unc_down->Clone(Form("h_unc_down_rel_%d", ibin_pt));
        h_unc_down_rel->Divide(h_nom_1d);
        h_unc_down_rel->Write();

        bottom_pads[ipad]->cd();
        bottom_pads[ipad]->SetTopMargin(0.01);
        bottom_pads[ipad]->SetBottomMargin(0.3);
        // bottom_pads[ipad]->SetRightMargin(0.05);
        // bottom_pads[ipad]->SetLeftMargin(0.2);

        h_unc_up_rel->Draw("pe1");
        h_unc_down_rel->Draw("pe1 same");
        
        TLine *line = new TLine(h_unc_up->GetXaxis()->GetBinLowEdge(ibin_x_min), 0, h_unc_up->GetXaxis()->GetBinUpEdge(ibin_x_max), 0);
        line->SetLineStyle(kDashed);
        line->Draw();

        c_cl_frac->cd();
        top_pads[ipad]->Draw();
        bottom_pads[ipad]->Draw();

        
    }

    c_cl_frac->Draw();
    c_cl_frac->Print("plots_an/cl_frac_unc_XXT_"+observable+".png");

    // fout->Close();
    // delete fout;
}