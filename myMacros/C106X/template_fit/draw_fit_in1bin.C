#include "../draw_utils.h"
#include "../cms_palette.h"

void draw_fit_in1bin(TString observable = "rg", TString jer_opt="nom", TString jec_opt="nom")
{
    gStyle->SetCanvasPreferGL(kTRUE);
    // gStyle->SetErrorX(0.5);

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;

    Float_t text_size = 32.;
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

    // Make projections 

    Int_t nbins_x = h_data->GetNbinsX();
    Int_t nbins_pt = h_data->GetNbinsZ();

    Int_t ibin_pt = 2;
    Int_t ibin_x = 2;
    if (observable=="zpt") ibin_x = nbins_x;

    Double_t pt_min = h_sig_training_dijet->GetZaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_sig_training_dijet->GetZaxis()->GetBinUpEdge(ibin_pt);

    Double_t x_min = h_data->GetXaxis()->GetBinLowEdge(ibin_x);
    Double_t x_max = h_data->GetXaxis()->GetBinUpEdge(ibin_x);

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

    TString range_text;
    TString xlabel;
    if (observable=="rg") {
        xlabel = "R_{g}";
        x_min = 0.4*std::exp(-x_min);
        x_max = 0.4*std::exp(-x_max);
        range_text = Form("%.3f_{} < %s < %.3f", x_max, xlabel.Data(), x_min);
    } else if (observable=="zg") {
        xlabel = "z_{g}";
        range_text = Form("%.1f_{} < %s < %.1f", x_min, xlabel.Data(), x_max);
    } else if (observable=="zpt") {
        xlabel = "z_{b,ch}";
        range_text = Form("%.1f_{} < %s < %.1f", x_min, xlabel.Data(), x_max);
    }

    // if (observable!="zpt" && ibin_x==1) range_text = "fail SD or k_{T} < 1 GeV";
    // else if (observable=="rg" && ibin_x==nbins_x) range_text = Form("%.2f < %s", x_min, xlabel.Data());
    // else range_text = Form("%.2f < %s < %.2f", x_min, xlabel.Data(), x_max);

    // Format histos
    // for (auto h : {h_data_mb, h_sig_mb, h_bkg_bb_mb, h_bkg_rest_mb}) {
    //     h->Sumw2();
    // }
    // h_data_mb->Sumw2();
    h_data_mb->SetMarkerStyle(kFullCircle);
    h_data_mb->SetMarkerColor(kBlack);
    h_data_mb->SetMarkerSize(1);
    double labelOffset = h_data_mb->GetXaxis()->GetLabelOffset();
    h_data_mb->GetXaxis()->SetLabelOffset(10.);
    h_data_mb->GetYaxis()->SetTitleOffset(1.7);
    h_data_mb->GetYaxis()->SetTitle("Entries / 1000");
    h_data_mb->Scale(1/1000.);

    h_sig_mb->Scale(ndata*sig_fraction/h_sig_mb->Integral(1, nbins_mb));
    h_sig_mb->SetFillStyle(1001);
    h_sig_mb->SetFillColorAlpha(cmsBlue, 0.5);
    h_sig_mb->SetLineColorAlpha(cmsBlue, 1.);
    h_sig_mb->SetMarkerStyle(1);
    h_sig_mb->Scale(1/1000.);

    h_bkg_bb_mb->Scale(ndata*bkg_bb_fraction/h_bkg_bb_mb->Integral(1, nbins_mb));
    h_bkg_bb_mb->SetFillStyle(1001);
    h_bkg_bb_mb->SetFillColorAlpha(cmsViolet, 0.5);
    h_bkg_bb_mb->SetLineColorAlpha(cmsViolet, 1.);
    h_bkg_bb_mb->SetMarkerStyle(1);
    h_bkg_bb_mb->Scale(1/1000.);

    h_bkg_rest_mb->Scale(ndata*bkg_rest_fraction/h_bkg_rest_mb->Integral(1, nbins_mb));
    h_bkg_rest_mb->SetFillStyle(1001);
    h_bkg_rest_mb->SetFillColorAlpha(cmsOrange, 0.5);
    h_bkg_rest_mb->SetLineColorAlpha(cmsOrange, 1.);
    h_bkg_rest_mb->SetMarkerStyle(1);
    h_bkg_rest_mb->Scale(1/1000.);

    TH1D *h_fit_mb = (TH1D *) h_sig_mb->Clone(Form("h_fit_mb_%d_%d", ibin_pt, ibin_x));
    h_fit_mb->Add(h_bkg_bb_mb);
    h_fit_mb->Add(h_bkg_rest_mb);

    Double_t xs[50];
    Double_t ys[50];
    Double_t dxs[50];
    Double_t dys[50];
    for (int ibin_x=1; ibin_x<=h_fit_mb->GetNbinsX(); ibin_x++) {
        int index = ibin_x - 1;

        double x = h_fit_mb->GetXaxis()->GetBinCenter(ibin_x);
        double dx = h_fit_mb->GetXaxis()->GetBinUpEdge(ibin_x) - x;

        double y = h_fit_mb->GetBinContent(ibin_x);
        double dy = h_fit_mb->GetBinError(ibin_x);

        xs[index] = x;
        ys[index] = y;
        dxs[index] = dx;
        dys[index] = dy;
    }

    TGraphErrors *gr_err = new TGraphErrors(h_fit_mb->GetNbinsX(), xs, ys, dxs, dys);
    gr_err->SetFillStyle(3004);
    gr_err->SetFillColorAlpha(kBlack, 0.8);
    gr_err->SetLineWidth(5);

    THStack *h_stack_mb = new THStack(Form("h_stack_mb_%d_%d", ibin_pt, ibin_x), "");
    // h_stack_mb->SetTitle("; m_{b,ch}; entries");
    h_stack_mb->Add(h_bkg_rest_mb);
    h_stack_mb->Add(h_sig_mb);
    h_stack_mb->Add(h_bkg_bb_mb);

    TLegend *leg_mb = new TLegend(0.65, 0.37, 0.95, 0.62);
    if (observable=="zpt") leg_mb = new TLegend(0.67, 0.38, 0.97, 0.63);
    leg_mb->SetFillStyle(0);
    leg_mb->SetBorderSize(0);
    leg_mb->SetMargin(0.2);
    leg_mb->SetTextSize(text_size);
    leg_mb->SetEntrySeparation(0.05);
    // leg_mb->SetHeader(Form("b tagged jets, k_{T} > 1 GeV"));

    leg_mb->AddEntry(h_data_mb, "Data", "pe1");
    leg_mb->AddEntry(h_sig_mb, "Single b", "f");
    leg_mb->AddEntry(h_bkg_bb_mb, "Double b", "f");
    leg_mb->AddEntry(h_bkg_rest_mb, "Light+c", "f");

    TH1D *h_ratio = (TH1D *) h_data_mb->Clone(Form("h_ratio_%d_%d", ibin_pt, ibin_x));
    h_ratio->Divide(h_fit_mb);
    h_ratio->GetYaxis()->SetNdivisions(8);
    h_ratio->GetYaxis()->CenterTitle(1);
    // h_ratio->GetXaxis()->SetTickLength(0.09);

    //universal size values
	double canvasW = 650;
    double canvasH = 900;
	double marginL = 80;
	double marginR = 10;
	double marginB = 70;
	double marginT = 40;
	double marginM = 3;
	double sizeTick = 12;
	
    TCanvas *c_mb = new TCanvas("c_mb","",canvasW,canvasH);

    TPad *pad1 = new TPad("pad1","",0.,0.27,1.,1.);
    double pad1W = pad1->GetWw()*pad1->GetAbsWNDC();
	double pad1H = pad1->GetWh()*pad1->GetAbsHNDC();
    pad1->SetMargin(marginL/pad1W,marginR/pad1W,marginM/pad1H,marginT/pad1H);
    pad1->SetTicks(1,1);
    double tickScaleXT = (pad1W - marginL - marginR)/pad1W*pad1H;
	double tickScaleYT = (pad1H - marginM - marginT)/pad1H*pad1W;
    
    TPad *pad2 = new TPad("pad2","",0.,0.,1.,0.27);
    double pad2W = pad2->GetWw()*pad2->GetAbsWNDC();
	double pad2H = pad2->GetWh()*pad2->GetAbsHNDC();
	pad2->SetMargin(marginL/pad2W,marginR/pad2W,marginB/pad2H,marginM/pad2H);
	pad2->SetTicks(1,1);
    double tickScaleXB = (pad2W - marginL - marginR)/pad2W*pad2H;
	double tickScaleYB = (pad2H - marginB - marginM)/pad2H*pad2W;

    h_data_mb->GetXaxis()->SetTickLength(sizeTick/tickScaleXT);
	h_data_mb->GetYaxis()->SetTickLength(sizeTick/tickScaleYT);

    double ymax = std::max({h_data_mb->GetMaximum(), h_fit_mb->GetMaximum()});
    // ymax = ymax * 1.25;
    ymax = ymax * 1.6;
    ymax = 7.;
    h_data_mb->GetYaxis()->SetRangeUser(0.0001,ymax);

    pad1->cd();
    h_data_mb->Draw("pe1"); // draw once to have the correct scale
    h_stack_mb->Draw("hist same");
    h_data_mb->Draw("pe1 same");
    gr_err->Draw("e2 same");
    drawHeader();
    leg_mb->Draw();
    pad1->RedrawAxis();

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    jet_info->SetTextAlign(11);
    jet_info->DrawLatex(0.17, 0.87, "anti_{}-_{}k_{T} R = 0.4");
    jet_info->DrawLatex(0.17, 0.8, "b-tagged jets");
    jet_info->DrawLatex(0.17, 0.73, "100 <^{}_{} p_{T,reco}^{jet} < 120 GeV");
    jet_info->DrawLatex(0.17, 0.66, "|#eta^{jet}| < 2");

    TLatex *jet_info2 = new TLatex;
    jet_info2->SetNDC();
    jet_info2->SetTextSize(text_size);
    jet_info2->SetTextAlign(31);
    jet_info2->DrawLatex(0.95, 0.87, range_text);
    if (observable!="zpt") jet_info2->DrawLatex(0.95, 0.8, "k_{T} > 1 GeV");

    // Plot ratio
    pad2->cd();
    TLine *line = new TLine(0., 1., 7., 1.);
    line->SetLineColor(kGray);
    line->SetLineStyle(kDashed);

    h_ratio->GetXaxis()->SetLabelOffset(labelOffset);
    h_ratio->GetXaxis()->SetTitle("m_{b,ch} (GeV)");
    h_ratio->GetYaxis()->SetTitle("Data / fit");
    h_ratio->GetXaxis()->SetTitleOffset(3.5);
    h_ratio->GetYaxis()->SetTitleOffset(h_data_mb->GetYaxis()->GetTitleOffset());
    h_ratio->GetYaxis()->SetNdivisions(505);
    h_ratio->GetYaxis()->SetRangeUser(0.65,1.35);

    h_ratio->GetXaxis()->SetTickLength(sizeTick/tickScaleXB);
	h_ratio->GetYaxis()->SetTickLength(sizeTick/tickScaleYB);

    h_ratio->Draw("pe1");
    line->Draw();
    h_ratio->Draw("pe1 same");
    pad2->RedrawAxis();

    c_mb->cd();
    pad1->Draw();
    pad2->Draw();

    c_mb->Print("plots_an/template_fit_"+observable+"_in1bin.pdf");
    // c_mb->Print("plots_an/template_fit_"+observable+"_in1bin.png");
}