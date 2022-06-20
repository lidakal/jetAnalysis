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

class HistDrawer_pt2dscan
{
    public:
        std::string fname;

        // ---- Constructors ----
        HistDrawer_pt2dscan() {};
        HistDrawer_pt2dscan(std::string MCname);

        void draw_pt2dscan(bool GSPincl);
		void draw_1d_projection(bool GSPincl, std::string var = "rg", Float_t *ptrange = nullptr, Float_t *ktrange = nullptr);
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
    
    HistLoader_pt2dscan HL_reco(fname_reco, "hBtag_rgkt"); // use tagged b-jets at truth level
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

        TH2D *h2d_reco = (TH2D *) HL_reco.do_2d_projection("yx", ptrange[i], "h2d_reco");
        TH2D *h2d_ref = (TH2D *) HL_ref.do_2d_projection("yx", ptrange[i], "h2d_ref");
        
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

		h2d_ref->GetZaxis()->SetRangeUser(zmin, zmax);
        h2d_ref->Draw("colz");
        line->Draw();
        info_ref->Draw();
        gsptxt->Draw();

        c->cd(i + 1 + npt);
        c->cd(i + 1 + npt)->SetGrid();
    
		h2d_reco->GetZaxis()->SetRangeUser(zmin, zmax);
        h2d_reco->Draw("colz");
        line->Draw();
        info_reco->Draw();
        gsptxt->Draw();
    }

    c->Draw();
    
    std::string savename_c = fname + "_bjets_ref_vs_reco" + noGSP + ".png";
    c->Print(savename_c.c_str());
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

	std::string fname_merged_ref = "~/rootFiles/aggregateB_truthInfo_fixedBugs" + noGSP + "_ref.root"; // bjet truth merge
    std::string fname_merged_truth_reco = "~/rootFiles/aggregateB_truthInfo_fixedBugs" + noGSP + "_reco.root"; // bjet reco merge with truth info

    std::string fname_chargedSJ_bJetMC_reco = "~/rootFiles/chargedSJ_new_bJetMC" + noGSP + "_reco.root"; // bjet reco no merge

    // New selections can be added here
    std::string fname_merged_reco = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco" + noGSP + "_reco.root"; // bjet reco merge only SV
    std::string fname_merged_highestEfficiency_reco = "~/rootFiles/aggregateB_highestEfficiency" + noGSP + "_reco.root"; // bjet reco merge (inSV OR (|3D IP/#sigma| > 3))
    std::string fname_merged_ip3dSigOnly_reco = "~/rootFiles/aggregateB_ip3dSigOnly" + noGSP + "_reco.root"; // bjet reco merge (|3D IP/#sigma| > 3)
    std::string fname_merged_SV_ip3dSig_tight_reco = "~/rootFiles/aggregateB_SV_ip3dSig_tight" + noGSP + "_reco.root"; // bjet reco merge ((inSV AND (|3D IP/#sigma| > 3)) OR (|3D IP/#sigma| > 9)) 

    THStack *hs = new THStack("hs", "");

    TLegend *leg = new TLegend(0.55, 0.65, 0.85, 0.9);
    leg->SetBorderSize(1);
    gStyle->SetLegendTextSize(15);
    leg->SetHeader("Track aggregation criteria:");
    leg->SetMargin(0.1);

    TLegendEntry *header = (TLegendEntry*) leg->GetListOfPrimitives()->First();
    header->SetTextAlign(22);
    //header->SetTextColor(2);
    header->SetTextSize(17);

    std::string xtitle;
    if (var == "rg") {
        xtitle = "ln(1/R_{g})";
    } else if (var == "kt") {
        xtitle = "ln(k_{T})";
    } else if (var == "zg") {
        xtitle = "z_{g}";
    }
    std::string ytitle = "1/N_{2-prong jets} dN/d" + xtitle;

    HistLoader_pt2dscan HL_ljet_ref(fname_chargedSJ_qcdMC_ref, Form("hL_%skt", var.c_str()));
    TH1D *h1d_ljet_ref = HL_ljet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_ljet_ref", xtitle, ytitle);
    h1d_ljet_ref->SetLineColor(kBlack);
    h1d_ljet_ref->SetLineStyle(2);
    //leg->AddEntry(h1d_ljet_ref, "l-jets, truth", "l");
    //hs->Add(h1d_ljet_ref);

    HistLoader_pt2dscan HL_ljet_reco(fname_chargedSJ_qcdMC_reco, Form("hL_%skt", var.c_str()));
    TH1D *h1d_ljet_reco = HL_ljet_reco.do_1d_projection("x", ptrange, ktrange, "h1d_ljet_reco", xtitle, ytitle);
    h1d_ljet_reco->SetLineColor(kGray);
    h1d_ljet_reco->SetLineStyle(2);
    //leg->AddEntry(h1d_ljet_reco, "l-jets, reco", "l");
    //hs->Add(h1d_ljet_reco);

    HistLoader_pt2dscan HL_bjet_ref(fname_merged_ref, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_ref = HL_bjet_ref.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_ref", xtitle, ytitle);
    h1d_bjet_ref->SetLineColor(mykBlue);
    h1d_bjet_ref->SetLineWidth(2);
    h1d_bjet_ref->SetLineStyle(1);
    //leg->AddEntry(h1d_bjet_ref, "b-jets, truth, aggregated pseudo-B's", "l");
    //hs->Add(h1d_bjet_ref);

    HistLoader_pt2dscan HL_bjet_reco_truth(fname_merged_truth_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_truth = HL_bjet_reco_truth.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_truth", xtitle, ytitle);
    h1d_bjet_reco_truth->SetLineColor(mykBlue);
    h1d_bjet_reco_truth->SetLineWidth(2);
    h1d_bjet_reco_truth->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_truth, "Truth level information (IDEAL CASE)", "l");
    hs->Add(h1d_bjet_reco_truth);

    HistLoader_pt2dscan HL_bjet_reco(fname_chargedSJ_bJetMC_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco = HL_bjet_reco.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco", xtitle, ytitle);
    h1d_bjet_reco->SetLineColor(mykRed);
    h1d_bjet_reco->SetLineWidth(2);
    h1d_bjet_reco->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco, "No aggregation", "l");
    hs->Add(h1d_bjet_reco);

    HistLoader_pt2dscan HL_bjet_reco_SV(fname_merged_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_SV = HL_bjet_reco_SV.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_SV", xtitle, ytitle);
    h1d_bjet_reco_SV->SetLineColor(mykGreenLight);
    h1d_bjet_reco_SV->SetLineWidth(2);
    h1d_bjet_reco_SV->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_SV, "inSV", "l");
    hs->Add(h1d_bjet_reco_SV);    

    HistLoader_pt2dscan HL_bjet_reco_ip3dSigOnly(fname_merged_ip3dSigOnly_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_ip3dSigOnly = HL_bjet_reco_ip3dSigOnly.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_ip3dSigOnly", xtitle, ytitle);
    h1d_bjet_reco_ip3dSigOnly->SetLineColor(mykMagentaLight);
    h1d_bjet_reco_ip3dSigOnly->SetLineWidth(2);
    h1d_bjet_reco_ip3dSigOnly->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_ip3dSigOnly, "|3D IP/#sigma| > 3", "l");
    hs->Add(h1d_bjet_reco_ip3dSigOnly); 

    HistLoader_pt2dscan HL_bjet_reco_highestEfficiency(fname_merged_highestEfficiency_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_highestEfficiency = HL_bjet_reco_highestEfficiency.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_highestEfficiency", xtitle, ytitle);
    h1d_bjet_reco_highestEfficiency->SetLineColor(mykOrangeLight);
    h1d_bjet_reco_highestEfficiency->SetLineWidth(2);
    h1d_bjet_reco_highestEfficiency->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_highestEfficiency, "inSV OR |3D IP/#sigma| > 3", "l");
    hs->Add(h1d_bjet_reco_highestEfficiency);

    HistLoader_pt2dscan HL_bjet_reco_SV_ip3dSig_tight(fname_merged_SV_ip3dSig_tight_reco, Form("hBtag_%skt", var.c_str()));
    TH1D *h1d_bjet_reco_SV_ip3dSig_tight = HL_bjet_reco_SV_ip3dSig_tight.do_1d_projection("x", ptrange, ktrange, "h1d_bjet_reco_SV_ip3dSig_tight", xtitle, ytitle);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineColor(mykCyanLight);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineWidth(2);
    h1d_bjet_reco_SV_ip3dSig_tight->SetLineStyle(1);
    leg->AddEntry(h1d_bjet_reco_SV_ip3dSig_tight, "(inSV AND |3D IP/#sigma| > 3) OR |3D IP/#sigma| > 9", "l");
    hs->Add(h1d_bjet_reco_SV_ip3dSig_tight);     

	hs->SetTitle(Form("; %s; %s", xtitle.c_str(), ytitle.c_str()));

    TCanvas *c = new TCanvas("c", "", 1200, 800);
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

    info->AddText(Form("%.0f < #it{p_{T}^{jet}} < %0.f (GeV)", ptmin, ptmax));
    info->AddText("-2 < #it{#eta^{jet}} < 2");
    info->AddText("Reconstructed #it{b}-tagged #it{b}-jets");
    //info->AddText("that originate from #it{b} quarks");
    if (!GSPincl) {
	  //info->AddText("truth doesn't pass b-tag, NO GSP");
      //info->AddText("excluding #it{g#rightarrow b#bar{b}} inside the jet");
    }
	//info->AddText(Form("%0.f < ln(k_{T}) < %.0f", ktmin, ktmax));
    info->Draw();

    TPaveText *mcinfo = new TPaveText(0.15, 0.9, 0.4, 0.95, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");
    mcinfo->Draw();

    c->SetGrid(1);
    c->Draw();

	std::string savename = var + "_projections_1ptBin" + noGSP + ".png";
	c->Print(savename.c_str());
}
