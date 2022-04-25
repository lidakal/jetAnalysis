#include "TH3F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"
#include "TStyle.h"
#include "../../utils.h"
#include <regex>
#include <iostream>

using namespace std;

void draw_decay_effect_reco(bool chargedSJ = true, bool GSPincl = true)
{
    /* Only charged SJ */
    chargedSJ = true;

    cout << "Running with options : " << endl;
    cout << "chargedSJ : " << chargedSJ << endl;
    cout << "GSPincl : " << GSPincl << endl;
    
    cout << "\n" << endl;
    
    //string decayed_fname = "";
    //string undecayed_fname = "";
    string charged_decayed_fname = "";
    string charged_undecayed_fname = "";    

    string savename_c = "";

    if (GSPincl) {
        //decayed_fname += "~/rootFiles/SD_ref.root";
        //undecayed_fname += "~/rootFiles/mergedB_ref.root";
        charged_decayed_fname += "~/rootFiles/chargedSJ_ref.root";
        charged_undecayed_fname += "~/rootFiles/aggregatedB_truthInfo_reco.root";    
    } else {
        //decayed_fname += "~/rootFiles/SD_noGSP_ref.root";
        //undecayed_fname += "~/rootFiles/mergedB_noGSP_ref.root";
        charged_decayed_fname += "~/rootFiles/chargedSJ_noGSP_ref.root";
        charged_undecayed_fname += "~/rootFiles/aggregatedB_truthInfo_noGSP_reco.root";    
        savename_c += "noGSP_";
    }


    TFile *fin_decayed;
    TFile *fin_undecayed;

    // Load ref 3D histograms -- X = rg, Y = kt, Z = pt 
    //if (chargedSJ) {
        fin_decayed = new TFile(charged_decayed_fname.c_str());
        fin_undecayed = new TFile(charged_undecayed_fname.c_str());
        savename_c += "chargedSJ_dec_vs_nodec";
    //} else {
    //    fin_decayed = new TFile(decayed_fname.c_str());
    //    fin_undecayed = new TFile(undecayed_fname.c_str());
    //    savename_c += "SD_dec_vs_nodec_mergedB";
    //}

    string savename_c_dynKt = savename_c + "_dynKt.png";
    string savename_c_ratio = savename_c + "_ratio";
    savename_c += ".png";

    // decayed = normal final state particles
    TH3D *h3d_decayed = (TH3D *) fin_decayed->Get("hB_rgkt")->Clone();
    h3d_decayed->SetName("decayed");
    TH3D *h3d_decayed_dynKt = (TH3D *) fin_decayed->Get("hB_rgkt_dynKt")->Clone();
    h3d_decayed_dynKt->SetName("decayed_dynKt");
    // undecayed = clustered B decay products
    TH3D *h3d_undecayed = (TH3D *) fin_undecayed->Get("hB_rgkt")->Clone();
    h3d_undecayed->SetName("undecayed");
    TH3D *h3d_undecayed_dynKt = (TH3D *) fin_undecayed->Get("hB_rgkt_dynKt")->Clone();
    h3d_undecayed_dynKt->SetName("undecayed_dynKt");

    // Create / Draw 2D histograms - Projections
    const int npt = 3;

    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};
    
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};

    string xtitle = "ln(1/R_{g})";
    string ytitle = "ln(kt/GeV)";
    
    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    c->Divide(npt, 2); // 2 for decayed, undecayed

    TCanvas *c_dynKt = new TCanvas("c_dynKt", "c_dynKt", 1800, 1000);
    c_dynKt->Divide(npt, 2); // 2 for decayed, undecayed with dynKt

    
    TCanvas *c_ratio = new TCanvas("c_ratio", "c_ratio", 1800, 1000);
    c_ratio->Divide(npt, 2); // 2 for decayed/undecayed, decayed/undecayed with dynKt
    
    

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        // Clone the histograms inside the loop so that the original range doesn't change
        TH3D *h3d_decayed_clone = (TH3D *) h3d_decayed->Clone();
        TH3D *h3d_decayed_dynKt_clone = (TH3D *) h3d_decayed_dynKt->Clone();

        TH3D *h3d_undecayed_clone = (TH3D *) h3d_undecayed->Clone();
        TH3D *h3d_undecayed_dynKt_clone = (TH3D *) h3d_undecayed_dynKt->Clone();

        h3d_decayed_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_decayed_dynKt_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);

        h3d_undecayed_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_undecayed_dynKt_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        
        // Create 2D projections : X = ln(1/rg), Y = ln(kt)
        TH2D *h2d_decayed = (TH2D *) h3d_decayed_clone->Project3D(Form("yx%d", i)); // Need the i so it doesn't get overwritten
        TH2D *h2d_decayed_dynKt = (TH2D *) h3d_decayed_dynKt_clone->Project3D(Form("yx%d", i));

        TH2D *h2d_undecayed = (TH2D *) h3d_undecayed_clone->Project3D(Form("yx%d", i));
        TH2D *h2d_undecayed_dynKt = (TH2D *) h3d_undecayed_dynKt_clone->Project3D(Form("yx%d", i));

        TLine *line = new TLine(0.91, 0, 5, 0);
        line->SetLineWidth(2);

        // Create information text boxes
        TPaveText *info_decayed = new TPaveText(0.35, 0.75, 0.85, 0.9, "ndc");
        info_decayed->SetFillColor(0);
        info_decayed->SetBorderSize(1);
        info_decayed->SetTextSize(15);
        
        TPaveText *info_undecayed = (TPaveText *) info_decayed->Clone();
        TPaveText *info_ratio = (TPaveText *) info_decayed->Clone();        
        TPaveText *info_ratio_large = (TPaveText *) info_decayed->Clone();
        info_ratio_large->SetTextSize(25);
        
        TPaveText *gsptxt = new TPaveText(0., 0.0001, 0., 0.00001, "ndc");
        if (!GSPincl) {
            gsptxt = new TPaveText(0.2, 0.25, 0.35, 0.4, "ndc");
            gsptxt->AddText("NO GSP");
            gsptxt->SetFillColor(0);
            gsptxt->SetBorderSize(0);
            gsptxt->SetTextSize(20);
        }

        //if (chargedSJ) {
            info_decayed->AddText("chargedSJ b-jets");
            info_undecayed->AddText("aggregatedB_truthInfo b-jets");
            info_ratio->AddText("ratio chargedSJ / aggregatedB_truthInfo b-jets");
        //} else {
        //    info_decayed->AddText("SD b-jets");
        //    info_undecayed->AddText("fullB b-jets");
        //    info_ratio->AddText("ratio SD / fullB b-jets");
        //}

        info_decayed->AddLine(0., 0.7, 1., 0.7);
        info_undecayed->AddLine(0., 0.7, 1., 0.7);
        info_ratio->AddLine(0., 0.7, 1., 0.7);

        info_decayed->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
        info_undecayed->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
        info_ratio->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));

        TPaveText *info_decayed_dynKt = (TPaveText *) info_decayed->Clone();
        TPaveText *info_undecayed_dynKt = (TPaveText *) info_undecayed->Clone();
        TPaveText *info_ratio_dynKt = (TPaveText *) info_ratio->Clone();

	string level_decayed = "hadron level";
        string level_undecayed = "reco level";
	string level_ratio = "hadron / reco level";
        
        if (!GSPincl) { 
            level_decayed += ", noGSP";
	    level_undecayed += ", noGSP";
	    level_ratio += ", noGSP";
        }
        
        
        string level_decayed_dynKt = level_decayed + ", dynKt";
	string level_undecayed_dynKt = level_undecayed + ", dynKt";
	string level_ratio_dynKt = level_ratio + ", dynKt";

        info_decayed->AddText(level_decayed.c_str());
        info_undecayed->AddText(level_undecayed.c_str());
        info_ratio->AddText(level_ratio.c_str());

        info_decayed_dynKt->AddText(level_decayed_dynKt.c_str());
        info_undecayed_dynKt->AddText(level_undecayed_dynKt.c_str());
        info_ratio_dynKt->AddText(level_ratio_dynKt.c_str());

        // c : decayed, undecayed
        Float_t zmin = 0.;
        Float_t zmax = 0.3;
        
        c->cd(i + 1);
        c->cd(i + 1)->SetGrid();
        
        set_axes_labels(h2d_decayed, xtitle, ytitle);
        normalise_histo(h2d_decayed);
        set_zrange(h2d_decayed, zmin, zmax);

        h2d_decayed->Draw("colz");
        line->Draw();
        info_decayed->Draw();
        gsptxt->Draw();
        
        c->cd(i + 1 + npt);
        c->cd(i + 1 + npt)->SetGrid();
        
        set_axes_labels(h2d_undecayed, xtitle, ytitle);
        normalise_histo(h2d_undecayed);
        set_zrange(h2d_undecayed, zmin, zmax);
        
        h2d_undecayed->Draw("colz");
        line->Draw();
        info_undecayed->Draw();
        gsptxt->Draw();

        // c_dynKt : decayed, undecayed dynKt
        Float_t zmin_dynKt = 0.;
        Float_t zmax_dynKt = 0.3;

        c_dynKt->cd(i + 1);
        c_dynKt->cd(i + 1)->SetGrid();
        set_axes_labels(h2d_decayed_dynKt, xtitle, ytitle);
        normalise_histo(h2d_decayed_dynKt);
        set_zrange(h2d_decayed_dynKt, zmin_dynKt, zmax_dynKt);

        h2d_decayed_dynKt->Draw("colz");
        line->Draw();
        info_decayed_dynKt->Draw();
        gsptxt->Draw();

        c_dynKt->cd(i + 1 + npt);
        c_dynKt->cd(i + 1 + npt)->SetGrid();
        set_axes_labels(h2d_undecayed_dynKt, xtitle, ytitle);
        normalise_histo(h2d_undecayed_dynKt);
        set_zrange(h2d_undecayed_dynKt, zmin_dynKt, zmax_dynKt);
        
        h2d_undecayed_dynKt->Draw("colz");
        line->Draw();
        info_undecayed_dynKt->Draw();
        gsptxt->Draw();

        // c_ratio : decayed / undecayed, decayed / undecayed dynKt
        Float_t zmin_ratio = 0.;
        Float_t zmax_ratio = 10.;

        c_ratio->cd(i + 1);
        c_ratio->cd(i + 1)->SetLogz();
        c_ratio->cd(i + 1)->SetGrid();

        TH2D *h2d_ratio = (TH2D *) h2d_decayed->Clone();
        h2d_ratio->Divide(h2d_undecayed);
        set_zrange(h2d_ratio, zmin_ratio, zmax_ratio);

        h2d_ratio->Draw("colz");
        line->Draw();
        info_ratio->Draw();
        gsptxt->Draw();

        c_ratio->cd(i + 1 + npt);
        c_ratio->cd(i + 1 + npt)->SetLogz();
        c_ratio->cd(i + 1 + npt)->SetGrid();

        TH2D *h2d_ratio_dynKt = (TH2D *) h2d_decayed_dynKt->Clone();
        h2d_ratio_dynKt->Divide(h2d_undecayed_dynKt);
        set_zrange(h2d_ratio_dynKt, zmin_ratio, zmax_ratio);

        h2d_ratio_dynKt->Draw("colz");
        line->Draw();
        info_ratio->Draw();
        gsptxt->Draw();

        /*
        // c_ratio_large : draw one by one with text option 

        // Change format of Draw text option
        gStyle->SetPaintTextFormat(".2f");

        TCanvas *c_ratio_large = new TCanvas(Form("c_ratio_large%d", i), "c_ratio_large", 1600, 1000);
        c_ratio_large->SetLogz();
        c_ratio_large->SetGrid();

        TH2D *h2d_ratio_large = (TH2D *) h2d_decayed->Clone();
        h2d_ratio_large->Divide(h2d_undecayed);
        h2d_ratio_large->SetMarkerSize(500);
        h2d_ratio_large->GetXaxis()->SetTitleOffset(1.5);
        h2d_ratio_large->GetYaxis()->SetTitleOffset(1.);
        set_zrange(h2d_ratio_large, zmin_ratio, zmax_ratio);
        
        h2d_ratio_large->Draw("text colz");
        line->Draw();
        info_ratio->Draw();
        gsptxt->Draw();

        string savename_c_ratio_large = savename_c_ratio + Form("_%.0f_to_%0.f_GeV.png", ptmin, ptmax);
        c_ratio_large->Draw();
        c_ratio_large->Print(savename_c_ratio_large.c_str());

        TCanvas *c_ratio_large_dynKt = new TCanvas(Form("c_ratio_large_dynKt%d", i), "c_ratio_large", 1600, 1000);
        c_ratio_large_dynKt->SetLogz();
        c_ratio_large_dynKt->SetGrid();

        TH2D *h2d_ratio_large_dynKt = (TH2D *) h2d_decayed_dynKt->Clone();
        h2d_ratio_large_dynKt->Divide(h2d_undecayed_dynKt);
        h2d_ratio_large_dynKt->SetMarkerSize(500);
        h2d_ratio_large_dynKt->GetXaxis()->SetTitleOffset(1.5);
        h2d_ratio_large_dynKt->GetYaxis()->SetTitleOffset(1.);
        set_zrange(h2d_ratio_large_dynKt, zmin_ratio, zmax_ratio);

        h2d_ratio_large_dynKt->Draw("text colz");
        line->Draw();
        info_ratio_dynKt->Draw();
        gsptxt->Draw();

        string name_dynKt = savename_c_ratio_large + Form("_%.0f_to_%0.f_GeV_dynKt.png", ptmin, ptmax);
        c_ratio_large_dynKt->Draw();
        c_ratio_large_dynKt->Print(name_dynKt.c_str());
        */
    }

    c->Draw();
    c_dynKt->Draw();
    c_ratio->Draw();
    
    c->Print(savename_c.c_str());
    c_dynKt->Print(savename_c_dynKt.c_str());

    savename_c_ratio += ".png";
    c_ratio->Print(savename_c_ratio.c_str());
}
