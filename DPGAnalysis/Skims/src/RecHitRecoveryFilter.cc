// -*- C++ -*-
//
// Package:    RecHitRecoveryFilter
// Class:      RecHitRecoveryFilter
// 
/**\class RecHitRecoveryFilter RecHitRecoveryFilter.cc 

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Silvia Taroni
//         Created:  Mon 6 Aug 2018
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

//
// class declaration
//

class RecHitRecoveryFilter : public edm::EDFilter {
   public:
      explicit RecHitRecoveryFilter(const edm::ParameterSet&);
      ~RecHitRecoveryFilter() override;

   private:
      void beginJob() override ;
      bool filter(edm::Event&, const edm::EventSetup&) override;
      void endJob() override ;

      // RecHit input tags
      edm::InputTag ebRecHitsTag_;
      edm::InputTag eeRecHitsTag_;
      
      // RecHit handles
      edm::Handle<EcalRecHitCollection> ebRecHitsH_;
      edm::Handle<EcalRecHitCollection> eeRecHitsH_;
      const EcalRecHitCollection *ebRecHits_;
      const EcalRecHitCollection *eeRecHits_;     
      edm::EDGetTokenT<EcalRecHitCollection>   tok_EB_;
      edm::EDGetTokenT<EcalRecHitCollection>   tok_EE_;

      // Procesing control

      bool doEb_;
      bool doEe_;
      const edm::InputTag            labelEB_, labelEE_;
      double ebThresh_;
      double eeThresh_;
};

//
// constructors and destructor
//
RecHitRecoveryFilter::RecHitRecoveryFilter(const edm::ParameterSet& iConfig):
  doEb_( iConfig.getParameter<bool>("DoEB")),
  doEe_( iConfig.getParameter<bool>("DoEE")),
  labelEB_ (iConfig.getParameter<edm::InputTag>("EBRecHits")),
  labelEE_ (iConfig.getParameter<edm::InputTag>("EERecHits"))
{
   tok_EB_       = consumes<EcalRecHitCollection>(labelEB_);
   tok_EE_       = consumes<EcalRecHitCollection>(labelEE_);
}


RecHitRecoveryFilter::~RecHitRecoveryFilter()
{
}


//
// member functions
//
// ------------ method called on each new Event  ------------
bool
RecHitRecoveryFilter::filter(edm::Event& evt, const edm::EventSetup& iSetup)
{

  bool finalHitFlag = false; 
   if(doEb_)
   {
     //std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl ; 
      evt.getByToken(tok_EB_, ebRecHitsH_);
      ebRecHits_ = ebRecHitsH_.product();
      for(EcalRecHitCollection::const_iterator it = ebRecHits_->begin(); it != ebRecHits_->end(); ++it)
      {
	bool hitFlag = it->checkFlag(EcalRecHit::kNeighboursRecovered);
	
	//std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << " " << hitFlag << " " << it->isRecovered()<< std::endl; 
	if (hitFlag) {
	  std::cout << "recovered event: "<< evt.id().run()<< ":"<< evt.id().luminosityBlock()<< ":" << evt.id().event()<< std::endl;
	  finalHitFlag=true;
	}
        
      }
   }
   if(doEe_)
   {
      evt.getByToken(tok_EE_, eeRecHitsH_);
      eeRecHits_ = eeRecHitsH_.product();
      for(EcalRecHitCollection::const_iterator it = eeRecHits_->begin(); it != eeRecHits_->end(); ++it)
      {
        bool hitFlag = it->checkFlag(EcalRecHit::kNeighboursRecovered);
        
        
      }
   }

   return finalHitFlag;
}

// ------------ method called once each job just before starting event loop  ------------
void 
RecHitRecoveryFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RecHitRecoveryFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecHitRecoveryFilter);
