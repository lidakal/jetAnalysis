#include "../tTree.h"
#include "../hist_utils.h"

void fill_response_2d_square_fromJelena()
{
    // Notes: In order to unfold using the underflow/overflow bins, 
    //        need to manually make them into actual histogram bins 

    // ---- Load MC

    TString label = "aggrTMVA_withNb";
    TString sample = "bjet";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/" + sample + "/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    TString rootf_out = "./histos/qcd_" + sample + "_" + label + "_unfold_2d_square_fromJelena.root";

    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref", "nvtx",
                                             "refeta", "refpt", "jtHadFlav", "jtNbHad",
                                             "jtpt",
                                             "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", "rsjt1Y",
                                             "rsjt2Pt", "rsjt2Eta", "rsjt2Phi", "rsjt2Y",
                                             "jtpt", "jteta", "jtphi", 
                                             "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt1Y",
                                             "sjt2Pt", "sjt2Eta", "sjt2Phi", "sjt2Y",
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "jtmB", "refmB",
                                             "weight"
                                             };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    // ---- Setup 

    double kt_cut = 1.;
    double btag_cut = 0.7;
    double jetR = 0.4;

    // ---- Create the histograms 

    // jet pt bins : no underflow or overflow
    const Int_t z1bins = 7;
    Double_t z1min = 60.;
    Double_t z1max = 200.; 

    // ln(0.4/rg) bins : 1st bin untagged (unphysical)
    const Int_t x1bins = 6;
    Double_t x1min = -0.4; // if untagged or kT < cut : ln(R/Rg) = -0.2
    Double_t x1max = 2.; 

    Double_t logrg_underflow = -0.2;

    TH2D *h_data_reco_rgpt = new TH2D("h_data_reco_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_data_true_rgpt = new TH2D("h_data_true_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_data_fake_rgpt = new TH2D("h_data_fake_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_mc_true_rgpt = new TH2D("h_mc_true_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_mc_reco_rgpt = new TH2D("h_mc_reco_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_mc_fake_rgpt = new TH2D("h_mc_fake_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    RooUnfoldResponse response_rgpt(h_mc_reco_rgpt, h_mc_true_rgpt, "response_rgpt", "response for 2d: rg and jet pt"); 

    TH2D *h_mc_purity_rgpt = new TH2D("h_mc_purity_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_mc_efficiency_rgpt = new TH2D("h_mc_efficiency_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    // Notes: 
    // data_reco = pseudo data reco with any gen (including fakes)
    // data_true = pseudo data true with any reco
    // mc_reco = MC reco with any gen (but gen exists)
    // mc_true = MC true with any reco
    // purity = h_mc_purity_rgpt / h_mc_reco_rgpt
    // efficiency = h_mc_efficiency_rgpt / h_mc_true_rgpt

    const int bin_det_pt=4;
    const int bin_true_pt=4;
    const int bin_det_rg=7;
    const int bin_true_rg=7;

    Double_t ptmin_det=80;
    Double_t ptmin_true=80;

    Double_t ptmax_det=140;
    Double_t ptmax_true=140;

    Double_t rgmax_det=2.;
    Double_t rgmax_true=2.;    
  
    Double_t rgmin_det=-0.4;
    Double_t rgmin_true=-0.4;

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
    xbins[0]=-0.4;
    xbins[1]=0.;
    xbins[2]=0.4;
    xbins[3]=0.8;
    xbins[4]=1.2;
    xbins[5]=1.6;
    xbins[6]=2.;

    //true binning in Rg
    Double_t xbins_true[bin_true_rg];
    xbins_true[0]=-0.4;
    xbins_true[1]=0.;
    xbins_true[2]=0.4;
    xbins_true[3]=0.8;
    xbins_true[4]=1.2;
    xbins_true[5]=1.6;
    xbins_true[6]=2.;
                    
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


    // ---- Fill histos
    Long64_t total_events = 13000000;
    Long64_t nentries = t.GetEntries();
    if (total_events > nentries) total_events = nentries;

    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 1000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        if (ient > total_events) break;

        t.GetEntry(ient);

        for (Int_t ijet = 0; ijet < t.nref; ijet++) {
            // event selection 
            if (std::abs(t.jteta[ijet]) > 2) continue;
            bool passWP = ((t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_lepb[ijet]) > btag_cut);
            if (!passWP) continue;

            Float_t jtpt = t.jtpt[ijet];
            Float_t refpt = t.refpt[ijet];
            Float_t weight = t.weight;

            // Initialize observables -- values for untagged jets
            Float_t rg = -1.;
            Float_t kt = -1.;

            Float_t logrg = logrg_underflow;
            Float_t logkt = -100.;

            Float_t rg_gen = -1.;
            Float_t kt_gen = -1.;

            Float_t logrg_gen = logrg_underflow;
            Float_t logkt_gen = -100.;

            // Calculate rg, kt only for 2 prong jets
            if (t.sjt2Pt[ijet] > 0.) {
                rg = calc_rg(t.sjt1Y[ijet], t.sjt1Phi[ijet], t.sjt2Y[ijet], t.sjt2Phi[ijet]);
                kt = t.sjt2Pt[ijet] * rg;
                
                // calculate logs
                logrg = std::log(jetR/rg);
                logkt = std::log(kt);
            }

            if (t.rsjt2Pt[ijet] > 0.) {
                rg_gen = calc_rg(t.rsjt1Y[ijet], t.rsjt1Phi[ijet], t.rsjt2Y[ijet], t.rsjt2Phi[ijet]);
                kt_gen = t.rsjt2Pt[ijet] * rg_gen;
                
                // calculate logs
                logrg_gen = std::log(jetR/rg_gen);
                logkt_gen = std::log(kt_gen);
            }

            // kt cut 
            if (kt < kt_cut) logrg = logrg_underflow;
            if (kt_gen < kt_cut) logrg_gen = logrg_underflow;

            // Fill histograms
            if (ient < total_events / 2) {
                // pseudo data
                h_data_reco_rgpt->Fill(logrg, jtpt, weight);
                if (refpt > 0)
                    h_data_true_rgpt->Fill(logrg_gen, refpt, weight);
                else
                    h_data_fake_rgpt->Fill(logrg, jtpt, weight);
            } else {
                // MC 
                bool has_gen_match = (refpt > 0);
                if (has_gen_match) {
                    // Signal = single b
                    if (!(t.jtNbHad[ijet] == 1)) continue;
                    // response_rgpt.Fill(logrg, jtpt, logrg_gen, refpt, weight);
                    // if (refpt > z1min && refpt < z1max && logrg_gen > x1min && logrg_gen < x1max) {
                    //     h_mc_reco_rgpt->Fill(logrg, jtpt, weight);
                    //     response_rgpt.Fill(logrg, jtpt, logrg_gen, refpt, weight);
                    // }
                    // if (jtpt > z1min && jtpt < z1max && logrg > x1min && logrg < x1max) {
                    //     h_mc_true_rgpt->Fill(logrg_gen, refpt, weight);
                    // }

                    // // Fill purity
                    // if (refpt > z1min && refpt < z1max && logrg_gen > x1min && logrg_gen < x1max)
                    //     h_mc_purity_rgpt->Fill(logrg, jtpt, weight);

                    // // Fill efficiency
                    // if (jtpt > z1min && jtpt < z1max && logrg > x1min && logrg < x1max)
                    //     h_mc_efficiency_rgpt->Fill(logrg_gen, refpt, weight);

                    double pt_det = jtpt;
                    double pt_true = refpt;
                    double rg_det = logrg;
                    double rg_true = logrg_gen;
                    double scale = weight;

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
                } else {
                    // Fakes = no gen match
                    h_mc_fake_rgpt->Fill(logrg, jtpt, weight); 
                }                
            }

            // Note: I do not have gen with no reco match in my ntuples
            //       but for jet pt > 60 GeV, matching efficiency ~ 1

        } // end jet loop       
    } // end entries loop

    // ---- Save histos 
    TH2D *h_mc_purity_denominator = (TH2D *) h_mc_reco_rgpt->Clone("h_mc_purity_denominator");
    // h_mc_purity_denominator->Add(h_mc_fake_rgpt);
    h_mc_purity_rgpt->Divide(h_mc_purity_denominator);
    h_mc_efficiency_rgpt->Divide(h_mc_true_rgpt);

    TFile *fout = new TFile(rootf_out, "recreate");
    for (auto h : {h_data_reco_rgpt, h_data_true_rgpt, h_data_fake_rgpt,
                   h_mc_true_rgpt, h_mc_reco_rgpt, h_mc_fake_rgpt,
                   h_mc_purity_rgpt, h_mc_efficiency_rgpt,
                   denom, hPurity, h2fulleff, h2true, h2smeared
                   }) {
                    h->Write();
                   }
    response_rgpt.Write();
    response.Write();

    fout->Close();
    delete fout;
}