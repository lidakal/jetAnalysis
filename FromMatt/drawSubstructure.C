void drawSubstructure(bool doLateSD = false, bool zgORrg=false){

  TFile *f = NULL;
  if(doLateSD) f=new TFile("qcd_lateSD.root");
  else f=new TFile("qcd_SD.root");

  TH2F *h2b = NULL;
  TH2F *h2l = NULL;
  TH2F *h2b_dynKt = NULL;
  TH2F *h2l_dynKt = NULL;
  if(zgORrg){
    h2b = (TH2F*)f->Get("hB_RgVsJetPt");
    h2l = (TH2F*)f->Get("hL_RgVsJetPt");
    h2b_dynKt = (TH2F*)f->Get("hB_RgVsJetPt_dynKt");
    h2l_dynKt = (TH2F*)f->Get("hL_RgVsJetPt_dynKt");
  }
  else{
    h2b = (TH2F*)f->Get("hB_ZgVsJetPt");
    h2l = (TH2F*)f->Get("hL_ZgVsJetPt");
    h2b_dynKt = (TH2F*)f->Get("hB_ZgVsJetPt_dynKt");
    h2l_dynKt = (TH2F*)f->Get("hL_ZgVsJetPt_dynKt");
  }
  TH1D *hb = (TH1D*)h2b->ProjectionX("hb",11,20);
  TH1D *hl = (TH1D*)h2l->ProjectionX("hl",11,20);
  TH1D *hb_dynKt = (TH1D*)h2b_dynKt->ProjectionX("hb_dynKt",11,20);
  TH1D *hl_dynKt = (TH1D*)h2l_dynKt->ProjectionX("hl_dynKt",11,20);

  if(zgORrg){
    hl->SetXTitle("R_{g}");
    hl->SetYTitle("1/N_{jet} dN/dR_{g}");
    hl_dynKt->SetXTitle("R_{g}");
    hl_dynKt->SetYTitle("1/N_{jet} dN/dR_{g}");
  }
  else{
    hl->SetXTitle("z_{g}");
    hl->SetXTitle("1/N_{jet} dN/dz_{g}");
    hl_dynKt->SetXTitle("z_{g}");
    hl_dynKt->SetXTitle("1/N_{jet} dN/dz_{g}");
  }


  TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->AddEntry(hb,"b","pl");
  leg->AddEntry(hl,"l","pl");

  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  hb->Scale(1/hb->Integral());
  hl->Scale(1/hl->Integral());
  hb->SetLineColor(2);
  hb->SetMarkerColor(2);
  hl->Draw();
  hb->Draw("same");
  leg->Draw();

  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  hb_dynKt->Scale(1/hb_dynKt->Integral());
  hl_dynKt->Scale(1/hl_dynKt->Integral());
  hb_dynKt->SetLineColor(2);
  hb_dynKt->SetMarkerColor(2);
  hl_dynKt->Draw();
  hb_dynKt->Draw("same");
  leg->Draw();
  

}
