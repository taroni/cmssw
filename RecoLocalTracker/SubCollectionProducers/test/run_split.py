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

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
        'root://eoscms//eos/cms/store/user/taroni/ClusterSplitting/ZpTauTau8TeV/GEN-SIM-RAW/ZpTT_1500_8TeV_Tauola_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT.root'
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
    simSplitPixel         = cms.bool(False), # ideal pixel splitting turned OFF
    simSplitStrip         = cms.bool(False), # ideal strip splitting turned OFF
    tmpSplitPixel         = cms.bool(True), # template pixel spliting
    tmpSplitStrip         = cms.bool(True), # template strip splitting
    useStraightTracks     = cms.bool(True),
    LoadTemplatesFromDB  = cms.bool(True),
    StripTemplateID      = cms.uint32(10),
    test     = cms.bool(True)
    )

process.mySiPixelRecHits = process.siPixelRecHits.clone(src = cms.InputTag("splitClusters"))
process.mySiStripRecHits = process.siStripMatchedRecHits.clone(
    src = cms.InputTag("splitClusters"),  ClusterProducer = cms.InputTag("splitClusters")
    )


process.newrechits = cms.Sequence(process.mySiPixelRecHits*process.mySiStripRecHits)

######## track to vertex assoc ##################3
from CommonTools.RecoUtils.pf_pu_assomap_cfi import AssociationMaps
process.Vertex2TracksDefault = AssociationMaps.clone(
    AssociationType = cms.InputTag("VertexToTracks"),
    MaxNumberOfAssociations = cms.int32(1)
)

# The commands included in splitter_tracking_setup_cff.py instruct 
# the tracking machinery to use the clusters and rechits generated after 
# cluster splitting (instead of the default clusters and rechits)
#process.load('RecoLocalTracker.SubCollectionProducers.splitter_tracking_setup_cff')
from RecoLocalTracker.SubCollectionProducers.splitter_tracking_setup_cff import customizeTracking
customizeTracking('splitClusters', 'splitClusters', 'mySiPixelRecHits', 'mySiStripRecHits','splitSiPixelClustersCache')

process.splitSiPixelClustersCache = cms.EDProducer( "SiPixelClusterShapeCacheProducer",
                                                  src = cms.InputTag( "splitClusters" ),
                                                  onDemand = cms.bool( False )
)



process.fullreco = cms.Sequence(process.globalreco*process.highlevelreco)
process.Globalreco = cms.Sequence(process.globalreco)
process.Highlevelreco = cms.Sequence(process.highlevelreco)

process.options = cms.untracked.PSet(

)

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

# Output definition
process.PixelTreeSplit = cms.EDAnalyzer(
    "PixelAnalysisTree",
    verbose                = cms.untracked.int32(0),
    rootFileName           = cms.untracked.string('pixelTrees_TS_DATA.root'),
    treeName               = cms.untracked.string('treeTmpSplit'),
    dumpAllEvents          = cms.untracked.int32(1),
    globalTag              = cms.string("GR_R_53::All"),
    muonCollectionLabel    = cms.untracked.InputTag('muons'),
#    trajectoryInputLabel   = cms.untracked.InputTag('TrackRefitter'),
    trajectoryInputLabel   = cms.untracked.InputTag('generalTracks::SPLIT'),          
    trackCollectionLabel   = cms.untracked.InputTag('generalTracks::SPLIT'),
    pixelClusterLabel      = cms.untracked.InputTag('splitClusters'),
    pixelRecHitLabel       = cms.untracked.InputTag('mySiPixelRecHits'),
    L1GTReadoutRecordLabel = cms.untracked.InputTag("gtDigis"),
    hltL1GtObjectMap       = cms.untracked.InputTag("hltL1GtObjectMap"),          
    HLTResultsLabel        = cms.untracked.InputTag("TriggerResults::HLT"),
    HLTProcessName         = cms.untracked.string("HLT"),
    mapTrack2Vertex        = cms.untracked.bool(True)
    )


process.RECOoutput = cms.OutputModule("PoolOutputModule",
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('/tmp/taroni/ZpTauTau8TeV_split_71X_test.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO')
    )
)
## process.RECOoutput.outputCommands.append( 'keep TrackingParticles_mergedtruth_MergedTrackTruth_*')
## process.RECOoutput.outputCommands.append( 'keep TrackingVertexs_mergedtruth_MergedTrackTruth_*')

# Additional output definition
process.dump = cms.EDAnalyzer("EventContentAnalyzer")
# Other statements


process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')




from RecoLocalCalo.HcalRecProducers.HBHEIsolatedNoiseReflagger_cfi import *
process.hbhereco.hbheInput= cms.InputTag("hbheprereco::SPLIT")
from CondCore.DBCommon.CondDBSetup_cfi import *
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CalibTracker.SiPixelESProducers.SiPixelTemplateDBObjectESProducer_cfi")
process.myPixelTemplate = cms.ESSource("PoolDBESSource",CondDBSetup,
                                             connect = cms.string("sqlite_file:/afs/cern.ch/work/t/taroni/private/ClusterSplitting/templates/siPixelTemplates38T_IOV7.db"),
                                             toGet = cms.VPSet(cms.PSet(record = cms.string("SiPixelTemplateDBObjectRcd"),
                                             tag = cms.string("SiPixelTemplateDBObject38Tv3"))
                                                           )
                                             )
process.es_prefer_myPixelTemplate = cms.ESPrefer("PoolDBESSource","myPixelTemplate")

process.myPixel2DTemplate = cms.ESSource("PoolDBESSource",CondDBSetup,
                                         connect = cms.string("sqlite_file:/afs/cern.ch/work/t/taroni/private/ClusterSplitting/templates/siPixel2DTemplates38T_IOV7.db"),
                                         toGet = cms.VPSet(cms.PSet(record = cms.string("SiPixel2DTemplateDBObjectRcd"),
                                         tag = cms.string("SiPixel2DTemplateDBObject38Tv17"))
                                                           )
                                         )
process.es_prefer_myPixel2DTemplate = cms.ESPrefer("PoolDBESSource","myPixel2DTemplate")


# Path and EndPath definitions
#process.pre_init  = cms.Path(cms.Sequence(process.pdigi*process.SimL1Emulator*process.DigiToRaw))

#process.init_step = cms.Path(cms.Sequence(process.RawToDigi*process.localreco*process.offlineBeamSpot+process.recopixelvertexing))
process.init_step = cms.Path(cms.Sequence(process.RawToDigi*process.localreco*process.offlineBeamSpot+process.siPixelClusterShapeCache*process.recopixelvertexing))

process.dump_step = cms.Path(process.dump)
process.splitClusters_step=cms.Path(process.mix+process.splitClusters+process.splitSiPixelClustersCache)
process.newrechits_step=cms.Path(process.newrechits)

process.fullreco_step=cms.Path(process.fullreco)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)
#process.pixeltree_tempsplit =cms.Path(process.PixelTreeSplit)
#process.vertex_assoc = cms.Path(process.Vertex2TracksDefault)

# Schedule definition

#process.schedule = cms.Schedule(process.init_step,process.splitClusters_step,process.newrechits_step,process.fullreco_step,process.RECOoutput_step)
#process.schedule = cms.Schedule(process.init_step)
process.schedule = cms.Schedule(process.init_step, process.splitClusters_step,process.newrechits_step, process.fullreco_step,process.RECOoutput_step )
