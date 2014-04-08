#  Marc Osherson, Test Code for Pixel Template Uploader
#     Oct 2012

import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("SiPixelTemplateDBUpload")
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "MC_70_V4::All"
#process.GlobalTag.globaltag = "START71_V1::All"


MagFieldValue = float(sys.argv[2])

print '\nMagField = %f \n' % (MagFieldValue)
#version = 'v2'
version = sys.argv[3]

# Removed all but mag = 3.8 for testing.
if ( MagFieldValue==3.8 or MagFieldValue==38 ):
    MagFieldString = '38'
    file_path = "CalibTracker/SiPixelESProducers/data/templates_1D_IOV7_6_2013/template_summary_"
    suffix = ".out"
    files_to_upload = cms.vstring(
	file_path + "zp5260" + suffix,
	file_path + "zp5360" + suffix,	
	file_path + "zp6061" + suffix,
	file_path + "zp6161" + suffix,
	file_path + "zp3462" + suffix,
	file_path + "zp3562" + suffix,
	file_path + "zp0960" + suffix,
	file_path + "zp0961" + suffix,
	file_path + "zp0962" + suffix
	
)
### We must now ID each of these templates. Match each ID "zp####" in the appropriate array position below:
    theTemplateIds = cms.vuint32(5360,5360,5360,5360,5260,5260,5260,5260,6161,6161,6161,
#Corresponds to array position.	# 0    1     2    3    4    5    6    7    8    9   10
				 6161,6061,6061,6061,6061,3562,3562,3562,3562,3462,3462,
                               	#11    12   13   14   15   16   17   18   19   20   21
				 3462,3462,960,961,962,962,960,961,962,962,960,
                                #22   23    24    25   26  27   28   29   30    31    32
				 961,962,962,960,961,962,962,960,961,962,960,
                                #33   34   35   36   37   38   39   40   41   42    43
				 961,962,960,961,962,960,961,962)
                                #44   45    46   47   48   49   50    51


 			
else :
    print 'ERROR... please use values 38 and v3.'


template_base = 'SiPixelTemplateDBObject' + MagFieldString + 'T'
#theTemplateBaseString = cms.string(template_base)

print '\nUploading %s%s with record SiPixelTemplateDBObjectRcd in file siPixelTemplates%sT_IOV7.db\n' % (template_base,version,MagFieldString)

process.source = cms.Source("EmptyIOVSource",
                            timetype = cms.string('runnumber'),
                            firstValue = cms.uint64(1),
                            lastValue = cms.uint64(1),
                            interval = cms.uint64(1)
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
                                          DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(0),
    authenticationPath = cms.untracked.string('.')
    ),
                                          timetype = cms.untracked.string('runnumber'),
                                          connect = cms.string('sqlite_file:siPixelTemplates' + MagFieldString + 'T_IOV7_6_2013.db'),
                                          toPut = cms.VPSet(cms.PSet(
    record = cms.string('SiPixelTemplateDBObjectRcd'),
    tag = cms.string(template_base + version)
    ))
                                          )

process.uploader = cms.EDAnalyzer("SiPixelTemplateDBObjectUploader",
                                  siPixelTemplateCalibrations = files_to_upload,
                                  theTemplateBaseString = cms.string(template_base),
                                  Version = cms.double("3.0"),
                                  MagField = cms.double(MagFieldValue),
                                  templateIds = theTemplateIds
)


process.myprint = cms.OutputModule("AsciiOutputModule")

process.p = cms.Path(process.uploader)
process.CondDBCommon.connect = 'sqlite_file:siPixelTemplates' + MagFieldString + 'T.db'
process.CondDBCommon.DBParameters.messageLevel = 0
process.CondDBCommon.DBParameters.authenticationPath = './'
