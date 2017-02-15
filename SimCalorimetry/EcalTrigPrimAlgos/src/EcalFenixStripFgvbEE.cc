#include <SimCalorimetry/EcalTrigPrimAlgos/interface/EcalFenixStripFgvbEE.h>
#include <DataFormats/EcalDigi/interface/EEDataFrame.h>
#include <CondFormats/EcalObjects/interface/EcalTPGFineGrainStripEE.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

#define debug_ 1

EcalFenixStripFgvbEE::EcalFenixStripFgvbEE()
{
  identif_=0;
  flagBadStripMiss_=false;
}

EcalFenixStripFgvbEE::~EcalFenixStripFgvbEE(){
}

void EcalFenixStripFgvbEE::process( std::vector<std::vector<int> > &linout ,std::vector<int> & output)
{
  std::vector<int> indexLut(output.size());

  for (unsigned int i=0;i<output.size();i++) {
    output[i]=0;
    indexLut[i]=0;
    for (unsigned int ixtal=0;ixtal<linout.size();ixtal++) {
      int adc=linout[ixtal][i];
      int res = (((adc & 0xffff) > threshold_fg_) || ((adc & 0x30000) != 0x0)) ? 1 : 0;
      std::cout << __PRETTY_FUNCTION__ << " line " << __LINE__ <<  " xtal " << ixtal << " threshold "<< threshold_fg_ << " adc " << adc << " "<< (adc & 0xffff) << " res " << res <<  std::endl; 
      indexLut[i] = indexLut[i] | (res << ixtal);
    }
    int mask = 1<<(indexLut[i]);
    output[i]= ((lut_fg_ & mask) == 0x0) ? 0 : 1;
    std::cout << __PRETTY_FUNCTION__ << " line " << __LINE__ << " mask " << std::dec<< mask << " lut " << std::hex<< lut_fg_ << " "<< std::dec << output[i]<<  std::endl;
    if(i > 0) output[i-1] = output[i]; // Delay one clock
  }


  return;
}  

void EcalFenixStripFgvbEE::setParameters(int identif, uint32_t id,const EcalTPGFineGrainStripEE * ecaltpgFgStripEE)
{
  const EcalTPGFineGrainStripEEMap &fgmap = ecaltpgFgStripEE -> getMap();
  EcalTPGFineGrainStripEEMapIterator it=fgmap.find(id);
  if (it!=fgmap.end()){
     threshold_fg_ = it->second.threshold;
     lut_fg_ = it->second.lut;
     if (debug_) std::cout<< __PRETTY_FUNCTION__ << " line " << __LINE__ << " threshold " << threshold_fg_ << ", Lut " << lut_fg_ << std::endl;  
  }
  else
  {
     if (identif == false){
       edm::LogWarning("EcalTPG")<<" could not find EcalTPGFineGrainStripEEMap entry for at least one stripId";
       flagBadStripMiss_ = true;
     }
     
     // Use the FENIX power-up values
     threshold_fg_ = 65535;
     lut_fg_ = 0x0;
  }

}
