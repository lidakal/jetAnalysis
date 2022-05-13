#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveText.h"

#include "../utils.h" // normalise_histo, set_axes_labels

#include <algorithm> // std::max

void draw_discriminants() 
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

    // Modify the appearence of the histograms
    std::string x1title = "ip3dSig";
    std::string y1title = "1/N_{total tracks (not) from B decays | not in SV} dN_{tracks (not) from B decays | not in SV}/dip3dSig";
    std::string y2title = "1/N_{total tracks (not) from B decays | in SV} dN_{tracks (not) from B decays | in SV}/dip3dSig";

    normalise_histo(hsig_ip3dSig_notInSV);
    set_axes_labels(hsig_ip3dSig_notInSV, x1title, y1title);
    hsig_ip3dSig_notInSV->SetLineColor(1);

    normalise_histo(hbkg_ip3dSig_notInSV);
    set_axes_labels(hbkg_ip3dSig_notInSV, x1title, y1title);
    hbkg_ip3dSig_notInSV->SetLineColor(2);

    normalise_histo(hsig_ip3dSig_inSV);
    set_axes_labels(hsig_ip3dSig_inSV, x1title, y2title);
    hsig_ip3dSig_inSV->SetLineColor(1);

    normalise_histo(hbkg_ip3dSig_inSV);
    set_axes_labels(hbkg_ip3dSig_inSV, x1title, y2title);
    hbkg_ip3dSig_inSV->SetLineColor(2);
   
    // Create info box and legend 
    TPaveText *info_notInSV = new TPaveText(0.2, 0.7, 0.5, 0.85);
    info_notInSV->AddText("track p_{T} > 1 GeV");

    TPaveText *info_inSV = (TPaveText *) info_notInSV->Clone();
    info_inSV->AddText("in SV");

    info_notInSV->AddText("not in SV");

    TLegend *leg_notInSV = new TLegend(0.2, 0.75, 0.5, 0.85);
    leg_notInSV->SetBorderSize(0);
    leg_notInSV->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);

    TLegend *leg_inSV = (TLegend *) leg_notInSV->Clone();

    leg_notInSV->AddEntry(hsig_ip3dSig_notInSV, "tracks from B decays", "l");
    leg_notInSV->AddEntry(hbkg_ip3dSig_notInSV, "tracks NOT from B decays", "l");

    leg_inSV->AddEntry(hsig_ip3dSig_inSV, "tracks from B decays", "l");
    leg_inSV->AddEntry(hbkg_ip3dSig_inSV, "tracks NOT from B decays", "l");

    TCanvas *c_ip3dSig = new TCanvas("c_ip3dSig", "", 1800, 800);
    c_ip3dSig->Divide(1, 2);

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
}
