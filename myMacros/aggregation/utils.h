#include "TGraph.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TStyle.h"

TGraph * make_graph_eff_mistag(std::string fname, std::vector<Int_t> v_selN, Style_t markerStyle, Color_t markerColor, TLegend *leg, std::string label)
{
    TFile *fin = new TFile(fname.c_str());
    TGraph *gr = new TGraph();
 
    for (Int_t selN : v_selN) {
        TH1D *h = (TH1D *) fin->Get(Form("heff_sel%d", selN));

        Float_t totalFromB = h->GetBinContent(1, 1) + h->GetBinContent(1, 2);
        Float_t totalNotFromB = h->GetBinContent(2, 1) + h->GetBinContent(2, 2);

        Float_t passFromB = h->GetBinContent(1, 1); 
        Float_t passNotFromB = h->GetBinContent(2, 1);

        Float_t x = passFromB / totalFromB; // efficiency
        Float_t y = passNotFromB / totalNotFromB; // mistag rate

        gr->SetPoint(gr->GetN(), x, y);
    }

    gr->SetMarkerStyle(markerStyle);
    gr->SetMarkerColor(markerColor);
    gr->SetMarkerSize(2);

    gr->SetLineStyle(kDashed);
    gr->SetLineColor(markerColor);

    leg->AddEntry(gr, label.c_str(), "pl");
    return gr;
}

TGraph * make_graph_eff_pur(std::string fname, std::vector<Int_t> v_selN, Style_t markerStyle, Color_t markerColor, TLegend *leg, std::string label)
{
    TFile *fin = new TFile(fname.c_str());
    TGraph *gr = new TGraph();
 
    for (Int_t selN : v_selN) {
        TH1D *h = (TH1D *) fin->Get(Form("heff_sel%d", selN));

        Float_t totalFromB = h->GetBinContent(1, 1) + h->GetBinContent(1, 2);
        Float_t totalNotFromB = h->GetBinContent(2, 1) + h->GetBinContent(2, 2);

        Float_t passFromB = h->GetBinContent(1, 1);
        Float_t passNotFromB = h->GetBinContent(2, 1);

        Float_t x = passFromB / totalFromB; // efficiency
        Float_t y = passFromB / (passFromB + passNotFromB); // purity 

        gr->SetPoint(gr->GetN(), x, y);
    }

    gr->SetMarkerStyle(markerStyle);
    gr->SetMarkerColor(markerColor);
    gr->SetMarkerSize(2);
    gr->SetLineWidth(2);

    gr->SetLineStyle(kDashed);
    gr->SetLineColor(markerColor);

    if (label != "") {
        leg->AddEntry(gr, label.c_str(), "pl");
    }
    return gr;
}
