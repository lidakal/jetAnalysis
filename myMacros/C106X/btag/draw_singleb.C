void draw_singleb(TString observable="rg")
{
    TString xlabel;
    if (observable=="rg") xlabel = "ln(#frac{0.4}{R_{g}})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";
    TString ylabel = "1/N dN/d" + xlabel;

    TFile *fin = new TFile("../unfolding/histos/aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom.root");
    TH2D *h_data_singleb = (TH2D *) fin->Get("h_data_singleb");
    TH2D *h_mc_singleb = (TH2D *) fin->Get("h_mc_singleb");

    int ibin_pt = 2;

    TH1D *h_data_singleb_1d = (TH1D *) h_data_singleb->ProjectionX("h_data_singleb_1d", ibin_pt, ibin_pt);
    h_data_singleb_1d->SetMarkerColor(kBlack);
    h_data_singleb_1d->SetLineColor(kBlack);
    h_data_singleb_1d->SetMarkerStyle(kFullCircle);
    h_data_singleb_1d->SetTitle("data single b reco");
    h_data_singleb_1d->Scale(1/h_data_singleb_1d->Integral(), "width");
    h_data_singleb_1d->GetXaxis()->SetTitle(xlabel);
    h_data_singleb_1d->GetYaxis()->SetTitle(ylabel);

    TH1D *h_mc_singleb_1d = (TH1D *) h_mc_singleb->ProjectionX("h_mc_singleb_1d", ibin_pt, ibin_pt);
    h_mc_singleb_1d->SetMarkerColor(kRed);
    h_mc_singleb_1d->SetLineColor(kRed);
    h_mc_singleb_1d->SetMarkerStyle(kFullCircle);
    h_mc_singleb_1d->SetTitle("mc single b reco");
    h_mc_singleb_1d->Scale(1/h_mc_singleb_1d->Integral(), "width");
    h_data_singleb_1d->GetXaxis()->SetTitle(xlabel);
    h_data_singleb_1d->GetYaxis()->SetTitle(ylabel);

    TCanvas *c_singleb = new TCanvas("c_singleb", "", 800, 600);
    h_data_singleb_1d->Draw();
    h_mc_singleb_1d->Draw("same");
    c_singleb->BuildLegend();


}