#include "HistLoader_pt2dscan.h"

#include "TH2D.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"

class HistDrawer_pt2dscan
{
    std::string fname;

    // ---- Constructors ----
    HistDrawer_pt2dscan() {};
    HistDrawer_pt2dscan(std::string MCname);

    void do_pt2dscan(bool GSPincl);
};

HistDrawer_pt2dscan::HistDrawer_pt2dscan(std::string MCname)
{
    fname = MCname;
}

void HistDrawer_pt2dscan::do_pt2dscan(bool GSPincl)
{
    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }
    
    std::string fname_reco = "~/rootFiles/" + fname + noGSP + "_reco.root";
    std::string fname_ref = "~/rootFiles/" + fname + noGSP + "_ref.root";
    
    HistLoader_pt2dscan HL_reco(fname_reco, "hB_rgkt"); // use true b-jets at truth level
    HistLoader_pt2dscan HL_ref(fname_ref, "hBtag_rgkt"); // use tagged b-jets at reco level

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

        TH2D *h2d_reco = (TH2D *) HL_reco.do_rgkt_projection(ptrange[i], "h2d_reco");
        TH2D *h2d_ref = (TH2D *) HL_ref.do_rgkt_projection(ptrange[i], "h2d_ref");
        
        TLine *line = new TLine(0.91, 0, 5, 0);
        line->SetLineWidth(2);

        TPaveText *info = new TPaveText(0.4, 0.7, 0.8, 0.85, "ndc");
        info->AddText((fname + " b-jets").c_str());
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
    
    string savename_c = fname + "_bjets_ref_vs_reco" + noGSP + ".png";
    c->Print(savename_c.c_str());
}