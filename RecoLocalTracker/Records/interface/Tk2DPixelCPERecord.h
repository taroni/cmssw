#ifndef RecoLocalTracker_Records_Tk2DPixelCPERecord_h
#define RecoLocalTracker_Records_Tk2DPixelCPERecord_h

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"      
#include "CondFormats/DataRecord/interface/SiPixelLorentzAngleRcd.h"
#include "CondFormats/DataRecord/interface/SiPixelCPEGenericErrorParmRcd.h"
#include "CondFormats/DataRecord/interface/SiPixel2DTemplateDBObjectRcd.h"
#include "CalibTracker/Records/interface/SiPixel2DTemplateDBObjectESProducerRcd.h"

#include "boost/mpl/vector.hpp"

class  Tk2DPixelCPERecord: public edm::eventsetup::DependentRecordImplementation<Tk2DPixelCPERecord,
  boost::mpl::vector<TrackerDigiGeometryRecord,IdealMagneticFieldRecord,SiPixelLorentzAngleRcd,SiPixelCPEGenericErrorParmRcd,SiPixel2DTemplateDBObjectESProducerRcd> > {};

#endif 

