#ifndef RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryBDTG_H
#define RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryBDTG_H

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"

#include "CommonTools/MVAUtils/interface/TMVAZipReader.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TTree.h>
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

#include <string>
#include <memory>

template <typename DetIdT> class EcalDeadChannelRecoveryBDTG {
 public:
  EcalDeadChannelRecoveryBDTG();
  ~EcalDeadChannelRecoveryBDTG();
  
  void setParameters(const edm::ParameterSet&ps);
  void setCaloTopology(const CaloTopology *topo );
  // void setCaloTopology(const CaloTopology *topo,const edm::ParameterSet&ps );
  double recover(const DetIdT id, const EcalRecHitCollection &hit_collection, double single8Cut, double sum8Cut, bool *AcceptFlag);
  void loadFile();

  TMVA::Reader *readerNoCrack;
  TMVA::Reader *readerCrack;

 private:
  const CaloTopology* topology_;
  edm::ParameterSet ps_;
  struct XtalMatrix {
    Float_t rEn[9], ieta[9], iphi[9], sumE8;
  };

  XtalMatrix mx;
  long a[301];
  
  edm::FileInPath bdtWeightFileNoCracks_;
  edm::FileInPath bdtWeightFileCracks_;

};

#endif
