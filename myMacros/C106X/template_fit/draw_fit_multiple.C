#include <chrono>
#include "../draw_utils.h"

void draw_fit_multiple(TString observable = "rg", TString jer_opt="nom", TString jec_opt="nom")
{
    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;
    // // current date/time based on current system
    // time_t now = time(0);

    // // convert now to string form
    // char* dt = ctime(&now);

    // std::cout << "The local date and time is: " << dt << std::endl;

    Float_t text_size = 18.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString option = "_glued";
    TString sample = "bjet";
    TString label = "aggrTMVA_XXT";
    TString label_in = "data";
    if (sample.Contains("herwig")) label_in += "_herwig_" + label;
    else label_in += "_" + label;
    TString method = "RooFit";
    TString fin_name = "histos/fitted_parameters_" + method + "_" + label_in + "_" + observable + option + suffix + ".root";
    std::cout << "fin = " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TH3D *h_data = (TH3D *) fin->Get(TString("h_data"));
    TH3D *h_sig_training_dijet = (TH3D *) fin->Get("h_sig_training_dijet");
    TH3D *h_bkg_bb_training_dijet = (TH3D *) fin->Get("h_bkg_bb_training_dijet");
    TH3D *h_bkg_rest_training_dijet = (TH3D *) fin->Get("h_bkg_rest_training_dijet");
    TH3D *h_sig_training_bjet = (TH3D *) fin->Get("h_sig_training_bjet");
    TH3D *h_bkg_bb_training_bjet = (TH3D *) fin->Get("h_bkg_bb_training_bjet");
    TH3D *h_bkg_rest_training_bjet = (TH3D *) fin->Get("h_bkg_rest_training_bjet");
    TH3D *h_sig_testing_dijet = (TH3D *) fin->Get("h_sig_testing_dijet");
    TH3D *h_bkg_bb_testing_dijet = (TH3D *) fin->Get("h_bkg_bb_testing_dijet");
    TH3D *h_bkg_rest_testing_dijet = (TH3D *) fin->Get("h_bkg_rest_testing_dijet");
    TH3D *h_sig_testing_bjet = (TH3D *) fin->Get("h_sig_testing_bjet");
    TH3D *h_bkg_bb_testing_bjet = (TH3D *) fin->Get("h_bkg_bb_testing_bjet");
    TH3D *h_bkg_rest_testing_bjet = (TH3D *) fin->Get("h_bkg_rest_testing_bjet");

    // Note: combine after making the projections

    TH2D *h_sig_fraction = (TH2D *) fin->Get("h_sig_fraction");
    TH2D *h_bkg_bb_fraction = (TH2D *) fin->Get("h_bkg_bb_fraction");
    TH2D *h_bkg_rest_fraction = (TH2D *) fin->Get("h_bkg_rest_fraction");

    Int_t nbins_x = h_data->GetNbinsX();
    Int_t nbins_pt = h_data->GetNbinsZ();

    Int_t choose_bin = 2;
    for (Int_t ibin_pt = choose_bin; ibin_pt <= choose_bin; ibin_pt++) {
        TCanvas *c_mb = new TCanvas(Form("c_mb_%d", ibin_pt), "", 1200, 1000);

        TPad *pad11 = new TPad("pad11", "",0.0,0.77,0.33,0.99);
        TPad *pad12 = new TPad("pad12", "",0.33,0.77,0.66,0.99);
        TPad *pad13 = new TPad("pad13", "",0.66,0.77,0.99,0.99);

        TPad *pad21 = new TPad("pad21", "",0.0,0.44,0.33,0.66);
        TPad *pad22 = new TPad("pad22", "",0.33,0.44,0.66,0.66);
        TPad *pad23 = new TPad("pad23", "",0.66,0.44,0.99,0.66);

        TPad *pad31 = new TPad("pad31", "",0.0,0.11,0.33,0.33);
        TPad *pad32 = new TPad("pad32", "",0.33,0.11,0.66,0.33);
        TPad *pad33 = new TPad("pad33", "",0.66,0.11,0.99,0.33);

        TPad *pad111 = new TPad("pad111", "",0.0,0.66,0.33,0.77);
        TPad *pad121 = new TPad("pad121", "",0.33,0.66,0.66,0.77);
        TPad *pad131 = new TPad("pad131", "",0.66,0.66,0.99,0.77);

        TPad *pad211 = new TPad("pad211", "",0.0,0.33,0.33,0.44);
        TPad *pad221 = new TPad("pad221", "",0.33,0.33,0.66,0.44);
        TPad *pad231 = new TPad("pad231", "",0.66,0.33,0.99,0.44);

        TPad *pad311 = new TPad("pad311", "",0.0,0.,0.33,0.11);
        TPad *pad321 = new TPad("pad321", "",0.33,0.,0.66,0.11);
        TPad *pad331 = new TPad("pad331", "",0.66,0.,0.99,0.11);
        

        std::vector<TPad *> dist_pads = {pad11, pad12, pad13,
                                         pad21, pad22, pad23,
                                         pad31, pad32};

        std::vector<TPad *> ratio_pads = {pad111, pad121, pad131,
                                          pad211, pad221, pad231,
                                          pad311, pad321};

        Double_t pt_min = h_sig_training_dijet->GetZaxis()->GetBinLowEdge(ibin_pt);
        Double_t pt_max = h_sig_training_dijet->GetZaxis()->GetBinUpEdge(ibin_pt);

        for (Int_t ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            int ipad = ibin_x - 1;
            
            Double_t x_min = h_data->GetXaxis()->GetBinLowEdge(ibin_x);
            Double_t x_max = h_data->GetXaxis()->GetBinUpEdge(ibin_x);

            // Make projections
            TH1D *h_data_mb = (TH1D *) h_data->ProjectionY(Form("h_data_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_sig_training_dijet_mb = (TH1D *) h_sig_training_dijet->ProjectionY(Form("h_sig_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_dijet_mb = (TH1D *) h_bkg_bb_training_dijet->ProjectionY(Form("h_bkg_bb_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_dijet_mb = (TH1D *) h_bkg_rest_training_dijet->ProjectionY(Form("h_bkg_rest_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_sig_training_bjet_mb = (TH1D *) h_sig_training_bjet->ProjectionY(Form("h_sig_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_bjet_mb = (TH1D *) h_bkg_bb_training_bjet->ProjectionY(Form("h_bkg_bb_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_bjet_mb = (TH1D *) h_bkg_rest_training_bjet->ProjectionY(Form("h_bkg_rest_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_sig_testing_dijet_mb = (TH1D *) h_sig_testing_dijet->ProjectionY(Form("h_sig_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_dijet_mb = (TH1D *) h_bkg_bb_testing_dijet->ProjectionY(Form("h_bkg_bb_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_dijet_mb = (TH1D *) h_bkg_rest_testing_dijet->ProjectionY(Form("h_bkg_rest_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_sig_testing_bjet_mb = (TH1D *) h_sig_testing_bjet->ProjectionY(Form("h_sig_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_bjet_mb = (TH1D *) h_bkg_bb_testing_bjet->ProjectionY(Form("h_bkg_bb_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_bjet_mb = (TH1D *) h_bkg_rest_testing_bjet->ProjectionY(Form("h_bkg_rest_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            // Compile dijet + bjet training and testing - no need to add them all together
            TH1D *h_sig_mb = (TH1D *) h_sig_training_dijet_mb->Clone(Form("h_sig_mb_%d_%d", ibin_pt, ibin_x));
            h_sig_mb->Add(h_sig_testing_dijet_mb);
            h_sig_mb->Add(h_sig_training_bjet_mb);
            h_sig_mb->Add(h_sig_testing_bjet_mb);

            TH1D *h_bkg_bb_mb = (TH1D *) h_bkg_bb_training_dijet_mb->Clone(Form("h_bkg_bb_mb_%d_%d", ibin_pt, ibin_x));
            h_bkg_bb_mb->Add(h_bkg_bb_testing_dijet_mb);
            h_bkg_bb_mb->Add(h_bkg_bb_training_bjet_mb);
            h_bkg_bb_mb->Add(h_bkg_bb_testing_bjet_mb);

            TH1D *h_bkg_rest_mb = (TH1D *) h_bkg_rest_training_dijet_mb->Clone(Form("h_bkg_rest_mb_%d_%d", ibin_pt, ibin_x));
            h_bkg_rest_mb->Add(h_bkg_rest_testing_dijet_mb);
            h_bkg_rest_mb->Add(h_bkg_rest_training_bjet_mb);
            h_bkg_rest_mb->Add(h_bkg_rest_testing_bjet_mb);

            Int_t nbins_mb = h_data_mb->GetNbinsX();
            Double_t ndata = h_data_mb->Integral(1, nbins_mb);
            Double_t sig_fraction = h_sig_fraction->GetBinContent(ibin_x, ibin_pt);
            Double_t bkg_bb_fraction = h_bkg_bb_fraction->GetBinContent(ibin_x, ibin_pt);
            Double_t bkg_rest_fraction = h_bkg_rest_fraction->GetBinContent(ibin_x, ibin_pt);

            THStack *h_stack_mb = new THStack(Form("h_stack_mb_%d_%d", ibin_pt, ibin_x), "");
            h_stack_mb->SetTitle("; m_{B}^{ch}; entries");

            TLegend *leg_mb = new TLegend(0.3, 0.2, 0.7, 0.6);
            leg_mb->SetFillStyle(0);
            leg_mb->SetBorderSize(0);
            leg_mb->SetMargin(0.15);
            // leg_mb->SetHeader(Form("b tagged jets, k_{T} > 1 GeV"));

            TPaveText *rg_range_text = new TPaveText(0.5, 0.7, 1., 0.8, "ndc");
            rg_range_text->SetTextSize(text_size);
            rg_range_text->SetFillStyle(0);
            rg_range_text->SetBorderSize(0);
            // rg_range_text->AddText(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));
            TString xlabel;
            if (observable=="rg") xlabel = "ln(#frac{0.4}{R_{g}})";
            else if (observable=="zg") xlabel = "z_{g}";
            else if (observable=="zpt") xlabel = "z";

            if (observable!="zpt" && ibin_x==1) rg_range_text->AddText("fail SD or k_{T} < 1 GeV");
            else if (observable=="rg" && ibin_x==nbins_x) rg_range_text->AddText(Form("%.2f < %s", x_min, xlabel.Data()));
            else rg_range_text->AddText(Form("%.2f < %s < %.2f", x_min, xlabel.Data(), x_max));

            // Format histos
            // for (auto h : {h_data_mb, h_sig_mb, h_bkg_bb_mb, h_bkg_rest_mb}) {
            //     h->Sumw2();
            // }
            // h_data_mb->Sumw2();
            h_data_mb->SetMarkerStyle(kFullCircle);
            h_data_mb->SetMarkerColor(kBlack);
            h_data_mb->SetMarkerSize(1);
            leg_mb->AddEntry(h_data_mb, "data", "pe1");

            h_sig_mb->Scale(ndata*sig_fraction/h_sig_mb->Integral(1, nbins_mb));
            h_sig_mb->SetFillStyle(1001);
            h_sig_mb->SetFillColor(kRed-7);
            h_sig_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_sig_mb);
            leg_mb->AddEntry(h_sig_mb, "single b jets", "f");

            h_bkg_bb_mb->Scale(ndata*bkg_bb_fraction/h_bkg_bb_mb->Integral(1, nbins_mb));
            h_bkg_bb_mb->SetFillStyle(1001);
            h_bkg_bb_mb->SetFillColor(kBlue-3);
            h_bkg_bb_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_bkg_bb_mb);
            leg_mb->AddEntry(h_bkg_bb_mb, "bb jets", "f");

            h_bkg_rest_mb->Scale(ndata*bkg_rest_fraction/h_bkg_rest_mb->Integral(1, nbins_mb));
            h_bkg_rest_mb->SetFillStyle(1001);
            h_bkg_rest_mb->SetFillColor(kGreen-6);
            h_bkg_rest_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_bkg_rest_mb);
            leg_mb->AddEntry(h_bkg_rest_mb, "gudsc jets", "f");

            // make sure the fractions are correct 
            double int0 = h_sig_mb->Integral(1, nbins_mb);
            double int1 = h_bkg_bb_mb->Integral(1, nbins_mb);
            double int2 = h_bkg_rest_mb->Integral(1, nbins_mb);

            // std::cout << "(" << ibin_pt << ", " << ibin_x << "):" << std::endl;
            // std::cout << "\tsignal fraction = " << int0/(int0+int1+int2) << std::endl;
            // std::cout << "\tbb fraction = " << int1/(int0+int1+int2) << std::endl;
            // std::cout << "\tlight+c fraction = " << int2/(int0+int1+int2) << std::endl;

            dist_pads[ipad]->cd();
            // std::cout << "Top: " << dist_pads[ipad]->GetTopMargin() << std::endl;
            dist_pads[ipad]->SetTopMargin(0.15);
            dist_pads[ipad]->SetBottomMargin(0.);
            dist_pads[ipad]->SetLeftMargin(0.2);
            dist_pads[ipad]->SetRightMargin(0.05);

            h_data_mb->Draw("pe1"); // draw once to have the correct scale
            h_stack_mb->Draw("hist e1 same");
            h_data_mb->Draw("pe1 same");
            // leg_mb->Draw();
            rg_range_text->Draw();

            if (ibin_x==1) {
                TLatex *prelim = new TLatex;
                prelim->SetNDC();
                prelim->SetTextSize(20);
                prelim->DrawLatex(0.185, 0.92, "#bf{CMS} #it{Work in Progress}");
                prelim->Draw();
            }

            if (ibin_x==3) {
                TLatex *lumi = new TLatex;
                lumi->SetNDC();
                lumi->SetTextSize(20);
                lumi->DrawLatex(0.45, 0.92, "#bf{299 fb^{-1} (pp 5.02 TeV)}");
                lumi->Draw();
            }

            // Legend and jet info in empty pad
            TPaveText *info_jets = new TPaveText(0.3, 0.65, 0.7, 0.85);
            info_jets->SetTextSize(text_size);
            info_jets->SetFillStyle(0);
            info_jets->SetBorderSize(0);
            // info_jets->AddText("b tagged jets, k_{T} > 1 GeV");
            info_jets->AddText(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));

            pad33->cd();
            leg_mb->Draw();
            info_jets->Draw();

            // Plot ratio
            TLine *line = new TLine(0., 1., 7., 1.);
            line->SetLineColor(kGray);
            line->SetLineStyle(kDashed);

            TH1D *h_fit_mb = (TH1D *) h_sig_mb->Clone(Form("h_fit_mb_%d_%d", ibin_pt, ibin_x));
            h_fit_mb->Add(h_bkg_bb_mb);
            h_fit_mb->Add(h_bkg_rest_mb);
            h_fit_mb->Sumw2();

            TH1D *h_ratio = (TH1D *) h_data_mb->Clone(Form("h_ratio_%d_%d", ibin_pt, ibin_x));
            h_ratio->Divide(h_fit_mb);

            h_ratio->GetXaxis()->SetTitle("m_{B}^{ch}");
            h_ratio->GetYaxis()->SetTitle("data / fit");

            h_ratio->GetYaxis()->SetNdivisions(6);
            
            // c_mb_ratio->cd(ibin_x);
            ratio_pads[ipad]->cd();
            ratio_pads[ipad]->SetTopMargin(0.);
            ratio_pads[ipad]->SetBottomMargin(0.4);
            ratio_pads[ipad]->SetLeftMargin(0.2);
            ratio_pads[ipad]->SetRightMargin(0.05);
            // std::cout << ratio_pads[ipad]->GetLeftMargin() << std::endl;
            // std::cout << ratio_pads[ipad]->GetRightMargin() << std::endl;
            h_ratio->Draw("pe1");
            line->Draw();

            c_mb->cd();
            dist_pads[ipad]->Draw();
            ratio_pads[ipad]->Draw();
            pad33->Draw();

            h_stack_mb->GetYaxis()->SetTitleOffset(5.);
            h_ratio->GetYaxis()->SetTitleOffset(5.);
            h_ratio->GetXaxis()->SetTitleOffset(5.5);
        }      
        c_mb->Draw();
        c_mb->Print("plots_an/"+sample+"_"+label+"_fits_"+observable+".png");
    }

    // // current date/time based on current system
    // time_t now2 = time(0);

    // // convert now to string form
    // char* dt2 = ctime(&now2);

    // std::cout << "The local date and time is: " << dt2 << std::endl;
}
