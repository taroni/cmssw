import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
# Include the RandomNumberGeneratorService definition
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")

# Famos sequences
process.load("FastSimulation.Configuration.CommonInputsFake_cff")

process.load("FastSimulation.Configuration.FamosSequences_cff")

# Keep the logging output to a nice level #
process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)
process.source = cms.Source("FlatRandomPtGunSource",
    PGunParameters = cms.untracked.PSet(
        MaxPt = cms.untracked.double(150.0),
        MinPt = cms.untracked.double(150.0),
        # you can request more than 1 particle
        #untracked vint32  PartID = { 211, 11, -13 }
        PartID = cms.untracked.vint32(511, 531),
        MaxEta = cms.untracked.double(2.5),
        MaxPhi = cms.untracked.double(3.14159265359),
        MinEta = cms.untracked.double(-2.5),
        MinPhi = cms.untracked.double(-3.14159265359) ## it must be in radians

    ),
    Verbosity = cms.untracked.int32(0), ## for printouts, set it to 1 (or greater)   

    firstRun = cms.untracked.uint32(1)
)

process.Timing = cms.Service("Timing")

process.o1 = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('test_muisol.root')
)

process.p1 = cms.Path(process.famosWithMuonsAndIsolation)
process.outpath = cms.EndPath(process.o1)
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.paramMuons.MUONS.ProduceL1Muons = True
process.paramMuons.MUONS.ProduceL3Muons = True
process.paramMuons.MUONS.ProduceGlobalMuons = True
process.MessageLogger.destinations = ['detailedInfo.txt']


