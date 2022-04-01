#include "TH3F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"
#include <regex>

using namespace std;

void draw_decay_effect(bool chargedSJ = true, bool GSPincl = false)
{
    string decayed_fname = "";
    string undecayed_fname = "";
    string charged_decayed_fname = "";
    string charged_undecayed_fname = "";    

    string savename_c = "";

    if (GSPincl) {
        decayed_fname += "~/rootFiles/SD_ref.root";
        undecayed_fname += "~/rootFiles/undecayHF_SD_ref.root";
        charged_decayed_fname += "~/rootFiles/chargedSJ_ref.root";
        charged_undecayed_fname += "~/rootFiles/charged_partialB_ref.root";    
    } else {
        decayed_fname += "~/rootFiles/SD_noGSP_ref.root";
        undecayed_fname += "~/rootFiles/undecayHF_SD_noGSP_ref.root";
        charged_decayed_fname += "~/rootFiles/chargedSJ_noGSP_ref.root";
        charged_undecayed_fname += "~/rootFiles/charged_partialB_noGSP_ref.root";    
        savename_c += "noGSP";
    }


    TFile *fin_decayed;
    TFile *fin_undecayed;

    // Load ref 3D histograms -- X = rg, Y = kt, Z = pt 
    if (chargedSJ) {
        fin_decayed = new TFile(charged_decayed_fname.c_str());
        fin_undecayed = new TFile(charged_undecayed_fname.c_str());
        savename_c += "_chargedSJ_dec_vs_nodec";
    } else {
        fin_decayed = new TFile(decayed_fname.c_str());
        fin_undecayed = new TFile(undecayed_fname.c_str());
        savename_c += "_SD_dec_vs_nodec";
    }

    string savename_c_dynKt = savename_c + "_dynKt.png";
    string savename_c_ratio = savename_c + "_ratio.png";
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
        TPaveText *info_decayed = new TPaveText(0.35, 0.7, 0.85, 0.85, "ndc");
        info_decayed->SetFillColor(0);
        info_decayed->SetBorderSize(1);
        info_decayed->SetTextSize(15);
        
        TPaveText *info_undecayed = (TPaveText *) info_decayed->Clone();
        TPaveText *info_ratio = (TPaveText *) info_decayed->Clone();

        if (chargedSJ) {
            info_decayed->AddText("chargedSJ b-jets");
            info_undecayed->AddText("chargedSJ_partialB b-jets");
            info_ratio->AddText("ratio chargedSJ / chargedSJ_partialB b-jets");
        } else {
            info_decayed->AddText("SD b-jets");
            info_undecayed->AddText("undecayHF_SD b-jets");
            info_ratio->AddText("ratio SD / undecayHF_SD b-jets");
        }

        info_decayed->AddLine(0., 0.7, 1., 0.7);
        info_undecayed->AddLine(0., 0.7, 1., 0.7);
        info_ratio->AddLine(0., 0.7, 1., 0.7);

        info_decayed->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
        info_undecayed->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
        info_ratio->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));

        TPaveText *info_decayed_dynKt = (TPaveText *) info_decayed->Clone();
        TPaveText *info_undecayed_dynKt = (TPaveText *) info_undecayed->Clone();
        TPaveText *info_ratio_dynKt = (TPaveText *) info_ratio->Clone();

        string level = "hadron level";

        if (!GSPincl) { 
            level += ", noGSP";
        }

        string level_dynKt = level + ", dynKt";

        info_decayed->AddText(level.c_str());
        info_undecayed->AddText(level.c_str());
        info_ratio->AddText(level.c_str());

        info_decayed_dynKt->AddText(level_dynKt.c_str());
        info_undecayed_dynKt->AddText(level_dynKt.c_str());
        info_ratio_dynKt->AddText(level_dynKt.c_str());

        // c : decayed, undecayed
        Float_t zmin = 0.;
        Float_t zmax = 0.4;
        
        c->cd(i + 1);
        
        h2d_decayed->SetYTitle(ytitle.c_str());
        h2d_decayed->SetXTitle(xtitle.c_str());
        h2d_decayed->GetXaxis()->SetTitleOffset(2.5);
        h2d_decayed->GetYaxis()->SetTitleOffset(2.5);
        // Normalise the histogram
        h2d_decayed->GetXaxis()->SetRange(0, h2d_decayed->GetNbinsX()+1);
        h2d_decayed->GetYaxis()->SetRange(0, h2d_decayed->GetNbinsY()+1);
        h2d_decayed->Scale(1/h2d_decayed->Integral("width"));
        // Hide the under/overflow again
        h2d_decayed->GetXaxis()->SetRange(1, h2d_decayed->GetNbinsX());
        h2d_decayed->GetYaxis()->SetRange(1, h2d_decayed->GetNbinsY());
        // Fix colormap (Z) range
        h2d_decayed->GetZaxis()->SetRangeUser(zmin, zmax);

        h2d_decayed->Draw("colz");
        line->Draw();
        info_decayed->Draw();
        
        c->cd(i + 1 + npt);
        
        h2d_undecayed->SetYTitle(ytitle.c_str());
        h2d_undecayed->SetXTitle(xtitle.c_str());
        h2d_undecayed->GetXaxis()->SetTitleOffset(2.5);
        h2d_undecayed->GetYaxis()->SetTitleOffset(2.5);
        // Normalise the histogram
        h2d_undecayed->GetXaxis()->SetRange(0, h2d_undecayed->GetNbinsX()+1);
        h2d_undecayed->GetYaxis()->SetRange(0, h2d_undecayed->GetNbinsY()+1);
        h2d_undecayed->Scale(1/h2d_undecayed->Integral("width"));
        // Hide the under/overflow again
        h2d_undecayed->GetXaxis()->SetRange(1, h2d_undecayed->GetNbinsX());
        h2d_undecayed->GetYaxis()->SetRange(1, h2d_undecayed->GetNbinsY());
        // Fix colormap (Z) range
        h2d_undecayed->GetZaxis()->SetRangeUser(zmin, zmax);
        
        h2d_undecayed->Draw("colz");
        line->Draw();
        info_undecayed->Draw();

        // c_dynKt : decayed, undecayed dynKt
        Float_t zmin_dynKt = 0.;
        Float_t zmax_dynKt = 0.3;

        c_dynKt->cd(i + 1);
        h2d_decayed_dynKt->SetYTitle(ytitle.c_str());
        h2d_decayed_dynKt->SetXTitle(xtitle.c_str());
        h2d_decayed_dynKt->GetXaxis()->SetTitleOffset(2.5);
        h2d_decayed_dynKt->GetYaxis()->SetTitleOffset(2.5);
        // Normalise the histogram
        h2d_decayed_dynKt->GetXaxis()->SetRange(0, h2d_decayed_dynKt->GetNbinsX()+1);
        h2d_decayed_dynKt->GetYaxis()->SetRange(0, h2d_decayed_dynKt->GetNbinsY()+1);
        h2d_decayed_dynKt->Scale(1/h2d_decayed_dynKt->Integral("width"));
        // Hide the under/overflow again
        h2d_decayed_dynKt->GetXaxis()->SetRange(1, h2d_decayed_dynKt->GetNbinsX());
        h2d_decayed_dynKt->GetYaxis()->SetRange(1, h2d_decayed_dynKt->GetNbinsY());
        // Fix colormap (Z) range
        h2d_decayed_dynKt->GetZaxis()->SetRangeUser(zmin_dynKt, zmax_dynKt);

        h2d_decayed_dynKt->Draw("colz");
        line->Draw();
        info_decayed_dynKt->Draw();

        c_dynKt->cd(i + 1 + npt);
        h2d_undecayed_dynKt->SetYTitle(ytitle.c_str());
        h2d_undecayed_dynKt->SetXTitle(xtitle.c_str());
        h2d_undecayed_dynKt->GetXaxis()->SetTitleOffset(2.5);
        h2d_undecayed_dynKt->GetYaxis()->SetTitleOffset(2.5);
        // Normalise the histogram
        h2d_undecayed_dynKt->GetXaxis()->SetRange(0, h2d_undecayed_dynKt->GetNbinsX()+1);
        h2d_undecayed_dynKt->GetYaxis()->SetRange(0, h2d_undecayed_dynKt->GetNbinsY()+1);
        h2d_undecayed_dynKt->Scale(1/h2d_undecayed_dynKt->Integral("width"));
        // Hide the under/overflow again
        h2d_undecayed_dynKt->GetXaxis()->SetRange(1, h2d_undecayed_dynKt->GetNbinsX());
        h2d_undecayed_dynKt->GetYaxis()->SetRange(1, h2d_undecayed_dynKt->GetNbinsY());
        // Fix colormap (Z) range
        h2d_undecayed_dynKt->GetZaxis()->SetRangeUser(zmin_dynKt, zmax_dynKt);
        
        h2d_undecayed_dynKt->Draw("colz");
        line->Draw();
        info_undecayed_dynKt->Draw();

        // c_ratio : decayed / undecayed, decayed / undecayed dynKt
        Float_t zmin_ratio = 0.;
        Float_t zmax_ratio = 6.;

        c_ratio->cd(i + 1);
        c_ratio->cd(i + 1)->SetLogz();
        TH2D *h2d_ratio = (TH2D *) h2d_decayed->Clone();
        h2d_ratio->Divide(h2d_undecayed);
        // Fix the colormap (Z) axis
        h2d_ratio->GetZaxis()->SetRangeUser(zmin_ratio, zmax_ratio);

        h2d_ratio->Draw("colz");
        line->Draw();
        info_ratio->Draw();

        c_ratio->cd(i + 1 + npt);
        c_ratio->cd(i + 1 + npt)->SetLogz();
        TH2F *h2d_ratio_dynKt = (TH2F *) h2d_decayed_dynKt->Clone();
        h2d_ratio_dynKt->Divide(h2d_undecayed_dynKt);
        // Fix the colormap (Z) axis
        h2d_ratio_dynKt->GetZaxis()->SetRangeUser(zmin_ratio, zmax_ratio);

        h2d_ratio_dynKt->Draw("colz");
        line->Draw();
        info_ratio_dynKt->Draw();
        
    }

    c->Draw();
    c_dynKt->Draw();
    c_ratio->Draw();  

    
    c->Print(savename_c.c_str());
    c_dynKt->Print(savename_c_dynKt.c_str());
    c_ratio->Print(savename_c_ratio.c_str());

}