import FWCore.ParameterSet.Config as cms

recHitRecoveryFilter = cms.EDFilter("RecHitRecoveryFilter",
   DoEB = cms.bool(True),
   DoEE = cms.bool(False),
   EBRecHits = cms.InputTag('ecalRecHit', 'EcalRecHitsEB'),
   EERecHits = cms.InputTag('ecalRecHit', 'EcalRecHitsEE')
)

