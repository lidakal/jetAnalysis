from ROOT import TFile, TTree, TGraph, TMultiGraph, TCanvas, TLegend 
from ROOT import kOpenCircle, kFullCircle, kOpenDiamond, kFullDiamond
from ROOT import kBlack, kRed, kBlue, kGreen

import numpy as np

def passTrackBTag(inSV, ip3dSig):
    if inSV or (np.abs(ip3dSig) > 3):
        return True
    return False

def draw_event(ient):
    fname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSig_looserCut_fixedBugs/merged_HiForestAOD.root"
    f = TFile(fname)
    t = f.Get("ak4PFJetAnalyzer/t")
    hi = f.Get("bDecayAna/hi")

    t.SetBranchStatus("*", 0)

    activeBranches = ["nref", "jtHadFlav", "jtNbHad", "jteta", "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", "nselIPtrk",
                    "ipEta", "ipPhi", "ipEtaMatch", "ipPhiMatch", "ipMatchStatus",
                    "ip3dSig",
                    "svtxTrEta", "svtxTrPhi", "svtxdls", "svtxm", "svtxmcorr", "nsvtx", "svtxntrk"]

    for branch in activeBranches:
        t.SetBranchStatus(branch, 1)

    # ---- Select entry ----
    #ient = 13
    t.GetEntry(ient)
    hi.GetEntry(ient)

    mg = TMultiGraph()

    # draw all gen particles

    phi_all = []
    eta_all = []

    phi_b = []
    eta_b = []

    for ipar in range(hi.mult):
        phi = hi.phi[ipar]
        eta = hi.eta[ipar]
        sta = hi.sta[ipar]

        phi_all.append(phi)
        eta_all.append(eta)

        if (sta >= 100):
            phi_b.append(phi)
            eta_b.append(eta)

    phi_all = np.array(phi_all)
    eta_all = np.array(eta_all)

    gr_all = TGraph(len(phi_all), phi_all, eta_all)
    gr_all.SetMarkerStyle(kOpenCircle)
    gr_all.SetMarkerColorAlpha(kBlack, 0.5)
    mg.Add(gr_all)

    # color in gen particles coming from b decays
    phi_b = np.array(phi_b)
    eta_b = np.array(eta_b)

    gr_b = TGraph(len(phi_b), phi_b, eta_b)
    gr_b.SetMarkerStyle(kFullCircle)
    gr_b.SetMarkerColorAlpha(kBlack, 0.5)
    mg.Add(gr_b)

    jetColor = 2
    itrackOffset = 0

    for ijet in range(t.nref):
        ntracks = t.nselIPtrk[ijet]
        
        # universal eta cut
        if (np.abs(t.jteta[ijet]) > 2):
            itrackOffset += ntracks
            continue
            
        # b-jet cut -> Change to btag at some point
        if (t.jtHadFlav[ijet] != 5):
            itrackOffset += ntracks
            continue
            
        # get tracks only in this jet
        phi_jet = []
        eta_jet = []
        
        phi_match = []
        eta_match = []
        
        phi_pass = []
        eta_pass = []

        for itrack in range(ntracks):
            ipPhi = t.ipPhi[itrackOffset + itrack]
            ipEta = t.ipEta[itrackOffset + itrack]
            
            phi_jet.append(ipPhi)
            eta_jet.append(ipEta)
            
            phi_match.append(t.ipPhiMatch[itrackOffset + itrack])
            eta_match.append(t.ipEtaMatch[itrackOffset + itrack])
            
            # Go over SVs
            eps = 0.000001
            iSVtrackOffset = 0
            inSV = False
            for isv in range(t.nsvtx[ijet]):
                for iSVtrack in range(t.svtxntrk[ijet][isv]):
                    trackEta = t.svtxTrEta[ijet][iSVtrackOffset + iSVtrack];
                    trackPhi = t.svtxTrPhi[ijet][iSVtrackOffset + iSVtrack];

                    if (np.abs(trackEta - ipEta) > eps): continue
                    if (np.abs(trackPhi - ipPhi) > eps): continue

                    inSV = True
                    break
                    
            passSelection = passTrackBTag(inSV, t.ip3dSig[itrackOffset + itrack])
            if passSelection:
                phi_pass.append(ipPhi)
                eta_pass.append(ipEta)

        
        phi_jet = np.array(phi_jet)
        eta_jet = np.array(eta_jet)
        
        gr_jet = TGraph(len(phi_jet), phi_jet, eta_jet)
        gr_jet.SetMarkerStyle(kOpenDiamond)
        gr_jet.SetMarkerColor(jetColor)
        gr_jet.SetMarkerSize(4)
        mg.Add(gr_jet)
        
        phi_pass = np.array(phi_pass)
        eta_pass = np.array(eta_pass)

        gr_pass = TGraph(len(phi_pass), phi_pass, eta_pass)
        gr_pass.SetMarkerStyle(kFullDiamond)
        gr_pass.SetMarkerColorAlpha(jetColor, 0.1)
        gr_pass.SetMarkerSize(4)
        mg.Add(gr_pass)
        
        phi_match = np.array(phi_match)
        eta_match = np.array(eta_match)

        gr_match = TGraph(len(phi_match), phi_match, eta_match)
        gr_match.SetMarkerStyle(kOpenCircle)
        gr_match.SetMarkerColor(jetColor)
        mg.Add(gr_match)
        
        jetColor += 1
        itrackOffset += ntracks

    c = TCanvas("c", "", 1000, 800)

    mg.GetXaxis().SetTitle("#phi")
    mg.GetYaxis().SetTitle("#eta")
    mg.GetYaxis().SetTitleOffset(1.)

    mg.Draw("pa")

    mg.SetMinimum(-2.)
    mg.SetMaximum(2.)

    c.Draw()
    c.Print("test.png")

if __name__ == "__main__":
    draw_event(13)