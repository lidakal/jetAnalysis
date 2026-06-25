#include "../draw_utils.h"

void draw_eff(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{  
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;

    TString sample = "pythia_PF40";
    TString label = "aggrTMVA";

    bool sfDown = false;
    bool sfUp = false;
    suffix += "_withSF";
    if (sfUp) suffix += "Up";
    if (sfDown) suffix += "Down";


    // Load histograms
    TString fin_name = "histos/"+sample+"_"+label+"_inclusive_histograms"+suffix+".root";
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TH2D *h_inclusive = (TH2D *) fin->Get("h_"+observable+"pt");
    TH2D *h_btag = (TH2D *) fin->Get("h_"+observable+"pt_tagged");
    TH2D *h_btag_withSF = (TH2D *) fin->Get("h_"+observable+"pt_tagged_withSF");

    TH2D *h_inclusive_gen = (TH2D *) fin->Get("h_"+observable+"pt_gen");
    TH2D *h_btag_gen = (TH2D *) fin->Get("h_"+observable+"pt_gen_tagged");
    TH2D *h_btag_withSF_gen = (TH2D *) fin->Get("h_"+observable+"pt_gen_tagged_withSF");

    // Create efficiency histograms 
    TH2D *h_eff = (TH2D *) h_btag->Clone("h_eff");
    h_eff->Divide(h_btag, h_inclusive, 1., 1., "b");

    TH2D *h_eff_withSF = (TH2D *) h_btag_withSF->Clone("h_eff_withSF");
    h_eff_withSF->Divide(h_btag_withSF, h_inclusive, 1., 1., "b");

    TH2D *h_eff_gen = (TH2D *) h_btag_gen->Clone("h_eff_gen");
    h_eff_gen->Divide(h_btag_gen, h_inclusive_gen, 1., 1., "b");

    TH2D *h_eff_withSF_gen = (TH2D *) h_btag_withSF_gen->Clone("h_eff_withSF_gen");
    h_eff_withSF_gen->Divide(h_btag_withSF_gen, h_inclusive_gen, 1., 1., "b");

    // Draw the efficiencies
    int ibin_pt = 2;
    double pt_min = h_eff_withSF->GetYaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_eff_withSF->GetYaxis()->GetBinUpEdge(ibin_pt);
    TString header = Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max);

    TH1D *h_eff_1d = (TH1D *) h_eff->ProjectionX("h_eff_1d", ibin_pt, ibin_pt);
    h_eff_1d->SetMarkerStyle(kFullCircle);
    h_eff_1d->SetMarkerColor(kRed);
    h_eff_1d->SetLineColor(kRed);
    h_eff_1d->SetMinimum(0.);
    h_eff_1d->SetMaximum(0.7);
    if (observable=="zpt") h_eff_1d->SetMaximum(1.);
    h_eff_1d->GetXaxis()->SetTitle(xlabel);
    h_eff_1d->GetYaxis()->SetTitle("b jet efficiency");
    h_eff_1d->SetTitle("mc reco efficiency");

    TH1D *h_eff_withSF_1d = (TH1D *) h_eff_withSF->ProjectionX("h_eff_withSF_1d", ibin_pt, ibin_pt);
    h_eff_withSF_1d->SetMarkerStyle(kOpenCircle);
    h_eff_withSF_1d->SetMarkerColor(kRed);
    h_eff_withSF_1d->SetLineColor(kRed);
    h_eff_withSF_1d->SetTitle("mc reco efficiency with SF");

    TH1D *h_eff_gen_1d = (TH1D *) h_eff_gen->ProjectionX("h_eff_gen_1d", ibin_pt, ibin_pt);
    h_eff_gen_1d->SetMarkerStyle(kFullCross);
    h_eff_gen_1d->SetMarkerColor(kBlue);
    h_eff_gen_1d->SetLineColor(kBlue);
    h_eff_gen_1d->SetMinimum(0.);
    h_eff_gen_1d->GetXaxis()->SetTitle(xlabel);
    h_eff_gen_1d->GetYaxis()->SetTitle("b jet efficiency");
    h_eff_gen_1d->SetTitle("mc gen efficiency");

    TH1D *h_eff_withSF_gen_1d = (TH1D *) h_eff_withSF_gen->ProjectionX("h_eff_withSF_gen_1d", ibin_pt, ibin_pt);
    h_eff_withSF_gen_1d->SetMarkerStyle(kOpenCross);
    h_eff_withSF_gen_1d->SetMarkerColor(kBlue);
    h_eff_withSF_gen_1d->SetLineColor(kBlue);
    h_eff_withSF_gen_1d->SetTitle("mc gen efficiency with SF");

    
    TCanvas *c_eff = new TCanvas("c_eff", "", 800, 600);
    h_eff_1d->Draw("pe1");
    h_eff_withSF_1d->Draw("pe1 same");
    h_eff_gen_1d->Draw("pe1 same");
    h_eff_withSF_gen_1d->Draw("pe1 same");
    auto leg_eff = c_eff->BuildLegend();
    leg_eff->SetHeader(header);
    drawHeaderSimulation();
    c_eff->Draw();
    c_eff->Print("plots_an/btag_eff_gen_"+observable+".png");
}