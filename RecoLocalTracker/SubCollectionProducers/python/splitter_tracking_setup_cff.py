from RecoLocalTracker.SubCollectionProducers.SeedClusterRemover_cfi import *
seedClusterRemover.stripClusters = 'splitClusters'
seedClusterRemover.pixelClusters = 'splitClusters'

from RecoTracker.IterativeTracking.InitialStep_cff import *


initialStepSeedLayers.BPix.HitProducer = 'mySiPixelRecHits'
initialStepSeedLayers.FPix.HitProducer = 'mySiPixelRecHits'
initialStepClusters.pixelClusters = 'splitClusters'
initialStepClusters.stripClusters = 'splitClusters'
initialStepClusters.stripRecHits = 'mySiStripRecHits'
initialStepSeeds.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
initialStepSeeds.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'
initialStepSeeds.OrderedHitsFactoryPSet.SeedingLayers = 'initialStepSeedLayers'

from RecoTracker.IterativeTracking.ElectronSeeds_cff  import *
initialStepSeedClusterMask.stripClusters = 'splitClusters'
initialStepSeedClusterMask.pixelClusters = 'splitClusters'

pixelPairStepSeedClusterMask.stripClusters  = 'splitClusters'
pixelPairStepSeedClusterMask.pixelClusters = 'splitClusters'

mixedTripletStepSeedClusterMask.stripClusters  = 'splitClusters'
mixedTripletStepSeedClusterMask.pixelClusters = 'splitClusters'


tripletElectronClusterMask.stripClusters  = 'splitClusters'
tripletElectronClusterMask.pixelClusters = 'splitClusters'

tripletElectronSeedLayers.BPix.HitProducer = 'mySiPixelRecHits'
tripletElectronSeedLayers.FPix.HitProducer = 'mySiPixelRecHits'

pixelPairElectronSeedLayers.BPix.HitProducer = 'mySiPixelRecHits'
pixelPairElectronSeedLayers.FPix.HitProducer = 'mySiPixelRecHits'

stripPairElectronSeedLayers.TIB.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
stripPairElectronSeedLayers.TID.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
stripPairElectronSeedLayers.TEC.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")

from RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi import *
MeasurementTracker.pixelClusterProducer = cms.string('splitClusters')
MeasurementTracker.stripClusterProducer = cms.string('splitClusters')
from RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi import *#new in 700pre8
MeasurementTrackerEvent.pixelClusterProducer = cms.string('splitClusters')
MeasurementTrackerEvent.stripClusterProducer = cms.string('splitClusters')



from RecoTracker.IterativeTracking.LowPtTripletStep_cff import *
lowPtTripletStepClusters.pixelClusters = 'splitClusters'
lowPtTripletStepClusters.stripClusters = 'splitClusters'
lowPtTripletStepSeedLayers.BPix.HitProducer = 'mySiPixelRecHits'
lowPtTripletStepSeedLayers.FPix.HitProducer = 'mySiPixelRecHits'
lowPtTripletStepSeeds.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
lowPtTripletStepSeeds.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'


from RecoTracker.IterativeTracking.PixelPairStep_cff import *
pixelPairStepClusters.pixelClusters = 'splitClusters'
pixelPairStepClusters.stripClusters = 'splitClusters'
pixelPairStepSeedLayers.BPix.HitProducer = 'mySiPixelRecHits'
pixelPairStepSeedLayers.FPix.HitProducer = 'mySiPixelRecHits'
pixelPairStepSeeds.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
pixelPairStepSeeds.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'

from RecoTracker.IterativeTracking.DetachedTripletStep_cff import *
detachedTripletStepClusters.pixelClusters = 'splitClusters'
detachedTripletStepClusters.stripClusters = 'splitClusters'
detachedTripletStepSeedLayers.BPix.HitProducer = 'mySiPixelRecHits'
detachedTripletStepSeedLayers.FPix.HitProducer = 'mySiPixelRecHits'
detachedTripletStepSeeds.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
detachedTripletStepSeeds.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'
detachedTripletStepClusters.stripRecHits = 'mySiStripRecHits'

from RecoTracker.IterativeTracking.MixedTripletStep_cff import *
mixedTripletStepClusters.pixelClusters = 'splitClusters'
mixedTripletStepClusters.stripClusters = 'splitClusters'
mixedTripletStepSeedLayersA.BPix.HitProducer = 'mySiPixelRecHits'
mixedTripletStepSeedLayersA.FPix.HitProducer = 'mySiPixelRecHits'
mixedTripletStepSeedLayersB.BPix.HitProducer = 'mySiPixelRecHits'
mixedTripletStepSeedsA.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
mixedTripletStepSeedsA.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'
mixedTripletStepSeedsB.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
mixedTripletStepSeedsB.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'
mixedTripletStepSeedLayersA.TEC.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
mixedTripletStepSeedLayersB.TIB.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")


from RecoTracker.IterativeTracking.PixelLessStep_cff import *
pixelLessStepSeedClusterMask.stripClusters  = 'splitClusters'
pixelLessStepSeedClusterMask.pixelClusters = 'splitClusters'
pixelLessStepClusters.pixelClusters = 'splitClusters'
pixelLessStepClusters.stripClusters = 'splitClusters'
pixelLessStepSeeds.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
pixelLessStepSeeds.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'
#pixelLessStepSeedsB.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
#pixelLessStepSeedsB.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'
pixelLessStepSeedClusters.stripRecHits = 'mySiStripRecHits'
pixelLessStepSeedClusters.pixelClusters = 'splitClusters'
pixelLessStepSeedClusters.stripClusters = 'splitClusters'
pixelLessStepSeedLayers.MTIB.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")
pixelLessStepSeedLayers.MTID.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")
pixelLessStepSeedLayers.MTEC.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")

pixelLessStepSeedLayers.TIB.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
pixelLessStepSeedLayers.TID.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
pixelLessStepSeedLayers.TEC.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")

from RecoTracker.IterativeTracking.TobTecStep_cff import *
tobTecStepClusters.pixelClusters = 'splitClusters'
tobTecStepClusters.stripClusters = 'splitClusters'
#tobTecStepClusters.oldClusterRemovalInfo = 'splitClusters'
#tobTecStepSeeds.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'
tobTecStepSeedClusters.pixelClusters = 'splitClusters'
tobTecStepSeedClusters.stripClusters = 'splitClusters'
tobTecStepSeedClusters.stripRecHits = 'mySiStripRecHits'
tobTecStepSeedLayersPair.TOB.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
tobTecStepSeedLayersPair.TEC.matchedRecHits= cms.InputTag("mySiStripRecHits","matchedRecHit")
tobTecStepSeedLayersTripl.TOB.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
tobTecStepSeedLayersTripl.MTOB.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")
tobTecStepSeedLayersTripl.MTEC.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")


from RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi import *
MeasurementTracker.pixelClusterProducer = cms.string('splitClusters')
MeasurementTracker.stripClusterProducer = cms.string('splitClusters')
from RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi import *#new in 700pre8
MeasurementTrackerEvent.pixelClusterProducer = cms.string('splitClusters')
MeasurementTrackerEvent.stripClusterProducer = cms.string('splitClusters')
from RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi import *#new in 700pre8
MeasurementTrackerEvent.pixelClusterProducer = cms.string('splitClusters')
MeasurementTrackerEvent.stripClusterProducer = cms.string('splitClusters')

from  RecoTracker.TkSeedGenerator.GlobalSeedsFromTriplets_cff import *
globalSeedsFromTriplets.ClusterCheckPSet.ClusterCollectionLabel='splitClusters'
globalSeedsFromTriplets.ClusterCheckPSet.PixelClusterCollectionLabel='splitClusters'



from RecoTracker.ConversionSeedGenerators.ConversionStep_cff import *
photonConvTrajSeedFromSingleLeg.ClusterCheckPSet.PixelClusterCollectionLabel = 'splitClusters'
photonConvTrajSeedFromSingleLeg.ClusterCheckPSet.ClusterCollectionLabel      = 'splitClusters'
convClusters.pixelClusters = 'splitClusters'
convClusters.stripClusters = 'splitClusters'
convLayerPairs.BPix.HitProducer = 'mySiPixelRecHits'
convLayerPairs.FPix.HitProducer = 'mySiPixelRecHits'
convLayerPairs.TIB1.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
convLayerPairs.TIB2.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
convLayerPairs.TIB3.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")
convLayerPairs.TIB4.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")
convLayerPairs.TID1.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
convLayerPairs.TID1.stereoRecHits = cms.InputTag("mySiStripRecHits","stereoRecHitUnmatched")
convLayerPairs.TID1.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHitUnmatched")
convLayerPairs.TID2.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
convLayerPairs.TID2.stereoRecHits = cms.InputTag("mySiStripRecHits","stereoRecHitUnmatched")
convLayerPairs.TID2.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHitUnmatched")
convLayerPairs.TID3.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
convLayerPairs.TID3.stereoRecHits = cms.InputTag("mySiStripRecHits","stereoRecHitUnmatched")
convLayerPairs.TID3.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHitUnmatched")
convLayerPairs.TEC.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
convLayerPairs.TEC.stereoRecHits = cms.InputTag("mySiStripRecHits","stereoRecHitUnmatched")
convLayerPairs.TEC.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHitUnmatched")
convLayerPairs.TOB1.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
convLayerPairs.TOB2.matchedRecHits = cms.InputTag("mySiStripRecHits","matchedRecHit")
convLayerPairs.TOB3.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")
convLayerPairs.TOB4.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")
convLayerPairs.TOB5.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")
convLayerPairs.TOB6.rphiRecHits = cms.InputTag("mySiStripRecHits","rphiRecHit")

from RecoLocalCalo.HcalRecProducers.HBHEIsolatedNoiseReflagger_cfi import *
hbhereco.hbheInput= cms.InputTag("hbheprereco::SPLIT")

