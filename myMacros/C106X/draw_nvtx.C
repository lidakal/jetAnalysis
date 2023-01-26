#include <TFile.h>
#include <TH3.h>
#include <TH1.h>
#include <THStack.h>
#include <TString.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>

#include "myPalette.h"
#include "hist_utils.h"

void draw_nvtx()
{
    // Load & draw the nvtx histo
    TString fdir_nvtx = "./histos/";
    TString label_nvtx = "nvtx";
    TString suffix_nvtx = "_truth";
    TString fname_nvtx = label_nvtx + suffix_nvtx + ".root";
    TString rootf_nvtx = fdir_nvtx + fname_nvtx;

    TFile *fin_nvtx = new TFile(rootf_nvtx);
    TH1D *hNvtx = (TH1D *) fin_nvtx->Get("hNvtx");
    normalize_histo(hNvtx);
    format_histo(hNvtx, kBlue);
    hNvtx->GetXaxis()->SetTitle("nvtx");
    hNvtx->GetYaxis()->SetTitle("1/N_{events} dN / dnvtx");

    TCanvas *cNvtx = new TCanvas("cNvtx", "", 800, 500);
    hNvtx->Draw("histo");
    cNvtx->Draw();

    TString cname = "./plots/nvtx.png";
    cNvtx->Print(cname);
}
