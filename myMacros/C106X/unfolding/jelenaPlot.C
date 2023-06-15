#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TInterpreter.h>
#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH1D.h>
#include <TString.h>
#include <TF1.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TDatabasePDG.h>
#include <TGraph.h>
#include <TTree.h>
#include <TMath.h>
#endif


void DrawLatex(Float_t x, Float_t y, Int_t color, const char* text, Float_t textSize = 0.06)
{
    TLatex* latex = new TLatex(x, y, text);
    latex->SetNDC();
    latex->SetTextSize(textSize);
    latex->SetTextColor(color);
    latex->SetTextFont(42);
    latex->Draw();
}



void jelenaPlot(int flagdummy,std:: string file, std::string fileother, int bintruexj1, int bintruexj2, int bintruerg1, int bintruerg2, int iter_ref, bool savePlots){


    int bindetxj1 = bintruexj1;
    int bindetxj2 = bintruexj2;

    int bindetrg1 = bintruerg1;
    int bindetrg2 = bintruerg2;
 
    TString label;
    if(flagdummy==0) label = "MatrixInversion/All/plots_data_inclusive60_3p5_purity";
    if(flagdummy==1) label = "MatrixInversion/All/plots_dummy_inclusive_3p5_purity";
    if(flagdummy==2) label = "MatrixInversion/All/plots_split_inclusive_3p5_purity";
   
    TString makedir = "mkdir -p Plots/"+label+"/";
    const char *mkDIR = makedir.Data();
    gSystem->Exec(mkDIR);

    TFile* f = new TFile(file.c_str());
    TFile* fprior = new TFile(fileother.c_str());
 
    //raw data or smeared MC
    TH2D* raw;
//    if(flagdummy==2) raw = (TH2D*)f->Get("smeared2");
    if(flagdummy==2) raw = (TH2D*)f->Get("raw");
//    if(flagdummy==1) raw = (TH2D*)f->Get("smeared");
    if(flagdummy==1) raw = (TH2D*)f->Get("raw");    
//    if(flagdummy==0) raw = (TH2D*)f->Get("raw_corrected");
    if(flagdummy==0) raw = (TH2D*)f->Get("raw");
    

    TH1D* raw1D = (TH1D*)raw->ProjectionX("raw1D",bindetxj1,bindetxj2);
    TH1D* raw1D_xj = (TH1D*)raw->ProjectionY("raw1D_xj",bindetrg1,bindetrg2);
    raw1D->Sumw2();
    raw1D_xj->Sumw2();

//raw1D_xj->SaveAs("raw_purity_xj.root");

    TH2D* smeared = (TH2D*)f->Get("smeared");
    TH1D* smeared1D = (TH1D*)smeared->ProjectionX("smeared1D",bindetxj1,bindetxj2);
    TH1D* smeared1D_xj = (TH1D*)smeared->ProjectionY("smeared1D_xj",bindetrg1,bindetrg2);
    smeared1D->Sumw2();
    smeared1D_xj->Sumw2();   


    //efficiency 
    //fully efficient                        
    TH2D* htrue = (TH2D*)f->Get("truef");
    TH1D* htrue1D=(TH1D*)htrue->ProjectionX("true1D",bintruexj1,bintruexj2);
    TH1D* htrue1D_xj=(TH1D*)htrue->ProjectionY("true1D_xj",bintruerg1,bintruerg2);
    htrue1D->Sumw2();
    htrue1D_xj->Sumw2();

//    TH2D* htrue2 = (TH2D*)f->Get("truef2");
//    TH1D* htrue1D_2=(TH1D*)htrue2->ProjectionX("true1D_2",bintruexj1,bintruexj2);

    //with det-level selection
    TH2D* htrue_ineff = (TH2D*)f->Get("true");
    TH1D* htrue_ineff_1D = htrue_ineff->ProjectionX("htrue_ineff_1D",bintruexj1,bintruexj2);
    TH1D* eff = htrue_ineff->ProjectionX("eff",bintruexj1,bintruexj2);
    TH1D* htrue_ineff_1D_xj = htrue_ineff->ProjectionY("htrue_ineff_1D_xj",bintruerg1,bintruerg2);
    TH1D* eff_xj = htrue_ineff->ProjectionY("eff_xj",bintruerg1,bintruerg2);

    eff->Divide(eff,htrue1D,1.0,1.0,"b");
    eff->Sumw2();
    eff_xj->Divide(eff_xj,htrue1D_xj,1.0,1.0,"b");
    eff_xj->Sumw2();          

    htrue1D->Scale(1./htrue1D->Integral(1,-1));                       
    htrue1D->Scale(1,"width");           
    htrue1D_xj->Scale(1./htrue1D_xj->Integral(1,-1));
    htrue1D_xj->Scale(1,"width");

//    htrue1D_2->Scale(1./htrue1D_2->Integral(1,-1));
//    htrue1D_2->Scale(1,"width");

    htrue_ineff_1D->Scale(1./htrue_ineff_1D->Integral(1,-1));
    htrue_ineff_1D->Scale(1,"width");
    htrue_ineff_1D_xj->Scale(1./htrue_ineff_1D_xj->Integral(1,-1));
    htrue_ineff_1D_xj->Scale(1,"width");

    //purity 
    TH2D* hpurity = (TH2D*)f->Get("hPurity");
       
    //unfolded
    TH2D* invert2d = (TH2D*)f->Get("MatrixInverision_Ufolded");
    TH1D* inverted = (TH1D*)invert2d->ProjectionX("projection_rg",bintruexj1,bintruexj2,"");
    TH1D* inverted_xj = (TH1D*)invert2d->ProjectionY("projection_xj",bintruerg1,bintruerg2,"");

    inverted->Sumw2();
    inverted->Divide(eff);
    inverted->Scale(1/inverted->Integral());
    inverted->Scale(1,"width");

    inverted_xj->Sumw2();
    inverted_xj->Divide(eff_xj);
    inverted_xj->Scale(1/inverted_xj->Integral());
    inverted_xj->Scale(1,"width");

    TH1D* inverted_div = (TH1D*)inverted->Clone("inverted_div") ;
    TH1D* inverted_ratio = (TH1D*)inverted->Clone("inverted_ratio") ;

    //folded back
    TH2D* folded_invert2d = (TH2D*)f->Get("MatrixInverision_Folded");
    TH1D* folded_inverted = (TH1D*)folded_invert2d->ProjectionX("folded_projection_rg",bindetxj1,bindetxj2,"");
    TH1D* folded_inverted_xj = (TH1D*)folded_invert2d->ProjectionY("folded_projection_xj",bindetrg1,bindetrg2,"");

    folded_inverted->Scale(1/folded_inverted->Integral());
    folded_inverted->Scale(1,"width");

    folded_inverted_xj->Scale(1/folded_inverted_xj->Integral());
    folded_inverted_xj->Scale(1,"width");


    //////////////////Plots/////////////////////////

    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    //Unfolded results
    TCanvas* canv1= new TCanvas(Form("canvas1"),Form("canvas1") ,1000,800);
    canv1->SetTicks();
    canv1->cd();
    TPad* pad1=new TPad("pad0","this is pad",0,0,1,1);
    pad1->SetFillColor(0);
    pad1->Draw();
    pad1->SetTicks(1,1);
    pad1->cd();
    pad1->Draw();
    pad1->SetTicks(1,1);
    pad1->cd();

    inverted->GetYaxis()->SetTitleOffset(0.9);
    inverted->GetXaxis()->SetTitleOffset(0.9);
    inverted->GetXaxis()->SetLabelFont(42);
    inverted->GetYaxis()->SetLabelFont(42);
    inverted->GetXaxis()->SetTitleFont(42);
    inverted->GetYaxis()->SetTitleFont(42);

    inverted->SetMaximum(2);
    inverted->SetMinimum(0.0001);
    inverted->GetXaxis()->SetTitleSize(0.05);
    inverted->GetYaxis()->SetTitleSize(0.05);
    if(flagdummy==0) inverted->GetYaxis()->SetTitle("Unfolded");
    if(flagdummy>=1) inverted->GetYaxis()->SetTitle("True MC");
    inverted->GetXaxis()->SetTitle("ln(1/#theta_{l})");
    inverted->Draw("e");
    inverted->SetLineColor(1);
    inverted->SetMarkerColor(1);
    inverted->SetMarkerStyle(21);
    htrue1D->Draw("same e");
    htrue1D->SetLineColor(2);
    htrue1D->SetMarkerColor(2);
    htrue1D->SetMarkerStyle(8);
    raw1D->Scale(1/raw1D->Integral());
    raw1D->Scale(1,"width");
    raw1D->Draw("same e");
    raw1D->SetLineColor(3);
    raw1D->SetMarkerColor(3);
    raw1D->SetMarkerStyle(22);

    //inverted->SaveAs("unfolded_matrixInversion.root");
    TLegend* lego1 = new TLegend(0.65, 0.7, 0.85, 0.85);
    lego1->SetBorderSize(0);
    lego1->SetTextSize(0.05);
    lego1->SetTextFont(42);
    lego1->AddEntry(inverted,"Unfolded", "lp");
    lego1->AddEntry(htrue1D,"True", "lp");
    lego1->AddEntry(raw1D,"Data", "lp");
    lego1->Draw();
    lego1->SetFillColor(0);

    TLatex Label1;
    Label1.SetNDC();
    Label1.SetTextFont(42);
    Label1.SetTextSize(0.03);
    Label1.DrawLatex(0.7,0.5,Form("%.f < p_{T}^{jet}< %.f", inverted_xj->GetBinLowEdge(bintruexj1), inverted_xj->GetBinLowEdge(bintruexj2+1)));

    if (savePlots) canv1->SaveAs(Form("Plots/"+label+"/Rg_unfolded_jetpt%.fto%.f.png",inverted_xj->GetBinLowEdge(bintruexj1),inverted_xj->GetBinLowEdge(bintruexj2+1)));


    //folded back
    TCanvas* canv2= new TCanvas(Form("canvas2"),Form("canvas2") ,1000,800);
    canv2->SetTicks();
    canv2->cd();
    TPad* pad2=new TPad("pad0","this is pad",0,0,1,1);
    pad2->SetFillColor(0);
    pad2->Draw();
    pad2->SetTicks(1,1);
    pad2->cd();
    pad2->Draw();
    pad2->SetTicks(1,1);
    pad2->cd();

    folded_inverted->GetYaxis()->SetTitleOffset(0.9);
    folded_inverted->GetXaxis()->SetTitleOffset(0.9);
    folded_inverted->GetXaxis()->SetLabelFont(42);
    folded_inverted->GetYaxis()->SetLabelFont(42);
    folded_inverted->GetXaxis()->SetTitleFont(42);
    folded_inverted->GetYaxis()->SetTitleFont(42);
 
    folded_inverted->SetMaximum(1.4);
    folded_inverted->SetMinimum(0.6);
    folded_inverted->GetXaxis()->SetTitleSize(0.05);
    folded_inverted->GetYaxis()->SetTitleSize(0.05);
    if(flagdummy==0) folded_inverted->GetYaxis()->SetTitle("Folded/Raw");
    if(flagdummy>=1) folded_inverted->GetYaxis()->SetTitle("Folded/Raw");
    folded_inverted->GetXaxis()->SetTitle("ln(1/#theta_{l})");
    folded_inverted->SetLineColor(1);
    folded_inverted->SetMarkerColor(1);
    folded_inverted->SetMarkerStyle(21);
    raw1D->SetLineColor(3);
    raw1D->SetMarkerColor(3);
    raw1D->SetMarkerStyle(22);

    folded_inverted->Divide(raw1D);
    folded_inverted->Draw("e");

    TLegend* lego2 = new TLegend(0.65, 0.7, 0.85, 0.85);
    lego2->SetBorderSize(0);
    lego2->SetTextSize(0.05);
    lego2->SetTextFont(42);
    lego2->AddEntry(folded_inverted,"Folded", "lp");
    lego2->AddEntry(raw1D,"Data", "lp");
    lego2->SetFillColor(0);

    TLatex Label2;
    Label2.SetNDC();
    Label2.SetTextFont(42);
    Label2.SetTextSize(0.03);
    Label2.DrawLatex(0.7,0.5,Form("%.f < p_{T}^{jet}< %.f", folded_inverted_xj->GetBinLowEdge(bindetxj1), folded_inverted_xj->GetBinLowEdge(bindetxj2+1)));
    if (savePlots) canv2->SaveAs(Form("Plots/"+label+"/Folded_unfolded_jetpt%.fto%.f.png",folded_inverted_xj->GetBinLowEdge(bindetxj1),folded_inverted_xj->GetBinLowEdge(bindetxj2+1)));


    //unfolded vs true
    TCanvas* canv3= new TCanvas(Form("canvas3"),Form("canvas3") ,1000,800);
    canv3->SetTicks();
    canv3->cd();
    TPad* pad3=new TPad("pad0","this is pad",0,0,1,1);
    pad3->SetFillColor(0);
    pad3->Draw();
    pad3->SetTicks(1,1);
    pad3->cd();
    pad3->Draw();
    pad3->SetTicks(1,1);
    pad3->cd();

    inverted_div->GetYaxis()->SetTitleOffset(0.9);
    inverted_div->GetXaxis()->SetTitleOffset(0.9);
    inverted_div->GetXaxis()->SetLabelFont(42);
    inverted_div->GetYaxis()->SetLabelFont(42);
    inverted_div->GetXaxis()->SetTitleFont(42);
    inverted_div->GetYaxis()->SetTitleFont(42);

    inverted_div->GetXaxis()->SetTitleSize(0.05);
    inverted_div->GetYaxis()->SetTitleSize(0.05);
    if(flagdummy==0) inverted_div->GetYaxis()->SetTitle("Unfolded/True");
    if(flagdummy>=1) inverted_div->GetYaxis()->SetTitle("Unfolded/True");

    inverted_div->GetXaxis()->SetTitle("ln(1/#theta_{l})");
    inverted_div->Divide(htrue1D);
    inverted_div->SetMaximum(2.0);
    inverted_div->SetMinimum(0.0);
    inverted_div->SetLineColor(1);
    inverted_div->SetMarkerColor(1);
    inverted_div->SetMarkerStyle(21);
    inverted_div->Draw("e");


    TLatex Label3;
    Label3.SetNDC();
    Label3.SetTextFont(42);
    Label3.SetTextSize(0.03);
    Label3.DrawLatex(0.7,0.5,Form("%.f < p_{T}^{jet}< %.f", inverted_xj->GetBinLowEdge(bintruexj1), inverted_xj->GetBinLowEdge(bintruexj2+1)));


    if (savePlots) canv3->SaveAs(Form("Plots/"+label+"/Unfolded_vs_true_jetpt%.fto%.f.png",inverted_xj->GetBinLowEdge(bintruexj1),inverted_xj->GetBinLowEdge(bintruexj2+1)));


    //bottom line test

    TCanvas* canv4= new TCanvas(Form("canvas4"),Form("canvas4") ,1000,800);
    canv4->SetTicks();
    canv4->cd();
    TPad* pad4=new TPad("pad0","this is pad",0,0,1,1);
    pad4->SetFillColor(0);
    pad4->Draw();
    pad4->SetTicks(1,1);
    pad4->cd();
    pad4->Draw();
    pad4->SetTicks(1,1);
    pad4->cd();

    TH1D* det_ratio=(TH1D*)raw1D->Clone("det_ratio") ;
    smeared1D->Scale(1/smeared1D->Integral());
    smeared1D->Scale(1,"width");
//    det_ratio->Chi2Test(smeared1D,"");
//    inverted_ratio->Chi2Test(htrue1D,"");    

    det_ratio->GetYaxis()->SetTitleOffset(0.9);
    det_ratio->GetXaxis()->SetTitleOffset(0.9);
    det_ratio->GetXaxis()->SetLabelFont(42);
    det_ratio->GetYaxis()->SetLabelFont(42);
    det_ratio->GetXaxis()->SetTitleFont(42);
    det_ratio->GetYaxis()->SetTitleFont(42);
    det_ratio->GetXaxis()->SetTitleSize(0.05);
    det_ratio->GetYaxis()->SetTitleSize(0.05);
    det_ratio->GetYaxis()->SetTitle("Bottom line");
    det_ratio->GetXaxis()->SetTitle("ln(1/#theta_{l})");
    det_ratio->SetMaximum(2.0);
    det_ratio->SetMinimum(0.0);
    det_ratio->SetLineColor(kBlack);
    det_ratio->SetMarkerColor(kBlack);
    det_ratio->SetMarkerStyle(21);

    inverted_ratio->SetLineColor(kCyan+2);
    inverted_ratio->SetMarkerColor(kCyan+2);
    inverted_ratio->SetMarkerStyle(21);

    //double chi2_det = det_ratio->Chi2Test(smeared1D,"P");
    //double chi2_true = inverted_ratio->Chi2Test(htrue1D,"P");
    //std::cout << " chi2 det = " << chi2_det << " chi2 true = " << chi2_true << std::endl;

    det_ratio->Divide(smeared1D);
    inverted_ratio->Divide(htrue1D);
//    det_ratio->Divide(inverted_ratio);

    det_ratio->Draw("e");
    inverted_ratio->Draw("same e");

    TLegend* lego4 = new TLegend(0.65, 0.7, 0.85, 0.85);
    lego4->SetBorderSize(0);
    lego4->SetTextSize(0.05);
    lego4->SetTextFont(42);
    lego4->AddEntry(inverted_ratio," true", "lp");
    lego4->AddEntry(det_ratio," det", "lp");
    lego4->Draw();
    lego4->SetFillColor(0);

    TLatex Label4;
    Label4.SetNDC();
    Label4.SetTextFont(42);
    Label4.SetTextSize(0.03);
    Label4.DrawLatex(0.7,0.5,Form("%.f < p_{T}^{jet}< %.f", inverted_xj->GetBinLowEdge(bintruexj1), inverted_xj->GetBinLowEdge(bintruexj2+1)));

    if (savePlots) canv4->SaveAs(Form("Plots/"+label+"/Bottom_line_jetpt%.fto%.f.png",inverted_xj->GetBinLowEdge(bintruexj1),inverted->GetBinLowEdge(bintruexj2+1)));

    //pt dimension
    //folded back
    TCanvas* canv5= new TCanvas(Form("canvas5"),Form("canvas5") ,1000,800);
    canv5->SetTicks();
    canv5->cd();
    TPad* pad5=new TPad("pad0","this is pad",0,0,1,1);
    pad5->SetFillColor(0);
    pad5->Draw();
    pad5->SetTicks(1,1);
    pad5->cd();
    pad5->Draw();
    pad5->SetTicks(1,1);
    pad5->cd();

    folded_inverted_xj->GetYaxis()->SetTitleOffset(0.9);
    folded_inverted_xj->GetXaxis()->SetTitleOffset(0.9);
    folded_inverted_xj->GetXaxis()->SetLabelFont(42);
    folded_inverted_xj->GetYaxis()->SetLabelFont(42);
    folded_inverted_xj->GetXaxis()->SetTitleFont(42);
    folded_inverted_xj->GetYaxis()->SetTitleFont(42);

    folded_inverted_xj->SetMaximum(1.4);
    folded_inverted_xj->SetMinimum(0.6);
    folded_inverted_xj->GetXaxis()->SetTitleSize(0.05);
    folded_inverted_xj->GetYaxis()->SetTitleSize(0.05);
    if(flagdummy==0) folded_inverted_xj->GetYaxis()->SetTitle("Folded/Raw");
    if(flagdummy>=1) folded_inverted_xj->GetYaxis()->SetTitle("Folded/Raw");
    folded_inverted_xj->GetXaxis()->SetTitle("ln(1/#theta_{l})");
    folded_inverted_xj->SetLineColor(1);
    folded_inverted_xj->SetMarkerColor(1);
    folded_inverted_xj->SetMarkerStyle(21);
    raw1D_xj->SetLineColor(3);
    raw1D_xj->SetMarkerColor(3);
    raw1D_xj->SetMarkerStyle(22);
    raw1D_xj->Scale(1/raw1D_xj->Integral());
    raw1D_xj->Scale(1,"width");

    folded_inverted_xj->Divide(raw1D_xj);
    folded_inverted_xj->Draw("e");

    TLegend* lego5 = new TLegend(0.65, 0.7, 0.85, 0.85);
    lego5->SetBorderSize(0);
    lego5->SetTextSize(0.05);
    lego5->SetTextFont(42);
    lego5->AddEntry(folded_inverted_xj,"Folded", "lp");
    lego5->AddEntry(raw1D_xj,"Data", "lp");
    lego5->SetFillColor(0);

    TLatex Label5;
    Label5.SetNDC();
    Label5.SetTextFont(42);
    Label5.SetTextSize(0.03);
    Label5.DrawLatex(0.7,0.5,Form("%.1f < ln(1/#theta_{l})< %.1f", folded_inverted->GetBinLowEdge(bindetrg1), folded_inverted->GetBinLowEdge(bindetrg2+1)));
//    if (savePlots) canv5->SaveAs(Form("Plots/"+label+"/Folded_unfolded_thetal%.1fto%.1f.png",folded_inverted->GetBinLowEdge(bindetrg1),folded_inverted->GetBinLowEdge(bindetrg2+1)));    


    //unfolded vs true    
    TCanvas* canv6= new TCanvas(Form("canvas6"),Form("canvas6") ,1000,800);
    canv6->SetTicks();
    canv6->cd();
    TPad* pad6=new TPad("pad0","this is pad",0,0,1,1);
    pad6->SetFillColor(0);
    pad6->Draw();
    pad6->SetTicks(1,1);
    pad6->cd();
    pad6->Draw();
    pad6->SetTicks(1,1);
    pad6->cd();

    TH1D* inverted_div_xj = (TH1D*)inverted_xj->Clone("inverted_div_xj") ;

    inverted_div_xj->GetYaxis()->SetTitleOffset(0.9);
    inverted_div_xj->GetXaxis()->SetTitleOffset(0.9);
    inverted_div_xj->GetXaxis()->SetLabelFont(42);
    inverted_div_xj->GetYaxis()->SetLabelFont(42);
    inverted_div_xj->GetXaxis()->SetTitleFont(42);
    inverted_div_xj->GetYaxis()->SetTitleFont(42);

    inverted_div_xj->GetXaxis()->SetTitleSize(0.05);
    inverted_div_xj->GetYaxis()->SetTitleSize(0.05);
    if(flagdummy==0) inverted_div_xj->GetYaxis()->SetTitle("Unfolded/True");
    if(flagdummy>=1) inverted_div_xj->GetYaxis()->SetTitle("Unfolded/True");

    inverted_div_xj->GetXaxis()->SetTitle("p_{T}^{jet}");
    inverted_div_xj->Divide(htrue1D_xj);
    inverted_div_xj->SetMaximum(2.0);
    inverted_div_xj->SetMinimum(0.0);
    inverted_div_xj->SetLineColor(1);
    inverted_div_xj->SetMarkerColor(1);
    inverted_div_xj->SetMarkerStyle(21);
    inverted_div_xj->Draw("e");

    inverted_div_xj->SaveAs("jetpt_purity.root");

    TLatex Label6;
    Label6.SetNDC();
    Label6.SetTextFont(42);
    Label6.SetTextSize(0.03);
    Label6.DrawLatex(0.7,0.5,Form("%.1f < ln(1/#theta_{l})< %.1f", inverted->GetBinLowEdge(bintruerg1), inverted->GetBinLowEdge(bintruerg2+1)));

//    if (savePlots) canv6->SaveAs(Form("Plots/"+label+"/Unfolded_vs_true_rg%.1fto%.1f.png",inverted->GetBinLowEdge(bintruerg1),inverted->GetBinLowEdge(bintruerg2+1)));

    //kine efficiency
    TCanvas *canv7= new TCanvas(Form("canvas7"),Form("canvas7") ,1000,800);
    canv7->SetTicks();
    canv7->cd();
    TPad* pad7=new TPad("pad0","this is pad",0,0,1,1);
    pad7->SetFillColor(0);
    pad7->Draw();
    pad7->SetTicks(1,1);
    pad7->cd();

    eff->GetYaxis()->SetTitleOffset(0.9);
    eff->GetXaxis()->SetTitleOffset(0.9);
    eff->GetXaxis()->SetLabelFont(42);
    eff->GetYaxis()->SetLabelFont(42);
    eff->GetXaxis()->SetTitleFont(42);
    eff->GetYaxis()->SetTitleFont(42);
    eff->GetYaxis()->SetRangeUser(0.4,1.1);
    eff->GetXaxis()->SetTitleSize(0.05);
    eff->GetYaxis()->SetTitleSize(0.05);
    eff->GetYaxis()->SetTitle("Kine Efficiency");
    eff->GetXaxis()->SetTitle("ln(1/#theta_{l})");
    eff->Draw("");

    DrawLatex(0.2, 0.8, 1,Form("Kinematic efficiency for the true bin p_{T}^{jet} %.f - %.f GeV",eff_xj->GetBinLowEdge(bintruexj1),eff_xj->GetBinLowEdge(bintruexj2+1)),0.025);
    if (savePlots) canv7->SaveAs(Form("Plots/"+label+"/Kineff_xj_RJ_%.fto%.f.png",eff_xj->GetBinLowEdge(bintruexj1),eff_xj->GetBinLowEdge(bintruexj1+1)));
    
    //purity
    TCanvas *canv8= new TCanvas(Form("canvas8"),Form("canvas8") ,1000,800);
    canv8->SetTicks();
    canv8->cd();
    TPad* pad8=new TPad("pad0","this is pad",0,0,1,1);
    pad8->SetFillColor(0);
    pad8->Draw();
    pad8->SetTicks(1,1);
    pad8->cd();

    hpurity->SetTitle("Purity");
    hpurity->GetYaxis()->SetTitleOffset(0.9);
    hpurity->GetXaxis()->SetTitleOffset(0.9);
    hpurity->GetXaxis()->SetLabelFont(42);
    hpurity->GetYaxis()->SetLabelFont(42);
    hpurity->GetXaxis()->SetTitleFont(42);
    hpurity->GetYaxis()->SetTitleFont(42);
    hpurity->GetXaxis()->SetTitleSize(0.05);
    hpurity->GetYaxis()->SetTitleSize(0.05);
    hpurity->GetYaxis()->SetTitle("p_{T}^{jet}");
    hpurity->GetXaxis()->SetTitle("ln(1/#theta_{l})");
    hpurity->SetMinimum(0);
    hpurity->SetMaximum(1);
    gStyle->SetPaintTextFormat("1.2f");
    hpurity->Draw("colz text");

    if (savePlots) canv8->SaveAs("Plots/"+label+"/Purity.png");

    //efficiency 2d
    TCanvas *canv9= new TCanvas(Form("canvas9"),Form("canvas9") ,1000,800);
    canv9->SetTicks();
    canv9->cd();
    TPad* pad9=new TPad("pad0","this is pad",0,0,1,1);
    pad9->SetFillColor(0);
    pad9->Draw();
    pad9->SetTicks(1,1);
    pad9->cd();

    TH2D* eff_2D = (TH2D*)htrue_ineff->Clone("eff_2D");    
    eff_2D->SetTitle("Kine efficiency");
    eff_2D->GetYaxis()->SetTitleOffset(0.9);
    eff_2D->GetXaxis()->SetTitleOffset(0.9);
    eff_2D->GetXaxis()->SetLabelFont(42);
    eff_2D->GetYaxis()->SetLabelFont(42);
    eff_2D->GetXaxis()->SetTitleFont(42);
    eff_2D->GetYaxis()->SetTitleFont(42);
    eff_2D->GetXaxis()->SetTitleSize(0.05);
    eff_2D->GetYaxis()->SetTitleSize(0.05);
    eff_2D->GetYaxis()->SetTitle("p_{T}^{jet}");
    eff_2D->GetXaxis()->SetTitle("ln(1/#theta_{l})");
    eff_2D->SetMinimum(0);
    eff_2D->SetMaximum(1);
    gStyle->SetPaintTextFormat("1.2f");
    eff_2D->Divide(htrue);
    eff_2D->Draw("colz text");

//    if (savePlots) canv8->SaveAs("Plots/"+label+"/Efficiency2D.png");
 

}

