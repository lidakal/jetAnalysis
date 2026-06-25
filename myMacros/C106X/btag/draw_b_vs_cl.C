#include "../myPalette.h"

void draw_b_vs_cl()
{
    // ---- Settings
    float ptMin = 60.;
    float ptMax = 140.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos
    TFile *fmini = new TFile("./histos/mini_discr.root");
    TH2F *hB_discr_mini = (TH2F *) fmini->Get("hB_discr");
    hB_discr_mini->SetName("hB_discr_mini");
    TH2F *hSingleB_discr_mini = (TH2F *) fmini->Get("hSingleB_discr");
    hSingleB_discr_mini->SetName("hSingleB_discr_mini");
    TH2F *hBB_discr_mini = (TH2F *) fmini->Get("hBB_discr");
    hBB_discr_mini->SetName("hBB_discr_mini");
    TH2F *hL_discr_mini = (TH2F *) fmini->Get("hL_discr");
    hL_discr_mini->SetName("hL_discr_mini");
    TH2F *hC_discr_mini = (TH2F *) fmini->Get("hC_discr");
    hC_discr_mini->SetName("hC_discr_mini");

    TFile *faod = new TFile("./histos/aod_discr.root");
    TH2F *hB_discr_aod = (TH2F *) faod->Get("hB_discr");
    hB_discr_aod->SetName("hB_discr_aod");
    TH2F *hSingleB_discr_aod = (TH2F *) faod->Get("hSingleB_discr");
    hSingleB_discr_aod->SetName("hSingleB_discr_aod");
    TH2F *hBB_discr_aod = (TH2F *) faod->Get("hBB_discr");
    hBB_discr_aod->SetName("hBB_discr_aod");
    TH2F *hL_discr_aod = (TH2F *) faod->Get("hL_discr");
    hL_discr_aod->SetName("hL_discr_aod");
    TH2F *hC_discr_aod = (TH2F *) faod->Get("hC_discr");
    hC_discr_aod->SetName("hC_discr_aod");

    // ---- Calculate roc
    Int_t iymin = hB_discr_mini->GetYaxis()->FindBin(ptMin);
    Int_t iymax = hB_discr_mini->GetYaxis()->FindBin(ptMax) - 1;

    int nbins = hB_discr_mini->GetNbinsX();

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
    TGraph *b_vs_c_mini = new TGraph(nbins);
    b_vs_c_mini->SetMarkerStyle(kFullDiamond);
    b_vs_c_mini->SetMarkerColor(mykGreen);
    b_vs_c_mini->SetMarkerSize(3);
    b_vs_c_mini->SetLineColor(b_vs_c_mini->GetMarkerColor());
    b_vs_c_mini->SetLineStyle(1);
    rocs->Add(b_vs_c_mini);
    leg_roc->AddEntry(b_vs_c_mini, "b vs c, mini", "pl");

    TGraph *b_vs_l_mini = new TGraph(nbins);
    b_vs_l_mini->SetMarkerStyle(kFullStar);
    b_vs_l_mini->SetMarkerColor(mykRed);
    b_vs_l_mini->SetMarkerSize(3);
    b_vs_l_mini->SetLineColor(b_vs_l_mini->GetMarkerColor());
    b_vs_l_mini->SetLineStyle(1);
    rocs->Add(b_vs_l_mini);
    leg_roc->AddEntry(b_vs_l_mini, "b vs guds, mini", "pl");

    // Add aod ROCs
    TGraph *b_vs_c_aod = new TGraph(nbins);
    b_vs_c_aod->SetMarkerStyle(kOpenDiamond);
    b_vs_c_aod->SetMarkerColor(mykGreen);
    b_vs_c_aod->SetMarkerSize(3);
    b_vs_c_aod->SetLineColor(b_vs_c_aod->GetMarkerColor());
    b_vs_c_aod->SetLineStyle(kDashed);
    rocs->Add(b_vs_c_aod);
    leg_roc->AddEntry(b_vs_c_aod, "b vs c, aod", "pl");

    TGraph *b_vs_l_aod = new TGraph(nbins);
    b_vs_l_aod->SetMarkerStyle(kOpenStar);
    b_vs_l_aod->SetMarkerColor(mykRed);
    b_vs_l_aod->SetMarkerSize(3);
    b_vs_l_aod->SetLineColor(b_vs_l_aod->GetMarkerColor());
    b_vs_l_aod->SetLineStyle(kDashed);
    rocs->Add(b_vs_l_aod);
    leg_roc->AddEntry(b_vs_l_aod, "b vs guds, aod", "pl");

    // calculate ROCs
    for (int ibin = 1; ibin <= nbins; ibin++) {
        double ball_mini = hB_discr_mini->Integral(1, nbins, iymin, iymax);
        double call_mini = hC_discr_mini->Integral(1, nbins, iymin, iymax);
        double lall_mini = hL_discr_mini->Integral(1, nbins, iymin, iymax);
        double singleball_mini = hSingleB_discr_mini->Integral(1, nbins, iymin, iymax);
        double bball_mini = hBB_discr_mini->Integral(1, nbins, iymin, iymax);

        double bpass_mini = hB_discr_mini->Integral(ibin, nbins, iymin, iymax);
        double cpass_mini = hC_discr_mini->Integral(ibin, nbins, iymin, iymax);
        double lpass_mini = hL_discr_mini->Integral(ibin, nbins, iymin, iymax);
        double singlebpass_mini = hSingleB_discr_mini->Integral(ibin, nbins, iymin, iymax);
        double bbpass_mini = hBB_discr_mini->Integral(ibin, nbins, iymin, iymax);

        double beff_mini = bpass_mini / ball_mini;
        double cmis_mini = cpass_mini / call_mini;
        double lmis_mini = lpass_mini / lall_mini;
        double singlebeff_mini = singlebpass_mini / singleball_mini;
        double bbeff_mini = bbpass_mini / bball_mini;
        double bpur_mini = bpass_mini / (bpass_mini + cpass_mini + lpass_mini);
        double cpur_mini = cpass_mini / (bpass_mini + cpass_mini + lpass_mini);
        double lpur_mini = lpass_mini / (bpass_mini + cpass_mini + lpass_mini);
        double singlebpur_mini = singlebpass_mini / (bpass_mini + cpass_mini + lpass_mini);
        double bbpur_mini = bbpass_mini / (bpass_mini + cpass_mini + lpass_mini);
        

        b_vs_c_mini->SetPoint(ibin - 1, beff_mini, cmis_mini);
        b_vs_l_mini->SetPoint(ibin - 1, beff_mini, lmis_mini);

        double ball_aod = hB_discr_aod->Integral(1, nbins, iymin, iymax);
        double call_aod = hC_discr_aod->Integral(1, nbins, iymin, iymax);
        double lall_aod = hL_discr_aod->Integral(1, nbins, iymin, iymax);

        double bpass_aod = hB_discr_aod->Integral(ibin, nbins, iymin, iymax);
        double cpass_aod = hC_discr_aod->Integral(ibin, nbins, iymin, iymax);
        double lpass_aod = hL_discr_aod->Integral(ibin, nbins, iymin, iymax);

        double beff_aod = bpass_aod / ball_aod;
        double cmis_aod = cpass_aod / call_aod;
        double lmis_aod = lpass_aod / lall_aod;
        double bpur_aod = bpass_aod / (bpass_aod + cpass_aod + lpass_aod);
        double cpur_aod = cpass_aod / (bpass_aod + cpass_aod + lpass_aod);
        double lpur_aod = lpass_aod / (bpass_aod + cpass_aod + lpass_aod);


        b_vs_c_aod->SetPoint(ibin - 1, beff_aod, cmis_aod);
        b_vs_l_aod->SetPoint(ibin - 1, beff_aod, lmis_aod);

        if (ibin==19) {
            std::cout << "For b+lepb+bb > " << hB_discr_mini->GetXaxis()->GetBinLowEdge(ibin) << "\n" << std::endl;
            
            std::cout << "\nmini:" << std::endl;
            std::cout << "b eff = " << beff_mini
                      << "\nsingle b eff = " << singlebeff_mini
                      << "\nbb eff = " << bbeff_mini
                      << "\nc mis = " << cmis_mini
                      << "\nl mis = " << lmis_mini
                      << "\nb pur = " << bpur_mini
                      << "\nc pur = " << cpur_mini
                      << "\nl pur = " << lpur_mini
                      << "\nsingle b pur = " << singlebpur_mini
                      << "\nbb pur = " << bbpur_mini
                      << std::endl;

            std::cout << "\nAOD:" << std::endl;
            std::cout << "b eff = " << beff_aod
                      << "\nc mis = " << cmis_aod
                      << "\nl mis = " << lmis_aod
                      << "\nb pur = " << bpur_aod
                      << "\nc pur = " << cpur_aod
                      << "\nl pur = " << lpur_aod
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

    TLine *line = new TLine(0.9, 1.99e-5, 0.9, 1.99);
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

    c_roc->Print("./plots/b_vs_cl_roc.png");
    


}