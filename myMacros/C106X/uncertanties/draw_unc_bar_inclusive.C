void draw_unc_bar_inclusive(TString observable="rg")
{
    gStyle->SetErrorX(0.5);

    TFile *fin_jer = new TFile("histos/"+observable+"_jer_unc_inclusive.root");
    TFile *fin_jec = new TFile("histos/"+observable+"_jec_unc_inclusive.root");
    TFile *fin_trk_inef = new TFile("histos/"+observable+"_trk_inef_inclusive.root");
    TFile *fin_model_unc = new TFile("histos/"+observable+"_model_unc_inclusive.root");
    TFile *fin_stat_unc = new TFile("histos/"+observable+"_stat_unc_inclusive.root");

    int nbins_pt = 3;
    

    TCanvas *c_unc = new TCanvas("c_unc", "", 1600, 600);
    TPad *pad11 = new TPad("pad11", "", 0., 0., 0.33, 1.);
    TPad *pad12 = new TPad("pad12", "", 0.33, 0., 0.66, 1.);
    TPad *pad13 = new TPad("pad13", "", 0.66, 0., 0.99, 1.);
    std::vector<TPad *> pads = {pad11, pad12, pad13};

    for (int ibin_pt=1; ibin_pt<=nbins_pt; ibin_pt++) {
        int ipad = ibin_pt - 1;

        TLegend *leg = new TLegend(0.2, 0.6, 0.6, 0.9);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetMargin(0.15);
        leg->SetHeader(Form("jet pt bin %d", ibin_pt));

        TH1D *h_stat_unc_rel = (TH1D *) fin_stat_unc->Get(Form("h_stat_unc_rel_%d", ibin_pt))->Clone(Form("h_stat_unc_rel_%d", ibin_pt));
        // for (int ibin_x=1; ibin_x<=h_stat_unc_rel->GetNbinsX(); ibin_x++) {
        //     double err = h_stat_unc_rel->GetBinContent(ibin_x);
        //     h_stat_unc_rel->SetBinContent(ibin_x, 0.);
        //     h_stat_unc_rel->SetBinError(ibin_x, err);
        // }
        h_stat_unc_rel->SetLineColor(kBlack);
        h_stat_unc_rel->SetMarkerStyle(1);
        h_stat_unc_rel->SetLineWidth(2);
        h_stat_unc_rel->GetYaxis()->SetTitle("relative uncertainty");
        h_stat_unc_rel->GetYaxis()->SetTitleOffset(2.5);
        leg->AddEntry(h_stat_unc_rel, "statistical (sym)", "l");

        TH1D *h_stat_unc_rel_down = (TH1D *) h_stat_unc_rel->Clone(Form("h_stat_unc_rel_down_%d", ibin_pt));
        h_stat_unc_rel_down->Scale(-1.);

        TH1D *h_jer_up_rel = (TH1D *) fin_jer->Get(Form("h_unc_up_rel_%d", ibin_pt))->Clone(Form("h_jer_up_rel_%d", ibin_pt));
        h_jer_up_rel->SetLineColor(kRed);
        h_jer_up_rel->SetLineWidth(2);
        h_jer_up_rel->GetYaxis()->SetTitle("relative uncertainty");
        h_jer_up_rel->GetYaxis()->SetTitleOffset(2.5);
        leg->AddEntry(h_jer_up_rel, "JER up", "l");
        
        TH1D *h_jer_down_rel = (TH1D *) fin_jer->Get(Form("h_unc_down_rel_%d", ibin_pt))->Clone(Form("h_jer_down_rel_%d", ibin_pt));
        h_jer_down_rel->SetLineColor(kRed+2);
        h_jer_down_rel->SetLineWidth(2);
        h_jer_down_rel->GetYaxis()->SetTitle("relative uncertainty");
        h_jer_down_rel->GetYaxis()->SetTitleOffset(2.5);
        leg->AddEntry(h_jer_down_rel, "JER down", "l");

        TH1D *h_jec_up_rel = (TH1D *) fin_jec->Get(Form("h_unc_up_rel_%d", ibin_pt))->Clone(Form("h_jec_up_rel_%d", ibin_pt));
        h_jec_up_rel->SetLineColor(kBlue);
        h_jec_up_rel->SetLineWidth(2);
        h_jec_up_rel->GetYaxis()->SetTitle("relative uncertainty");
        h_jec_up_rel->GetYaxis()->SetTitleOffset(2.5);
        leg->AddEntry(h_jec_up_rel, "JEC up", "l");

        TH1D *h_jec_down_rel = (TH1D *) fin_jec->Get(Form("h_unc_down_rel_%d", ibin_pt))->Clone(Form("h_jec_down_rel_%d", ibin_pt));
        h_jec_down_rel->SetLineColor(kBlue+2);
        h_jec_down_rel->SetLineWidth(2);
        h_jec_down_rel->GetYaxis()->SetTitle("relative uncertainty");
        h_jec_down_rel->GetYaxis()->SetTitleOffset(2.5);
        leg->AddEntry(h_jec_down_rel, "JEC down", "l");

        TH1D *h_trk_inef_rel = (TH1D *) fin_trk_inef->Get(Form("h_inef_unc_rel_%d", ibin_pt))->Clone(Form("h_trk_inef_rel_%d", ibin_pt));
        h_trk_inef_rel->SetLineColor(kMagenta);
        h_trk_inef_rel->SetLineWidth(2);
        h_trk_inef_rel->GetYaxis()->SetTitle("relative uncertainty");
        h_trk_inef_rel->GetYaxis()->SetTitleOffset(2.5);
        leg->AddEntry(h_trk_inef_rel, "3% trk inef", "l");

        TH1D *h_model_unc_rel = (TH1D *) fin_model_unc->Get(Form("h_model_unc_rel_%d", ibin_pt))->Clone(Form("h_model_unc_rel_%d", ibin_pt));
        h_model_unc_rel->SetLineColor(kYellow);
        h_model_unc_rel->SetLineWidth(2);
        h_model_unc_rel->GetYaxis()->SetTitle("relative uncertainty");
        h_model_unc_rel->GetYaxis()->SetTitleOffset(2.5);
        leg->AddEntry(h_model_unc_rel, "modelling", "l");

        std::vector<TH1D *> histos = {h_jer_up_rel, h_jer_down_rel, h_jec_up_rel, h_jec_down_rel, h_trk_inef_rel, h_model_unc_rel};
        double ymax=-100;
        double ymin=100;

        for (auto h : histos) {
            double maxh=h->GetBinContent(h->GetMaximumBin());
            double minh=h->GetBinContent(h->GetMinimumBin());

            if (maxh > ymax) ymax = maxh;
            if (minh < ymin) ymin = minh;
        }

        if (ymin<0) ymin*=1.1;
        else ymin*=0.9;

        if (ymax<0) ymax*=0.9;
        else ymax*=1.1;

        pads[ipad]->cd();
        pads[ipad]->SetRightMargin(0.05);

        int nhists = histos.size()+1;
        double bwidth = 1./(nhists+1.);
        double boffset = (1-(nhists*bwidth)) / (nhists+1);
        
        for (auto h : {h_stat_unc_rel, h_stat_unc_rel_down}) {
            h->GetYaxis()->SetRangeUser(ymin, ymax);
            h->SetFillColor(h->GetLineColor());
            h->SetFillStyle(1001);
            h->SetBarWidth(bwidth);
            h->SetBarOffset(boffset);
            h->Draw("hist b same");
        }

        for (int i=0; i<histos.size(); i++) {
            TH1D *h = histos[i];
            h->GetYaxis()->SetRangeUser(ymin, ymax);
            h->SetFillColor(h->GetLineColor());
            h->SetFillStyle(1001);
            h->SetBarWidth(bwidth);
            h->SetBarOffset(((i+2)*boffset)+((i+1)*bwidth));
            h->Draw("hist b same");
        }
        // h_stat_unc_rel->Draw("pe2 same");
        leg->Draw();

        TLine *line = new TLine(h_jer_up_rel->GetXaxis()->GetBinLowEdge(1), 0, h_jer_up_rel->GetXaxis()->GetBinUpEdge(h_jer_up_rel->GetNbinsX()), 0);
        line->SetLineStyle(kDashed);
        line->Draw();

        for (int ibin_x=1; ibin_x<h_jer_up_rel->GetNbinsX(); ibin_x++) {
            double x = h_jer_up_rel->GetXaxis()->GetBinUpEdge(ibin_x);
            TLine *line_x = new TLine(x, ymin, x, ymax);
            line_x->SetLineColor(kGray);
            line_x->SetLineStyle(kDashed);
            line_x->Draw();
        }

        double x1 = h_jer_up_rel->GetXaxis()->GetBinLowEdge(h_jer_up_rel->GetNbinsX());
        double x2 = h_jer_up_rel->GetXaxis()->GetBinUpEdge(h_jer_up_rel->GetNbinsX());
        double y1 = ymin;
        double y2 = ymax;
        TPaveText *overflow = new TPaveText(x1, y1, x2, y2);
        overflow->SetBorderSize(1);
        overflow->SetFillStyle(3644);
        auto overflow_txt = overflow->AddText("overflow");
        overflow_txt->SetTextAngle(90);
        overflow_txt->SetTextSize(20);
        if (observable=="rg") overflow->Draw();

        c_unc->cd();
        pads[ipad]->Draw();
    }
}