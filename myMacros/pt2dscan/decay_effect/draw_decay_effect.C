#include "../HistDrawer_pt2dscan.h"

void draw_decay_effect(bool GSPincl = false)
{          
    const Int_t Ncolors = 1000;

    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }

    std::string chargedSJ = "chargedSJ_new";
    std::string aggregateB = "aggregateB_truthInfo_fixedBugs";

    std::string fname_chargedSJ = "~/rootFiles/" + chargedSJ + noGSP + "_ref.root";
    std::string fname_aggregateB = "~/rootFiles/" + aggregateB + noGSP + "_ref.root";

    HistLoader_pt2dscan HL_chargedSJ(fname_chargedSJ, "hB_rgkt"); 
    HistLoader_pt2dscan HL_aggregateB(fname_aggregateB, "hB_rgkt"); 

    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {100., 150.};
    Float_t highpt[2] = {200., 250.};

    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};

    TCanvas *cref = new TCanvas("cref", "cref", 1800, 450);
    // Divide into 3 pads

    cref->cd();
    TPad *pad31 = new TPad("pad31", "pad31", 0., 0., 0.33, 1.);
    pad31->SetRightMargin(0.);
    pad31->SetGrid(1);
    pad31->Draw();

    cref->cd();
    TPad *pad32 = new TPad("pad32", "pad32", 0.33, 0., 0.615, 1.);
    pad32->SetLeftMargin(0.);
    pad32->SetRightMargin(0.);
    pad32->SetGrid(1);
    pad32->Draw();

    cref->cd();
    TPad *pad33 = new TPad("pad33", "pad33", 0.615, 0., 1., 1.);
    pad33->SetLeftMargin(0.);
    pad33->SetRightMargin(0.275);
    pad33->SetGrid(1);
    pad33->Draw();

    TPad *padsRatio[npt] = {pad31, pad32, pad33};

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        std::string xtitle = "";
        if (i == 2) xtitle = "ln(1/R_{g})";
        std::string ytitle = "";
        if (i == 0) ytitle = "ln(k_{T})";
        std::string ztitleRatio = "N_{#it{b-}jets} / N_{#it{b-}jets with aggregated pseudo#it{-B}'s}";
        std::string title = "#it{b-}jets / #it{b-}jets with aggregated pseudo#it{-B}'s, hadron level";


        TH2D *h2d_chargedSJ = (TH2D *) HL_chargedSJ.do_2d_projection("yx", ptrange[i], "h2d_chargedSJ", xtitle, ytitle);
        TH2D *h2d_aggregateB = (TH2D *) HL_aggregateB.do_2d_projection("yx", ptrange[i], "h2d_aggregateB", xtitle, ytitle);

        Float_t xmin = 0.91;
        Float_t xmax = 4.99;

        TPaveText *info = new TPaveText(2., 2., 4.9, 4.5);
        info->AddText(title.c_str());
        info->AddText(Form("%.0f < #it{p_{T}^{jet}} < %.0f (GeV)", ptmin, ptmax));
        info->AddText("-2 < #it{#eta^{jet}} < 2");
        //info->AddText("#it{b-}jets / #it{b-}jets with aggregated pseudo#it{-B}'s");
        info->SetFillColor(0);
        info->SetFillStyle(0);
        info->SetBorderSize(0);
        info->SetTextSize(15);

        padsRatio[i]->cd();
        padsRatio[i]->SetLogz();

        TH2D *hratio = (TH2D *) h2d_chargedSJ->Clone();
        hratio->SetName("hratio");
        hratio->Divide(h2d_aggregateB);
        hratio->GetXaxis()->SetTitle(xtitle.c_str());
        hratio->GetXaxis()->SetTitleOffset(1.5);
        hratio->GetYaxis()->SetTitle(ytitle.c_str());

        Float_t zminRatio = 0.;
        Float_t zmaxRatio = 10.;

        hratio->GetXaxis()->SetLimits(xmin, xmax);
        hratio->GetZaxis()->SetRangeUser(zminRatio, zmaxRatio);

         hratio->SetContour(Ncolors);
        hratio->Draw("col");        
        if (i == 2) {
            hratio->GetZaxis()->SetTitle(ztitleRatio.c_str());
            hratio->Draw("colz");
            gPad->Update();
            TPaletteAxis *paletteRatio = (TPaletteAxis *) hratio->GetListOfFunctions()->FindObject("palette");
            paletteRatio->SetX1NDC(0.74);
            paletteRatio->SetX2NDC(0.845);
            paletteRatio->SetY1NDC(0.15);
            paletteRatio->SetY2NDC(0.9);
            gPad->Modified();
            gPad->Update();
        }

        info->Draw();
    } // pt loop

    TPaveText *mcinfo = new TPaveText(0.3, 0.92, 0.6, 0.97, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");

    pad31->cd();
    mcinfo->Draw();

    cref->Draw();
    std::string savename = "decay_eff.png";
    cref->Print(savename.c_str());
}