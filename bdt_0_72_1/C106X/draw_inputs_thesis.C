#include "cms_palette.h"

void draw_inputs_thesis(TString model="qcd_bjet_TMVA")
{
    // Setup plot
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetCanvasPreferGL(kTRUE);

    std::vector<TString> vars = {
        "trkIp3dSig", "trkIp2dSig", "trkDistToAxis",
        "svtxdls", "svtxdls2d", "svtxnormchi2",
        "svtxm", "svtxmcorr", "svtxNtrk",
        "svtxTrkPtOverSv", "jtpt"
    };

    std::vector<TString> var_names = {
        "Track 3D IP sig.",
        "Track 2D IP sig.",
        "Track distance to jet axis",
        "SV 3D flight distance sig.",
        "SV 2D flight distance sig.",
        "SV normalized #chi^{2}",
        "SV mass",
        "SV corrected mass",
        "SV number of tracks",
        "Track p_{T} / SV p_{T}",
        "Jet p_{T}",
    };

    TString fin_name = "histos/"+model+"_inputs.root";
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TCanvas *c = new TCanvas("c", "", 1300, 1500);

    double x=0.3;
    double xl=0.08, xr=1-(3*x)-xl;
    double x1=xl+x, x2=x1+x;

    double y=0.22;
    double yb=0.08, yt=1-(4*y)-yb;
    double y1=yb+y, y2=y1+y, y3=y2+y;

    double gapx = 0.01, gapy = 0.01;

    std::cout << "x1 = " << x1 << ", x2 = " << x2 << std::endl;
    std::cout << "y1 = " << y1 << ", y2 = " << y2 << ", y3 = " << y3 << std::endl;

    // Create pads
    TPad *pad11 = new TPad("pad11", "", 0., 0., x1, y1);
    TPad *pad12 = new TPad("pad12", "", x1, 0., x2, y1);
    TPad *pad13 = new TPad("pad13", "", x2, 0., 1., y1);

    TPad *pad21 = new TPad("pad21", "", 0., y1, x1, y2);
    TPad *pad22 = new TPad("pad22", "", x1, y1, x2, y2);
    TPad *pad23 = new TPad("pad23", "", x2, y1, 1., y2);

    TPad *pad31 = new TPad("pad31", "", 0., y2, x1, y3);
    TPad *pad32 = new TPad("pad32", "", x1, y2, x2, y3);
    TPad *pad33 = new TPad("pad33", "", x2, y2, 1., y3);

    TPad *pad41 = new TPad("pad41", "", 0., y3, x1, 1.);
    TPad *pad42 = new TPad("pad42", "", x1, y3, x2, 1.);
    TPad *pad43 = new TPad("pad43", "", x2, y3, 1., 1.);

    for (auto pad : {pad41, pad42, pad43}) {
        pad->SetBottomMargin(5*gapy/(1-y3));
        pad->SetTopMargin((yt+gapy)/(1-y3));
    }
    for (auto pad : {pad31, pad32, pad33}) {
        pad->SetBottomMargin(5*gapy/y);
        pad->SetTopMargin(gapy/y);
    }
    for (auto pad : {pad21, pad22, pad23}) {
        pad->SetBottomMargin(5*gapy/y);
        pad->SetTopMargin(gapy/y);
    }
    for (auto pad : {pad11, pad12, pad13}) {
        pad->SetBottomMargin((yb+5*gapy)/y1);
        pad->SetTopMargin(gapy/y1);
    }

    for (auto pad : {pad41, pad31, pad21, pad11}) {
        pad->SetLeftMargin((xl + gapx) / x1);
        pad->SetRightMargin(gapx / x1);
    }
    for (auto pad : {pad42, pad32, pad22, pad12}) {
        pad->SetLeftMargin(gapx / x);
        pad->SetRightMargin(gapx / x);
    }
    for (auto pad : {pad43, pad33, pad23, pad13}) {
        pad->SetLeftMargin(gapx / (1 - x2));
        pad->SetRightMargin((xr + gapx) / (1 - x2));
    }

    std::vector<TPad *> pads = {
        pad41, pad42, pad43,
        pad31, pad32, pad33,
        pad21, pad22, pad23,
        pad11, pad12, pad13
    };

    for (int i=0; i<vars.size(); i++) {
        TString var = vars[i];

        TH1F *h_sig = (TH1F *) fin->Get("h_"+var+"_sig");
        h_sig->GetXaxis()->SetRange(0, h_sig->GetNbinsX()+1);
        h_sig->Scale(1/h_sig->Integral());
        h_sig->SetFillStyle(1001);
        h_sig->SetFillColorAlpha(cmsOrange,0.5);
        h_sig->SetLineColor(cmsOrange);
        h_sig->GetXaxis()->SetTitle(var_names[i]);
        h_sig->GetXaxis()->SetTitleOffset(3.9);
        h_sig->GetYaxis()->SetTitle("Normalized per track");
        h_sig->GetYaxis()->SetTitleOffset(4.);
        if (i%3!=0) h_sig->GetYaxis()->SetLabelOffset(10.);
        h_sig->GetYaxis()->SetNdivisions(5);

        TH1F *h_bkg = (TH1F *) fin->Get("h_"+var+"_bkg");
        h_bkg->GetXaxis()->SetRange(0, h_bkg->GetNbinsX()+1);
        h_bkg->Scale(1/h_bkg->Integral());
        h_bkg->SetFillStyle(1001);
        h_bkg->SetFillColorAlpha(cmsViolet,0.5);
        h_bkg->SetLineColor(cmsViolet);

        double ymax = std::max({h_bkg->GetMaximum(), h_sig->GetMaximum()});
        h_sig->GetYaxis()->SetRangeUser(0, ymax*1.1);
        h_bkg->GetYaxis()->SetRangeUser(0, ymax*1.1);

        pads[i]->cd();

        h_sig->Draw("hist");
        h_bkg->Draw("hist same");

        TLegend *leg = new TLegend(0.05, 0.45, 0.8, 0.6);
        leg->SetFillStyle(0);
        leg->SetMargin(0.1);
        leg->AddEntry(h_sig, "Signal (from b decay)", "f");
        leg->AddEntry(h_bkg, "Background (from primary vertex)", "f");

        if (i==0) {
            TLatex *prelim = new TLatex;
            prelim->SetNDC();
            prelim->SetTextSize(28);
            prelim->SetTextAlign(12);   
            prelim->DrawLatex(0.22, 0.9, "#bf{CMS} #it{Private work}");
        }

        if (i==2) {
            TLatex *lumi = new TLatex;
            lumi->SetNDC();
            lumi->SetTextSize(28);
            lumi->SetTextSize(28);
            lumi->SetTextAlign(32);
            lumi->DrawLatex(0.93, 0.9, "PYTHIA8 (pp 5.02 TeV)");
        }

        if (i==0) {
            pads[11]->cd();
            TLatex *info = new TLatex;
            info->SetNDC();
            info->SetTextSize(24);
            info->DrawLatex(0.1, 0.9, "Charged particles with p_{T}^{track} > 1 GeV");
            info->DrawLatex(0.1, 0.8, "from anti-k_{T}, R=0.4 b-tagged b jets");
            info->DrawLatex(0.1, 0.7, "p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2");
            info->Draw();
            
            // TLegend *leg = new TLegend(0.05, 0.45, 0.8, 0.6);
            // leg->SetFillStyle(0);
            // leg->SetMargin(0.1);
            // leg->AddEntry(h_sig, "Signal (from b decay)", "f");
            // leg->AddEntry(h_bkg, "Background (from primary vertex)", "f");
            leg->Draw();
        }

        c->cd();
        pads[i]->Draw();
    }
    c->cd();
    pads[11]->Draw();
    c->Draw();
    c->Print("../../myMacros/C106X/plots_thesis/"+model+"_inputs.pdf");
}
