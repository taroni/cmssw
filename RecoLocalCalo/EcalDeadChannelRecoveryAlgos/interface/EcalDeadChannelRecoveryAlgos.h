#ifndef RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryAlgos_HH
#define RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryAlgos_HH

// Reconstruction Classes
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include <string>

#include "RecoLocalCalo/EcalDeadChannelRecoveryAlgos/interface/EcalDeadChannelRecoveryNN.h"
#include "RecoLocalCalo/EcalDeadChannelRecoveryAlgos/interface/EcalDeadChannelRecoveryBDTG.h"

template <typename DetIdT> class EcalDeadChannelRecoveryAlgos {
 public:
  void setCaloTopology(const CaloTopology *topology);
  EcalRecHit correct(const DetIdT id,
                     const EcalRecHitCollection &hit_collection,
                     std::string algo, double Single8Cut, double Sum8Cut, bool *AccFlag);

 private:
  EcalDeadChannelRecoveryNN<DetIdT> nn;
  EcalDeadChannelRecoveryBDTG<DetIdT> bdtg;
};
#endif // RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryAlgos_HH
