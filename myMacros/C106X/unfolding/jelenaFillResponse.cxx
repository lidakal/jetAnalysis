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
#include <random>
#include <TRandom.h>
#include <TRandom3.h>


#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
//#include "RooUnfoldTestHarness2D.h"
#endif

#define MAX_GEN      6000
#define MAX_XB       20000
static const int MAXJETS = 1000;
using namespace std;

//==============================================================================
// Global definitions
//==============================================================================

const Double_t cutdummy= -99999.0;

//==============================================================================
// Gaussian smearing, systematic translation, and variable inefficiency
//==============================================================================


//==============================================================================
// Example Unfolding
//==============================================================================
Double_t RelativePhi(Double_t mphi,Double_t vphi) {

    if (vphi < -1 * TMath::Pi()) vphi += (2 * TMath::Pi());
    else if (vphi > TMath::Pi()) vphi -= (2 * TMath::Pi());
    if (mphi < -1 * TMath::Pi()) mphi += (2 * TMath::Pi());
    else if (mphi > TMath::Pi()) mphi -= (2 * TMath::Pi());
    double dphi = mphi - vphi;
    if (dphi < -1 * TMath::Pi()) dphi += (2 * TMath::Pi());
    else if (dphi > TMath::Pi()) dphi -= (2 * TMath::Pi());
    return dphi; // dphi in [-Pi, Pi]                                                                                                                                                         
}

                                              
TH2D* CorrelationHist(const TMatrixD& cov, const char* name, const char* title, Int_t na, Int_t nb, Int_t kbin)
{

    TH2D* h=new TH2D (name, title,na,0,na,na,0,na);
    for(int l=0;l<na;l++){
        for(int n=0;n<na;n++){
            int index1=l+na*kbin;
            int index2=n+na*kbin;
            double vv=cov(index1,index1)*cov(index2,index2);
            if(vv>0.0) h->SetBinContent(l+1,n+1,cov(index1,index2)/sqrt(vv));
        }
    }
    return h;
}

double phi0to2pi(double phi){
    double pi = 3.141592653589793238;
    while (phi >= 2. * pi) phi -= 2. * pi;
    while (phi < 0.) phi += 2. * pi;
    return phi;
}


double deltaPhi(double Phi1, double Phi2)
{
    // build the delta Phi angle between the two vectors
    double pi = 3.141592653589793238;
    double phi1 = phi0to2pi(Phi1);
    double phi2 = phi0to2pi(Phi2);
    double dPhi = phi0to2pi(phi1 - phi2);
    dPhi = (dPhi > (2 * pi - dPhi)) ? 2 * pi - dPhi : dPhi;
    return dPhi;
}


double deltaR(double Phi1, double Eta1, double Phi2, double Eta2)
{
    double dEta = Eta1 - Eta2;
    double dPhi = deltaPhi(Phi1, Phi2);
    return sqrt(dEta * dEta + dPhi * dPhi);
}


void RooUnfoldReadResponsepp_inclusive_matrixInv(std::string filename="/eos/cms/store/user/jmijusko/DfinderRun2/MC/pp/QCD_pThat-15_Dijet_TuneCP5_5p02TeV_pythia8/crab_Dfinder_pp_QCD_Dijet_MC_cuts_DCA_noFilter_ch/230509_081758/Merged_full.root")
{

    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;        

    const int bin_det_pt=4;
    const int bin_true_pt=4;
    const int bin_det_rg=8;
    const int bin_true_rg=8;

    Double_t ptmin_det=80;
    Double_t ptmin_true=80;

    Double_t ptmax_det=140;
    Double_t ptmax_true=140;

    Double_t rgmax_det=3.5;
    Double_t rgmax_true=3.5;    
  
    Double_t rgmin_det=1;
    Double_t rgmin_true=1;


    //det binning in pt
    Double_t ybins[bin_det_pt];      
    ybins[0]=ptmin_det;
    ybins[1]=100;
    ybins[2]=120;
    ybins[3]=140;

    //true binning in pt
    Double_t ybins_true[bin_true_pt];
    ybins_true[0]=ptmin_true;
    ybins_true[1]=100;
    ybins_true[2]=120;
    ybins_true[3]=140;
 
    //det binning in Rg
    Double_t xbins[bin_det_rg];
    xbins[0]=1;
    xbins[1]=1.6;
    xbins[2]=2.;
    xbins[3]=2.3;
    xbins[4]=2.55;
    xbins[5]=2.8;
    xbins[6]=3.0;
    xbins[7]=3.5;

    //true binning in Rg
    Double_t xbins_true[bin_true_rg];
    xbins_true[0]=1;
    xbins_true[1]=1.6;
    xbins_true[2]=2.;
    xbins_true[3]=2.3;
    xbins_true[4]=2.55;
    xbins_true[5]=2.8;
    xbins_true[6]=3.0;
    xbins_true[7]=3.5;
                    
    //MC detector correlation
    TH2D *h2smeared=new TH2D("smeared","smeared",bin_det_rg-1,xbins,bin_det_pt-1,ybins);
    //true correlations with det-level cuts
    TH2D *h2true=new TH2D("true","true",bin_true_rg-1,xbins_true,bin_true_pt-1,ybins_true);
     
    //fully-efficient true correlation (no det-level cuts)
    TH2D *h2fulleff=new TH2D("truef","truef",bin_true_rg-1,xbins_true,bin_true_pt-1,ybins_true);

    //setup the binnings of the 4D response
    RooUnfoldResponse response;
    response.Setup(h2smeared,h2true); 

    //Purity
    TH2D *hPurity=new TH2D("hPurity","hPurity",bin_det_rg-1,xbins,bin_det_pt-1,ybins);
    TH2D *denom=new TH2D("denom","denom",bin_det_rg-1,xbins,bin_det_pt-1,ybins);


    TFile *input=TFile::Open(filename.data());
    cout<<"open the file"<<endl;
    TTree *Jettree = (TTree*)input->Get("ak2PFJetAnalyzer/t"); //pp AOD                            
    TTree *Eventtree = (TTree*)input->Get("hiEvtAnalyzer/HiTree");
    TTree *HLTtree = (TTree*)input->Get("hltanalysis/HltTree"); //pp AOD


    cout<<"the trees"<<endl;
    Jettree->AddFriend(Eventtree);
    Jettree->AddFriend(HLTtree);
    Long64_t nentries = Jettree->GetEntries();

    //event
    int     RunNo;
    int     EvtNo;
    int     LumiNo;
    int     Dsize;
    float   PVz;
    float   weight;

    //jets 
    int     nref= 0;
    float   jtpt[MAX_XB];
    float   jteta[MAX_XB];
    float   jtphi[MAX_XB];
    float   jtsym[MAXJETS];
    float   jtrg[MAXJETS];
    float   jtdynkt[MAXJETS];
    float   jtangu[MAXJETS];
    float   refrg[MAXJETS];
    float   refpt[MAX_XB];
    float   refeta[MAX_XB];
    float   refphi[MAX_XB];
    float   refdynkt[MAX_XB];
    int     refparton_flavor[MAX_XB];
    int     HLT_HIAK4CaloJet60_v1;


    Jettree->SetBranchAddress("jtpt", &jtpt);
    Jettree->SetBranchAddress("jteta", &jteta);
    Jettree->SetBranchAddress("jtphi", &jtphi);
    Jettree->SetBranchAddress("nref", &nref);
    Jettree->SetBranchAddress("jtsym",&jtsym);
    Jettree->SetBranchAddress("jtrg",&jtrg);
    Jettree->SetBranchAddress("jtdynkt",&jtdynkt);
    Jettree->SetBranchAddress("jtangu",&jtangu);
 
    Jettree->SetBranchAddress("refpt", &refpt);
    Jettree->SetBranchAddress("refeta", &refeta);
    Jettree->SetBranchAddress("refphi", &refphi);
    Jettree->SetBranchAddress("refrg",&refrg);
    Jettree->SetBranchAddress("refdynkt",&refdynkt);
    Jettree->SetBranchAddress("weight",&weight);
    Jettree->SetBranchAddress("refparton_flavor",&refparton_flavor);

    Jettree->SetBranchAddress("HLT_HIAK4CaloJet60_v1",&HLT_HIAK4CaloJet60_v1);


    TFile *jetResSF = TFile::Open("jetResolutionSF.root");
    TH1F  *jetSF = (TH1F*)jetResSF->Get("hSF");


    TFile *file = TFile::Open("PurityHist.root");

    TRandom3 *random = new TRandom3(0);

    cout<<"number of entries "<<nentries<<endl;
    for (int i=1; i<20000000; i++){

        if (i%100000==0) std::cout << i <<std::endl;

        Jettree->GetEntry(i);

        double scale=weight;


        for (int indjet=0; indjet<nref; indjet++){


            double smearFactor=1;
            double pt_det=jtpt[indjet]*smearFactor;
            double pt_true=refpt[indjet];
            double rg_det=0;

            if(jtrg[indjet]!=0) rg_det= TMath::Log(1./jtrg[indjet]);
            else rg_det=1;
            double rg_true=0;
            if(refrg[indjet]!=0) rg_true= TMath::Log(1./refrg[indjet]);
            else rg_true=1;


            if(pt_det<=ptmax_det && pt_det>=ptmin_det && rg_det<=rgmax_det && rg_det>=rgmin_det){
                denom->Fill(rg_det, pt_det, scale);
                if (pt_true<=ptmax_true && pt_true>=ptmin_true && rg_true<=rgmax_true && rg_true>=rgmin_true){
                    hPurity->Fill(rg_det, pt_det, scale);
                }
            }

            if(pt_true>ptmax_true || pt_true<ptmin_true) continue;
            if(rg_true>rgmax_true || rg_true<rgmin_true) continue;

            h2fulleff->Fill(rg_true,pt_true,scale);

            if(pt_det>ptmax_det || pt_det<ptmin_det) continue;
            if(rg_det>rgmax_det || rg_det<rgmin_det) continue;

            h2true->Fill(rg_true,pt_true,scale);

            response.Fill(rg_det,pt_det,rg_true,pt_true, scale);
            h2smeared->Fill(rg_det,pt_det,scale);


        }//jets
     
    }//events

    TFile *fout=new TFile ("Output_Response_inclusive_matrixInv_3p5_PurHist_ch_nocut_80.root","RECREATE");
    fout->cd();
    h2smeared->SetName("smeared");
    h2smeared->Write();
    h2fulleff->Write();
    h2true->Write();
    hjetpt->Write();
    hjeteta->Write();
    hjetphi->Write();
    hjetdynkt->Write();
    hgenjetpt->Write();
    hrg_true->Write();
    hrg_det->Write();
    response_100to120->Write();
    hresidual_100to120->Write();
    hresolution_jet->Write();
    denom->Write();
    hPurity->Write();

    response.Write();

}

#ifndef __CINT__
int main () { RooUnfoldReadResponsepp_inclusive_matrixInv(); return 0; }  // Main program when run stand-alone
#endif
