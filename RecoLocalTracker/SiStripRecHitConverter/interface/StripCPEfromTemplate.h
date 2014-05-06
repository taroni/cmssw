#ifndef RecoLocalTracker_SiStripRecHitConverter_StripCPEfromTemplate_H
#define RecoLocalTracker_SiStripRecHitConverter_StripCPEfromTemplate_H

#include "RecoLocalTracker/SiStripRecHitConverter/interface/StripCPE.h"

#include "RecoLocalTracker/SiStripRecHitConverter/interface/SiStripTemplate.h"
#include "RecoLocalTracker/SiStripRecHitConverter/interface/SiStripTemplateReco.h"


class StripCPEfromTemplate : public StripCPE 
{

 public:
  

  StripClusterParameterEstimator::LocalValues 
    localParameters( const SiStripCluster&, const GeomDetUnit&, const LocalTrajectoryParameters&) const;

  StripCPEfromTemplate( edm::ParameterSet & conf, 
			  const MagneticField& mag, 
			  const TrackerGeometry& geom, 
			  const SiStripLorentzAngle& lorentz,
			  const SiStripBackPlaneCorrection& backplaneCorrections,
			  const SiStripConfObject& confObj,
			  const SiStripLatency& latency) 
    : StripCPE(conf, mag, geom, lorentz, backplaneCorrections, confObj, latency ),
    use_template_reco( conf.getParameter<bool>("UseTemplateReco") ),
    template_reco_speed( conf.getParameter<int>("TemplateRecoSpeed") ),
    SID( conf.getParameter<int>("StripTemplateID")), 
    use_strip_split_cluster_errors( conf.getParameter<bool>("UseStripSplitClusterErrors") )
    {
      SiStripTemplate::pushfile( SID + 1, theStripTemp_ );
      SiStripTemplate::pushfile( SID + 2, theStripTemp_ );
      SiStripTemplate::pushfile( SID + 3, theStripTemp_ );
      SiStripTemplate::pushfile( SID + 4, theStripTemp_ );
      SiStripTemplate::pushfile( SID + 5, theStripTemp_ );
      SiStripTemplate::pushfile( SID + 6, theStripTemp_ );


      //cout << "STRIPS: (int)use_template_reco = " << (int)use_template_reco << endl;
      //cout << "template_reco_speed    = " << template_reco_speed    << endl;
      //cout << "(int)use_strip_split_cluster_errors = " << (int)use_strip_split_cluster_errors << endl;
    }
  
 private:

  std::vector< SiStripTemplateStore > theStripTemp_;

  bool use_template_reco;
  int template_reco_speed;
  int SID;
  bool use_strip_split_cluster_errors;

};
#endif
