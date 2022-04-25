#include "TH3D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"
#include "../../utils.h"
#include <regex>

using namespace std;

void draw_SD(bool GSPincl = true)
{      
    string histfile_ref = "";
    string histfile_par = "";

    if (GSPincl) {
        histfile_ref += "~/rootFiles/SD_ref.root";
        histfile_par += "~/rootFiles/SD_par.root";
        histfile_reco += "~/rootFiles/SD_reco.root";
    } else {
        histfile_ref += "~/rootFiles/SD_noGSP_ref.root";
        histfile_par += "~/rootFiles/SD_noGSP_par.root";
        histfile_reco += "~/rootFiles/SD_noGSP_reco.root";
    }

    string xtitle = "ln(1/R_{g})";
    string ytitle = "ln(kt/GeV)";

    // Load ref 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_ref = new TFile(histfile_ref.c_str());
    TH3D *h3d_ref = (TH3D *) fin_ref->Get("hB_rgkt")->Clone();
    TH3D *h3d_ref_dynKt = (TH3D *) fin_ref->Get("hB_rgkt_dynKt")->Clone();

    // Load reco 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_reco = new TFile(histfile_reco.c_str());
    TH3D *h3d_reco = (TH3D *) fin_ref->Get("hB_rgkt")->Clone();
    TH3D *h3d_reco_dynKt = (TH3D *) fin_ref->Get("hB_rgkt_dynKt")->Clone();

    // Load par 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_par = new TFile(histfile_par.c_str());
    TH3D *h3d_par = (TH3D *) fin_par->Get("hB_rgkt")->Clone();

    // Create / Draw 2D histograms - Projections
    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};
    
    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    c->Divide(npt, 3); // 3 for parton, truth, reco

    TCanvas *c_dynKt = new TCanvas("c_dynKt", "c_dynKt", 1800, 1000);
    c_dynKt->Divide(npt, 3); // 3 for parton, truth dynKt, reco dynKt

    TCanvas *c_ratio_ref = new TCanvas("c_ratio_ref", "c_ratio_ref", 1800, 1000);
    c_ratio_ref->Divide(npt, 2); // 2 for truth/parton, truth dynKt/parton

    TCanvas *c_ratio_reco = new TCanvas("c_ratio_reco", "c_ratio_reco", 1800, 1000);
    c_ratio_reco->Divide(npt, 2); // 2 for reco/parton, reco dynKt/parton

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        // Clone 3D histograms so they can be modified for specific pt range
        TH3D *h3d_ref_clone = (TH3D *) h3d_ref->Clone();
        TH3D *h3d_ref_dynKt_clone = (TH3D *) h3d_ref_dynKt->Clone();
        TH3D *h3d_reco_clone = (TH3D *) h3d_reco->Clone();
        TH3D *h3d_reco_dynKt_clone = (TH3D *) h3d_reco_dynKt->Clone();
        TH3D *h3d_par_clone = (TH3D *) h3d_par->Clone();

        // Set the pt range 
        h3d_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_ref_dynKt_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_reco_dynKt_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_par_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);

        // Project in rg, kt for specific pt range
        TH2D *h2d_ref = (TH2D *) h3d_ref_clone->Project3D(Form("yx%d_ref", i)); 
        TH2D *h2d_ref_dynKt = (TH2D *) h3d_ref_dynKt_clone->Project3D(Form("yx%d_ref_dynKt", i)); 
        TH2D *h2d_reco = (TH2D *) h3d_reco_clone->Project3D(Form("yx%d_reco", i)); 
        TH2D *h2d_reco_dynKt = (TH2D *) h3d_reco_dynKt_clone->Project3D(Form("yx%d_reco_dynKt", i)); 
        TH2D *h2d_par = (TH2D *) h3d_par_clone->Project3D(Form("yx%d_par", i));

        // Create line to Draw at ln(kt) = 0
        TLine *line = new TLine(0.91, 0, 5, 0);
        line->SetLineWidth(2);

        // Create info pave texts
        TPaveText *info = new TPaveText(0.45, 0.7, 0.8, 0.85, "ndc");
        info->AddText("SD b-jets");
        info->AddLine(0., 0.7, 1., 0.7);
        info->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
        info->SetFillColor(0);
        info->SetBorderSize(1);
        info->SetTextSize(15);
        
        TPaveText *info_par = (TPaveText *) info->Clone();
        info_par->AddText("parton level");
        
        TPaveText *info_ref = (TPaveText *) info->Clone();
        info_ref->AddText("truth level");
        
        TPaveText *info_ref_dynKt = (TPaveText *) info->Clone();
        info_ref_dynKt->AddText("truth level, dynKt");

        TPaveText *info_reco = (TPaveText *) info->Clone();
        info_reco->AddText("reco level");
        
        TPaveText *info_reco_dynKt = (TPaveText *) info->Clone();
        info_reco_dynKt->AddText("reco level, dynKt");
        
        TPaveText *info_ratio_ref = (TPaveText *) info->Clone();
        info_ratio_ref->AddText("truth / parton ratio");
        
        TPaveText *info_ratio_ref_dynKt = (TPaveText *) info->Clone();
        info_ratio_ref_dynKt->AddText("truth / parton ratio, dynKt");

        TPaveText *info_ratio_reco = (TPaveText *) info->Clone();
        info_ratio_reco->AddText("reco / parton ratio");
        
        TPaveText *info_ratio_reco_dynKt = (TPaveText *) info->Clone();
        info_ratio_reco_dynKt->AddText("reco / parton ratio, dynKt");
        
        TPaveText *gsptxt = new TPaveText(0.2, 0.25, 0.35, 0.4, "ndc");
        gsptxt->SetFillColor(0);
        gsptxt->SetBorderSize(0);
        gsptxt->SetTextSize(20);
        if (!GSPincl) {
            gsptxt->AddText("NO GSP");
        }

        // Draw in c : par, ref, reco
        Float_t zmin = 0.;
        Float_t zmax = 0.4;

        c->cd(i + 1);
        c->cd(i + 1)->SetGrid();
        set_axes_labels(h2d_par, xtitle, ytitle);
        normalise_histo(h2d_par);
        set_zrange(h2d_par, zmin, zmax);

        h2d_par->Draw("colz");
        line->Draw();
        info_par->Draw();
        gsptxt->Draw();

        c->cd(i + 1 + npt);
        c->cd(i + 1 + npt)->SetGrid();
        set_axes_labels(h2d_ref, xtitle, ytitle);
        normalise_histo(h2d_ref);
        set_zrange(h2d_ref, zmin, zmax);
        
        h2d_ref->Draw("colz");
        line->Draw();
        info_ref->Draw();
        gsptxt->Draw();

        c->cd(i + 1 + 3 + npt);
        c->cd(i + 1 + 3 + npt)->SetGrid();
        set_axes_labels(h2d_reco, xtitle, ytitle);
        normalise_histo(h2d_reco);
        set_zrange(h2d_reco, zmin, zmax);
        
        h2d_reco->Draw("colz");
        line->Draw();
        info_reco->Draw();
        gsptxt->Draw();

        // Draw in c_dynKt : par, ref dynKt, reco dynKt
        Float_t zmin_dynKt = 0.;
        Float_t zmax_dynKt = 0.3;

        c_dynKt->cd(i + 1);
        c_dynKt->cd(i + 1)->SetGrid();
        TH2D *h2d_par_clone = (TH2D *) h2d_par->Clone();
        set_zrange(h2d_par_clone, zmin_dynKt, zmax_dynKt);

        h2d_par_clone->Draw("colz");
        line->Draw();
        info_par->Draw();
        gsptxt->Draw();

        c_dynKt->cd(i + 1 + npt);
        c_dynKt->cd(i + 1 + npt)->SetGrid();
        set_axes_labels(h2d_ref_dynKt, xtitle, ytitle);
        normalise_histo(h2d_ref_dynKt);
        set_zrange(h2d_ref_dynKt, zmin_dynKt, zmax_dynKt);
        
        h2d_ref_dynKt->Draw("colz");
        line->Draw();
        info_ref_dynKt->Draw();
        gsptxt->Draw();

        c_dynKt->cd(i + 1 + 3 + npt);
        c_dynKt->cd(i + 1 + 3 + npt)->SetGrid();
        set_axes_labels(h2d_reco_dynKt, xtitle, ytitle);
        normalise_histo(h2d_reco_dynKt);
        set_zrange(h2d_reco_dynKt, zmin_dynKt, zmax_dynKt);
        
        h2d_reco_dynKt->Draw("colz");
        line->Draw();
        info_reco_dynKt->Draw();
        gsptxt->Draw();

        // Draw in c_ratio_ref : ref/par, ref dynKt/par
        Float_t zmin_ratio_ref = 0.;
        Float_t zmax_ratio_ref = 20.;

        c_ratio_ref->cd(i + 1);
        c_ratio_ref->cd(i + 1)->SetLogz();
        c_ratio_ref->cd(i + 1)->SetGrid();
        TH2D *h2d_ratio_ref= (TH2D *) h2d_ref->Clone();
        h2d_ratio_ref->Divide(h2d_par);
        // Fix the colormap (Z) axis
        set_zrange(h2d_ratio_ref, zmin_ratio_ref, zmax_ratio_ref);

        h2d_ratio_ref->Draw("colz");
        line->Draw();
        info_ratio_ref->Draw();
        gsptxt->Draw();

        c_ratio_ref->cd(i + 1 + npt);
        c_ratio_ref->cd(i + 1 + npt)->SetGrid();
        c_ratio_ref->cd(i + 1 + npt)->SetLogz();
        TH2D *h2d_ratio_ref_dynKt = (TH2D *) h2d_ref_dynKt->Clone();
        h2d_ratio_ref_dynKt->Divide(h2d_par);
        // Fix the colormap (Z) axis
        set_zrange(h2d_ratio_ref_dynKt, zmin_ratio_ref, zmax_ratio_ref);

        h2d_ratio_ref_dynKt->Draw("colz");
        line->Draw();
        info_ratio_ref_dynKt->Draw();
        gsptxt->Draw();

        // Draw in c_ratio_reco : reco/par, reco dynKt/par
        Float_t zmin_ratio_reco = 0.;
        Float_t zmax_ratio_reco = 20.;

        c_ratio_reco->cd(i + 1);
        c_ratio_reco->cd(i + 1)->SetLogz();
        c_ratio_reco->cd(i + 1)->SetGrid();
        TH2D *h2d_ratio_reco = (TH2D *) h2d_reco->Clone();
        h2d_ratio_reco->Divide(h2d_par);
        // Fix the colormap (Z) axis
        set_zrange(h2d_ratio_reco, zmin_ratio_reco, zmax_ratio_reco);

        h2d_ratio_reco->Draw("colz");
        line->Draw();
        info_ratio_reco->Draw();
        gsptxt->Draw();

        c_ratio_reco->cd(i + 1 + npt);
        c_ratio_reco->cd(i + 1 + npt)->SetGrid();
        c_ratio_reco->cd(i + 1 + npt)->SetLogz();
        TH2D *h2d_ratio_reco_dynKt = (TH2D *) h2d_reco_dynKt->Clone();
        h2d_ratio_reco_dynKt->Divide(h2d_par);
        // Fix the colormap (Z) axis
        set_zrange(h2d_ratio_reco_dynKt, zmin_ratio_reco, zmax_ratio_reco);

        h2d_ratio_reco_dynKt->Draw("colz");
        line->Draw();
        info_ratio_reco_dynKt->Draw();
        gsptxt->Draw();
    }

    c->Draw();
    c_dynKt->Draw();
    c_ratio_ref->Draw();
    c_ratio_reco->Draw();
    
    string savename_c = "SD_bjets_all_levels";
    if (!GSPincl) {
        savename_c += "_noGSP";
    }
    string savename_c_dynKt = savename_c + "_dynKt.png";
    string savename_c_ratio_ref = savename_c + "_ratio_ref.png";
    string savename_c_ratio_reco = savename_c + "_ratio_reco.png";
    savename_c += ".png";
    
    c->Print(savename_c.c_str());
    c_dynKt->Print(savename_c_dynKt.c_str());
    c_ratio_ref->Print(savename_c_ratio_ref.c_str());
    c_ratio_reco->Print(savename_c_ratio_reco.c_str());
}