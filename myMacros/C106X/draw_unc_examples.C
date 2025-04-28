#include "cms_palette.h"

const Float_t text_size_large = 28.;
const Float_t text_size_small = 26.;

void draw_unc_examples()
{
    gStyle->SetLegendTextSize(text_size_large);
    gStyle->SetLabelSize(text_size_small, "XYZ");
    gStyle->SetTitleSize(text_size_large, "XYZ");
    int colors[3] = {kBlack, cmsBlue, cmsRed};

    TH1D *h1 = new TH1D("h1", "h1", 20, -1, 1);
    TH1D *h2 = new TH1D("h2", "h2", 20, -1, 1);
    TH1D *h3 = new TH1D("h3", "h3", 20, -1, 1);

    for (int i=0; i<50000; i++) {
        h1->Fill(gRandom->Gaus(0.,0.5));
        h2->Fill(gRandom->Gaus(0.1,0.5));
        h3->Fill(gRandom->Gaus(-0.1,0.5));
    }

    for (auto h : {h1, h2, h3}) {
        h->Scale(1/h->Integral());
    }

    TH1D *rel2 = (TH1D*) h2->Clone("rel2");
    rel2->Add(h1, -1.);
    rel2->Divide(h1);

    TH1D *rel3 = (TH1D*) h3->Clone("rel3");
    rel3->Add(h1, -1.);
    rel3->Divide(h1);

    TH1D *up1 = (TH1D *) h1->Clone("up1"); // 1-point variation
    TH1D *up2 = (TH1D *) h1->Clone("up2"); // 2-point variation
    for (int i=1; i<=h1->GetNbinsX(); i++) {
        double unc1 = rel2->GetBinContent(i);
        double unc2 = rel3->GetBinContent(i);
        up1->SetBinContent(i, std::abs(unc1));
        up2->SetBinContent(i, std::max(std::abs(unc1),std::abs(unc2)));
    }

    TH1D *down1 = (TH1D *) up1->Clone("down1");
    down1->Scale(-1.);
    TH1D *down2 = (TH1D *) up2->Clone("down2");
    down2->Scale(-1.);

    for (auto h : {up1, up2, down1, down2}) {
        h->SetFillColor(kGray);
        h->SetFillStyle(1001);
        h->SetLineColor(kGray);
    }

    h1->SetLineColor(kBlack);
    h2->SetLineColor(cmsRed);
    rel2->SetLineColor(cmsRed);
    h3->SetLineColor(cmsBlue);
    rel3->SetLineColor(cmsBlue);

    // 1-point variation symmetrization 

    TLegend *leg1 = new TLegend(0.4, 0.2, 0.6, 0.4);
    leg1->SetMargin(0.3);
    leg1->AddEntry(h1, "Nominal", "l");
    leg1->AddEntry(h2, "Variation", "l");

    TLegend *leg1_unc = new TLegend(0.3, 0.7, 0.6, 0.9);
    leg1_unc->SetMargin(0.3);
    leg1_unc->AddEntry(rel2, "Uncertainty", "l");
    leg1_unc->AddEntry(up1, "Symmetrized uncertainty", "f");

    TCanvas *c1 = new TCanvas("c1","",600,800);
    TPad *pad1b = new TPad("pad1b", "", 0., 0., 1., 0.5);
    TPad *pad1t = new TPad("pad1t", "", 0., 0.5, 1., 1.);

    pad1b->SetBottomMargin(0.2);
    pad1b->SetTopMargin(0.03);
    pad1t->SetBottomMargin(0.02);
    pad1t->SetTopMargin(0.1);
    for (auto pad : {pad1b, pad1t}) {
        pad->SetLeftMargin(0.15);
        pad->SetRightMargin(0.05);
    }

    pad1t->cd();
    for (auto h : {h1, h2}) {
        h->SetLineWidth(3);
        h->GetYaxis()->SetRangeUser(1e-3,0.1);
        int ny1 = 5; // 1st divisions
        int ny2 = 3; // 2nd divisions
        int ny3 = 0; // 3rd divisions
        int nydiv = ny1 + 100*ny2 + 10000*ny3;
        h->GetYaxis()->SetNdivisions(nydiv);
        h->GetYaxis()->SetTitle("1/N dN/dx");
        h->GetYaxis()->SetTitleOffset(2.);

        // h->GetXaxis()->SetRangeUser(-1.,1.);
        // int nx1 = 5; // 1st divisions
        // int nx2 = 2; // 2nd divisions
        // int nx3 = 0; // 3rd divisions
        // int nxdiv = nx1 + 100*nx2 + 10000*nx3;
        // h->GetXaxis()->SetNdivisions(nxdiv);
        h->GetXaxis()->SetLabelOffset(20);

        h->Draw("same hist c");
    }
    leg1->Draw();

    pad1b->cd();

    rel2->GetYaxis()->SetRangeUser(-0.75,1.5);
    int ny1 = 5; // 1st divisions
    int ny2 = 3; // 2nd divisions
    int ny3 = 0; // 3rd divisions
    int nydiv = ny1 + 100*ny2 + 10000*ny3;
    rel2->GetYaxis()->SetNdivisions(nydiv);
    rel2->GetYaxis()->SetTitle("Relative uncertainty");
    rel2->GetYaxis()->SetTitleOffset(2.);

    rel2->GetXaxis()->SetTitle("x");
    rel2->GetXaxis()->SetTitleOffset(2.);
    rel2->SetLineWidth(3);

    rel2->Draw("same hist c");
    up1->Draw("same hist c");
    down1->Draw("same hist c");
    rel2->Draw("same hist c");
    leg1_unc->Draw();

    c1->cd();
    pad1t->Draw();
    pad1b->Draw();
    c1->Print("plots_thesis/unc_1p.pdf");

    // 2-point variation symmetrization

    TLegend *leg2 = new TLegend(0.4, 0.1, 0.6, 0.4);
    leg2->SetMargin(0.3);
    leg2->AddEntry(h1, "Nominal", "l");
    leg2->AddEntry(h2, "Variation1", "l");
    leg2->AddEntry(h3, "Variation2", "l");

    TLegend *leg2_unc = new TLegend(0.3, 0.6, 0.6, 0.9);
    leg2_unc->SetMargin(0.3);
    leg2_unc->AddEntry(rel2, "Uncertainty1", "l");
    leg2_unc->AddEntry(rel3, "Uncertainty2", "l");
    leg2_unc->AddEntry(up2, "Symmetrized uncertainty", "f");
    
    TCanvas *c2 = new TCanvas("c2","",600,800);
    TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.5);
    TPad *pad2 = new TPad("pad2", "", 0., 0.5, 1., 1.);

    pad1->SetBottomMargin(0.2);
    pad1->SetTopMargin(0.03);
    pad2->SetBottomMargin(0.02);
    pad2->SetTopMargin(0.1);
    for (auto pad : {pad1, pad2}) {
        pad->SetLeftMargin(0.15);
        pad->SetRightMargin(0.05);
    }

    pad2->cd();
    for (auto h : {h1, h2, h3}) {
        h->SetLineWidth(3);

        h->GetYaxis()->SetRangeUser(1e-3,0.1);
        int ny1 = 5; // 1st divisions
        int ny2 = 3; // 2nd divisions
        int ny3 = 0; // 3rd divisions
        int nydiv = ny1 + 100*ny2 + 10000*ny3;
        h->GetYaxis()->SetNdivisions(nydiv);
        h->GetYaxis()->SetTitle("1/N dN/dx");
        h->GetYaxis()->SetTitleOffset(2.);

        // h->GetXaxis()->SetRangeUser(-1.,1.);
        // int nx1 = 5; // 1st divisions
        // int nx2 = 2; // 2nd divisions
        // int nx3 = 0; // 3rd divisions
        // int nxdiv = nx1 + 100*nx2 + 10000*nx3;
        // h->GetXaxis()->SetNdivisions(nxdiv);
        h->GetXaxis()->SetLabelOffset(20);

        h->Draw("same hist c");
    }
    leg2->Draw();

    pad1->cd();
    for (auto h : {rel2, rel3}) {
        h->SetLineWidth(3);

        h->GetYaxis()->SetRangeUser(-0.75,1.5);
        int ny1 = 5; // 1st divisions
        int ny2 = 3; // 2nd divisions
        int ny3 = 0; // 3rd divisions
        int nydiv = ny1 + 100*ny2 + 10000*ny3;
        h->GetYaxis()->SetNdivisions(nydiv);
        h->GetYaxis()->SetTitle("Relative uncertainty");
        h->GetYaxis()->SetTitleOffset(2.);

        h->GetXaxis()->SetTitle("x");
        h->GetXaxis()->SetTitleOffset(2.);

        h->Draw("same hist c");
    }
    up2->Draw("same hist c");
    down2->Draw("same hist c");
    rel2->Draw("same hist c");
    rel3->Draw("same hist c");
    leg2_unc->Draw();

    c2->cd();
    pad2->Draw();
    pad1->Draw();
    c2->Print("plots_thesis/unc_2p.pdf");
}