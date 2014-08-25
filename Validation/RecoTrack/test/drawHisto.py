import ROOT
from DataFormats.FWLite import Events, Handle

file1 = ROOT.TFile.Open("splittedTracks_sqlite2D.root")
file2 = ROOT.TFile.Open("simsplittedTracks_sqlite2D.root")
file3 = ROOT.TFile.Open("nosplittedTracks_sqlite2D.root")

ROOT.gROOT.SetBatch()        # don't pop up canvases

ROOT.gROOT.SetStyle('Plain') # white background
ROOT.gStyle.SetOptStat(0)    


histo = ["hPt", "hEta", "hPhi", "hChi2","hChi2Norm","hNHIts","hNValidHits","hNLostHits"]

title = ['p_{T} (GeV)', "#eta", '#phi', '#chi^2', '#chi^2/ndof', 'number of hits', 'number of valid hits', 'number of lost hits']


c1 = ROOT.TCanvas()
leg = ROOT.TLegend(0.6, 0.4, 0.75, 0.3)


for n, hname  in enumerate(histo):
    h1 = file1.Get(hname)
    h2 = file2.Get(hname)
    h3 = file3.Get(hname)
    h1.Draw()
    h1.SetLineColor(2)
    h1.SetLineWidth(2)
    h2.Draw("SAME")
    h2.SetLineWidth(2)
    h2.SetLineStyle(1)
    h3.Draw("SAME")
    h3.SetLineWidth(2)
    h3.SetLineStyle(1)
    h3.SetLineColor(4)
    h1.SetTitle("")
    if n==0: 
        leg.AddEntry(h1, "temp split", "lfb")
        leg.AddEntry(h2, "sim split", "lfb")
        leg.AddEntry(h3, "no split", "lfb")
    leg.Draw()
    leg.SetFillColor(0)
    h1.GetXaxis().SetTitle(title[n])
    c1.Update()
    c1.SetLogy(0)
    if hname=="hPt": c1.SetLogy(1) 
    c1.Print(hname+".png")

    
