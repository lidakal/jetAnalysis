#include "TH3D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"
#include "TPad.h"

#include "../HistDrawer_pt2dscan.h"

#include <regex>
#include "stdlib.h"

void draw_projections1d(std::string var = "zg", bool GSPincl = false)
{
    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }

    Float_t lowpt[] = {50., 80.};
    Float_t midpt[] = {100., 150.};
    Float_t highpt[] = {200., 250.};

    Float_t ktrange[] = {0., 5.};    
    Float_t ktmin = ktrange[0];
    Float_t ktmax = ktrange[1];

    const Int_t npt = 3;
    Float_t *ptranges[npt] = {lowpt, midpt, highpt};

    TCanvas *c = new TCanvas("c", "", 1800, 450);
    // Divide canvas into 3 pads
    c->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0., 0., 0.34, 1.);
    pad1->SetRightMargin(0.);
    pad1->SetGrid(1);
    pad1->Draw();

    c->cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0.34, 0., 0.66, 1.);
    pad2->SetLeftMargin(0.);
    pad2->SetRightMargin(0.);
    pad2->SetGrid(1);
    pad2->Draw();

    c->cd();
    TPad *pad3 = new TPad("pad3", "pad3", 0.66, 0., 1., 1.);
    pad3->SetLeftMargin(0.);
    pad3->SetGrid(1);
    pad3->Draw();

    TPad *pads[npt] = {pad1, pad2, pad3};

    for (Int_t i = 0; i < npt; i++) {
        Float_t ptmin = ptranges[i][0];
        Float_t ptmax = ptranges[i][1];
        Float_t ptrange[2] = {ptmin, ptmax};

        THStack *hs = new THStack("hs", "");

        TLegend *leg = new TLegend(0.57, 0.65, 1., 0.9);
        leg->SetBorderSize(1);
        gStyle->SetLegendTextSize(15);
        leg->SetMargin(0.1);

        std::string xtitle = "";        
        if (var == "rg") {
            xtitle = "ln(1/R_{g})";
        } else if (var == "kt") {
            xtitle = "ln(k_{T})";
        } else if (var == "zg") {
            xtitle = "z_{g}";
        }
        std::string ytitle = "1/N_{2-prong jets} dN/d" + xtitle;

        if (i != 0) ytitle = "";
        if (i != 2) xtitle = "";

        HistDrawer_pt2dscan HD;
        HD.draw_1d_projection_1ptbin(hs, leg, GSPincl, var, ptrange, ktrange);

        pads[i]->cd();

        hs->SetTitle(Form("; %s; %s", xtitle.c_str(), ytitle.c_str()));

        Float_t ymin = 0.;
        Float_t ymax = 0.9;

        if ((ktmin >= 0) && (var == "rg")) {
            ymax = 1.4;
        }
        if (var == "zg") {
            ymax = 7.;
        }

        hs->SetMinimum(ymin);
        hs->SetMaximum(ymax);

        hs->Draw("nostack hist");

        Float_t xmin = 0.91;
        Float_t xmax = 4.99;
        
        if (var == "zg") {
            xmin = 0.1;
            xmax = 0.49;
        }
        if ((ktmin >= 0) && (var == "rg")) {
            xmax = 3.45;
        }
        hs->GetXaxis()->SetLimits(xmin, xmax);
        hs->GetYaxis()->SetTitleOffset(1.5);

        leg->Draw();
        gPad->Update();
        if (i == 2) {
            leg->SetX1NDC(0.4);
            leg->SetX2NDC(0.85);
        }
        gPad->Modified();

        TPaveText *info = new TPaveText(0.55, 0.45, 0.85, 0.58, "ndc");
        info->SetBorderSize(0);
        info->SetFillColor(0);
        info->SetFillStyle(0);
        info->SetTextSize(15);
        info->AddText(Form("%.0f < #it{p_{T}^{jet}} < %0.f (GeV)", ptmin, ptmax));
        info->AddText("-2 < #it{#eta^{jet}} < 2");
        info->AddText(Form("%.0f < ln(k_{T}) < %.0f", ktmin, ktmax));
        info->Draw();
    } // end loop over pt ranges
    
    pad1->cd();
    TPaveText *mcinfo = new TPaveText(0.35, 0.92, 0.4, 0.95, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");
    mcinfo->Draw();

    c->Draw();
	std::string savename = var + "_projections_" + var + noGSP + ".png";
	c->Print(savename.c_str());
}
