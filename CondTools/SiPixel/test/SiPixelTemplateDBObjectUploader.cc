#include "CondTools/SiPixel/test/SiPixelTemplateDBObjectUploader.h"
#include "CondFormats/DataRecord/interface/SiPixelTemplateDBObjectRcd.h"

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include <fstream>
#include <stdio.h>
#include <iostream>

using namespace std;

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include <stdio.h>
#include <iostream>
using namespace std;

SiPixelTemplateDBObjectUploader::SiPixelTemplateDBObjectUploader(const edm::ParameterSet& iConfig):
	theTemplateCalibrations( iConfig.getParameter<vstring>("siPixelTemplateCalibrations") ),
	theTemplateBaseString( iConfig.getParameter<std::string>("theTemplateBaseString") ),
	theVersion( iConfig.getParameter<double>("Version") ),
	theMagField( iConfig.getParameter<double>("MagField") ),
	theTemplIds( iConfig.getParameter<std::vector<uint32_t> >("templateIds") )
{
}


SiPixelTemplateDBObjectUploader::~SiPixelTemplateDBObjectUploader()
{
}

void 
SiPixelTemplateDBObjectUploader::beginJob()
{
}

void
SiPixelTemplateDBObjectUploader::analyze(const edm::Event& iEvent, const edm::EventSetup& es)
{
	//--- Make the POOL-ORA object to store the database object
	SiPixelTemplateDBObject* obj = new SiPixelTemplateDBObject;

	// Local variables 
	const char *tempfile;
	int m;
	
	// Set the number of templates to be passed to the dbobject
	obj->setNumOfTempl(theTemplateCalibrations.size());

	// Set the version of the template dbobject - this is an external parameter
	obj->setVersion(theVersion);

	// Open the template file(s) 
	for(m=0; m< obj->numOfTempl(); ++m){

		edm::FileInPath file( theTemplateCalibrations[m].c_str() );
		tempfile = (file.fullPath()).c_str();

		std::ifstream in_file(tempfile, std::ios::in);
			
		if(in_file.is_open()){
			//edm::LogInfo("Template Info") << "Opened Template File: " << file.fullPath().c_str();
			cout << "Opened Template File: " << file.fullPath().c_str() << "\n";

			// Local variables 
			char title_char[80], c;
			SiPixelTemplateDBObject::char2float temp;
			float tempstore;
			int iter,j;
			
			// Templates contain a header char - we must be clever about storing this
			for (iter = 0; (c=in_file.get()) != '\n'; ++iter) {
				if(iter < 79) {title_char[iter] = c;}
			}
			if(iter > 78) {iter=78;}
			title_char[iter+1] ='\n';
			
			for(j=0; j<80; j+=4) {
				temp.c[0] = title_char[j];
				temp.c[1] = title_char[j+1];
				temp.c[2] = title_char[j+2];
				temp.c[3] = title_char[j+3];
				obj->push_back(temp.f);
				obj->setMaxIndex(obj->maxIndex()+1);
	    }
	    
	    // Fill the dbobject
	    in_file >> tempstore;
	    while(!in_file.eof()) {
	      obj->setMaxIndex(obj->maxIndex()+1);
				obj->push_back(tempstore);
				in_file >> tempstore;
			}
			
			in_file.close();
		}
		else {
			// If file didn't open, report this
			//edm::LogError("SiPixelTemplateDBObjectUploader") << "Error opening File" << tempfile;
			cout << "Error opening File " << tempfile << "\n";
		}
	}
	
	edm::ESHandle<TrackerGeometry> pDD;
	es.get<TrackerDigiGeometryRecord>().get( pDD );
	
	short templids[52];
	for(int k = 0; k < 52; k++){
	  templids[k] = (short) theTemplIds[k];
	}
 
	    
	for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
	  
	  if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0){
	    DetId detid=(*it)->geographicalId();
	    
	    if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel) &&
	       (detid.subdetId() == s_detid || s_detid == 0) ) {
	      if ( ! (*obj).putTemplateID( detid.rawId(),templid ) )
		edm::LogInfo("Template Info") << " Could not fill barrel det unit";
	    }
	    if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap) &&
	       (detid.subdetId() == s_detid || s_detid == 0) ) {
	      if ( ! (*obj).putTemplateID( detid.rawId(),templid ) )
		edm::LogInfo("Template Info") << " Could not fill endcap det unit";
	    }
	    
	  }
	}

	//--- Create a new IOV
	edm::Service<cond::service::PoolDBOutputService> poolDbService;
	if( !poolDbService.isAvailable() ) // Die if not available
		throw cms::Exception("NotAvailable") << "PoolDBOutputService not available";
	if(poolDbService->isNewTagRequest("SiPixelTemplateDBObjectRcd"))
		poolDbService->writeOne( obj, poolDbService->beginOfTime(), "SiPixelTemplateDBObjectRcd");
	else
		poolDbService->writeOne( obj, poolDbService->currentTime(), "SiPixelTemplateDBObjectRcd");
}

void SiPixelTemplateDBObjectUploader::endJob()
{
}

