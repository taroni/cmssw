import FWCore.ParameterSet.Config as cms

process = cms.Process('SPLIT')
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

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_0.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_1.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_2.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_3.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_4.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_5.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_6.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_7.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_8.root',
     'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTauTau_GEN-SIM-RAW_9.root'

),
)
                            
# from Jean-Roch
process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTemplate_cfi")
process.StripCPEfromTrackAngleESProducer = process.StripCPEfromTemplateESProducer.clone(ComponentName='StripCPEfromTrackAngle')

process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTemplate_cfi")
process.StripCPEfromTemplateESProducer.UseTemplateReco = True

# Include the latest pixel templates, which are not in DB. 
# These are to be used for pixel splitting.
process.load('RecoLocalTracker.SiPixelRecHits.PixelCPETemplateReco_cfi')
process.templates.LoadTemplatesFromDB = False

# This is the default speed. Recommended.
process.StripCPEfromTrackAngleESProducer.TemplateRecoSpeed = 0;

# Split clusters have larger errors. Appropriate errors can be 
# assigned by turning UseStripSplitClusterErrors = True. The strip hit pull  
# distributons improve considerably, but it does not help with b-tagging, 
# so leave it False by default 
process.StripCPEfromTrackAngleESProducer.UseStripSplitClusterErrors = True

# Turn OFF track hit sharing
process.load("TrackingTools.TrajectoryCleaning.TrajectoryCleanerBySharedHits_cfi")
process.trajectoryCleanerBySharedHits.fractionShared = 0.0
process.trajectoryCleanerBySharedHits.allowSharedFirstHit = False
process.load("RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi")
process.simpleTrackListMerger.ShareFrac = 0.0
process.simpleTrackListMerger.allowFirstHitShare = False

# The second step is to split merged clusters.
process.splitClusters = cms.EDProducer(
    "TrackClusterSplitter",
    stripClusters         = cms.InputTag("siStripClusters::SPLIT"),
    pixelClusters         = cms.InputTag("siPixelClusters::SPLIT"),
    useTrajectories       = cms.bool(False),
    trajTrackAssociations = cms.InputTag('generalTracks::SPLIT'),
    tracks                = cms.InputTag('pixelTracks::SPLIT'),
    propagator            = cms.string('AnalyticalPropagator'),
    vertices              = cms.InputTag('pixelVertices::SPLIT'),
    simSplitPixel         = cms.bool(True), # ideal pixel splitting turned OFF
    simSplitStrip         = cms.bool(True), # ideal strip splitting turned OFF
    tmpSplitPixel         = cms.bool(False), # template pixel spliting
    tmpSplitStrip         = cms.bool(False), # template strip splitting
    useStraightTracks     = cms.bool(True),
    test     = cms.bool(True)
    )

process.mySiPixelRecHits = process.siPixelRecHits.clone(src = cms.InputTag("splitClusters"))
process.mySiStripRecHits = process.siStripMatchedRecHits.clone(
    src = cms.InputTag("splitClusters"),  ClusterProducer = cms.InputTag("splitClusters")
    )

############################## inserted new stuff
                            
# from Jean-Roch 
process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTemplate_cfi")
process.StripCPEfromTrackAngleESProducer = process.StripCPEfromTemplateESProducer.clone(ComponentName='StripCPEfromTrackAngle')

process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTemplate_cfi")
process.StripCPEfromTemplateESProducer.UseTemplateReco = True

# Include the latest pixel templates, which are not in DB. 
# These are to be used for pixel splitting.
process.load('RecoLocalTracker.SiPixelRecHits.PixelCPETemplateReco_cfi')
process.templates.LoadTemplatesFromDB = False

# This is the default speed. Recommended.
process.StripCPEfromTrackAngleESProducer.TemplateRecoSpeed = 0;
tgrIndex = process.globalreco.index(process.trackingGlobalReco)
tgrIndexFromReco = process.reconstruction_fromRECO.index(process.trackingGlobalReco)
process.globalreco.remove(process.trackingGlobalReco)
process.reconstruction_fromRECO.remove(process.trackingGlobalReco)
del process.trackingGlobalReco
del process.ckftracks
del process.ckftracks_wodEdX
del process.ckftracks_plus_pixelless
del process.ckftracks_woBH
del process.iterTracking
del process.InitialStep
del process.LowPtTripletStep
del process.PixelPairStep
del process.DetachedTripletStep
del process.MixedTripletStep
del process.PixelLessStep
del process.TobTecStep

# Load the new Iterative Tracking configuration
process.load("RecoTracker.Configuration.RecoTrackerRunI_cff")

process.globalreco.insert(tgrIndex, process.trackingGlobalReco)
process.reconstruction_fromRECO.insert(tgrIndexFromReco, process.trackingGlobalReco)

############################## inserted new stuff

process.newrechits = cms.Sequence(process.mySiPixelRecHits*process.mySiStripRecHits)

######## track to vertex assoc ##################3
## from CommonTools.RecoUtils.pf_pu_assomap_cfi import AssociationMaps
## process.Vertex2TracksDefault = AssociationMaps.clone(
##     AssociationType = cms.InputTag("VertexToTracks"),
##     MaxNumberOfAssociations = cms.int32(1)
## )

# The commands included in splitter_tracking_setup_cff.py instruct 
# the tracking machinery to use the clusters and rechits generated after 
# cluster splitting (instead of the default clusters and rechits)
process.load('RecoLocalTracker.SubCollectionProducers.splitter_tracking_RunI_setup_cff')

process.fullreco = cms.Sequence(process.globalreco*process.highlevelreco)
process.options = cms.untracked.PSet(

)

#process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

# Output definition


process.RECOoutput = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('ZpTauTau8TeV_simsplit.root'),
    dataset = cms.untracked.PSet(
        #filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)
process.RECOoutput.outputCommands.append( 'keep TrackingParticles_mergedtruth_MergedTrackTruth_*')
process.RECOoutput.outputCommands.append( 'keep TrackingVertexs_mergedtruth_MergedTrackTruth_*')

# Additional output definition
process.dump = cms.EDAnalyzer("EventContentAnalyzer")
# Other statement
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'START70_V5::All', '')

process.GlobalTag.globaltag = 'START70_V5::All'

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

# Path and EndPath definitions
process.pre_init  = cms.Path(cms.Sequence(process.pdigi*process.SimL1Emulator*process.DigiToRaw))
process.init_step = cms.Path(cms.Sequence(process.RawToDigi*process.localreco*process.offlineBeamSpot+process.recopixelvertexing))
process.rechits_step=cms.Path(process.siPixelRecHits)
process.dump_step = cms.Path(process.dump)
process.splitClusters_step=cms.Path(process.mix+process.splitClusters)
process.newrechits_step=cms.Path(process.newrechits)
process.fullreco_step=cms.Path(process.fullreco)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)
#process.pixeltree_tempsplit =cms.Path(process.PixelTreeSplit)
#process.vertex_assoc = cms.Path(process.Vertex2TracksDefault)

process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
#process.load("SimTracker.TrackAssociation.quickTrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.load("Validation.RecoTrack.cuts_cff")
process.load("Validation.RecoTrack.MultiTrackValidator_cff")
process.load("DQMServices.Components.EDMtoMEConverter_cff")
process.load("Validation.Configuration.postValidation_cff")
#process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')
process.TrackAssociatorByChi2ESProducer.chi2cut = 50.0
process.TrackAssociatorByChi2ESProducer.onlyDiagonal = True

########### configuration MultiTrackValidator ########
process.multiTrackValidator.outputFile = 'valid_SS_Zp8TeV_runI_chi2.root'
process.multiTrackValidator.associators = ['TrackAssociatorByChi2']
process.multiTrackValidator.skipHistoFit=cms.untracked.bool(False)
#process.cutsRecoTracks.quality = cms.vstring('','highPurity')
#process.cutsRecoTracks.quality = cms.vstring('')
process.multiTrackValidator.runStandalone = True


process.multiTrackValidator.label = ['generalTracks::SPLIT']
process.multiTrackValidator.useLogPt=cms.untracked.bool(False)
process.multiTrackValidator.minpT = cms.double(0.1)
process.multiTrackValidator.maxpT = cms.double(3000.0)
process.multiTrackValidator.nintpT = cms.int32(40)
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

process.validation = cms.Sequence(
    process.tpClusterProducer *
    process.multiTrackValidator
)

process.p = cms.Path(
       process.cutsRecoTracks
       *
       process.validation
)


#Schedule definition
process.schedule = cms.Schedule(process.init_step,process.splitClusters_step,process.newrechits_step,process.fullreco_step, process.RECOoutput_step)
