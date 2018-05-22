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
  readerNoCrack = new TMVA::Reader( "!Color:!Silent" );

  readerNoCrack->AddVariable("E1", &(mx.en[0]));
  readerNoCrack->AddVariable("E2", &(mx.en[1]));
  readerNoCrack->AddVariable("E3", &(mx.en[2]));
  readerNoCrack->AddVariable("E4", &(mx.en[3]));
  readerNoCrack->AddVariable("E6", &(mx.en[5]));
  readerNoCrack->AddVariable("E7", &(mx.en[6]));
  readerNoCrack->AddVariable("E8", &(mx.en[7]));
  readerNoCrack->AddVariable("E9", &(mx.en[8]));

  readerNoCrack->AddVariable("iEta1", &(mx.ieta[0]));
  readerNoCrack->AddVariable("iEta2", &(mx.ieta[1]));
  readerNoCrack->AddVariable("iEta3", &(mx.ieta[2]));
  readerNoCrack->AddVariable("iEta4", &(mx.ieta[3]));
  readerNoCrack->AddVariable("iEta5", &(mx.ieta[4]));
  readerNoCrack->AddVariable("iEta6", &(mx.ieta[5]));
  readerNoCrack->AddVariable("iEta7", &(mx.ieta[6]));
  readerNoCrack->AddVariable("iEta8", &(mx.ieta[7]));
  readerNoCrack->AddVariable("iEta9", &(mx.ieta[8]));

  readerNoCrack->AddVariable("iPhi1", &(mx.iphi[0]));
  readerNoCrack->AddVariable("iPhi2", &(mx.iphi[1]));
  readerNoCrack->AddVariable("iPhi3", &(mx.iphi[2]));
  readerNoCrack->AddVariable("iPhi4", &(mx.iphi[3]));
  readerNoCrack->AddVariable("iPhi5", &(mx.iphi[4]));
  readerNoCrack->AddVariable("iPhi6", &(mx.iphi[5]));
  readerNoCrack->AddVariable("iPhi7", &(mx.iphi[6]));
  readerNoCrack->AddVariable("iPhi8", &(mx.iphi[7]));
  readerNoCrack->AddVariable("iPhi9", &(mx.iphi[8]));

  readerCrack = new TMVA::Reader( "!Color:!Silent" );

  readerCrack->AddVariable("E1", &(mx.en[0]));
  readerCrack->AddVariable("E2", &(mx.en[1]));
  readerCrack->AddVariable("E3", &(mx.en[2]));
  readerCrack->AddVariable("E4", &(mx.en[3]));
  readerCrack->AddVariable("E6", &(mx.en[5]));
  readerCrack->AddVariable("E7", &(mx.en[6]));
  readerCrack->AddVariable("E8", &(mx.en[7]));
  readerCrack->AddVariable("E9", &(mx.en[8]));

  readerCrack->AddVariable("iEta1", &(mx.ieta[0]));
  readerCrack->AddVariable("iEta2", &(mx.ieta[1]));
  readerCrack->AddVariable("iEta3", &(mx.ieta[2]));
  readerCrack->AddVariable("iEta4", &(mx.ieta[3]));
  readerCrack->AddVariable("iEta5", &(mx.ieta[4]));
  readerCrack->AddVariable("iEta6", &(mx.ieta[5]));
  readerCrack->AddVariable("iEta7", &(mx.ieta[6]));
  readerCrack->AddVariable("iEta8", &(mx.ieta[7]));
  readerCrack->AddVariable("iEta9", &(mx.ieta[8]));

  readerCrack->AddVariable("iPhi1", &(mx.iphi[0]));
  readerCrack->AddVariable("iPhi2", &(mx.iphi[1]));
  readerCrack->AddVariable("iPhi3", &(mx.iphi[2]));
  readerCrack->AddVariable("iPhi4", &(mx.iphi[3]));
  readerCrack->AddVariable("iPhi5", &(mx.iphi[4]));
  readerCrack->AddVariable("iPhi6", &(mx.iphi[5]));
  readerCrack->AddVariable("iPhi7", &(mx.iphi[6]));
  readerCrack->AddVariable("iPhi8", &(mx.iphi[7]));
  readerCrack->AddVariable("iPhi9", &(mx.iphi[8]));

  edm::FileInPath weightFileNoCrackEdm("RecoLocalCalo/EcalDeadChannelRecoveryAlgos/data/TMVARegression_trainingWithAliveCrystalsAllRH_8GT280MeV_noCracks_data_BDTG.weights.xml");
  reco::details::loadTMVAWeights(readerNoCrack, "BDTG", weightFileNoCrackEdm.fullPath());

  edm::FileInPath weightFileCrackEdm("RecoLocalCalo/EcalDeadChannelRecoveryAlgos/data/TMVARegression_trainingWithAliveCrystalsAllRH_8GT280MeV_onlyCracks_data_BDTG.weights.xml");
  reco::details::loadTMVAWeights(readerCrack, "BDTG", weightFileNoCrackEdm.fullPath());

    
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

  bool isCrack=false;

  //find the matrix around id
  std::vector<DetId> m3x3aroundDC= EcalClusterTools::matrixDetId( topology_, id, -1, 1, -1, 1 );

  double EnergyMax = 0.0;

  //  Loop over all cells in the vector "NxNaroundDC", and for each cell find it's energy
  //  (from the EcalRecHits collection).
  std::vector<DetId>::const_iterator theCells;
  int cellIndex=0.;
  for (theCells = m3x3aroundDC.begin(); theCells != m3x3aroundDC.end(); ++theCells) {
    EBDetId cell = DetIdT(*theCells);
    if (cell==id) {
      int iEtaCentral = cell.ieta();
      int iPhiCentral = cell.iphi();
      if ( ( iEtaCentral < 2 && iEtaCentral > -2 )     ||
	   ( iEtaCentral > 23 && iEtaCentral < 27 )    ||
	   ( iEtaCentral > -27 && iEtaCentral < -23 )  ||
	   ( iEtaCentral > 43 && iEtaCentral < 47 )    ||
	   ( iEtaCentral > -47 && iEtaCentral < -43 )  ||
	   ( iEtaCentral > 63 && iEtaCentral < 67 )    ||
	   ( iEtaCentral > -67 && iEtaCentral < -63 )  ||
	   ( iEtaCentral > 83 || iEtaCentral < -83 )    ||
	   (int(iPhiCentral+0.5)%20 ==0)
	   )  isCrack=true;
      std::cout << __PRETTY_FUNCTION__ << " " << id << ", ieta" << iEtaCentral << ", iphi "<< iPhiCentral << std::endl;
      continue;
    }
    if (! cell.null()) {
      EcalRecHitCollection::const_iterator goS_it = hit_collection.find(cell);
      //keep the en, iphi, ieta of xtals of the matrix
      if ( goS_it !=  hit_collection.end() ) {
	if (goS_it->energy()==0) {
	  *AcceptFlag=false;
	  return 0;
	}
        mx.en[cellIndex]=log(goS_it->energy());
	mx.iphi[cellIndex]=cell.iphi();
        mx.ieta[cellIndex]=cell.ieta();
        cellIndex++;
      }
    }
  }

  // evaluate the regression 
  Float_t val =0. ;
  if (isCrack) {
    val = (readerCrack->EvaluateRegression("BDTG"))[0];
    std::cout << __PRETTY_FUNCTION__ << " Central evaluation Crack " << exp(val) << " GeV" << std::endl;
  }else {
    val= (readerNoCrack->EvaluateRegression("BDTG"))[0];
    std::cout << __PRETTY_FUNCTION__ << " Central evaluation NoCrack " << exp(val) << " GeV" << std::endl;

  }
  *AcceptFlag=true;
  //return the estimated energy
  return exp(val);


}

template class EcalDeadChannelRecoveryBDTG<EBDetId>;
template class EcalDeadChannelRecoveryBDTG<EEDetId>; //not used. Needed to make EcalDeadChannelRecoveryAlgos compile
