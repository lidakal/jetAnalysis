#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "../utils.h" // normalise_histo, set_axes_labels

#include <algorithm> // std::max

void draw_svTagInfo() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "bDiscriminants_histos.root";

    TFile *fin = new TFile((indir + fname).c_str());
    TH1D *hsig_ip3dSig = (TH1D *) fin->Get("hsig_ip3dSig");
    TH1D *hbkg_ip3dSig = (TH1D *) fin->Get("hbkg_ip3dSig");
    TH1D *hsig_inSV = (TH1D *) fin->Get("hsig_inSV");
    TH1D *hbkg_inSV = (TH1D *) fin->Get("hbkg_inSV");

    std::string x1title = "ip3dSig";
    std::string y1title = "1/N_{total tracks (not) from B decays} dN_{tracks (not) from B decays}/dip3dSig";

    std::string y2title = "1/N_{total tracks (not) from B decays} N_{tracks (not) from B decays}";

    const Int_t nh = 2;
    TH1D *hs_ip3dSig[nh] = {hsig_ip3dSig, hbkg_ip3dSig};
    TH1D *hs_inSV[nh] = {hsig_inSV, hbkg_inSV};
    for (Int_t ih = 0; ih < nh; ih++) {
        normalise_histo(hs_ip3dSig[ih]);
        normalise_histo(hs_inSV[ih]);
        set_axes_labels(hs_ip3dSig[ih], x1title, y1title);
        set_axes_labels(hs_inSV[ih], "", y2title);
        hs_inSV[ih]->GetXaxis()->SetBinLabel(1, "not in SV");
        hs_inSV[ih]->GetXaxis()->SetBinLabel(2, "in SV");
        hs_ip3dSig[ih]->SetLineColor(ih + 1);
    }

    Float_t ymax_inSV = std::max({hsig_inSV->GetMaximum(), hbkg_inSV->GetMaximum()});
    Float_t ymax_ip3dSig = std::max(hsig_ip3dSig->GetMaximum(), hbkg_ip3dSig->GetMaximum());
    for (Int_t ih = 0; ih < nh; ih++) {
        hs_inSV[ih]->GetYaxis()->SetRangeUser(0, ymax_inSV);
        hs_ip3dSig[ih]->GetYaxis()->SetRangeUser(0, ymax_ip3dSig);
    }

    TLegend *leg_ip3dSig = new TLegend(0.5, 0.6, 0.8, 0.8);
    leg_ip3dSig->SetBorderSize(0);
    leg_ip3dSig->SetFillStyle(0);

    TLegend *leg_inSV = (TLegend *) leg_ip3dSig->Clone();

    leg_ip3dSig->AddEntry(hsig_ip3dSig, "tracks from B decays", "l");
    leg_ip3dSig->AddEntry(hbkg_ip3dSig, "tracks NOT from B decays", "l");

    leg_inSV->AddEntry(hsig_inSV, "tracks from B decays", "l");
    leg_inSV->AddEntry(hbkg_inSV, "tracks NOT from B decays", "l");

    TCanvas *c_ip3dSig = new TCanvas("c_ip3dSig", "", 1000, 800);
    hsig_ip3dSig->Draw("hist");
    hbkg_ip3dSig->Draw("hist same");
    leg_ip3dSig->Draw();
    c_ip3dSig->Draw();

    TCanvas *c_inSV = new TCanvas("c_inSV", "", 1000, 800);
    hsig_inSV->Draw("hist");
    hbkg_inSV->Draw("hist same");
    leg_inSV->Draw();
    c_inSV->Draw();

}