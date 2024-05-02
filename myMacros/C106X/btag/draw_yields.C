void draw_yields(TString observable="rg")
{
    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    Float_t text_size = 26.;
    gStyle->SetPalette(57);
    // gStyle->SetPaintTextFormat("2.1e");
    TString label = "aggrTMVA_inclusive";
    
    // Load data histogram
    TFile *fin_data = new TFile("histos/data_" + label + "_histograms_for_fit_JP.root");
    TH3D *h_data = (TH3D *) fin_data->Get("h_data_"+observable)->Clone("h_data");
    TH3D *h_data_tagged = (TH3D *) fin_data->Get("h_data_"+observable+"_tagged")->Clone("h_data_tagged");

    // Load dijet histograms
    TFile *fin_dijet = new TFile("histos/dijet_" + label + "_histograms_for_fit_JP.root");
    TH3D *h_dijet_b = (TH3D *) fin_dijet->Get("h_b_"+observable)->Clone("h_dijet_b");
    TH3D *h_dijet_b_tagged = (TH3D *) fin_dijet->Get("h_b_"+observable+"_tagged")->Clone("h_dijet_b_tagged");
    TH3D *h_dijet_bb = (TH3D *) fin_dijet->Get("h_bb_"+observable)->Clone("h_dijet_bb");
    TH3D *h_dijet_bb_tagged = (TH3D *) fin_dijet->Get("h_bb_"+observable+"_tagged")->Clone("h_dijet_bb_tagged");
    TH3D *h_dijet_c = (TH3D *) fin_dijet->Get("h_c_"+observable)->Clone("h_dijet_c");
    TH3D *h_dijet_c_tagged = (TH3D *) fin_dijet->Get("h_c_"+observable+"_tagged")->Clone("h_dijet_c_tagged");
    TH3D *h_dijet_l = (TH3D *) fin_dijet->Get("h_l_"+observable)->Clone("h_dijet_l");
    TH3D *h_dijet_l_tagged = (TH3D *) fin_dijet->Get("h_l_"+observable+"_tagged")->Clone("h_dijet_l_tagged");

    // Load bjet histograms
    TFile *fin_bjet = new TFile("histos/bjet_" + label + "_histograms_for_fit_JP.root");
    TH3D *h_bjet_b = (TH3D *) fin_bjet->Get("h_b_"+observable)->Clone("h_bjet_b");
    TH3D *h_bjet_b_tagged = (TH3D *) fin_bjet->Get("h_b_"+observable+"_tagged")->Clone("h_bjet_b_tagged");
    TH3D *h_bjet_bb = (TH3D *) fin_bjet->Get("h_bb_"+observable)->Clone("h_bjet_bb");
    TH3D *h_bjet_bb_tagged = (TH3D *) fin_bjet->Get("h_bb_"+observable+"_tagged")->Clone("h_bjet_bb_tagged");
    TH3D *h_bjet_c = (TH3D *) fin_bjet->Get("h_c_"+observable)->Clone("h_bjet_c");
    TH3D *h_bjet_c_tagged = (TH3D *) fin_bjet->Get("h_c_"+observable+"_tagged")->Clone("h_bjet_c_tagged");
    TH3D *h_bjet_l = (TH3D *) fin_bjet->Get("h_l_"+observable)->Clone("h_bjet_l");
    TH3D *h_bjet_l_tagged = (TH3D *) fin_bjet->Get("h_l_"+observable+"_tagged")->Clone("h_bjet_l_tagged");

    // Create the templates 
    TH3D *h_bbb = (TH3D *) h_dijet_b->Clone("h_bbb");
    h_bbb->Add(h_dijet_bb);
    h_bbb->Add(h_bjet_b);
    h_bbb->Add(h_bjet_bb);
    TH3D *h_c = (TH3D *) h_dijet_c->Clone("h_c");
    TH3D *h_l = (TH3D *) h_dijet_l->Clone("h_l");

    TH3D *h_bbb_tagged = (TH3D *) h_dijet_b_tagged->Clone("h_bbb_tagged");
    h_bbb_tagged->Add(h_dijet_bb_tagged);
    h_bbb_tagged->Add(h_bjet_b_tagged);
    h_bbb_tagged->Add(h_bjet_bb_tagged);
    TH3D *h_c_tagged = (TH3D *) h_dijet_c_tagged->Clone("h_c_tagged");
    TH3D *h_l_tagged = (TH3D *) h_dijet_l_tagged->Clone("h_l_tagged");

    // Make projections
    TH2D *h_data_2d = (TH2D *) h_data->Project3D("zx");
    TH2D *h_bbb_2d = (TH2D *) h_bbb->Project3D("zx");
    TH2D *h_c_2d = (TH2D *) h_c->Project3D("zx");
    TH2D *h_l_2d = (TH2D *) h_l->Project3D("zx");

    TH2D *h_data_tagged_2d = (TH2D *) h_data_tagged->Project3D("zx");
    TH2D *h_bbb_tagged_2d = (TH2D *) h_bbb_tagged->Project3D("zx");
    TH2D *h_c_tagged_2d = (TH2D *) h_c_tagged->Project3D("zx");
    TH2D *h_l_tagged_2d = (TH2D *) h_l_tagged->Project3D("zx");

    for (auto h : {
        h_data_2d, h_data_tagged_2d,
        h_bbb_2d, h_bbb_tagged_2d,
        h_c_2d, h_c_tagged_2d,
        h_l_2d, h_l_tagged_2d,
    }) {
        h->SetMarkerSize(1000);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetYaxis()->SetTitle("p_{T}^{jet}");
    }

    // TCanvas *c_data_incl = new TCanvas("c_data_incl", "Nincl", 1400, 600);
    // h_data_2d->Draw("colz text");
    // TLatex *data_incl_info = new TLatex;
    // data_incl_info->SetNDC();
    // data_incl_info->SetTextSize(text_size);
    // data_incl_info->DrawLatex(0.45, 0.92, "N^{incl} weighted counts");
    // data_incl_info->Draw();
    // c_data_incl->Print("plots/"+observable+"_nincl_yield.png");

    // TCanvas *c_data_tag = new TCanvas("c_data_tag", "Ntag", 1400, 600);
    // h_data_tagged_2d->Draw("colz text");
    // TLatex *data_tag_info = new TLatex;
    // data_tag_info->SetNDC();
    // data_tag_info->SetTextSize(text_size);
    // data_tag_info->DrawLatex(0.45, 0.92, "N^{tag} weighted counts");
    // data_tag_info->Draw();
    // c_data_tag->Print("plots/"+observable+"_ntag_yield.png");

    // TCanvas *c_bbb_incl = new TCanvas("c_bbb_incl", "Nincl", 1400, 600);
    // h_bbb_2d->Draw("colz text");
    // TLatex *bbb_incl_info = new TLatex;
    // bbb_incl_info->SetNDC();
    // bbb_incl_info->SetTextSize(text_size);
    // bbb_incl_info->DrawLatex(0.42, 0.92, "inclusive b jet template counts");
    // bbb_incl_info->Draw();
    // c_bbb_incl->Print("plots/"+observable+"_bbb_incl_template_yield.png");

    TCanvas *c_bbb_tag = new TCanvas("c_bbb_tag", "Ntag", 1400, 600);
    h_bbb_tagged_2d->Draw("colz text");
    TLatex *bbb_tag_info = new TLatex;
    bbb_tag_info->SetNDC();
    bbb_tag_info->SetTextSize(text_size);
    bbb_tag_info->DrawLatex(0.42, 0.92, "tagged b jet template counts");
    bbb_tag_info->Draw();
    c_bbb_tag->Print("plots/"+observable+"_bbb_tag_template_yield.png");

    // TCanvas *c_c_incl = new TCanvas("c_c_incl", "Nincl", 1400, 600);
    // h_c_2d->Draw("colz text");
    // TLatex *c_incl_info = new TLatex;
    // c_incl_info->SetNDC();
    // c_incl_info->SetTextSize(text_size);
    // c_incl_info->DrawLatex(0.42, 0.92, "inclusive c jet template counts");
    // c_incl_info->Draw();
    // c_c_incl->Print("plots/"+observable+"_c_incl_template_yield.png");

    TCanvas *c_c_tag = new TCanvas("c_c_tag", "Ntag", 1400, 600);
    h_c_tagged_2d->Draw("colz text");
    TLatex *c_tag_info = new TLatex;
    c_tag_info->SetNDC();
    c_tag_info->SetTextSize(text_size);
    c_tag_info->DrawLatex(0.42, 0.92, "tagged c jet template counts");
    c_tag_info->Draw();
    c_c_tag->Print("plots/"+observable+"_c_tag_template_yield.png");

    // TCanvas *c_l_incl = new TCanvas("c_l_incl", "Nincl", 1400, 600);
    // h_l_2d->Draw("colz text");
    // TLatex *l_incl_info = new TLatex;
    // l_incl_info->SetNDC();
    // l_incl_info->SetTextSize(text_size);
    // l_incl_info->DrawLatex(0.42, 0.92, "inclusive guds jet template counts");
    // l_incl_info->Draw();
    // c_l_incl->Print("plots/"+observable+"_l_incl_template_yield.png");

    TCanvas *c_l_tag = new TCanvas("c_l_tag", "Ntag", 1400, 600);
    h_l_tagged_2d->Draw("colz text");
    TLatex *l_tag_info = new TLatex;
    l_tag_info->SetNDC();
    l_tag_info->SetTextSize(text_size);
    l_tag_info->DrawLatex(0.42, 0.92, "tagged guds jet template counts");
    l_tag_info->Draw();
    c_l_tag->Print("plots/"+observable+"_l_tag_template_yield.png");


}