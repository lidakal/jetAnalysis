#include "draw_utils.h"

void draw_info()
{
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.12, 0.96, "#bf{CMS} #it{Simulation Preliminary}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.82, 0.96, "PYTHIA8 CP5 (pp 5.02 TeV)");
}

void draw_substructure(TString observable="rg")
{
    // Run with ROOT 6.30, take PDFs

    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetPalette(57);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";

    TString label = "noAggr_withPNET";
    TString sample = "bjet";
    TString fin_name = "./histos/" + sample + "_" + label + "_substructure.root"; 
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    // Draw lund plane at gen level without b tagging 
    TH2F *hSingleB_gen = (TH2F *) fin->Get("hSingleB_"+observable+"_gen");
    hSingleB_gen->Scale(1/hSingleB_gen->Integral());
    hSingleB_gen->GetXaxis()->SetTitle(xlabel);
    hSingleB_gen->GetXaxis()->SetTitleOffset(1.2);
    hSingleB_gen->GetYaxis()->SetTitle("ln(k_{T}/GeV)");
    hSingleB_gen->GetYaxis()->SetTitleOffset(1.8);
    hSingleB_gen->GetZaxis()->SetTitle("Normalized per jet");
    hSingleB_gen->GetZaxis()->SetTitleOffset(1.95);
    hSingleB_gen->GetZaxis()->SetRangeUser(0., 0.03);
    hSingleB_gen->GetYaxis()->SetRangeUser(-2., 3.);

    TCanvas *c_SingleB_gen = new TCanvas("c_SingleB_gen", "Truth level lund plane no tag", 1000, 700);
    c_SingleB_gen->SetRightMargin(0.18);
    c_SingleB_gen->SetLeftMargin(0.12);
    c_SingleB_gen->SetTopMargin(0.07);
    c_SingleB_gen->SetBottomMargin(0.12);
    hSingleB_gen->Draw("colz");
    // c_SingleB_gen->SetLogz();

    draw_info();

    // for pdf
    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    jet_info->SetTextAlign(32);
    jet_info->DrawLatex(0.795, 0.82, "Particle level anti-k_{T}, R=0.4 b jets");
    jet_info->DrawLatex(0.795, 0.77, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info->DrawLatex(0.795, 0.72, "Soft drop (charged particles)");
    jet_info->DrawLatex(0.795, 0.67, "z_{cut}=0.1, #beta=0");
    if (label.Contains("aggr")) {
        auto txt1 = jet_info->DrawLatex(0.795, 0.87, "Clustered b hadron decay daughters");
        // txt1->SetTextColor(kWhite);
    } else {
        auto txt1 = jet_info->DrawLatex(0.795, 0.87, "Unclustered b hadron decay daughters");
        // txt1->SetTextColor(kWhite);
    }

    // for png
    // TLatex *jet_info = new TLatex;
    // jet_info->SetNDC();
    // jet_info->SetTextSize(text_size);
    // jet_info->DrawLatex(0.37, 0.79, "Particle level anti-k_{T}, R=0.4 single b jets");
    // jet_info->DrawLatex(0.47, 0.74, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    // jet_info->DrawLatex(0.6, 0.69, "Soft drop (charged particles)");
    // jet_info->DrawLatex(0.65, 0.64, "z_{cut}=0.1, #beta=0");
    // if (label.Contains("aggr")) {
    //     auto txt1 = jet_info->DrawLatex(0.42, 0.84, "Clustered b hadron decay daughters");
    //     // txt1->SetTextColor(kWhite);
    // } else {
    //     auto txt1 = jet_info->DrawLatex(0.39, 0.84, "Unclustered b hadron decay daughters");
    //     // txt1->SetTextColor(kWhite);
    // }


    c_SingleB_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_vs_kt.pdf");
    c_SingleB_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_vs_kt.png");

    // Draw gen vs reco 
    // TH2F *hSingleBtag_reco_vs_gen = (TH2F *) fin->Get("hSingleBtag_"+observable+"_reco_vs_gen");
    // int nbins_x = hSingleBtag_reco_vs_gen->GetNbinsX();
    // int nbins_y = hSingleBtag_reco_vs_gen->GetNbinsY();
    // for (int iy=1; iy<=nbins_y; iy++) {
    //     double integral = hSingleBtag_reco_vs_gen->Integral(1, nbins_x, iy, iy);
    //     for (int ix=1; ix<=nbins_x; ix++) {
    //         double content = hSingleBtag_reco_vs_gen->GetBinContent(ix, iy);
    //         hSingleBtag_reco_vs_gen->SetBinContent(ix, iy, content/integral);
    //     }
    // }
    // hSingleBtag_reco_vs_gen->GetXaxis()->SetTitle("Detector level "+xlabel);
    // hSingleBtag_reco_vs_gen->GetYaxis()->SetTitle("Particle level "+xlabel);
    // hSingleBtag_reco_vs_gen->GetYaxis()->SetTitleOffset(1.2);
    // hSingleBtag_reco_vs_gen->GetZaxis()->SetTitle("normalized per particle level bin");
    // hSingleBtag_reco_vs_gen->GetZaxis()->SetTitleOffset(1.5);
    // hSingleBtag_reco_vs_gen->GetZaxis()->SetRangeUser(0.,1.);

    // TCanvas *c_SingleBtag_reco_vs_gen = new TCanvas("c_SingleBtag_reco_vs_gen", "x=reco,y=gen", 800, 600);
    // c_SingleBtag_reco_vs_gen->SetRightMargin(0.2);
    // hSingleBtag_reco_vs_gen->Draw("colz");

    // TLatex *jet_info2 = new TLatex;
    // jet_info2->SetNDC();
    // jet_info2->SetTextSize(text_size);
    // jet_info2->DrawLatex(0.63, 0.35, "Particle level");
    // jet_info2->DrawLatex(0.39, 0.3, "anti-k_{T}, R=0.4 b-tagged single b jets");
    // jet_info2->DrawLatex(0.45, 0.25, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    // jet_info2->DrawLatex(0.53, 0.2, "Charged particles only");
    // auto txt11 = jet_info2->DrawLatex(0.2, 0.8, "b hadron decay products");
    // txt11->SetTextColor(kRed);
    // TString bdecay2;
    // if (label.Contains("aggrTMVA")) bdecay2 = "aggregated for both axes";
    // else if (label.Contains("aggrGenNoReco")) bdecay2 = "aggregated for particle level";
    // else bdecay2 = "present for both axes";
    // auto txt22 = jet_info2->DrawLatex(0.2, 0.75, bdecay2);
    // txt22->SetTextColor(kRed);
    // jet_info2->Draw();

    // draw_info();

    // c_SingleBtag_reco_vs_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_reco_vs_gen.pdf");
    // c_SingleBtag_reco_vs_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_reco_vs_gen.png");
    
}