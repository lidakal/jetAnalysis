#include "TFile.h"
#include "TH3D.h"
#include "TH1D.h"

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