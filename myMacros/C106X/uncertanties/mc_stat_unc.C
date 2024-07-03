#include "../draw_utils.h"

double rmsValue(std::vector<double> arr)
{
    double square = 0;
    double mean = 0.0, root = 0.0;

    int n = arr.size();
 
    // Calculate square.
    for (int i = 0; i < n; i++) {
        square += pow(arr[i], 2);
    }
 
    // Calculate Mean.
    mean = (square / (double)(n));
 
    // Calculate Root.
    root = sqrt(mean);
 
    return root;
}

double stdValue(std::vector<double> arr, double mean) {
    int n = arr.size();

    // double sum = 0.;
    // for(int i = 0; i < n; ++i) {
    //     sum += arr[i];
    // }

    // double mean = sum / n;

    double standardDeviation = 0.;
    for(int i = 0; i < n; ++i) {
        standardDeviation += pow(arr[i] - mean, 2);
    }

    return sqrt(standardDeviation / n);
}

void mc_stat_unc(TString observable="rg")
{
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch}";
    TString ylabel = "1/N dN/d" + xlabel;

    // JK RESAMPLING METHOD
    TString fin_name = "../unfolding/histos/pythia_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root";
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);
    
    TH2D *h_nom = (TH2D *) fin->Get("h_data_unfolded");

    TH2D *h_mc_stat_unc = (TH2D *) h_nom->Clone("h_mc_stat_unc");
    h_mc_stat_unc->Reset();

    int nbins_x = h_nom->GetNbinsX();
    int nbins_pt = h_nom->GetNbinsY();
    int ibin_x_min = 1;
    int ibin_x_max = nbins_x;
    if (observable!="zpt") ibin_x_min = 2;
    if (observable=="rg") ibin_x_max = nbins_x - 1;

    h_nom->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
    h_nom->Scale(1/h_nom->Integral(), "width");

    TH2D *h_jk_0, 
        *h_jk_1,
        *h_jk_2,
        *h_jk_3,
        *h_jk_4,
        *h_jk_5,
        *h_jk_6,
        *h_jk_7,
        *h_jk_8,
        *h_jk_9;

    std::vector<TH2D *> histos_jk = {
        h_jk_0,
        h_jk_1,
        h_jk_2,
        h_jk_3,
        h_jk_4,
        h_jk_5,
        h_jk_6,
        h_jk_7,
        h_jk_8,
        h_jk_9,
    };

    for (int i=0; i<10; i++) {
        histos_jk[i] = (TH2D *) fin->Get(Form("h_fully_corrected_jk_%d",i));
        histos_jk[i]->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        histos_jk[i]->Scale(1/histos_jk[i]->Integral(), "width");
    }

    TH2D *h_rms = (TH2D *) h_nom->Clone("h_rms");
    h_rms->Reset();

    for (int ibin_pt=1; ibin_pt<=nbins_pt; ibin_pt++) {
        for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
            std::vector<double> jk_contents;
            for (int i=0; i<10; i++) {
                jk_contents.push_back(histos_jk[i]->GetBinContent(ibin_x, ibin_pt));
            }
            double rms = rmsValue(jk_contents);
            double nom_val = h_nom->GetBinContent(ibin_x, ibin_pt);
            double std_val = stdValue(jk_contents, nom_val);
            // std::cout << std_val << std::endl;
            std_val *= 10./9.;

            // h_mc_stat_unc->SetBinContent(ibin_x, ibin_pt, (rms-nom_val)*10./9.);
            h_mc_stat_unc->SetBinContent(ibin_x, ibin_pt, std_val);
            h_rms->SetBinContent(ibin_x, ibin_pt, rms);
        }
    }

    TH2D *h_mc_stat_unc_rel = (TH2D *) h_mc_stat_unc->Clone("h_mc_stat_unc_rel");
    h_mc_stat_unc_rel->Divide(h_mc_stat_unc, h_nom);

 
    TString fout_name = "./histos/"+observable+"_mc_stat_unc_XXT.root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");
    
    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        if (ibin_pt!=2) continue;

        double pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);

        // Extract uncertainties 
        TH1D *h_mc_stat_unc_up_rel = (TH1D *) h_mc_stat_unc_rel->ProjectionX(Form("h_mc_stat_unc_up_rel_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_mc_stat_unc_down_rel = (TH1D *) h_mc_stat_unc_up_rel->Clone(Form("h_mc_stat_unc_down_rel_%d", ibin_pt));
        h_mc_stat_unc_down_rel->Scale(-1.);

        h_mc_stat_unc_up_rel->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h_mc_stat_unc_down_rel->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h_mc_stat_unc_up_rel->GetYaxis()->SetRangeUser(-0.2, 0.2);
        h_mc_stat_unc_up_rel->GetXaxis()->SetTitle(xlabel);
        h_mc_stat_unc_up_rel->GetYaxis()->SetTitle("Relative MC stat unc");
        

        TCanvas *c = new TCanvas("c","",800,600);
        TPad *pad12 = new TPad("pad12", "", 0., 0., 1., 0.33);
        TPad *pad22 = new TPad("pad22", "", 0., 0.33, 1., 1.);

        pad12->SetTopMargin(0.08);
        pad12->SetBottomMargin(0.3);
        pad22->SetBottomMargin(0.01);

        pad22->cd();
        TLegend *leg = new TLegend(0.4, 0.55, 0.8, 0.85);
        if (observable=="zpt") leg = new TLegend(0.2, 0.55, 0.6, 0.85);
        leg->SetNColumns(2);

        TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX(Form("h_nom_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_nom_1d->SetTitle("nominal");
        h_nom_1d->GetYaxis()->SetRangeUser(0., 0.018);
        if (observable=="zg") h_nom_1d->GetYaxis()->SetRangeUser(0., 0.07);
        if (observable=="zpt") h_nom_1d->GetYaxis()->SetRangeUser(0., 0.04);
        h_nom_1d->GetYaxis()->SetTitle(ylabel);
        h_nom_1d->SetLineWidth(3);
        h_nom_1d->Draw("hist");
        leg->AddEntry(h_nom_1d, "nominal", "l");

        TH1D *h_ghost = (TH1D *) h_nom_1d->Clone("h_ghost");
        h_ghost->SetLineColor(kWhite);
        leg->AddEntry(h_ghost, " ", "l");
        leg->SetHeader("Response stat variations");

        // TH1D *h_rms_1d = (TH1D *) h_rms->ProjectionX(Form("h_rms_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        // h_rms_1d->SetLineColor(kMagenta+2);
        // h_rms_1d->SetLineWidth(2);
        // h_rms_1d->Draw("hist same");
        // leg->AddEntry(h_rms_1d, "RMS", "l");

        for (int i=0; i<10; i++) {
            pad22->cd();
            TH1D *h_var_1d = (TH1D *) histos_jk[i]->ProjectionX(Form("h_var_1d_%d_%d", ibin_pt, i), ibin_pt, ibin_pt);
            h_var_1d->SetTitle(Form("variation i=%d",i+1));
            h_var_1d->SetLineColor(i+2);
            if (i==8) h_var_1d->SetLineColor(38);
            h_var_1d->Draw("hist same");
            leg->AddEntry(h_var_1d, Form("variation i=%d",i+1), "l");

            pad12->cd();
            TH1D *h_diff_1d = (TH1D *) h_var_1d->Clone("h_diff_1d");
            h_diff_1d->Add(h_nom_1d, -1.);
            h_diff_1d->Divide(h_nom_1d);
            // h_diff_1d->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
            h_diff_1d->GetYaxis()->SetRangeUser(-0.25, 0.25);
            if (observable=="zg") h_diff_1d->GetYaxis()->SetRangeUser(-0.1, 0.1);
            if (observable=="zpt") h_diff_1d->GetYaxis()->SetRangeUser(-0.1, 0.1);
            h_diff_1d->GetYaxis()->SetNdivisions(5);
            h_diff_1d->GetXaxis()->SetTitle(xlabel);
            h_diff_1d->GetYaxis()->SetTitle("Relative MC stat unc");
            h_diff_1d->GetXaxis()->SetTitleOffset(3.5);

            h_diff_1d->Draw("hist same");
        }
        pad22->cd();
        leg->Draw();
        drawHeaderWIP();

        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(20);
        jet_info->DrawLatex(0.2, 0.28, "anti-k_{T}, R=0.4 single b jets");
        jet_info->DrawLatex(0.2, 0.22, Form("%.0f < p_{T}^{jet} < %.0f GeV", pt_min, pt_max));
        jet_info->DrawLatex(0.2, 0.16, "Charged Soft Drop emissions");
        jet_info->DrawLatex(0.2, 0.1, "z_{cut}=0.1, #beta=0, p_{T}^{trk} > 1 GeV");

        pad12->cd();
        TH1D *h_mc_stat_unc_1d = (TH1D *) h_mc_stat_unc->ProjectionX(Form("h_mc_stat_unc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_mc_stat_unc_1d->SetLineColor(kMagenta+2);
        h_mc_stat_unc_1d->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h_mc_stat_unc_1d->SetLineWidth(3);
        // h_mc_stat_unc_1d->Draw("hist same");
        h_mc_stat_unc_up_rel->GetYaxis()->SetNdivisions(8);
        h_mc_stat_unc_up_rel->GetXaxis()->SetTitleOffset(3.5);
        h_mc_stat_unc_up_rel->SetLineWidth(2);
        h_mc_stat_unc_down_rel->SetLineWidth(2);
        h_mc_stat_unc_up_rel->Draw("hist same");
        h_mc_stat_unc_down_rel->Draw("hist same");

        c->cd();
        pad12->Draw();
        pad22->Draw();


        
        h_mc_stat_unc_up_rel->Write();
        h_mc_stat_unc_down_rel->Write();

        c->Draw();
        c->Print("plots_an/mc_stat_unc_XXT_"+observable+".png");
    }
}
