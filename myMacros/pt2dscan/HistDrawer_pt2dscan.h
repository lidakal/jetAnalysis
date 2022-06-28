#include "HistLoader_pt2dscan.h"
#include "myPalette.h"

#include "TH2D.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLine.h"
#include "TPad.h"
#include "TPaletteAxis.h"

class HistDrawer_pt2dscan
{
    public:
        std::string fname;

        // ---- Constructors ----
        HistDrawer_pt2dscan() {};
        HistDrawer_pt2dscan(std::string MCname);

        void draw_pt2dscan(std::string = "Btag", std::string title = "", bool GSPincl = false);
        void draw_pt2dscan_parVSref(std::string = "Btag", std::string title = "", bool GSPincl = false);
		void draw_1d_projection(bool GSPincl, std::string var = "rg", Float_t *ptrange = nullptr, Float_t *ktrange = nullptr);
        void draw_1d_projection_1ptbin(THStack *hs, TLegend *leg, bool GSPincl = false, std::string var = "rg", Float_t *ptrange = nullptr, Float_t *ktrange = nullptr);
};

HistDrawer_pt2dscan::HistDrawer_pt2dscan(std::string MCname)
{
    fname = MCname;
}

void HistDrawer_pt2dscan::draw_pt2dscan(std::string flavour = "Btag", std::string title = "", bool GSPincl = false)
{
    const Int_t Ncolors = 1000;

    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }

    std::string fname_par = "~/rootFiles/" + fname + noGSP + "_par.root";
    std::string fname_ref = "~/rootFiles/" + fname + noGSP + "_ref.root";
    std::string fname_reco = "~/rootFiles/" + fname + noGSP + "_reco.root";

    HistLoader_pt2dscan HL_par(fname_par, Form("h%s_rgkt", flavour.c_str()));
    HistLoader_pt2dscan HL_reco(fname_reco, Form("h%s_rgkt", flavour.c_str())); 
    HistLoader_pt2dscan HL_ref(fname_ref, Form("h%s_rgkt", flavour.c_str())); 

    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {100., 150.};
    Float_t highpt[2] = {200., 250.};

    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    //TCanvas *cpar = new TCanvas("cpar", "cpar", 1600, 600);
    //cpar->SetWindowSize(1800, 800);
    //cpar->Divide(npt, 1, 0., 0.); 
    
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

    TPad *padsRef[npt] = {pad31, pad32, pad33};

    //TCanvas *creco = new TCanvas("creco", "creco", 1800, 600);
    //creco->Divide(npt, 1, 0., 0.); 

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        std::string xtitle = "";
        if (i == 2) xtitle = "ln(1/R_{g})";
        std::string ytitle = "";
        if (i == 0) ytitle = "ln(k_{T})";
        std::string ztitle = "1/N_{2-prong jets} dN^{2}/dln(1/R_{g})dln(k_{T})";
        std::string ztitleRatio = "N_{truth level} / N_{parton level}";

        TH2D *h2d_par = (TH2D *) HL_par.do_2d_projection("yx", ptrange[i], "h2d_par");
        TH2D *h2d_ref = (TH2D *) HL_ref.do_2d_projection("yx", ptrange[i], "h2d_ref", xtitle, ytitle);
        TH2D *h2d_reco = (TH2D *) HL_reco.do_2d_projection("yx", ptrange[i], "h2d_reco");
        
        TLine *line = new TLine(0.91, 0, 5, 0);
        line->SetLineWidth(2);

        TPaveText *info = new TPaveText(0.4, 0.65, 0.75, 0.85, "ndc");
        info->AddText(title.c_str());
        //info->AddLine(0., 0.7, 1., 0.7);
        info->AddText(Form("%.0f < #it{p_{T}^{jet}} < %.0f (GeV)", ptmin, ptmax));
        info->AddText("-2 < #it{#eta^{jet}} < 2");
        info->SetFillColor(0);
        info->SetFillStyle(0);
        info->SetBorderSize(0);
        info->SetTextSize(15);
        
        TPaveText *info_par = (TPaveText *) info->Clone();
        info_par->AddText("parton level");

        TPaveText *info_ref = (TPaveText *) info->Clone();
        info_ref->AddText("truth level");
        
        TPaveText *info_reco = (TPaveText *) info->Clone();
        info_reco->AddText("reco level");
   
        TPaveText *gsptxt = new TPaveText(0.2, 0.25, 0.35, 0.4, "ndc");
        gsptxt->SetFillColor(0);
		gsptxt->SetFillStyle(0);
        gsptxt->SetBorderSize(0);
        gsptxt->SetTextSize(20);
        if (!GSPincl) {
            gsptxt->AddText("NO GSP");
        }

        // c : ref, reco
        Float_t zmin = 0.;
        Float_t zmax = 0.4;

        Float_t xmin = 0.91;
        Float_t xmax = 4.99;

        //cpar->cd(i + 1);
        //cpar->cd(i + 1)->SetGrid();

        //h2d_par->GetZaxis()->SetRangeUser(zmin, zmax);
        //h2d_par->Draw("col");
        //info_par->Draw();

        padsRef[i]->cd();

		h2d_ref->SetContour(Ncolors);
        h2d_ref->GetXaxis()->SetLimits(xmin, xmax);
        //h2d_ref->GetYaxis()->SetRangeUser(ymin, ymax);
		h2d_ref->GetZaxis()->SetRangeUser(zmin, zmax);
        h2d_ref->GetXaxis()->SetTitleOffset(1.3);
        h2d_ref->Draw("col");
        if (i == 2) { 
            h2d_ref->Draw("colz");
        }
        info_ref->Draw();

        //creco->cd(i + 1);
        //creco->cd(i + 1)->SetGrid();
    
		//h2d_reco->GetZaxis()->SetRangeUser(zmin, zmax);
        //h2d_reco->Draw("col");
        //line->Draw();
        //info_reco->Draw();
        //gsptxt->Draw();
    }

    TPaveText *mcinfo = new TPaveText(0.2, 0.9, 0.6, 0.95, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");
    
    //cpar->cd();
    //mcinfo->Draw();
    //cpar->Draw();

    pad31->cd();
    mcinfo->Draw();

    cref->Draw();

    //creco->cd();
    //mcinfo->Draw();
    //creco->Draw();
    
    //std::string savename_cpar = fname + "_" + flavour + "_jets" + noGSP + "_par.png";
    std::string savename_cref = fname + "_" + flavour + "_jets" + noGSP + "_ref.png";
    //std::string savename_creco = fname + "_" + flavour + "_jets" + noGSP + "_reco.png";

    //cpar->Print(savename_cpar.c_str());
    cref->Print(savename_cref.c_str());
    //creco->Print(savename_creco.c_str());
}

void HistDrawer_pt2dscan::draw_pt2dscan_parVSref(std::string flavour = "Btag", std::string title = "", bool GSPincl = false)
{
    const Int_t Ncolors = 1000;

    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }

    std::string fname_par = "~/rootFiles/" + fname + noGSP + "_par.root";
    std::string fname_ref = "~/rootFiles/" + fname + noGSP + "_ref.root";
    std::string fname_reco = "~/rootFiles/" + fname + noGSP + "_reco.root";

    HistLoader_pt2dscan HL_par(fname_par, Form("h%s_rgkt", flavour.c_str()));
    HistLoader_pt2dscan HL_reco(fname_reco, Form("h%s_rgkt", flavour.c_str())); 
    HistLoader_pt2dscan HL_ref(fname_ref, Form("h%s_rgkt", flavour.c_str())); 

    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {100., 150.};
    Float_t highpt[2] = {200., 250.};

    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    TCanvas *c = new TCanvas("c", "c", 1800, 900);
    // Divide canvas into 7 pads

    // Pad to display the colorbar (palette)
    c->cd();
    TPad *pad = new TPad("pad", "pad", 0.6, 0., 1., 1.);
    pad->SetTopMargin(0.);
    pad->SetBottomMargin(0.);
    pad->SetRightMargin(0.5);
    pad->Draw();

    c->cd(); 
    TPad *pad11 = new TPad("pad11", "pad11", 0., 0.5, 0.33, 1.);
    pad11->SetBottomMargin(0.);
    pad11->SetRightMargin(0.);
    pad11->SetGrid(1);
    pad11->Draw();

    c->cd();
    TPad *pad12 = new TPad("pad12", "pad12", 0.33, 0.5, 0.615, 1.);
    pad12->SetBottomMargin(0.);
    pad12->SetLeftMargin(0.);
    pad12->SetRightMargin(0.);
    pad12->SetGrid(1);
    pad12->Draw();

    c->cd();
    TPad *pad13 = new TPad("pad13", "pad13", 0.615, 0.5, 0.9, 1.);
    pad13->SetBottomMargin(0.);
    pad13->SetLeftMargin(0.);
    pad13->SetRightMargin(0.02);
    pad13->SetGrid(1);
    pad13->Draw();

    c->cd();
    TPad *pad21 = new TPad("pad21", "pad21", 0., 0., 0.33, 0.5);
    pad21->SetTopMargin(0.);
    pad21->SetRightMargin(0.);
    pad21->SetGrid(1);
    pad21->Draw();

    c->cd();
    TPad *pad22 = new TPad("pad22", "pad22", 0.33, 0., 0.615, 0.5);
    pad22->SetTopMargin(0.);
    pad22->SetLeftMargin(0.);
    pad22->SetRightMargin(0.);
    pad22->SetGrid(1);
    pad22->Draw();

    c->cd();
    TPad *pad23 = new TPad("pad23", "pad23", 0.615, 0., 0.9, 0.5);
    pad23->SetTopMargin(0.);
    pad23->SetLeftMargin(0.);
    pad23->SetRightMargin(0.02);
    pad23->SetGrid(1);
    pad23->Draw();

    TPad *padsTop[npt] = {pad11, pad12, pad13};
    TPad *padsBottom[npt] = {pad21, pad22, pad23};

    // Another canvas for the ratio 
    TCanvas *cRatio = new TCanvas("cRatio", "cRatio", 1800, 450);
    // Divide into 3 pads

    cRatio->cd();
    TPad *pad31 = new TPad("pad31", "pad31", 0., 0., 0.33, 1.);
    pad31->SetRightMargin(0.);
    pad31->SetGrid(1);
    pad31->Draw();

    cRatio->cd();
    TPad *pad32 = new TPad("pad32", "pad32", 0.33, 0., 0.615, 1.);
    pad32->SetLeftMargin(0.);
    pad32->SetRightMargin(0.);
    pad32->SetGrid(1);
    pad32->Draw();

    cRatio->cd();
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
        std::string ztitle = "1/N_{2-prong jets} dN^{2}/dln(1/R_{g})dln(k_{T})";
        std::string ztitleRatio = "N_{truth level} / N_{parton level}";

        TH2D *h2d_par = (TH2D *) HL_par.do_2d_projection("yx", ptrange[i], "h2d_par", "", ytitle);
        TH2D *h2d_ref = (TH2D *) HL_ref.do_2d_projection("yx", ptrange[i], "h2d_ref", xtitle, "");

        //TPaveText *info = new TPaveText(0.6, 0.65, 0.85, 0.85, "ndc");
        TPaveText *info = new TPaveText(3., 2., 4.5, 4.5);
        info->AddText(title.c_str());
        info->AddText(Form("%.0f < #it{p_{T}^{jet}} < %.0f (GeV)", ptmin, ptmax));
        info->AddText("-2 < #it{#eta^{jet}} < 2");
        info->SetFillColor(0);
        info->SetFillStyle(0);
        info->SetBorderSize(0);
        info->SetTextSize(15);
        
        TPaveText *info_par = (TPaveText *) info->Clone();
        info_par->AddText("parton level");

        TPaveText *info_ref = (TPaveText *) info->Clone();
        info_ref->AddText("truth level");

        TPaveText *info_ratio = (TPaveText *) info->Clone();
        info_ratio->AddText("truth / parton level");

        Float_t xmin = 0.91;
        Float_t xmax = 4.99;

        Float_t ymin = -5.;
        Float_t ymax = 5.;

        Float_t zmin = 0.;
        Float_t zmax = 0.4;

        padsTop[i]->cd();
        h2d_par->SetContour(Ncolors);
        h2d_par->GetXaxis()->SetLimits(xmin, xmax);
        h2d_par->GetYaxis()->SetTitleOffset(2.);
        h2d_par->GetZaxis()->SetRangeUser(zmin, zmax);
        h2d_par->Draw("col");
        info_par->Draw();

        padsBottom[i]->cd();
        h2d_ref->SetContour(Ncolors);
        h2d_ref->GetXaxis()->SetLimits(xmin, xmax);
        //h2d_ref->GetYaxis()->SetRangeUser(ymin, ymax);
		h2d_ref->GetZaxis()->SetRangeUser(zmin, zmax);
        h2d_ref->Draw("col");
        info_ref->Draw();
        
        pad->cd();
        TH2D *hpal = (TH2D *) h2d_ref->Clone();
        hpal->SetName("hpal");
        hpal->SetContour(Ncolors);
        hpal->GetZaxis()->SetTitle(ztitle.c_str());
        hpal->GetZaxis()->SetTitleOffset(2.5);
        hpal->Draw("colz");
        gPad->Update();
        TPaletteAxis *palette = (TPaletteAxis *) hpal->GetListOfFunctions()->FindObject("palette");
        palette->SetX1NDC(0.7);
        palette->SetX2NDC(0.85);
        palette->SetY1NDC(0.075);
        palette->SetY2NDC(0.95);
        gPad->Modified();
        gPad->Update();

        // Ratio histogram
        padsRatio[i]->cd();
        padsRatio[i]->SetLogz();

        TH2D *hratio = (TH2D *) h2d_ref->Clone();
        hratio->SetName("hratio");
        hratio->Divide(h2d_par);
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

        info_ratio->Draw();
    } // end pt loop

    TPaveText *mcinfo = new TPaveText(0.3, 0.92, 0.6, 0.97, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");
    
    pad11->cd();
    mcinfo->Draw();

    pad31->cd();
    mcinfo->Draw();

    c->Draw();
    cRatio->Draw();

    std::string savename_c = fname + "_" + flavour + "_jets" + noGSP + "_parVSref.png";
    std::string savename_cRatio = fname + "_" + flavour + "_jets" + noGSP + "_parVSref_ratio.png";
    c->Print(savename_c.c_str());
    cRatio->Print(savename_cRatio.c_str());
}

void HistDrawer_pt2dscan::draw_1d_projection(bool GSPincl, std::string var = "rg", Float_t *ptrange = nullptr, Float_t *ktrange = nullptr)
{
    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }

    Float_t ptmin = 30., ptmax = 300.;
    Float_t ktmin = -5., ktmax = 5.;

    if (ptrange != nullptr) {
        ptmin = ptrange[0]; ptmax = ptrange[1];
    }

    if (ktrange != nullptr) {
        ktmin = ktrange[0]; ktmax = ktrange[1];
    }
    
    // These do not change
    std::string fname_chargedSJ_qcdMC_ref = "~/rootFiles/chargedSJ_new_qcdMC_ref.root"; // ljet truth
    std::string fname_chargedSJ_qcdMC_reco = "~/rootFiles/chargedSJ_new_qcdMC_reco.root"; // ljet reco

    std::string fname_Cmerged_ref = "~/rootFiles/aggregateC_truthInfo" + noGSP + "_ref.root"; // cjet truth merge
    std::string fname_chargedSJ_cjets_ref = "~/rootFiles/chargedSJ_new_qcdMC" + noGSP + "_ref.root"; // cjet truth merge

	std::string fname_merged_ref = "~/rootFiles/aggregateB_truthInfo_fixedBugs" + noGSP + "_ref.root"; // bjet truth merge
    std::string fname_merged_truth_reco = "~/rootFiles/aggregateB_truthInfo_fixedBugs" + noGSP + "_reco.root"; // bjet reco merge with truth info
    std::string fname_chargedSJ_bJetMC_reco = "~/rootFiles/chargedSJ_new_bJetMC" + noGSP + "_reco.root"; // bjet reco no merge
    std::string fname_chargedSJ_bJetMC_ref = "~/rootFiles/chargedSJ_new_bJetMC" + noGSP + "_ref.root"; // bjet ref no merge

    // New selections can be added here
    std::string fname_merged_reco = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco" + noGSP + "_reco.root"; // bjet reco merge only SV
    std::string fname_merged_highestEfficiency_reco = "~/rootFiles/aggregateB_highestEfficiency" + noGSP + "_reco.root"; // bjet reco merge (inSV OR (|3D IP/#sigma| > 3))
    std::string fname_merged_ip3dSigOnly_reco = "~/rootFiles/aggregateB_ip3dSigOnly" + noGSP + "_reco.root"; // bjet reco merge (|3D IP/#sigma| > 3)
    std::string fname_merged_SV_ip3dSig_tight_reco = "~/rootFiles/aggregateB_SV_ip3dSig_tight" + noGSP + "_reco.root"; // bjet reco merge ((inSV AND (|3D IP/#sigma| > 3)) OR (|3D IP/#sigma| > 9)) 

    THStack *hs = new THStack("hs", "");

    TLegend *leg = new TLegend(0.5, 0.75, 0.95, 0.9);
    leg->SetBorderSize(1);
    gStyle->SetLegendTextSize(15);
    //leg->SetHeader("Track aggregation criteria:");
    leg->SetMargin(0.1);

    //TLegendEntry *header = (TLegendEntry*) leg->GetListOfPrimitives()->First();
    //header->SetTextAlign(22);
        //header->SetTextColor(2);
    //header->SetTextSize(17);

    std::string xtitle;
    if (var == "rg") {
        xtitle = "ln(1/R_{g})";
    } else if (var == "kt") {
        xtitle = "ln(k_{T})";
    } else if (var == "zg") {
        xtitle = "z_{g}";
    }
    std::string ytitle = "1/N_{2-prong jets} dN/d" + xtitle;
    std::string y2title = "N_{reconstruction level} / N_{hadron level}";

    HistLoader_pt2dscan HL_ljet_ref(fname_chargedSJ_qcdMC_ref, Form("hL_%skt", var.c_str()));
    TH1D *h1d_ljet_ref = HL_ljet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_ljet_ref", xtitle, ytitle);
    h1d_ljet_ref->SetLineColor(mykRed);
    h1d_ljet_ref->SetLineWidth(2);
    h1d_ljet_ref->SetLineStyle(1);
    //leg->AddEntry(h1d_ljet_ref, "light-jets", "l");
    //hs->Add(h1d_ljet_ref);

    HistLoader_pt2dscan HL_ljet_reco(fname_chargedSJ_qcdMC_reco, Form("hL_%skt", var.c_str()));
    TH1D *h1d_ljet_reco = HL_ljet_reco.do_1d_projection("x", ptrange, ktrange, "h1d_ljet_reco", xtitle, ytitle);
    h1d_ljet_reco->SetLineColor(kGray);
    h1d_ljet_reco->SetLineStyle(1);
    //leg->AddEntry(h1d_ljet_reco, "l-jets, reco", "l");
    //hs->Add(h1d_ljet_reco);

    HistLoader_pt2dscan HL_cjet_ref(fname_Cmerged_ref, Form("hC_%skt", var.c_str()));
    TH1D *h1d_cjet_ref = HL_cjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_cjet_ref", xtitle, ytitle);
    h1d_cjet_ref->SetLineColor(mykGreen);
    h1d_cjet_ref->SetLineWidth(2);
    h1d_cjet_ref->SetLineStyle(1);
    //leg->AddEntry(h1d_cjet_ref, "#it{c-}jets with aggregated pseudo#it{-D}'s", "l");
    //hs->Add(h1d_cjet_ref);

    HistLoader_pt2dscan HL_chargedSJ_cjet_ref(fname_chargedSJ_cjets_ref, Form("hC_%skt", var.c_str()));
    TH1D *h1d_chargedSJ_cjet_ref = HL_chargedSJ_cjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_chargedSJ_cjet_ref", xtitle, ytitle);
    h1d_chargedSJ_cjet_ref->SetLineColor(mykGreenLight);
    h1d_chargedSJ_cjet_ref->SetLineWidth(2);
    h1d_chargedSJ_cjet_ref->SetLineStyle(2);
    //leg->AddEntry(h1d_chargedSJ_cjet_ref, "#it{c-}jets", "l");
    //hs->Add(h1d_chargedSJ_cjet_ref);

    HistLoader_pt2dscan HL_merged_bjet_ref(fname_merged_ref, Form("hB_%skt", var.c_str()));
    TH1D *h1d_bjet_merged_ref = HL_merged_bjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_merged_ref", xtitle, ytitle);
    h1d_bjet_merged_ref->SetLineColor(mykBlue);
    h1d_bjet_merged_ref->SetLineWidth(2);
    h1d_bjet_merged_ref->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_merged_ref, "#it{b}-jets with aggregated pseudo#it{-B}'s at truth level", "l");
    hs->Add(h1d_bjet_merged_ref);

    HistLoader_pt2dscan HL_bjet_ref(fname_chargedSJ_bJetMC_ref, Form("hB_%skt", var.c_str()));
    TH1D *h1d_bjet_ref = HL_bjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_ref", xtitle, ytitle);
    h1d_bjet_ref->SetLineColor(mykBlueLight);
    h1d_bjet_ref->SetLineWidth(2);
    h1d_bjet_ref->SetLineStyle(2);
    //leg->AddEntry(h1d_bjet_ref, "#it{b-}jets", "l");
    //hs->Add(h1d_bjet_ref);

    HistLoader_pt2dscan HL_bjet_reco_truth(fname_merged_truth_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_truth = HL_bjet_reco_truth.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_truth", xtitle, ytitle);
    h1d_bjet_reco_truth->SetLineColor(mykBlueLight);
    h1d_bjet_reco_truth->SetLineWidth(2);
    h1d_bjet_reco_truth->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_truth, "#it{b}-jets with aggregated pseudo#it{-B}'s at reconstruction level", "l");
    hs->Add(h1d_bjet_reco_truth);

    HistLoader_pt2dscan HL_bjet_reco(fname_chargedSJ_bJetMC_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco = HL_bjet_reco.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco", xtitle, ytitle);
    h1d_bjet_reco->SetLineColor(mykRed);
    h1d_bjet_reco->SetLineWidth(2);
    h1d_bjet_reco->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco, "No aggregation", "l");
    //hs->Add(h1d_bjet_reco);

    HistLoader_pt2dscan HL_bjet_reco_SV(fname_merged_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_SV = HL_bjet_reco_SV.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_SV", xtitle, ytitle);
    h1d_bjet_reco_SV->SetLineColor(mykGreenLight);
    h1d_bjet_reco_SV->SetLineWidth(2);
    h1d_bjet_reco_SV->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_SV, "inSV", "l");
    //hs->Add(h1d_bjet_reco_SV);    

    HistLoader_pt2dscan HL_bjet_reco_ip3dSigOnly(fname_merged_ip3dSigOnly_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_ip3dSigOnly = HL_bjet_reco_ip3dSigOnly.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_ip3dSigOnly", xtitle, ytitle);
    h1d_bjet_reco_ip3dSigOnly->SetLineColor(mykMagentaLight);
    h1d_bjet_reco_ip3dSigOnly->SetLineWidth(2);
    h1d_bjet_reco_ip3dSigOnly->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_ip3dSigOnly, "|3D IP/#sigma| > 3", "l");
    //hs->Add(h1d_bjet_reco_ip3dSigOnly); 

    HistLoader_pt2dscan HL_bjet_reco_highestEfficiency(fname_merged_highestEfficiency_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_highestEfficiency = HL_bjet_reco_highestEfficiency.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_highestEfficiency", xtitle, ytitle);
    h1d_bjet_reco_highestEfficiency->SetLineColor(mykOrangeLight);
    h1d_bjet_reco_highestEfficiency->SetLineWidth(2);
    h1d_bjet_reco_highestEfficiency->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_highestEfficiency, "inSV OR |3D IP/#sigma| > 3", "l");
    //hs->Add(h1d_bjet_reco_highestEfficiency);

    HistLoader_pt2dscan HL_bjet_reco_SV_ip3dSig_tight(fname_merged_SV_ip3dSig_tight_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_SV_ip3dSig_tight = HL_bjet_reco_SV_ip3dSig_tight.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_SV_ip3dSig_tight", xtitle, ytitle);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineColor(mykCyanLight);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineWidth(2);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_SV_ip3dSig_tight, "(inSV AND |3D IP/#sigma| > 3) OR |3D IP/#sigma| > 9", "l");
    //hs->Add(h1d_bjet_reco_SV_ip3dSig_tight);     

	hs->SetTitle(Form("; %s; %s", xtitle.c_str(), ytitle.c_str()));

    // Create ratio plot for truth aggr vs reco aggr with truth info
    TH1D *hratio = (TH1D *) h1d_bjet_reco_truth->Clone();
    hratio->SetName("hratio");
    hratio->Divide(h1d_bjet_merged_ref);
    hratio->SetTitle(Form("; %s; %s", xtitle.c_str(), y2title.c_str()));
    hratio->SetLineColor(mykBlue);

    TCanvas *c = new TCanvas("c", "", 1800, 600);
    // Divide canvas into 2 pads
    c->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0., 0., 0.5, 1.);
    pad1->SetRightMargin(0.05);
    pad1->SetGrid(1);
    pad1->Draw();

    c->cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0.5, 0., 1., 1.);
    pad2->SetLeftMargin(0.12);
    pad2->SetGrid(1);
    pad2->Draw();

    pad1->cd();
    hs->Draw("nostack hist");
    if ((ktmin >= 0) && (var == "rg")) {
        hs->GetXaxis()->SetLimits(0.91, 3.5);
    }
    leg->Draw();

    TPaveText *info = new TPaveText(0.55, 0.45, 0.85, 0.58, "ndc");
    info->SetBorderSize(0);
    info->SetFillColor(0);
    info->SetFillStyle(0);
    info->SetTextSize(15);

    info->AddText("#it{b-}tagged true #it{b-}jets");
    info->AddText(Form("%.0f < #it{p_{T}^{jet}} < %0.f (GeV)", ptmin, ptmax));
    info->AddText("-2 < #it{#eta^{jet}} < 2");
    //info->AddText("Reconstructed #it{b}-tagged #it{b}-jets");
    //info->AddText("that originate from #it{b} quarks");
    if (!GSPincl) {
	  //info->AddText("truth doesn't pass b-tag, NO GSP");
      //info->AddText("excluding #it{g#rightarrow b#bar{b}} inside the jet");
    }
	//info->AddText(Form("%0.f < ln(k_{T}) < %.0f", ktmin, ktmax));
    info->Draw();

    TPaveText *mcinfo = new TPaveText(0.23, 0.9, 0.4, 0.95, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");
    mcinfo->Draw();

    TPaveText *inforatio = new TPaveText(0.6, 0.65, 0.8, 0.8, "ndc");
    inforatio->SetBorderSize(0);
    inforatio->SetFillColor(0);
    inforatio->SetFillStyle(0);
    inforatio->SetTextSize(15);

    inforatio->AddText("#it{b-}tagged true #it{b-}jets");
    inforatio->AddText(Form("%.0f < #it{p_{T}^{jet}} < %0.f (GeV)", ptmin, ptmax));
    inforatio->AddText("-2 < #it{#eta^{jet}} < 2");

    pad2->cd();
    hratio->Draw("hist");
    inforatio->Draw();

    c->Draw();

	std::string savename = var + "_projections_1ptBin" + noGSP + ".png";
	c->Print(savename.c_str());
}

void HistDrawer_pt2dscan::draw_1d_projection_1ptbin(THStack *hs, TLegend *leg, bool GSPincl = false, std::string var = "rg", Float_t *ptrange = nullptr, Float_t *ktrange = nullptr)
{
    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }

    Float_t ptmin = 30., ptmax = 300.;
    Float_t ktmin = -5., ktmax = 5.;

    if (ptrange != nullptr) {
        ptmin = ptrange[0]; ptmax = ptrange[1];
    }

    if (ktrange != nullptr) {
        ktmin = ktrange[0]; ktmax = ktrange[1];
    }
    
    // These do not change
    std::string fname_chargedSJ_qcdMC_ref = "~/rootFiles/chargedSJ_new_qcdMC_ref.root"; // ljet truth
    std::string fname_chargedSJ_qcdMC_reco = "~/rootFiles/chargedSJ_new_qcdMC_reco.root"; // ljet reco

    std::string fname_Cmerged_ref = "~/rootFiles/aggregateC_truthInfo" + noGSP + "_ref.root"; // cjet truth merge
    std::string fname_chargedSJ_cjets_ref = "~/rootFiles/chargedSJ_new_qcdMC" + noGSP + "_ref.root"; // cjet truth merge

	std::string fname_merged_ref = "~/rootFiles/aggregateB_truthInfo_fixedBugs" + noGSP + "_ref.root"; // bjet truth merge
    std::string fname_merged_truth_reco = "~/rootFiles/aggregateB_truthInfo_fixedBugs" + noGSP + "_reco.root"; // bjet reco merge with truth info
    std::string fname_chargedSJ_bJetMC_reco = "~/rootFiles/chargedSJ_new_bJetMC" + noGSP + "_reco.root"; // bjet reco no merge
    std::string fname_chargedSJ_bJetMC_ref = "~/rootFiles/chargedSJ_new_bJetMC" + noGSP + "_ref.root"; // bjet ref no merge

    // New selections can be added here
    std::string fname_merged_reco = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco" + noGSP + "_reco.root"; // bjet reco merge only SV
    std::string fname_merged_highestEfficiency_reco = "~/rootFiles/aggregateB_highestEfficiency" + noGSP + "_reco.root"; // bjet reco merge (inSV OR (|3D IP/#sigma| > 3))
    std::string fname_merged_ip3dSigOnly_reco = "~/rootFiles/aggregateB_ip3dSigOnly" + noGSP + "_reco.root"; // bjet reco merge (|3D IP/#sigma| > 3)
    std::string fname_merged_SV_ip3dSig_tight_reco = "~/rootFiles/aggregateB_SV_ip3dSig_tight" + noGSP + "_reco.root"; // bjet reco merge ((inSV AND (|3D IP/#sigma| > 3)) OR (|3D IP/#sigma| > 9)) 

    std::string xtitle = "";
    std::string ytitle = "";

    HistLoader_pt2dscan HL_ljet_ref(fname_chargedSJ_qcdMC_ref, Form("hL_%skt", var.c_str()));
    TH1D *h1d_ljet_ref = HL_ljet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_ljet_ref", xtitle, ytitle);
    h1d_ljet_ref->SetLineColor(mykRed);
    h1d_ljet_ref->SetLineWidth(2);
    h1d_ljet_ref->SetLineStyle(1);
    leg->AddEntry(h1d_ljet_ref, "light-jets", "l");
    hs->Add(h1d_ljet_ref);

    HistLoader_pt2dscan HL_ljet_reco(fname_chargedSJ_qcdMC_reco, Form("hL_%skt", var.c_str()));
    TH1D *h1d_ljet_reco = HL_ljet_reco.do_1d_projection("x", ptrange, ktrange, "h1d_ljet_reco", xtitle, ytitle);
    h1d_ljet_reco->SetLineColor(kGray);
    h1d_ljet_reco->SetLineStyle(1);
    //leg->AddEntry(h1d_ljet_reco, "l-jets, reco", "l");
    //hs->Add(h1d_ljet_reco);

    HistLoader_pt2dscan HL_chargedSJ_cjet_ref(fname_chargedSJ_cjets_ref, Form("hC_%skt", var.c_str()));
    TH1D *h1d_chargedSJ_cjet_ref = HL_chargedSJ_cjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_chargedSJ_cjet_ref", xtitle, ytitle);
    h1d_chargedSJ_cjet_ref->SetLineColor(mykGreenLight);
    h1d_chargedSJ_cjet_ref->SetLineWidth(2);
    h1d_chargedSJ_cjet_ref->SetLineStyle(2);
    leg->AddEntry(h1d_chargedSJ_cjet_ref, "#it{c-}jets", "l");
    hs->Add(h1d_chargedSJ_cjet_ref);

    HistLoader_pt2dscan HL_cjet_ref(fname_Cmerged_ref, Form("hC_%skt", var.c_str()));
    TH1D *h1d_cjet_ref = HL_cjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_cjet_ref", xtitle, ytitle);
    h1d_cjet_ref->SetLineColor(mykGreen);
    h1d_cjet_ref->SetLineWidth(2);
    h1d_cjet_ref->SetLineStyle(1);
    leg->AddEntry(h1d_cjet_ref, "#it{c-}jets with aggregated pseudo#it{-D}'s", "l");
    hs->Add(h1d_cjet_ref);

    HistLoader_pt2dscan HL_bjet_ref(fname_chargedSJ_bJetMC_ref, Form("hB_%skt", var.c_str()));
    TH1D *h1d_bjet_ref = HL_bjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_ref", xtitle, ytitle);
    h1d_bjet_ref->SetLineColor(mykBlueLight);
    h1d_bjet_ref->SetLineWidth(2);
    h1d_bjet_ref->SetLineStyle(2);
    leg->AddEntry(h1d_bjet_ref, "#it{b-}jets", "l");
    hs->Add(h1d_bjet_ref);

    HistLoader_pt2dscan HL_merged_bjet_ref(fname_merged_ref, Form("hB_%skt", var.c_str()));
    TH1D *h1d_bjet_merged_ref = HL_merged_bjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_merged_ref", xtitle, ytitle);
    h1d_bjet_merged_ref->SetLineColor(mykBlue);
    h1d_bjet_merged_ref->SetLineWidth(2);
    h1d_bjet_merged_ref->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_merged_ref, "#it{b-}jets with aggregated pseudo#it{-B}'s", "l");
    hs->Add(h1d_bjet_merged_ref);

    HistLoader_pt2dscan HL_bjet_reco_truth(fname_merged_truth_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_truth = HL_bjet_reco_truth.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_truth", xtitle, ytitle);
    h1d_bjet_reco_truth->SetLineColor(mykBlue);
    h1d_bjet_reco_truth->SetLineWidth(2);
    h1d_bjet_reco_truth->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_truth, "Truth level information (IDEAL CASE)", "l");
    //hs->Add(h1d_bjet_reco_truth);

    HistLoader_pt2dscan HL_bjet_reco(fname_chargedSJ_bJetMC_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco = HL_bjet_reco.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco", xtitle, ytitle);
    h1d_bjet_reco->SetLineColor(mykRed);
    h1d_bjet_reco->SetLineWidth(2);
    h1d_bjet_reco->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco, "No aggregation", "l");
    //hs->Add(h1d_bjet_reco);

    HistLoader_pt2dscan HL_bjet_reco_SV(fname_merged_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_SV = HL_bjet_reco_SV.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_SV", xtitle, ytitle);
    h1d_bjet_reco_SV->SetLineColor(mykGreenLight);
    h1d_bjet_reco_SV->SetLineWidth(2);
    h1d_bjet_reco_SV->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_SV, "inSV", "l");
    //hs->Add(h1d_bjet_reco_SV);    

    HistLoader_pt2dscan HL_bjet_reco_ip3dSigOnly(fname_merged_ip3dSigOnly_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_ip3dSigOnly = HL_bjet_reco_ip3dSigOnly.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_ip3dSigOnly", xtitle, ytitle);
    h1d_bjet_reco_ip3dSigOnly->SetLineColor(mykMagentaLight);
    h1d_bjet_reco_ip3dSigOnly->SetLineWidth(2);
    h1d_bjet_reco_ip3dSigOnly->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_ip3dSigOnly, "|3D IP/#sigma| > 3", "l");
    //hs->Add(h1d_bjet_reco_ip3dSigOnly); 

    HistLoader_pt2dscan HL_bjet_reco_highestEfficiency(fname_merged_highestEfficiency_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_highestEfficiency = HL_bjet_reco_highestEfficiency.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_highestEfficiency", xtitle, ytitle);
    h1d_bjet_reco_highestEfficiency->SetLineColor(mykOrangeLight);
    h1d_bjet_reco_highestEfficiency->SetLineWidth(2);
    h1d_bjet_reco_highestEfficiency->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_highestEfficiency, "inSV OR |3D IP/#sigma| > 3", "l");
    //hs->Add(h1d_bjet_reco_highestEfficiency);

    HistLoader_pt2dscan HL_bjet_reco_SV_ip3dSig_tight(fname_merged_SV_ip3dSig_tight_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_SV_ip3dSig_tight = HL_bjet_reco_SV_ip3dSig_tight.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_SV_ip3dSig_tight", xtitle, ytitle);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineColor(mykCyanLight);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineWidth(2);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_reco_SV_ip3dSig_tight, "(inSV AND |3D IP/#sigma| > 3) OR |3D IP/#sigma| > 9", "l");
    //hs->Add(h1d_bjet_reco_SV_ip3dSig_tight);     
}