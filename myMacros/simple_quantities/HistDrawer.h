#include "TH1D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"

#include "HistLoader.h"

// Class to load a 3D histo and create the projections
class HistDrawer
{
    public:
        HistDrawer() {};
        void draw_sjdiff(bool GSPincl);
};

void HistDrawer::draw_sjdiff(bool GSPincl)
{
    std::string noGSP = "";
    if (!GSPincl) {
        noGSP = "_noGSP";
    }
    std::string finname = "~/rootFiles/sjdiff" + noGSP + ".root";

    THStack *hs = new THStack("hs", "");
    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.85);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    gStyle->SetLegendTextSize(15);

    Float_t ptrange[2] = {100., 150.};

    HistLoader HL_sjt1(finname, "hB_sjt1", 1);
    TH1D *hpt_sjt1 = HL_sjt1.do_dpt_projection(ptrange, "hpt_sjt1");
    hpt_sjt1->SetLineColor(1);
    leg->AddEntry(hpt_sjt1, "leading subjet", "l");
    hs->Add(hpt_sjt1);

    HistLoader HL_sjt2(finname, "hB_sjt2", 2);
    TH1D *hpt_sjt2 = HL_sjt2.do_dpt_projection(ptrange, "hpt_sjt2");
    hpt_sjt2->SetLineColor(2);
    leg->AddEntry(hpt_sjt2, "subleading subjet", "l");
    hs->Add(hpt_sjt2);

    TCanvas *c = new TCanvas("c", "", 1000, 800);
    hs->Draw("nostack hist");
    leg->Draw();

    c->Draw();


}