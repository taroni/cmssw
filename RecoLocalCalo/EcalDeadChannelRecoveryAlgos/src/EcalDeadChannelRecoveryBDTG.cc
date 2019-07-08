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

#include<iostream>
#include<ostream>
#include<string>
#include<fstream>

template <>
void EcalDeadChannelRecoveryBDTG<EBDetId>::loadFile() {

  readerNoCrack = new TMVA::Reader( "!Color:!Silent" );

  for (int i =0; i< 4; ++i) readerNoCrack->AddVariable(TString("E")+(i+1)+"/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx_.rEn[i]));
  for (int i =5; i< 9; ++i) readerNoCrack->AddVariable(TString("E")+(i+1)+"/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx_.rEn[i]));
  readerNoCrack->AddVariable("E1+E2+E3+E4+E6+E7+E8+E9" , &(mx_.sumE8) );  
  for (int i =0; i< 9; ++i) readerNoCrack->AddVariable(TString("iEta")+(i+1), &(mx_.ieta[i]));
  for (int i =0; i< 9; ++i) readerNoCrack->AddVariable(TString("iPhi")+(i+1), &(mx_.iphi[i]));



  readerCrack =  new TMVA::Reader( "!Color:!Silent" );
 
  for (int i =0; i< 4; ++i) readerCrack->AddVariable(TString("E")+(i+1)+"/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx_.rEn[i]));
  for (int i =5; i< 9; ++i) readerCrack->AddVariable(TString("E")+(i+1)+"/(E1+E2+E3+E4+E6+E7+E8+E9)", &(mx_.rEn[i]));
  readerCrack->AddVariable("E1+E2+E3+E4+E6+E7+E8+E9" , &(mx_.sumE8) );   
  for (int i =0; i< 9; ++i) readerCrack->AddVariable(TString("iEta")+(i+1), &(mx_.ieta[i]));
  for (int i =0; i< 9; ++i) readerCrack->AddVariable(TString("iPhi")+(i+1), &(mx_.iphi[i]));

 

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
  bdtWeightFileNoCracks_ = ps.getParameter<edm::FileInPath>("bdtWeightFileNoCracks");
  bdtWeightFileCracks_ = ps.getParameter<edm::FileInPath>("bdtWeightFileCracks");
  this->loadFile();

}


template <>
void EcalDeadChannelRecoveryBDTG<EEDetId>::setParameters(const edm::ParameterSet&ps)
{

}


template <>  
double EcalDeadChannelRecoveryBDTG<EEDetId>::recover(const EEDetId id, const EcalRecHitCollection &hit_collection, double single8Cut, double sum8Cut,  bool *acceptFlag) {
  return 0;
}

template <>  
double EcalDeadChannelRecoveryBDTG<EBDetId>::recover(const EBDetId id, const EcalRecHitCollection &hit_collection, double single8Cut, double sum8Cut,  bool *acceptFlag) {

  bool isCrack=false;
  int cellIndex=0.;
  double neighTotEn=0.;
  float val =0. ;
  
  //find the matrix around id
  std::vector<DetId> m3x3aroundDC= EcalClusterTools::matrixDetId( topology_, id, 1);
  // do not apply the recovery if the matrix is not 3x3
  if (m3x3aroundDC.size()<9){
    *acceptFlag=false;
    return 0; 
  }

  //  Loop over all cells in the vector "NxNaroundDC", and for each cell find it's energy
  //  (from the EcalRecHits collection).
 

  for (auto const& theCells : m3x3aroundDC) {
    EBDetId cell = EBDetId(theCells);
    if (cell==id) {
      int iEtaCentral = cell.ieta();
      int iPhiCentral = cell.iphi();

      if( std::abs(iEtaCentral) < 2 || 
	  std::abs(iEtaCentral - 25) < 2 || std::abs(iEtaCentral + 25) < 2 ||
	  std::abs(iEtaCentral - 45) < 2 || std::abs(iEtaCentral + 45) < 2 ||
	  std::abs(iEtaCentral - 65) < 2 || std::abs(iEtaCentral + 65) < 2 ||
	  std::abs(iEtaCentral) > 83    ||
	   (int(iPhiCentral+0.5)%20 ==0)
	   )  isCrack=true;

    }
    if (bool(cell.null())==false) {
      EcalRecHitCollection::const_iterator goS_it = hit_collection.find(cell);
      if ( goS_it!=hit_collection.end() ){
	//keep the en, iphi, ieta of xtals of the matrix
	if ( cell!=id ) {	
	  if (goS_it->energy()<=0. || goS_it->energy()<single8Cut) {
	    *acceptFlag=false;
	    return 0.;
	  }else{
	    neighTotEn+=goS_it->energy();
	    mx_.rEn[cellIndex]=goS_it->energy();
	    mx_.iphi[cellIndex]=cell.iphi();
	    mx_.ieta[cellIndex]=cell.ieta();
	    cellIndex++;
	  }
       	} else { // the cell is the central one
	  mx_.rEn[cellIndex]=0;
	  cellIndex++;
	}
      } else { //goS_it is not in the rechitcollection
	*acceptFlag=false;
	return 0.;
      }
    } else { //cell is null
      *acceptFlag=false;
      return 0.;
    }
  }
  if ( cellIndex>0 && neighTotEn>=single8Cut*8. && neighTotEn >=sum8Cut){
    bool allneighs=true;
    mx_.sumE8=neighTotEn;
    for (unsigned int icell=0; icell<9 ; icell++){
      if (mx_.rEn[icell]<single8Cut && icell!=4){
	allneighs=false;
      }
      mx_.rEn[icell]=mx_.rEn[icell]/neighTotEn;
    } 
    if (allneighs==true){
      // evaluate the regression 
      if (isCrack) {
	val = exp((readerCrack->EvaluateRegression("BDTG"))[0]);
      }else {
	val= exp((readerNoCrack->EvaluateRegression("BDTG"))[0]);
      }
      
      *acceptFlag=true;
      //return the estimated energy
      return val;
    }else{
      *acceptFlag=false;
      return 0;
    }
  }else{
    *acceptFlag=false;
    return 0;
  }

}

template class EcalDeadChannelRecoveryBDTG<EBDetId>;
template class EcalDeadChannelRecoveryBDTG<EEDetId>; //not used.
