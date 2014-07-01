#include "CondTools/SiPixel/test/SiPixel2DTemplateDBObjectUploader.h"
#include "CondFormats/DataRecord/interface/SiPixel2DTemplateDBObjectRcd.h"

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

SiPixel2DTemplateDBObjectUploader::SiPixel2DTemplateDBObjectUploader(const edm::ParameterSet& iConfig):
	theTemplateCalibrations( iConfig.getParameter<vstring>("siPixelTemplateCalibrations") ),
	theTemplateBaseString( iConfig.getParameter<std::string>("theTemplateBaseString") ),
	theVersion( iConfig.getParameter<double>("Version") ),
	theMagField( iConfig.getParameter<double>("MagField") ),
	theTemplIds( iConfig.getParameter<std::vector<uint32_t> >("templateIds") )
{
}


SiPixel2DTemplateDBObjectUploader::~SiPixel2DTemplateDBObjectUploader()
{
}

void 
SiPixel2DTemplateDBObjectUploader::beginJob()
{
}

void
SiPixel2DTemplateDBObjectUploader::analyze(const edm::Event& iEvent, const edm::EventSetup& es)
{
	//--- Make the POOL-ORA object to store the database object
	SiPixel2DTemplateDBObject* obj = new SiPixel2DTemplateDBObject;

	// Local variables 
	const char *tempfile;
	int m;
	
	// Set the number of templates to be passed to the dbobject
	obj->setNumOfTempl(theTemplateCalibrations.size());
	cout << obj->numOfTempl() << endl;
	cout << theVersion << endl;
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
			SiPixel2DTemplateDBObject::char2float temp;
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
			//edm::LogError("SiPixel2DTemplateDBObjectUploader") << "Error opening File" << tempfile;
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

			// Here is the actual looping step over all DetIds:				
			DetId detid=(*it)->geographicalId();

			unsigned int layer=0, disk=0, side=0, blade=0, panel=0, module=0;
					
			// Now we sort them into the Barrel and Endcap:
			if(detid.subdetId() == 1) {
				PXBDetId pdetId = PXBDetId(detid);
				//unsigned int detTypeP=pdetId.det();
				//unsigned int subidP=pdetId.subdetId();
				//short temp123abc = (short) theTemplIds[1];
		}
	}

	//--- Create a new IOV
	edm::Service<cond::service::PoolDBOutputService> poolDbService;
	if( !poolDbService.isAvailable() ) // Die if not available
		throw cms::Exception("NotAvailable") << "PoolDBOutputService not available";
	if(poolDbService->isNewTagRequest("SiPixel2DTemplateDBObjectRcd"))
		poolDbService->writeOne( obj, poolDbService->beginOfTime(), "SiPixel2DTemplateDBObjectRcd");
	else
		poolDbService->writeOne( obj, poolDbService->currentTime(), "SiPixel2DTemplateDBObjectRcd");
}

void SiPixel2DTemplateDBObjectUploader::endJob()
{
}

