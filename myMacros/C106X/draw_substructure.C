#include "draw_utils.h"
#include "cms_palette.h"

void draw_info()
{
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(32);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.12, 0.96, "#bf{CMS} #it{Simulation}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(32);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.82, 0.96, "PYTHIA8 CP5 (pp 5.02 TeV)");
}

void draw_substructure(TString observable="rg")
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

    TString label = "aggrGenNoReco_fixedMassBug";
    TString sample = "bjet";
    TString fin_name = "./histos/" + sample + "_" + label + "_substructure.root"; 
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    // Draw lund plane at gen level without b tagging 
    TH2F *hSingleB_gen = (TH2F *) fin->Get("hSingleB_"+observable+"_gen");
    hSingleB_gen->Scale(1/hSingleB_gen->Integral());
    hSingleB_gen->GetXaxis()->SetTitle(xlabel);
    hSingleB_gen->GetXaxis()->SetTitleOffset(1.1);
    hSingleB_gen->GetYaxis()->SetTitle("ln(k_{T}/GeV)");
    hSingleB_gen->GetYaxis()->SetTitleOffset(1.2);
    hSingleB_gen->GetZaxis()->SetTitle("Normalized per 2-prong jet");
    hSingleB_gen->GetZaxis()->SetTitleOffset(1.95);
    hSingleB_gen->GetZaxis()->SetRangeUser(0., 0.03);
    hSingleB_gen->GetYaxis()->SetRangeUser(-2., 3.);

    TCanvas *c_SingleB_gen = new TCanvas("c_SingleB_gen", "Truth level lund plane no tag", 1000, 700);
    c_SingleB_gen->SetRightMargin(0.16);
    c_SingleB_gen->SetLeftMargin(0.1);
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
    jet_info->DrawLatex(0.81, 0.82, "Particle level anti-k_{T}, R = 0.4 b jets");
    jet_info->DrawLatex(0.81, 0.77, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info->DrawLatex(0.81, 0.72, "Soft drop (charged particles)");
    jet_info->DrawLatex(0.81, 0.67, "z_{cut} = 0.1, #beta = 0");
    if (label.Contains("aggr")) {
        auto txt1 = jet_info->DrawLatex(0.81, 0.87, "Clustered b hadron decay daughters");
        // txt1->SetTextColor(kWhite);
    } else {
        auto txt1 = jet_info->DrawLatex(0.81, 0.87, "Unclustered b hadron decay daughters");
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


    // c_SingleB_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_vs_kt.pdf");
    // c_SingleB_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_vs_kt.png");

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
    hSingleBtag_reco_vs_gen->GetXaxis()->SetTitle("Detector level^{} "+xlabel);
    hSingleBtag_reco_vs_gen->GetXaxis()->SetTitleOffset(1.2);
    hSingleBtag_reco_vs_gen->GetYaxis()->SetTitle("Particle level^{} "+xlabel);
    hSingleBtag_reco_vs_gen->GetYaxis()->SetTitleOffset(1.4);
    hSingleBtag_reco_vs_gen->GetZaxis()->SetTitle("Migration probability");
    hSingleBtag_reco_vs_gen->GetZaxis()->SetTitleOffset(1.25);
    hSingleBtag_reco_vs_gen->GetZaxis()->SetRangeUser(0.,1.);
    hSingleBtag_reco_vs_gen->GetXaxis()->SetRangeUser(0.,3.);
    hSingleBtag_reco_vs_gen->GetYaxis()->SetRangeUser(0.,3.);

    TCanvas *c_SingleBtag_reco_vs_gen = new TCanvas("c_SingleBtag_reco_vs_gen", "x=reco,y=gen", 1000, 700);
    c_SingleBtag_reco_vs_gen->SetRightMargin(0.18);
    c_SingleBtag_reco_vs_gen->SetLeftMargin(0.12);
    c_SingleBtag_reco_vs_gen->SetTopMargin(0.07);
    c_SingleBtag_reco_vs_gen->SetBottomMargin(0.12);
    hSingleBtag_reco_vs_gen->Draw("colz");

    // TLatex *jet_info2 = new TLatex;
    // jet_info2->SetNDC();
    // jet_info2->SetTextSize(text_size);
    // jet_info2->SetTextColor(kBlack);
    // jet_info2->SetTextAlign(12);
    // // jet_info2->SetTextFont(63);
    // jet_info2->DrawLatex(0.15, 0.85, "anti-k_{T}, R = 0.4 b-tagged b jets");
    // jet_info2->DrawLatex(0.15, 0.79, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    // jet_info2->DrawLatex(0.15, 0.73, "Soft drop (charged particles)");
    // jet_info2->DrawLatex(0.15, 0.67, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");

    TLatex *jet_info4 = new TLatex;
    jet_info4->SetNDC();
    jet_info4->SetTextSize(text_size);
    jet_info4->SetTextColor(kWhite);
    jet_info4->SetTextAlign(12);
    jet_info4->SetTextFont(43);
    jet_info4->DrawLatex(0.15, 0.85, "anti-k_{T}, R = 0.4 b-tagged b jets");
    jet_info4->DrawLatex(0.15, 0.79, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info4->DrawLatex(0.15, 0.73, "Soft drop (charged particles)");
    jet_info4->DrawLatex(0.15, 0.67, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");

    TLatex *jet_info3 = new TLatex;
    jet_info3->SetNDC();
    jet_info3->SetTextSize(text_size);
    jet_info3->SetTextColor(kWhite);
    jet_info3->SetTextAlign(32);
    jet_info3->DrawLatex(0.78, 0.25, "b hadron decay daughters");
    TString bdecay2;
    if (label.Contains("aggrTMVA")) bdecay2 = "clustered on both levels";
    else if (label.Contains("aggrGenNoReco")) bdecay2 = "clustered only on particle level";
    else bdecay2 = "not clustered on both levels";
    jet_info3->DrawLatex(0.78, 0.2, bdecay2);

    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(26);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.12, 0.96, "#bf{CMS} #it{Simulation Supplementary}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(26);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.82, 0.96, "PYTHIA8 CP5 (pp 5.02 TeV)");

    c_SingleBtag_reco_vs_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_reco_vs_gen.pdf");
    c_SingleBtag_reco_vs_gen->Print("plots_an/"+sample+"_"+label+"_"+observable+"_reco_vs_gen.png");
    c_SingleBtag_reco_vs_gen->Print("plots_thesis/"+sample+"_"+label+"_"+observable+"_reco_vs_gen.pdf");
    
}