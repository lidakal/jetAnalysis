#include "../binning.h"

void calc_sf_unc(TString observable="rg")
{
    gStyle->SetErrorX(0.5);
    gStyle->SetCanvasPreferGL(kTRUE);


    TString fin_nom_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_nom.root";
    TString fin_gspUp_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_gspUp50.root";
    TString fin_gspDown_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_gspDown50.root";
    TString fin_muonJets_name = "histos/variations/aggrTMVA_inclusive_"+observable+"_sfs_Mu5.root";

    for (auto name : {
        fin_nom_name,
        fin_gspUp_name,
        fin_gspDown_name,
        fin_muonJets_name
    }) {
        std::cout << "fin: " << name << std::endl;
    }

    TFile *fin_nom = new TFile(fin_nom_name);
    TH2D *h_sf_nom = (TH2D *) fin_nom->Get("h_eff_sf")->Clone("h_sf_nom");
    TFile *fin_gspUp = new TFile(fin_gspUp_name);
    TH2D *h_sf_gspUp = (TH2D *) fin_gspUp->Get("h_eff_sf")->Clone("h_sf_gspUp");
    TFile *fin_gspDown = new TFile(fin_gspDown_name);
    TH2D *h_sf_gspDown = (TH2D *) fin_gspDown->Get("h_eff_sf")->Clone("h_sf_gspDown");
    TFile *fin_muonJets = new TFile(fin_muonJets_name);
    TH2D *h_sf_muonJets = (TH2D *) fin_muonJets->Get("h_eff_sf")->Clone("h_sf_muonJets");

    // No need to normalize, SFs are absolute values

    TH2D *h_gspUp_unc = (TH2D *) h_sf_gspUp->Clone("h_gspUp_unc");
    h_gspUp_unc->Add(h_sf_nom, -1);

    TH2D *h_gspDown_unc = (TH2D *) h_sf_gspDown->Clone("h_gspDown_unc");
    h_gspDown_unc->Add(h_sf_nom, -1);

    TH2D *h_muonJets_unc = (TH2D *) h_sf_muonJets->Clone("h_muonJets_unc");
    h_muonJets_unc->Add(h_sf_nom, -1);

    TH2D *h_sf_unc_up = (TH2D *) h_sf_nom->Clone("h_sf_unc_up");
    h_sf_unc_up->Reset();
    TH2D *h_sf_unc_down = (TH2D *) h_sf_unc_up->Clone("h_sf_unc_down");
    for (int ix=1; ix<=h_sf_nom->GetNbinsX(); ix++) {
        for (int iy=1; iy<=h_sf_nom->GetNbinsY(); iy++) {
            double gspUp_unc = h_gspUp_unc->GetBinContent(ix, iy);
            double gspDown_unc = h_gspDown_unc->GetBinContent(ix, iy);
            double muonJets_unc = h_muonJets_unc->GetBinContent(ix, iy);

            double total_unc_up2 = 0;
            double total_unc_down2 = 0;

            if (gspUp_unc>0) total_unc_up2 += gspUp_unc*gspUp_unc;
            else total_unc_down2 += gspUp_unc*gspUp_unc;
            if (gspDown_unc>0) total_unc_up2 += gspDown_unc*gspDown_unc;
            else total_unc_down2 += gspDown_unc*gspDown_unc;
            if (muonJets_unc>0) total_unc_up2 += muonJets_unc*muonJets_unc;
            else total_unc_down2 += muonJets_unc*muonJets_unc;

            h_sf_unc_up->SetBinContent(ix,iy,std::sqrt(total_unc_up2));
            h_sf_unc_down->SetBinContent(ix,iy,-std::sqrt(total_unc_down2));
        }
    }

    int nbins = 0;
    double *vbins;
    if (observable=="rg") {
        nbins=logrg_bins;
        vbins = logrg_binsVector;
    }
    if (observable=="zg") {
        nbins=zg_bins;
        vbins = zg_binsVector;
    }
    if (observable=="zpt") {
        nbins=zpt_bins;
        vbins = zpt_binsVector;
    }
    TH2D *h_sf_unc_up_3bins = new TH2D("h_sf_unc_up_3bins", "", nbins, vbins, jtpt_bins, jtpt_binsVector);
    TH2D *h_sf_unc_down_3bins = new TH2D("h_sf_unc_down_3bins", "", nbins, vbins, jtpt_bins, jtpt_binsVector);
    for (int ix=1; ix<=h_sf_unc_up_3bins->GetNbinsX();ix++) {
        for (int iy=1; iy<=h_sf_unc_up_3bins->GetNbinsX();iy++) {
            double unc_up= 0;
            double unc_down= 0;
            if (h_sf_nom->GetNbinsY()==1) {
                unc_up = h_sf_unc_up->GetBinContent(ix, 1);
                unc_down = h_sf_unc_down->GetBinContent(ix, 1);
            } else {
                unc_up = h_sf_unc_up->GetBinContent(ix, iy);
                unc_down = h_sf_unc_down->GetBinContent(ix, iy);
            }
            h_sf_unc_up_3bins->SetBinContent(ix,iy,unc_up);
            h_sf_unc_up_3bins->SetBinError(ix,iy,0.);
            h_sf_unc_down_3bins->SetBinContent(ix,iy,unc_down);
            h_sf_unc_down_3bins->SetBinError(ix,iy,0.);
        }
    }

    TString fout_name = "histos/aggrTMVA_inclusive_"+observable+"_sf_unc.root";
    std::cout <<"fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    h_sf_unc_up->Write();
    h_sf_unc_down->Write();
    h_sf_unc_up_3bins->Write();
    h_sf_unc_down_3bins->Write();
}
