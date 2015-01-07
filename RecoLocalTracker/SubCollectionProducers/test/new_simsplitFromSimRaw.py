# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 -n 10 --conditions auto:run2_mc -s RAW2DIGI,L1Reco,RECO,EI --datatier GEN-SIM-RECO,AODSIM --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --eventcontent RECOSIM,AODSIM --magField 38T_PostLS1 --io RECOPRODUP15.io --python RECOPRODUP15.py --no_exec --filein file:step2.root --fileout file:step3.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('SPLIT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('CommonTools.ParticleFlow.EITopPAG_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_7_3_0_pre3/RelValProdTTbar_13/GEN-SIM-RAW/MCRUN2_73_V5-v1/00000/30BCF1D1-6176-E411-B3F1-02163E010EEA.root',
'/store/relval/CMSSW_7_3_0_pre3/RelValProdTTbar_13/GEN-SIM-RAW/MCRUN2_73_V5-v1/00000/5E42E05E-4676-E411-861E-FA163EB22FE1.root',
'/store/relval/CMSSW_7_3_0_pre3/RelValProdTTbar_13/GEN-SIM-RAW/MCRUN2_73_V5-v1/00000/6646A08E-4F76-E411-9C1A-02163E010EF0.root',
'/store/relval/CMSSW_7_3_0_pre3/RelValProdTTbar_13/GEN-SIM-RAW/MCRUN2_73_V5-v1/00000/8CB5B058-4476-E411-8D05-02163E00EA37.root')
)

process.options = cms.untracked.PSet(

)
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
    LoadTemplatesFromDB  = cms.bool(True),
    StripTemplateID      = cms.uint32(10),
    test     = cms.bool(True)
    )

process.mySiPixelRecHits = process.siPixelRecHits.clone(src = cms.InputTag("splitClusters"))
process.mySiStripRecHits = process.siStripMatchedRecHits.clone(
    src = cms.InputTag("splitClusters"),  ClusterProducer = cms.InputTag("splitClusters")
    )


process.newrechits = cms.Sequence(process.mySiPixelRecHits*process.mySiStripRecHits)
from RecoLocalTracker.SubCollectionProducers.splitter_tracking_setup_cff import customizeTracking

customizeTracking('splitClusters', 'splitClusters', 'mySiPixelRecHits', 'mySiStripRecHits','splitSiPixelClustersCache')

process.splitSiPixelClustersCache = cms.EDProducer( "SiPixelClusterShapeCacheProducer",
                                                  src = cms.InputTag( "splitClusters" ),
                                                  onDemand = cms.bool( False )
)
#from RecoLocalCalo.HcalRecProducers.HBHEIsolatedNoiseReflagger_cfi import *
#process.hbhereco.hbheInput= cms.InputTag("hbheprereco::SPLIT")

process.fullreco = cms.Sequence(process.globalreco*process.highlevelreco)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('step3 nevts:10'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:relvalTT_step3_simsplittedfromsimraw.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

#process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
#    compressionLevel = cms.untracked.int32(4),
#    compressionAlgorithm = cms.untracked.string('LZMA'),
#    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
#    outputCommands = process.AODSIMEventContent.outputCommands,
#    fileName = cms.untracked.string('file:step3_inAODSIMfromsimraw.root'),
#    dataset = cms.untracked.PSet(
#        filterName = cms.untracked.string(''),
#        dataTier = cms.untracked.string('AODSIM')
#    )
#)
 
# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi*process.localreco*process.offlineBeamSpot+process.siPixelClusterShapeCache*process.recopixelvertexing)
process.L1Reco_step = cms.Path(process.L1Reco)
process.splitClusters_step=cms.Path(process.mix+process.splitClusters+process.splitSiPixelClustersCache)
process.newrechits_step=cms.Path(process.newrechits)

process.reconstruction_step = cms.Path(process.fullreco)
process.eventinterpretaion_step = cms.Path(process.EIsequence)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
#process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.splitClusters_step,process.newrechits_step, process.reconstruction_step,process.eventinterpretaion_step,process.endjob_step,process.RECOSIMoutput_step)#,process.AODSIMoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

# End of customisation functions
