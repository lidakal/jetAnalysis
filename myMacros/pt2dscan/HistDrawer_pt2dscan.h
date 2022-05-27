#include "HistLoader_pt2dscan.h"

#include "TH2D.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLine.h"

class HistDrawer_pt2dscan
{
    public:
        std::string fname;

        // ---- Constructors ----
        HistDrawer_pt2dscan() {};
        HistDrawer_pt2dscan(std::string MCname);

        void draw_pt2dscan(bool GSPincl);
		void draw_rg_projection(bool GSPincl);
};

HistDrawer_pt2dscan::HistDrawer_pt2dscan(std::string MCname)
{
    fname = MCname;
}

void HistDrawer_pt2dscan::draw_pt2dscan(bool GSPincl)
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
    
    std::string savename_c = fname + "_bjets_ref_vs_reco" + noGSP + ".png";
    c->Print(savename_c.c_str());
}

void HistDrawer_pt2dscan::draw_rg_projection(bool GSPincl)
{
    std::string noGSP = "";
    if (!GSPincl) {
        noGSP += "_noGSP";
    }

    // These do not change
    std::string fname_chargedSJ_qcdMC_ref = "~/rootFiles/chargedSJ_new_qcdMC" + noGSP + "_ref.root"; // ljet truth
    std::string fname_chargedSJ_qcdMC_reco = "~/rootFiles/chargedSJ_new_qcdMC" + noGSP + "_reco.root"; // ljet reco
    std::string fname_chargedSJ_bJetMC_reco = "~/rootFiles/chargedSJ_new_bJetMC" + noGSP + "_reco.root"; // bjet reco no merge
	std::string fname_merged_ref = "~/rootFiles/aggregateB_ip3dSig_looserCut" + noGSP + "_ref.root"; // bjet truth merge

    // New selections can be added here
    std::string fname_merged_reco = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco" + noGSP + "_reco.root"; // bjet reco merge only SV
    std::string fname_merged_ip3dSig_looser_reco = "~/rootFiles/aggregateB_ip3dSig_looserCut" + noGSP + "_reco.root"; // bjet reco merge ((inSV && (|ip3dSig| > 3)) || ((!inSV) && (9 < ip3dSig < 100)))
    std::string fname_merged_ip3dSig_reco = "~/rootFiles/aggregateB_ip3dSig" + noGSP + "_reco.root"; // bjet reco merge (tight cut)

    // Load histograms
    Float_t ptrange[2] = {100., 150.};
    THStack *hs = new THStack("hs", "");
    TLegend *leg = new TLegend(0.5, 0.5, 0.8, 0.8);
    gStyle->SetLegendTextSize(15);

    HistLoader_pt2dscan HL_ljet_ref(fname_chargedSJ_qcdMC_ref, "hL_rgkt");
    TH1D *h1d_ljet_ref = HL_ljet_ref.do_rg_projection(ptrange, "h1d_ljet_ref");
    h1d_ljet_ref->SetLineColor(1);
    h1d_ljet_ref->SetLineStyle(2);
    leg->AddEntry(h1d_ljet_ref, "l-jets, truth", "l");
    hs->Add(h1d_ljet_ref);

    HistLoader_pt2dscan HL_ljet_reco(fname_chargedSJ_qcdMC_reco, "hL_rgkt");
    TH1D *h1d_ljet_reco = HL_ljet_reco.do_rg_projection(ptrange, "h1d_ljet_reco");
    h1d_ljet_reco->SetLineColor(2);
    h1d_ljet_reco->SetLineStyle(2);
    leg->AddEntry(h1d_ljet_reco, "l-jets, reco", "l");
    hs->Add(h1d_ljet_reco);

    HistLoader_pt2dscan HL_bjet_ref(fname_merged_ref, "hB_rgkt");
    TH1D *h1d_bjet_ref = HL_bjet_ref.do_rg_projection(ptrange, "h1d_bjet_ref");
    h1d_bjet_ref->SetLineColor(4);
    h1d_bjet_ref->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_ref, "b-jets, truth, aggregated pseudo-B's", "l");
    hs->Add(h1d_bjet_ref);

    HistLoader_pt2dscan HL_bjet_reco(fname_chargedSJ_bJetMC_reco, "hBtag_rgkt");
    TH1D *h1d_bjet_reco = HL_bjet_reco.do_rg_projection(ptrange, "h1d_bjet_reco");
    h1d_bjet_reco->SetLineColor(6);
    h1d_bjet_reco->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco, "b-jets, reco, no aggregation", "l");
    hs->Add(h1d_bjet_reco);

    HistLoader_pt2dscan HL_bjet_reco_SV(fname_merged_reco, "hBtag_rgkt");
    TH1D *h1d_bjet_reco_SV = HL_bjet_reco_SV.do_rg_projection(ptrange, "h1d_bjet_reco_SV");
    h1d_bjet_reco_SV->SetLineColor(8);
    h1d_bjet_reco_SV->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_SV, "b-jets, reco, aggregation if inSV", "l");
    hs->Add(h1d_bjet_reco_SV);

    HistLoader_pt2dscan HL_bjet_reco_ip3dSig(fname_merged_ip3dSig_reco, "hBtag_rgkt");
    TH1D *h1d_bjet_reco_ip3dSig = HL_bjet_reco_ip3dSig.do_rg_projection(ptrange, "h1d_bjet_reco_ip3dSig");
    h1d_bjet_reco_ip3dSig->SetLineColor(9);
    h1d_bjet_reco_ip3dSig->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_ip3dSig, "b-jets, reco, aggregation with ip3dSig", "l");
    hs->Add(h1d_bjet_reco_ip3dSig);

    HistLoader_pt2dscan HL_bjet_reco_ip3dSig_looser(fname_merged_ip3dSig_looser_reco, "hBtag_rgkt");
    TH1D *h1d_bjet_reco_ip3dSig_looser = HL_bjet_reco_ip3dSig_looser.do_rg_projection(ptrange, "h1d_bjet_reco_ip3dSig_looser");
    h1d_bjet_reco_ip3dSig_looser->SetLineColor(9);
    h1d_bjet_reco_ip3dSig_looser->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_ip3dSig_looser, "b-jets, reco, aggregation with ip3dSig - looserCut", "l");
    hs->Add(h1d_bjet_reco_ip3dSig_looser);    

    TCanvas *c = new TCanvas("c", "", 800, 800);
    hs->Draw("nostack hist");
    leg->Draw();

    TPaveText *info = new TPaveText(0.2, 0.17, 0.45, 0.28, "ndc");
    info->SetBorderSize(1);
    info->SetFillColor(0);
    info->SetTextSize(15);
    info->AddText(Form("%.0f < p_{T}^{jet} < %0.f (GeV)", ptrange[0], ptrange[1]));
    if (!GSPincl) {
        info->AddText("NO GSP");
    }
    info->Draw();

    c->Draw();
}
