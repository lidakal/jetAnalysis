// #include "draw_utils.h"
#include "cms_palette.h"

void drawHeader(void) {
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

void draw_kinematics()
{
    // ---- Setup 
    gStyle->SetCanvasPreferGL(kTRUE);
    gStyle->SetErrorX(0.5);
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Load data
    TString fin_data_name = "histos/data_PF40to100_aggrTMVA_inclusive_kinematics_jer_nom_jec_nom.root";
    std::cout << "Reading data from: " << fin_data_name << std::endl;
    TFile *fin_data = new TFile(fin_data_name);
    TH1D *h_jtpt_data = (TH1D *) fin_data->Get("h_data_jtpt")->Clone("h_jtpt_data");
    TH1D *h_jtpt_data_tagged = (TH1D *) fin_data->Get("h_data_jtpt_tagged")->Clone("h_jtpt_data_tagged");
    TH1D *h_jteta_data = (TH1D *) fin_data->Get("h_data_jteta")->Clone("h_jteta_data");
    TH1D *h_jteta_data_tagged = (TH1D *) fin_data->Get("h_data_jteta_tagged")->Clone("h_jteta_data_tagged");
    TH1D *h_jtphi_data = (TH1D *) fin_data->Get("h_data_jtphi")->Clone("h_jtphi_data");
    TH1D *h_jtphi_data_tagged = (TH1D *) fin_data->Get("h_data_jtphi_tagged")->Clone("h_jtphi_data_tagged");

    // Load pythia
    TString fin_dijet_name = "histos/dijet_PF40_aggrTMVA_inclusive_kinematics_jer_nom_jec_nom.root";
    std::cout << "Reading pythia inclusive from: " << fin_dijet_name << std::endl;
    TFile *fin_dijet = new TFile(fin_dijet_name);
    TH1D *h_sig_jtpt_dijet = (TH1D *) fin_dijet->Get("h_sig_jtpt")->Clone("h_sig_jtpt_dijet");
    TH1D *h_sig_jteta_dijet = (TH1D *) fin_dijet->Get("h_sig_jteta")->Clone("h_sig_jteta_dijet");
    TH1D *h_sig_jtphi_dijet = (TH1D *) fin_dijet->Get("h_sig_jtphi")->Clone("h_sig_jtphi_dijet");
    TH1D *h_bkg_bb_jtpt_dijet = (TH1D *) fin_dijet->Get("h_bkg_bb_jtpt")->Clone("h_bkg_bb_jtpt_dijet");
    TH1D *h_bkg_bb_jteta_dijet = (TH1D *) fin_dijet->Get("h_bkg_bb_jteta")->Clone("h_bkg_bb_jteta_dijet");
    TH1D *h_bkg_bb_jtphi_dijet = (TH1D *) fin_dijet->Get("h_bkg_bb_jtphi")->Clone("h_bkg_bb_jtphi_dijet");
    TH1D *h_bkg_rest_jtpt_dijet = (TH1D *) fin_dijet->Get("h_bkg_rest_jtpt")->Clone("h_bkg_rest_jtpt_dijet");
    TH1D *h_bkg_rest_jteta_dijet = (TH1D *) fin_dijet->Get("h_bkg_rest_jteta")->Clone("h_bkg_rest_jteta_dijet");
    TH1D *h_bkg_rest_jtphi_dijet = (TH1D *) fin_dijet->Get("h_bkg_rest_jtphi")->Clone("h_bkg_rest_jtphi_dijet");

    TH1D *h_jtpt_dijet = (TH1D *) h_sig_jtpt_dijet->Clone("h_jtpt_dijet");
    h_jtpt_dijet->Add(h_bkg_bb_jtpt_dijet);
    h_jtpt_dijet->Add(h_bkg_rest_jtpt_dijet);

    TH1D *h_jteta_dijet = (TH1D *) h_sig_jteta_dijet->Clone("h_jteta_dijet");
    h_jteta_dijet->Add(h_bkg_bb_jteta_dijet);
    h_jteta_dijet->Add(h_bkg_rest_jteta_dijet);

    TH1D *h_jtphi_dijet = (TH1D *) h_sig_jtphi_dijet->Clone("h_jtphi_dijet");
    h_jtphi_dijet->Add(h_bkg_bb_jtphi_dijet);
    h_jtphi_dijet->Add(h_bkg_rest_jtphi_dijet);
    
    TString fin_bjet_name = "histos/bjet_PF40_aggrTMVA_inclusive_kinematics_jer_nom_jec_nom.root";
    std::cout << "Reading pythia tagged from: " << fin_bjet_name << std::endl;
    TFile *fin_bjet = new TFile(fin_bjet_name);
    TH1D *h_sig_jtpt_bjet_tagged = (TH1D *) fin_bjet->Get("h_sig_jtpt_tagged")->Clone("h_sig_jtpt_bjet_tagged");
    TH1D *h_sig_jteta_bjet_tagged = (TH1D *) fin_bjet->Get("h_sig_jteta_tagged")->Clone("h_sig_jteta_bjet_tagged");
    TH1D *h_sig_jtphi_bjet_tagged = (TH1D *) fin_bjet->Get("h_sig_jtphi_tagged")->Clone("h_sig_jtphi_bjet_tagged");
    TH1D *h_bkg_bb_jtpt_bjet_tagged = (TH1D *) fin_bjet->Get("h_bkg_bb_jtpt_tagged")->Clone("h_bkg_bb_jtpt_bjet_tagged");
    TH1D *h_bkg_bb_jteta_bjet_tagged = (TH1D *) fin_bjet->Get("h_bkg_bb_jteta_tagged")->Clone("h_bkg_bb_jteta_bjet_tagged");
    TH1D *h_bkg_bb_jtphi_bjet_tagged = (TH1D *) fin_bjet->Get("h_bkg_bb_jtphi_tagged")->Clone("h_bkg_bb_jtphi_bjet_tagged");
    TH1D *h_bkg_rest_jtpt_bjet_tagged = (TH1D *) fin_bjet->Get("h_bkg_rest_jtpt_tagged")->Clone("h_bkg_rest_jtpt_bjet_tagged");
    TH1D *h_bkg_rest_jteta_bjet_tagged = (TH1D *) fin_bjet->Get("h_bkg_rest_jteta_tagged")->Clone("h_bkg_rest_jteta_bjet_tagged");
    TH1D *h_bkg_rest_jtphi_bjet_tagged = (TH1D *) fin_bjet->Get("h_bkg_rest_jtphi_tagged")->Clone("h_bkg_rest_jtphi_bjet_tagged");

    TH1D *h_jtpt_bjet_tagged = (TH1D *) h_sig_jtpt_bjet_tagged->Clone("h_jtpt_dijet");
    h_jtpt_bjet_tagged->Add(h_bkg_bb_jtpt_bjet_tagged);
    h_jtpt_bjet_tagged->Add(h_bkg_rest_jtpt_bjet_tagged);

    TH1D *h_jteta_bjet_tagged = (TH1D *) h_sig_jteta_bjet_tagged->Clone("h_jteta_dijet");
    h_jteta_bjet_tagged->Add(h_bkg_bb_jteta_bjet_tagged);
    h_jteta_bjet_tagged->Add(h_bkg_rest_jteta_bjet_tagged);

    TH1D *h_jtphi_bjet_tagged = (TH1D *) h_sig_jtphi_bjet_tagged->Clone("h_jtphi_dijet");
    h_jtphi_bjet_tagged->Add(h_bkg_bb_jtphi_bjet_tagged);
    h_jtphi_bjet_tagged->Add(h_bkg_rest_jtphi_bjet_tagged);

    // Load herwig
    TString fin_herwig_dijet_name = "histos/herwig_dijet_PF40_aggrTMVA_inclusive_kinematics_jer_nom_jec_nom.root";
    std::cout << "Reading herwig inclusive from: " << fin_herwig_dijet_name << std::endl;
    TFile *fin_herwig_dijet = new TFile(fin_herwig_dijet_name);
    TH1D *h_sig_jtpt_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_sig_jtpt")->Clone("h_sig_jtpt_herwig_dijet");
    TH1D *h_sig_jteta_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_sig_jteta")->Clone("h_sig_jteta_herwig_dijet");
    TH1D *h_sig_jtphi_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_sig_jtphi")->Clone("h_sig_jtphi_herwig_dijet");
    TH1D *h_bkg_bb_jtpt_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_bkg_bb_jtpt")->Clone("h_bkg_bb_jtpt_herwig_dijet");
    TH1D *h_bkg_bb_jteta_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_bkg_bb_jteta")->Clone("h_bkg_bb_jteta_herwig_dijet");
    TH1D *h_bkg_bb_jtphi_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_bkg_bb_jtphi")->Clone("h_bkg_bb_jtphi_herwig_dijet");
    TH1D *h_bkg_rest_jtpt_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_bkg_rest_jtpt")->Clone("h_bkg_rest_jtpt_herwig_dijet");
    TH1D *h_bkg_rest_jteta_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_bkg_rest_jteta")->Clone("h_bkg_rest_jteta_herwig_dijet");
    TH1D *h_bkg_rest_jtphi_herwig_dijet = (TH1D *) fin_herwig_dijet->Get("h_bkg_rest_jtphi")->Clone("h_bkg_rest_jtphi_herwig_dijet");

    TH1D *h_jtpt_herwig_dijet = (TH1D *) h_sig_jtpt_herwig_dijet->Clone("h_jtpt_herwig_dijet");
    h_jtpt_herwig_dijet->Add(h_bkg_bb_jtpt_herwig_dijet);
    h_jtpt_herwig_dijet->Add(h_bkg_rest_jtpt_herwig_dijet);

    TH1D *h_jteta_herwig_dijet = (TH1D *) h_sig_jteta_herwig_dijet->Clone("h_jteta_herwig_dijet");
    h_jteta_herwig_dijet->Add(h_bkg_bb_jteta_herwig_dijet);
    h_jteta_herwig_dijet->Add(h_bkg_rest_jteta_herwig_dijet);

    TH1D *h_jtphi_herwig_dijet = (TH1D *) h_sig_jtphi_herwig_dijet->Clone("h_jtphi_herwig_dijet");
    h_jtphi_herwig_dijet->Add(h_bkg_bb_jtphi_herwig_dijet);
    h_jtphi_herwig_dijet->Add(h_bkg_rest_jtphi_herwig_dijet);
    
    TString fin_herwig_bjet_name = "histos/herwig_bjet_PF40_aggrTMVA_inclusive_kinematics_jer_nom_jec_nom.root";
    std::cout << "Reading herwig tagged from: " << fin_herwig_bjet_name << std::endl;
    TFile *fin_herwig_bjet = new TFile(fin_herwig_bjet_name);
    TH1D *h_sig_jtpt_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_sig_jtpt_tagged")->Clone("h_sig_jtpt_herwig_bjet_tagged");
    TH1D *h_sig_jteta_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_sig_jteta_tagged")->Clone("h_sig_jteta_herwig_bjet_tagged");
    TH1D *h_sig_jtphi_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_sig_jtphi_tagged")->Clone("h_sig_jtphi_herwig_bjet_tagged");
    TH1D *h_bkg_bb_jtpt_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_bkg_bb_jtpt_tagged")->Clone("h_bkg_bb_jtpt_herwig_bjet_tagged");
    TH1D *h_bkg_bb_jteta_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_bkg_bb_jteta_tagged")->Clone("h_bkg_bb_jteta_herwig_bjet_tagged");
    TH1D *h_bkg_bb_jtphi_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_bkg_bb_jtphi_tagged")->Clone("h_bkg_bb_jtphi_herwig_bjet_tagged");
    TH1D *h_bkg_rest_jtpt_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_bkg_rest_jtpt_tagged")->Clone("h_bkg_rest_jtpt_herwig_bjet_tagged");
    TH1D *h_bkg_rest_jteta_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_bkg_rest_jteta_tagged")->Clone("h_bkg_rest_jteta_herwig_bjet_tagged");
    TH1D *h_bkg_rest_jtphi_herwig_bjet_tagged = (TH1D *) fin_herwig_bjet->Get("h_bkg_rest_jtphi_tagged")->Clone("h_bkg_rest_jtphi_herwig_bjet_tagged");

    TH1D *h_jtpt_herwig_bjet_tagged = (TH1D *) h_sig_jtpt_herwig_bjet_tagged->Clone("h_jtpt_dijet");
    h_jtpt_herwig_bjet_tagged->Add(h_bkg_bb_jtpt_herwig_bjet_tagged);
    h_jtpt_herwig_bjet_tagged->Add(h_bkg_rest_jtpt_herwig_bjet_tagged);

    TH1D *h_jteta_herwig_bjet_tagged = (TH1D *) h_sig_jteta_herwig_bjet_tagged->Clone("h_jteta_dijet");
    h_jteta_herwig_bjet_tagged->Add(h_bkg_bb_jteta_herwig_bjet_tagged);
    h_jteta_herwig_bjet_tagged->Add(h_bkg_rest_jteta_herwig_bjet_tagged);

    TH1D *h_jtphi_herwig_bjet_tagged = (TH1D *) h_sig_jtphi_herwig_bjet_tagged->Clone("h_jtphi_dijet");
    h_jtphi_herwig_bjet_tagged->Add(h_bkg_bb_jtphi_herwig_bjet_tagged);
    h_jtphi_herwig_bjet_tagged->Add(h_bkg_rest_jtphi_herwig_bjet_tagged);

    // Normalize histograms 
    for (auto h : {
        h_jtpt_data, h_jtpt_data_tagged,
        h_jtpt_dijet, h_jtpt_bjet_tagged,
        h_jtpt_herwig_dijet, h_jtpt_herwig_bjet_tagged,
        h_jteta_data, h_jteta_data_tagged,
        h_jteta_dijet, h_jteta_bjet_tagged,
        h_jteta_herwig_dijet, h_jteta_herwig_bjet_tagged,
        h_jtphi_data, h_jtphi_data_tagged,
        h_jtphi_dijet, h_jtphi_bjet_tagged,
        h_jtphi_herwig_dijet, h_jtphi_herwig_bjet_tagged,
    }) {
        h->Scale(1/h->Integral(), "width");
    }

    // Make ratio 
    TH1D *h_jtpt_dijet_ratio = (TH1D *) h_jtpt_dijet->Clone("h_jtpt_dijet_ratio");
    h_jtpt_dijet_ratio->Divide(h_jtpt_data);

    TH1D *h_jtpt_bjet_tagged_ratio = (TH1D *) h_jtpt_bjet_tagged->Clone("h_jtpt_bjet_tagged_ratio");
    h_jtpt_bjet_tagged_ratio->Divide(h_jtpt_data_tagged);

    TH1D *h_jteta_dijet_ratio = (TH1D *) h_jteta_dijet->Clone("h_jteta_dijet_ratio");
    h_jteta_dijet_ratio->Divide(h_jteta_data);

    TH1D *h_jteta_bjet_tagged_ratio = (TH1D *) h_jteta_bjet_tagged->Clone("h_jteta_bjet_tagged_ratio");
    h_jteta_bjet_tagged_ratio->Divide(h_jteta_data_tagged);

    TH1D *h_jtphi_dijet_ratio = (TH1D *) h_jtphi_dijet->Clone("h_jtphi_dijet_ratio");
    h_jtphi_dijet_ratio->Divide(h_jtphi_data);

    TH1D *h_jtphi_bjet_tagged_ratio = (TH1D *) h_jtphi_bjet_tagged->Clone("h_jtphi_bjet_tagged_ratio");
    h_jtphi_bjet_tagged_ratio->Divide(h_jtphi_data_tagged);

    TH1D *h_jtpt_herwig_dijet_ratio = (TH1D *) h_jtpt_herwig_dijet->Clone("h_jtpt_herwig_dijet_ratio");
    h_jtpt_herwig_dijet_ratio->Divide(h_jtpt_data);

    TH1D *h_jtpt_herwig_bjet_tagged_ratio = (TH1D *) h_jtpt_herwig_bjet_tagged->Clone("h_jtpt_herwig_bjet_tagged_ratio");
    h_jtpt_herwig_bjet_tagged_ratio->Divide(h_jtpt_data_tagged);

    TH1D *h_jteta_herwig_dijet_ratio = (TH1D *) h_jteta_herwig_dijet->Clone("h_jteta_herwig_dijet_ratio");
    h_jteta_herwig_dijet_ratio->Divide(h_jteta_data);

    TH1D *h_jteta_herwig_bjet_tagged_ratio = (TH1D *) h_jteta_herwig_bjet_tagged->Clone("h_jteta_herwig_bjet_tagged_ratio");
    h_jteta_herwig_bjet_tagged_ratio->Divide(h_jteta_data_tagged);

    TH1D *h_jtphi_herwig_dijet_ratio = (TH1D *) h_jtphi_herwig_dijet->Clone("h_jtphi_herwig_dijet_ratio");
    h_jtphi_herwig_dijet_ratio->Divide(h_jtphi_data);

    TH1D *h_jtphi_herwig_bjet_tagged_ratio = (TH1D *) h_jtphi_herwig_bjet_tagged->Clone("h_jtphi_herwig_bjet_tagged_ratio");
    h_jtphi_herwig_bjet_tagged_ratio->Divide(h_jtphi_data_tagged);

    // Format histograms 
    for (auto h : {h_jtpt_data, h_jteta_data, h_jtphi_data}) {
        h->SetMarkerColor(cmsBlue);
        h->SetLineColor(cmsBlue);
        h->SetMarkerStyle(kFullCircle);
    }

    for (auto h : {h_jtpt_data_tagged, h_jteta_data_tagged, h_jtphi_data_tagged}) {
        h->SetMarkerColor(cmsRed);
        h->SetLineColor(cmsRed);
        h->SetMarkerStyle(kOpenSquare);
    }

    for (auto h : {h_jtpt_dijet, h_jteta_dijet, h_jtphi_dijet, 
                   h_jtpt_dijet_ratio, h_jteta_dijet_ratio, h_jtphi_dijet_ratio}) {
        h->SetMarkerColor(cmsBlue);
        h->SetLineColor(cmsBlue);
        h->SetMarkerStyle(1);
        h->SetLineStyle(5);
        h->SetLineWidth(3);
    }

    for (auto h : {h_jtpt_bjet_tagged, h_jteta_bjet_tagged, h_jtphi_bjet_tagged,
                   h_jtpt_bjet_tagged_ratio, h_jteta_bjet_tagged_ratio, h_jtphi_bjet_tagged_ratio}) {
        h->SetMarkerColor(cmsRed);
        h->SetLineColor(cmsRed);
        h->SetMarkerStyle(1);
        h->SetLineStyle(2);
        h->SetLineWidth(3);
    }

    for (auto h : {h_jtpt_herwig_dijet, h_jteta_herwig_dijet, h_jtphi_herwig_dijet,
                   h_jtpt_herwig_dijet_ratio, h_jteta_herwig_dijet_ratio, h_jtphi_herwig_dijet_ratio}) {
        h->SetMarkerColor(cmsBlue);
        h->SetLineColor(cmsBlue);
        h->SetMarkerStyle(1);
        h->SetLineStyle(7);
        h->SetLineWidth(3);
    }

    for (auto h : {h_jtpt_herwig_bjet_tagged, h_jteta_herwig_bjet_tagged, h_jtphi_herwig_bjet_tagged,
                   h_jtpt_herwig_bjet_tagged_ratio, h_jteta_herwig_bjet_tagged_ratio, h_jtphi_herwig_bjet_tagged_ratio}) {
        h->SetMarkerColor(cmsRed);
        h->SetLineColor(cmsRed);
        h->SetMarkerStyle(1);
        h->SetLineStyle(8);
        h->SetLineWidth(3);
    }

    // Make a legend 
    TLegend *leg_jtpt = new TLegend(0.45, 0.45, 0.85, 0.85);
    leg_jtpt->SetBorderSize(0);
    leg_jtpt->SetFillStyle(0);
    leg_jtpt->SetMargin(0.15);
    leg_jtpt->AddEntry(h_jtpt_data, "Data inclusive jets", "lpe1");
    leg_jtpt->AddEntry(h_jtpt_data_tagged, "Data b-tagged jets", "lpe1");
    leg_jtpt->AddEntry(h_jtpt_dijet, "PYTHIA8 CP5 inclusive jets", "l");
    leg_jtpt->AddEntry(h_jtpt_bjet_tagged, "PYTHIA8 CP5 b-tagged jets", "l");
    leg_jtpt->AddEntry(h_jtpt_herwig_dijet, "HERWIG CH3 inclusive jets", "l");
    leg_jtpt->AddEntry(h_jtpt_herwig_bjet_tagged, "HERWIG CH3 b-tagged jets", "l");

    TLegend *leg_jteta = new TLegend(0.32, 0.05, 0.77, 0.45);
    leg_jteta->SetBorderSize(0);
    leg_jteta->SetFillStyle(0);
    leg_jteta->SetMargin(0.15);
    leg_jteta->AddEntry(h_jtpt_data, "Data inclusive jets", "lpe1");
    leg_jteta->AddEntry(h_jtpt_data_tagged, "Data b-tagged jets", "lpe1");
    leg_jteta->AddEntry(h_jtpt_dijet, "PYTHIA8 CP5 inclusive jets", "l");
    leg_jteta->AddEntry(h_jtpt_bjet_tagged, "PYTHIA8 CP5 b-tagged jets", "l");
    leg_jteta->AddEntry(h_jtpt_herwig_dijet, "HERWIG CH3 inclusive jets", "l");
    leg_jteta->AddEntry(h_jtpt_herwig_bjet_tagged, "HERWIG CH3 b-tagged jets", "l");

    TLegend *leg_jtphi = new TLegend(0.35, 0.1, 0.8, 0.5);
    leg_jtphi->SetBorderSize(0);
    leg_jtphi->SetFillStyle(0);
    leg_jtphi->SetMargin(0.15);
    leg_jtphi->AddEntry(h_jtpt_data, "Data inclusive jets", "lpe1");
    leg_jtphi->AddEntry(h_jtpt_data_tagged, "Data b-tagged jets", "lpe1");
    leg_jtphi->AddEntry(h_jtpt_dijet, "PYTHIA8 CP5 inclusive jets", "l");
    leg_jtphi->AddEntry(h_jtpt_bjet_tagged, "PYTHIA8 CP5 b-tagged jets", "l");
    leg_jtphi->AddEntry(h_jtpt_herwig_dijet, "HERWIG CH3 inclusive jets", "l");
    leg_jtphi->AddEntry(h_jtpt_herwig_bjet_tagged, "HERWIG CH3 b-tagged jets", "l");

    // Line for ratio
    TLine *line_jtpt = new TLine(h_jtpt_data->GetXaxis()->GetBinLowEdge(1), 1, h_jtpt_data->GetXaxis()->GetBinUpEdge(h_jtpt_data->GetNbinsX()), 1);
    line_jtpt->SetLineColor(kBlack);
    line_jtpt->SetLineStyle(kSolid);

    TLine *line_jteta = new TLine(h_jteta_data->GetXaxis()->GetBinLowEdge(1), 1, h_jteta_data->GetXaxis()->GetBinUpEdge(h_jteta_data->GetNbinsX()), 1);
    line_jteta->SetLineColor(kBlack);
    line_jteta->SetLineStyle(kSolid);

    TLine *line_jtphi = new TLine(h_jtphi_data->GetXaxis()->GetBinLowEdge(1), 1, h_jtphi_data->GetXaxis()->GetBinUpEdge(h_jtphi_data->GetNbinsX()), 1);
    line_jtphi->SetLineColor(kBlack);
    line_jtphi->SetLineStyle(kSolid);

    TCanvas *c_jtpt = new TCanvas("c_jtpt", "", 700, 600);
    TPad *top_pad_jtpt = new TPad("top_pad_jtpt", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad_jtpt = new TPad("top_pad_jtpt", "", 0., 0., 1., 0.33);

    top_pad_jtpt->SetLeftMargin(0.15);
    top_pad_jtpt->SetRightMargin(0.05);
    top_pad_jtpt->SetTopMargin(0.09);
    top_pad_jtpt->SetBottomMargin(0.03);
    bottom_pad_jtpt->SetLeftMargin(0.15);
    bottom_pad_jtpt->SetRightMargin(0.05);
    bottom_pad_jtpt->SetTopMargin(0.04);
    bottom_pad_jtpt->SetBottomMargin(0.35);

    top_pad_jtpt->cd();

    h_jtpt_data->GetYaxis()->SetTitle("1/N dN/dp_{T}^{jet}");
    h_jtpt_data->GetYaxis()->SetRangeUser(0, 0.055);
    h_jtpt_data->GetXaxis()->SetTitleOffset(20.);
    h_jtpt_data->GetXaxis()->SetLabelOffset(20.);
    h_jtpt_data->Draw("pe1 same");
    h_jtpt_dijet->Draw("hist same");
    h_jtpt_herwig_dijet->Draw("hist same");
    h_jtpt_data_tagged->Draw("pe1 same");
    h_jtpt_bjet_tagged->Draw("hist same");
    h_jtpt_herwig_bjet_tagged->Draw("hist same");

    TLatex *jtpt_info = new TLatex;
    jtpt_info->SetNDC();
    jtpt_info->SetTextSize(text_size-4);
    jtpt_info->SetTextAlign(32);
    jtpt_info->DrawLatex(0.9, 0.36, "anti-k_{T}, R=0.4 jets");
    jtpt_info->DrawLatex(0.9, 0.3, "|#eta^{jet}| < 2");
    jtpt_info->Draw();

    leg_jtpt->Draw();
    drawHeader();

    bottom_pad_jtpt->cd();

    h_jtpt_dijet_ratio->GetXaxis()->SetTitle("p_{T}^{jet} (GeV/c)");
    h_jtpt_dijet_ratio->GetXaxis()->SetTitleOffset(1.);
    h_jtpt_dijet_ratio->GetYaxis()->SetRangeUser(0.9, 1.1);
    h_jtpt_dijet_ratio->GetYaxis()->SetTitle("Ratio to data");
    h_jtpt_dijet_ratio->GetYaxis()->SetNdivisions(8);
    h_jtpt_dijet_ratio->Draw("hist same");
    line_jtpt->Draw();
    h_jtpt_dijet_ratio->Draw("hist same");
    h_jtpt_bjet_tagged_ratio->Draw("hist same");
    h_jtpt_herwig_dijet_ratio->Draw("hist same");
    h_jtpt_herwig_bjet_tagged_ratio->Draw("hist same");

    c_jtpt->cd();
    top_pad_jtpt->Draw();
    bottom_pad_jtpt->Draw();

    c_jtpt->Draw();
    // c_jtpt->Print("plots_an/jtpt_distribution.pdf");
    c_jtpt->Print("plots_thesis/jtpt_distribution.pdf");

    TCanvas *c_jteta = new TCanvas("c_jteta", "", 700, 600);
    TPad *top_pad_jteta = new TPad("top_pad_jteta", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad_jteta = new TPad("top_pad_jteta", "", 0., 0., 1., 0.33);

    top_pad_jteta->SetLeftMargin(0.15);
    top_pad_jteta->SetRightMargin(0.05);
    top_pad_jteta->SetTopMargin(0.09);
    top_pad_jteta->SetBottomMargin(0.03);
    bottom_pad_jteta->SetLeftMargin(0.15);
    bottom_pad_jteta->SetRightMargin(0.05);
    bottom_pad_jteta->SetTopMargin(0.04);
    bottom_pad_jteta->SetBottomMargin(0.35);

    top_pad_jteta->cd();

    h_jteta_data->GetYaxis()->SetTitle("1/N dN/d#eta^{jet}");
    h_jteta_data->GetYaxis()->SetRangeUser(0, 0.5);
    h_jteta_data->GetXaxis()->SetTitleOffset(20.);
    h_jteta_data->GetXaxis()->SetLabelOffset(20.);
    h_jteta_data->Draw("pe1 same");
    h_jteta_dijet->Draw("hist same");
    h_jteta_herwig_dijet->Draw("hist same");
    h_jteta_data_tagged->Draw("pe1 same");
    h_jteta_bjet_tagged->Draw("hist same");
    h_jteta_herwig_bjet_tagged->Draw("hist same");

    TLatex *jteta_info = new TLatex;
    jteta_info->SetNDC();
    jteta_info->SetTextSize(text_size-4);
    jteta_info->SetTextAlign(12);
    jteta_info->DrawLatex(0.2, 0.8, "anti-k_{T}, R=0.4 jets");
    jteta_info->DrawLatex(0.2, 0.72, "80 < p_{T}^{jet} < 140 GeV (GeV/c)");
    jteta_info->Draw();

    leg_jteta->Draw();
    drawHeader();

    bottom_pad_jteta->cd();

    h_jteta_dijet_ratio->GetXaxis()->SetTitle("#eta^{jet}");
    h_jteta_dijet_ratio->GetXaxis()->SetTitleOffset(1.);
    h_jteta_dijet_ratio->GetYaxis()->SetRangeUser(0.9, 1.1);
    h_jteta_dijet_ratio->GetYaxis()->SetTitle("Ratio to data");
    h_jteta_dijet_ratio->GetYaxis()->SetNdivisions(8);
    h_jteta_dijet_ratio->Draw("hist same");
    line_jteta->Draw();
    h_jteta_dijet_ratio->Draw("hist same");
    h_jteta_bjet_tagged_ratio->Draw("hist same");
    h_jteta_herwig_dijet_ratio->Draw("hist same");
    h_jteta_herwig_bjet_tagged_ratio->Draw("hist same");

    c_jteta->cd();
    top_pad_jteta->Draw();
    bottom_pad_jteta->Draw();

    c_jteta->Draw();
    // c_jteta->Print("plots_an/jteta_distribution.pdf");
    c_jteta->Print("plots_thesis/jteta_distribution.pdf");

    TCanvas *c_jtphi = new TCanvas("c_jtphi", "", 700, 600);
    TPad *top_pad_jtphi = new TPad("top_pad_jtphi", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad_jtphi = new TPad("top_pad_jtphi", "", 0., 0., 1., 0.33);

    top_pad_jtphi->SetLeftMargin(0.15);
    top_pad_jtphi->SetRightMargin(0.05);
    top_pad_jtphi->SetTopMargin(0.09);
    top_pad_jtphi->SetBottomMargin(0.03);
    bottom_pad_jtphi->SetLeftMargin(0.15);
    bottom_pad_jtphi->SetRightMargin(0.05);
    bottom_pad_jtphi->SetTopMargin(0.04);
    bottom_pad_jtphi->SetBottomMargin(0.35);

    top_pad_jtphi->cd();

    h_jtphi_data->GetYaxis()->SetTitle("1/N dN/d#phi^{jet}");
    h_jtphi_data->GetYaxis()->SetRangeUser(0, 0.4);
    h_jtphi_data->GetXaxis()->SetTitleOffset(20.);
    h_jtphi_data->GetXaxis()->SetLabelOffset(20.);
    h_jtphi_data->Draw("pe1 same");
    h_jtphi_dijet->Draw("hist same");
    h_jtphi_herwig_dijet->Draw("hist same");
    h_jtphi_data_tagged->Draw("pe1 same");
    h_jtphi_bjet_tagged->Draw("hist same");
    h_jtphi_herwig_bjet_tagged->Draw("hist same");

    TLatex *jtphi_info = new TLatex;
    jtphi_info->SetNDC();
    jtphi_info->SetTextSize(text_size-4);
    jtphi_info->SetTextAlign(12);
    jtphi_info->DrawLatex(0.2, 0.8, "anti-k_{T}, R=0.4 jets");
    jtphi_info->DrawLatex(0.2, 0.72, "|#eta^{jet}| < 2, 80 < p_{T}^{jet} < 140 (GeV/c)");
    jtphi_info->Draw();

    leg_jtphi->Draw();
    drawHeader();

    bottom_pad_jtphi->cd();

    h_jtphi_dijet_ratio->GetXaxis()->SetTitle("#phi^{jet}");
    h_jtphi_dijet_ratio->GetXaxis()->SetTitleOffset(1.);
    h_jtphi_dijet_ratio->GetYaxis()->SetRangeUser(0.9, 1.1);
    h_jtphi_dijet_ratio->GetYaxis()->SetTitle("Ratio to data");
    h_jtphi_dijet_ratio->GetYaxis()->SetNdivisions(8);
    h_jtphi_dijet_ratio->Draw("hist same");
    line_jtphi->Draw();
    h_jtphi_dijet_ratio->Draw("hist same");
    h_jtphi_bjet_tagged_ratio->Draw("hist same");
    h_jtphi_herwig_dijet_ratio->Draw("hist same");
    h_jtphi_herwig_bjet_tagged_ratio->Draw("hist same");

    c_jtphi->cd();
    top_pad_jtphi->Draw();
    bottom_pad_jtphi->Draw();

    c_jtphi->Draw();
    // c_jtphi->Print("plots_an/jtphi_distribution.pdf");
    c_jtphi->Print("plots_thesis/jtphi_distribution.pdf");

}