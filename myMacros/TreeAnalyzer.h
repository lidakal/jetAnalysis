class TreeAnalyzer {
    public:
        bool qcdMC = false;
        TFile *fin; 
        TTree *t;
        TTree *HiTree;
    
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

        // Constructors
        TreeAnalyzer(bool qcd = true, bool init = true);
        //TreeAnalyzer(std::string fpath, bool init = true);
    
        // Class methods
        void Init();
        void SetAnalysisLevel(bool parORref);
        void SetExcludeGSP(bool excl = true);
        void CreateLocalTree();
        void CreateParLocalTree();
    
    private:
        bool exclGSP = false;
        Int_t *njets;
        Float_t *jetpt;
        Float_t *jeteta;
        Float_t *jetFlav;
        Long64_t nentries;
};

// qcd = true for qcdMC, qcd = false for bJetMC
TreeAnalyzer::TreeAnalyzer(bool qcd = true, bool init = true) 
{
    std::string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    std::string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";

    if (qcd) {
        fin = new TFile(path_qcd.c_str());
        qcdMC = true;
    } else {
        fin = new TFile(path_bJet.c_str());
        qcdMC = false;
    }
    
    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    HiTree = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");
    
    if (init) {
        Init();
    }
}

/*
TreeAnalyzer::TreeAnalyzer(std::string fpath, bool init = true) 
{
    fin = new TFile(fpath.c_str());
    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    HiTree = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");
    
    if (inString("qcd", fpath)) {
        qcdMC = true;
    }
    
    if (init) {
        Init();
    }        
}
*/

void TreeAnalyzer::Init() 
{
    // Set branch addresses for t tree
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

    // Set branch addressses for HiTree
    HiTree->SetBranchAddress("run", &run);
    HiTree->SetBranchAddress("evt", &evt);
    HiTree->SetBranchAddress("lumi", &lumi);
    HiTree->SetBranchAddress("vx", &vx);
    HiTree->SetBranchAddress("vy", &vy);
    HiTree->SetBranchAddress("vz", &vz);
    HiTree->SetBranchAddress("pthat", &pthat);
    HiTree->SetBranchAddress("weight", &weight);
    
    nentries = t->GetEntries();
}

void TreeAnalyzer::CreateLocalTree()
{ 
    CreateParLocalTree();
    //CreateRefLocalTree();
}
    
void TreeAnalyzer::CreateParLocalTree()
{
    std::string fname = "localTrees.root";
    
    std::cout << "Creating partonTree in " << fname << endl;
    
    t->SetBranchStatus("*", 0);
    for (auto activeBranchName : {"npar", "parpt", "pareta", "parNb", "parNc",
                                  "psjt1Pt", "psjt1Eta", "psjt1Phi", 
                                  "psjt2Pt", "psjt2Eta", "psjt2Phi"}) {
            t->SetBranchStatus(activeBranchName, 1);
    }
    
    HiTree->SetBranchStatus("*", 0);
    HiTree->SetBranchStatus("weight", 1);
    
    TFile *fout = new TFile(fname.c_str(), "recreate");
    TTree *partonTree = (TTree *) t->CloneTree();
    partonTree->SetName("partonTree");
    partonTree->SetEntries(nentries);
    
    Float_t w;
    Float_t zg[30];
    Float_t rg[30];
    Float_t kt[30];
    Int_t parFlav[30];
    
    TBranch *bw = partonTree->Branch("weight", &w, "weight/F");
    TBranch *bzg = partonTree->Branch("zg", zg, "zg[npar]/F");
    TBranch *zrg = partonTree->Branch("rg", rg, "rg[npar]/F");
    TBranch *bkt = partonTree->Branch("kt", kt, "kt[npar]/F");
    TBranch *bparFlav = partonTree->Branch("parFlav", parFlav, "parFlav[npar]/F");
    
    for (Long64_t i = 0; i < nentries; i++) {
        if (i % 1000000 == 0) {
                std::cout << "Processing i = " << i << endl;
        }
        
        HiTree->GetEntry(i);
        partonTree->GetEntry(i);
        
        w = weight;
        
        for (int j = 0; j < npar; j++) {
            zg[j] = -1;
            rg[j] = -1;
            kt[j] = -10;
            
            if (psjt2Pt[j] > 0.) {                
                Float_t deta = psjt1Eta[j] - psjt2Eta[j];
                Float_t dphi = acos(cos(psjt1Phi[j] - psjt2Phi[j]));
                
                zg[j] = psjt2Pt[j] / (psjt1Pt[j] + psjt2Pt[j]);
                rg[j] = sqrt(deta * deta + dphi * dphi);
                kt[j] = psjt2Pt[j] * rg[j];
            }   
            if (parNb[j] > 0) {
                parFlav[j] = 5;
            } else if (parNc[j] > 0) {
                parFlav[j] = 4;
            } else {
                parFlav[j] = 0;
            }
            bw->Fill();
            bzg->Fill();
            brg->Fill();
            bkt->Fill();
            bparFlav->Fill();
        }
    }
    
    // Rename branches
    
    
    fout->Close();
}

void TreeAnalyzer::CreateRefLocalTree()
{
    std::string fname = "localTrees.root";
    
    std::cout << "Creating hadronTree in " << fname << endl;
    
    t->SetBranchStatus("*", 0);
    for (auto activeBranchName : {"nref", "refpt", "refeta", "jtHadFlav", "jtNbHad", "jtHadNc",
                                  "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", 
                                  "rsjt2Pt", "rsjt2Eta", "rsjt2Phi"}) {
            t->SetBranchStatus(activeBranchName, 1);
    }
    
    HiTree->SetBranchStatus("*", 0);
    HiTree->SetBranchStatus("weight", 1);
    
    TFile *fout = new TFile(fname.c_str(), "recreate");
    TTree *refTree = (TTree *) t->CloneTree();
    refTree->SetName("refTree");
    refTree->SetEntries(nentries);
    
    Float_t w;
    Float_t zg[30];
    Float_t rg[30];
    Float_t kt[30];
    Int_t refFlav[30];
    
    refTree->Branch("weight", &w);
    refTree->Branch("zg", zg);
    refTree->Branch("rg", rg);
    refTree->Branch("kt", kt);
    
    refTree->Branch("refFlav", refFlav);
    
    for (Long64_t i = 0; i < nentries; i++) {
        if (i % 1000000 == 0) { 
            std::cout << "Processing i = " << i << endl;
        }
        
        HiTree->GetEntry(i);
        refTree->GetEntry(i);
        
        w = weight;
        
        for (int j = 0; j < npar; j++) {
            zg[j] = -1;
            rg[j] = -1;
            kt[j] = -10;
            
            if (psjt2Pt[j] > 0.) {                
                Float_t deta = psjt1Eta[j] - psjt2Eta[j];
                Float_t dphi = acos(cos(psjt1Phi[j] - psjt2Phi[j]));
                
                zg[j] = psjt2Pt[j] / (psjt1Pt[j] + psjt2Pt[j]);
                rg[j] = sqrt(deta * deta + dphi * dphi);
                kt[j] = psjt2Pt[j] * rg[j];
            }   
        }
    }
    fout->Close();
}
            
            
    
    
    
    
        
        