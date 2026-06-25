void calc_sf_gen(TString observable="rg")
{
    // Get sf at reco 
    TString fin_sf_name = "histos/aggrTMVA_inclusive_" +observable + "_sfs.root";
    TFile *fin_sf = new TFile(fin_sf_name);
    TH2D *h_sf_reco = (TH2D *) fin_sf->Get("h_eff_sf")->Clone("h_sf_reco");

    // Get response matrix
    TString fin_response_name = "../unfolding/histos/bjet_aggrTMVA_XXT_response_full_jer_nom_jec_nom.root";
    TFile *fin_response = new TFile(fin_response_name);
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin_response->Get("response_"+observable+"pt");
    TMatrixD response_matrix = response->Mresponse();
    TH2D *response_histogram = new TH2D(response_matrix); // 2D 3*8 x 3*8 histogram, normalized per gen bin

    TH2D *h_sf_gen = (TH2D *) h_sf_reco->Clone("h_sf_gen");
    h_sf_gen->Reset();

    int width = h_sf_reco->GetNbinsX();
    int nbins_x = width;
    int nbins_pt = h_sf_reco->GetNbinsY();

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        for (int ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            double new_sf = 0;
            double denom = 0;
            // find the correct row in the response
            int response_row = (ibin_pt-1) * nbins_pt + ibin_x ;
            for (int response_col = 1; response_col <= response_histogram->GetNbinsX(); response_col++) {
                double coeff = response_histogram->GetBinContent(response_col, response_row);
                int sf_row = (response_col - 1) / width + 1;
                int sf_col = (response_col - 1) % width + 1;
                double old_sf = h_sf_reco->GetBinContent(sf_col, sf_row);

                new_sf += coeff * old_sf;
                denom += coeff;
            }
            new_sf /= denom;
            h_sf_gen->SetBinContent(ibin_x, ibin_pt, new_sf);
        }
    }

    // Draw sfs at reco and gen
    TCanvas *c_sfs = new TCanvas("c_sfs", "", 1200, 1000);
    c_sfs->Divide(1,nbins_pt);

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        double pt_min = h_sf_reco->GetYaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_sf_reco->GetYaxis()->GetBinUpEdge(ibin_pt);

        c_sfs->cd(ibin_pt);
        TLegend *leg_sfs = new TLegend(0.2, 0.7, 0.5, 0.9);
        leg_sfs->SetFillStyle(0);
        leg_sfs->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));

        // Make projections
        TH1D *h_sf_reco_1d = (TH1D *) h_sf_reco->ProjectionX(Form("h_sf_reco_1d_%d",ibin_pt), ibin_pt, ibin_pt);
        h_sf_reco_1d->SetLineColor(kBlack);
        h_sf_reco_1d->SetMarkerColor(kBlack);
        h_sf_reco_1d->SetMarkerStyle(kFullCircle);
        leg_sfs->AddEntry(h_sf_reco_1d, "SF at reco", "pe1");

        TH1D *h_sf_gen_1d = (TH1D *) h_sf_gen->ProjectionX(Form("h_sf_gen_1d_%d",ibin_pt), ibin_pt, ibin_pt);
        h_sf_gen_1d->SetLineColor(kRed);
        h_sf_gen_1d->SetMarkerColor(kRed);
        h_sf_gen_1d->SetMarkerStyle(kFullCircle);
        leg_sfs->AddEntry(h_sf_gen_1d, "SF at gen", "pe1");

        for (auto h : {h_sf_reco_1d, h_sf_gen_1d}) {
            h->GetYaxis()->SetTitle("b tag efficiency SF");
            h->GetYaxis()->SetRangeUser(0.8,1.8);
        }

        h_sf_reco_1d->Draw("pe1");
        h_sf_gen_1d->Draw("pe1 same");
        leg_sfs->Draw();
    }

    TString fout_name = "histos/aggrTMVA_inclusive_" + observable + "_sfs_gen.root";
    TFile *fout = new TFile(fout_name, "recreate");

    h_sf_gen->Write();

    fout->Close();
}