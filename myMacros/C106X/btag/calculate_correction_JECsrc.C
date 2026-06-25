#include "../draw_utils.h"

void calculate_correction_JECsrc(TString observable="rg", TString jer_opt="nom", TString jec_opt="up_JECsrc")
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
    TFile *fin = new TFile("histos/"+sample+"_"+label+"_inclusive_histograms"+suffix+".root");
    TH2D *h_inclusive = (TH2D *) fin->Get("h_"+observable+"pt_gen");
    TH2D *h_btag = (TH2D *) fin->Get("h_"+observable+"pt_gen_tagged");

    // Create efficiency histograms 
    TH2D *h_eff = (TH2D *) h_btag->Clone("h_eff");
    h_eff->Divide(h_btag, h_inclusive, 1., 1., "b");

    // One per variation 
    TH2D *h0_eff_withSF,
         *h1_eff_withSF,
         *h2_eff_withSF,
         *h3_eff_withSF,
         *h4_eff_withSF,
         *h5_eff_withSF,
         *h6_eff_withSF,
         *h7_eff_withSF,
         *h8_eff_withSF,
         *h9_eff_withSF,
         *h10_eff_withSF,
         *h11_eff_withSF,
         *h12_eff_withSF,
         *h13_eff_withSF,
         *h14_eff_withSF,
         *h15_eff_withSF,
         *h16_eff_withSF,
         *h17_eff_withSF,
         *h18_eff_withSF,
         *h19_eff_withSF,
         *h20_eff_withSF,
         *h21_eff_withSF,
         *h22_eff_withSF,
         *h23_eff_withSF,
         *h24_eff_withSF,
         *h25_eff_withSF,
         *h26_eff_withSF,
         *h27_eff_withSF;

    std::vector<TH2D *> histos_eff_withSF = {
        h0_eff_withSF,
        h1_eff_withSF,
        h2_eff_withSF,
        h3_eff_withSF,
        h4_eff_withSF,
        h5_eff_withSF,
        h6_eff_withSF,
        h7_eff_withSF,
        h8_eff_withSF,
        h9_eff_withSF,
        h10_eff_withSF,
        h11_eff_withSF,
        h12_eff_withSF,
        h13_eff_withSF,
        h14_eff_withSF,
        h15_eff_withSF,
        h16_eff_withSF,
        h17_eff_withSF,
        h18_eff_withSF,
        h19_eff_withSF,
        h20_eff_withSF,
        h21_eff_withSF,
        h22_eff_withSF,
        h23_eff_withSF,
        h24_eff_withSF,
        h25_eff_withSF,
        h26_eff_withSF,
        h27_eff_withSF
    };

    std::vector<TH2D *> histos_btag_withSF;

    for (int i=0; i<28; i++) {
        TH2D *h_btag_withSF = (TH2D *) fin->Get(Form("h%d_%spt_gen_tagged_withSF", i, observable.Data()));
        histos_btag_withSF.push_back(h_btag_withSF);

        histos_eff_withSF[i] = (TH2D *) h_btag_withSF->Clone(Form("h%d_eff_withSF",i));
        histos_eff_withSF[i]->Divide(h_btag_withSF, h_inclusive, 1., 1., "b");
    }

    // Draw the efficiencies
    // int ibin_pt = 2;
    // double pt_min = h_eff_withSF->GetYaxis()->GetBinLowEdge(ibin_pt);
    // double pt_max = h_eff_withSF->GetYaxis()->GetBinUpEdge(ibin_pt);
    // TString header = Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max);

    // TH1D *h_eff_1d = (TH1D *) h_eff->ProjectionX("h_eff_1d", ibin_pt, ibin_pt);
    // h_eff_1d->SetMarkerColor(kGreen);
    // h_eff_1d->SetLineColor(kGreen);
    // h_eff_1d->SetMinimum(0.);
    // h_eff_1d->SetMaximum(0.7);
    // h_eff_1d->GetXaxis()->SetTitle(xlabel);
    // h_eff_1d->GetYaxis()->SetTitle("b jet efficiency");
    // h_eff_1d->SetTitle("mc gen efficiency");

    // TH1D *h_eff_withSF_1d = (TH1D *) h_eff_withSF->ProjectionX("h_eff_withSF_1d", ibin_pt, ibin_pt);
    // h_eff_withSF_1d->SetMarkerColor(kBlue);
    // h_eff_withSF_1d->SetLineColor(kBlue);
    // h_eff_withSF_1d->SetTitle("mc gen efficiency with SF");

    // TCanvas *c_eff = new TCanvas("c_eff", "", 800, 600);
    // h_eff_1d->Draw("pe1");
    // h_eff_withSF_1d->Draw("pe1 same");
    // auto leg_eff = c_eff->BuildLegend();
    // leg_eff->SetHeader(header);
    // drawHeaderSimulation();
    // c_eff->Draw();
    // c_eff->Print("plots_an/btag_eff_gen_"+observable+".png");

    // // Create and draw SF gen and compare to reco 
    // TFile *fin_sf_reco = new TFile("histos/aggrTMVA_inclusive_"+observable+"_sfs.root");
    // TH2D *h_sf_reco = (TH2D *) fin_sf_reco->Get("h_eff_sf")->Clone("h_sf_reco");
    // TH1D *h_sf_reco_1d = (TH1D *) h_sf_reco->ProjectionX("h_sf_reco_1d", ibin_pt, ibin_pt);
    // h_sf_reco_1d->SetTitle("SF reco");
    // h_sf_reco_1d->SetLineColor(kRed);
    // h_sf_reco_1d->SetMarkerColor(kRed);
    // h_sf_reco_1d->GetXaxis()->SetTitle(xlabel);
    // h_sf_reco_1d->GetYaxis()->SetTitle("b tagging efficiency scale factor");
    // h_sf_reco_1d->SetMinimum(0.85);
    // h_sf_reco_1d->SetMaximum(1.5);

    // TH2D *h_sf_gen = (TH2D *) h_eff_withSF->Clone("h_sf_gen");
    // h_sf_gen->Divide(h_eff);

    // TH1D *h_sf_gen_1d = (TH1D *) h_sf_gen->ProjectionX("h_sf_gen_1d", ibin_pt, ibin_pt);
    // h_sf_gen_1d->SetTitle("SF gen");

    // TCanvas *c_sf_gen = new TCanvas("c_sf_gen", "", 800, 600);
    // h_sf_reco_1d->Draw("pe1");
    // h_sf_gen_1d->Draw("pe1 same");
    // auto leg_sf = c_sf_gen->BuildLegend();
    // leg_sf->SetHeader(header);
    // drawHeader();
    // c_sf_gen->Draw();
    // c_sf_gen->Print("plots_an/sf_reco_vs_gen_"+observable+".png");


    // Create scale histogram : btag data * (inclusive/btag) = inclusive data
    // TH2D *h_correction = (TH2D *) h_inclusive->Clone("h_correction");
    // h_correction->Divide(h_correction, h_btag, 1., 1., "b");

    // Draw 
    // // int ibin_pt = 2;
    // TH1D *h_inclusive_1d = (TH1D *) h_inclusive->ProjectionX("h_inclusive_1d", ibin_pt, ibin_pt);
    // h_inclusive_1d->SetMarkerColor(kBlack);
    // h_inclusive_1d->SetLineColor(kBlack);
    // h_inclusive_1d->GetXaxis()->SetTitle(observable);
    // h_inclusive_1d->GetYaxis()->SetTitle("1/N dN/d"+observable);
    // h_inclusive_1d->SetTitle("not tagged");
    // h_inclusive_1d->Scale(1/h_inclusive_1d->Integral(), "width");

    // TH1D *h_btag_1d = (TH1D *) h_btag->ProjectionX("h_btag_1d", ibin_pt, ibin_pt);
    // h_btag_1d->SetMarkerColor(kRed);
    // h_btag_1d->SetLineColor(kRed);
    // h_btag_1d->GetXaxis()->SetTitle(observable);
    // h_btag_1d->GetYaxis()->SetTitle("1/N dN/d"+observable);
    // h_btag_1d->SetTitle("tagged");
    // h_btag_1d->Scale(1/h_btag_1d->Integral(), "width");

    // TH1D *h_btag_withSF_1d = (TH1D *) h_btag_withSF->ProjectionX("h_btag_withSF_1d", ibin_pt, ibin_pt);
    // h_btag_withSF_1d->SetMarkerColor(kBlue);
    // h_btag_withSF_1d->SetLineColor(kBlue);
    // h_btag_withSF_1d->GetXaxis()->SetTitle(observable);
    // h_btag_withSF_1d->GetYaxis()->SetTitle("1/N dN/d"+observable);
    // h_btag_withSF_1d->SetTitle("tagged with SF");
    // h_btag_withSF_1d->Scale(1/h_btag_withSF_1d->Integral(), "width");

    // TCanvas *c_bias = new TCanvas("c_bias","",800,600);
    // h_inclusive_1d->Draw("pe1");
    // h_btag_1d->Draw("pe1 same");
    // h_btag_withSF_1d->Draw("pe1 same");
    // c_bias->BuildLegend();

    // TH1D *h_correction_1d = (TH1D *) h_correction->ProjectionX("h_correction_1d", ibin_pt, ibin_pt);
    // h_correction_1d->GetXaxis()->SetTitle(observable);
    // h_correction_1d->GetYaxis()->SetTitle("1/efficiency");
    // h_correction_1d->SetMarkerColor(kBlue);
    // h_correction_1d->SetLineColor(kBlue);
    // h_correction_1d->SetTitle("1/efficiency");

    // TFile *fin_sf = new TFile("histos/aggrTMVA_inclusive_"+observable+"_sfs.root");
    // std::cout << "File in : " << "histos/aggrTMVA_inclusive_"+observable+"_sfs.root" << std::endl;
    // TH2D *h_eff_sf = (TH2D *) fin_sf->Get("h_eff_sf");
    // TH1D *h_eff_sf_1d = (TH1D *) h_eff_sf->ProjectionX("h_eff_sf_1d", ibin_pt, ibin_pt);
    // TH1D *h_correction_1d_sf = (TH1D *) h_correction_1d->Clone("h_correction_1d_sf");
    // h_correction_1d_sf->Divide(h_eff_sf_1d);
    // h_correction_1d_sf->GetXaxis()->SetTitle(observable);
    // h_correction_1d_sf->GetYaxis()->SetTitle("1/efficiency");
    // h_correction_1d_sf->SetMarkerColor(kGreen);
    // h_correction_1d_sf->SetLineColor(kGreen);
    // h_correction_1d_sf->SetTitle("1/(efficiency*SF)");

    // TCanvas *c_corr = new TCanvas("c_corr","", 800, 600);
    // h_correction_1d->Draw();
    // h_correction_1d_sf->Draw("same");

    // c_corr->BuildLegend();
    

    TString fout_name = "./histos/"+sample+"_"+label+"_XXT_" + observable + "_efficiency"+suffix+".root";
    std::cout << "fout : " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    h_inclusive->Write();
    h_btag->Write();
    h_eff->Write();

    for (int i=0; i<28; i++) {
        histos_btag_withSF[i]->Write();
        histos_eff_withSF[i]->Write();
    }

    fout->Close();
    delete fout;

    // gApplication -> Terminate(0);
}