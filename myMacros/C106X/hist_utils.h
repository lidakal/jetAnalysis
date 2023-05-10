#include "myPalette.h"

void normalize_histo(TH1D *h, Int_t ixmin=0, Int_t ixmax=-1)
{
    if (ixmax == -1) ixmax = h->GetNbinsX() + 1;
    h->GetXaxis()->SetRange(ixmin, ixmax);
    h->Scale(1 / h->Integral("width"));
    h->GetXaxis()->SetRange(1, h->GetNbinsX());
}

void format_histo(TH1D *h, int color, int width=2)
{
    h->SetLineColor(color);
    h->SetFillColor(color);
    h->SetLineWidth(width);
}

void format_histo(TH1F *h, int color, int width=2)
{
    h->SetLineColor(color);
    h->SetFillColor(color);
    h->SetFillStyle(3000);
    h->SetLineWidth(width);
}

void format_histo(TH1I *h, int color, int width=2)
{
    h->SetLineColor(color);
    h->SetLineWidth(width);
}

void draw_histos_by_class(TH2F *h, THStack *hStack, bool normByTotalTracks=true, TLegend *leg=nullptr) {
    double totalTracks = h->GetEntries();

    TString label = hStack->GetName();
    // std::cout << "DEBUG: "
    //           << "hStack->GetName() = " << label
    //           << std::endl;

    // ---- PU tracks ----
    TH1F *hPU = (TH1F *) h->ProjectionX("hPU_" + label, 1, 1);
    // if (normByTotalTracks) {
    //     hPU->Scale(1. / (totalTracks * hPU->GetBinWidth(1)));
    // } else {
    //     hPU->GetXaxis()->SetRange(0, hPU->GetNbinsX() + 1);
    //     hPU->Scale(1. / hPU->Integral("width"));
        // hPU->GetXaxis()->SetRange(1, hPU->GetNbinsX());
    // }
    format_histo(hPU, mykGreen);
    if (leg) leg->AddEntry(hPU, "No match", "l");
    hStack->Add(hPU);

    // ---- Bkg tracks ----
    TH1F *hBkg = (TH1F *) h->ProjectionX("hBkg_" + label, 2, 2);
    // if (normByTotalTracks) {
    //     hBkg->Scale(1. / (totalTracks * hBkg->GetBinWidth(1)));
    // } else {
    //     hBkg->GetXaxis()->SetRange(0, hBkg->GetNbinsX() + 1);
    //     hBkg->Scale(1. / hBkg->Integral("width"));
        // hBkg->GetXaxis()->SetRange(1, hBkg->GetNbinsX());
    // }
    // hBkg->Scale(0.5);
    format_histo(hBkg, mykRed);
    if (leg) leg->AddEntry(hBkg, "Match to bkg", "l");
    hStack->Add(hBkg);

    // ---- Sig tracks ----
    TH1F *hSig = (TH1F *) h->ProjectionX("hSig_" + label, 3, 3);
    // if (normByTotalTracks) {
    //     hSig->Scale(1. / (totalTracks * hSig->GetBinWidth(1)));
    // } else {
    //     hSig->GetXaxis()->SetRange(0, hSig->GetNbinsX() + 1);
    //     hSig->Scale(1. / hSig->Integral("width"));
        // hSig->GetXaxis()->SetRange(1, hSig->GetNbinsX());
    // }
    format_histo(hSig, mykBlue);
    if (leg) leg->AddEntry(hSig, "Match to sig", "l");
    hStack->Add(hSig);
}

Float_t calc_rg(Float_t y1, Float_t phi1, Float_t y2, Float_t phi2) {
    ROOT::Math::PtEtaPhiMVector v1;
    v1.SetPhi(phi1);

    ROOT::Math::PtEtaPhiMVector v2;
    v2.SetPhi(phi2);

    Float_t dphi = ROOT::Math::VectorUtil::DeltaPhi(v1, v2);
    Float_t dy = y1 - y2;

    // Float_t dphi_test = std::acos(std::cos(phi1-phi2));

    // std::cout << "dphi = " << dphi << std::endl;
    // std::cout << "dphi_test = " << dphi_test << std::endl;
    Float_t rg = std::sqrt((dy*dy) + (dphi*dphi));
    return rg;
}