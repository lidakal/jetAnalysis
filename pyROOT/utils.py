from ROOT import TFile, TCanvas, TLegend, TGraph, TMultiGraph, gStyle
from ROOT import kCircle, kFullCircle, kOpenDiamond, kOpenTriangleUp

import numpy as np
import pandas as pd

from myPalette import palette

class TreeAnalyzer():
    def __init__(self, filename = "HiForestAOD.root", treename = "ak4PFJetAnalyzer/t"):
        self.f = TFile(filename)
        self.t = self.f.Get(treename)
        self.hi = self.f.Get("bDecayAna/hi")
        self.HiTree = self.f.Get("hiEvtAnalyzer/HiTree")
        self.t.AddFriend(self.hi)
        self.t.AddFriend(self.HiTree)
        
        
    def makeMatches(self, ptCut = -1, entries = -1):
        self.matches = self.trackGenPartMatching(self.t, ptCut, entries)
        self.particles = self.makeParticlesdf(self.t)
        self.tracks = self.makeTracksdf(self.t)
        
        
    ''' 
        function to match reconstructed tracks to gen level particles

        tree:    TTree with data (both reconstructed tracks and particles);
                 if they are in two different trees, they can be joined
                 with tree1.AddFriend(tree2)
        ptCut:   cut on pt of gen level particles (in GeV)
        entries: list of entries to analyze; if entry = -1, all entries
    '''
    def trackGenPartMatching(self, tree, ptCut = -1, entries = -1, ):
        # variables to return; pandas.DataFrame
        trackGenPartMatches = pd.DataFrame(columns = ["Entry", "Jet", "Track", "genPartMatchID"], dtype = int)

        # unpack arguments
        if (entries == -1):
            entries = range(tree.GetEntries())
        entries = np.array(entries, ndmin = 1)

        # prepare tree
        tree.SetBranchStatus("*", 0)
        for branchName in ["nref", "jteta", "jtHadFlav", "nselIPtrk", "mult", "sta", "ipEta", "ipPhi", "ipPt", "eta", "phi", "pt"]:
            tree.SetBranchStatus(branchName, 1)

        for ient in entries:
            ient = int(ient)
            tree.GetEntry(ient)
            njets = tree.nref

            # track nb offset for ipEta, ipPhi: 
            # they are a vector of size nref*tracksPerJet
            itrackOffset = 0 

            # create a list of gen level particles that have already been matched
            matchedGenParts = []

            for ijet in range(njets):
                ijet = int(ijet)

                # eta cut on jet
                if (abs(tree.jteta[ijet]) > 2):
                    continue

                # tracks per jet
                ntracks = tree.nselIPtrk[ijet] 

                for itrack in range(ntracks):
                    itrack = int(itrack)
                    genPartMatchID = -1
                    drmin = 100

                    # apply pt cut on track
                    '''
                    if (tree.ipPt[itrack + itrackOffset] < ptCut) or (t.jtHadFlav[ijet] != 5):
                        pass
                    '''
                    if (False):
                        pass
                    else:
                        for imult in range (tree.mult):
                            imult = int(imult)
                            '''
                            # if the particle had already been matched to a track, continue
                            if (imult in matchedGenParts):
                                continue
                            '''

                            # apply ptCut on gen level particles
                            if (tree.pt[imult] < ptCut):
                                continue

                            deta = abs(tree.ipEta[itrack + itrackOffset] - tree.eta[imult])
                            dphi = abs(tree.ipPhi[itrack + itrackOffset] - tree.phi[imult])
                            dr = np.sqrt(deta**2+dphi**2)

                            if dr < drmin:
                                drmin = dr
                                genPartMatchID = imult
                                matchedGenParts.append(genPartMatchID)
                    trackGenPartMatches = pd.concat([trackGenPartMatches, pd.DataFrame({"Entry" : [ient], "Jet" : [ijet], "Track" : [itrack], "genPartMatchID" : [genPartMatchID]})], ignore_index = True)
                itrackOffset += ntracks
        key = "noPtCut"
        if (ptCut > 0):
            key = "ptCut%dGeV" % ptCut
            key = key.replace(".", "_")
        trackGenPartMatches.to_hdf("trackGenPartMatches.h5", key = key, index = False)
        return trackGenPartMatches

    def makeTracksdf(self, tree):
        # variables to return; pandas.DataFrame
        tracks = pd.DataFrame(columns = ["Entry", "Jet", "Flavour", "Track", "Pt", "Eta", "Phi"], dtype = int)

        # prepare tree
        tree.SetBranchStatus("*", 0)
        for branchName in ["nref", "jteta", "jtHadFlav", "nselIPtrk", "mult", "sta", "ipEta", "ipPhi", "ipPt", "eta", "phi", "pt"]:
            tree.SetBranchStatus(branchName, 1)

        entries = np.arange(tree.GetEntries())

        for ient in entries:
            ient = int(ient)
            tree.GetEntry(ient)
            njets = tree.nref

            # track nb offset for ipEta, ipPhi: 
            # they are a vector of size nref*tracksPerJet
            itrackOffset = 0 

            for ijet in range(njets):
                ijet = int(ijet)

                flavour = int(tree.jtHadFlav[ijet])

                # eta cut on jet
                if (abs(tree.jteta[ijet]) > 2):
                    continue

                # tracks per jet
                ntracks = tree.nselIPtrk[ijet] 

                for itrack in range(ntracks):
                    itrack = int(itrack)

                    pt = float(tree.ipPt[itrack + itrackOffset])
                    eta = float(tree.ipEta[itrack + itrackOffset])
                    phi = float(tree.ipPhi[itrack + itrackOffset])

                    tracks = pd.concat([tracks, pd.DataFrame({"Entry" : [ient], "Jet" : [ijet], "Flavour" : [flavour], "Track" : [itrack], "Pt" : [pt], "Eta" : [eta], "Phi" : [phi]})], ignore_index = True)

                itrackOffset += ntracks
        return tracks

    def makeParticlesdf(self, tree):
        # variables to return; pandas.DataFrame
        particles = pd.DataFrame(columns = ["Entry", "ID", "STA", "Pt", "Eta", "Phi"], dtype = int)

        # prepare tree
        tree.SetBranchStatus("*", 0)
        for branchName in ["nref", "jteta", "jtHadFlav", "nselIPtrk", "mult", "sta", "ipEta", "ipPhi", "ipPt", "eta", "phi", "pt"]:
            tree.SetBranchStatus(branchName, 1)

        entries = np.arange(tree.GetEntries())

        for ient in entries:
            ient = int(ient)
            tree.GetEntry(ient)

            for imult in range(tree.mult):
                imult = int(imult)

                ID = imult
                sta = int(tree.sta[imult])
                pt = float(tree.pt[imult])
                eta = float(tree.eta[imult])
                phi = float(tree.phi[imult])

                particles = pd.concat([particles, pd.DataFrame({"Entry" : [ient], "ID" : [ID], "STA" : [sta], "Pt" : [pt], "Eta" : [eta], "Phi" : [phi]})], ignore_index = True)
        return particles
    
    
    def plotEvent(self, entry):
        # variable to return; TCanvas
        c = TCanvas("c", "c", 800, 600)
        
        matches = self.matches
        particles = self.particles
        tracks = self.tracks

        mg = TMultiGraph()

        leg = TLegend(0.15, 0.15, 0.4, 0.3)
        leg.SetNColumns(2)

        gStyle.SetLegendTextSize(0.1)

        sel = tracks["Entry"] == entry
        njets = len(np.unique(tracks.loc[sel, "Jet"]))

        # add not matched particles
        selp = particles["Entry"] == entry
        selp1 = selp & (particles["STA"] == 1)
        xs = particles.loc[selp1, "Phi"].to_numpy()
        ys = particles.loc[selp1, "Eta"].to_numpy()
        gr1 = TGraph(len(xs), xs, ys)

        gr1.SetMarkerStyle(kCircle)
        gr1.SetLineWidth(0)
        gr1.SetMarkerSize(1)
        gr1.SetMarkerColorAlpha(1, 0.5)
        mg.Add(gr1)

        leg.AddEntry(gr1, "gen particles")

        selp1 = selp & (particles["STA"] == 2)
        xs = particles.loc[selp1, "Phi"].to_numpy()
        ys = particles.loc[selp1, "Eta"].to_numpy()
        gr2 = TGraph(len(xs), xs, ys)

        gr2.SetMarkerStyle(kFullCircle)
        gr2.SetLineWidth(0)
        gr2.SetMarkerSize(1)
        gr2.SetMarkerColorAlpha(1, 0.5)
        mg.Add(gr2)
        leg.AddEntry(gr2, "from B decays")


        for ijet in range(njets):
            ijet = int(ijet)

            selj = (tracks["Entry"] == entry) & (tracks["Jet"] == ijet)

            # Create 3 graphs for each jet: 
            # one for the tracks, one for the particles from B decays, one for the rest of the particles 
            grt = TGraph()
            grt.SetMarkerColor(palette(ijet))
            grt.SetMarkerSize(4)

            grp = TGraph()
            grp.SetMarkerColor(palette(ijet))
            grp.SetMarkerSize(1)
            grp.SetMarkerStyle(kCircle)

            grb = TGraph()
            grb.SetMarkerColor(palette(ijet))
            grb.SetMarkerSize(1)

            grt.SetMarkerStyle(kOpenDiamond)
            grp.SetMarkerStyle(kCircle)
            grb.SetMarkerStyle(kFullCircle)

            flavour = tracks.loc[selj, "Flavour"].to_numpy()[0]
            if (flavour == 5):
                grt.SetMarkerStyle(kOpenDiamond)
            else:
                grt.SetMarkerStyle(kOpenTriangleUp)

            ntracks = len(np.unique(tracks.loc[selj, "Track"]))

            # Add points to graph for each track / matched particle
            for itrack in range(ntracks):
                selt = (tracks["Entry"] == entry) & (tracks["Jet"] == ijet) & (tracks["Track"] == itrack)
                selm = (matches["Entry"] == entry) & (matches["Jet"] == ijet) & (matches["Track"] == itrack)

                xt = float(tracks.loc[selt, "Phi"])
                yt = float(tracks.loc[selt, "Eta"])

                grt.SetPoint(grt.GetN(), xt, yt)

                partID = int(matches.loc[selm, "genPartMatchID"])
                if (partID > 0):
                    selp = (particles["Entry"] == entry) & (particles["ID"] == partID)

                    xp = float(particles.loc[selp, "Phi"])
                    yp = float(particles.loc[selp, "Eta"])

                    sta = int(particles.loc[selp, "STA"])
                    if (sta == 2):
                        grb.SetPoint(grb.GetN(), xp, yp)
                    else:
                        grp.SetPoint(grp.GetN(), xp, yp)


            mg.Add(grt)
            if (grp.GetN() > 0):
                mg.Add(grp)
            if (grb.GetN() > 0): 
                mg.Add(grb)    

        mg.GetXaxis().SetTitle("#phi")
        mg.GetYaxis().SetTitle("#eta")

        mg.Draw("P")

        #mg.GetXaxis().SetLimits(-1.6,2.8)
        #mg.SetMinimum(-1.2)
        #mg.SetMaximum(-0.6)
        #mg.GetYaxis().SetLimits(-2,0)

        # add custom entries to legend
        gr3 = TGraph()
        gr3.SetMarkerStyle(kOpenTriangleUp)
        gr3.SetMarkerColor(1)
        gr3.SetMarkerSize(1)
        gr3.SetLineWidth(0)
        leg.AddEntry(gr3, "gudsc jets")

        gr4 = TGraph()
        gr4.SetMarkerStyle(kOpenDiamond)
        gr4.SetMarkerColor(1)
        gr4.SetMarkerSize(1)
        gr4.SetLineWidth(0)
        leg.AddEntry(gr4, "b jets")


        leg.Draw()

        c.SetGrid()
        c.Draw()
        return c
