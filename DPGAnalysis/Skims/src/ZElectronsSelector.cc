#include "FWCore/PluginManager/interface/ModuleDef.h"

// system include files
#include <memory>
#include <algorithm>
#include <iostream>
#include <vector> 

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
//
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "DataFormats/Common/interface/View.h"

#include <string>
using namespace std;
using namespace reco;
namespace edm { class EventSetup; }

class ZElectronsSelector {

public:
  ZElectronsSelector(const edm::ParameterSet&, edm::ConsumesCollector & iC );
  bool operator()(const reco::GsfElectron & ) const;
  void newEvent (const edm::Event&, const edm::EventSetup&);
  EffectiveAreas _effectiveAreas;
  edm::InputTag rhoTag;
  //int nSelectedEvents;

  edm::EDGetTokenT<double> theRhoToken;
  edm::EDGetTokenT<reco::GsfElectronCollection> theGsfEToken;
  edm::Handle< double > _rhoHandle;
 
};

ZElectronsSelector::ZElectronsSelector(const edm::ParameterSet& cfg, edm::ConsumesCollector & iC ) :
  _effectiveAreas( (cfg.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath()),
  theRhoToken(iC.consumes <double> (cfg.getParameter<edm::InputTag>("rho")))
{}

void  ZElectronsSelector::newEvent(const edm::Event& ev, const edm::EventSetup& ){

  ev.getByToken(theRhoToken,_rhoHandle);

}

bool ZElectronsSelector::operator()(const reco::GsfElectron& el) const{

  bool keepEle = false; 
  float pt_e = el.pt();
  float eta_e = el.eta(); 
  std::cout << __LINE__ << std::endl;
  

  auto etrack  = el.gsfTrack().get(); 
  std::cout << __LINE__ << std::endl;
  
  if (el.isEB()){
    if (etrack->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS)>2) return keepEle; 
    if (el.full5x5_sigmaIetaIeta() >  0.0115) return keepEle; 
    if (fabs(el.deltaPhiSuperClusterTrackAtVtx())> 0.228) return keepEle; 
    if (fabs(el.deltaEtaSuperClusterTrackAtVtx())> 0.00749) return keepEle; 
    if (el.hadronicOverEm()>0.346) return keepEle;
    float abseta = fabs((el.superCluster().get())->position().eta());
    if (abseta> 1.479) return keepEle; // check if it is really needed
    const float  eA = _effectiveAreas.getEffectiveArea( abseta );
    const float rho = _rhoHandle.isValid() ? (float)(*_rhoHandle) : 0; 
    std::cout << __LINE__ << " rhoHandle.isValid() " << _rhoHandle.isValid()  << std::endl; 
    std::cout << __LINE__ << " ept:" <<  pt_e << ", e eta: "<< eta_e << " " << eA<< " " << rho << std::endl; 
    if (( el.pfIsolationVariables().sumChargedHadronPt + 
	  std::max(float(0.0), el.pfIsolationVariables().sumNeutralHadronEt +  
		   el.pfIsolationVariables().sumPhotonEt -  eA*rho )
	  ) > 0.175*pt_e) return keepEle; 
    keepEle=true;
    
  }else if (el.isEE()){
    if (etrack->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS)>3) return keepEle; 
    if (el.full5x5_sigmaIetaIeta() > 0.037) return keepEle; 
    if (fabs(el.deltaPhiSuperClusterTrackAtVtx())> 0.213) return keepEle; 
    if (fabs(el.deltaEtaSuperClusterTrackAtVtx())> 0.00895) return keepEle; 
    if (el.hadronicOverEm()>0.211) return keepEle;
    float abseta = fabs((el.superCluster())->position().eta());
    if (abseta< 1.479) return keepEle; // check if it is really needed
    if (abseta>=2.5  ) return keepEle; // check if it is really needed
    
    const float  eA = _effectiveAreas.getEffectiveArea( abseta );
    const float rho = _rhoHandle.isValid() ? (float)(*_rhoHandle) : 0; 
    std::cout << __LINE__ << " rhoHandle.isValid() " << _rhoHandle.isValid()  << std::endl; 
    std::cout << __LINE__ << " ept:" <<  pt_e << ", e eta: "<< eta_e << " " << eA<< " " << rho << std::endl; 
    
    if ((el.pfIsolationVariables().sumChargedHadronPt + 
	 std::max(float(0.0), el.pfIsolationVariables().sumNeutralHadronEt + 
		 el.pfIsolationVariables().sumPhotonEt - eA*rho)
	 ) >0.159*pt_e) return keepEle; 
    keepEle=true;
	
  }//isEE
  return keepEle;

}

EVENTSETUP_STD_INIT(ZElectronsSelector);
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/AndSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/EventSetupInitTrait.h"

typedef SingleObjectSelector<
  edm::View<reco::GsfElectron>,
  ZElectronsSelector
  //  AndSelector< ZElectronsSelector,   StringCutObjectSelector<reco::GsfElectron> >
> ZElectronsSelectorAndSkim;




DEFINE_FWK_MODULE( ZElectronsSelectorAndSkim );
