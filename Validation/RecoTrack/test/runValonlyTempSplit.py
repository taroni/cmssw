import FWCore.ParameterSet.Config as cms
process = cms.Process('VAL')
# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("SimGeneral.MixingModule.mixNoPU_cfi")
process.load('Configuration/StandardSequences/Simulation_cff')
process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load("DQMServices.Components.DQMFileSaver_cfi")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.MessageLogger.cerr.FwkReport.reportEvery = 10
# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
#        'file:/tmp/taroni/ZpTauTau8TeV_nosplit_71X.root'
        #'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/ZpTauTau8TeV_nosplit_71X.root'
        'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/RECO-FEVT/2D716/ZpTauTau8TeV_split_716.root',
),
)

process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsPixelBarrelHighTof')
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsPixelBarrelLowTof')
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsPixelEndcapHighTof') 
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsPixelEndcapLowTof')
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsTECHighTof')
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsTECLowTof')
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsTIBHighTof') 
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsTIBLowTof')
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsTIDHighTof') 
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsTIDLowTof')
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsTOBHighTof') 
process.mix.mixObjects.mixSH.crossingFrames.append('TrackerHitsTOBLowTof')
process.mix.mixObjects.mixHepMC.makeCrossingFrame = True
process.mix.mixObjects.mixTracks.makeCrossingFrame = True
process.mix.mixObjects.mixVertices.makeCrossingFrame = True
process.mix.digitizers = cms.PSet(process.theDigitizersValid)

#process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")


process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
#process.load("SimTracker.TrackAssociation.quickTrackAssociatorByHits_cfi")
#process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.load("Validation.RecoTrack.cuts_cff")
process.load("Validation.RecoTrack.MultiTrackValidator_cff")
process.load("DQMServices.Components.EDMtoMEConverter_cff")
process.load("Validation.Configuration.postValidation_cff")
#process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')
process.TrackAssociatorByChi2ESProducer.chi2cut = 50.0
process.TrackAssociatorByChi2ESProducer.onlyDiagonal = True

########### configuration MultiTrackValidator ########
process.multiTrackValidator.outputFile = 'valid_NS_Zp8TeV_chi2_valOnly_slc6_71X.root'
process.multiTrackValidator.associators = ['TrackAssociatorByChi2']
process.multiTrackValidator.skipHistoFit=cms.untracked.bool(False)
#process.cutsRecoTracks.quality = cms.vstring('','highPurity')
#process.cutsRecoTracks.quality = cms.vstring('')
process.multiTrackValidator.runStandalone = True

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')
print process.GlobalTag

process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.multiTrackValidator.skipHistoFit=cms.untracked.bool(False)

process.multiTrackValidator.label = ['generalTracks::SPLIT']
process.multiTrackValidator.useLogPt=cms.untracked.bool(True)
process.multiTrackValidator.minpT = cms.double(0.9)
process.multiTrackValidator.maxpT = cms.double(1000.0)
process.multiTrackValidator.nintpT = cms.int32(50)
process.multiTrackValidator.UseAssociators = cms.bool(True)
## process.multiTrackValidator.label_tv=cms.InputTag("mergedtruthNoSimHits","MergedTrackTruth")
## process.multiTrackValidator.label_tp_effic=cms.InputTag("mergedtruthNoSimHits","MergedTrackTruth")
## process.multiTrackValidator.label_tp_fake=cms.InputTag("mergedtruthNoSimHits","MergedTrackTruth")

process.load("Validation.RecoTrack.cuts_cff")
process.cutsRecoTracks.ptMin    = cms.double(0.5)
process.cutsRecoTracks.minHit   = cms.int32(10)
#process.cutsRecoTracks.minRapidity  = cms.int32(-1.0)
#process.cutsRecoTracks.maxRapidity  = cms.int32(1.0)

process.load("SimTracker.TrackerHitAssociation.clusterTpAssociationProducer_cfi")


#process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)
process.dqmSaver.workflow = "/ZpTauTau/7_1_6/split"


process.validation = cms.Sequence(
    process.mix*process.tpClusterProducer *
    process.multiTrackValidator
)
process.p = cms.Path(process.cutsRecoTracks*process.validation
)
process.dqmsave_step = cms.Path(process.dqmSaver)
# Schedule definition
process.schedule = cms.Schedule(process.p,process.dqmsave_step)
