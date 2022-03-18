// L and C jets from qcdMC, B jets from bJetMC

void drawzgVSrg_par(bool ktORzgrg = true)
{
    //gStyle->SetOptTitle(0);
    //gStyle->SetTitleFontSize(1);
       
    // Change histfile if using subset
    std::string histfile = "kt_rgzg_par.root";
    //std::string histfile = "subset_kt_rgzg_par.root";
    TFile *fin = new TFile(histfile.c_str());
    
    
    // h__1 = B jets, h__2 = C jets, h__3 = light jets
    std::string hname1 = "h_bJet";
    std::string hname2 = "h_qcd";
    std::string hname3 = "h_qcd";
    //std::string title1d = "";
    std::string title2d = "";
    std::string xtitle = "ln(1/R_{g})";
    std::string ytitle2d = "";
    //std::string ytitle1d = "1/N dN/d";

    if (ktORzgrg) { 
        hname1 += "_ktB";
        hname2 += "_ktC";
        hname3 += "_ktL";
        title2d += "ln(k_{T}) vs ln(1/R_{g})";
        //title1d += "z_{g}";
        //xtitle += "z_{g}";
        //ytitle1d += "z_{g}";
        ytitle2d += "ln(k_{T}/GeV)";
    } else {
        hname1 += "_rgzgB";
        hname2 += "_rgzgC";
        hname3 += "_rgzgL";
        title2d += "ln(1/z_{g}) vs ln(1/R_{g})";
        //title1d += "R_{g}";
        //xtitle += "ln(1/R_{g})";
        //ytitle1d += "ln(1/R_{g})";
        ytitle2d += "ln(z_{g})";
    }
    
    // Load 2D histograms
    //float minpt = 50;
    //float maxpt = 80;
    
    TH2F *h2d1 = (TH2F *) fin->Get(hname1.c_str())->Clone();
    TH2F *h2d2 = (TH2F *) fin->Get(hname2.c_str())->Clone();
    TH2F *h2d3 = (TH2F *) fin->Get(hname3.c_str())->Clone();
    /*
    // Create 1D histograms - Projections
    int firstbin = h2d1->GetYaxis()->FindBin(minpt);
    minpt = h2d1->GetYaxis()->GetBinLowEdge(firstbin);

    int lastbin = h2d1->GetYaxis()->FindBin(maxpt);
    maxpt = h2d1->GetYaxis()->GetBinUpEdge(lastbin);
    
    TH1F *h1d1 =(TH1F *) h2d1->ProjectionX("h1d1", firstbin, lastbin, "e"); // We want underflow in zg, rg not pt
    TH1F *h1d2 =(TH1F *) h2d2->ProjectionX("h1d2", firstbin, lastbin, "e");
    TH1F *h1d3 =(TH1F *) h2d3->ProjectionX("h1d3", firstbin, lastbin, "e");
    */
    // Draw 2D histograms
    TCanvas *c2d = new TCanvas("c2d", "", 1300, 600);
    c2d->Divide(3, 1);
    
    title2d += ", parton level";
    
    // Add canvas title, histograms' titles
    c2d->cd();  
    TPad *pad2d = new TPad("all", "all", 0, 0, 1, 1);
    pad2d->SetFillStyle(4000);  // transparent
    pad2d->Draw();
    pad2d->cd();
    
    TLatex *ctitle = new TLatex();
    ctitle->SetTextSize(35);
    ctitle->DrawLatexNDC(0.38, 0.96, title2d.c_str());
    
    TLatex *lefttitle = new TLatex();
    lefttitle->SetTextSize(30);
    lefttitle->DrawLatexNDC(0.1, 0.92, "b-jets");
    
    TLatex *midtitle = new TLatex();
    midtitle->SetTextSize(30);
    midtitle->DrawLatexNDC(0.55, 0.92, "c-jets");
    
    TLatex *righttitle = new TLatex();
    righttitle->SetTextSize(30);
    righttitle->DrawLatexNDC(0.85, 0.92, "light jets");
    
    // Draw histograms    
    const int n = 3;
    TH2F *hs2d[n] = {h2d1, h2d2, h2d3};
    
    for (int ni = 0; ni < n; ni++) {
        c2d->cd(ni + 1);
        //c2d->cd(ni + 1)->SetLogz();
        
        TH2F *h = hs2d[ni];
        //h->Scale(100);
        h->GetXaxis()->SetCanExtend(true);
        //h->GetXaxis()->SetRange(0, h->GetXaxis()->GetNbins());
        //h->GetYaxis()->SetRange(h->GetYaxis()->FindBin(minpt), h->GetYaxis()->FindBin(maxpt));
        h->SetXTitle(xtitle.c_str());
        h->SetYTitle(ytitle2d.c_str());
        h->Draw("COLZ");
    }
    
    c2d->SetTitle(title2d.c_str());
    c2d->Draw();
    c2d->Show();   
    
    // Change savename if using subset
    std::string savename2d = "/home/llr/cms/kalipoliti/myMacros/par/bVSl/bVSl_" + title2d;
    //std::string savename2d = "/home/llr/cms/kalipoliti/myMacros/par/bVSl_subset/bVSl_sub_" + title2d;
    savename2d = std::regex_replace(savename2d, std::regex(", "), "_");
    savename2d = std::regex_replace(savename2d, std::regex(" "), "_");
    savename2d = std::regex_replace(savename2d, std::regex("_\\{g\\}"), "g");
    savename2d += ".png";
    
    //c2d->Print(savename2d.c_str());
    /*
    // Draw 1D histograms - Projections
    
    title1d += Form(", for p_{T} in [%.0f, %.0f] GeV, parton level", minpt, maxpt);
    
    TH1F *hs1d[n] = {h1d1, h1d2, h1d3};
    
    TCanvas *c1d = new TCanvas("c1d", "", 600, 600);
    
    float ymax = 0;
    
    Int_t counts;
    
    for (int ni = 0; ni < n; ni++) {
        TH1F *h = hs1d[ni];
        
        cout << "underflow: " << h->GetBinContent(0) << endl;
        counts = 0;
        for (int k = 1; k <= h->GetNbinsX(); k++) {
            counts += h->GetBinContent(k);
        }
        cout << "rest of the bins: " << counts << endl;
        
        
        // Extend to include underflow bin in normalisation
        h->GetXaxis()->SetCanExtend(true);
        h->GetXaxis()->SetRange(0, h->GetXaxis()->GetNbins());
        
        h->Scale(1/h->Integral("width"));
       
        // Fix the range drawn (underflow too large to show)
        h->GetXaxis()->SetRange(1, h->GetNbinsX());
        
        h->SetMarkerColor(ni + 2);
        h->SetMarkerSize(0.8);
        h->SetLineWidth(2);
        h->SetXTitle(xtitle.c_str());
        h->SetYTitle(ytitle1d.c_str());
        h->SetTitle(title1d.c_str());
        
        float hymax = h->GetMaximum();
        if (hymax > ymax) {
            ymax = hymax;
        }
        
        // To have a line outlining the bins
        h->SetFillColor(0);
        h->SetLineColor(ni + 2);
        h->Draw("hist same");
        h->Draw("e same");
    }
    
    // Fix Y range to see all histograms
    h1d1->GetYaxis()->SetRangeUser(0, ymax + 0.5);
    
    TLegend *leg = new TLegend(0.75, 0.7, 0.85, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(h1d1, Form("#kern[-0.2]{ } b"), "pl");
    leg->AddEntry(h1d2, Form("#kern[-0.2]{ } c"), "pl");
    leg->AddEntry(h1d3, Form("#kern[-0.2]{ } l"), "pl");
    leg->Draw();
    
    TLatex *c1dtitle = new TLatex();
    c1dtitle->SetTextSize(25);
    c1dtitle->DrawLatexNDC(0.15, 0.94, title1d.c_str());
    
    TLatex *txt1 = new TLatex();
    txt1->SetTextSize(25);
    //txt1->DrawLatexNDC(0.25, 0.6, "underflow: 15-54% of total entries");
    
    
    // Include if using subset
    TLatex *text = new TLatex();
    text->SetTextSize(25);
    text->DrawLatexNDC(0.5, 0.5, "only 1 b/c per jet");
    
    c1d->SetTitle(title1d.c_str());
    c1d->Draw();
    c1d->Show();
    
    // Change savename if using subset
    //std::string savename1d = "/home/llr/cms/kalipoliti/myMacros/par/bVSl/bVSl_" + title1d;
    std::string savename1d = "/home/llr/cms/kalipoliti/myMacros/par/bVSl_subset/bVSl_sub_" + title1d;
    savename1d = std::regex_replace(savename1d, std::regex(", "), "_");
    savename1d = std::regex_replace(savename1d, std::regex(" "), "_");
    savename1d = std::regex_replace(savename1d, std::regex("_\\{g\\}"), "g");
    savename1d = std::regex_replace(savename1d, std::regex("_\\{T\\}"), "T");
    savename1d = std::regex_replace(savename1d, std::regex("\\["), "");
    savename1d = std::regex_replace(savename1d, std::regex("\\]"), "");
    savename1d += ".png";
    
    c1d->Print(savename1d.c_str());
    */
}
