#include "RecoLocalCalo/EcalDeadChannelRecoveryAlgos/interface/EcalDeadChannelRecoveryBDTG.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h" // can I use a egammatools here?
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include <iostream>
#include <TMath.h>
#include <TDirectory.h>

template <typename T> EcalDeadChannelRecoveryBDTG<T>::EcalDeadChannelRecoveryBDTG() {

  //std::string filepath
  //ADD the file here. How to pass the file? 
  //TFile file(filepath.c_str()).fullPath().c_str());
  reader = new TMVA::Reader( "!Color:!Silent" );

  reader->AddVariable("E1", &(mx.en[0]));
  reader->AddVariable("E2", &(mx.en[1]));
  reader->AddVariable("E3", &(mx.en[2]));
  reader->AddVariable("E4", &(mx.en[3]));
  reader->AddVariable("E6", &(mx.en[4]));
  reader->AddVariable("E7", &(mx.en[5]));
  reader->AddVariable("E8", &(mx.en[6]));
  reader->AddVariable("E9", &(mx.en[7]));

  // reader->AddVariable("iEta1", &(mx.ieta[0]));
  // reader->AddVariable("iEta2", &(mx.ieta[1]));
  // reader->AddVariable("iEta3", &(mx.ieta[2]));
  // reader->AddVariable("iEta4", &(mx.ieta[3]));
  // reader->AddVariable("iEta5", &(mx.ieta[4]));
  // reader->AddVariable("iEta6", &(mx.ieta[5]));
  // reader->AddVariable("iEta7", &(mx.ieta[6]));
  // reader->AddVariable("iEta8", &(mx.ieta[7]));

  // reader->AddVariable("iPhi1", &(mx.iphi[0]));
  // reader->AddVariable("iPhi2", &(mx.iphi[1]));
  // reader->AddVariable("iPhi3", &(mx.iphi[2]));
  // reader->AddVariable("iPhi4", &(mx.iphi[3]));
  // reader->AddVariable("iPhi5", &(mx.iphi[4]));
  // reader->AddVariable("iPhi6", &(mx.iphi[5]));
  // reader->AddVariable("iPhi7", &(mx.iphi[6]));
  // reader->AddVariable("iPhi8", &(mx.iphi[7]));

  edm::FileInPath weightFileEdm("RecoLocalCalo/EcalDeadChannelRecoveryAlgos/data/TMVARegression_trainingWithAliveCrystalsAllRH_mc_BDTG.weights.xml");
  reco::details::loadTMVAWeights(reader, "BDTG", weightFileEdm.fullPath());
    
 }

template <typename T>EcalDeadChannelRecoveryBDTG<T>::~EcalDeadChannelRecoveryBDTG() {
}

template <>
void EcalDeadChannelRecoveryBDTG<EBDetId>::setCaloTopology(const CaloTopology  *topo)
{
  topology_ = topo;
}

template <>
void EcalDeadChannelRecoveryBDTG<EEDetId>::setCaloTopology(const CaloTopology  *topo)
{
  topology_ = topo;
}



template <typename DetIdT>  
double EcalDeadChannelRecoveryBDTG<DetIdT>::recover(const DetIdT id, const EcalRecHitCollection &hit_collection, bool *AcceptFlag) {

  //find the matrix around id
  std::vector<DetId> m3x3aroundDC= EcalClusterTools::matrixDetId( topology_, id, -1, 1, -1, 1 );

  double EnergyMax = 0.0;

  //  Loop over all cells in the vector "NxNaroundDC", and for each cell find it's energy
  //  (from the EcalRecHits collection).
  std::vector<DetId>::const_iterator theCells;
  int cellIndex=0.;
  for (theCells = m3x3aroundDC.begin(); theCells != m3x3aroundDC.end(); ++theCells) {
    DetIdT cell = DetIdT(*theCells);
    if (cell==id) continue;
    if (! cell.null()) {
      EcalRecHitCollection::const_iterator goS_it = hit_collection.find(cell);
      //keep the en, iphi, ieta of xtals of the matrix
      if ( goS_it !=  hit_collection.end() ) {
	if (goS_it->energy()==0) {
	  *AcceptFlag=false;
	  return 0;
	}
        mx.en[cellIndex]=log(goS_it->energy());
	//mx.iphi[cellIndex]=cell.iphi();
	//mx.ieta[cellIndex]=cell.ieta();
        cellIndex++;
      }
    }
  }

  // evaluate the regression 
  Float_t val = (reader->EvaluateRegression("BDTG"))[0];
  *AcceptFlag=true;
  //return the estimated energy
  return exp(val);


}

template class EcalDeadChannelRecoveryBDTG<EBDetId>;
template class EcalDeadChannelRecoveryBDTG<EEDetId>; //not used. Needed to make EcalDeadChannelRecoveryAlgos compile
