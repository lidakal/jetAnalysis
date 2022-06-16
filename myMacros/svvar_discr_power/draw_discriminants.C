#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

#include "../utils.h" // normalise_histo, set_axes_labels
#include "../pt2dscan/myPalette.h"

void draw_discriminants() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "bDiscriminants_histos.root";

    TFile *fin = new TFile((indir + fname).c_str());
    TH1D *hsig_ip3dSig = (TH1D *) fin->Get("hsig_ip3dSig");
    TH1D *hbkg_ip3dSig = (TH1D *) fin->Get("hbkg_ip3dSig");
    TH1D *hsig_inSV = (TH1D *) fin->Get("hsig_inSV");
    TH1D *hbkg_inSV = (TH1D *) fin->Get("hbkg_inSV");

    hsig_ip3dSig->GetXaxis()->SetRange(0, hsig_ip3dSig->GetNbinsX() + 1);
    hbkg_ip3dSig->GetXaxis()->SetRange(0, hbkg_ip3dSig->GetNbinsX() + 1);
    Float_t total_tracks = hsig_ip3dSig->Integral() + hbkg_ip3dSig->Integral();
    Float_t ip3dSig_bin_width = hsig_ip3dSig->GetXaxis()->GetBinWidth(1);

    std::string x1title = "ip3dSig";
    std::string y1title = "1/N_{total tracks} dN_{tracks (not) from B decays}/dip3dSig";
    std::string y2title = "1/N_{total tracks} N_{tracks (not) from B decays}";

    const Int_t nh = 2;
    TH1D *hs_ip3dSig[nh] = {hbkg_ip3dSig, hsig_ip3dSig};
    TH1D *hs_inSV[nh] = {hbkg_inSV, hsig_inSV};
    int colors[nh] = {mykRed, mykBlue};

    THStack *hstack_ip3dSig = new THStack();
    THStack *hstack_inSV = new THStack();

    for (Int_t ih = 0; ih < nh; ih++) {
        // normalise the histograms
        hs_ip3dSig[ih]->Scale(1 / (total_tracks * ip3dSig_bin_width));
        hs_inSV[ih]->Scale(1 / total_tracks);

        // Set SV bin labels
        hs_inSV[ih]->GetXaxis()->SetBinLabel(1, "not in SV");
        hs_inSV[ih]->GetXaxis()->SetBinLabel(2, "in SV");

        // Set appearence of histos
        hs_ip3dSig[ih]->SetLineColor(kBlack);
		hs_inSV[ih]->SetLineColor(kBlack);
        hs_ip3dSig[ih]->SetFillColor(colors[ih]);
		hs_inSV[ih]->SetFillColor(colors[ih]);
        hs_ip3dSig[ih]->SetFillStyle(1000);
		hs_inSV[ih]->SetFillStyle(1000);

        // Add to stack histos
        hstack_ip3dSig->Add(hs_ip3dSig[ih]);
        hstack_inSV->Add(hs_inSV[ih]);
    }

    TLegend *leg_ip3dSig = new TLegend(0.6, 0.8, 0.85, 0.9);
    leg_ip3dSig->SetBorderSize(1);
    leg_ip3dSig->SetFillStyle(1000);
	gStyle->SetLegendTextSize(15);
    leg_ip3dSig->SetMargin(0.2);
    //leg_ip3dSig->SetHeader("");

    TLegend *leg_inSV = (TLegend *) leg_ip3dSig->Clone();

    leg_ip3dSig->AddEntry(hsig_ip3dSig, "Tracks from B decays", "f");
    leg_ip3dSig->AddEntry(hbkg_ip3dSig, "Tracks NOT from B decays", "f");

    leg_inSV->AddEntry(hsig_inSV, "Tracks from B decays", "f");
    leg_inSV->AddEntry(hbkg_inSV, "Tracks NOT from B decays", "f");

    TPaveText *mcinfo = new TPaveText(0.2, 0.9, 0.4, 0.95, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");
    

    TCanvas *c_ip3dSig = new TCanvas("c_ip3dSig", "", 1000, 800);
    c_ip3dSig->SetGrid(1);
    hstack_ip3dSig->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
    hstack_ip3dSig->Draw("hist");
    leg_ip3dSig->Draw();
    mcinfo->Draw();
    c_ip3dSig->Draw();
	std::string savename_ip3dSig = "ip3dSig.png";
	c_ip3dSig->Print(savename_ip3dSig.c_str(), "png");

    TCanvas *c_inSV = new TCanvas("c_inSV", "", 1000, 800);
    c_inSV->SetGrid(1);
    hstack_inSV->SetTitle(Form("; %s; %s", "", y2title.c_str()));
    hstack_inSV->Draw("hist");
    hstack_inSV->SetMaximum(0.7);
    mcinfo->Draw();
    leg_inSV->Draw();
    c_inSV->Draw();
	std::string savename_inSV = "inSV.png";
	c_inSV->Print(savename_inSV.c_str(), "png");	
}
