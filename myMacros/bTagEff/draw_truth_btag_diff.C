#include "TFile.h"
#include "TH1D.h"
#include "TStyle.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "utils.h" // do_rg_projection()

void draw_truth_btag_diff()
{
    TCanvas *c = new TCanvas("c", "", 1200, 800);
    TCanvas *c_ratio = new TCanvas("c_ratio", "", 1200, 800);

	THStack *hs = new THStack("hs", "");
	THStack *hs_ratio = new THStack("hs_ratio", "");

	std::string x1title = "ln(1/R_{g})";
	std::string y1title = "1/N_{2-prong jets} dN/dln(1/R_{g})";
	std::string y2title = "N_{true b-jets} / N_{tagged b-jets}";

	hs->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
	hs_ratio->SetTitle(Form("; %s; %s", x1title.c_str(), y2title.c_str()));

	TLegend *leg = new TLegend(0.55, 0.7, 0.85, 0.9);
    leg->SetFillStyle(1000);
    leg->SetBorderSize(1);
    gStyle->SetLegendTextSize(15);

    TLegend *leg_ratio = new TLegend(0.65, 0.15, 0.85, 0.27);
	leg_ratio->SetFillStyle(1000);
	leg_ratio->SetBorderSize(1);

    Float_t ptrange1[2] = {50., 80.};
    draw_rg_projection(c, c_ratio, hs, leg, hs_ratio, leg_ratio, ptrange1);

    Float_t ptrange2[2] = {100., 150.};
    draw_rg_projection(c, c_ratio, hs, leg, hs_ratio, leg_ratio, ptrange2);

    Float_t ptrange3[2] = {200., 250.};
    draw_rg_projection(c, c_ratio, hs, leg, hs_ratio, leg_ratio, ptrange3);

    TPaveText *info = new TPaveText(0.15, 0.22, 0.5, 0.37, "ndc");
	info->AddText("Truth level #it{b}-jets with aggregated pseudo-B's");
    info->SetTextSize(15);
    info->SetFillStyle(0);
    info->SetBorderSize(0);

    TPaveText *mcinfo = new TPaveText(0.17, 0.9, 0.4, 0.95, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");

    c->cd();
    c->SetGrid(1);
	hs->Draw("nostack hist");
	leg->Draw();
    info->Draw();
    mcinfo->Draw();

    c->Draw();
	std::string savename = "true_tagged_b_diff.png";
	c->Print(savename.c_str(), "png");

    c_ratio->SetGrid(1);
    c_ratio->cd();
	hs_ratio->Draw("nostack hist");
	leg_ratio->Draw();
	info->Draw();
    mcinfo->Draw();

    c_ratio->Draw();
	std::string savename_ratio = "true_tagged_b_diff_ratio.png";
	c_ratio->Print(savename_ratio.c_str(), "png");

    
}
