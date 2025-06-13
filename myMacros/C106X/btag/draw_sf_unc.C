#include "../cms_palette.h"

void draw_sf_unc(TString observable="rg")
{
    gStyle->SetErrorX(0.5);
    gStyle->SetCanvasPreferGL(kTRUE);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R^{}/^{}R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch} #equiv p_{T}^{b,ch}/^{}p_{T}^{jet,ch}";

    // Setup plot
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");


    TString fin_nom_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_nom.root";
    TString fin_gspUp_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_gspUp50.root";
    TString fin_gspDown_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_gspDown50.root";
    // TString fin_mcWithDataJPCalibration_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_mcWithDataJPCalibration.root";
    // TString fin_dataWithMCJPCalibration_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_dataWithMCJPCalibration.root";
    TString fin_muonJets_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_Mu5.root";

    for (auto name : {
        fin_nom_name,
        fin_gspUp_name,
        fin_gspDown_name,
        // fin_mcWithDataJPCalibration_name,
        // fin_dataWithMCJPCalibration_name,
        fin_muonJets_name
    }) {
        std::cout << "fin: " << name << std::endl;
    }

    TFile *fin_nom = new TFile(fin_nom_name);
    TH2D *h_sf_nom = (TH2D *) fin_nom->Get("h_eff_sf")->Clone("h_sf_nom");
    TFile *fin_gspUp = new TFile(fin_gspUp_name);
    TH2D *h_sf_gspUp = (TH2D *) fin_gspUp->Get("h_eff_sf")->Clone("h_sf_gspUp");
    TFile *fin_gspDown = new TFile(fin_gspDown_name);
    TH2D *h_sf_gspDown = (TH2D *) fin_gspDown->Get("h_eff_sf")->Clone("h_sf_gspDown");
    // TFile *fin_mcWithDataJPCalibration = new TFile(fin_mcWithDataJPCalibration_name);
    // TH2D *h_sf_mcWithDataJPCalibration = (TH2D *) fin_mcWithDataJPCalibration->Get("h_eff_sf")->Clone("h_sf_mcWithDataJPCalibration");
    // TFile *fin_dataWithMCJPCalibration = new TFile(fin_dataWithMCJPCalibration_name);
    // TH2D *h_sf_dataWithMCJPCalibration = (TH2D *) fin_dataWithMCJPCalibration->Get("h_eff_sf")->Clone("h_sf_dataWithMCJPCalibration");
    TFile *fin_muonJets = new TFile(fin_muonJets_name);
    TH2D *h_sf_muonJets = (TH2D *) fin_muonJets->Get("h_eff_sf")->Clone("h_sf_muonJets");

    int ibin_pt = 1; // merged pt bins

    // Make projections
    TH1D *h_sf_nom_1d = (TH1D *) h_sf_nom->ProjectionX("h_sf_nom_1d", ibin_pt, ibin_pt);
    h_sf_nom_1d->SetMarkerStyle(kFullCrossX);
    h_sf_nom_1d->SetMarkerColor(kBlack);
    h_sf_nom_1d->SetLineColor(kBlack);
    h_sf_nom_1d->SetMarkerSize(2);

    TH1D *h_sf_gspUp_1d = (TH1D *) h_sf_gspUp->ProjectionX("h_sf_gspUp_1d", ibin_pt, ibin_pt);
    h_sf_gspUp_1d->SetMarkerStyle(1);
    h_sf_gspUp_1d->SetMarkerColor(cmsBlue);
    h_sf_gspUp_1d->SetLineColorAlpha(cmsBlue,0.5);
    h_sf_gspUp_1d->SetLineWidth(4);

    TH1D *h_sf_gspDown_1d = (TH1D *) h_sf_gspDown->ProjectionX("h_sf_gspDown_1d", ibin_pt, ibin_pt);
    h_sf_gspDown_1d->SetMarkerStyle(1);
    h_sf_gspDown_1d->SetMarkerColor(cmsOrange);
    h_sf_gspDown_1d->SetLineColorAlpha(cmsOrange,0.5);
    h_sf_gspDown_1d->SetLineWidth(4);

    // TH1D *h_sf_mcWithDataJPCalibration_1d = (TH1D *) h_sf_mcWithDataJPCalibration->ProjectionX("h_sf_mcWithDataJPCalibration_1d", ibin_pt, ibin_pt);
    // h_sf_mcWithDataJPCalibration_1d->SetMarkerStyle(1);
    // h_sf_mcWithDataJPCalibration_1d->SetMarkerColor(kBlue);
    // h_sf_mcWithDataJPCalibration_1d->SetLineColor(kBlue);

    // TH1D *h_sf_dataWithMCJPCalibration_1d = (TH1D *) h_sf_dataWithMCJPCalibration->ProjectionX("h_sf_dataWithMCJPCalibration_1d", ibin_pt, ibin_pt);
    // h_sf_dataWithMCJPCalibration_1d->SetMarkerStyle(1);
    // h_sf_dataWithMCJPCalibration_1d->SetMarkerColor(kMagenta);
    // h_sf_dataWithMCJPCalibration_1d->SetLineColor(kMagenta);

    TH1D *h_sf_muonJets_1d = (TH1D *) h_sf_muonJets->ProjectionX("h_sf_muonJets_1d", ibin_pt, ibin_pt);
    h_sf_muonJets_1d->SetMarkerStyle(1);
    h_sf_muonJets_1d->SetMarkerColor(cmsViolet);
    h_sf_muonJets_1d->SetLineColorAlpha(cmsViolet,0.5);
    h_sf_muonJets_1d->SetLineWidth(4);

    TH1D *h_unc_1d = (TH1D *) h_sf_nom_1d->Clone("h_unc_1d");
    for (int ibin_x=0; ibin_x<=h_unc_1d->GetNbinsX(); ibin_x++) {
        double unc = std::abs(h_sf_nom_1d->GetBinContent(ibin_x) - 1.);
        h_unc_1d->SetBinError(ibin_x, unc);
    }
    h_unc_1d->SetFillColorAlpha(kBlack,0.1);
    h_unc_1d->SetFillStyle(1001);
    h_unc_1d->SetMarkerSize(0);
    h_unc_1d->SetLineWidth(0);
    h_unc_1d->SetLineColorAlpha(kBlack, 0.);
    h_unc_1d->SetTitle("|SF-1| variation");

    double ymin = 0.9;
    double ymax = 1.9;
    h_sf_nom_1d->GetYaxis()->SetRangeUser(ymin, ymax);
    h_sf_nom_1d->GetXaxis()->SetTitle(xlabel);
    h_sf_nom_1d->GetXaxis()->SetTitleOffset(0.5);
    h_sf_nom_1d->GetYaxis()->SetTitle("Fraction");
    h_sf_nom_1d->GetYaxis()->SetTitleOffset(1.2);
    h_sf_nom_1d->GetXaxis()->SetTitle(xlabel);
    h_sf_nom_1d->GetXaxis()->SetTitleOffset(1.2);
    h_sf_nom_1d->GetYaxis()->SetTitle("SF_{b}");

    TCanvas *c_sfs = new TCanvas("c_sfs", "", 700, 600);
    c_sfs->SetRightMargin(0.05);
    c_sfs->SetLeftMargin(0.12);
    h_sf_nom_1d->Draw("pe1");
    h_sf_gspUp_1d->Draw("hist same");
    h_sf_gspDown_1d->Draw("hist same");
    // h_sf_mcWithDataJPCalibration_1d->Draw("hist same");
    // h_sf_dataWithMCJPCalibration_1d->Draw("hist same");
    h_sf_muonJets_1d->Draw("hist same");
    // h_unc_1d->Draw("e2 same");
    h_sf_nom_1d->Draw("pe1 same");

    // c_sfs->BuildLegend();
    TLegend *leg_sfs = new TLegend(0.15, 0.45, 0.6, 0.6);
    if (observable=="zpt") leg_sfs = new TLegend(0.45, 0.5, 0.9, 0.65);
    leg_sfs->SetMargin(0.3);
    leg_sfs->SetFillStyle(0);
    leg_sfs->SetNColumns(2);
    leg_sfs->AddEntry(h_sf_nom_1d, "Nominal", "pe1l");
    leg_sfs->AddEntry(h_sf_muonJets_1d, "Muon jets", "l");
    leg_sfs->AddEntry(h_sf_gspUp_1d, "GSP +50%", "l");
    leg_sfs->AddEntry(h_sf_gspDown_1d, "GSP -50%", "l");
    // leg_sfs->AddEntry(h_sf_dataWithMCJPCalibration_1d, "JP swap data", "l");
    // leg_sfs->AddEntry(h_sf_mcWithDataJPCalibration_1d, "JP swap MC(b,c)", "l");
    // leg_sfs->AddEntry(h_unc_1d, "|SF-1| band", "f");
    leg_sfs->Draw();

    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.12, 0.93, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.95, 0.93, "pp 301^{} pb^{-1} (5.02 TeV)");

    // Jets text
    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size-4);
    jet_info->SetTextAlign(32);
    jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 jets");
    jet_info->DrawLatex(0.9, 0.77, "80 < p_{T}^{jet,reco} < 140 GeV/c, |#eta^{jet}| < 2");
    if (observable!="zpt") {
        jet_info->DrawLatex(0.9, 0.71, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.9, 0.65, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV/c");
    }

    if (observable!="zpt") {
        TPaveText *untagged_text = new TPaveText(0.09, 0.02, 0.37, 0.137, "NDC");
        untagged_text->SetFillColor(0);
        untagged_text->SetBorderSize(0);
        untagged_text->SetTextAlign(22);
        untagged_text->SetTextSize(text_size-4);
        untagged_text->AddText("SD-untagged");
        untagged_text->AddText("or k_{T} < 1 GeV/c");
        untagged_text->Draw();
    }

    c_sfs->Print("../plots_thesis/"+observable+"_sfs.pdf");
}