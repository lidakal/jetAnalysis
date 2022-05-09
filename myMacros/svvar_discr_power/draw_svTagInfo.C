#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"

void draw_svTagInfo(bool GSPincl = true) 
{
    std::string finname = "/home/llr/cms/kalipoliti/rootFiles/svTagInfo_histos.root";
    TFile *fin = new TFile(finname.c_str());

    TH1D *hsv = (TH1D *) fin->Get("hsv");

    TCanvas *csv = new TCanvas("csv", "csv", 800, 800);
    hsv->Draw();
    csv->Draw();
}