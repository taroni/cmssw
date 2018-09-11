#ifndef RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryBDTG_H
#define RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryBDTG_H

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"

#include "CommonTools/Utils/interface/TMVAZipReader.h"
#include "CondFormats/EgammaObjects/interface/GBRForest.h"

#include <TTree.h>
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

#include <string>
#include <memory>

template <typename DetIdT> class EcalDeadChannelRecoveryBDTG {
 public:
  EcalDeadChannelRecoveryBDTG();
  ~EcalDeadChannelRecoveryBDTG();
  
  
  void setCaloTopology(const CaloTopology *topo);
  double recover(const DetIdT id, const EcalRecHitCollection &hit_collection, float single8Cut,  bool *AcceptFlag);

  TMVA::Reader *readerNoCrack;
  TMVA::Reader *readerCrack;

 private:
  const CaloTopology* topology_;
  struct XtalMatrix {
    Float_t en[9], ieta[9], iphi[9];
  };

  XtalMatrix mx;
};

#endif
