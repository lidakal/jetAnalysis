// TreeAnalyzer class for b-jets
// Looking to include l, c-jets next

class TreeAnalyzer {
    public:
        TFile *fin; 
        TTree *t;
    
        // 
            Float_tweight;   

        // Constructors
        TreeAnalyzer(bool qcd = true, bool init = true);
        //TreeAnalyzer(std::string fpath, bool init = true);
    
        // Class methods
        void Init();
        void SetAnalysisLevel(bool parORref);
        /*
        void SetExcludeGSP(bool excl = true);
        void CreateLocalTrees();
        void CreateParLocalTree(TFile *ftrees);
        void CreateRefLocalTree(TFile *ftrees);
        */
        void TreeAnalyzer::GetEntry(Long64_t i);
    
        //bool exclGSP = false;
    
        // Fat jet properties
        Int_t *njet;
        Float_t *jetpt;
        Float_t *jeteta;
        Float_t *jetNb;
        Float_t *jetNc;
        
        // Subjet properties
        Float_t *subjet1pt;
        Float_t *subjet1eta;
        Float_t *subjet1phi;
    
        Float_t *subjet2pt;
        Float_t *subjet2eta;
        Float_t *subjet2phi;
    
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

void TreeAnalyzer::GetEntry(Long64_t i) 
{
    t->GetEntry(i);
    HiTree->GetEntry(i);
}

void TreeAnalyzer::SetAnalysisLevel(bool parORref)
{
    if (parORref) {
        // fat jet properties
        njet = &npar;
        jetpt = parpt;
        jeteta = pareta;
        jetNb = parNb;
        jetNc = parNc;
        
        // Subjet properties
        subjet1pt = psjt1Pt;
        subjet1eta = psjt1Eta;
        subjet1phi = psjt1Phi;
        
        subjet2pt = psjt2Pt;
        subjet2eta = psjt2Eta;
        subjet2phi = psjt2Phi;
    } else {
        // fat jet properties
        njet = &ref;
        jetpt = refpt;
        jeteta = refeta;
        jetNb = jtNbHad;
        jetNc = jtNcHad;
        
        // Subjet properties
        subjet1pt = rsjt1Pt;
        subjet1eta = rsjt1Eta;
        subjet1phi = rsjt1Phi;
        
        subjet2pt = rsjt2Pt;
        subjet2eta = rsjt2Eta;
        subjet2phi = rsjt2Phi;
    }
}
        

/*
void TreeAnalyzer::CreateLocalTrees()
{ 
    std::string fname = "~/rootFiles/localTrees.root";
    TFile *ftrees = new TFile(fname.c_str(), "recreate");
    
    std::cout << "Creating partonTree in " << fname << endl;
    CreateParLocalTree(ftrees);
    
    //std::cout << "Creating refTree in " << fname << endl;
    //CreateRefLocalTree();
    
    ftrees->Close();
}
    
void TreeAnalyzer::CreateParLocalTree(TFile *ftrees)
{       
    t->SetBranchStatus("*", 0);
    for (auto activeBranchName : {"npar", "parpt", "pareta", "parNb", "parNc",
                                  "psjt1Pt", "psjt1Eta", "psjt1Phi", 
                                  "psjt2Pt", "psjt2Eta", "psjt2Phi"}) {
            t->SetBranchStatus(activeBranchName, 1);
    }
    
    HiTree->SetBranchStatus("*", 0);
    HiTree->SetBranchStatus("weight", 1);
    
    //ftrees->ReOpen();
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
    TBranch *brg = partonTree->Branch("rg", rg, "rg[npar]/F");
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
    
    partonTree->Write();
    // Rename branches
    
}

void TreeAnalyzer::CreateRefLocalTree(TFile *ftrees)
{
    std::string fname = "~/rootFiles/localTrees.root";
    
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
*/
            
            
    
    
    
    
        
        