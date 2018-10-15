#include "RecoLocalCalo/EcalRecProducers/plugins/EcalRecHitWorkerRecover.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/DataRecord/interface/EcalTimeCalibConstantsRcd.h"
#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDetId/interface/EcalScDetId.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

#include "CondFormats/EcalObjects/interface/EcalTimeCalibConstants.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EDProducer.h"


#define DEBUG_ 0


EcalRecHitWorkerRecover::EcalRecHitWorkerRecover(const edm::ParameterSet&ps, edm::ConsumesCollector&  c) :
  EcalRecHitWorkerBaseClass(ps,c)
{
        rechitMaker_ = std::make_unique<EcalRecHitSimpleAlgo>();
        // isolated channel recovery
        singleRecoveryMethod_    = ps.getParameter<std::string>("singleChannelRecoveryMethod");
        singleRecoveryThreshold_ = ps.getParameter<double>("singleChannelRecoveryThreshold");
        killDeadChannels_        = ps.getParameter<bool>("killDeadChannels");
        recoverEBIsolatedChannels_ = ps.getParameter<bool>("recoverEBIsolatedChannels");
        recoverEEIsolatedChannels_ = ps.getParameter<bool>("recoverEEIsolatedChannels");
        recoverEBVFE_ = ps.getParameter<bool>("recoverEBVFE");
        recoverEEVFE_ = ps.getParameter<bool>("recoverEEVFE");
        recoverEBFE_ = ps.getParameter<bool>("recoverEBFE");
        recoverEEFE_ = ps.getParameter<bool>("recoverEEFE");

	dbStatusToBeExcludedEE_ = ps.getParameter<std::vector<int> >("dbStatusToBeExcludedEE");
	dbStatusToBeExcludedEB_ = ps.getParameter<std::vector<int> >("dbStatusToBeExcludedEB");

        logWarningEtThreshold_EB_FE_ = ps.getParameter<double>("logWarningEtThreshold_EB_FE");
        logWarningEtThreshold_EE_FE_ = ps.getParameter<double>("logWarningEtThreshold_EE_FE");

        tpDigiToken_ = 
          c.consumes<EcalTrigPrimDigiCollection>(ps.getParameter<edm::InputTag>("triggerPrimitiveDigiCollection"));
}


void EcalRecHitWorkerRecover::set(const edm::EventSetup& es)
{
 
        es.get<EcalLaserDbRecord>().get(laser);
        es.get<CaloTopologyRecord>().get(caloTopology_);
        ecalScale_.setEventSetup( es );
        es.get<EcalMappingRcd>().get(pEcalMapping_);
        ecalMapping_ = pEcalMapping_.product();
        // geometry...
        es.get<EcalBarrelGeometryRecord>().get("EcalBarrel",pEBGeom_);
	es.get<CaloGeometryRecord>().get(caloGeometry_);
	es.get<EcalChannelStatusRcd>().get(chStatus_);
        geo_ = caloGeometry_.product();
        ebGeom_ = pEBGeom_.product();
        es.get<IdealGeometryRecord>().get(ttMap_);
        recoveredDetIds_EB_.clear();
        recoveredDetIds_EE_.clear();
	tpgscale_.setEventSetup(es);
	vIphiIeta.clear();
vIphiIeta.push_back(std::make_pair(329, -43));
vIphiIeta.push_back(std::make_pair(109, 53));
vIphiIeta.push_back(std::make_pair(110, 53));
vIphiIeta.push_back(std::make_pair(106, 53));
vIphiIeta.push_back(std::make_pair(107, 53));
vIphiIeta.push_back(std::make_pair(108, 53));
vIphiIeta.push_back(std::make_pair(305, -69));
vIphiIeta.push_back(std::make_pair(169, -26));
vIphiIeta.push_back(std::make_pair(170, -26));
vIphiIeta.push_back(std::make_pair(166, -26));
vIphiIeta.push_back(std::make_pair(167, -26));
vIphiIeta.push_back(std::make_pair(168, -26));
vIphiIeta.push_back(std::make_pair(225, -7));
vIphiIeta.push_back(std::make_pair(108, 54));
vIphiIeta.push_back(std::make_pair(357, 85));
vIphiIeta.push_back(std::make_pair(358, 85));
vIphiIeta.push_back(std::make_pair(107, 54));
vIphiIeta.push_back(std::make_pair(358, 84));
vIphiIeta.push_back(std::make_pair(181, -52));
vIphiIeta.push_back(std::make_pair(182, -52));
vIphiIeta.push_back(std::make_pair(183, -52));
vIphiIeta.push_back(std::make_pair(184, -52));
vIphiIeta.push_back(std::make_pair(185, -52));
vIphiIeta.push_back(std::make_pair(359, 85));
vIphiIeta.push_back(std::make_pair(360, 85));
vIphiIeta.push_back(std::make_pair(341, 72));
vIphiIeta.push_back(std::make_pair(46, 58));
vIphiIeta.push_back(std::make_pair(47, 58));
vIphiIeta.push_back(std::make_pair(48, 58));
vIphiIeta.push_back(std::make_pair(49, 58));
vIphiIeta.push_back(std::make_pair(50, 58));
vIphiIeta.push_back(std::make_pair(224, -7));
vIphiIeta.push_back(std::make_pair(191, -83));
vIphiIeta.push_back(std::make_pair(194, -83));
vIphiIeta.push_back(std::make_pair(345, 72));
vIphiIeta.push_back(std::make_pair(195, -85));
vIphiIeta.push_back(std::make_pair(192, -85));
vIphiIeta.push_back(std::make_pair(191, -85));
vIphiIeta.push_back(std::make_pair(194, -85));
vIphiIeta.push_back(std::make_pair(193, -85));
vIphiIeta.push_back(std::make_pair(115, 10));
vIphiIeta.push_back(std::make_pair(112, 10));
vIphiIeta.push_back(std::make_pair(276, -85));
vIphiIeta.push_back(std::make_pair(277, -85));
vIphiIeta.push_back(std::make_pair(278, -85));
vIphiIeta.push_back(std::make_pair(193, 61));
vIphiIeta.push_back(std::make_pair(194, 61));
vIphiIeta.push_back(std::make_pair(191, 61));
vIphiIeta.push_back(std::make_pair(192, 61));
vIphiIeta.push_back(std::make_pair(195, 61));
vIphiIeta.push_back(std::make_pair(24, -17));
vIphiIeta.push_back(std::make_pair(21, -17));
vIphiIeta.push_back(std::make_pair(55, -6));
vIphiIeta.push_back(std::make_pair(169, -30));
vIphiIeta.push_back(std::make_pair(51, -6));
vIphiIeta.push_back(std::make_pair(52, -6));
vIphiIeta.push_back(std::make_pair(53, -6));
vIphiIeta.push_back(std::make_pair(175, 55));
vIphiIeta.push_back(std::make_pair(112, 6));
vIphiIeta.push_back(std::make_pair(185, -54));
vIphiIeta.push_back(std::make_pair(181, -54));
vIphiIeta.push_back(std::make_pair(182, -54));
vIphiIeta.push_back(std::make_pair(183, -54));
vIphiIeta.push_back(std::make_pair(184, -54));
vIphiIeta.push_back(std::make_pair(167, -30));
vIphiIeta.push_back(std::make_pair(112, 7));
vIphiIeta.push_back(std::make_pair(111, 7));
vIphiIeta.push_back(std::make_pair(114, 7));
vIphiIeta.push_back(std::make_pair(113, 7));
vIphiIeta.push_back(std::make_pair(115, 7));
vIphiIeta.push_back(std::make_pair(305, -70));
vIphiIeta.push_back(std::make_pair(301, -70));
vIphiIeta.push_back(std::make_pair(302, -70));
vIphiIeta.push_back(std::make_pair(303, -70));
vIphiIeta.push_back(std::make_pair(304, -70));
vIphiIeta.push_back(std::make_pair(224, -6));
vIphiIeta.push_back(std::make_pair(223, -6));
vIphiIeta.push_back(std::make_pair(222, -6));
vIphiIeta.push_back(std::make_pair(221, -6));
vIphiIeta.push_back(std::make_pair(225, -6));
vIphiIeta.push_back(std::make_pair(73, 69));
vIphiIeta.push_back(std::make_pair(65, -5));
vIphiIeta.push_back(std::make_pair(360, 81));
vIphiIeta.push_back(std::make_pair(359, 81));
vIphiIeta.push_back(std::make_pair(62, -5));
vIphiIeta.push_back(std::make_pair(61, -5));
vIphiIeta.push_back(std::make_pair(64, -5));
vIphiIeta.push_back(std::make_pair(63, -5));
vIphiIeta.push_back(std::make_pair(358, 81));
vIphiIeta.push_back(std::make_pair(357, 81));
vIphiIeta.push_back(std::make_pair(356, 81));
vIphiIeta.push_back(std::make_pair(106, 54));
vIphiIeta.push_back(std::make_pair(216, -74));
vIphiIeta.push_back(std::make_pair(217, -74));
vIphiIeta.push_back(std::make_pair(218, -74));
vIphiIeta.push_back(std::make_pair(219, -74));
vIphiIeta.push_back(std::make_pair(220, -74));
vIphiIeta.push_back(std::make_pair(127, -52));
vIphiIeta.push_back(std::make_pair(128, -52));
vIphiIeta.push_back(std::make_pair(331, -66));
vIphiIeta.push_back(std::make_pair(126, -52));
vIphiIeta.push_back(std::make_pair(169, 82));
vIphiIeta.push_back(std::make_pair(170, 82));
vIphiIeta.push_back(std::make_pair(166, 82));
vIphiIeta.push_back(std::make_pair(167, 82));
vIphiIeta.push_back(std::make_pair(168, 82));
vIphiIeta.push_back(std::make_pair(192, -84));
vIphiIeta.push_back(std::make_pair(191, -84));
vIphiIeta.push_back(std::make_pair(194, -84));
vIphiIeta.push_back(std::make_pair(193, -84));
vIphiIeta.push_back(std::make_pair(303, -66));
vIphiIeta.push_back(std::make_pair(304, -66));
vIphiIeta.push_back(std::make_pair(225, -10));
vIphiIeta.push_back(std::make_pair(223, -10));
vIphiIeta.push_back(std::make_pair(224, -10));
vIphiIeta.push_back(std::make_pair(221, -10));
vIphiIeta.push_back(std::make_pair(222, -10));
vIphiIeta.push_back(std::make_pair(115, 9));
vIphiIeta.push_back(std::make_pair(112, 9));
vIphiIeta.push_back(std::make_pair(111, 9));
vIphiIeta.push_back(std::make_pair(114, 9));
vIphiIeta.push_back(std::make_pair(113, 9));
vIphiIeta.push_back(std::make_pair(328, -43));
vIphiIeta.push_back(std::make_pair(327, -43));
vIphiIeta.push_back(std::make_pair(326, -43));
vIphiIeta.push_back(std::make_pair(62, -3));
vIphiIeta.push_back(std::make_pair(61, -3));
vIphiIeta.push_back(std::make_pair(64, -3));
vIphiIeta.push_back(std::make_pair(63, -3));
vIphiIeta.push_back(std::make_pair(191, -82));
vIphiIeta.push_back(std::make_pair(192, -82));
vIphiIeta.push_back(std::make_pair(175, 52));
vIphiIeta.push_back(std::make_pair(194, -82));
vIphiIeta.push_back(std::make_pair(195, -82));
vIphiIeta.push_back(std::make_pair(174, 52));
vIphiIeta.push_back(std::make_pair(330, -43));
vIphiIeta.push_back(std::make_pair(172, 52));
vIphiIeta.push_back(std::make_pair(19, -14));
vIphiIeta.push_back(std::make_pair(16, -11));
vIphiIeta.push_back(std::make_pair(17, -11));
vIphiIeta.push_back(std::make_pair(18, -11));
vIphiIeta.push_back(std::make_pair(19, -11));
vIphiIeta.push_back(std::make_pair(20, -11));
vIphiIeta.push_back(std::make_pair(17, -14));
vIphiIeta.push_back(std::make_pair(18, -14));
vIphiIeta.push_back(std::make_pair(16, -14));
vIphiIeta.push_back(std::make_pair(55, -8));
vIphiIeta.push_back(std::make_pair(356, 85));
vIphiIeta.push_back(std::make_pair(171, 52));
vIphiIeta.push_back(std::make_pair(113, 10));
vIphiIeta.push_back(std::make_pair(243, -43));
vIphiIeta.push_back(std::make_pair(244, -43));
vIphiIeta.push_back(std::make_pair(241, -43));
vIphiIeta.push_back(std::make_pair(242, -43));
vIphiIeta.push_back(std::make_pair(245, -43));
vIphiIeta.push_back(std::make_pair(192, 64));
vIphiIeta.push_back(std::make_pair(170, -28));
vIphiIeta.push_back(std::make_pair(169, -28));
vIphiIeta.push_back(std::make_pair(4, -76));
vIphiIeta.push_back(std::make_pair(3, -76));
vIphiIeta.push_back(std::make_pair(2, -76));
vIphiIeta.push_back(std::make_pair(1, -76));
vIphiIeta.push_back(std::make_pair(166, -28));
vIphiIeta.push_back(std::make_pair(240, -45));
vIphiIeta.push_back(std::make_pair(168, -28));
vIphiIeta.push_back(std::make_pair(5, -76));
vIphiIeta.push_back(std::make_pair(54, -7));
vIphiIeta.push_back(std::make_pair(53, -7));
vIphiIeta.push_back(std::make_pair(52, -7));
vIphiIeta.push_back(std::make_pair(51, -7));
vIphiIeta.push_back(std::make_pair(55, -7));
vIphiIeta.push_back(std::make_pair(225, 83));
vIphiIeta.push_back(std::make_pair(191, 64));
vIphiIeta.push_back(std::make_pair(175, 51));
vIphiIeta.push_back(std::make_pair(221, 83));
vIphiIeta.push_back(std::make_pair(222, 83));
vIphiIeta.push_back(std::make_pair(223, 83));
vIphiIeta.push_back(std::make_pair(224, 83));
vIphiIeta.push_back(std::make_pair(241, -45));
vIphiIeta.push_back(std::make_pair(194, 64));
vIphiIeta.push_back(std::make_pair(302, -67));
vIphiIeta.push_back(std::make_pair(175, 54));
vIphiIeta.push_back(std::make_pair(304, -67));
vIphiIeta.push_back(std::make_pair(303, -67));
vIphiIeta.push_back(std::make_pair(172, 54));
vIphiIeta.push_back(std::make_pair(305, -67));
vIphiIeta.push_back(std::make_pair(174, 54));
vIphiIeta.push_back(std::make_pair(173, 54));
vIphiIeta.push_back(std::make_pair(280, -81));
vIphiIeta.push_back(std::make_pair(130, -54));
vIphiIeta.push_back(std::make_pair(129, -54));
vIphiIeta.push_back(std::make_pair(359, 84));
vIphiIeta.push_back(std::make_pair(360, 84));
vIphiIeta.push_back(std::make_pair(357, 84));
vIphiIeta.push_back(std::make_pair(343, 72));
vIphiIeta.push_back(std::make_pair(342, 72));
vIphiIeta.push_back(std::make_pair(356, 84));
vIphiIeta.push_back(std::make_pair(126, -54));
vIphiIeta.push_back(std::make_pair(193, -82));
vIphiIeta.push_back(std::make_pair(128, -54));
vIphiIeta.push_back(std::make_pair(127, -54));
vIphiIeta.push_back(std::make_pair(191, -81));
vIphiIeta.push_back(std::make_pair(173, 52));
vIphiIeta.push_back(std::make_pair(172, 55));
vIphiIeta.push_back(std::make_pair(171, 55));
vIphiIeta.push_back(std::make_pair(174, 55));
vIphiIeta.push_back(std::make_pair(173, 55));
vIphiIeta.push_back(std::make_pair(169, 81));
vIphiIeta.push_back(std::make_pair(170, 81));
vIphiIeta.push_back(std::make_pair(166, 81));
vIphiIeta.push_back(std::make_pair(167, 81));
vIphiIeta.push_back(std::make_pair(168, 81));
vIphiIeta.push_back(std::make_pair(193, -83));
vIphiIeta.push_back(std::make_pair(330, -45));
vIphiIeta.push_back(std::make_pair(329, -45));
vIphiIeta.push_back(std::make_pair(129, -52));
vIphiIeta.push_back(std::make_pair(130, -52));
vIphiIeta.push_back(std::make_pair(305, -66));
vIphiIeta.push_back(std::make_pair(342, 75));
vIphiIeta.push_back(std::make_pair(343, 75));
vIphiIeta.push_back(std::make_pair(344, 75));
vIphiIeta.push_back(std::make_pair(301, -66));
vIphiIeta.push_back(std::make_pair(302, -66));
vIphiIeta.push_back(std::make_pair(328, -45));
vIphiIeta.push_back(std::make_pair(327, -45));
vIphiIeta.push_back(std::make_pair(20, -14));
vIphiIeta.push_back(std::make_pair(275, -50));
vIphiIeta.push_back(std::make_pair(274, -50));
vIphiIeta.push_back(std::make_pair(273, -50));
vIphiIeta.push_back(std::make_pair(272, -50));
vIphiIeta.push_back(std::make_pair(271, -50));
vIphiIeta.push_back(std::make_pair(245, -44));
vIphiIeta.push_back(std::make_pair(244, -44));
vIphiIeta.push_back(std::make_pair(243, -44));
vIphiIeta.push_back(std::make_pair(242, -44));
vIphiIeta.push_back(std::make_pair(241, -44));
vIphiIeta.push_back(std::make_pair(219, -73));
vIphiIeta.push_back(std::make_pair(220, -73));
vIphiIeta.push_back(std::make_pair(216, -73));
vIphiIeta.push_back(std::make_pair(217, -73));
vIphiIeta.push_back(std::make_pair(344, 72));
vIphiIeta.push_back(std::make_pair(170, -27));
vIphiIeta.push_back(std::make_pair(190, -85));
vIphiIeta.push_back(std::make_pair(175, 53));
vIphiIeta.push_back(std::make_pair(171, 85));
vIphiIeta.push_back(std::make_pair(172, 53));
vIphiIeta.push_back(std::make_pair(171, 53));
vIphiIeta.push_back(std::make_pair(174, 53));
vIphiIeta.push_back(std::make_pair(173, 53));
vIphiIeta.push_back(std::make_pair(326, -45));
vIphiIeta.push_back(std::make_pair(111, 10));
vIphiIeta.push_back(std::make_pair(114, 10));
vIphiIeta.push_back(std::make_pair(344, 71));
vIphiIeta.push_back(std::make_pair(343, 71));
vIphiIeta.push_back(std::make_pair(342, 71));
vIphiIeta.push_back(std::make_pair(225, 81));
vIphiIeta.push_back(std::make_pair(224, 81));
vIphiIeta.push_back(std::make_pair(223, 81));
vIphiIeta.push_back(std::make_pair(222, 81));
vIphiIeta.push_back(std::make_pair(221, 81));
vIphiIeta.push_back(std::make_pair(19, -12));
vIphiIeta.push_back(std::make_pair(20, -12));
vIphiIeta.push_back(std::make_pair(25, -20));
vIphiIeta.push_back(std::make_pair(21, -20));
vIphiIeta.push_back(std::make_pair(22, -20));
vIphiIeta.push_back(std::make_pair(23, -20));
vIphiIeta.push_back(std::make_pair(18, -12));
vIphiIeta.push_back(std::make_pair(25, -18));
vIphiIeta.push_back(std::make_pair(23, -18));
vIphiIeta.push_back(std::make_pair(24, -18));
vIphiIeta.push_back(std::make_pair(21, -18));
vIphiIeta.push_back(std::make_pair(22, -18));
vIphiIeta.push_back(std::make_pair(111, 6));
vIphiIeta.push_back(std::make_pair(106, 55));
vIphiIeta.push_back(std::make_pair(108, 55));
vIphiIeta.push_back(std::make_pair(107, 55));
vIphiIeta.push_back(std::make_pair(110, 55));
vIphiIeta.push_back(std::make_pair(109, 55));
vIphiIeta.push_back(std::make_pair(341, 73));
vIphiIeta.push_back(std::make_pair(342, 73));
vIphiIeta.push_back(std::make_pair(343, 73));
vIphiIeta.push_back(std::make_pair(344, 73));
vIphiIeta.push_back(std::make_pair(345, 73));
vIphiIeta.push_back(std::make_pair(41, 4));
vIphiIeta.push_back(std::make_pair(106, 51));
vIphiIeta.push_back(std::make_pair(50, 60));
vIphiIeta.push_back(std::make_pair(49, 60));
vIphiIeta.push_back(std::make_pair(48, 60));
vIphiIeta.push_back(std::make_pair(47, 60));
vIphiIeta.push_back(std::make_pair(46, 60));
vIphiIeta.push_back(std::make_pair(257, -64));
vIphiIeta.push_back(std::make_pair(258, -64));
vIphiIeta.push_back(std::make_pair(256, -64));
vIphiIeta.push_back(std::make_pair(259, -64));
vIphiIeta.push_back(std::make_pair(260, -64));
vIphiIeta.push_back(std::make_pair(360, 82));
vIphiIeta.push_back(std::make_pair(359, 82));
vIphiIeta.push_back(std::make_pair(356, 82));
vIphiIeta.push_back(std::make_pair(358, 82));
vIphiIeta.push_back(std::make_pair(357, 82));
vIphiIeta.push_back(std::make_pair(170, 83));
vIphiIeta.push_back(std::make_pair(169, 83));
vIphiIeta.push_back(std::make_pair(218, -73));
vIphiIeta.push_back(std::make_pair(166, 83));
vIphiIeta.push_back(std::make_pair(168, 83));
vIphiIeta.push_back(std::make_pair(167, 83));
vIphiIeta.push_back(std::make_pair(342, 74));
vIphiIeta.push_back(std::make_pair(341, 74));
vIphiIeta.push_back(std::make_pair(344, 74));
vIphiIeta.push_back(std::make_pair(343, 74));
vIphiIeta.push_back(std::make_pair(345, 74));
vIphiIeta.push_back(std::make_pair(185, -53));
vIphiIeta.push_back(std::make_pair(274, -46));
vIphiIeta.push_back(std::make_pair(181, -53));
vIphiIeta.push_back(std::make_pair(182, -53));
vIphiIeta.push_back(std::make_pair(183, -53));
vIphiIeta.push_back(std::make_pair(184, -53));
vIphiIeta.push_back(std::make_pair(280, -83));
vIphiIeta.push_back(std::make_pair(16, -15));
vIphiIeta.push_back(std::make_pair(18, -15));
vIphiIeta.push_back(std::make_pair(17, -15));
vIphiIeta.push_back(std::make_pair(51, -8));
vIphiIeta.push_back(std::make_pair(52, -8));
vIphiIeta.push_back(std::make_pair(53, -8));
vIphiIeta.push_back(std::make_pair(54, -8));
vIphiIeta.push_back(std::make_pair(20, -15));
vIphiIeta.push_back(std::make_pair(19, -15));
vIphiIeta.push_back(std::make_pair(110, 54));
vIphiIeta.push_back(std::make_pair(109, 54));
vIphiIeta.push_back(std::make_pair(243, -41));
vIphiIeta.push_back(std::make_pair(244, -41));
vIphiIeta.push_back(std::make_pair(241, -41));
vIphiIeta.push_back(std::make_pair(221, -9));
vIphiIeta.push_back(std::make_pair(225, 82));
vIphiIeta.push_back(std::make_pair(245, -41));
vIphiIeta.push_back(std::make_pair(225, -9));
vIphiIeta.push_back(std::make_pair(25, -19));
vIphiIeta.push_back(std::make_pair(22, -19));
vIphiIeta.push_back(std::make_pair(21, -19));
vIphiIeta.push_back(std::make_pair(24, -19));
vIphiIeta.push_back(std::make_pair(23, -19));
vIphiIeta.push_back(std::make_pair(185, -55));
vIphiIeta.push_back(std::make_pair(182, -55));
vIphiIeta.push_back(std::make_pair(181, -55));
vIphiIeta.push_back(std::make_pair(184, -55));
vIphiIeta.push_back(std::make_pair(183, -55));
vIphiIeta.push_back(std::make_pair(166, -27));
vIphiIeta.push_back(std::make_pair(167, -27));
vIphiIeta.push_back(std::make_pair(168, -27));
vIphiIeta.push_back(std::make_pair(330, -44));
vIphiIeta.push_back(std::make_pair(329, -44));
vIphiIeta.push_back(std::make_pair(328, -44));
vIphiIeta.push_back(std::make_pair(65, -1));
vIphiIeta.push_back(std::make_pair(326, -44));
vIphiIeta.push_back(std::make_pair(62, -1));
vIphiIeta.push_back(std::make_pair(61, -2));
vIphiIeta.push_back(std::make_pair(64, -1));
vIphiIeta.push_back(std::make_pair(63, -1));
vIphiIeta.push_back(std::make_pair(224, -9));
vIphiIeta.push_back(std::make_pair(223, -9));
vIphiIeta.push_back(std::make_pair(222, -9));
vIphiIeta.push_back(std::make_pair(242, -41));
vIphiIeta.push_back(std::make_pair(48, 61));
vIphiIeta.push_back(std::make_pair(41, 5));
vIphiIeta.push_back(std::make_pair(326, -41));
vIphiIeta.push_back(std::make_pair(341, 71));
vIphiIeta.push_back(std::make_pair(327, -41));
vIphiIeta.push_back(std::make_pair(49, 59));
vIphiIeta.push_back(std::make_pair(50, 59));
vIphiIeta.push_back(std::make_pair(345, 71));
vIphiIeta.push_back(std::make_pair(46, 59));
vIphiIeta.push_back(std::make_pair(47, 59));
vIphiIeta.push_back(std::make_pair(48, 59));
vIphiIeta.push_back(std::make_pair(49, 56));
vIphiIeta.push_back(std::make_pair(50, 56));
vIphiIeta.push_back(std::make_pair(275, -48));
vIphiIeta.push_back(std::make_pair(221, -7));
vIphiIeta.push_back(std::make_pair(222, -7));
vIphiIeta.push_back(std::make_pair(223, -7));
vIphiIeta.push_back(std::make_pair(271, -48));
vIphiIeta.push_back(std::make_pair(274, -48));
vIphiIeta.push_back(std::make_pair(25, -17));
vIphiIeta.push_back(std::make_pair(23, -17));
vIphiIeta.push_back(std::make_pair(46, 56));
vIphiIeta.push_back(std::make_pair(47, 56));
vIphiIeta.push_back(std::make_pair(22, -17));
vIphiIeta.push_back(std::make_pair(106, 52));
vIphiIeta.push_back(std::make_pair(107, 52));
vIphiIeta.push_back(std::make_pair(108, 52));
vIphiIeta.push_back(std::make_pair(109, 52));
vIphiIeta.push_back(std::make_pair(110, 52));
vIphiIeta.push_back(std::make_pair(273, -48));
vIphiIeta.push_back(std::make_pair(193, 62));
vIphiIeta.push_back(std::make_pair(194, 62));
vIphiIeta.push_back(std::make_pair(191, 62));
vIphiIeta.push_back(std::make_pair(192, 62));
vIphiIeta.push_back(std::make_pair(195, 62));
vIphiIeta.push_back(std::make_pair(272, -48));
vIphiIeta.push_back(std::make_pair(16, -12));
vIphiIeta.push_back(std::make_pair(17, -12));
vIphiIeta.push_back(std::make_pair(24, -20));
vIphiIeta.push_back(std::make_pair(243, -45));
vIphiIeta.push_back(std::make_pair(244, -45));
vIphiIeta.push_back(std::make_pair(193, 64));
vIphiIeta.push_back(std::make_pair(242, -45));
vIphiIeta.push_back(std::make_pair(195, 64));
vIphiIeta.push_back(std::make_pair(245, -45));
vIphiIeta.push_back(std::make_pair(113, 6));
vIphiIeta.push_back(std::make_pair(166, 85));
vIphiIeta.push_back(std::make_pair(167, 85));
vIphiIeta.push_back(std::make_pair(168, 85));
vIphiIeta.push_back(std::make_pair(115, 6));
vIphiIeta.push_back(std::make_pair(54, -6));
vIphiIeta.push_back(std::make_pair(169, 85));
vIphiIeta.push_back(std::make_pair(170, 85));
vIphiIeta.push_back(std::make_pair(330, -66));
vIphiIeta.push_back(std::make_pair(22, -16));
vIphiIeta.push_back(std::make_pair(21, -16));
vIphiIeta.push_back(std::make_pair(52, -9));
vIphiIeta.push_back(std::make_pair(51, -9));
vIphiIeta.push_back(std::make_pair(54, -9));
vIphiIeta.push_back(std::make_pair(53, -9));
vIphiIeta.push_back(std::make_pair(55, -9));
vIphiIeta.push_back(std::make_pair(111, 8));
vIphiIeta.push_back(std::make_pair(112, 8));
vIphiIeta.push_back(std::make_pair(113, 8));
vIphiIeta.push_back(std::make_pair(114, 8));
vIphiIeta.push_back(std::make_pair(115, 8));
vIphiIeta.push_back(std::make_pair(328, -41));
vIphiIeta.push_back(std::make_pair(327, -44));
vIphiIeta.push_back(std::make_pair(330, -41));
vIphiIeta.push_back(std::make_pair(329, -41));
vIphiIeta.push_back(std::make_pair(62, -2));
vIphiIeta.push_back(std::make_pair(61, -1));
vIphiIeta.push_back(std::make_pair(64, -2));
vIphiIeta.push_back(std::make_pair(63, -2));
vIphiIeta.push_back(std::make_pair(194, 63));
vIphiIeta.push_back(std::make_pair(192, 63));
vIphiIeta.push_back(std::make_pair(191, 63));
vIphiIeta.push_back(std::make_pair(181, -51));
vIphiIeta.push_back(std::make_pair(182, -51));
vIphiIeta.push_back(std::make_pair(183, -51));
vIphiIeta.push_back(std::make_pair(184, -51));
vIphiIeta.push_back(std::make_pair(185, -51));
vIphiIeta.push_back(std::make_pair(167, -28));
vIphiIeta.push_back(std::make_pair(191, 65));
vIphiIeta.push_back(std::make_pair(192, 65));
vIphiIeta.push_back(std::make_pair(193, 65));
vIphiIeta.push_back(std::make_pair(194, 65));
vIphiIeta.push_back(std::make_pair(195, 65));
vIphiIeta.push_back(std::make_pair(35, 51));
vIphiIeta.push_back(std::make_pair(49, 57));
vIphiIeta.push_back(std::make_pair(50, 57));
vIphiIeta.push_back(std::make_pair(65, -3));
vIphiIeta.push_back(std::make_pair(301, -69));
vIphiIeta.push_back(std::make_pair(302, -69));
vIphiIeta.push_back(std::make_pair(303, -69));
vIphiIeta.push_back(std::make_pair(304, -69));
vIphiIeta.push_back(std::make_pair(195, -83));
vIphiIeta.push_back(std::make_pair(192, -83));
vIphiIeta.push_back(std::make_pair(46, 57));
vIphiIeta.push_back(std::make_pair(47, 57));
vIphiIeta.push_back(std::make_pair(48, 57));
vIphiIeta.push_back(std::make_pair(170, 84));
vIphiIeta.push_back(std::make_pair(217, -75));
vIphiIeta.push_back(std::make_pair(218, -75));
vIphiIeta.push_back(std::make_pair(216, -75));
vIphiIeta.push_back(std::make_pair(219, -75));
vIphiIeta.push_back(std::make_pair(220, -75));
vIphiIeta.push_back(std::make_pair(341, 75));
vIphiIeta.push_back(std::make_pair(61, -4));
vIphiIeta.push_back(std::make_pair(62, -4));
vIphiIeta.push_back(std::make_pair(63, -4));
vIphiIeta.push_back(std::make_pair(64, -4));
vIphiIeta.push_back(std::make_pair(65, -4));
vIphiIeta.push_back(std::make_pair(305, -68));
vIphiIeta.push_back(std::make_pair(301, -68));
vIphiIeta.push_back(std::make_pair(302, -68));
vIphiIeta.push_back(std::make_pair(303, -68));
vIphiIeta.push_back(std::make_pair(304, -68));
vIphiIeta.push_back(std::make_pair(273, -49));
vIphiIeta.push_back(std::make_pair(274, -49));
vIphiIeta.push_back(std::make_pair(271, -49));
vIphiIeta.push_back(std::make_pair(272, -49));
vIphiIeta.push_back(std::make_pair(275, -49));
vIphiIeta.push_back(std::make_pair(243, -42));
vIphiIeta.push_back(std::make_pair(244, -42));
vIphiIeta.push_back(std::make_pair(241, -42));
vIphiIeta.push_back(std::make_pair(242, -42));
vIphiIeta.push_back(std::make_pair(245, -42));
vIphiIeta.push_back(std::make_pair(166, 84));
vIphiIeta.push_back(std::make_pair(170, -30));
vIphiIeta.push_back(std::make_pair(168, 84));
vIphiIeta.push_back(std::make_pair(174, 51));
vIphiIeta.push_back(std::make_pair(167, 84));
vIphiIeta.push_back(std::make_pair(173, 51));
vIphiIeta.push_back(std::make_pair(195, -84));
vIphiIeta.push_back(std::make_pair(192, -81));
vIphiIeta.push_back(std::make_pair(193, -81));
vIphiIeta.push_back(std::make_pair(194, -81));
vIphiIeta.push_back(std::make_pair(195, -81));
vIphiIeta.push_back(std::make_pair(172, 51));
vIphiIeta.push_back(std::make_pair(74, 68));
vIphiIeta.push_back(std::make_pair(171, 51));
vIphiIeta.push_back(std::make_pair(330, -42));
vIphiIeta.push_back(std::make_pair(329, -42));
vIphiIeta.push_back(std::make_pair(328, -42));
vIphiIeta.push_back(std::make_pair(327, -42));
vIphiIeta.push_back(std::make_pair(326, -42));
vIphiIeta.push_back(std::make_pair(225, -8));
vIphiIeta.push_back(std::make_pair(128, -51));
vIphiIeta.push_back(std::make_pair(345, 75));
vIphiIeta.push_back(std::make_pair(127, -51));
vIphiIeta.push_back(std::make_pair(169, -27));
vIphiIeta.push_back(std::make_pair(107, 51));
vIphiIeta.push_back(std::make_pair(108, 51));
vIphiIeta.push_back(std::make_pair(48, 56));
vIphiIeta.push_back(std::make_pair(275, -46));
vIphiIeta.push_back(std::make_pair(271, -46));
vIphiIeta.push_back(std::make_pair(272, -46));
vIphiIeta.push_back(std::make_pair(109, 51));
vIphiIeta.push_back(std::make_pair(110, 51));
vIphiIeta.push_back(std::make_pair(167, -29));
vIphiIeta.push_back(std::make_pair(168, -29));
vIphiIeta.push_back(std::make_pair(166, -29));
vIphiIeta.push_back(std::make_pair(169, -29));
vIphiIeta.push_back(std::make_pair(170, -29));
vIphiIeta.push_back(std::make_pair(128, -55));
vIphiIeta.push_back(std::make_pair(127, -55));
vIphiIeta.push_back(std::make_pair(126, -55));
vIphiIeta.push_back(std::make_pair(130, -55));
vIphiIeta.push_back(std::make_pair(129, -55));
vIphiIeta.push_back(std::make_pair(223, 82));
vIphiIeta.push_back(std::make_pair(224, 82));
vIphiIeta.push_back(std::make_pair(221, 82));
vIphiIeta.push_back(std::make_pair(222, 82));
vIphiIeta.push_back(std::make_pair(273, -46));
vIphiIeta.push_back(std::make_pair(166, -30));
vIphiIeta.push_back(std::make_pair(275, -47));
vIphiIeta.push_back(std::make_pair(274, -47));
vIphiIeta.push_back(std::make_pair(273, -47));
vIphiIeta.push_back(std::make_pair(272, -47));
vIphiIeta.push_back(std::make_pair(271, -47));
vIphiIeta.push_back(std::make_pair(279, -82));
vIphiIeta.push_back(std::make_pair(130, -51));
vIphiIeta.push_back(std::make_pair(129, -51));
vIphiIeta.push_back(std::make_pair(130, -53));
vIphiIeta.push_back(std::make_pair(129, -53));
vIphiIeta.push_back(std::make_pair(24, -16));
vIphiIeta.push_back(std::make_pair(23, -16));
vIphiIeta.push_back(std::make_pair(25, -16));
vIphiIeta.push_back(std::make_pair(169, 84));
vIphiIeta.push_back(std::make_pair(195, 63));
vIphiIeta.push_back(std::make_pair(126, -53));
vIphiIeta.push_back(std::make_pair(193, 63));
vIphiIeta.push_back(std::make_pair(128, -53));
vIphiIeta.push_back(std::make_pair(127, -53));
vIphiIeta.push_back(std::make_pair(33, 51));
vIphiIeta.push_back(std::make_pair(34, 51));
vIphiIeta.push_back(std::make_pair(31, 51));
vIphiIeta.push_back(std::make_pair(32, 51));
vIphiIeta.push_back(std::make_pair(168, -30));
vIphiIeta.push_back(std::make_pair(301, -67));
vIphiIeta.push_back(std::make_pair(52, -10));
vIphiIeta.push_back(std::make_pair(51, -10));
vIphiIeta.push_back(std::make_pair(54, -10));
vIphiIeta.push_back(std::make_pair(53, -10));
vIphiIeta.push_back(std::make_pair(55, -10));
vIphiIeta.push_back(std::make_pair(114, 6));
vIphiIeta.push_back(std::make_pair(224, -8));
vIphiIeta.push_back(std::make_pair(223, -8));
vIphiIeta.push_back(std::make_pair(222, -8));
vIphiIeta.push_back(std::make_pair(221, -8));
vIphiIeta.push_back(std::make_pair(171, 54));
vIphiIeta.push_back(std::make_pair(357, 83));
vIphiIeta.push_back(std::make_pair(358, 83));
vIphiIeta.push_back(std::make_pair(126, -51));
vIphiIeta.push_back(std::make_pair(356, 83));
vIphiIeta.push_back(std::make_pair(359, 83));
vIphiIeta.push_back(std::make_pair(360, 83));
vIphiIeta.push_back(std::make_pair(19, -13));
vIphiIeta.push_back(std::make_pair(20, -13));
vIphiIeta.push_back(std::make_pair(16, -13));
vIphiIeta.push_back(std::make_pair(17, -13));
vIphiIeta.push_back(std::make_pair(18, -13));
vIphiIeta.push_back(std::make_pair(65, -2));


}


bool
EcalRecHitWorkerRecover::run( const edm::Event & evt, 
                const EcalUncalibratedRecHit& uncalibRH,
                EcalRecHitCollection & result)
{
        DetId detId=uncalibRH.id();
	uint32_t flags = (0xF & uncalibRH.flags()); 

        // get laser coefficient
        //float lasercalib = laser->getLaserCorrection( detId, evt.time());

        // killDeadChannels_ = true, means explicitely kill dead channels even if the recovered energies are computed in the code
        // if you don't want to store the recovered energies in the rechit you can produce LogWarnings if logWarningEtThreshold_EB(EE)_FE>0 
	// logWarningEtThreshold_EB(EE)_FE_<0 will not compute the recovered energies at all (faster)

        if ( killDeadChannels_ ) {
                if (    (flags == EcalRecHitWorkerRecover::EB_single && !recoverEBIsolatedChannels_)
                     || (flags == EcalRecHitWorkerRecover::EE_single && !recoverEEIsolatedChannels_)
                     || (flags == EcalRecHitWorkerRecover::EB_VFE && !recoverEBVFE_)
                     || (flags == EcalRecHitWorkerRecover::EE_VFE && !recoverEEVFE_)
                     ) {
                        EcalRecHit hit( detId, 0., 0., EcalRecHit::kDead );
                        hit.setFlag( EcalRecHit::kDead)  ;
                        insertRecHit( hit, result); // insert trivial rechit with kDead flag
                        return true;
                } 
                if ( flags == EcalRecHitWorkerRecover::EB_FE && !recoverEBFE_) {
                        EcalTrigTowerDetId ttDetId( ((EBDetId)detId).tower() );
                        std::vector<DetId> vid = ttMap_->constituentsOf( ttDetId );
                        for ( std::vector<DetId>::const_iterator dit = vid.begin(); dit != vid.end(); ++dit ) {
                                EcalRecHit hit( (*dit), 0., 0., EcalRecHit::kDead );
                                hit.setFlag( EcalRecHit::kDead ) ;
                                insertRecHit( hit, result ); // insert trivial rechit with kDead flag
                        }
			if(logWarningEtThreshold_EB_FE_<0)return true; // if you don't want log warning just return true
                }
                if ( flags == EcalRecHitWorkerRecover::EE_FE && !recoverEEFE_) {
                        EEDetId id( detId );
                        EcalScDetId sc( 1+(id.ix()-1)/5, 1+(id.iy()-1)/5, id.zside() );
                        std::vector<DetId> eeC;
                        for(int dx=1; dx<=5; ++dx){
                                for(int dy=1; dy<=5; ++dy){
                                        int ix = (sc.ix()-1)*5 + dx;
                                        int iy = (sc.iy()-1)*5 + dy;
                                        int iz = sc.zside();
                                        if(EEDetId::validDetId(ix, iy, iz)){
                                                eeC.push_back(EEDetId(ix, iy, iz));
                                        }
                                }
                        }
                        for ( size_t i = 0; i < eeC.size(); ++i ) {
                                EcalRecHit hit( eeC[i], 0., 0., EcalRecHit::kDead );
                                hit.setFlag( EcalRecHit::kDead ) ;
                                insertRecHit( hit, result ); // insert trivial rechit with kDead flag
                        }
		   	if(logWarningEtThreshold_EE_FE_<0)   return true; // if you don't want log warning just return true
                }
        }

        if ( flags == EcalRecHitWorkerRecover::EB_single ) {
                    // recover as single dead channel
                    ebDeadChannelCorrector.setCaloTopology(caloTopology_.product());

                    // channel recovery. Accepted new RecHit has the flag AcceptRecHit=TRUE
		    //std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << " "<< singleRecoveryThreshold_<< std::endl;
                    bool AcceptRecHit=true;
                    //EcalRecHit hit = ebDeadChannelCorrector.correct( detId, result, singleRecoveryMethod_, singleRecoveryThreshold_, &AcceptRecHit);
		    EBDetId ebDetId( detId );
		    int myieta=ebDetId.ieta();
		    int myiphi=ebDetId.iphi();

		    std::pair<int,int> myiphieta;
		    //myiphieta= std::make_pair<myiphi,myieta>;
		    bool checkPair=false;
		    for (uint i =0 ; i<vIphiIeta.size(); i++){
		      if (vIphiIeta[i].first==myiphi && vIphiIeta[i].second==myieta) checkPair=true;
		      
		    }
		    EcalRecHit hit ( detId, 0., 0., 0 );
		    AcceptRecHit=false;
		    if (checkPair==false){
		      hit = ebDeadChannelCorrector.correct( detId, result, singleRecoveryMethod_, singleRecoveryThreshold_, &AcceptRecHit);
		    }
 
		    if( DEBUG_)std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << " recovered energy "<<  hit.energy() << " accept flag " << AcceptRecHit << std::endl ; 
                    if ( hit.energy() > 0. && AcceptRecHit == true ) {
                       hit.setFlag( EcalRecHit::kNeighboursRecovered ) ;
		       if( DEBUG_)std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << " recovered energy "<<  hit.energy() << " accept flag " << AcceptRecHit<< " hit flag "<< hit.checkFlag(EcalRecHit::kNeighboursRecovered ) << std::endl ; 
                    } else {
                        // recovery failed
                        hit.setFlag( EcalRecHit::kDead ) ;
			if( DEBUG_)std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << " recovery FAILED " <<  hit.checkFlag(EcalRecHit::kDead ) << std::endl;
                    }
                    insertRecHit( hit, result );
                
        } else if ( flags == EcalRecHitWorkerRecover::EE_single ) {
                    // recover as single dead channel
		    eeDeadChannelCorrector.setCaloTopology(caloTopology_.product());

                    // channel recovery. Accepted new RecHit has the flag AcceptRecHit=TRUE
                    bool AcceptRecHit=true;
                    EcalRecHit hit = eeDeadChannelCorrector.correct( detId, result, singleRecoveryMethod_, singleRecoveryThreshold_, &AcceptRecHit);
                    if ( hit.energy() != 0 && AcceptRecHit == true ) {
                        hit.setFlag( EcalRecHit::kNeighboursRecovered ) ;
		    } else {
                       // recovery failed
                       hit.setFlag( EcalRecHit::kDead ) ;
                    }
                    insertRecHit( hit, result );
                
        } else if ( flags == EcalRecHitWorkerRecover::EB_VFE ) {
                // recover as dead VFE
                EcalRecHit hit( detId, 0., 0.);
                hit.setFlag( EcalRecHit::kDead ) ;
                // recovery not implemented
                insertRecHit( hit, result );
        } else if ( flags == EcalRecHitWorkerRecover::EB_FE ) {
                // recover as dead TT

                EcalTrigTowerDetId ttDetId( ((EBDetId)detId).tower() );
                edm::Handle<EcalTrigPrimDigiCollection> pTPDigis;
                evt.getByToken(tpDigiToken_, pTPDigis);
                const EcalTrigPrimDigiCollection * tpDigis = nullptr;               
		tpDigis = pTPDigis.product();
           
                EcalTrigPrimDigiCollection::const_iterator tp = tpDigis->find( ttDetId );
                // recover the whole trigger tower
                if ( tp != tpDigis->end() ) {
                        //std::vector<DetId> vid = ecalMapping_->dccTowerConstituents( ecalMapping_->DCCid( ttDetId ), ecalMapping_->iTT( ttDetId ) );
                        std::vector<DetId> vid = ttMap_->constituentsOf( ttDetId );
                        float tpEt  = ecalScale_.getTPGInGeV( tp->compressedEt(), tp->id() );
                        float tpEtThreshEB = logWarningEtThreshold_EB_FE_;
                        if(tpEt>tpEtThreshEB){
                                edm::LogWarning("EnergyInDeadEB_FE")<<"TP energy in the dead TT = "<<tpEt<<" at "<<ttDetId;
                        }
                        if ( !killDeadChannels_ || recoverEBFE_ ) {  
                                // democratic energy sharing
                                
                                for ( std::vector<DetId>::const_iterator dit = vid.begin(); dit != vid.end(); ++dit ) {
				        if (alreadyInserted(*dit)) continue;
				        float theta = ebGeom_->getGeometry(*dit)->getPosition().theta();
                                        float tpEt  = ecalScale_.getTPGInGeV( tp->compressedEt(), tp->id() );
                                        if(checkChannelStatus(*dit, dbStatusToBeExcludedEB_)){
						EcalRecHit hit( *dit, tpEt /((float)vid.size()) / sin(theta), 0.);
						hit.setFlag( EcalRecHit::kTowerRecovered ) ;
						if ( tp->compressedEt() == 0xFF ) hit.setFlag( EcalRecHit::kTPSaturated );
                                        	if ( tp->sFGVB() ) hit.setFlag( EcalRecHit::kL1SpikeFlag );
                                        	insertRecHit( hit, result );
                                	}
				}
                } else {
                        // tp not found => recovery failed
                        std::vector<DetId> vid = ttMap_->constituentsOf( ttDetId );
                        for ( std::vector<DetId>::const_iterator dit = vid.begin(); dit != vid.end(); ++dit ) {
			  if (alreadyInserted(*dit)) continue;
			  EcalRecHit hit( *dit,0., 0. );
                                hit.setFlag( EcalRecHit::kDead ) ;
                                insertRecHit( hit, result );
			}
                        }
                }
        } else if ( flags == EcalRecHitWorkerRecover::EE_FE ) {
                        // Structure for recovery:
                        // ** SC --> EEDetId constituents (eeC) --> associated Trigger Towers (aTT) --> EEDetId constituents (aTTC)
                        // ** energy for a SC EEDetId = [ sum_aTT(energy) - sum_aTTC(energy) ] / N_eeC
                        // .. i.e. the total energy of the TTs covering the SC minus 
                        // .. the energy of the recHits in the TTs but not in the SC
                        //std::vector<DetId> vid = ecalMapping_->dccTowerConstituents( ecalMapping_->DCCid( ttDetId ), ecalMapping_->iTT( ttDetId ) );
			// due to lack of implementation of the EcalTrigTowerDetId ix,iy methods in EE we compute Et recovered energies (in EB we compute E)

                        EEDetId eeId( detId );
                        EcalScDetId sc( (eeId.ix()-1)/5+1, (eeId.iy()-1)/5+1, eeId.zside() );
                        std::set<DetId> eeC;
                        for(int dx=1; dx<=5; ++dx){
                                for(int dy=1; dy<=5; ++dy){
                                        int ix = (sc.ix()-1)*5 + dx;
                                        int iy = (sc.iy()-1)*5 + dy;
                                        int iz = sc.zside();
                                        if(EEDetId::validDetId(ix, iy, iz)){
					  EEDetId id(ix, iy, iz);
					  if (checkChannelStatus(id,dbStatusToBeExcludedEE_)){
                                                eeC.insert(id);
					  } // check status
                                        }
                                }
                        }
                        
                        edm::Handle<EcalTrigPrimDigiCollection> pTPDigis;
                        evt.getByToken(tpDigiToken_, pTPDigis);
                        const EcalTrigPrimDigiCollection * tpDigis = nullptr;
			tpDigis = pTPDigis.product();

                        // associated trigger towers
                        std::set<EcalTrigTowerDetId> aTT;
                        for ( std::set<DetId>::const_iterator it = eeC.begin(); it!=eeC.end(); ++it ) {
                                aTT.insert( ttMap_->towerOf( *it ) );
                        }
                        // associated trigger towers: total energy
                        float totE = 0;
                        // associated trigger towers: EEDetId constituents
                        std::set<DetId> aTTC;
                        bool atLeastOneTPSaturated = false;
                        for ( std::set<EcalTrigTowerDetId>::const_iterator it = aTT.begin(); it != aTT.end(); ++it ) {
                                // add the energy of this trigger tower
                                EcalTrigPrimDigiCollection::const_iterator itTP = tpDigis->find( *it );
                                if ( itTP != tpDigis->end() ) {

					std::vector<DetId> v = ttMap_->constituentsOf( *it );

					// from the constituents, remove dead channels
					std::vector<DetId>::iterator ttcons = v.begin();
					while (ttcons != v.end()){
					  if (!checkChannelStatus(*ttcons,dbStatusToBeExcludedEE_)){
						ttcons=v.erase(ttcons);
					  } else {
					    ++ttcons;
					  }
					}// while 

                                        if ( itTP->compressedEt() == 0xFF ){ // In the case of a saturated trigger tower, a fraction
					  atLeastOneTPSaturated = true; //of the saturated energy is put in: number of xtals in dead region/total xtals in TT *63.75
                                        	
					  //Alternative recovery algorithm that I will now investigate.
					  //Estimate energy sums the energy in the working channels, then decides how much energy
					  //to put here depending on that. Duncan 20101203
					  
					  totE += estimateEnergy(itTP->id().ietaAbs(), &result, eeC, v);
					  
					  /* 
					     These commented out lines use
					     64GeV*fraction of the TT overlapping the dead FE
					    
					  int count = 0;
					  for (std::vector<DetId>::const_iterator idsit = v.begin(); idsit != v.end(); ++ idsit){
					  std::set<DetId>::const_iterator itFind = eeC.find(*idsit);
					  if (itFind != eeC.end())
					  ++count;
					  }
					  //std::cout << count << ", " << v.size() << std::endl;
					  totE+=((float)count/(float)v.size())* ((it->ietaAbs()>26)?2*ecalScale_.getTPGInGeV( itTP->compressedEt(), itTP->id() ):ecalScale_.getTPGInGeV( itTP->compressedEt(), itTP->id() ));*/
                                        }
                                        else {totE += ((it->ietaAbs()>26)?2:1)*ecalScale_.getTPGInGeV( itTP->compressedEt(), itTP->id() );}
                                        
                                
					// get the trigger tower constituents
					
					if (itTP->compressedEt() == 0){ // If there's no energy in TT, the constituents are removed from the recovery.
						 for (size_t i = 0 ; i < v.size(); ++i)
							 eeC.erase(v[i]);
					}
					else if (itTP->compressedEt()!=0xFF){ //If it's saturated the energy has already been determined, so we do not want to subtract any channels
					 for ( size_t j = 0; j < v.size(); ++j ) {
						 aTTC.insert( v[j] );
					 }
					}
                                    
                                }
                        }
                        // remove crystals of dead SC
                        // (this step is not needed if sure that SC crystals are not 
                        // in the recHit collection)
			
			for ( std::set<DetId>::const_iterator it = eeC.begin(); it != eeC.end(); ++it ) {
			  aTTC.erase(*it);
                        }
                        // compute the total energy for the dead SC
                        const EcalRecHitCollection * hits = &result;
                        for ( std::set<DetId>::const_iterator it = aTTC.begin(); it != aTTC.end(); ++it ) {
                                EcalRecHitCollection::const_iterator jt = hits->find( *it );
                                if ( jt != hits->end() ) {
				  float energy = jt->energy(); // Correct conversion to Et
				  float eta = geo_->getPosition(jt->id()).eta();
				  float pf = 1.0/cosh(eta);
				  // use Et instead of E, consistent with the Et estimation of the associated TT
				  totE -= energy*pf;
                                }
                        }
                        

			float scEt = totE;
			float scEtThreshEE = logWarningEtThreshold_EE_FE_;
			if(scEt>scEtThreshEE){
				edm::LogWarning("EnergyInDeadEE_FE")<<"TP energy in the dead TT = "<<scEt<<" at "<<sc;
			}

                        // assign the energy to the SC crystals
			if ( !killDeadChannels_ || recoverEEFE_ ) { // if eeC is empty, i.e. there are no hits 
			                                            // in the tower, nothing is returned. No negative values from noise.
			  for ( std::set<DetId>::const_iterator it = eeC.begin(); it != eeC.end(); ++it ) {

			    float eta = geo_->getPosition(*it).eta(); //Convert back to E from Et for the recovered hits
			    float pf = 1.0/cosh(eta);
			    EcalRecHit hit( *it, totE / ((float)eeC.size()*pf), 0);
			    
			    if (atLeastOneTPSaturated) hit.setFlag(EcalRecHit::kTPSaturated );                            
			    hit.setFlag(EcalRecHit::kTowerRecovered); 
			    insertRecHit( hit, result );
			    
			  }// for
                        }// if 
        }
        return true;
}

float EcalRecHitWorkerRecover::estimateEnergy(int ieta, EcalRecHitCollection* hits, const std::set<DetId>& sId, const std::vector<DetId>& vId  ) {
	
	float xtalE=0;
	int count = 0;
	for (std::vector<DetId>::const_iterator vIdit = vId.begin(); vIdit != vId.end(); ++ vIdit){
		std::set<DetId>::const_iterator sIdit = sId.find(*vIdit);
		if (sIdit==sId.end()){
			float energy = hits->find(*vIdit)->energy();
			float eta = geo_->getPosition(*vIdit).eta();
			float pf = 1.0/cosh(eta);
			xtalE += energy*pf;
			count++;
		}
	}
	
	if (count==0) {                    // If there are no overlapping crystals return saturated value.

	  double etsat = tpgscale_.getTPGInGeV(0xFF,
					       ttMap_->towerOf(*vId.begin())); // get saturation value for the first
                                                                               // constituent, for the others it's the same 

	  return etsat/cosh(ieta)*(ieta>26?2:1); // account for duplicated TT in EE for ieta>26
	}
	else return xtalE*((vId.size()/(float)count) - 1)*(ieta>26?2:1);
	
	
}


void EcalRecHitWorkerRecover::insertRecHit( const EcalRecHit &hit, EcalRecHitCollection &collection )
{
        // skip already inserted DetId's and raise a log warning
        if ( alreadyInserted( hit.id() ) ) {
	  edm::LogWarning("EcalRecHitWorkerRecover") << "DetId already recovered! Skipping...";
                return;
        }
        EcalRecHitCollection::iterator it = collection.find( hit.id() );
        if ( it == collection.end() ) {
                // insert the hit in the collection
                collection.push_back( hit );
        } else {
                // overwrite existing recHit
                *it = hit;
        }
	//	std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << " recovered energy "<<  hit.energy() <<  " hit flag "<< hit.checkFlag(EcalRecHit::kNeighboursRecovered ) << " collection size " << collection.size() << std::endl;
        if ( hit.id().subdetId() == EcalBarrel ) {
                recoveredDetIds_EB_.insert( hit.id() );
        } else if ( hit.id().subdetId() == EcalEndcap ) {
                recoveredDetIds_EE_.insert( hit.id() );
        } else {
                edm::LogError("EcalRecHitWorkerRecover::InvalidDetId") << "Invalid DetId " << hit.id().rawId();
        }
}


bool EcalRecHitWorkerRecover::alreadyInserted( const DetId & id )
{
        bool res = false;
        if ( id.subdetId() == EcalBarrel ) {
                res = ( recoveredDetIds_EB_.find( id ) != recoveredDetIds_EB_.end() );
        } else if ( id.subdetId() == EcalEndcap ) {
                res = ( recoveredDetIds_EE_.find( id ) != recoveredDetIds_EE_.end() );
        } else {
                edm::LogError("EcalRecHitWorkerRecover::InvalidDetId") << "Invalid DetId " << id.rawId();
        }
        return res;
}

// In the future, this will be used to calibrate the TT energy. There is a dependance on
// eta at lower energies that can be corrected for here after more validation.
float EcalRecHitWorkerRecover::recCheckCalib(float eTT, int ieta){
	 
	 return eTT;
	 
}

// return false is the channel has status  in the list of statusestoexclude
// true otherwise (channel ok)
// Careful: this function works on raw (encoded) channel statuses
bool EcalRecHitWorkerRecover::checkChannelStatus(const DetId& id, 
						 const std::vector<int>& statusestoexclude){
  

  if (!chStatus_.isValid())     
    edm::LogError("ObjectNotFound") << "Channel Status not set"; 
  
  
  EcalChannelStatus::const_iterator chIt = chStatus_->find( id );
  uint16_t dbStatus = 0;
  if ( chIt != chStatus_->end() ) {
    dbStatus = chIt->getEncodedStatusCode();
  } else {
    edm::LogError("ObjectNotFound") << "No channel status found for xtal " 
				    << id.rawId() 
				    << "! something wrong with EcalChannelStatus in your DB? ";
  }
  
  for (std::vector<int>::const_iterator status = statusestoexclude.begin();
       status!= statusestoexclude.end(); ++status){
    
    if ( *status == dbStatus) return false;
    
  }

  return true;
}

#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoLocalCalo/EcalRecProducers/interface/EcalRecHitWorkerFactory.h"
DEFINE_EDM_PLUGIN( EcalRecHitWorkerFactory, EcalRecHitWorkerRecover, "EcalRecHitWorkerRecover" );
