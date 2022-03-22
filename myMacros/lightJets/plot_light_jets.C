#include <initializer_list>

void plot_light_jets(bool ktCut = true)
{
    // Read data from file
    string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    
    cout << "\nReading from " << path_qcd << endl;
    TFile *fin = new TFile(path_qcd.c_str());
    
    if (ktCut) { 
        cout << "Running with kt cut: 1 GeV << endl;
    } else { 
        cout << "Running without kt cut." << endl;
    }
    
    TTree *t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    TTree *HiTree = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");
    
    // Declaration of leaves' types

    // For JetAnalyzer tree
    Int_t           nref;
    Float_t         rawpt[30];
    Float_t         jtpt[30];
    Float_t         jteta[30];
    Float_t         jtphi[30];
    Float_t         jtHadFlav[30];
    Float_t         jtParFlav[30];
    Int_t           jtNbHad[30];
    Int_t           jtNcHad[30];
    Int_t           jtNbPar[30];
    Int_t           jtNcPar[30];
    Bool_t          jtHasGSPB[30];
    Bool_t          jtHasGSPC[30];
    Bool_t          jtIsHardest[30];
    Float_t         sjt1Pt[30];
    Float_t         sjt1Eta[30];
    Float_t         sjt1Phi[30];
    Float_t         sjt1HadFlav[30];
    Float_t         sjt1ParFlav[30];
    Float_t         sjt2Pt[30];
    Float_t         sjt2Eta[30];
    Float_t         sjt2Phi[30];
    Float_t         sjt2HadFlav[30];
    Float_t         sjt2ParFlav[30];
    Float_t         rsjt1Pt[30]; //ref
    Float_t         rsjt1Eta[30];
    Float_t         rsjt1Phi[30];
    Float_t         rsjt2Pt[30];
    Float_t         rsjt2Eta[30];
    Float_t         rsjt2Phi[30];
    Float_t         jtDiscCSVV2[30];
    Float_t         jtDiscDeepCSVB[30];
    Float_t         jtDiscDeepCSVBB[30];
    Float_t         jtDiscDeepCSVC[30];
    Float_t         jtDiscDeepFlavourB[30];
    Float_t         jtDiscDeepFlavourBB[30];
    Float_t         jtDiscDeepFlavourLEPB[30];
    Float_t         jtDiscDeepFlavourC[30];
    Float_t         jtDiscProb[30];
    Int_t           nsvtx[30];
    vector<vector<int> > svtxntrk;
    vector<vector<float> > svtxdls;
    vector<vector<float> > svtxdls2d;
    vector<vector<float> > svtxm;
    vector<vector<float> > svtxpt;
    vector<vector<float> > svtxmcorr;
    Float_t         refpt[30];
    Float_t         refeta[30];
    Float_t         refphi[30];
    Float_t         refdphijt[30];
    Float_t         refdrjt[30];
    Float_t         refparton_pt[30];
    Int_t           refparton_flavor[30];
    Bool_t          refIsHardest[30];
    Int_t           ngen;
    Int_t           genmatchindex[30];
    Float_t         genpt[30];
    Float_t         geneta[30];
    Float_t         genphi[30];
    Int_t           npar;
    Float_t         parpt[30];
    Float_t         pareta[30];
    Float_t         parphi[30];
    Int_t           parNb[30];
    Int_t           parNc[30];
    Bool_t          parHasGSPB[30];
    Bool_t          parHasGSPC[30];
    Bool_t          genIsHardest[30];
    Float_t         gsjt1Pt[30];
    Float_t         gsjt1Eta[30];
    Float_t         gsjt1Phi[30];
    Float_t         gsjt2Pt[30];
    Float_t         gsjt2Eta[30];
    Float_t         gsjt2Phi[30];
    Float_t         psjt1Pt[30];
    Float_t         psjt1Eta[30];
    Float_t         psjt1Phi[30];
    Float_t         psjt2Pt[30];
    Float_t         psjt2Eta[30];
    Float_t         psjt2Phi[30];

    // For EvtAnalyzer tree
    UInt_t          run;
    ULong64_t       evt;
    UInt_t          lumi;
    Float_t         vx;
    Float_t         vy;
    Float_t         vz;
    Float_t         pthat;
    Float_t         weight;    

    // Set branch addresses

    // For JetAnalyzer tree
    t->SetBranchAddress("nref", &nref);
    t->SetBranchAddress("rawpt", rawpt);
    t->SetBranchAddress("jtpt", jtpt);
    t->SetBranchAddress("jteta", jteta);
    t->SetBranchAddress("jtphi", jtphi);
    t->SetBranchAddress("jtHadFlav", jtHadFlav);
    t->SetBranchAddress("jtParFlav", jtParFlav);
    t->SetBranchAddress("jtNbHad", jtNbHad);
    t->SetBranchAddress("jtNcHad", jtNcHad);
    t->SetBranchAddress("jtNbPar", jtNbPar);
    t->SetBranchAddress("jtNcPar", jtNcPar);
    t->SetBranchAddress("jtHasGSPB", jtHasGSPB);
    t->SetBranchAddress("jtHasGSPC", jtHasGSPC);
    t->SetBranchAddress("jtIsHardest", jtIsHardest);
    t->SetBranchAddress("sjt1Pt", sjt1Pt);
    t->SetBranchAddress("sjt1Eta", sjt1Eta);
    t->SetBranchAddress("sjt1Phi", sjt1Phi);
    t->SetBranchAddress("sjt1HadFlav", sjt1HadFlav);
    t->SetBranchAddress("sjt1ParFlav", sjt1ParFlav);
    t->SetBranchAddress("sjt2Pt", sjt2Pt);
    t->SetBranchAddress("sjt2Eta", sjt2Eta);
    t->SetBranchAddress("sjt2Phi", sjt2Phi);
    t->SetBranchAddress("sjt2HadFlav", sjt2HadFlav);
    t->SetBranchAddress("sjt2ParFlav", sjt2ParFlav);
    t->SetBranchAddress("rsjt1Pt", rsjt1Pt);
    t->SetBranchAddress("rsjt1Eta", rsjt1Eta);
    t->SetBranchAddress("rsjt1Phi", rsjt1Phi);
    t->SetBranchAddress("rsjt2Pt", rsjt2Pt);
    t->SetBranchAddress("rsjt2Eta", rsjt2Eta);
    t->SetBranchAddress("rsjt2Phi", rsjt2Phi);
    t->SetBranchAddress("jtDiscCSVV2", jtDiscCSVV2);
    t->SetBranchAddress("jtDiscDeepCSVB", jtDiscDeepCSVB);
    t->SetBranchAddress("jtDiscDeepCSVBB", jtDiscDeepCSVBB);
    t->SetBranchAddress("jtDiscDeepCSVC", jtDiscDeepCSVC);
    t->SetBranchAddress("jtDiscDeepFlavourB", jtDiscDeepFlavourB);
    t->SetBranchAddress("jtDiscDeepFlavourBB", jtDiscDeepFlavourBB);
    t->SetBranchAddress("jtDiscDeepFlavourLEPB", jtDiscDeepFlavourLEPB);
    t->SetBranchAddress("jtDiscDeepFlavourC", jtDiscDeepFlavourC);
    t->SetBranchAddress("jtDiscProb", jtDiscProb);
    t->SetBranchAddress("nsvtx", nsvtx);
    /*
    t->SetBranchAddress("svtxntrk", &svtxntrk);
    t->SetBranchAddress("svtxdls", &svtxdls);
    t->SetBranchAddress("svtxdls2d", &svtxdls2d);
    t->SetBranchAddress("svtxm", &svtxm);
    t->SetBranchAddress("svtxpt", &svtxpt);
    t->SetBranchAddress("svtxmcorr", &svtxmcorr);
    */
    t->SetBranchAddress("refpt", refpt);
    t->SetBranchAddress("refeta", refeta);
    t->SetBranchAddress("refphi", refphi);
    t->SetBranchAddress("refdphijt", refdphijt);
    t->SetBranchAddress("refdrjt", refdrjt);
    t->SetBranchAddress("refparton_pt", refparton_pt);
    t->SetBranchAddress("refparton_flavor", refparton_flavor);
    t->SetBranchAddress("refIsHardest", refIsHardest);
    t->SetBranchAddress("ngen", &ngen);
    t->SetBranchAddress("genmatchindex", genmatchindex);
    t->SetBranchAddress("genpt", genpt);
    t->SetBranchAddress("geneta", geneta);
    t->SetBranchAddress("genphi", genphi);
    t->SetBranchAddress("npar", &npar);
    t->SetBranchAddress("parpt", parpt);
    t->SetBranchAddress("pareta", pareta);
    t->SetBranchAddress("parphi", parphi);
    t->SetBranchAddress("parNb", parNb);
    t->SetBranchAddress("parNc", parNc);
    t->SetBranchAddress("parHasGSPB", parHasGSPB);
    t->SetBranchAddress("parHasGSPC", parHasGSPC);
    t->SetBranchAddress("genIsHardest", genIsHardest);
    t->SetBranchAddress("gsjt1Pt", gsjt1Pt);
    t->SetBranchAddress("gsjt1Eta", gsjt1Eta);
    t->SetBranchAddress("gsjt1Phi", gsjt1Phi);
    t->SetBranchAddress("gsjt2Pt", gsjt2Pt);
    t->SetBranchAddress("gsjt2Eta", gsjt2Eta);
    t->SetBranchAddress("gsjt2Phi", gsjt2Phi);
    t->SetBranchAddress("psjt1Pt", psjt1Pt);
    t->SetBranchAddress("psjt1Eta", psjt1Eta);
    t->SetBranchAddress("psjt1Phi", psjt1Phi);
    t->SetBranchAddress("psjt2Pt", psjt2Pt);
    t->SetBranchAddress("psjt2Eta", psjt2Eta);
    t->SetBranchAddress("psjt2Phi", psjt2Phi);

    // For EvtAnalyzer tree
    HiTree->SetBranchAddress("run", &run);
    HiTree->SetBranchAddress("evt", &evt);
    HiTree->SetBranchAddress("lumi", &lumi);
    HiTree->SetBranchAddress("vx", &vx);
    HiTree->SetBranchAddress("vy", &vy);
    HiTree->SetBranchAddress("vz", &vz);
    HiTree->SetBranchAddress("pthat", &pthat);
    HiTree->SetBranchAddress("weight", &weight);

    // Activate specific branches
    t->SetBranchStatus("*", 0);
    for (auto activeBranchName : {"npar", "parpt", "pareta", "parNb", "parNc",
                                  "psjt1Pt", "psjt1Eta", "psjt1Phi", 
                                  "psjt2Pt", "psjt2Eta", "psjt2Phi",
                                  "nref", "refpt", "refeta", "jtHadFlav", "refIsHardest",
                                  "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", 
                                  "rsjt2Pt", "rsjt2Eta", "rsjt2Phi"}) {
        //std::cout << "Activating branch " << activeBranchName << endl;
        t->SetBranchStatus(activeBranchName, 1);
    }
    HiTree->SetBranchStatus("*", 0);
    HiTree->SetBranchStatus("weight", 1);
    
    // Create the new histograms
    
    // zg
    Int_t x1bins = 40;
    Float_t x1min = 0.1;
    Float_t x1max = 0.5;
    
    // rg
    Int_t x2bins = 40;
    Float_t x2min = 0.91;
    Float_t x2max = 5.;
    
    // pt 
    Int_t y1bins = 27 * 2;
    Float_t y1min = 30.;
    Float_t y1max = 300.;
    
    TH2F *hL_zg_par = new TH2F("hL_zg_par", "zg, parpt, light jets", x1bins, x1min, x1max, y1bins, y1min, y1max);
    TH2F *hL_zg_ref = new TH2F("hL_zg_ref", "zg, refpt, light jets", x1bins, x1min, x1max, y1bins, y1min, y1max);
    TH2F *hL_zg_ref_dynKt = new TH2F("hL_zg_ref_dynKt", "zg, refpt, dynKt only, light jets", x1bins, x1min, x1max, y1bins, y1min, y1max);
    
    TH2F *hL_rg_par = new TH2F("hL_rg_par", "logrg, parpt, light jets", x2bins, x2min, x2max, y1bins, y1min, y1max);
    TH2F *hL_rg_ref = new TH2F("hL_rg_ref", "logrg, refpt, light jets", x2bins, x2min, x2max, y1bins, y1min, y1max);
    TH2F *hL_rg_ref_dynKt = new TH2F("hL_rg_ref_dynKt", "logrg, refpt, dynKt only, light jets", x2bins, x2min, x2max, y1bins, y1min, y1max);
    
    Long64_t nentries = t->GetEntries();
    
    for (Long64_t i = 0; i < nentries; i++) {
        if ((i % 1000000) == 0) { 
            cout << "i = " << i << endl;
        }        
        
        t->GetEntry(i);
        HiTree->GetEntry(i);
        
        // par jets

        for (int j = 0; j < npar; j++) {
            // cut on eta = 2
            if (pareta[j] > 2.) { 
                continue;
            }
            
            // Initialize zg, rg, kt, ln(1/rg), ln(kt)
            Float_t zg = -1.;
            Float_t rg = -1.;
            Float_t kt = -1.;
            
            Float_t logrg = -1;
            Float_t logkt = -10;
            
            if (psjt2Pt[j] > 0.) {
                // Calculate zg, rg, kt
                zg = psjt2Pt[j] / (psjt2Pt[j] + psjt1Pt[j]);

                Float_t deta = psjt1Eta[j] - psjt2Eta[j];
                Float_t dphi = acos(cos(psjt1Phi[j] - psjt2Phi[j]));
                rg = sqrt(deta * deta + dphi * dphi);

                kt = psjt2Pt[j] * rg;

                // Calculate logs
                logrg = log(1/rg);
                logkt = log(kt);
            }
        

            // Cut on kt = 1 GeV 
            if (ktCut) { 
                if (logkt < 0.) { 
                    zg = -1.;
                    logrg = -1.;
                }
            }

            // Fill histograms : if not b and not c
            if ((!(parNb[j] > 0)) && (!(parNc[j] > 0))) {
                hL_zg_par->Fill(zg, parpt[j], weight);
                hL_rg_par->Fill(logrg, parpt[j], weight);
            }
        }

        // ref jets

        for (int j = 0; j < nref; j++) {
            // cut on eta = 2
            if (refeta[j] > 2.) { 
                continue;
            }
            
            // Initialize zg, rg, kt, ln(1/rg), ln(1/zg), ln(kt)
            Float_t zg = -1.;
            Float_t rg = -1.;
            Float_t kt = -1.;
            
            Float_t logrg = -1.;
            Float_t logkt = -10.;
            
            if (rsjt2Pt[j] > 0.) {
                // Calculate zg, rg, kt
                zg = rsjt2Pt[j] / (rsjt2Pt[j] + rsjt1Pt[j]);

                Float_t deta = rsjt1Eta[j] - rsjt2Eta[j];
                Float_t dphi = acos(cos(rsjt1Phi[j] - rsjt2Phi[j]));
                rg = sqrt(deta * deta + dphi * dphi);

                kt = rsjt2Pt[j] * rg;

                // Calculate logs
                logrg = log(1/rg);
                logkt = log(kt);
            }

            // Cut on kt = 0.1
            if (ktCut) { 
                if (logkt < 0.1) { 
                    logrg = -1.;
                    zg = -1.;
                }
            }
            
            // Fill histograms : jtHadFlav == 0 -> light jets
            if (jtHadFlav[j] == 0) {
                hL_zg_ref->Fill(zg, refpt[j], weight);
                hL_rg_ref->Fill(logrg, refpt[j], weight);
            }

            // Dynamical grooming
            if (!refIsHardest[j]) {
                logrg = -1.;
                zg = -1.;
            }

            if (jtHadFlav[j] == 0) {
                hL_zg_ref_dynKt->Fill(zg, refpt[j], weight);
                hL_rg_ref_dynKt->Fill(logrg, refpt[j], weight);
            }
        } 
    }

    if (ktCut)  {
        string path_out = "~/rootFiles/lightJetHistos_ktCut.root";
        cout << "Saving histograms in " << path_out << endl;
        TFile *fout = new TFile(path_out.c_str(), "recreate");

        hL_zg_par->Write();
        hL_zg_ref->Write();
        hL_zg_ref_dynKt->Write();

        hL_rg_par->Write();
        hL_rg_ref->Write();
        hL_rg_ref_dynKt->Write();
        
        fout->Close();
    } else {
        string path_out = "~/rootFiles/lightJetHistos.root";
        cout << "Saving histograms in " << path_out << endl;
        TFile *fout = new TFile(path_out.c_str(), "recreate");

        hL_zg_par->Write();
        hL_zg_ref->Write();
        hL_zg_ref_dynKt->Write();

        hL_rg_par->Write();
        hL_rg_ref->Write();
        hL_rg_ref_dynKt->Write();
        
        fout->Close();
    }
}


            
            
            
            
            