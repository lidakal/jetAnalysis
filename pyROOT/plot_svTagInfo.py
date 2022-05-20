from ROOT import TFile, TTree, TH1D, TCanvas
import numpy as np

fin = TFile("/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root")
t = fin.Get("ak4PFJetAnalyzer/t")
t.AddFriend("hiEvtAnalyzer/HiTree") # to get the evt weight

fout = TFile("/home/llr/cms/kalipoliti/rootFiles/svTagInfo_histos.root", "recreate")

hsv = TH1D("hsv", "Percentage of tracks from B products associated to any SV per event", 25, 0, 100)
hsv.SetXTitle("% b-product tracks in SVs per event")
hsv.SetYTitle("(Weighted) nb of events")

t.SetBranchStatus("*", 0)
for branchName in ["nref", "nIP", "ipEta", "ipPhi", "ipMatchStatus", "svtxTrEta", "svtxTrPhi", "weight"]:
    t.SetBranchStatus(branchName, 1)

nentries = t.GetEntries()

for ient in range(nentries):
    ient = int(ient)
    
    # for debugging purposes 
    #if (ient < 33200): 
    #    continue
    #if (ient > 33290):
    #    break
        
    # Show progress
    if (ient % 1000000 == 0):
        print("i = ", ient)
    
    t.GetEntry(ient)
    
    bProducts = 0
    bProductsInSV = 0
    for itrack in range(t.nIP):
        itrack = int(itrack)
        
        # Keep only B decay products
        sta = t.ipMatchStatus[itrack]
        if (sta < 100): 
            continue
        bProducts += 1
        
        # Look for track in SV
        eps = 1e-3
        
        trackFoundInSV = False
        svtxTrEta = np.concatenate([np.array(t.svtxTrEta[ijet]) if (len(t.svtxTrEta[ijet]) > 0) else [] for ijet in range(t.nref)])
        svtxTrPhi = np.concatenate([np.array(t.svtxTrPhi[ijet]) if (len(t.svtxTrEta[ijet]) > 0) else [] for ijet in range(t.nref)])
        for svEta, svPhi in zip(svtxTrEta, svtxTrPhi):
            if ((t.ipEta[itrack] - svEta) > eps): 
                continue
            if ((t.ipPhi[itrack] - svPhi) > eps): 
                continue
            trackFoundInSV = True
            break
            
        if trackFoundInSV:
            bProductsInSV += 1
    perc = -1
    if (bProducts > 0):
        perc = (bProductsInSV / bProducts) * 100
    hsv.Fill(perc, t.weight)       
    
hsv.Write()

c = TCanvas("c", "c", 800, 800)
hsv.Draw("hist")
c.Draw()

fout.Close()
