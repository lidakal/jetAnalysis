#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"
#include "../../utils.h"
#include <regex>

#include "stdlib.h"

void draw_underflow()
{
    std::string fname_chargedSJ_ref = "~/rootFiles/chargedSJ_new_ref.root"; 
    std::string fname_merged_ref = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco_ref.root"; // bjet truth merge
   
    TFile *f_chargedSJ_ref = new TFile(fname_chargedSJ_ref.c_str());
    TH3D *hL3d_chargedSJ_ref = (TH3D *) f_chargedSJ_ref->Get("hL_rgkt");
    hL3d_chargedSJ_ref->SetName("hL3d_chargedSJ_ref");
    TH3D *hB3d_chargedSJ_ref = (TH3D *) f_chargedSJ_ref->Get("hB_rgkt");
    hB3d_chargedSJ_ref->SetName("hB3d_chargedSJ_ref");
   
    TFile *f_merged_ref = new TFile(fname_merged_ref.c_str());
    TH3D *hB3d_merged_ref = (TH3D *) f_merged_ref->Get("hB_rgkt");
    hB3d_merged_ref->SetName("hB3d_merged_ref");

    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};

    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};

    TCanvas *c = new TCanvas("c", "c", 1800, 600);
    c->Divide(npt, 1); // 2 for truth + reco, truth dynKt + reco dynKt

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        // Clone histograms as to not destroy the pt range
        TH3D *hL3d_chargedSJ_ref_clone = (TH3D *) hL3d_chargedSJ_ref->Clone();
        TH3D *hB3d_chargedSJ_ref_clone = (TH3D *) hB3d_chargedSJ_ref->Clone();
        TH3D *hB3d_merged_ref_clone = (TH3D *) hB3d_merged_ref->Clone();
        
        // Set pt range
        hL3d_chargedSJ_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        hB3d_chargedSJ_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        hB3d_merged_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        
        // Make projections
        TH1D *hL1d_chargedSJ_ref = (TH1D *) hL3d_chargedSJ_ref_clone->Project3D(Form("x%d", i));
        TH1D *hB1d_chargedSJ_ref = (TH1D *) hB3d_chargedSJ_ref_clone->Project3D(Form("x%d", i));
        TH1D *hB1d_merged_ref = (TH1D *) hB3d_merged_ref_clone->Project3D(Form("x%d", i));
        
        // Normalise histograms
        normalise_histo(hL1d_chargedSJ_ref);
        normalise_histo(hB1d_chargedSJ_ref);
        normalise_histo(hB1d_merged_ref);

        // New histogram to fill with underflow bins
        TH1D *hunderflow = new TH1D(Form("hunderflow%d", i), "histogram with underflow bin content", 3, 0, 1);
        
        Float_t barWidth = 0.7;
        hunderflow->SetBarOffset((1.-barWidth) / 2.);
        hunderflow->GetXaxis()->SetRangeUser(0., 1.);
        hunderflow->GetXaxis()->SetTickLength(0.);
        hunderflow->SetBarWidth(barWidth);
		hunderflow->SetFillColor(40);
		hunderflow->SetLineColor(1);

        hunderflow->GetXaxis()->SetBinLabel(1, "chargedSJ, L");
        hunderflow->GetXaxis()->SetBinLabel(2, "chargedSJ, B");
        hunderflow->GetXaxis()->SetBinLabel(3, "merged, B");
        
        hunderflow->SetBinContent(1, hL1d_chargedSJ_ref->GetBinContent(0));
        hunderflow->SetBinContent(2, hB1d_chargedSJ_ref->GetBinContent(0));
        hunderflow->SetBinContent(3, hB1d_merged_ref->GetBinContent(0));

		TPaveText *info = new TPaveText(0.2, 0.75, 0.5, 0.85, "ndc");
		info->SetBorderSize(1);
		info->SetFillColor(0);
		info->SetTextSize(15);
		info->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
		
        std::string ytitle = "1/N_{jets} dN_{1-prong jets}/d(ln(1/R_{g}))";
        hunderflow->SetYTitle(ytitle.c_str());

        c->cd(i + 1);

        hunderflow->Draw("b");
		info->Draw();
    }
    c->Draw();

	std::string savename = "underflow_comparison.png";
	c->Print(savename.c_str(), "png");
}
