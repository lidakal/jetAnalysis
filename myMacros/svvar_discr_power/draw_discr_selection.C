#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "THStack.h"

#include "../utils.h" // normalise_histo, set_axes_labels

#include <algorithm> // std::max

void draw_discr_selection() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "bDiscriminants_histos.root";

    TFile *fin = new TFile((indir + fname).c_str());
    TH2D *hsig_2d = (TH2D *) fin->Get("hsig_2d");
    TH2D *hbkg_2d = (TH2D *) fin->Get("hbkg_2d");
    // X = ip3dSig, Y = not in SV (bin 1) / in SV (bin 2)

    // Get histograms of ip3dSig for sig/bkg when not in SV / in SV
    TH2D *hsig_2d_notInSV = (TH2D *) hsig_2d->Clone();
    hsig_2d_notInSV->SetName("hsig_2d_notInSV");
    hsig_2d_notInSV->GetYaxis()->SetRange(1,1);
    TH1D *hsig_ip3dSig_notInSV = (TH1D *) hsig_2d_notInSV->ProjectionX();

    TH2D *hsig_2d_inSV = (TH2D *) hsig_2d->Clone();
    hsig_2d_inSV->SetName("hsig_2d_inSV");
    hsig_2d_inSV->GetYaxis()->SetRange(2, 2);
    TH1D *hsig_ip3dSig_inSV = (TH1D *) hsig_2d_inSV->ProjectionX();

    TH2D *hbkg_2d_notInSV = (TH2D *) hbkg_2d->Clone();
    hbkg_2d_notInSV->SetName("hbkg_2d_notInSV");
    hbkg_2d_notInSV->GetYaxis()->SetRange(1,1);
    TH1D *hbkg_ip3dSig_notInSV = (TH1D *) hbkg_2d_notInSV->ProjectionX();

    TH2D *hbkg_2d_inSV = (TH2D *) hbkg_2d->Clone();
    hbkg_2d_inSV->SetName("hbkg_2d_inSV");
    hbkg_2d_inSV->GetYaxis()->SetRange(2, 2);
    TH1D *hbkg_ip3dSig_inSV = (TH1D *) hbkg_2d_inSV->ProjectionX();

    // Copy histograms to make stack plot
    TH1D *hsig_ip3dSig_inSV_stack = (TH1D *) hsig_ip3dSig_inSV->Clone();
    TH1D *hsig_ip3dSig_notInSV_stack = (TH1D *) hsig_ip3dSig_notInSV->Clone();
    TH1D *hbkg_ip3dSig_inSV_stack = (TH1D *) hbkg_ip3dSig_inSV->Clone();
    TH1D *hbkg_ip3dSig_notInSV_stack = (TH1D *) hbkg_ip3dSig_notInSV->Clone();

    // Create sum histogram to normalise stack histograms
    TH1D *h_inSV_stack = (TH1D *) hsig_ip3dSig_inSV->Clone();
    h_inSV_stack->Add(hsig_ip3dSig_notInSV_stack);
    h_inSV_stack->GetXaxis()->SetRange(0, h_inSV_stack->GetNbinsX() + 1);
    Float_t inSV_norm = h_inSV_stack->Integral();

    TH1D *h_notInSV_stack = (TH1D *) hsig_ip3dSig_notInSV->Clone();
    h_notInSV_stack->Add(hsig_ip3dSig_notInSV_stack);
    h_notInSV_stack->GetXaxis()->SetRange(0, h_notInSV_stack->GetNbinsX() + 1);
    Float_t notInSV_norm = h_notInSV_stack->Integral();

	std::string x1title = "ip3dSig";
    std::string y1title = "1/N_{total tracks (not) from B decays | not in SV} dN_{tracks (not) from B decays | not in SV}/dip3dSig";
    std::string y2title = "1/N_{total tracks (not) from B decays | in SV} dN_{tracks (not) from B decays | in SV}/dip3dSig";
    std::string y3title = "1/N_{total tracks | in SV} dN_{tracks | in SV}/dip3dSig";
    std::string y4title = "1/N_{total tracks | not in SV} dN_{tracks | not in SV}/dip3dSig";


    // Create stack histograms
    THStack *hs_inSV = new THStack("hs_inSV", "");

    hbkg_ip3dSig_inSV_stack->Scale(1 / inSV_norm);
    hbkg_ip3dSig_inSV_stack->SetFillColor(2);
    hbkg_ip3dSig_inSV_stack->SetFillStyle(1001);
	set_axes_labels(hbkg_ip3dSig_inSV_stack, x1title, y1title);
    hs_inSV->Add(hbkg_ip3dSig_inSV_stack);

    hsig_ip3dSig_inSV_stack->Scale(1 / inSV_norm);
    hsig_ip3dSig_inSV_stack->SetFillColor(4);
    hsig_ip3dSig_inSV_stack->SetFillStyle(1001);
	set_axes_labels(hsig_ip3dSig_inSV_stack, x1title, y1title);
    hs_inSV->Add(hsig_ip3dSig_inSV_stack);

	hs_inSV->SetTitle(Form("; %s; %s", x1title.c_str(), y3title.c_str()));

    THStack *hs_notInSV = new THStack("hs_notInSV", "");

	hbkg_ip3dSig_notInSV_stack->Scale(1 / notInSV_norm);
    hbkg_ip3dSig_notInSV_stack->SetFillColor(2);
    hbkg_ip3dSig_notInSV_stack->SetFillStyle(1001);
	set_axes_labels(hbkg_ip3dSig_notInSV_stack, x1title, y1title);
    hs_notInSV->Add(hbkg_ip3dSig_notInSV_stack);

    hsig_ip3dSig_notInSV_stack->Scale(1 / notInSV_norm);
    hsig_ip3dSig_notInSV_stack->SetFillColor(4);
    hsig_ip3dSig_notInSV_stack->SetFillStyle(1001);
	set_axes_labels(hsig_ip3dSig_notInSV_stack, x1title, y1title);
    hs_notInSV->Add(hsig_ip3dSig_notInSV_stack);

	hs_notInSV->SetTitle(Form("; %s; %s", x1title.c_str(), y4title.c_str()));

    // Modify the appearence of the histograms
    
    normalise_histo(hsig_ip3dSig_notInSV);
	normalise_histo(hbkg_ip3dSig_notInSV);
	normalise_histo(hsig_ip3dSig_inSV);
	normalise_histo(hbkg_ip3dSig_inSV);

	Float_t ymax = std::max({hsig_ip3dSig_notInSV->GetMaximum(), hsig_ip3dSig_inSV->GetMaximum(), 
		  hbkg_ip3dSig_notInSV->GetMaximum(), hbkg_ip3dSig_inSV->GetMaximum()}) + 0.02;

    set_axes_labels(hsig_ip3dSig_notInSV, x1title, y1title);
	hsig_ip3dSig_notInSV->GetYaxis()->SetTitleOffset(2.);
	hsig_ip3dSig_notInSV->GetYaxis()->SetRangeUser(0., ymax);
    hsig_ip3dSig_notInSV->SetLineColor(1);

    
    set_axes_labels(hbkg_ip3dSig_notInSV, x1title, y1title);
    hbkg_ip3dSig_notInSV->SetLineColor(2);

    normalise_histo(hsig_ip3dSig_inSV);
    set_axes_labels(hsig_ip3dSig_inSV, x1title, y2title);
	hsig_ip3dSig_inSV->GetYaxis()->SetTitleOffset(2.);
	hsig_ip3dSig_inSV->GetYaxis()->SetRangeUser(0., ymax);
    hsig_ip3dSig_inSV->SetLineColor(1);

    normalise_histo(hbkg_ip3dSig_inSV);
    set_axes_labels(hbkg_ip3dSig_inSV, x1title, y2title);
    hbkg_ip3dSig_inSV->SetLineColor(2);
   
    // Create info box and legend 
    TPaveText *info_notInSV = new TPaveText(0.2, 0.75, 0.35, 0.85, "ndc");
	info_notInSV->SetFillColor(0);
	info_notInSV->SetBorderSize(1);
	info_notInSV->SetTextSize(15);
    info_notInSV->AddText("track p_{T} > 1 GeV");

    TPaveText *info_inSV = (TPaveText *) info_notInSV->Clone();
    info_inSV->AddText("in SV");

    info_notInSV->AddText("not in SV");

    TLegend *leg_notInSV = new TLegend(0.55, 0.75, 0.8, 0.85);
    leg_notInSV->SetBorderSize(0);
    leg_notInSV->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);

    TLegend *leg_inSV = (TLegend *) leg_notInSV->Clone();
    TLegend *leg_inSV_stack = (TLegend *) leg_notInSV->Clone();
    TLegend *leg_notInSV_stack = (TLegend *) leg_notInSV->Clone();

    leg_notInSV->AddEntry(hsig_ip3dSig_notInSV, "tracks from B decays", "l");
    leg_notInSV->AddEntry(hbkg_ip3dSig_notInSV, "tracks NOT from B decays", "l");

    leg_inSV->AddEntry(hsig_ip3dSig_inSV, "tracks from B decays", "l");
    leg_inSV->AddEntry(hbkg_ip3dSig_inSV, "tracks NOT from B decays", "l");

    TCanvas *c_ip3dSig = new TCanvas("c_ip3dSig", "", 1800, 800);
    c_ip3dSig->Divide(2, 1);

    c_ip3dSig->cd(1);
    hsig_ip3dSig_notInSV->Draw("hist");
    hbkg_ip3dSig_notInSV->Draw("hist same");
    info_notInSV->Draw();
    leg_notInSV->Draw();
    
    c_ip3dSig->cd(2);
    hsig_ip3dSig_inSV->Draw("hist");
    hbkg_ip3dSig_inSV->Draw("hist same");
    info_inSV->Draw();
    leg_inSV->Draw();

    c_ip3dSig->Draw();

    // Same for stack histograms
    leg_inSV_stack->AddEntry(hsig_ip3dSig_inSV_stack, "tracks from B decays", "f");
    leg_inSV_stack->AddEntry(hbkg_ip3dSig_inSV_stack, "tracks NOT from B decays", "f");

    leg_notInSV_stack->AddEntry(hsig_ip3dSig_notInSV_stack, "tracks from B decays", "f");
    leg_notInSV_stack->AddEntry(hbkg_ip3dSig_notInSV_stack, "tracks NOT from B decays", "f");
    
    TCanvas *c_ip3dSig_stack = new TCanvas("c_ip3dSig_stack", "", 1800, 800);
    c_ip3dSig_stack->Divide(2, 1);

    c_ip3dSig_stack->cd(1);
    hs_notInSV->Draw("hist");
    info_notInSV->Draw();
    leg_notInSV_stack->Draw();

    c_ip3dSig_stack->cd(2);
    hs_inSV->Draw("hist");
    info_inSV->Draw();
    leg_inSV_stack->Draw();

    c_ip3dSig_stack->Draw();
	
	std::string savename = "ip3dSig_inSV.png";
	c_ip3dSig->Print(savename.c_str(), "png");

    std::string savename_stack = "ip3dSig_inSV_stack.png";
	c_ip3dSig_stack->Print(savename_stack.c_str(), "png");

}
