void draw_inputs(TString model="qcd_bjet_TMVA")
{
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

    TCanvas *c = new TCanvas("c", "", 1200, 2000);
    TPad *pad11 = new TPad(Form("pad11"),"",0.,0.,0.33,0.25);
    TPad *pad12 = new TPad(Form("pad12"),"",0.33,0.,0.66,0.25);
    TPad *pad13 = new TPad(Form("pad13"),"",0.66,0.,0.99,0.25);
    TPad *pad21 = new TPad(Form("pad21"),"",0.,0.25,0.33,0.5);
    TPad *pad22 = new TPad(Form("pad22"),"",0.33,0.25,0.66,0.5);
    TPad *pad23 = new TPad(Form("pad23"),"",0.66,0.25,0.99,0.5);
    TPad *pad31 = new TPad(Form("pad31"),"",0.,0.5,0.33,0.75);
    TPad *pad32 = new TPad(Form("pad32"),"",0.33,0.5,0.66,0.75);
    TPad *pad33 = new TPad(Form("pad33"),"",0.66,0.5,0.99,0.75);
    TPad *pad41 = new TPad(Form("pad41"),"",0.,0.75,0.33,1.);
    TPad *pad42 = new TPad(Form("pad42"),"",0.33,0.75,0.66,1.);
    TPad *pad43 = new TPad(Form("pad43"),"",0.66,0.75,0.99,1.);

    std::vector<TPad *> pads = {
        pad41, pad42, pad43,
        pad31, pad32, pad33,
        pad21, pad22, pad23,
        pad11, pad12, pad13,
    };

    for (int i=0; i<vars.size(); i++) {
        TString var = vars[i];

        TH1F *h_sig = (TH1F *) fin->Get("h_"+var+"_sig");
        h_sig->GetXaxis()->SetRange(0, h_sig->GetNbinsX()+1);
        h_sig->Scale(1/h_sig->Integral());
        h_sig->SetFillStyle(3345);
        h_sig->SetFillColor(kRed);
        h_sig->SetLineColor(kRed);
        h_sig->SetTitle("Signal");
        h_sig->GetXaxis()->SetTitle(var_names[i]);
        h_sig->GetXaxis()->SetTitleOffset(3.5);
        h_sig->GetYaxis()->SetTitle("normalized per track");
        h_sig->GetYaxis()->SetTitleOffset(3.5);

        TH1F *h_bkg = (TH1F *) fin->Get("h_"+var+"_bkg");
        h_bkg->GetXaxis()->SetRange(0, h_bkg->GetNbinsX()+1);
        h_bkg->Scale(1/h_bkg->Integral());
        h_bkg->SetFillStyle(3354);
        h_bkg->SetFillColor(kBlue);
        h_bkg->SetLineColor(kBlue);
        h_bkg->SetTitle("Background");
        h_bkg->GetXaxis()->SetTitle(var);
        h_bkg->GetYaxis()->SetTitle("normalized per track");

        double ymax = std::max({h_bkg->GetMaximum(), h_sig->GetMaximum()});
        h_sig->GetYaxis()->SetRangeUser(0, ymax*1.1);
        h_bkg->GetYaxis()->SetRangeUser(0, ymax*1.1);

        pads[i]->SetRightMargin(0.1);
        pads[i]->SetBottomMargin(0.2);
        pads[i]->cd();

        h_sig->Draw("hist");
        h_bkg->Draw("hist same");

        if (i==0) {
            TLatex *prelim = new TLatex;
            prelim->SetNDC();
            prelim->SetTextSize(20);
            prelim->DrawLatex(0.175, 0.92, "#bf{CMS} #it{Simulation}");
            prelim->Draw();
        }

        if (i==2) {
            TLatex *lumi = new TLatex;
            lumi->SetNDC();
            lumi->SetTextSize(20);
            lumi->DrawLatex(0.38, 0.92, "#bf{PYTHIA8 (pp 5.02 TeV)}");
            lumi->Draw();
        }

        pads[11]->cd();
        TLatex *info = new TLatex;
        info->SetNDC();
        info->SetTextSize(20);
        info->DrawLatex(0.1, 0.8, "Charged particles with p_{T}^{track} > 1 GeV");
        info->DrawLatex(0.1, 0.7, "from anti-k_{T}, R=0.4 b-tagged true b jets");
        info->DrawLatex(0.1, 0.6, "p_{T}^{jet} > 30 GeV");
        info->Draw();
        
        TLegend *leg = new TLegend(0.1, 0.1, 0.8, 0.5);
        leg->SetFillStyle(0);
        leg->AddEntry(h_sig, "Signal (from b decay)", "f");
        leg->AddEntry(h_bkg, "Background (from primary vertex)", "f");
        leg->Draw();

        c->cd();
        pads[i]->Draw();
        pads[11]->Draw();

    }
    c->Draw();
    c->Print("plots_an/"+model+"_inputs.png");
}
