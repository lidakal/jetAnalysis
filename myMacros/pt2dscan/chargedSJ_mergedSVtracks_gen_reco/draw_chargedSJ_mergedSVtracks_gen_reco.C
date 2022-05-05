
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

void draw_chargedSJ_mergedSVtracks_gen_reco(bool GSPincl = true)
{      
    /* Draw truth vs reco level with manual B aggregation */ 

    string histfile_reco = "";
    string histfile_ref = "";

    if (GSPincl) {
        histfile_reco += "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco_reco.root";
        histfile_ref += "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco_ref.root";
    } else {
        histfile_reco += "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco_noGSP_reco.root";
        histfile_ref += "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco_noGSP_ref.root";
    }

    string xtitle = "ln(1/R_{g})";
    string ytitle = "ln(kt/GeV)";

    // Load reco 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_reco = new TFile(histfile_reco.c_str());
    TH3D *h3d_reco = (TH3D *) fin_reco->Get("hB_rgkt")->Clone();
    TH3D *h3d_reco_dynKt = (TH3D *) fin_reco->Get("hB_rgkt_dynKt")->Clone();

    // Load ref 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_ref = new TFile(histfile_ref.c_str());
    TH3D *h3d_ref = (TH3D *) fin_ref->Get("hB_rgkt")->Clone();
    TH3D *h3d_ref_dynKt = (TH3D *) fin_ref->Get("hB_rgkt_dynKt")->Clone();

    // Create / Draw 2D histograms - Projections
    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};
    
    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    c->Divide(npt, 2); // 2 for truth, reco

    TCanvas *c_dynKt = new TCanvas("c_dynKt", "c_dynKt", 1800, 1000);
    c_dynKt->Divide(npt, 2); // 2 for truth dynKt, reco dynKt

    TCanvas *c_ratio = new TCanvas("c_ratio", "c_ratio", 1800, 1000);
    c_ratio->Divide(npt, 2); // 2 for  reco / truth, reco dynKt / truth dynKt

    // Create / Draw 1D histograms - Projections
    TCanvas *crg = new TCanvas("crg", "crg", 1800, 1000);
    crg->Divide(npt, 2); // 2 for truth + reco, truth dynKt + reco dynKt

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        TH3D *h3d_reco_clone = (TH3D *) h3d_reco->Clone();
        TH3D *h3d_reco_dynKt_clone = (TH3D *) h3d_reco_dynKt->Clone();
        TH3D *h3d_ref_clone = (TH3D *) h3d_ref->Clone();
        TH3D *h3d_ref_dynKt_clone = (TH3D *) h3d_ref_dynKt->Clone();

        h3d_reco_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_reco_dynKt_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_ref_dynKt_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);

        TH2D *h2d_reco = (TH2D *) h3d_reco_clone->Project3D(Form("yx%d_reco", i)); 
        TH2D *h2d_reco_dynKt = (TH2D *) h3d_reco_dynKt_clone->Project3D(Form("yx%d_reco", i)); 
        TH2D *h2d_ref = (TH2D *) h3d_ref_clone->Project3D(Form("yx%d_ref", i));
        TH2D *h2d_ref_dynKt = (TH2D *) h3d_ref_dynKt_clone->Project3D(Form("yx%d_ref", i)); 

        TLine *line = new TLine(0.91, 0, 5, 0);
        line->SetLineWidth(2);

        TPaveText *info = new TPaveText(0.45, 0.7, 0.8, 0.85, "ndc");
        info->AddText("chargedSJ_mergedSVtracks_gen_reco b-jets");
        info->AddLine(0., 0.7, 1., 0.7);
        info->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
        info->SetFillColor(0);
        info->SetBorderSize(1);
        info->SetTextSize(15);
        
        TPaveText *info_ref = (TPaveText *) info->Clone();
        info_ref->AddText("truth level");
        
        TPaveText *info_reco = (TPaveText *) info->Clone();
        info_reco->AddText("reco level");
        
        TPaveText *info_reco_dynKt = (TPaveText *) info->Clone();
        info_reco_dynKt->AddText("reco level, dynKt");

        TPaveText *info_ref_dynKt = (TPaveText *) info->Clone();
        info_ref_dynKt->AddText("truth level, dynKt");
        
        TPaveText *info_ratio = (TPaveText *) info->Clone();
        info_ratio->AddText("reco / truth ratio");
        
        TPaveText *info_ratio_dynKt = (TPaveText *) info->Clone();
        info_ratio_dynKt->AddText("reco / truth ratio, dynKt");
        
        TPaveText *gsptxt = new TPaveText(0.2, 0.25, 0.35, 0.4, "ndc");
        gsptxt->SetFillColor(0);
        gsptxt->SetBorderSize(0);
        gsptxt->SetTextSize(20);
        if (!GSPincl) {
            gsptxt->AddText("NO GSP");
        }

        // c : ref, reco
        Float_t zmin = 0.;
        Float_t zmax = 0.2;

        c->cd(i + 1);
        c->cd(i + 1)->SetGrid();
        set_axes_labels(h2d_ref, xtitle, ytitle);
        normalise_histo(h2d_ref);
        set_zrange(h2d_ref, zmin, zmax);

        h2d_ref->Draw("colz");
        line->Draw();
        info_ref->Draw();
        gsptxt->Draw();

        c->cd(i + 1 + npt);
        c->cd(i + 1 + npt)->SetGrid();
        set_axes_labels(h2d_reco, xtitle, ytitle);
        normalise_histo(h2d_reco);
        set_zrange(h2d_reco, zmin, zmax);
        
        h2d_reco->Draw("colz");
        line->Draw();
        info_reco->Draw();
        gsptxt->Draw();

        // c_dynKt : ref, reco dynKt
        Float_t zmin_dynKt = 0.;
        Float_t zmax_dynKt = 0.2;

        c_dynKt->cd(i + 1);
        c_dynKt->cd(i + 1)->SetGrid();
        set_axes_labels(h2d_ref_dynKt, xtitle, ytitle);
        normalise_histo(h2d_ref_dynKt);
        set_zrange(h2d_ref_dynKt, zmin_dynKt, zmax_dynKt);

        h2d_ref_dynKt->Draw("colz");
        line->Draw();
        info_ref_dynKt->Draw();
        gsptxt->Draw();

        c_dynKt->cd(i + 1 + npt);
        c_dynKt->cd(i + 1 + npt)->SetGrid();
        set_axes_labels(h2d_reco_dynKt, xtitle, ytitle);
        normalise_histo(h2d_reco_dynKt);
        set_zrange(h2d_reco_dynKt, zmin_dynKt, zmax_dynKt);
        
        h2d_reco_dynKt->Draw("colz");
        line->Draw();
        info_reco_dynKt->Draw();
        gsptxt->Draw();

        // c_ratio : reco / ref, reco dynKt / ref dynKt
        Float_t zmin_ratio = 0.;
        Float_t zmax_ratio = 10.;

        c_ratio->cd(i + 1);
        c_ratio->cd(i + 1)->SetLogz();
        c_ratio->cd(i + 1)->SetGrid();
        TH2D *h2d_ratio = (TH2D *) h2d_reco->Clone();
        h2d_ratio->Divide(h2d_ref);
        // Fix the colormap (Z) axis
        set_zrange(h2d_ratio, zmin_ratio, zmax_ratio);

        h2d_ratio->Draw("colz");
        line->Draw();
        info_ratio->Draw();
        gsptxt->Draw();

        c_ratio->cd(i + 1 + npt);
        c_ratio->cd(i + 1 + npt)->SetGrid();
        c_ratio->cd(i + 1 + npt)->SetLogz();
        TH2D *h2d_ratio_dynKt = (TH2D *) h2d_reco_dynKt->Clone();
        h2d_ratio_dynKt->Divide(h2d_ref_dynKt);
        // Fix the colormap (Z) axis
        set_zrange(h2d_ratio_dynKt, zmin_ratio, zmax_ratio);

        h2d_ratio_dynKt->Draw("colz");
        line->Draw();
        info_ratio_dynKt->Draw();
        gsptxt->Draw();

        // 1D projections
        TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);

        TLegend *leg_dynKt = new TLegend(0.6, 0.7, 0.9, 0.9);
        leg_dynKt->SetBorderSize(0);
        leg_dynKt->SetFillStyle(0);
        

        TH1D *h1d_ref = (TH1D *) h3d_ref_clone->Project3D(Form("x%d_ref", i));
        TH1D *h1d_ref_dynKt = (TH1D *) h3d_ref_dynKt_clone->Project3D(Form("x%d_ref", i));
        TH1D *h1d_reco = (TH1D *) h3d_reco_clone->Project3D(Form("x%d_reco", i));
        TH1D *h1d_reco_dynKt = (TH1D *) h3d_reco_dynKt_clone->Project3D(Form("x%d_reco", i));

        normalise_histo(h1d_ref);
        normalise_histo(h1d_ref_dynKt);
        normalise_histo(h1d_reco);
        normalise_histo(h1d_reco_dynKt);

	float ymax = std::max({h1d_ref->GetMaximum(), h1d_reco->GetMaximum(), 
	                       h1d_ref_dynKt->GetMaximum(), h1d_reco_dynKt->GetMaximum()});
	ymax += 0.05;

	h1d_ref->GetYaxis()->SetRangeUser(0., ymax);
	h1d_ref_dynKt->GetYaxis()->SetRangeUser(0., ymax);
	h1d_reco->GetYaxis()->SetRangeUser(0., ymax);
	h1d_reco_dynKt->GetYaxis()->SetRangeUser(0., ymax);

	string xtitle1d = "ln(1/R_{g})";
	string ytitle1d = Form("1/N dN/d(ln(1/R_{g})) for jetpt in [%.0f, %.0f] GeV", ptmin, ptmax);

	set_axes_labels(h1d_ref, xtitle1d, ytitle1d);
	set_axes_labels(h1d_ref_dynKt, xtitle1d, ytitle1d);
	set_axes_labels(h1d_reco, xtitle1d, ytitle1d);
	set_axes_labels(h1d_reco_dynKt, xtitle1d, ytitle1d);

	h1d_ref->SetLineColor(1);
	h1d_ref_dynKt->SetLineColor(1);
	
	h1d_reco->SetLineColor(2);
	h1d_reco_dynKt->SetLineColor(2);

        leg->AddEntry(h1d_ref, "truth", "l");
        leg_dynKt->AddEntry(h1d_ref_dynKt, "truth dynKt", "l");
        leg->AddEntry(h1d_reco, "reco", "l");
        leg_dynKt->AddEntry(h1d_reco_dynKt, "reco dynKt", "l");

        crg->cd(i + 1);
        crg->cd(i + 1)->SetGrid();

        h1d_ref->Draw("hist");
        h1d_reco->Draw("hist same");
        leg->Draw();
	gsptxt->Draw();

        crg->cd(i + 1 + npt);
        crg->cd(i + 1 + npt)->SetGrid();

        h1d_ref_dynKt->Draw("hist");
        h1d_reco_dynKt->Draw("hist same");
        leg_dynKt->Draw();
	gsptxt->Draw();

    }

    c->Draw();
    c_dynKt->Draw();
    c_ratio->Draw();
    crg->Draw();
    
    string savename_c = "chargedSJ_mergedSVtracks_gen_reco_bjets_ref_vs_reco";
    if (!GSPincl) {
        savename_c += "_noGSP";
    }
    string savename_c_dynKt = savename_c + "_dynKt.png";
    string savename_c_ratio = savename_c + "_ratio.png";
    string savename_c_1d = savename_c + "_1d.png";
    savename_c += ".png";
    
    c->Print(savename_c.c_str());
    c_dynKt->Print(savename_c_dynKt.c_str());
    c_ratio->Print(savename_c_ratio.c_str());
    crg->Print(savename_c_1d.c_str());
}
