
import ROOT
from DataFormats.FWLite import Events, Handle

#events = Events ('root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/RECO-FEVT/2D716/ZpTauTau8TeV_nosplit_716.root')
#events = Events ('root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/RECO-FEVT/2D716/ZpTauTau8TeV_simsplit_716.root')
events = Events ('root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/RECO-FEVT/2D716/ZpTauTau8TeV_split_716.root')

# create handle outside of loop
handle  = Handle ('std::vector<reco::Track>')
# a label is just a tuple of strings that is initialized just
# like and edm::InputTag
label = ("generalTracks")

ROOT.gROOT.SetBatch()        # don't pop up canvases

ROOT.gROOT.SetStyle('Plain') # white background
hPt = ROOT.TH1F ("hPt", "hPt", 100, 0, 200)
hEta= ROOT.TH1F ("hEta", "Eta", 50, -2.5, 2.5)
hPhi= ROOT.TH1F ("hPhi", "Phi", 160, -3.2, 3.2)
hchi2= ROOT.TH1F ("hChi2", "Chi2", 100, 0, 20)
hchi2norm= ROOT.TH1F ("hChi2Norm", "Chi2Norm", 100, 0, 10)
hNHits= ROOT.TH1F ("hNHIts", "Nhits", 30, 0, 30)
hNValidHits= ROOT.TH1F ("hNValidHits", "NValidHits", 30, 0, 30)
hNLostHits= ROOT.TH1F ("hNLostHits", "hNLostHits", 30, 0, 30)

# loop over events
for event in events:
    # use getByLabel, just like in cmsRun
    event.getByLabel (label, handle)
    # get the product
    tracks = handle.product()
    
    for trk in tracks:
        hPt.Fill(trk.pt())
        hEta.Fill(trk.eta())
        hPhi.Fill(trk.phi())
        hchi2.Fill(trk.chi2())
        hchi2norm.Fill(trk.chi2()/trk.ndof())
        hNHits.Fill(trk.hitPattern().numberOfHits())
        hNValidHits.Fill(trk.hitPattern().numberOfValidHits())
        hNLostHits.Fill(trk.hitPattern().numberOfLostHits())

# make a canvas, draw, and save it
#c1 = ROOT.TCanvas()
#c1.SetLogy(1)
#hPt.Draw()
#c1.Print ("trackPt.png")


outfile = ROOT.TFile("splittedTracks_sqlite2D.root", "RECREATE")
#outfile = ROOT.TFile("simsplittedTracks_sqlite2D.root", "RECREATE")
#outfile = ROOT.TFile("nosplittedTracks_sqlite2D.root", "RECREATE")
outfile.cd()
hPt.Write()
hEta.Write()
hPhi.Write()
hchi2.Write()
hchi2norm.Write()
hNHits.Write()
hNValidHits.Write()
hNLostHits.Write()

outfile.Close()
