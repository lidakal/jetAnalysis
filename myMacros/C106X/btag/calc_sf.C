#include "../draw_utils.h"

void calc_sf(TString observable="rg") 
{
    gStyle->SetPaintTextFormat(".3f"); 
    gStyle->SetErrorX(0.5);
    
    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    TString sample = "dijet";
    TString label = "aggrTMVA_inclusive";

    TString fin_incl_name = "histos/" + observable + "_fit_result_JP_" + label + ".root";
    TFile *fin_incl = new TFile(fin_incl_name);
    TH3D *h_data_incl = (TH3D *) fin_incl->Get("h_data")->Clone("h_data_incl");
    TH3D *h_bbb_incl = (TH3D *) fin_incl->Get("h_bbb")->Clone("h_bbb_incl");
    TH2D *h_bbb_f_incl = (TH2D *) fin_incl->Get("h_bbb_f")->Clone("h_bbb_f_incl");

    TString fin_tag_name = "histos/" + observable + "_fit_result_JP_" + label + "_tagged.root";
    TFile *fin_tag = new TFile(fin_tag_name);
    TH3D *h_data_tag = (TH3D *) fin_tag->Get("h_data")->Clone("h_data_tag");
    TH3D *h_bbb_tag = (TH3D *) fin_tag->Get("h_bbb")->Clone("h_bbb_tag");
    TH2D *h_bbb_f_tag = (TH2D *) fin_tag->Get("h_bbb_f")->Clone("h_bbb_f_tag");
    TH2D *h_bbb_f_mc = (TH2D *) fin_tag->Get("h_bbb_f_mc")->Clone("h_bbb_f_mc");

    // Make 2D projections of data / MC -> easier for error propagation
    for (auto h : {
                   h_data_tag, h_bbb_tag,
                   h_data_incl, 
                   h_bbb_incl
                   }) {
        h->GetYaxis()->SetRange(1, h->GetNbinsY()); // choose whether to include JP-untagged
    }
    TH2D *h_data_incl_2d = (TH2D *) h_data_incl->Project3D("zx");
    TH2D *h_data_tag_2d = (TH2D *) h_data_tag->Project3D("zx");
    TH2D *h_bbb_incl_2d = (TH2D *) h_bbb_incl->Project3D("zx");
    TH2D *h_bbb_tag_2d = (TH2D *) h_bbb_tag->Project3D("zx");
    
    // Calculate Cb
    // TH3D *h_bbb_incl_clone = (TH3D*) h_bbb_incl->Clone("h_bbb_incl_clone");
    // h_bbb_incl_clone->GetYaxis()->SetRange(1, h_bbb_incl_clone->GetNbinsY()); // include JP-untagged
    // // std::cout << h_bbb_incl->Integral() << ", " << h_bbb_incl_clone->Integral() << std::endl;
    // TH2D *h_cb_denom = (TH2D *) h_bbb_incl_clone->Project3D("zx");

    // TH2D *h_cb = (TH2D *) h_bbb_incl_2d->Clone("h_cb");
    // h_cb->Divide(h_cb, h_cb_denom, 1., 1., "b");

    h_bbb_f_incl->GetXaxis()->SetTitle(xlabel);
    h_bbb_f_incl->GetYaxis()->SetTitle("p_{T}^{jet}");
    h_bbb_f_incl->SetMarkerSize(800);

    std::cout << "before b eff" << std::endl;
    // Make new histograms to store b eff
    TH2D *h_bbb_eff = (TH2D *) h_bbb_f_incl->Clone("h_bbb_eff");
    h_bbb_eff->Reset();
    h_bbb_eff->SetTitle("b eff in data");
    // h_bbb_eff->Multiply(h_bbb_f_mc, h_data_tag_2d);
    h_bbb_eff->Multiply(h_bbb_f_tag, h_data_tag_2d);
    // h_bbb_eff->Multiply(h_cb);
    h_bbb_eff->Divide(h_bbb_f_incl);
    h_bbb_eff->Divide(h_data_incl_2d);

    std::cout << "after b eff" << std::endl;

    TH2D *h_bbb_eff_mc = (TH2D *) h_bbb_tag_2d->Clone("h_bbb_eff_mc");
    h_bbb_eff_mc->SetTitle("b eff in mc");
    h_bbb_eff_mc->Divide(h_bbb_tag_2d, h_bbb_incl_2d, 1., 1., "b");
    std::cout << "after b eff mc" << std::endl;

    // TH2D *h_tag_ratio = (TH2D *) h_bbb_f_incl->Clone("h_tag_ratio");
    // h_tag_ratio->Reset();
    // h_tag_ratio->SetTitle("ntag/nincl");

    TH2D *h_eff_sf = (TH2D *) h_bbb_f_incl->Clone("h_eff_sf");
    h_eff_sf->Reset();
    h_eff_sf->SetTitle("beff data/beff mc");
    h_eff_sf->Divide(h_bbb_eff, h_bbb_eff_mc);


    // Loop over jet pt+observable bins
    int nbins_x = h_data_incl->GetNbinsX();
    int nbins_jp = h_data_incl->GetNbinsY();
    int nbins_pt = h_data_incl->GetNbinsZ();

    TCanvas *c_sf_per_pt = new TCanvas("c_sf_per_pt", "", 800, 600);
    TLegend *leg_sf_per_pt = new TLegend(0.15, 0.7, 0.5, 0.9);
    leg_sf_per_pt->SetFillStyle(0.);

    // relative uncertainties 
    std::vector<double> unc;
    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        double pt_min = h_data_incl->GetZaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_data_incl->GetZaxis()->GetBinUpEdge(ibin_pt);
        for (int ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            // if (ibin_x!=2) continue;
            TH1D *h_data_incl_1d = (TH1D *) h_data_incl->ProjectionY(Form("h_data_incl_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_data_tag_1d = (TH1D *) h_data_tag->ProjectionY(Form("h_data_tag_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            TH1D *h_bbb_incl_1d = (TH1D *) h_bbb_incl->ProjectionY(Form("h_bbb_incl_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bbb_tag_1d = (TH1D *) h_bbb_tag->ProjectionY(Form("h_bbb_tag_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            double nincl = h_data_incl_1d->Integral(1, nbins_jp);
            double ntag = h_data_tag_1d->Integral(1, nbins_jp);
            double bbb_f_incl = h_bbb_f_incl->GetBinContent(ibin_x, ibin_pt);
            // double bbb_f_tag = 1.;
            // double bbb_f_tag = h_bbb_f_mc->GetBinContent(ibin_x, ibin_pt);
            double bbb_f_tag = h_bbb_f_tag->GetBinContent(ibin_x, ibin_pt);

            double beff = (bbb_f_tag * ntag) / (bbb_f_incl * nincl);

            // std::cout << "\nibinpt=" << ibin_pt << ", ibinx=" << ibin_x << std::endl;

            // std::cout << "nincl=" << nincl << std::endl;
            // std::cout << "bbb_f_incl=" << bbb_f_incl << std::endl;
            // std::cout << "ntag=" << ntag << std::endl;
            // std::cout << "beff=" << beff << std::endl;

            double nincl_mc = h_bbb_incl_1d->Integral(1, nbins_jp);
            double ntag_mc = h_bbb_tag_1d->Integral(1, nbins_jp);
            double beff_mc = ntag_mc / nincl_mc;

            // std::cout << "beff_mc=" << beff_mc << std::endl;

            double sf = beff / beff_mc;

            // std::cout << "sf=" << sf << std::endl;

            // std::cout << "Nincl relative error = " << h_data_incl_2d->GetBinError(ibin_x, ibin_pt)/h_data_incl_2d->GetBinContent(ibin_x, ibin_pt) << std::endl;
            // std::cout << "Ntag relative error = " << h_data_tag_2d->GetBinError(ibin_x, ibin_pt)/h_data_tag_2d->GetBinContent(ibin_x, ibin_pt) << std::endl;
            // std::cout << "fbincl relative error = " << h_bbb_f_incl->GetBinError(ibin_x, ibin_pt)/h_bbb_f_incl->GetBinContent(ibin_x, ibin_pt) << std::endl;
            // std::cout << "fbtag relative error = " << h_bbb_f_mc->GetBinError(ibin_x, ibin_pt)/h_bbb_f_mc->GetBinContent(ibin_x, ibin_pt) << std::endl;
            // std::cout << "sf relative error = " << h_eff_sf->GetBinError(ibin_x, ibin_pt)/h_eff_sf->GetBinContent(ibin_x, ibin_pt) << std::endl;

            // h_bbb_eff->SetBinContent(ibin_x, ibin_pt, beff);
            // h_tag_ratio->SetBinContent(ibin_x, ibin_pt, ntag/nincl);
            // h_bbb_eff_mc->SetBinContent(ibin_x, ibin_pt, beff_mc);
            // h_eff_sf->SetBinContent(ibin_x, ibin_pt, sf);

            // h_eff_sf->SetBinError(ibin_x, ibin_pt, 1e-6);
            // if (ibin_x<nbins_x)
                unc.push_back(h_eff_sf->GetBinError(ibin_x, ibin_pt)/h_eff_sf->GetBinContent(ibin_x, ibin_pt));
        }

        c_sf_per_pt->cd();
        TH1D *h_eff_sf_1d = (TH1D *) h_eff_sf->ProjectionX(Form("h_eff_sf_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_eff_sf_1d->SetMarkerColor(ibin_pt);
        h_eff_sf_1d->SetLineColor(ibin_pt);
        h_eff_sf_1d->SetLineWidth(1);
        h_eff_sf_1d->SetMinimum(0.9);
        h_eff_sf_1d->SetMaximum(2.);
        h_eff_sf_1d->GetYaxis()->SetTitle("SF = #epsilon^{data} / #epsilon^{MC}");
        leg_sf_per_pt->AddEntry(h_eff_sf_1d, Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max), "pe1");
        h_eff_sf_1d->Draw("pe1 same");
    }

    c_sf_per_pt->cd();
    drawHeader();
    leg_sf_per_pt->Draw();

    c_sf_per_pt->Draw();
    c_sf_per_pt->Print("plots_an/"+sample+"_"+label+"_sfs_"+observable+".png");

    double x1 = 2.1;
    double x2 = 2.5;
    double y1 = 0.95;
    double y2 = 1.78;
    TPaveText *overflow = new TPaveText(x1, y1, x2, y2);
    overflow->SetBorderSize(1);
    overflow->SetFillStyle(3644);
    auto overflow_txt = overflow->AddText("overflow");
    overflow_txt->SetTextAngle(90);
    overflow_txt->SetTextSize(20);
    // if (observable=="rg") overflow->Draw();

    // TCanvas *c_bbb_f = new TCanvas("c_bbb_f", "b+bb fraction inclusive", 800, 600);
    // h_bbb_f_incl->Draw("colz text");

    // TCanvas *c_tag_ratio = new TCanvas("c_tag_ratio", "ntag/nincl", 800, 600);
    // h_tag_ratio->Draw("colz text");

    TCanvas *c_bbb_eff = new TCanvas("c_bbb_eff", "b+bb efficiency data", 800, 600);
    h_bbb_eff->Draw("colz text");

    TCanvas *c_bbb_eff_mc = new TCanvas("c_bbb_eff_mc", "b+bb efficiency mc", 800, 600);
    h_bbb_eff_mc->Draw("colz text");

    TCanvas *c_eff_sf = new TCanvas("c_eff_sf", "sf (eff data/eff mc)", 800, 600);
    h_eff_sf->Draw("colz texte");
    // TH1D *h_eff_sf_pt = (TH1D *) h_eff_sf->ProjectionY();
    // h_eff_sf_pt->GetYaxis()->SetTitle("SF");
    // h_eff_sf_pt->Draw("pe1");

    // TCanvas *c_cb = new TCanvas("c_cb", "Cb (from mc)", 800, 600);
    // h_cb->SetMarkerSize(800);
    // h_cb->Draw("colz text");

    std::cout << *max_element(unc.begin(), unc.end()) << std::endl;

    for (int i=0; i<nbins_pt; i++) {
        for (int j=0; j<nbins_x; j++) {
            std::cout << "(" << i << ", " << j << ") u = " << unc[nbins_x*i+j] << std::endl;
        }
    }

    TString fout_name = "histos/" + label + "_" + observable + "_sfs.root";
    TFile *fout = new TFile(fout_name, "recreate");

    h_eff_sf->Write();

    fout->Close();

}