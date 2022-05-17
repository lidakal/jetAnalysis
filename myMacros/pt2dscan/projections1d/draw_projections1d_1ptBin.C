#include "TH3D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"
#include "../../utils.h"
#include <regex>

#include "stdlib.h"

void draw_projections1d_1ptBin(bool GSPincl = false)
{
    std::string gspTxt = "_noGSP";
    if (GSPincl) gspTxt = "";

    std::string fname_chargedSJ_ref = "~/rootFiles/chargedSJ_new" + gspTxt + "_ref.root"; // ljet truth
    std::string fname_chargedSJ_reco = "~/rootFiles/chargedSJ_new" + gspTxt + "_reco.root"; // ljet reco, bjet reco no merge

    std::string fname_merged_ref = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco" + gspTxt + "_ref.root"; // bjet truth merge
    std::string fname_merged_reco = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco" + gspTxt + "_reco.root"; // bjet reco merge only SV

    std::string fname_merged_ip3dSig_reco = "~/rootFiles/aggregateB_ip3dSig_looserCut" + gspTxt + "_reco.root"; // bjet reco merge SV and ip3dSig   

    TFile *f_chargedSJ_ref = new TFile(fname_chargedSJ_ref.c_str());
    TH3D *hL3d_chargedSJ_ref = (TH3D *) f_chargedSJ_ref->Get("hL_rgkt");
    hL3d_chargedSJ_ref->SetName("hL3d_chargedSJ_ref"); // ljet truth

    TFile *f_chargedSJ_reco = new TFile(fname_chargedSJ_reco.c_str());
    TH3D *hL3d_chargedSJ_reco = (TH3D *) f_chargedSJ_reco->Get("hL_rgkt"); // ljet reco
    hL3d_chargedSJ_reco->SetName("hL3d_chargedSJ_reco");
    TH3D *hB3d_chargedSJ_reco = (TH3D *) f_chargedSJ_reco->Get("hB_rgkt"); // bjet reco no merge
    hB3d_chargedSJ_reco->SetName("hB3d_chargedSJ_reco");

    TFile *f_merged_ref = new TFile(fname_merged_ref.c_str());
    TH3D *hB3d_merged_ref = (TH3D *) f_merged_ref->Get("hB_rgkt"); // bjet truth merge 
    hB3d_merged_ref->SetName("hB3d_merged_ref");

    TFile *f_merged_reco = new TFile(fname_merged_reco.c_str());
    TH3D *hB3d_merged_reco = (TH3D *) f_merged_reco->Get("hB_rgkt"); // bjet reco merge only SV
    hB3d_merged_reco->SetName("hB3d_merged_reco");

    TFile *f_merged_ip3dSig_reco = new TFile(fname_merged_ip3dSig_reco.c_str());
    TH3D *hB3d_merged_ip3dSig_reco = (TH3D *) f_merged_ip3dSig_reco->Get("hB_rgkt"); // bjet reco merge SV and ip3dSig
    hB3d_merged_ip3dSig_reco->SetName("hB3d_merged_ip3dSig_reco");

    TCanvas *crg = new TCanvas("crg", "crg", 1000, 1000);
	crg->SetGrid(1);

    Float_t ptmin = 200.;
    Float_t ptmax = 250.;

    // Clone histograms as to not destroy the pt range
    TH3D *hL3d_chargedSJ_ref_clone = (TH3D *) hL3d_chargedSJ_ref->Clone();
    TH3D *hL3d_chargedSJ_reco_clone = (TH3D *) hL3d_chargedSJ_reco->Clone();
    TH3D *hB3d_chargedSJ_reco_clone = (TH3D *) hB3d_chargedSJ_reco->Clone();
    TH3D *hB3d_merged_ref_clone = (TH3D *) hB3d_merged_ref->Clone();
    TH3D *hB3d_merged_reco_clone = (TH3D *) hB3d_merged_reco->Clone();
    TH3D *hB3d_merged_ip3dSig_reco_clone = (TH3D *) hB3d_merged_ip3dSig_reco->Clone();

    // Set pt range
    hL3d_chargedSJ_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
    hL3d_chargedSJ_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
    hB3d_chargedSJ_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
    hB3d_merged_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
    hB3d_merged_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
    hB3d_merged_ip3dSig_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);

    // Make projections
    TH1D *hL1d_chargedSJ_ref = (TH1D *) hL3d_chargedSJ_ref_clone->Project3D(Form("x"));
    TH1D *hL1d_chargedSJ_reco = (TH1D *) hL3d_chargedSJ_reco_clone->Project3D(Form("x"));
    TH1D *hB1d_chargedSJ_reco = (TH1D *) hB3d_chargedSJ_reco_clone->Project3D(Form("x"));
    TH1D *hB1d_merged_ref = (TH1D *) hB3d_merged_ref_clone->Project3D(Form("x"));
    TH1D *hB1d_merged_reco = (TH1D *) hB3d_merged_reco_clone->Project3D(Form("x"));
    TH1D *hB1d_merged_ip3dSig_reco = (TH1D *) hB3d_merged_ip3dSig_reco_clone->Project3D(Form("x"));

    hL1d_chargedSJ_ref->GetXaxis()->SetRange(1, hL1d_chargedSJ_ref->GetNbinsX() + 1);
    hL1d_chargedSJ_reco->GetXaxis()->SetRange(1, hL1d_chargedSJ_reco->GetNbinsX() + 1);
    hB1d_chargedSJ_reco->GetXaxis()->SetRange(1, hB1d_chargedSJ_reco->GetNbinsX() + 1);
    hB1d_merged_ref->GetXaxis()->SetRange(1, hB1d_merged_ref->GetNbinsX() + 1);
    hB1d_merged_reco->GetXaxis()->SetRange(1, hB1d_merged_reco->GetNbinsX() + 1);
    hB1d_merged_ip3dSig_reco->GetXaxis()->SetRange(1, hB1d_merged_ip3dSig_reco->GetNbinsX() + 1);


    hL1d_chargedSJ_ref->Scale(1 / hL1d_chargedSJ_ref->Integral("width"));
    hL1d_chargedSJ_reco->Scale(1 / hL1d_chargedSJ_reco->Integral("width"));
    hB1d_chargedSJ_reco->Scale(1 / hB1d_chargedSJ_reco->Integral("width"));
    hB1d_merged_ref->Scale(1 / hB1d_merged_ref->Integral("width"));
    hB1d_merged_reco->Scale(1 / hB1d_merged_reco->Integral("width"));
    hB1d_merged_ip3dSig_reco->Scale(1 / hB1d_merged_ip3dSig_reco->Integral("width"));

    Float_t ymax = std::max({hL1d_chargedSJ_ref->GetMaximum(), hL1d_chargedSJ_reco->GetMaximum(), 
                            hB1d_chargedSJ_reco->GetMaximum(), hB1d_merged_ref->GetMaximum(), 
                            hB1d_merged_reco->GetMaximum()});

    ymax += 0.05;

    hL1d_chargedSJ_ref->GetYaxis()->SetRangeUser(0, ymax);
    hL1d_chargedSJ_reco->GetYaxis()->SetRangeUser(0, ymax);
    hB1d_chargedSJ_reco->GetYaxis()->SetRangeUser(0, ymax);
    hB1d_merged_ref->GetYaxis()->SetRangeUser(0, ymax);
    hB1d_merged_reco->GetYaxis()->SetRangeUser(0, ymax);

    std::string xtitle1d = "ln(1/R_{g})";
    std::string ytitle1d = "1/N dN/d(ln(1/R_{g}))"; 

    set_axes_labels(hL1d_chargedSJ_ref, xtitle1d, ytitle1d);
    set_axes_labels(hL1d_chargedSJ_reco, xtitle1d, ytitle1d);
    set_axes_labels(hB1d_chargedSJ_reco, xtitle1d, ytitle1d);
    set_axes_labels(hB1d_merged_ref, xtitle1d, ytitle1d);
    set_axes_labels(hB1d_merged_reco, xtitle1d, ytitle1d);

    hL1d_chargedSJ_ref->SetLineColor(1);
    hL1d_chargedSJ_reco->SetLineColor(2);
	hL1d_chargedSJ_reco->SetLineStyle(2);
	hL1d_chargedSJ_ref->SetLineStyle(2);

    hB1d_chargedSJ_reco->SetLineColor(3);
    hB1d_merged_ref->SetLineColor(4);
    hB1d_merged_reco->SetLineColor(6);
    hB1d_merged_ip3dSig_reco->SetLineColor(7);

    TLegend *leg;
    leg = new TLegend(0.18, 0.68, 0.48, 0.88);
	gStyle->SetLegendTextSize(13);

	leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);

    leg->AddEntry(hL1d_chargedSJ_ref, "inclusive jets, truth", "l");
    leg->AddEntry(hL1d_chargedSJ_reco, "inclusive jets, reco", "l");
    leg->AddEntry(hB1d_chargedSJ_reco, "bjets, reco", "l");
    leg->AddEntry(hB1d_merged_ref, "bjets, merged, truth", "l");
    leg->AddEntry(hB1d_merged_reco, "bjets, merged SV only, reco", "l");
    leg->AddEntry(hB1d_merged_ip3dSig_reco, "bjets, merged SV and ip3dSig, reco", "l");
    
    hL1d_chargedSJ_ref->Draw("hist");
    hL1d_chargedSJ_reco->Draw("hist same");
    hB1d_chargedSJ_reco->Draw("hist same");
    hB1d_merged_ref->Draw("hist same");
    hB1d_merged_reco->Draw("hist same");
    hB1d_merged_ip3dSig_reco->Draw("hist same");

    leg->Draw();

    TPaveText *info = new TPaveText(0.2, 0.17, 0.45, 0.28, "ndc");
    info->SetBorderSize(1);
    info->SetFillColor(0);
    info->SetTextSize(15);
    info->AddText(Form("%.0f < p_{T} < %0.f (GeV)", ptmin, ptmax));
	if (!GSPincl) {
	    info->AddText("NO GSP");
	}
    info->Draw();

    crg->Draw();

	std::string savename = "rg_comparison" + gspTxt + ".png";
	crg->Print(savename.c_str(), "png");
}
