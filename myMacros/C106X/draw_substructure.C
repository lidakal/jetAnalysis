#include "draw_utils.h"

void draw_info()
{
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(20);
    prelim->DrawLatex(0.175, 0.92, "#bf{CMS} #it{Simulation}");
    prelim->Draw();

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(20);
    lumi->DrawLatex(0.54, 0.92, "#bf{PYTHIA8 (pp 5.02 TeV)}");
    lumi->Draw();
}

void draw_substructure(TString observable="rg")
{
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetPalette(57);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";

    TString label = "aggrTMVA_newJP";
    TString sample = "bjet";
    TString fin_name = "./histos/" + sample + "_" + label + "_substructure.root"; 
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    // Draw lund plane at gen level without b tagging 
    TH2F *hSingleB_gen = (TH2F *) fin->Get("hSingleB_"+observable+"_gen");
    hSingleB_gen->Scale(1/hSingleB_gen->Integral());
    hSingleB_gen->GetXaxis()->SetTitle(xlabel);
    hSingleB_gen->GetYaxis()->SetTitle("ln(k_{T}/GeV)");
    hSingleB_gen->GetYaxis()->SetTitleOffset(1.2);
    hSingleB_gen->GetZaxis()->SetTitle("normalized per jet");
    hSingleB_gen->GetZaxis()->SetTitleOffset(1.8);
    hSingleB_gen->GetZaxis()->SetRangeUser(0., 0.03);

    TCanvas *c_SingleB_gen = new TCanvas("c_SingleB_gen", "Truth level lund plane no tag", 800, 600);
    c_SingleB_gen->SetRightMargin(0.2);
    hSingleB_gen->Draw("colz");
    // c_SingleB_gen->SetLogz();

    draw_info();

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    jet_info->DrawLatex(0.63, 0.83, "Particle level");
    jet_info->DrawLatex(0.49, 0.78, "anti-k_{T}, R=0.4 single b jets");
    jet_info->DrawLatex(0.45, 0.73, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info->DrawLatex(0.53, 0.68, "Charged particles only");
    auto txt1 = jet_info->DrawLatex(0.2, 0.24, "b hadron decay products");
    txt1->SetTextColor(kRed);
    TString bdecay;
    if (label.Contains("aggrTMVA")) bdecay = "aggregated";
    else bdecay = "present";
    auto txt2 = jet_info->DrawLatex(0.2, 0.19, bdecay);
    txt2->SetTextColor(kRed);
    jet_info->Draw();

    c_SingleB_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_vs_kt.pdf");
    c_SingleB_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_vs_kt.png");

    // Draw gen vs reco 
    TH2F *hSingleBtag_reco_vs_gen = (TH2F *) fin->Get("hSingleBtag_"+observable+"_reco_vs_gen");
    int nbins_x = hSingleBtag_reco_vs_gen->GetNbinsX();
    int nbins_y = hSingleBtag_reco_vs_gen->GetNbinsY();
    for (int iy=1; iy<=nbins_y; iy++) {
        double integral = hSingleBtag_reco_vs_gen->Integral(1, nbins_x, iy, iy);
        for (int ix=1; ix<=nbins_x; ix++) {
            double content = hSingleBtag_reco_vs_gen->GetBinContent(ix, iy);
            hSingleBtag_reco_vs_gen->SetBinContent(ix, iy, content/integral);
        }
    }
    hSingleBtag_reco_vs_gen->GetXaxis()->SetTitle("Detector level "+xlabel);
    hSingleBtag_reco_vs_gen->GetYaxis()->SetTitle("Particle level "+xlabel);
    hSingleBtag_reco_vs_gen->GetYaxis()->SetTitleOffset(1.2);
    hSingleBtag_reco_vs_gen->GetZaxis()->SetTitle("normalized per particle level bin");
    hSingleBtag_reco_vs_gen->GetZaxis()->SetTitleOffset(1.5);
    hSingleBtag_reco_vs_gen->GetZaxis()->SetRangeUser(0.,1.);

    TCanvas *c_SingleBtag_reco_vs_gen = new TCanvas("c_SingleBtag_reco_vs_gen", "x=reco,y=gen", 800, 600);
    c_SingleBtag_reco_vs_gen->SetRightMargin(0.2);
    hSingleBtag_reco_vs_gen->Draw("colz");

    TLatex *jet_info2 = new TLatex;
    jet_info2->SetNDC();
    jet_info2->SetTextSize(text_size);
    jet_info2->DrawLatex(0.63, 0.35, "Particle level");
    jet_info2->DrawLatex(0.39, 0.3, "anti-k_{T}, R=0.4 b-tagged single b jets");
    jet_info2->DrawLatex(0.45, 0.25, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info2->DrawLatex(0.53, 0.2, "Charged particles only");
    auto txt11 = jet_info2->DrawLatex(0.2, 0.8, "b hadron decay products");
    txt11->SetTextColor(kRed);
    TString bdecay2;
    if (label.Contains("aggrTMVA")) bdecay2 = "aggregated for both axes";
    else if (label.Contains("aggrGenNoReco")) bdecay2 = "aggregated for particle level";
    else bdecay2 = "present for both axes";
    auto txt22 = jet_info2->DrawLatex(0.2, 0.75, bdecay2);
    txt22->SetTextColor(kRed);
    jet_info2->Draw();

    draw_info();

    c_SingleBtag_reco_vs_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_reco_vs_gen.pdf");
    c_SingleBtag_reco_vs_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_reco_vs_gen.png");
    
}