#include "TH1D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPaveText.h"

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

    THStack *hs_dpt = new THStack("hs_dpt", "");
    THStack *hs_dr = new THStack("hs_dr", "");

    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.85);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    gStyle->SetLegendTextSize(15);

    Float_t ptrange[2] = {100., 150.};

    HistLoader HL_sjt1(finname, "hB_sjt1", 1);
    TH1D *hpt_sjt1 = HL_sjt1.do_dpt_projection(ptrange, "hpt_sjt1");
    hpt_sjt1->SetLineColor(1);
    leg->AddEntry(hpt_sjt1, "leading subjet", "l");
    hs_dpt->Add(hpt_sjt1);

    TH1D *hr_sjt1 = HL_sjt1.do_dr_projection(ptrange, "hr_sjt1");
    hr_sjt1->SetLineColor(1);
    hs_dr->Add(hr_sjt1);

    HistLoader HL_sjt2(finname, "hB_sjt2", 2);
    TH1D *hpt_sjt2 = HL_sjt2.do_dpt_projection(ptrange, "hpt_sjt2");
    hpt_sjt2->SetLineColor(2);
    leg->AddEntry(hpt_sjt2, "subleading subjet", "l");
    hs_dpt->Add(hpt_sjt2);

    TH1D *hr_sjt2 = HL_sjt2.do_dr_projection(ptrange, "hr_sjt2");
    hr_sjt2->SetLineColor(2);
    hs_dr->Add(hr_sjt2);

    std::string x1title = "#Delta p_{T}^{reco-gen}";
    std::string x2title = "#Delta R^{reco-gen}";

    std::string y1title = "1/N dN/d" + x1title;
    std::string y2title = "1/N dN/d" + x2title;

    hs_dpt->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
    hs_dr->SetTitle(Form("; %s; %s", x2title.c_str(), y2title.c_str()));

    TCanvas *c = new TCanvas("c", "", 1800, 800);
    c->Divide(2, 1);

    c->cd(1);
    hs_dpt->Draw("nostack hist");
    leg->Draw();

    c->cd(2);
    hs_dr->Draw("nostack hist");
    leg->Draw();

    TPaveText *info = new TPaveText(0.6, 0.4, 0.8, 0.5, "ndc");
    info->SetFillStyle(0);
    info->SetBorderSize(1);
    info->SetTextSize(15);

    info->AddText(Form("%.0f < p_{T}^{jet} < %.0f", ptrange[0], ptrange[1]));
    info->AddText(noGSP.c_str());

    c->Draw();

    std::string savename = "sjdiff" + noGSP + ".png";
    c->Print(savename.c_str());
}