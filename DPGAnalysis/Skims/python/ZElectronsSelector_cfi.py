import FWCore.ParameterSet.Config as cms

ELECTRON_CUT=("pt > 10 && abs(eta)<2.5")
goodZeeElectrons = cms.EDFilter("GsfElectronRefSelector",
                                src = cms.InputTag("gedGsfElectrons"),
                                cut = cms.string(ELECTRON_CUT)
                                )

identifiedElectrons = cms.EDFilter("ZElectronsSelectorAndSkim",
                         src    = cms.InputTag("goodZeeElectrons"),
                         effAreasConfigFile= cms.FileInPath("RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt"), # from https://github.com/ikrav/cmssw/blob/egm_id_80X_v1/RecoEgamma/ElectronIdentification/python/Identification/cutBasedElectronID_Summer16_80X_V1_cff.py#L131
                         rho = cms.InputTag("fixedGridRhoFastjetCentralCalo") #from https://github.com/cms-sw/cmssw/blob/09c3fce6626f70fd04223e7dacebf0b485f73f54/RecoEgamma/ElectronIdentification/python/Identification/cutBasedElectronID_tools.py#L564
                         )
DIELECTRON_CUT=("mass > 70 && mass < 110 && daughter(0).pt>20 && daughter(1).pt()>10")

diZeeElectrons = cms.EDProducer("CandViewShallowCloneCombiner",
                                decay       = cms.string("identifiedElectrons identifiedElectrons"),
                                checkCharge = cms.bool(False),
                                cut         = cms.string(DIELECTRON_CUT)
                                )
# dilepton counters
diZeeElectronsFilter = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("diZeeElectrons"),
                                    minNumber = cms.uint32(1)
                                    )


#sequences
zdiElectronSequence = cms.Sequence(goodZeeElectrons*identifiedElectrons*diZeeElectrons* diZeeElectronsFilter )
