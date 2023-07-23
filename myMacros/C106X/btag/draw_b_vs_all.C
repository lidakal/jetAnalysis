#include "../myPalette.h"

void draw_b_vs_all()
{
    // ---- Settings
    float ptMin = 80.;
    float ptMax = 100.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos
    TFile *fmini = new TFile("./histos/mini_discr.root");
    TH2F *hSingleB_discr_mini = (TH2F *) fmini->Get("hSingleB_discr");
    hSingleB_discr_mini->SetName("hSingleB_discr_mini");
    TH2F *hBB_discr_mini = (TH2F *) fmini->Get("hBB_discr");
    hBB_discr_mini->SetName("hBB_discr_mini");
    TH2F *hL_discr_mini = (TH2F *) fmini->Get("hL_discr");
    hL_discr_mini->SetName("hL_discr_mini");
    TH2F *hC_discr_mini = (TH2F *) fmini->Get("hC_discr");
    hC_discr_mini->SetName("hC_discr_mini");

    TFile *faod = new TFile("./histos/aod_discr.root");
    TH2F *hSingleB_discr_aod = (TH2F *) faod->Get("hSingleB_discr");
    hSingleB_discr_aod->SetName("hSingleB_discr_aod");
    TH2F *hBB_discr_aod = (TH2F *) faod->Get("hBB_discr");
    hBB_discr_aod->SetName("hBB_discr_aod");
    TH2F *hL_discr_aod = (TH2F *) faod->Get("hL_discr");
    hL_discr_aod->SetName("hL_discr_aod");
    TH2F *hC_discr_aod = (TH2F *) faod->Get("hC_discr");
    hC_discr_aod->SetName("hC_discr_aod");

    // ---- Calculate roc
    Int_t iymin = hSingleB_discr_mini->GetYaxis()->FindBin(ptMin);
    Int_t iymax = hSingleB_discr_mini->GetYaxis()->FindBin(ptMax) - 1;

    int nbins = hSingleB_discr_mini->GetNbinsX();

    TLegend *leg_roc = new TLegend(0.6, 0.2, 1., 0.45);
    leg_roc->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin, ptMax));
    leg_roc->SetFillStyle(0);
    leg_roc->SetBorderSize(0);
    leg_roc->SetMargin(0.15);

    TMultiGraph *rocs = new TMultiGraph();
    rocs->GetXaxis()->SetTitle("b jet efficiency");
    rocs->GetYaxis()->SetTitle("mistag rate");
    rocs->GetYaxis()->SetTitleOffset(0.01);

    // Add mini ROCs
    TGraph *b_vs_bb_mini = new TGraph(nbins);
    b_vs_bb_mini->SetMarkerStyle(20);
    b_vs_bb_mini->SetMarkerColor(mykBlue);
    b_vs_bb_mini->SetMarkerSize(2);
    b_vs_bb_mini->SetLineColor(b_vs_bb_mini->GetMarkerColor());
    b_vs_bb_mini->SetLineStyle(1);
    rocs->Add(b_vs_bb_mini);
    leg_roc->AddEntry(b_vs_bb_mini, "b vs bb, mini", "pl");

    TGraph *b_vs_c_mini = new TGraph(nbins);
    b_vs_c_mini->SetMarkerStyle(20);
    b_vs_c_mini->SetMarkerColor(mykGreen);
    b_vs_c_mini->SetMarkerSize(2);
    b_vs_c_mini->SetLineColor(b_vs_c_mini->GetMarkerColor());
    b_vs_c_mini->SetLineStyle(1);
    rocs->Add(b_vs_c_mini);
    leg_roc->AddEntry(b_vs_c_mini, "b vs c, mini", "pl");

    TGraph *b_vs_l_mini = new TGraph(nbins);
    b_vs_l_mini->SetMarkerStyle(kFullCircle);
    b_vs_l_mini->SetMarkerColor(mykRed);
    b_vs_l_mini->SetMarkerSize(2);
    b_vs_l_mini->SetLineColor(b_vs_l_mini->GetMarkerColor());
    b_vs_l_mini->SetLineStyle(1);
    rocs->Add(b_vs_l_mini);
    leg_roc->AddEntry(b_vs_l_mini, "b vs guds, mini", "pl");

    // Add aod ROCs
    TGraph *b_vs_bb_aod = new TGraph(nbins);
    b_vs_bb_aod->SetMarkerStyle(kOpenCircle);
    b_vs_bb_aod->SetMarkerColor(mykBlue);
    b_vs_bb_aod->SetMarkerSize(2);
    b_vs_bb_aod->SetLineColor(b_vs_bb_aod->GetMarkerColor());
    b_vs_bb_aod->SetLineStyle(kDashed);
    rocs->Add(b_vs_bb_aod);
    leg_roc->AddEntry(b_vs_bb_aod, "b vs bb, aod", "pl");

    TGraph *b_vs_c_aod = new TGraph(nbins);
    b_vs_c_aod->SetMarkerStyle(kOpenCircle);
    b_vs_c_aod->SetMarkerColor(mykGreen);
    b_vs_c_aod->SetMarkerSize(2);
    b_vs_c_aod->SetLineColor(b_vs_c_aod->GetMarkerColor());
    b_vs_c_aod->SetLineStyle(kDashed);
    rocs->Add(b_vs_c_aod);
    leg_roc->AddEntry(b_vs_c_aod, "b vs c, aod", "pl");

    TGraph *b_vs_l_aod = new TGraph(nbins);
    b_vs_l_aod->SetMarkerStyle(kOpenCircle);
    b_vs_l_aod->SetMarkerColor(mykRed);
    b_vs_l_aod->SetMarkerSize(2);
    b_vs_l_aod->SetLineColor(b_vs_l_aod->GetMarkerColor());
    b_vs_l_aod->SetLineStyle(kDashed);
    rocs->Add(b_vs_l_aod);
    leg_roc->AddEntry(b_vs_l_aod, "b vs guds, aod", "pl");

    // calculate ROCs
    for (int ibin = 1; ibin <= nbins; ibin++) {
        double eff_mini = hSingleB_discr_mini->Integral(ibin, nbins, iymin, iymax) / hSingleB_discr_mini->Integral(0, -1, iymin, iymax);
        double bbmis_mini = hBB_discr_mini->Integral(ibin, nbins, iymin, iymax) / hBB_discr_mini->Integral(0, -1, iymin, iymax);
        double cmis_mini = hC_discr_mini->Integral(ibin, nbins, iymin, iymax) / hC_discr_mini->Integral(0, -1, iymin, iymax);
        double lmis_mini = hL_discr_mini->Integral(ibin, nbins, iymin, iymax) / hL_discr_mini->Integral(0, -1, iymin, iymax);

        b_vs_bb_mini->SetPoint(ibin - 1, eff_mini, bbmis_mini);
        b_vs_c_mini->SetPoint(ibin - 1, eff_mini, cmis_mini);
        b_vs_l_mini->SetPoint(ibin - 1, eff_mini, lmis_mini);

        double eff_aod = hSingleB_discr_aod->Integral(ibin, nbins, iymin, iymax) / hSingleB_discr_aod->Integral(0, -1, iymin, iymax);
        double bbmis_aod = hBB_discr_aod->Integral(ibin, nbins, iymin, iymax) / hBB_discr_aod->Integral(0, -1, iymin, iymax);
        double cmis_aod = hC_discr_aod->Integral(ibin, nbins, iymin, iymax) / hC_discr_aod->Integral(0, -1, iymin, iymax);
        double lmis_aod = hL_discr_aod->Integral(ibin, nbins, iymin, iymax) / hL_discr_aod->Integral(0, -1, iymin, iymax);

        b_vs_bb_aod->SetPoint(ibin - 1, eff_aod, bbmis_aod);
        b_vs_c_aod->SetPoint(ibin - 1, eff_aod, cmis_aod);
        b_vs_l_aod->SetPoint(ibin - 1, eff_aod, lmis_aod);

        if (ibin==14) {
            std::cout << "b eff = " << eff_mini
                      << "\nbb mis = " << bbmis_mini
                      << "\nc mis = " << cmis_mini
                      << "\nl mis = " << lmis_mini
                      << std::endl;
        }
    }

    // Make decorations
    TPaveText *info_top_left = new TPaveText(0.4, 2, 0.6, 3.5, "nb ndc");
    info_top_left->SetTextSize(text_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Internal} Simulation");

    TPaveText *info_top_right = new TPaveText(0.75, 2, 1.05, 3.5, "nb ndc");
    info_top_right->SetTextSize(text_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");

    TLine *line = new TLine(0.7, 1.99e-5, 0.7, 1.99);
    line->SetLineColor(kGray);
    line->SetLineStyle(kDashed);

    TCanvas *c_roc = new TCanvas("c_roc", "", 1200, 1000);
    c_roc->SetLogy();
    rocs->Draw("pla");
    leg_roc->Draw();
    info_top_left->Draw();
    info_top_right->Draw();
    line->Draw();
    c_roc->Draw();

    c_roc->Print("./plots/b_vs_all_roc.png");
    


}