//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: RooUnfoldExample.cxx 279 2011-02-11 18:23:44Z T.J.Adye $
//
// Description:
//      Simple example usage of the RooUnfold package using toy MC.
//
// Authors: Tim Adye <T.J.Adye@rl.ac.uk> and Fergus Wilson <fwilson@slac.stanford.edu>
//
//==============================================================================


#if !(defined(__CINT__) || defined(__CLING__)) || defined(__ACLIC__)
#include <iostream>
using std::cout;
using std::endl;

#include "TRandom.h"
#include "TH1D.h"

#include "TFile.h"
#include "TVectorD.h"

#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom.h"
#include "TPostScript.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLine.h"
#include "TNtuple.h"
#include "TProfile.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
#include "sstream"
//#include "RooUnfoldTestHarness2D.h"
#endif

//==============================================================================
// Global definitions
//==============================================================================

const Double_t cutdummy= -99999.0;



void jelenaUnfold(std::string file_mc="histos/qcd_bjet_aggrTMVA_withNb_unfold_2d_square.root", std::string file_split="Output_Response2_matrixInv_3p5_30per_purCorr_ch.root",std::string file_data="Output_Data.root",int flag=1)

//void RooUnfoldExample_pp(std::string file_mc="Output_Response_pfDown.root", std::string file_split="Output_Response2.root",std::string file_data="Output_Data.root",int flag=0)
{

    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;   

    TFile* f_mc;
    TFile* f_data;
 
    f_mc=new TFile(file_mc.c_str());
    if(flag<2) f_data=new TFile(file_data.c_str());

    if(flag==2)f_data=new TFile(file_split.c_str());

    TH2D *h2raw;
    if(flag==2) h2raw=(TH2D*)f_data->Get("smeared2");
    if(flag==1) h2raw=(TH2D*)f_mc->Get("denom");
    if(flag==0) h2raw=(TH2D*)f_data->Get("raw_corrected");

    TH2D *h2smeared=(TH2D*)f_mc->Get("smeared");
    TH2D *h2true=(TH2D*)f_mc->Get("true");
    TH2D *h2fulleff=(TH2D*)f_mc->Get("truef");
    TH2D *h2purity=(TH2D*)f_mc->Get("hPurity");
    TH2D *h2purity_denom=(TH2D*)f_mc->Get("denom");


    h2purity->Divide(h2purity,h2purity_denom,1,1,"B");

    RooUnfoldResponse* response = (RooUnfoldResponse*)f_mc->Get("smeared_true");

    TH2D *effnum=(TH2D*)h2fulleff->Clone("effnum");
    TH2D *effdenom=(TH2D*)h2fulleff->Clone("effdenom");
  
    TH2D* hfold=(TH2D*)h2raw->Clone("hfold");
    hfold->Sumw2();

    std::stringstream ss;
    ss << "Unfold_nom_" << flag<< ".root";
    TFile* fout = new TFile(ss.str().c_str(), "recreate");
    ss.str("");

    h2smeared->Write();
    h2true->Write();
    h2fulleff->Write();
    h2purity->Write();

    TH2D *h2input;
    if(flag==0) h2input=(TH2D*)h2raw->Clone("h2input");
    if(flag==1) h2input=(TH2D*)h2purity_denom->Clone("h2input");
    if(flag==2) h2input=(TH2D*)h2raw->Clone("h2input");

    h2input->Multiply(h2purity);
    h2input->Write();

    h2raw->Multiply(h2purity);
    h2raw->SetName("raw");
    h2raw->Write();

    TDecompSVD *svd= new TDecompSVD (response->Mresponse());
    auto singular_values = svd->GetSig();
    svd->Print();

    //h2input->Write(); 

    cout<<"==============Unfold h1====================="<<endl;      

    RooUnfoldInvert unfold(response, h2input);    // OR
    TH2D* hunf= (TH2D*) unfold.Hreco(errorTreatment);

     //FOLD BACK
    hfold= (TH2D*)response->ApplyToTruth(hunf,"");    

    hunf->SetName("MatrixInverision_Ufolded");                
           
    hfold->SetName("MatrixInverision_Folded");

    hunf->Write();
    hfold->Write();

    fout->Close();         
}

#ifndef __CINT__
int main () { jelenaUnfold(); return 0; }  // Main program when run stand-alone
#endif
