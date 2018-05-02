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
  double recover(const DetIdT id, const EcalRecHitCollection &hit_collection, bool *AcceptFlag);

  TMVA::Reader *reader;

 private:
  const CaloTopology* topology_;
  struct XtalMatrix {
    Float_t en[8];//, ieta[8], iphi[8];
  };

  XtalMatrix mx;
};

#endif
