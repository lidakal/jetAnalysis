#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <iostream>

void draw_svTagInfo() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "svTagInfo_histos_cpp.root";

    TFile *fin = new TFile((indir + fname).c_str());
    TH1D *hsv_trueB = (TH1D *) fin->Get("hsv_trueB");
    TH1D *hsv_trueB_noGSP = (TH1D *) fin->Get("hsv_trueB_noGSP");
    TH1D *hsv_wpB = (TH1D *) fin->Get("hsv_wpB");

    TCanvas *csv = new TCanvas("csv", "csv", 800, 800);

    //hsv->GetXaxis()->SetRange(2, hsv->GetNbinsX());

    const Int_t nh = 3;
    TH1D *hs[nh];

    for (Int_t hi = 0; hi < nh; hi++) {
	    hs[hi]->SetLineColor(hi + 1);
        hs[hi]->Draw("hist");
	}

    TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg->SetBorderSize(0);
	leg->SetFillStyle(0);

	leg->AddEntry(hsv_trueB, "true b-jets", "l");
	leg->AddEntry(hsv_trueB_noGSP, "true b-jets, no GSP", "l");
	leg->AddEntry(hsv_wpB, "b-jets passing wp", "l");
	leg->Draw();

    csv->Draw();
}
