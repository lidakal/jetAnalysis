#include "TFile.h"
#include "TH3D.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

#include "../pt2dscan/myPalette.h"

TH1D * do_1d_projection(std::string fname, std::string hname, Float_t *ptrange)
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

void draw_rg_projection(std::string var, THStack *hs, TLegend *leg, THStack *hs_ratio, TLegend *leg_ratio, Float_t *ptrange)
{
    // var can be "rg" or "zg"
    Float_t ptmin = ptrange[0];
    Float_t ptmax = ptrange[1];

    Int_t pal[3] = {mykRed, mykGreen, mykBlue};

    static Int_t lcolor = 0;
    static Int_t lstyle = 1;

    std::string fname = "/home/llr/cms/kalipoliti/rootFiles/aggregateB_truthInfo_fixedBugs_noGSP_ref.root";

    TH1D *hB_rg = do_1d_projection(fname, Form("hB_%skt", var.c_str()), ptrange);
    hB_rg->Rebin(2);
    hB_rg->SetLineWidth(2);
    hB_rg->SetLineColor(pal[lcolor]);
    hB_rg->SetLineStyle(1);
    leg->AddEntry(hB_rg, Form("true b-jets, #it{p_{T}^{jet}} #in [%.0f, %.0f] (GeV)", ptmin, ptmax), "l");
    hs->Add(hB_rg);

    TH1D *hBtag_rg = do_1d_projection(fname, Form("hBtag_%skt", var.c_str()), ptrange);
    hBtag_rg->Rebin(2);
    hBtag_rg->SetLineColor(pal[lcolor]);
    hBtag_rg->SetLineWidth(2);
    hBtag_rg->SetLineStyle(2);
    leg->AddEntry(hBtag_rg, Form("#it{b}-tagged #it{b}-jets,  #it{p_{T}^{jet}} #in [%.0f, %.0f] (GeV)", ptmin, ptmax), "l");
    hs->Add(hBtag_rg);

    // Create ratio histogram
    TH1D *h_ratio = (TH1D *) hB_rg->Clone();
    h_ratio->Divide(hBtag_rg);
    h_ratio->SetLineWidth(2);
    h_ratio->SetLineColor(pal[lcolor]);
    leg_ratio->AddEntry(h_ratio, Form("%0.f < #it{p_{T}^{jet}} < %0.f (GeV)", ptmin, ptmax), "l");

	hs_ratio->Add(h_ratio);

    lcolor++;
}
