#include "../myPalette.h"

void draw_b_vs_bb()
{
    // ---- Settings
    float ptMin = 50.;
    float ptMax = 80.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos
    TFile *faod = new TFile("./histos/aod_discr.root");
    TH2F *hSingleB_discr_aod = (TH2F *) faod->Get("hSingleB_discr");
    hSingleB_discr_aod->SetName("hSingleB_discr_aod");
    TH2F *hBB_discr_aod = (TH2F *) faod->Get("hBB_discr");
    hBB_discr_aod->SetName("hBB_discr_aod");
    
    TFile *fmini = new TFile("./histos/mini_discr.root");
    TH2F *hSingleB_discr_mini = (TH2F *) fmini->Get("hSingleB_discr");
    hSingleB_discr_mini->SetName("hSingleB_discr_mini");
    TH2F *hBB_discr_mini = (TH2F *) fmini->Get("hBB_discr");
    hBB_discr_mini->SetName("hBB_discr_mini");

    // ---- Calculate roc
    Int_t iymin = hSingleB_discr_aod->GetYaxis()->FindBin(ptMin);
    Int_t iymax = hSingleB_discr_aod->GetYaxis()->FindBin(ptMax) - 1;

    int nbins = hSingleB_discr_aod->GetNbinsX();

    TLegend *leg_roc = new TLegend(0.2, 0.65, 0.5, 0.85);
    leg_roc->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin, ptMax));
    leg_roc->SetFillStyle(0);
    leg_roc->SetBorderSize(0);
    leg_roc->SetMargin(0.15);

    TMultiGraph *rocs = new TMultiGraph();
    rocs->GetXaxis()->SetTitle("single b jet efficiency");
    rocs->GetYaxis()->SetTitle("bb mistag rate");


    TGraph *b_vs_bb_aod = new TGraph(nbins);
    b_vs_bb_aod->SetMarkerStyle(21);
    b_vs_bb_aod->SetMarkerColor(mykRed);
    b_vs_bb_aod->SetMarkerSize(2);
    b_vs_bb_aod->SetLineColor(b_vs_bb_aod->GetMarkerColor());
    b_vs_bb_aod->SetLineStyle(1);
    rocs->Add(b_vs_bb_aod);
    leg_roc->AddEntry(b_vs_bb_aod, "aod", "pl");


    TGraph *b_vs_bb_mini = new TGraph(nbins);
    b_vs_bb_mini->SetMarkerStyle(20);
    b_vs_bb_mini->SetMarkerColor(mykBlue);
    b_vs_bb_mini->SetMarkerSize(2);
    b_vs_bb_mini->SetLineColor(b_vs_bb_mini->GetMarkerColor());
    b_vs_bb_mini->SetLineStyle(1);
    rocs->Add(b_vs_bb_mini);
    leg_roc->AddEntry(b_vs_bb_mini, "mini", "pl");

    for (int ibin = 1; ibin <= nbins; ibin++) {
        double eff_aod = hSingleB_discr_aod->Integral(ibin, nbins, iymin, iymax) / hSingleB_discr_aod->Integral(0, -1, iymin, iymax);
        double bbmis_aod = hBB_discr_aod->Integral(ibin, nbins, iymin, iymax) / hBB_discr_aod->Integral(0, -1, iymin, iymax);

        b_vs_bb_aod->SetPoint(ibin - 1, eff_aod, bbmis_aod);

        double eff_mini = hSingleB_discr_mini->Integral(ibin, nbins, iymin, iymax) / hSingleB_discr_mini->Integral(0, -1, iymin, iymax);
        double bbmis_mini = hBB_discr_mini->Integral(ibin, nbins, iymin, iymax) / hBB_discr_mini->Integral(0, -1, iymin, iymax);

        b_vs_bb_mini->SetPoint(ibin - 1, eff_mini, bbmis_mini);
    }

    TCanvas *c_roc = new TCanvas("c_roc", "", 1200, 1000);
    c_roc->SetLogy();
    rocs->Draw("pla");
    leg_roc->Draw();
    c_roc->Draw();

    // Working point
    double wp = 0.7;
    Int_t ibin_wp = hSingleB_discr_aod->GetXaxis()->FindBin(wp);
    std::cout << "ibin for wp=0.7 : " << ibin_wp << std::endl;

    double nb_aod = hSingleB_discr_aod->Integral(0, nbins, iymin, iymax);
    double nbb_aod = hBB_discr_aod->Integral(0, nbins, iymin, iymax);

    double nb_mini = hSingleB_discr_mini->Integral(0, nbins, iymin, iymax);
    double nbb_mini = hBB_discr_mini->Integral(0, nbins, iymin, iymax);

    double aod_wp_eff = hSingleB_discr_aod->Integral(ibin_wp, nbins, iymin, iymax) / hSingleB_discr_aod->Integral(0, nbins, iymin, iymax);
    double mini_wp_eff = hSingleB_discr_mini->Integral(ibin_wp, nbins, iymin, iymax) / hSingleB_discr_mini->Integral(0, nbins, iymin, iymax);

    double aod_wp_mis = hBB_discr_aod->Integral(ibin_wp, nbins, iymin, iymax) / hBB_discr_aod->Integral(0, nbins, iymin, iymax);
    double mini_wp_mis = hBB_discr_mini->Integral(ibin_wp, nbins, iymin, iymax) / hBB_discr_mini->Integral(0, nbins, iymin, iymax);

    double aod_single_b_purity = hSingleB_discr_aod->Integral(ibin_wp, nbins, iymin, iymax) / (hSingleB_discr_aod->Integral(ibin_wp, nbins, iymin, iymax) + hBB_discr_aod->Integral(ibin_wp, nbins, iymin, iymax));
    double mini_single_b_purity = hSingleB_discr_mini->Integral(ibin_wp, nbins, iymin, iymax) / (hSingleB_discr_mini->Integral(ibin_wp, nbins, iymin, iymax) + hBB_discr_aod->Integral(ibin_wp, nbins, iymin, iymax));

    double aod_total_efficiency = (hSingleB_discr_aod->Integral(ibin_wp, nbins, iymin, iymax) + hBB_discr_aod->Integral(ibin_wp, nbins, iymin, iymax)) / (hSingleB_discr_aod->Integral(0, nbins, iymin, iymax) + hBB_discr_aod->Integral(0, nbins, iymin, iymax));
    double mini_total_efficiency = (hSingleB_discr_mini->Integral(ibin_wp, nbins, iymin, iymax) + hBB_discr_mini->Integral(ibin_wp, nbins, iymin, iymax)) / (hSingleB_discr_mini->Integral(0, nbins, iymin, iymax) + hBB_discr_mini->Integral(0, nbins, iymin, iymax));

    std::cout << "WP : 0.7" 
              << "\nAOD"
              << "\n\t * b / (b + bb) = " << nb_aod / (nb_aod + nbb_aod) 
              << "\n\t * (b + bb) efficiency = " << aod_total_efficiency
              << "\n\t * single b efficiency = " << aod_wp_eff
              << "\n\t * bb+ mistag rate = " << aod_wp_mis
              << "\n\t * single b purity = " << aod_single_b_purity
              << "\nminiAOD"
              << "\n\t * b / (b + bb) = " << nb_mini / (nb_mini + nbb_mini) 
              << "\n\t * (b + bb) efficiency = " << mini_total_efficiency
              << "\n\t * single b efficiency = " << mini_wp_eff
              << "\n\t * bb+ mistag rate = " << mini_wp_mis
              << "\n\t * single b purity = " << mini_single_b_purity
              << std::endl;

    // DEBUG
    std::cout << "single b integral, pass wp = " << hSingleB_discr_mini->Integral(ibin_wp, -1, iymin, iymax) << std::endl;
    std::cout << "double b integral, pass wp = " << hBB_discr_mini->Integral(ibin_wp, -1, iymin, iymax) << std::endl;
}