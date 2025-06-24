// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldInvert.h"
#include "../draw_utils.h"

std::vector<TString> variations = {
    "FSRup", "FSRdown",
    "ISRup", "ISRdown",
    "Bothup", "Bothdown",
};

void bottomline_test_pythia_var(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{
    // ---------------- Options -------------------
    bool withSF = true; // don't change
    bool sfDown = false;
    bool sfUp = false;

    TString suffix_in = "_jer_" + jer_opt + "_jec_" + jec_opt;

    TString sample_data = "data_PF40to100";
    TString label_data = "aggrTMVA_XXT";

    bool unfoldBayes = false;

    TString sample_truth = "pythia_PF40"; // sample used for comparing to truth
    TString label_truth = "aggrTMVA_XXT";
    TString sample_unfolding = "pythia_PF40"; // sample used for unfolding
    TString label_unfolding = "aggrTMVA_XXT";

    TString sample_sf = "pythia_PF40"; // no herwig-extracted SFs
    TString label_sf = "aggrTMVA_XXT"; // always the label for corrections

    TString suffix_out = suffix_in;
    suffix_out += "_pythia_var";
    if (withSF) suffix_out += "_withSF";
    if (sfUp) suffix_out += "Up";
    if (sfDown) suffix_out += "Down";

    int ibin_pt = 2;

    std::cout << "Options:"
              << "\n\twithSF:" << withSF
              << "\n\tsfUp:" << sfUp
              << "\n\tsfDown:" << sfDown
              << "\n\tunfoldBayes:" << unfoldBayes
              << "\n\tibin_pt:" << ibin_pt
              << std::endl;

    TString fout_name = "./histos/"+sample_unfolding+"_"+label_unfolding+"_unfolded_histograms_"+observable+suffix_out+".root";
    // --------------------------------------------


    // ---------------- Plotting setup ------------
    gSystem->Load("libRooUnfold.so");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";
    TString ylabel = "1/N dN/d" + xlabel;

    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    // --------------------------------------------


    // ----------- Grab data (b tagged) -----------
    TString fname_data = "../template_fit/histos/"+sample_data+"_"+label_data+"_histograms.root";
    std::cout << "Getting data from " << fname_data << std::endl;
    TFile *fin_data = new TFile(fname_data);
    TH3D *h_data = (TH3D *) fin_data->Get("h_data_" + observable);
    h_data->GetYaxis()->SetRange(0, h_data->GetNbinsY() + 1); // all mb bins

    int nbins_x = h_data->GetNbinsX();
    int nbins_pt = h_data->GetNbinsZ();
    int dim = nbins_pt*nbins_x;

    int ibin_x_min = 1;
    int ibin_x_max = nbins_x;

    double pt_min = h_data->GetZaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_data->GetZaxis()->GetBinUpEdge(ibin_pt);

    double x_min = h_data->GetXaxis()->GetBinLowEdge(1);
    double x_max = h_data->GetXaxis()->GetBinUpEdge(nbins_x);
    
    TH2D *h_data_reco = (TH2D *) h_data->Project3D("zx");
    h_data_reco->GetXaxis()->SetRange(1, h_data_reco->GetNbinsX());
    h_data_reco->GetYaxis()->SetRange(1, h_data_reco->GetNbinsY());

    // Grab signal fraction from template fit
    TString fit_option = "_glued";
    TString fname_fit = "../template_fit/histos/fitted_parameters_RooFit_data_"+label_data+"_" + observable + fit_option + "_jer_nom_jec_nom.root";
    std::cout << "Getting signal fraction from " << fname_fit << std::endl;
    TFile *fin_fit = new TFile(fname_fit);
    TH2D *h_sig_fraction = (TH2D *) fin_fit->Get("h_sig_fraction");    

    // Multiply histograms by signal fraction
    std::cout << "\t---->Multiplying by signal fraction" << std::endl;
    TH2D *h_data_after_fit = (TH2D *) h_data_reco->Clone("h_data_after_fit");
    h_data_after_fit->Multiply(h_sig_fraction);

    // Multiply histograms by SF 
    TFile *fin_sf = new TFile("../btag/histos/aggrTMVA_inclusive_"+observable+"_sfs.root");
    TH2D *h_sf = (TH2D*) fin_sf->Get("h_eff_sf_3bins");
    h_data_after_fit->Divide(h_sf);

    // Note: Result = unfold(raw * purity) * 1 / (efficiency)
    //       fakes are negligible

    // ---- Grab response matrix + corrections
    TString fname_response_unfolding = "./histos/"+sample_unfolding+"_"+label_unfolding+"_response" + suffix_in + "_pythia_variations.root";
    std::cout << "Getting response + corrections from : " << fname_response_unfolding << std::endl;
    TFile *fin_response_unfolding = new TFile(fname_response_unfolding);

    TString fname_response_truth = "./histos/"+sample_truth+"_"+label_truth+"_response" + suffix_in + "_pythia_variations.root";
    std::cout << "Getting truth from : " << fname_response_truth << std::endl;
    TFile *fin_response_truth = new TFile(fname_response_truth);

    TString fname_b_tag_eff = "../btag/histos/"+sample_sf+"_"+label_sf+"_" +observable + "_efficiency" + suffix_in + "_withSF.root";
    std::cout << "Getting b tagging efficiency from: " << fname_b_tag_eff << std::endl;
    TFile *fin_b_tag_eff = new TFile(fname_b_tag_eff);
    
    // Loop over the variations
    std::vector<TH2D *> unfolded_histos = {};
    for (int ivar=0; ivar<6; ivar++) {
        // if (i>0) break;

        TString var = variations[ivar];
        std::cout << "\n***" << var << std::endl;

        TH2D *h_mc_reco = (TH2D *) fin_response_unfolding->Get(Form("h_%s_purity_numerator_%spt", var.Data(), observable.Data())); // reco MC to compare w/ data after purity correction
        TH2D *h_purity = (TH2D *) fin_response_unfolding->Get(Form("h_%s_purity_%spt", var.Data(), observable.Data())); // reconstruction purity correction
        TH2D *h_efficiency = (TH2D *) fin_response_unfolding->Get(Form("h_%s_efficiency_%spt", var.Data(), observable.Data())); // reconsturction efficiency correction
        RooUnfoldResponse *response = (RooUnfoldResponse *) fin_response_unfolding->Get(Form("response_%s_%spt", var.Data(), observable.Data())); // response 
        TH2D *h_mc_btag_true = (TH2D *) fin_response_unfolding->Get(Form("h_%s_efficiency_numerator_%spt", var.Data(), observable.Data())); // true MC to compare w/ data BEFORE efficiency corrections

        // ---- Print condition number
        TDecompSVD *svd= new TDecompSVD(response->Mresponse());  // response is a RooUnfold response object, svd is the singular value decomposition (SVD) matrix. the response->Mresponse() returns the normalized migration matrix
        auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
        double cond_number = singular_values.Max() / singular_values.Min();
        std::cout << "\t---->Condition number nominal = " << cond_number
                << std::endl;

        // ---- Grab the truth level MC from non b tagged jets ---- 
        TH2D *h_mc_true = (TH2D *) fin_response_truth->Get(Form("h_%s_efficiency_denominator_%spt", var.Data(), observable.Data())); // true MC to compare w/ data AFTER efficiency corrections

        //------- Apply purity correction
        std::cout << "\t---->Multiplying data by purity" << std::endl;
        TH2D *h_data_purity_corrected = (TH2D *) h_data_after_fit->Clone(Form("h_%s_data_purity_corrected", var.Data()));
        h_data_purity_corrected->Multiply(h_purity);

        // ---- Unfold
        std::cout << "\t---->Unfolding" << std::endl;
        RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
        TH2D *h_data_unfolded;
        TMatrixD covariance_matrix_before_unfolding(dim,dim);
        TMatrixD covariance_matrix_after_unfolding(dim,dim);
        if (unfoldBayes) {
            Int_t niter =  15;
            RooUnfoldBayes unfold(response, h_data_purity_corrected, niter);
            h_data_unfolded = (TH2D *) unfold.Hreco(errorTreatment);
            covariance_matrix_before_unfolding = unfold.GetMeasuredCov();
            covariance_matrix_after_unfolding = unfold.Ereco();
        } else {
            RooUnfoldInvert unfold(response, h_data_purity_corrected);
            h_data_unfolded = (TH2D *) unfold.Hreco(errorTreatment);
            covariance_matrix_before_unfolding = unfold.GetMeasuredCov();
            covariance_matrix_after_unfolding = unfold.Ereco();
        }    
        h_data_unfolded->SetName(Form("h_%s_data_unfolded",var.Data()));

        // ---- Fold back
        std::cout << "\t---->Refolding" << std::endl;
        TH2D *h_data_refolded = (TH2D *) response->ApplyToTruth(h_data_unfolded, Form("h_%s_data_refolded",var.Data()));

        // ---- Apply efficiency correction 
        std::cout << "\t---->Dividing by recostruction efficiency" << std::endl;
        TH2D *h_data_efficiency_corrected = (TH2D *) h_data_unfolded->Clone(Form("h_%s_data_efficiency_corrected",var.Data()));
        h_data_efficiency_corrected->Divide(h_efficiency);

        // ---- Grab b tagging efficiency correction 
        TH2D *h_data_fully_corrected = (TH2D *) h_data_efficiency_corrected->Clone(Form("h_%s_data_fully_corrected",var.Data()));
        std::cout << "\t---->Dividing by b tagging efficiency (with SF)" << std::endl;
        TH2D *h_eff = (TH2D *) fin_b_tag_eff->Get("h_eff"); 
        h_data_fully_corrected->Divide(h_eff);

        unfolded_histos.push_back(h_data_fully_corrected);

        // ---- Quantitative bottomline test
        if (true) {
            // for (int col=0; col<dim; col++) {
            //     int row1=2;
            //     int row2=3;
            //     covariance_matrix_before_unfolding[row1][col]=covariance_matrix_before_unfolding[row2][col];
            // }
            TMatrixD covariance_matrix_before_unfolding_inverted = covariance_matrix_before_unfolding;
            covariance_matrix_before_unfolding_inverted.Invert();
            TMatrixD covariance_matrix_after_unfolding_inverted = covariance_matrix_after_unfolding;
            covariance_matrix_after_unfolding_inverted.Invert();
            // ---- sanity checks ----
            // TMatrixD cc1(dim, dim);
            // cc1.Mult(covariance_matrix_before_unfolding, covariance_matrix_before_unfolding_inverted);
            // cc1.Mult(covariance_matrix_before_unfolding_inverted, covariance_matrix_before_unfolding);
            // cc1.Mult(covariance_matrix_after_unfolding, covariance_matrix_after_unfolding_inverted);
            // cc1.Mult(covariance_matrix_after_unfolding_inverted, covariance_matrix_after_unfolding);
            // cc1.Draw("colz");
            // ------------------------
            TVectorD ydiff_before_unfolding(dim); // data-MC after purity correction
            TVectorD ydiff_after_unfolding(dim); // data-MC after unfolding before efficiency correction
            TVectorD ystat_before_unfolding(dim); // data stat unc
            for (int ix=1; ix<=nbins_x; ix++) {
                for (int jy=1; jy<=nbins_pt; jy++) {
                    // x:ix=1,...,n ; y:jy=1,...,m ; from matrix(ix,jy) to vector(k) => k=(ix-1)+[(jy-1)*n]
                    ydiff_before_unfolding[ix-1+nbins_x*(jy-1)]=(h_data_purity_corrected->GetBinContent(ix,jy)-h_mc_reco->GetBinContent(ix,jy)); 
                    ydiff_after_unfolding[ix-1+nbins_x*(jy-1)]=(h_data_unfolded->GetBinContent(ix,jy)-h_mc_btag_true->GetBinContent(ix,jy)); 
                    ystat_before_unfolding[ix-1+nbins_x*(jy-1)]=h_data_purity_corrected->GetBinError(ix,jy); 
                }
            }

            TVectorD cov_times_ydiff_before_unfolding(dim); // [yn/sn^2 ... y1/s1^2] 
            TVectorD cov_times_ydiff_after_unfolding(dim); // [yn/sn^2 ... y1/s1^2]
            for ( int ix=dim-1; ix>=0; ix--) {
                TMatrixDRow row_before_unfolding(covariance_matrix_before_unfolding_inverted,ix);
                TMatrixDRow row_after_unfolding(covariance_matrix_after_unfolding_inverted,ix);
                double sum_before_unfolding=0;
                double sum_after_unfolding=0;
                for ( int jy=0; jy<dim; jy++) {
                    sum_before_unfolding += row_before_unfolding[jy]*ydiff_before_unfolding[jy];
                    sum_after_unfolding += row_after_unfolding[jy]*ydiff_after_unfolding[jy];
                }
                cov_times_ydiff_before_unfolding[dim-ix-1] = sum_before_unfolding;
                cov_times_ydiff_after_unfolding[dim-ix-1] = sum_after_unfolding;
            }

            double chi2_before_unfolding = 0.;
            double naive_chi2_before_unfolding = 0.;
            double chi2_after_unfolding = 0.;
            double test_chi2 = 0.;
            for ( int ix=0; ix<dim; ix++) {
                chi2_before_unfolding += ydiff_before_unfolding[ix]*cov_times_ydiff_before_unfolding[dim-ix-1];
                chi2_after_unfolding += ydiff_after_unfolding[ix]*cov_times_ydiff_after_unfolding[dim-ix-1];
                naive_chi2_before_unfolding += std::pow(ydiff_before_unfolding[ix], 2) / std::pow(ystat_before_unfolding[ix], 2);

                // std::cout << ydiff_before_unfolding[ix]*cov_times_ydiff_before_unfolding[dim-i-1] 
                //           << ", " 
                //           << ydiff_after_unfolding[ix]*cov_times_ydiff_after_unfolding[dim-i-1] << std::endl;
            }
            // std::cout << "ndof = " << 
            std::cout << Form("naive_chi2_before_unfolding = %f", naive_chi2_before_unfolding) << std::endl;
            std::cout << Form("chi2_before_unfolding = %f", chi2_before_unfolding) << std::endl;
            std::cout << Form("chi2_after_unfolding = %f", chi2_after_unfolding) << std::endl;
            std::cout << Form("chi2_after_unfolding / chi2_before_unfolding = %f", chi2_after_unfolding / chi2_before_unfolding) << std::endl;
        }

        // ---- Graphical bottomline test
        if (true) {
            std::cout << "Performing graphical bottomline test" << std::endl;
            TH1D *h_mc_reco_1d = (TH1D *) h_mc_reco->ProjectionX(Form("h_%s_mc_reco_1d",var.Data()), ibin_pt, ibin_pt);
            TH1D *h_mc_true_1d = (TH1D *) h_mc_true->ProjectionX(Form("h_%s_mc_true_1d",var.Data()), ibin_pt, ibin_pt);
            TH1D *h_data_purity_corrected_1d = (TH1D *) h_data_purity_corrected->ProjectionX(Form("h_%s_data_purity_corrected_1d",var.Data()), ibin_pt, ibin_pt);
            TH1D *h_data_fully_corrected_1d = (TH1D *) h_data_fully_corrected->ProjectionX(Form("h_%s_data_fully_corrected_1d",var.Data()), ibin_pt, ibin_pt);
            TH1D *h_data_refolded_1d = (TH1D *) h_data_refolded->ProjectionX(Form("h_%s_data_refolded_1d",var.Data()), ibin_pt, ibin_pt);

            double ymax = 0.;
            for (auto h : {
                        h_mc_reco_1d, 
                        h_mc_true_1d,
                        h_data_purity_corrected_1d,
                        h_data_fully_corrected_1d,
                        h_data_refolded_1d
                        }) {
                            h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
                            h->Scale(1/h->Integral(), "width");
                            if (h->GetMaximum()>ymax) ymax =  h->GetMaximum();
                        }

            TLegend *leg = new TLegend(0.4, 0.45, 0.9, 0.75);
            if (observable=="zpt") leg = new TLegend(0.2, 0.4, 0.6, 0.8);
            leg->SetFillStyle(0);
            leg->SetBorderSize(0);
            leg->SetMargin(0.15);
            leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));

            h_data_purity_corrected_1d->SetMarkerColor(kBlack);
            h_data_purity_corrected_1d->SetMarkerStyle(kFullCircle);
            h_data_purity_corrected_1d->SetMarkerSize(1);
            h_data_purity_corrected_1d->GetYaxis()->SetRangeUser(0., ymax*1.1);
            h_data_purity_corrected_1d->GetYaxis()->SetTitle(ylabel);
            leg->AddEntry(h_data_purity_corrected_1d, "Detector level data (tagged single b jets)", "pe1");

            h_mc_reco_1d->SetMarkerColor(kRed);
            h_mc_reco_1d->SetLineColor(kRed);
            h_mc_reco_1d->SetMarkerStyle(kFullTriangleUp);
            h_mc_reco_1d->SetMarkerSize(1);
            leg->AddEntry(h_mc_reco_1d, "Detector level MC (tagged single b jets)", "pe1");

            h_data_fully_corrected_1d->SetMarkerColor(kBlue);
            h_data_fully_corrected_1d->SetLineColor(kBlue);
            h_data_fully_corrected_1d->SetMarkerStyle(kOpenCross);
            h_data_fully_corrected_1d->SetMarkerSize(1);
            leg->AddEntry(h_data_fully_corrected_1d, "Unfolded data (single b jets)", "pe1");

            h_mc_true_1d->SetMarkerColor(kRed);
            h_mc_true_1d->SetLineColor(kRed);
            h_mc_true_1d->SetMarkerStyle(kOpenTriangleUp);
            h_mc_true_1d->SetMarkerSize(1);
            leg->AddEntry(h_mc_true_1d, "Particle level MC (single b jets)", "pe1");

            h_data_refolded_1d->SetMarkerColor(kBlue);
            h_data_refolded_1d->SetLineColor(kBlue);
            h_data_refolded_1d->SetMarkerStyle(kFullCross);
            h_data_refolded_1d->SetMarkerSize(1);
            leg->AddEntry(h_data_refolded_1d, "Refolded data", "pe1");

            TCanvas *c_unfold = new TCanvas(Form("c_%s_unfold",var.Data()), var, 800, 600);
            TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.3);
            TPad *pad2 = new TPad("pad2", "", 0., 0.3, 1., 1.);
            pad1->SetTopMargin(0.01);
            pad1->SetBottomMargin(0.3);
            pad2->SetBottomMargin(0.01);

            pad2->cd();
            h_data_purity_corrected_1d->Draw("pe1 same");
            h_mc_reco_1d->Draw("pe1 same");
            h_data_fully_corrected_1d->Draw("pe1 same");
            h_mc_true_1d->Draw("pe1 same");
            h_data_refolded_1d->Draw("pe1 same");
            leg->Draw();
            TLatex *test_info_text = new TLatex;
            test_info_text->SetNDC();
            test_info_text->SetTextSize(text_size);
            test_info_text->DrawLatex(0.5, 0.8, "BOTTOMLINE TEST");
            test_info_text->Draw();
            drawHeader();    

            TLine *line = new TLine(x_min, 1., x_max, 1.);
            line->SetLineWidth(2.); 
            line->SetLineStyle(kDashed);
            line->SetLineColor(kGray);

            TLegend *leg_ratio = new TLegend(0.5, 0.3, 0.85, 0.5);
            leg_ratio->SetBorderSize(1);

            TH1D *h_data_mc_reco_ratio = (TH1D *) h_data_purity_corrected_1d->Clone(Form("h_%s_data_mc_reco_ratio", var.Data()));
            h_data_mc_reco_ratio->Divide(h_mc_reco_1d);
            h_data_mc_reco_ratio->SetMarkerStyle(kFullCircle);
            h_data_mc_reco_ratio->SetMarkerColor(kBlack);
            h_data_mc_reco_ratio->SetLineColor(kBlack);
            h_data_mc_reco_ratio->SetMarkerSize(1);
            h_data_mc_reco_ratio->GetYaxis()->SetRangeUser(0.,2.);
            h_data_mc_reco_ratio->GetYaxis()->SetTitle("ratio");
            h_data_mc_reco_ratio->GetXaxis()->SetTitle(xlabel);
            h_data_mc_reco_ratio->GetXaxis()->SetTitleOffset(3.5);
            h_data_mc_reco_ratio->GetYaxis()->SetNdivisions(8);
            leg_ratio->AddEntry(h_data_mc_reco_ratio, "reco data / reco mc", "pe1");

            TH1D *h_data_mc_true_ratio = (TH1D *) h_data_fully_corrected_1d->Clone(Form("h_%s_data_mc_true_ratio", var.Data()));
            h_data_mc_true_ratio->Divide(h_mc_true_1d);
            h_data_mc_true_ratio->SetMarkerStyle(kOpenCross);
            h_data_mc_true_ratio->SetMarkerColor(kBlue);
            h_data_mc_true_ratio->SetLineColor(kBlue);
            h_data_mc_true_ratio->SetMarkerSize(1);
            leg_ratio->AddEntry(h_data_mc_true_ratio, "unfolded data / true mc", "pe1");

            pad1->cd();
            h_data_mc_reco_ratio->Draw("pe1 same");
            h_data_mc_true_ratio->Draw("pe1 same");
            leg_ratio->Draw();
            line->Draw();

            c_unfold->cd();
            pad1->Draw();
            pad2->Draw();
            c_unfold->Draw();
            // // c_unfold->Print("plots_an/"+sample_unfolding+"_"+label_unfolding+"_bottomline_test_"+observable+".png");
        }
    } // JEC src loop
    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (int ivar=0; ivar<6; ivar++) {
        TString var = variations[ivar];
        unfolded_histos[ivar]->SetName(Form("h_%s_data_unfolded",var.Data()));
        unfolded_histos[ivar]->Write();
    }

    fout->Close();
    delete fout;

    // gApplication -> Terminate(0);
}