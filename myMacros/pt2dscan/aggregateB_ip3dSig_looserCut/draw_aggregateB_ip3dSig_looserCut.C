
#include "TH3D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"

#include "../../utils.h"
#include "../HistDrawer_pt2dscan.h"

#include <regex>

using namespace std;

void draw_aggregateB_ip3dSig_looserCut(bool GSPincl = true)
{      
    /* Draw truth vs reco level with manual B aggregation */ 
    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }
    
    std::string fname_reco = "~/rootFiles/aggregateB_ip3dSig_looserCut" + noGSP + "_reco.root";
    std::string fname_ref = "~/rootFiles/aggregateB_ip3dSig_looserCut" + noGSP + "_ref.root";
    
    HistDrawer_pt2dscan HD_reco(fname_reco, "hBtag_rgkt");
    HistDrawer_pt2dscan HD_ref(fname_ref, "hB_rgkt");

    // Create / Draw 2D histograms - Projections
    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {100., 150.};
    Float_t highpt[2] = {200., 250.};
    
    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    c->Divide(npt, 2); // 2 for truth, reco

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        TH2D *h2d_reco = (TH2D *) HD_reco.do_rgkt_projection(ptrange[i], "h2d_reco");
        TH2D *h2d_ref = (TH2D *) HD_ref.do_rgkt_projection(ptrange[i], "h2d_ref");
        
        TLine *line = new TLine(0.91, 0, 5, 0);
        line->SetLineWidth(2);

        TPaveText *info = new TPaveText(0.45, 0.7, 0.8, 0.85, "ndc");
        info->AddText("aggregateB_ip3dSig_looserCut b-jets");
        info->AddLine(0., 0.7, 1., 0.7);
        info->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
        info->SetFillColor(0);
        info->SetBorderSize(1);
        info->SetTextSize(15);
        
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

		h2d_ref->GetZaxis()->SetRangeUser(zmin, zmax);
		h2d_reco->GetZaxis()->SetRangeUser(zmin, zmax);

        c->cd(i + 1);
        c->cd(i + 1)->SetGrid();
        
        h2d_ref->Draw("colz");
        line->Draw();
        info_ref->Draw();
        gsptxt->Draw();

        c->cd(i + 1 + npt);
        c->cd(i + 1 + npt)->SetGrid();
    
        h2d_reco->Draw("colz");
        line->Draw();
        info_reco->Draw();
        gsptxt->Draw();
    }

    c->Draw();
    
    string savename_c = "aggregateB_ip3dSig_looserCut_bjets_ref_vs_reco";
    if (!GSPincl) {
        savename_c += "_noGSP";
    }
    savename_c += ".png";
    
    c->Print(savename_c.c_str());
}
