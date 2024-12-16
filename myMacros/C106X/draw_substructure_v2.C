#include "draw_utils.h"
#include "cms_palette.h"

void draw_info()
{
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(32);
    prelim->SetTextAlign(11);
    prelim->DrawLatex(0.08, 0.45, "#bf{CMS} #it{Simulation}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(32);
    lumi->SetTextAlign(11);
    lumi->DrawLatex(0.08, 0.15, "PYTHIA8 CP5 (pp 5.02 TeV)");
}

void draw_substructure_v2(TString observable="rg")
{
    // Run with ROOT 6.30, take PDFs

    Float_t text_size = 32.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetPalette(57);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R^{}/^{}R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";

    double marginL = 0.15;
    double marginR = 0.20;
    double marginB = 0.13;
    double marginT = 0.02;
    double marginM = 0.005;

    double zmin = 0.;
    double zmax = 0.03;
    double ymin = -2.;
    double ymax = 3.;
    double xoffset = 1.3;
    double yoffset = 1.5;
    double zoffset = 1.7;

    TString fin_noAggr_name = "./histos/bjet_noAggr_withPNET_substructure.root"; 
    std::cout << "fin_noAggr: " << fin_noAggr_name << std::endl;
    TFile *fin_noAggr = new TFile(fin_noAggr_name);
    TH2F *h_noAggr = (TH2F *) fin_noAggr->Get("hSingleB_"+observable+"_gen")->Clone("h_noAggr");
    h_noAggr->Scale(1/h_noAggr->Integral());
    h_noAggr->GetZaxis()->SetRangeUser(zmin, zmax);
    h_noAggr->GetYaxis()->SetRangeUser(ymin, ymax);
    h_noAggr->GetXaxis()->SetTitle(xlabel);
    h_noAggr->GetXaxis()->SetTitleOffset(xoffset);
    h_noAggr->GetYaxis()->SetTitle("ln(k_{T}/GeV)");
    h_noAggr->GetYaxis()->SetTitleOffset(yoffset);
    h_noAggr->GetXaxis()->ChangeLabel(9,-1,-1,-1,-1,-1," ");


    TString fin_aggr_name = "./histos/bjet_aggrTMVA_newJP_substructure.root"; 
    std::cout << "fin_aggr: " << fin_aggr_name << std::endl;
    TFile *fin_aggr = new TFile(fin_aggr_name);
    TH2F *h_aggr = (TH2F *) fin_aggr->Get("hSingleB_"+observable+"_gen")->Clone("h_aggr");
    h_aggr->Scale(1/h_aggr->Integral());
    h_aggr->GetZaxis()->SetRangeUser(zmin, zmax);
    h_aggr->GetYaxis()->SetRangeUser(ymin, ymax);
    h_aggr->GetXaxis()->SetTitle(xlabel);
    h_aggr->GetXaxis()->SetTitleOffset(xoffset);
    h_aggr->GetZaxis()->SetTitle("Normalized per 2-prong jet");
    h_aggr->GetZaxis()->CenterTitle(1);
    h_aggr->GetZaxis()->SetTitleOffset(zoffset);
    h_aggr->GetXaxis()->ChangeLabel(1,-1,-1,-1,-1,-1," ");

    TCanvas *c_LJP = new TCanvas("c_LJP", "Truth level lund plane no tag", 1500, 700);

    TPad *pad1 = new TPad("pad1", "", 0., 0.8, 1., 1.);
    pad1->SetTopMargin(marginM);
    pad1->SetBottomMargin(marginM);

    TPad *pad2 = new TPad("pad2", "", 0., 0., 0.485, 0.8);
    pad2->SetLeftMargin(marginL);
    pad2->SetRightMargin(marginM);
    pad2->SetTopMargin(marginT);
    pad2->SetBottomMargin(marginB);

    TPad *pad3 = new TPad("pad3", "", 0.485, 0., 1., 0.8);
    pad3->SetRightMargin(marginR);
    pad3->SetLeftMargin(marginM);
    pad3->SetTopMargin(marginT);
    pad3->SetBottomMargin(marginB);

    pad1->cd();
    draw_info();
    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    jet_info->SetTextAlign(31);
    jet_info->DrawLatex(0.9, 0.75, "Particle level");
    jet_info->DrawLatex(0.9, 0.45, "anti-k_{T} R = 0.4 b jets");
    jet_info->DrawLatex(0.9, 0.15, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    TLatex *sd_info = new TLatex;
    sd_info->SetNDC();
    sd_info->SetTextSize(text_size);
    sd_info->SetTextAlign(11);
    sd_info->DrawLatex(0.37, 0.45, "Soft drop (charged particles)");
    sd_info->DrawLatex(0.42, 0.15, "z_{cut} = 0.1, #beta = 0");

    pad2->cd();
    h_noAggr->Draw("col");
    TLatex *noAggr_info = new TLatex;
    noAggr_info->SetNDC();
    noAggr_info->SetTextSize(text_size);
    noAggr_info->SetTextAlign(32);
    noAggr_info->DrawLatex(0.95, 0.92, "Not clustered b hadron");
    noAggr_info->DrawLatex(0.95, 0.85, "charged decay daughters");

    pad3->cd();
    h_aggr->Draw("colz");
    TLatex *aggr_info = new TLatex;
    aggr_info->SetNDC();
    aggr_info->SetTextSize(text_size);
    aggr_info->SetTextAlign(32);
    aggr_info->DrawLatex(0.75, 0.92, "Clustered b hadron");
    aggr_info->DrawLatex(0.75, 0.85, "charged decay daughters");

    c_LJP->cd();
    pad1->Draw();
    pad2->Draw();
    pad3->Draw();

    c_LJP->Print("plots_an/combined_"+observable+"_vs_kt.pdf");
    c_LJP->Print("plots_an/combined_"+observable+"_vs_kt.png");    
}