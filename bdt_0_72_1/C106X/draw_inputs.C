void draw_inputs(TString model="qcd_bjet_TMVA", TString var="trkIp3dSig")
{
    TString fin_name = "histos/"+model+"_inputs.root";
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TH1F *h_sig = (TH1F *) fin->Get("h_"+var+"_sig");
    h_sig->GetXaxis()->SetRange(0, h_sig->GetNbinsX()+1);
    h_sig->Scale(1/h_sig->Integral());
    h_sig->SetFillStyle(3345);
    h_sig->SetFillColor(kRed);
    h_sig->SetLineColor(kRed);
    h_sig->SetTitle("Signal");
    h_sig->GetXaxis()->SetTitle(var);
    h_sig->GetYaxis()->SetTitle("normalized per track");

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

    TLegend *leg = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg->SetFillStyle(0);
    leg->AddEntry(h_sig, "Signal", "f");
    leg->AddEntry(h_bkg, "Background", "f");

    TCanvas *c = new TCanvas("c", "", 600, 600);
    h_sig->Draw("hist");
    h_bkg->Draw("hist same");
    leg->Draw();

    // TLatex *prelim = new TLatex;
    // prelim->SetNDC();
    // prelim->SetTextSize(20);
    // prelim->DrawLatex(0.175, 0.92, "#bf{CMS} #it{Simulation}");
    // prelim->Draw();

    // TLatex *lumi = new TLatex;
    // lumi->SetNDC();
    // lumi->SetTextSize(20);
    // lumi->DrawLatex(0.58, 0.92, "#bf{PYTHIA8 (pp 5.02 TeV)}");
    // lumi->Draw();

    c->Draw();
    // c->Print("plots_an/"+model+"_"+var+".png");

}
