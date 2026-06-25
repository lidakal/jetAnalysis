#include "../draw_utils.h"

void compare_eff_pythia_var(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{  
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;

    bool sfDown = false;
    bool sfUp = false;
    suffix += "_withSF";
    if (sfUp) suffix += "Up";
    if (sfDown) suffix += "Down";


    // Load histograms
    TFile *fin_pythia = new TFile("histos/pythia_PF40_aggrTMVA_inclusive_histograms"+suffix+".root");

    // TH2D *h_inclusive_pythia = (TH2D *) fin_pythia->Get("h_"+observable+"pt")->Clone("h_inclusive_pythia");
    // TH2D *h_btag_pythia = (TH2D *) fin_pythia->Get("h_"+observable+"pt_tagged")->Clone("h_btag_pythia");
    // TH2D *h_btag_withSF_pythia = (TH2D *) fin_pythia->Get("h_"+observable+"pt_tagged_withSF")->Clone("h_btag_withSF_pythia");

    TH2D *h_inclusive_gen_pythia = (TH2D *) fin_pythia->Get("h_"+observable+"pt_gen")->Clone("h_inclusive_gen_pythia");
    TH2D *h_btag_gen_pythia = (TH2D *) fin_pythia->Get("h_"+observable+"pt_gen_tagged")->Clone("h_btag_gen_pythia");
    // TH2D *h_btag_withSF_gen_pythia = (TH2D *) fin_pythia->Get("h_"+observable+"pt_gen_tagged_withSF")->Clone("h_btag_withSF_gen_pythia");

    TFile *fin_var = new TFile("histos/pythia_PF40_aggrTMVA_inclusive_histograms_jer_nom_jec_nom_pythia_var_withSF.root");

    // TH2D *h_inclusive_var = (TH2D *) fin_var->Get("h_FSRup_"+observable+"pt")->Clone("h_inclusive_var");
    // TH2D *h_btag_var = (TH2D *) fin_var->Get("h_FSRup_"+observable+"pt_tagged")->Clone("h_btag_var");
    // TH2D *h_btag_withSF_var = (TH2D *) fin_var->Get("h_FSRup_"+observable+"pt_tagged_withSF")->Clone("h_btag_withSF_var");

    TH2D *h_inclusive_gen_var = (TH2D *) fin_var->Get("h_ISRdown_"+observable+"pt_gen")->Clone("h_inclusive_gen_var");
    TH2D *h_btag_gen_var = (TH2D *) fin_var->Get("h_ISRdown_"+observable+"pt_gen_tagged")->Clone("h_btag_gen_var");
    // TH2D *h_btag_withSF_gen_var = (TH2D *) fin_var->Get("h_FSRup_"+observable+"pt_gen_tagged_withSF")->Clone("h_btag_withSF_gen_var");


    // Create efficiency histograms 
    // TH2D *h_eff_pythia = (TH2D *) h_btag_pythia->Clone("h_eff_pythia");
    // h_eff_pythia->Divide(h_btag_pythia, h_inclusive_pythia, 1., 1., "b");

    // TH2D *h_eff_withSF_pythia = (TH2D *) h_btag_withSF_pythia->Clone("h_eff_withSF_pythia");
    // h_eff_withSF_pythia->Divide(h_btag_withSF_pythia, h_inclusive_pythia, 1., 1., "b");

    TH2D *h_eff_gen_pythia = (TH2D *) h_btag_gen_pythia->Clone("h_eff_gen_pythia");
    h_eff_gen_pythia->Divide(h_btag_gen_pythia, h_inclusive_gen_pythia, 1., 1., "b");

    // TH2D *h_eff_withSF_gen_pythia = (TH2D *) h_btag_withSF_gen_pythia->Clone("h_eff_withSF_gen_pythia");
    // h_eff_withSF_gen_pythia->Divide(h_btag_withSF_gen_pythia, h_inclusive_gen_pythia, 1., 1., "b");

    // TH2D *h_eff_var = (TH2D *) h_btag_var->Clone("h_eff_var");
    // h_eff_var->Divide(h_btag_var, h_inclusive_var, 1., 1., "b");

    // TH2D *h_eff_withSF_var = (TH2D *) h_btag_withSF_var->Clone("h_eff_withSF_var");
    // h_eff_withSF_var->Divide(h_btag_withSF_var, h_inclusive_var, 1., 1., "b");

    TH2D *h_eff_gen_var = (TH2D *) h_btag_gen_var->Clone("h_eff_gen_var");
    h_eff_gen_var->Divide(h_btag_gen_var, h_inclusive_gen_var, 1., 1., "b");

    // TH2D *h_eff_withSF_gen_var = (TH2D *) h_btag_withSF_gen_var->Clone("h_eff_withSF_gen_var");
    // h_eff_withSF_gen_var->Divide(h_btag_withSF_gen_var, h_inclusive_gen_var, 1., 1., "b");

    // Draw the efficiencies
    int ibin_pt = 2;
    double pt_min = h_eff_gen_pythia->GetYaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_eff_gen_pythia->GetYaxis()->GetBinUpEdge(ibin_pt);
    TString header = Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max);

    // TH1D *h_eff_pythia_1d = (TH1D *) h_eff_pythia->ProjectionX("h_eff_pythia_1d", ibin_pt, ibin_pt);
    // h_eff_pythia_1d->SetMarkerStyle(kFullCircle);
    // h_eff_pythia_1d->SetMarkerColor(kRed);
    // h_eff_pythia_1d->SetLineColor(kRed);
    // h_eff_pythia_1d->SetMinimum(0.);
    // h_eff_pythia_1d->SetMaximum(0.7);
    // if (observable=="zpt") h_eff_pythia_1d->SetMaximum(1.);
    // h_eff_pythia_1d->GetXaxis()->SetTitle(xlabel);
    // h_eff_pythia_1d->GetYaxis()->SetTitle("b jet efficiency");
    // h_eff_pythia_1d->SetTitle("pythia reco efficiency");

    // TH1D *h_eff_withSF_pythia_1d = (TH1D *) h_eff_withSF_pythia->ProjectionX("h_eff_withSF_pythia_1d", ibin_pt, ibin_pt);
    // h_eff_withSF_pythia_1d->SetMarkerStyle(kOpenCircle);
    // h_eff_withSF_pythia_1d->SetMarkerColor(kRed);
    // h_eff_withSF_pythia_1d->SetLineColor(kRed);
    // h_eff_withSF_pythia_1d->SetTitle("pythia reco efficiency with SF");

    TH1D *h_eff_gen_pythia_1d = (TH1D *) h_eff_gen_pythia->ProjectionX("h_eff_gen_pythia_1d", ibin_pt, ibin_pt);
    h_eff_gen_pythia_1d->SetMarkerStyle(kFullCircle);
    h_eff_gen_pythia_1d->SetMarkerColor(kRed);
    h_eff_gen_pythia_1d->SetLineColor(kRed);
    h_eff_gen_pythia_1d->SetMinimum(0.);
    h_eff_gen_pythia_1d->SetMaximum(0.7);
    if (observable=="zpt") h_eff_gen_pythia_1d->SetMaximum(1.);
    h_eff_gen_pythia_1d->GetXaxis()->SetTitle(xlabel);
    h_eff_gen_pythia_1d->GetYaxis()->SetTitle("b jet efficiency");
    h_eff_gen_pythia_1d->SetTitle("pythia gen efficiency");

    // TH1D *h_eff_withSF_gen_pythia_1d = (TH1D *) h_eff_withSF_gen_pythia->ProjectionX("h_eff_withSF_gen_pythia_1d", ibin_pt, ibin_pt);
    // h_eff_withSF_gen_pythia_1d->SetMarkerStyle(kOpenCircle);
    // h_eff_withSF_gen_pythia_1d->SetMarkerColor(kRed);
    // h_eff_withSF_gen_pythia_1d->SetLineColor(kRed);
    // h_eff_withSF_gen_pythia_1d->SetTitle("pythia gen efficiency with SF");

    // TH1D *h_eff_var_1d = (TH1D *) h_eff_var->ProjectionX("h_eff_var_1d", ibin_pt, ibin_pt);
    // h_eff_var_1d->SetMarkerStyle(kFullCross);
    // h_eff_var_1d->SetMarkerColor(kBlue);
    // h_eff_var_1d->SetLineColor(kBlue);
    // h_eff_var_1d->SetMinimum(0.);
    // h_eff_var_1d->SetMaximum(0.7);
    // if (observable=="zpt") h_eff_var_1d->SetMaximum(1.);
    // h_eff_var_1d->GetXaxis()->SetTitle(xlabel);
    // h_eff_var_1d->GetYaxis()->SetTitle("b jet efficiency");
    // h_eff_var_1d->SetTitle("var reco efficiency");

    // TH1D *h_eff_withSF_var_1d = (TH1D *) h_eff_withSF_var->ProjectionX("h_eff_withSF_var_1d", ibin_pt, ibin_pt);
    // h_eff_withSF_var_1d->SetMarkerStyle(kOpenCross);
    // h_eff_withSF_var_1d->SetMarkerColor(kBlue);
    // h_eff_withSF_var_1d->SetLineColor(kBlue);
    // h_eff_withSF_var_1d->SetTitle("var reco efficiency with SF");

    TH1D *h_eff_gen_var_1d = (TH1D *) h_eff_gen_var->ProjectionX("h_eff_gen_var_1d", ibin_pt, ibin_pt);
    h_eff_gen_var_1d->SetMarkerStyle(kFullCross);
    h_eff_gen_var_1d->SetMarkerColor(kBlue);
    h_eff_gen_var_1d->SetLineColor(kBlue);
    h_eff_gen_var_1d->SetMinimum(0.);
    h_eff_gen_var_1d->GetXaxis()->SetTitle(xlabel);
    h_eff_gen_var_1d->GetYaxis()->SetTitle("b jet efficiency");
    h_eff_gen_var_1d->SetTitle("var gen efficiency");

    // TH1D *h_eff_withSF_gen_var_1d = (TH1D *) h_eff_withSF_gen_var->ProjectionX("h_eff_withSF_gen_var_1d", ibin_pt, ibin_pt);
    // h_eff_withSF_gen_var_1d->SetMarkerStyle(kOpenCross);
    // h_eff_withSF_gen_var_1d->SetMarkerColor(kBlue);
    // h_eff_withSF_gen_var_1d->SetLineColor(kBlue);
    // h_eff_withSF_gen_var_1d->SetTitle("var gen efficiency with SF");

    // TCanvas *c_eff = new TCanvas("c_eff", "", 800, 600);
    // h_eff_pythia_1d->Draw("pe1");
    // h_eff_withSF_pythia_1d->Draw("pe1 same");
    // h_eff_var_1d->Draw("pe1 same");
    // h_eff_withSF_var_1d->Draw("pe1 same");
    // auto leg_eff = c_eff->BuildLegend();
    // leg_eff->SetHeader(header);
    // drawHeaderSimulation();
    // c_eff->Draw();
    // c_eff->Print("plots_an/pythia_vs_var_btag_eff_"+observable+".png");

    TCanvas *c_eff_gen = new TCanvas("c_eff_gen", "", 800, 600);
    h_eff_gen_pythia_1d->Draw("pe1 same");
    // h_eff_withSF_gen_pythia_1d->Draw("pe1 same");
    h_eff_gen_var_1d->Draw("pe1 same");
    // h_eff_withSF_gen_var_1d->Draw("pe1 same");
    auto leg_eff_gen = c_eff_gen->BuildLegend();
    leg_eff_gen->SetHeader(header);
    drawHeaderSimulation();
    c_eff_gen->Draw();
    // c_eff_gen->Print("plots_an/pythia_vs_var_btag_eff_gen_"+observable+".png");
}