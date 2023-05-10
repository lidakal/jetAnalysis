#include "../myPalette.h"

void draw_rocs()
{
    // ---- Settings
    float ptMin = 100.;
    float ptMax = 120.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos
    TFile *faod = new TFile("./histos/aod_discr.root");
    TH2F *hB_discr_aod = (TH2F *) faod->Get("hB_discr");
    hB_discr_aod->SetName("hB_discr_aod");
    TH2F *hL_discr_aod = (TH2F *) faod->Get("hL_discr");
    hL_discr_aod->SetName("hL_discr_aod");
    TH2F *hC_discr_aod = (TH2F *) faod->Get("hC_discr");
    hC_discr_aod->SetName("hC_discr_aod");

    TFile *fmini = new TFile("./histos/mini_discr.root");
    TH2F *hB_discr_mini = (TH2F *) fmini->Get("hB_discr");
    hB_discr_mini->SetName("hB_discr_mini");
    TH2F *hL_discr_mini = (TH2F *) fmini->Get("hL_discr");
    hL_discr_mini->SetName("hL_discr_mini");
    TH2F *hC_discr_mini = (TH2F *) fmini->Get("hC_discr");
    hC_discr_mini->SetName("hC_discr_mini");

    // ---- Calculate roc
    Int_t iymin = hB_discr_aod->GetYaxis()->FindBin(ptMin);
    Int_t iymax = hB_discr_aod->GetYaxis()->FindBin(ptMax) - 1;

    int nbins = hB_discr_aod->GetNbinsX();

    TLegend *leg_roc = new TLegend(0.2, 0.65, 0.5, 0.85);
    leg_roc->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin, ptMax));
    leg_roc->SetFillStyle(0);
    leg_roc->SetBorderSize(0);
    leg_roc->SetMargin(0.15);

    TMultiGraph *rocs = new TMultiGraph();
    rocs->GetXaxis()->SetTitle("b jet efficiency");
    rocs->GetYaxis()->SetTitle("mistag rate");


    TGraph *b_vs_c_aod = new TGraph(nbins);
    b_vs_c_aod->SetMarkerStyle(21);
    b_vs_c_aod->SetMarkerColor(mykRed);
    b_vs_c_aod->SetMarkerSize(2);
    b_vs_c_aod->SetLineColor(b_vs_c_aod->GetMarkerColor());
    b_vs_c_aod->SetLineStyle(1);
    rocs->Add(b_vs_c_aod);
    leg_roc->AddEntry(b_vs_c_aod, "b vs c, aod", "pl");

    TGraph *b_vs_l_aod = new TGraph(nbins);
    b_vs_l_aod->SetMarkerStyle(25);
    b_vs_l_aod->SetMarkerColor(mykRedLight);
    b_vs_l_aod->SetMarkerSize(2);
    b_vs_l_aod->SetLineColor(b_vs_l_aod->GetMarkerColor());
    b_vs_l_aod->SetLineStyle(kDashed);
    rocs->Add(b_vs_l_aod);
    leg_roc->AddEntry(b_vs_l_aod, "b vs guds, aod", "pl");

    TGraph *b_vs_c_mini = new TGraph(nbins);
    b_vs_c_mini->SetMarkerStyle(20);
    b_vs_c_mini->SetMarkerColor(mykBlue);
    b_vs_c_mini->SetMarkerSize(2);
    b_vs_c_mini->SetLineColor(b_vs_c_mini->GetMarkerColor());
    b_vs_c_mini->SetLineStyle(1);
    rocs->Add(b_vs_c_mini);
    leg_roc->AddEntry(b_vs_c_mini, "b vs c, mini", "pl");

    TGraph *b_vs_l_mini = new TGraph(nbins);
    b_vs_l_mini->SetMarkerStyle(24);
    b_vs_l_mini->SetMarkerColor(mykBlueLight);
    b_vs_l_mini->SetMarkerSize(2);
    b_vs_l_mini->SetLineColor(b_vs_l_mini->GetMarkerColor());
    b_vs_l_mini->SetLineStyle(kDashed);
    rocs->Add(b_vs_l_mini);
    leg_roc->AddEntry(b_vs_l_mini, "b vs guds, mini", "pl");

    for (int ibin = 1; ibin <= nbins; ibin++) {
        double eff_aod = hB_discr_aod->Integral(ibin, nbins, iymin, iymax) / hB_discr_aod->Integral(0, -1, iymin, iymax);
        double cmis_aod = hC_discr_aod->Integral(ibin, nbins, iymin, iymax) / hC_discr_aod->Integral(0, -1, iymin, iymax);
        double lmis_aod = hL_discr_aod->Integral(ibin, nbins, iymin, iymax) / hL_discr_aod->Integral(0, -1, iymin, iymax);

        b_vs_c_aod->SetPoint(ibin - 1, eff_aod, cmis_aod);
        b_vs_l_aod->SetPoint(ibin - 1, eff_aod, lmis_aod);

        double eff_mini = hB_discr_mini->Integral(ibin, nbins, iymin, iymax) / hB_discr_mini->Integral(0, -1, iymin, iymax);
        double cmis_mini = hC_discr_mini->Integral(ibin, nbins, iymin, iymax) / hC_discr_mini->Integral(0, -1, iymin, iymax);
        double lmis_mini = hL_discr_mini->Integral(ibin, nbins, iymin, iymax) / hL_discr_mini->Integral(0, -1, iymin, iymax);

        b_vs_c_mini->SetPoint(ibin - 1, eff_mini, cmis_mini);
        b_vs_l_mini->SetPoint(ibin - 1, eff_mini, lmis_mini);
    }

    TCanvas *c_roc = new TCanvas("c_roc", "", 1200, 1000);
    c_roc->SetLogy();
    rocs->Draw("pla");
    leg_roc->Draw();
    c_roc->Draw();
    


}