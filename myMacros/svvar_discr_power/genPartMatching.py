from ROOT import TFile, TTree
from array import array
from numpy import cos, sqrt
from numpy import arccos as acos

def genPartMatching(finname = "/data_CMS/cms/kalipoliti/aggregatedB_truthInfo/merged_HiForestAOD.root", ptCut = 1.):  
    # Read data
    fin = TFile(finname)
    t = fin.Get("ak4PFJetAnalyzer/t")
    hi = fin.Get("bDecayAna/hi")
    
    t.SetBranchStatus("*", 0)
    for branchName in ["nref", "jtHadFlav", "jteta", 
                       "nselIPtrk", "nIP", "ipEta", "ipPhi",
                       "nsvtx", "svtxntrk", "svtxdls", "svtxdls2d", "svtxm", "svtxpt", "svtxmcorr",
                       "svtxTrPt", "svtxTrEta", "svtxTrPhi"]: 
        t.SetBranchStatus(branchName, 1)
    
    # Create new file for match matches
    foutname = "matchedInfo.root"
    print("Creating file ", foutname)
    fout = TFile(foutname, "recreate")
    matches = TTree("matches", "tree with matches")
    
    # Add the new branches to the matches
    nIPmax = 100
    nIPcopy = array("i", [0])
    matches.Branch("nIPcopy", nIPcopy, "nIPcopy/I")
    
    genPartMatchID = array("i", [0] * nIPmax)   
    matches.Branch("genPartMatchID", genPartMatchID, "genPartMatchID[nIPcopy]/I")

    # Loop over t entries
    nentries = t.GetEntries() 
    for ient in range(nentries):
        ient = int(ient)
        
        #if (ient > 10): break
            
        # Print progress
        if (ient % 100000 == 0): 
            print("Entry: ", ient)

        t.GetEntry(ient)
        hi.GetEntry(ient)

        # All tracks per event
        nIP = t.nIP
        nIPcopy[0] = nIP
        
        for itrack in range(nIP):
            itrack = int(itrack)

            dRmin = 100
            dR = dRmin
            partMatchID = -1

            trkEta = t.ipEta[itrack]
            trkPhi = t.ipPhi[itrack]

            mult = hi.mult

            for imult in range(mult):
                imult = int(imult)

                partPt = hi.pt[imult]
                partEta = hi.eta[imult]
                partPhi = hi.phi[imult]

                if (partPt < ptCut): 
                    continue

                dEta = trkEta - partEta
                dPhi = acos(cos(trkPhi - partPhi))
                dR = sqrt((dEta * dEta) + (dPhi * dPhi))

                if (dR < dRmin):
                    dRmin = dR
                    partMatchID = imult
            genPartMatchID[itrack] = partMatchID
        matches.Fill()
    print("Writing in file ", foutname)
    matches.Write("matches")
    fout.Close()

if __name__ == "__main__": 
    finname = "/data_CMS/cms/kalipoliti/aggregatedB_truthInfo/merged_HiForestAOD.root"
    ptCut = 1.

    print("Making matches for root file:")
    print(finname)

    print("And ptCut:")
    print(ptCut)

    genPartMatching(finname, ptCut)
