#include "TFile.h"
#include "TH3D.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

TH1D * do_rg_projection(std::string fname, std::string hname, Float_t *ptrange)
{
    // Making sure the histograms have different names 
    static Int_t counter = 0;

    // Open histogram
    TFile *fin = new TFile(fname.c_str());
    TH3D *h3d = (TH3D *) fin->Get(hname.c_str());

    // Get pt range and project to 1d
    Float_t ptmin = ptrange[0];
    Float_t ptmax = ptrange[1];
    h3d->GetZaxis()->SetRangeUser(ptmin, ptmax);
    TH1D *h1d = (TH1D *) h3d->Project3D(Form("x%d", counter++));

    // Normalise histogram -- do not use untagged jets
    h1d->GetXaxis()->SetRange(1, h1d->GetNbinsX() + 1);
    h1d->Scale(1 / h1d->Integral("width"));

    return h1d;
}

void draw_rg_projection(TCanvas *c, Float_t *ptrange)
{
    Float_t ptmin = ptrange[0];
    Float_t ptmax = ptrange[1];

    static Int_t lcolor = 1;
    static Int_t lstyle = 1;

    Float_t x1leg = 0.6, x2leg = 0.8;
    static Float_t y1leg = 0.7, y2leg = 0.85;

    TLegend *leg = new TLegend(x1leg, y1leg, x2leg, y2leg);
    leg->SetFillStyle(0);
    leg->SetBorderSize(1);
    gStyle->SetLegendTextSize(15);

    TLegend *leg_ratio = (TLegend *) leg->Clone();
    leg_ratio->SetBorderSize(0);

    THStack *hs = new THStack("hs", "");

    std::string fname = "/home/llr/cms/kalipoliti/rootFiles/aggregateB_ip3dSig_looserCut_ref.root";

    TH1D *hB_rg = do_rg_projection(fname, "hB_rgkt", ptrange);
    hB_rg->SetLineColor(lcolor);
    hB_rg->SetLineStyle(1);
    leg->AddEntry(hB_rg, "truth level, true b-jets", "l");
    hs->Add(hB_rg);

    TH1D *hBtag_rg = do_rg_projection(fname, "hBtag_rgkt", ptrange);
    hBtag_rg->SetLineColor(lcolor);
    hBtag_rg->SetLineStyle(2);
    leg->AddEntry(hBtag_rg, "truth level, tagged b-jets", "l");
    hs->Add(hBtag_rg);

    std::string x1title = "ln(1/R_{g})";
	std::string y1title = "1/N_{2-prong jets} dN/dln(1/R_{g})";
    std::string y2title = "N_{true b-jets} / N_{tagged b-jets}";

	hs->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));

    // Create ratio histogram
    TH1D *h_ratio = (TH1D *) hB_rg->Clone();
    h_ratio->Divide(hBtag_rg);
    h_ratio->SetLineColor(lcolor);
    leg_ratio->AddEntry(h_ratio, Form("%0.f < p_{T}^{jet} < %0.f (GeV)", ptmin, ptmax));

    h_ratio->SetTitle(Form("; %s; %s", x1title.c_str(), y2title.c_str()));

    c->cd(1);
    hs->Draw("nostack hist same");
    leg->SetHeader(Form("p_{T} #in [%.0f, %.0f] GeV", ptmin, ptmax));
    leg->Draw();

    c->cd(2);
    h_ratio->Draw("hist same");
    leg_ratio->Draw();

    lcolor++;
    y1leg -= 0.2;
    y2leg -= 0.2;
}