#include "draw_utils.h"
#include "cms_palette.h"

void draw_mb_new()
{
    // Setup 
    gStyle->SetErrorX(0.5);
    gStyle->SetCanvasPreferGL(kTRUE);
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString xlabel = "m_{b,ch} (GeV/c^{2})";
    TString ylabel = "1/N dN/dm_{b,ch}";

    // Load histograms
    TString sample = "dijet_PF40";
    TString label = "aggrTMVA_XXT";
    TString fin_name = "histos/"+sample+"_"+label+"_mb_jer_nom_jec_nom.root";
    std::cout << "Reading mc from: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);
    TH1D *hSingleB_mb = (TH1D *) fin->Get("hSingleB_mb");
    TH1D *hBB_mb = (TH1D *) fin->Get("hBB_mb");
    TH1D *hC_mb = (TH1D *) fin->Get("hC_mb");
    TH1D *hL_mb = (TH1D *) fin->Get("hL_mb");

    // Combine C+L while they still have the correct proportions
    TH1D *hCL_mb = (TH1D *) hC_mb->Clone("hCL_mb");
    hCL_mb->Add(hL_mb);

    // Normalize histograms
    for (auto h : {hSingleB_mb, hBB_mb, hCL_mb}) {
        h->Scale(1/h->Integral(), "width");
        h->SetLineWidth(4);
        h->SetMarkerStyle(1);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetYaxis()->SetTitle(ylabel);
        h->GetYaxis()->SetTitleOffset(1.2);
        std::cout << "h->GetMaximum() = " << h->GetMaximum() << std::endl;
        h->GetYaxis()->SetRangeUser(0, 0.5);
    } 

    // Format histograms
    hSingleB_mb->SetLineStyle(1);
    hSingleB_mb->SetLineColor(cmsBlue);
    hSingleB_mb->SetTitle("Single-b");

    hBB_mb->SetLineStyle(7);
    hBB_mb->SetLineColor(cmsViolet);
    hBB_mb->SetTitle("Double-b");

    hCL_mb->SetLineStyle(9);
    hCL_mb->SetLineColor(cmsOrange);
    hCL_mb->SetTitle("Light+c");

    // Build legend
    TLegend *leg_mb = new TLegend(0.7, 0.45, 0.9, 0.65);
    leg_mb->SetBorderSize(0);
    leg_mb->SetFillStyle(0);
    leg_mb->SetMargin(0.3);
    leg_mb->AddEntry(hCL_mb, "Light+c", "l");
    leg_mb->AddEntry(hSingleB_mb, "Single-b", "l");
    leg_mb->AddEntry(hBB_mb, "Double-b", "l");
    
    // Draw histograms 
    TCanvas *c_mb = new TCanvas("c_mb", "", 700, 600);
    c_mb->SetRightMargin(0.05);
    c_mb->SetLeftMargin(0.12);

    hSingleB_mb->Draw("hist same");
    hBB_mb->Draw("hist same");
    hCL_mb->Draw("hist same");
    leg_mb->Draw();

    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.12, 0.93, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.95, 0.93, "PYTHIA8 (pp 5.02 TeV)");

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size-4);
    jet_info->SetTextAlign(32);
    jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 b-tagged jets");
    jet_info->DrawLatex(0.9, 0.77, "80 < p_{T}^{jet,reco} < 140 GeV/c");
    jet_info->DrawLatex(0.9, 0.71, "|#eta^{jet}| < 2");

    c_mb->Draw();
    c_mb->Print("plots_thesis/mb_templates.pdf");

}