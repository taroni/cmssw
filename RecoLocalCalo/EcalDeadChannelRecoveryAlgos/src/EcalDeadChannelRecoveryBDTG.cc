//  Original Authors:  S. Taroni, N. Marinelli
//  University of Notre Dame - US
//  Created:   
//
//
//


#include "RecoLocalCalo/EcalDeadChannelRecoveryAlgos/interface/EcalDeadChannelRecoveryBDTG.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h" // can I use a egammatools here?
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include <iostream>
#include <TMath.h>
#include <TDirectory.h>


#include<iostream>
#include<ostream>
#include<string>
#include<fstream>

template <>
void EcalDeadChannelRecoveryBDTG<EBDetId>::loadFile() {

  readerNoCrack = new TMVA::Reader( "!Color:!Silent" );

  readerNoCrack->AddVariable("E1/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[0]));
  readerNoCrack->AddVariable("E2/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[1]));
  readerNoCrack->AddVariable("E3/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[2]));
  readerNoCrack->AddVariable("E4/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[3]));
  readerNoCrack->AddVariable("E6/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[5]));
  readerNoCrack->AddVariable("E7/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[6]));
  readerNoCrack->AddVariable("E8/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[7]));
  readerNoCrack->AddVariable("E9/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[8]));
  readerNoCrack->AddVariable("E1+E2+E3+E4+E6+E7+E8+E9"     , &(mx.sumE8) ); 

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

  readerCrack->AddVariable("E1/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[0]));
  readerCrack->AddVariable("E2/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[1]));
  readerCrack->AddVariable("E3/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[2]));
  readerCrack->AddVariable("E4/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[3]));
  readerCrack->AddVariable("E6/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[5]));
  readerCrack->AddVariable("E7/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[6]));
  readerCrack->AddVariable("E8/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[7]));
  readerCrack->AddVariable("E9/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx.rEn[8]));
  readerCrack->AddVariable("E1+E2+E3+E4+E6+E7+E8+E9"     , &(mx.sumE8) ); 

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

  std::cout << "  EcalDeadChannelRecoveryBDTG<T>::EcalDeadChannelRecoveryBDTG bdtWeightFileNoCracks_ = " << bdtWeightFileNoCracks_.fullPath()  << std::endl;
  reco::details::loadTMVAWeights(readerNoCrack, "BDTG", bdtWeightFileNoCracks_.fullPath());
  reco::details::loadTMVAWeights(readerCrack, "BDTG", bdtWeightFileNoCracks_.fullPath());

}


template <typename T> EcalDeadChannelRecoveryBDTG<T>::EcalDeadChannelRecoveryBDTG () {
}


template <typename T>EcalDeadChannelRecoveryBDTG<T>::~EcalDeadChannelRecoveryBDTG() {
}

template <>
void EcalDeadChannelRecoveryBDTG<EBDetId>::setParameters(const edm::ParameterSet&ps)
{
  ps_=ps;
  bdtWeightFileNoCracks_ = ps.getParameter<edm::FileInPath>("bdtWeightFileNoCracks");
  bdtWeightFileCracks_ = ps.getParameter<edm::FileInPath>("bdtWeightFileCracks");
  this->loadFile();
  


}


template <>
void EcalDeadChannelRecoveryBDTG<EEDetId>::setParameters(const edm::ParameterSet&ps)
{
    ps_=ps;
}




template <>
void EcalDeadChannelRecoveryBDTG<EEDetId>::setCaloTopology(const CaloTopology  *topo)
{
  topology_ = topo;

}

template <>
void EcalDeadChannelRecoveryBDTG<EBDetId>::setCaloTopology(const CaloTopology  *topo)
{
  topology_ = topo;

}


template <typename DetIdT>  
double EcalDeadChannelRecoveryBDTG<DetIdT>::recover(const DetIdT id, const EcalRecHitCollection &hit_collection, double single8Cut, double sum8Cut,  bool *AcceptFlag) {

  bool isCrack=false;
  int cellIndex=0.;
  double neighTotEn=0.;
  Float_t val =0. ;
  
  //find the matrix around id
  std::vector<DetId> m3x3aroundDC= EcalClusterTools::matrixDetId( topology_, id, 1);
  //  Loop over all cells in the vector "NxNaroundDC", and for each cell find it's energy
  //  (from the EcalRecHits collection).
  std::vector<DetId>::const_iterator theCells;

  for (theCells = m3x3aroundDC.begin(); theCells != m3x3aroundDC.end(); ++theCells) {
    EBDetId cell = DetIdT(*theCells);
    if (cell==id) {
      //EcalRecHitCollection::const_iterator goS_it = hit_collection.find(cell);
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
    }
    if (bool(cell.null())==false) {
      EcalRecHitCollection::const_iterator goS_it = hit_collection.find(cell);
      //mx.rEn[cellIndex]=0;
      //mx.iphi[cellIndex]=-599;
      //mx.ieta[cellIndex]=-99;

      //keep the en, iphi, ieta of xtals of the matrix
      if ( cell!=id && goS_it!=hit_collection.end() ) {
		
	if (goS_it->energy()<=0. || goS_it->energy()<single8Cut) {
	  *AcceptFlag=false;
	  return 0.;
	}else{
	  neighTotEn+=goS_it->energy();
	  mx.rEn[cellIndex]=goS_it->energy();
	  mx.iphi[cellIndex]=cell.iphi();
	  mx.ieta[cellIndex]=cell.ieta();
	  cellIndex++;
	}
	
	
      } else if (cell==id){
	mx.rEn[cellIndex]=0;
	cellIndex++;
      }
      //std::cout << __LINE__ << " " << cellIndex-1 << " " <<  mx.rEn[cellIndex-1]<< std::endl;

    } else {
      *AcceptFlag=false;
      return 0.;
    }
  }
  if ( cellIndex>=0 && neighTotEn>=single8Cut*8. && neighTotEn >=sum8Cut){
    bool allneighs=true;
    mx.sumE8=neighTotEn;
    for (unsigned int icell=0; icell<9 ; icell++){
      if (mx.rEn[icell]<single8Cut && icell!=4){
	allneighs=false;
      }
      // std::cout << __PRETTY_FUNCTION__ << " "<< __LINE__ << " " << icell << " " << mx.rEn[icell] << " "  << neighTotEn << " " << mx.rEn[icell]/neighTotEn<< std::endl;
      mx.rEn[icell]=mx.rEn[icell]/neighTotEn;
    } 
    // std::cout << __PRETTY_FUNCTION__ << " "<< __LINE__ << " --------- "<< allneighs <<  std::endl;
    if (allneighs==true){
      // evaluate the regression 

      if (isCrack) {
	val = exp((readerCrack->EvaluateRegression("BDTG"))[0]);
      }else {
	val= exp((readerNoCrack->EvaluateRegression("BDTG"))[0]);
      }
      //std::cout << __PRETTY_FUNCTION__ << " "<< __LINE__ << " " << id<< " " << val << " " << log(val) << std::endl;
      
      *AcceptFlag=true;
      //return the estimated energy
      return val;
    }else{
      *AcceptFlag=false;
      return 0;
    }
  }else{
    *AcceptFlag=false;
    return 0;
  }

}

template class EcalDeadChannelRecoveryBDTG<EBDetId>;
template class EcalDeadChannelRecoveryBDTG<EEDetId>; //not used. Needed to make EcalDeadChannelRecoveryAlgos compile
