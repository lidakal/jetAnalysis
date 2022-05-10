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

void draw_projections1d()
{
    std::string fname_chargedSJ_ref = "~/rootFiles/chargedSJ_new_noGSP_ref.root"; // ljet truth
    std::string fname_chargedSJ_reco = "~/rootFiles/chargedSJ_new_noGSP_reco.root"; // ljet reco, bjet reco no merge

    std::string fname_merged_ref = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco_noGSP_ref.root"; // bjet truth merge
    std::string fname_merged_reco = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco_noGSP_reco.root"; // bjet reco merge

    TFile *f_chargedSJ_ref = new TFile(fname_chargedSJ_ref.c_str());
    TH3D *hL3d_chargedSJ_ref = (TH3D *) f_chargedSJ_ref->Get("hL_rgkt");
    hL3d_chargedSJ_ref->SetName("hL3d_chargedSJ_ref");

    TFile *f_chargedSJ_reco = new TFile(fname_chargedSJ_reco.c_str());
    TH3D *hL3d_chargedSJ_reco = (TH3D *) f_chargedSJ_reco->Get("hL_rgkt");
    hL3d_chargedSJ_reco->SetName("hL3d_chargedSJ_reco");
    TH3D *hB3d_chargedSJ_reco = (TH3D *) f_chargedSJ_reco->Get("hB_rgkt");
    hB3d_chargedSJ_reco->SetName("hB3d_chargedSJ_reco");

    TFile *f_merged_ref = new TFile(fname_merged_ref.c_str());
    TH3D *hB3d_merged_ref = (TH3D *) f_merged_ref->Get("hB_rgkt");
    hB3d_merged_ref->SetName("hB3d_merged_ref");

    TFile *f_merged_reco = new TFile(fname_merged_reco.c_str());
    TH3D *hB3d_merged_reco = (TH3D *) f_merged_reco->Get("hB_rgkt");
    hB3d_merged_reco->SetName("hB3d_merged_reco");

    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};

    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};

    TCanvas *crg = new TCanvas("crg", "crg", 1800, 600);
    crg->Divide(npt, 1); // 2 for truth + reco, truth dynKt + reco dynKt

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        // Clone histograms as to not destroy the pt range
        TH3D *hL3d_chargedSJ_ref_clone = (TH3D *) hL3d_chargedSJ_ref->Clone();
        TH3D *hL3d_chargedSJ_reco_clone = (TH3D *) hL3d_chargedSJ_reco->Clone();
        TH3D *hB3d_chargedSJ_reco_clone = (TH3D *) hB3d_chargedSJ_reco->Clone();
        TH3D *hB3d_merged_ref_clone = (TH3D *) hB3d_merged_ref->Clone();
        TH3D *hB3d_merged_reco_clone = (TH3D *) hB3d_merged_reco->Clone();

        // Set pt range
        hL3d_chargedSJ_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        hL3d_chargedSJ_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        hB3d_chargedSJ_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        hB3d_merged_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        hB3d_merged_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);

        // Make projections
        TH1D *hL1d_chargedSJ_ref = (TH1D *) hL3d_chargedSJ_ref_clone->Project3D(Form("x%d", i));
        TH1D *hL1d_chargedSJ_reco = (TH1D *) hL3d_chargedSJ_reco_clone->Project3D(Form("x%d", i));
        TH1D *hB1d_chargedSJ_reco = (TH1D *) hB3d_chargedSJ_reco_clone->Project3D(Form("x%d", i));
        TH1D *hB1d_merged_ref = (TH1D *) hB3d_merged_ref_clone->Project3D(Form("x%d", i));
        TH1D *hB1d_merged_reco = (TH1D *) hB3d_merged_reco_clone->Project3D(Form("x%d", i));

        normalise_histo(hL1d_chargedSJ_ref);
        normalise_histo(hL1d_chargedSJ_reco);
        normalise_histo(hB1d_chargedSJ_reco);
        normalise_histo(hB1d_merged_ref);
        normalise_histo(hB1d_merged_reco);

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
        std::string ytitle1d = Form("1/N dN/d(ln(1/R_{g})) for jetpt in [%.0f, %.0f] GeV", ptmin, ptmax);

        set_axes_labels(hL1d_chargedSJ_ref, xtitle1d, ytitle1d);
        set_axes_labels(hL1d_chargedSJ_reco, xtitle1d, ytitle1d);
        set_axes_labels(hB1d_chargedSJ_reco, xtitle1d, ytitle1d);
        set_axes_labels(hB1d_merged_ref, xtitle1d, ytitle1d);
        set_axes_labels(hB1d_merged_reco, xtitle1d, ytitle1d);

        hL1d_chargedSJ_ref->SetLineColor(1);
        hL1d_chargedSJ_reco->SetLineColor(2);
        hB1d_chargedSJ_reco->SetLineColor(3);
        hB1d_merged_ref->SetLineColor(4);
        hB1d_merged_reco->SetLineColor(6);

        TLegend *leg = new TLegend(0.5, 0.7, 0.7, 0.9);
        leg->SetBorderSize(0);
        leg->SetFillColor(0);

        leg->AddEntry(hL1d_chargedSJ_ref, "inclusive jets, truth", "l");
        leg->AddEntry(hL1d_chargedSJ_reco, "inclusive jets, reco", "l");
        leg->AddEntry(hB1d_chargedSJ_reco, "bjets, reco", "l");
        leg->AddEntry(hB1d_merged_ref, "bjets, merged, truth", "l");
        leg->AddEntry(hB1d_merged_reco, "bjets, merged, reco", "l");

        crg->cd(i + 1);
        crg->cd(i + 1)->SetGrid(1);

        hL1d_chargedSJ_ref->Draw("hist");
        hL1d_chargedSJ_reco->Draw("hist same");
        hB1d_chargedSJ_reco->Draw("hist same");
        hB1d_merged_ref->Draw("hist same");
        hB1d_merged_reco->Draw("hist same");

        leg->Draw();
    }
    crg->Draw();
}
