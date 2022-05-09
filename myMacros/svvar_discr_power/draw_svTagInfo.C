#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "../utils.h"

#include <iostream>
#include "stdlib.h"

void draw_svTagInfo() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "svTagInfo_histos_cpp.root";

    TFile *fin = new TFile((indir + fname).c_str());
    TH1D *hsv_trueB = (TH1D *) fin->Get("hsv_trueB");
    TH1D *hsv_trueB_noGSP = (TH1D *) fin->Get("hsv_trueB_noGSP");
    TH1D *hsv_wpB = (TH1D *) fin->Get("hsv_wpB");
    TH1D *hsv_wpB_2prod = (TH1D *) fin->Get("hsv_wpB_2prod");

    TCanvas *csv = new TCanvas("csv", "csv", 1200, 800);

    //hsv->GetXaxis()->SetRange(2, hsv->GetNbinsX());

    const Int_t nh = 4;
    TH1D *hs[nh] = {hsv_trueB, hsv_trueB_noGSP, hsv_wpB, hsv_wpB_2prod};
    //Float_t ymax = std::max({hsv_trueB->GetMaximum(), hsv_trueB_noGSP->GetMaximum(), hsv_wpB->GetMaximum()});
    for (Int_t hi = 0; hi < nh; hi++) {
	    // normalise histogram without underflow
	    hs[hi]->Scale(1 / hs[hi]->Integral());
        hs[hi]->SetYTitle("Nb of events");
	    hs[hi]->SetLineColor(hi + 1);
		//hs[hi]->GetYaxis()->SetRangeUser(0, ymax);
		//hs[hi]->GetXaxis()->SetRange(0, hs[hi]->GetNbinsX()+1);
        hs[hi]->Draw("hist same");
	}

    TLegend *leg = new TLegend(0.55, 0.7, 0.9, 0.9);
    leg->SetBorderSize(0);
	leg->SetFillStyle(0);

	leg->AddEntry(hsv_trueB, "true b-jets", "l");
	leg->AddEntry(hsv_trueB_noGSP, "true b-jets, no GSP", "l");
	leg->AddEntry(hsv_wpB, "b-jets passing wp", "l");
	leg->AddEntry(hsv_wpB_2prod, "b-jets passing wp, prods > 1", "l");
	leg->Draw();

    csv->Draw();
}
