#include "../myPalette.h"
// #include "../draw_utils.h"
#include "../cms_palette.h"

void drawHeaderSimulation(void) {
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.15, 0.965, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.95, 0.965, "pp 301^{} pb^{-1} (5.02 TeV)");
}

void draw_rocs_particleNet_BvsAll()
{
    // ---- Settings
    float ptMin = 80.;
    float ptMax = 140.;

    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos
    TFile *f_dijet = new TFile("./histos/particleNet_dijet_discr.root");
    TH2F *hL_particleNet_b = (TH2F *) f_dijet->Get("hL_particleNet_BvsAll");
    TH2F *hC_particleNet_b = (TH2F *) f_dijet->Get("hC_particleNet_BvsAll");

    TH2F *hL_deepFlavour_b = (TH2F *) f_dijet->Get("hL_deepFlavour_BvsAll");
    TH2F *hC_deepFlavour_b = (TH2F *) f_dijet->Get("hC_deepFlavour_BvsAll");

    // TFile *f_bjet = new TFile("./histos/particleNet_discr.root");
    TH2F *hB_particleNet_b = (TH2F *) f_dijet->Get("hB_particleNet_BvsAll");
    TH2F *hBB_particleNet_b = (TH2F *) f_dijet->Get("hBB_particleNet_BvsAll");

    TH2F *hB_deepFlavour_b = (TH2F *) f_dijet->Get("hB_deepFlavour_BvsAll");
    TH2F *hBB_deepFlavour_b = (TH2F *) f_dijet->Get("hBB_deepFlavour_BvsAll");

    // ---- Calculate roc
    Int_t iymin = hB_particleNet_b->GetYaxis()->FindBin(ptMin);
    Int_t iymax = hB_particleNet_b->GetYaxis()->FindBin(ptMax) - 1;

    int nbins = hB_particleNet_b->GetNbinsX();

    TLegend *leg_roc = new TLegend(0.6, 0.2, 0.9, 0.4);
    // leg_roc->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin, ptMax));
    leg_roc->SetFillStyle(0);
    leg_roc->SetBorderSize(0);
    leg_roc->SetMargin(0.15);

    TMultiGraph *rocs = new TMultiGraph();
    rocs->GetXaxis()->SetTitle("Signal efficiency");
    rocs->GetYaxis()->SetTitle("Misidentification rate");

    TGraph *bbb_vs_c_particleNet = new TGraph(nbins);
    bbb_vs_c_particleNet->SetMarkerStyle(kFullSquare);
    bbb_vs_c_particleNet->SetMarkerColor(cmsRed);
    bbb_vs_c_particleNet->SetMarkerSize(1);
    bbb_vs_c_particleNet->SetLineColor(bbb_vs_c_particleNet->GetMarkerColor());
    bbb_vs_c_particleNet->SetLineStyle(kSolid);
    bbb_vs_c_particleNet->SetLineWidth(2);
    rocs->Add(bbb_vs_c_particleNet);
    leg_roc->AddEntry(bbb_vs_c_particleNet, "B vs C", "pl");

    TGraph *bbb_vs_l_particleNet = new TGraph(nbins);
    bbb_vs_l_particleNet->SetMarkerStyle(kFullTriangleUp);
    bbb_vs_l_particleNet->SetMarkerColor(cmsViolet);
    bbb_vs_l_particleNet->SetMarkerSize(1);
    bbb_vs_l_particleNet->SetLineColor(bbb_vs_l_particleNet->GetMarkerColor());
    bbb_vs_l_particleNet->SetLineStyle(kSolid);
    bbb_vs_l_particleNet->SetLineWidth(2);
    rocs->Add(bbb_vs_l_particleNet);
    leg_roc->AddEntry(bbb_vs_l_particleNet, "B vs Light (guds)", "pl");

    TGraph *bbb_vs_all_particleNet = new TGraph(nbins);
    bbb_vs_all_particleNet->SetMarkerStyle(kFullCircle);
    bbb_vs_all_particleNet->SetMarkerColor(cmsBlue);
    bbb_vs_all_particleNet->SetMarkerSize(1);
    bbb_vs_all_particleNet->SetLineColor(bbb_vs_all_particleNet->GetMarkerColor());
    bbb_vs_all_particleNet->SetLineStyle(kSolid);
    bbb_vs_all_particleNet->SetLineWidth(2);
    rocs->Add(bbb_vs_all_particleNet);
    leg_roc->AddEntry(bbb_vs_all_particleNet, "B vs All", "pl");

    TGraph *bbb_vs_c_deepFlavour = new TGraph(nbins);
    bbb_vs_c_deepFlavour->SetMarkerStyle(kOpenSquare);
    bbb_vs_c_deepFlavour->SetMarkerColor(cmsRed);
    bbb_vs_c_deepFlavour->SetMarkerSize(2);
    bbb_vs_c_deepFlavour->SetLineColor(bbb_vs_c_deepFlavour->GetMarkerColor());
    bbb_vs_c_deepFlavour->SetLineStyle(kDashed);
    // rocs->Add(bbb_vs_c_deepFlavour);
    // leg_roc->AddEntry(bbb_vs_c_deepFlavour, "b+bb vs c, deepFlavour", "pl");

    TGraph *bbb_vs_l_deepFlavour = new TGraph(nbins);
    bbb_vs_l_deepFlavour->SetMarkerStyle(kOpenTriangleUp);
    bbb_vs_l_deepFlavour->SetMarkerColor(cmsViolet);
    bbb_vs_l_deepFlavour->SetMarkerSize(2);
    bbb_vs_l_deepFlavour->SetLineColor(bbb_vs_l_deepFlavour->GetMarkerColor());
    bbb_vs_l_deepFlavour->SetLineStyle(kDashed);
    // rocs->Add(bbb_vs_l_deepFlavour);
    // leg_roc->AddEntry(bbb_vs_l_deepFlavour, "b+bb vs guds, deepFlavour", "pl");

    double b_total_particleNet = hB_particleNet_b->Integral(0, nbins+1, iymin, iymax);
    double bb_total_particleNet = hBB_particleNet_b->Integral(0, nbins+1, iymin, iymax);
    double c_total_particleNet = hC_particleNet_b->Integral(0, nbins+1, iymin, iymax);
    double l_total_particleNet = hL_particleNet_b->Integral(0, nbins+1, iymin, iymax);

    double bbb_total_particleNet = b_total_particleNet + bb_total_particleNet;

    double b_total_deepFlavour = hB_deepFlavour_b->Integral(0, nbins+1, iymin, iymax);
    double bb_total_deepFlavour = hBB_deepFlavour_b->Integral(0, nbins+1, iymin, iymax);
    double c_total_deepFlavour = hC_deepFlavour_b->Integral(0, nbins+1, iymin, iymax);
    double l_total_deepFlavour = hL_deepFlavour_b->Integral(0, nbins+1, iymin, iymax);

    double bbb_total_deepFlavour = b_total_deepFlavour + bb_total_deepFlavour;

    // meaningful when everything is taken from the dijet histograms
    // double total_particleNet = b_total_particleNet + bb_total_particleNet + c_total_particleNet + l_total_particleNet;
    // double total_deepFlavour = b_total_deepFlavour + bb_total_deepFlavour + c_total_deepFlavour + l_total_deepFlavour;

    for (int ibin = 1; ibin <= nbins; ibin++) {

        double b_pass_particleNet = hB_particleNet_b->Integral(ibin, nbins, iymin, iymax);
        double bb_pass_particleNet = hBB_particleNet_b->Integral(ibin, nbins, iymin, iymax);
        double c_pass_particleNet = hC_particleNet_b->Integral(ibin, nbins, iymin, iymax);
        double l_pass_particleNet = hL_particleNet_b->Integral(ibin, nbins, iymin, iymax);

        double bbb_pass_particleNet = b_pass_particleNet + bb_pass_particleNet;

        double eff_particleNet =  bbb_pass_particleNet / bbb_total_particleNet;
        double cmis_particleNet = c_pass_particleNet / c_total_particleNet;
        double lmis_particleNet = l_pass_particleNet / l_total_particleNet;

        double allmis_particleNet = (c_pass_particleNet + l_pass_particleNet) / (c_total_particleNet + l_total_particleNet);
        double b_eff_particleNet = b_pass_particleNet / b_total_particleNet;
        double bb_eff_particleNet = bb_pass_particleNet / bb_total_particleNet;

        bbb_vs_c_particleNet->SetPoint(ibin - 1, eff_particleNet, cmis_particleNet);
        bbb_vs_l_particleNet->SetPoint(ibin - 1, eff_particleNet, lmis_particleNet);
        bbb_vs_all_particleNet->SetPoint(ibin - 1, eff_particleNet, allmis_particleNet);

        // meaningful when everything is taken from the dijet histograms
        double all_pass_particleNet = bbb_pass_particleNet + c_pass_particleNet + l_pass_particleNet;
        double bperc_particleNet = b_pass_particleNet / all_pass_particleNet;
        double bbperc_particleNet = bb_pass_particleNet / all_pass_particleNet;
        double bbbperc_particleNet = bbb_pass_particleNet / all_pass_particleNet;
        double cperc_particleNet = c_pass_particleNet / all_pass_particleNet;
        double lperc_particleNet = l_pass_particleNet / all_pass_particleNet;

        if (ibin==25) {
            std::cout << "ParticleNet x bin = " << ibin 
                      << ", BvsAll > " << hB_particleNet_b->GetXaxis()->GetBinLowEdge(ibin)
                      << "\n\tBvsAll eff = " << eff_particleNet 
                      << "\n\tsingle b eff = " << b_eff_particleNet 
                      << "\n\tbb eff = " << bb_eff_particleNet 
                      << "\n\tcmis = " << cmis_particleNet
                      << "\n\tlmis = " << lmis_particleNet
                      << "\n\tallmis = " << allmis_particleNet
                      << "\n\n\tbperc = " << bperc_particleNet 
                      << "\n\tbbperc = " << bbperc_particleNet
                      << "\n\tbbbperc = " << bbbperc_particleNet
                      << "\n\tcperc = " << cperc_particleNet
                      << "\n\tlperc = " << lperc_particleNet
                      << std::endl;
        }

        // double b_pass_deepFlavour = hB_deepFlavour_b->Integral(ibin, nbins, iymin, iymax);
        // double bb_pass_deepFlavour = hBB_deepFlavour_b->Integral(ibin, nbins, iymin, iymax);
        // double c_pass_deepFlavour = hC_deepFlavour_b->Integral(ibin, nbins, iymin, iymax);
        // double l_pass_deepFlavour = hL_deepFlavour_b->Integral(ibin, nbins, iymin, iymax);

        // double bbb_pass_deepFlavour = b_pass_deepFlavour + bb_pass_deepFlavour;

        // double eff_deepFlavour =  bbb_pass_deepFlavour / bbb_total_deepFlavour;
        // double cmis_deepFlavour = c_pass_deepFlavour / c_total_deepFlavour;
        // double lmis_deepFlavour = l_pass_deepFlavour / l_total_deepFlavour;

        // bbb_vs_c_deepFlavour->SetPoint(ibin - 1, eff_deepFlavour, cmis_deepFlavour);
        // bbb_vs_l_deepFlavour->SetPoint(ibin - 1, eff_deepFlavour, lmis_deepFlavour);

        // // meaningful when everything is taken from the dijet histograms
        // double all_pass_deepFlavour = bbb_pass_deepFlavour + c_pass_deepFlavour + l_pass_deepFlavour;
        // double bperc_deepFlavour = b_pass_deepFlavour / all_pass_deepFlavour;
        // double bbperc_deepFlavour = bb_pass_deepFlavour / all_pass_deepFlavour;
        // double bbbperc_deepFlavour = bbb_pass_deepFlavour / all_pass_deepFlavour;
        // double cperc_deepFlavour = c_pass_deepFlavour / all_pass_deepFlavour;
        // double lperc_deepFlavour = l_pass_deepFlavour / all_pass_deepFlavour;

        // if (ibin==17) {
        //     std::cout << "DeelFlavour x bin = " << ibin 
        //               << "\n\tb+bb eff = " << eff_deepFlavour 
        //               << "\n\tcmis = " << cmis_deepFlavour
        //               << "\n\tlmis = " << lmis_deepFlavour
        //               << "\n\n\tbperc = " << bperc_deepFlavour 
        //               << "\n\tbbperc = " << bbperc_deepFlavour
        //               << "\n\tbbbperc = " << bbbperc_deepFlavour
        //               << "\n\tcperc = " << cperc_deepFlavour
        //               << "\n\tlperc = " << lperc_deepFlavour
        //               << std::endl;
        // }

    }

    rocs->GetXaxis()->SetRangeUser(0.45, 1.1);
    rocs->GetYaxis()->SetRangeUser(1e-6, 1e1);

    TCanvas *c_roc = new TCanvas("c_roc", "", 700, 600);
    c_roc->SetLeftMargin(0.15);
    c_roc->SetRightMargin(0.05);
    c_roc->SetTopMargin(0.07);
    c_roc->SetBottomMargin(0.15);
    c_roc->SetLogy();
    rocs->Draw("pla");
    leg_roc->Draw();
    drawHeaderSimulation();

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size-4);
    jet_info->SetTextAlign(12);
    jet_info->DrawLatex(0.2, 0.86, "anti-k_{T}, R=0.4 jets");
    jet_info->DrawLatex(0.2, 0.8, "|#eta^{jet}| < 2, 80 < p_{T}^{jet} < 140 (GeV/c)");
    jet_info->Draw();

    c_roc->Draw();
    // c_roc->Print("plots_an/particleNet_BvsAll_roc.png");
    c_roc->Print("../plots_thesis/particleNet_BvsAll_roc.pdf");
}