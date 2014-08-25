#!/bin/tcsh -f
    
    set W_DIR = "/afs/cern.ch/work/t/taroni/private/ClusterSplitting/databasetest/val/CMSSW_7_1_6/src/RecoLocalTracker/SubCollectionProducers/test"
    set CFG = "/afs/cern.ch/work/t/taroni/private/ClusterSplitting/databasetest/val/CMSSW_7_1_6/src/RecoLocalTracker/SubCollectionProducers/test/runValonlyTempSplit.py"
    
    
    cd $W_DIR
    
    eval `scramv1 runtime -csh`
    cd -
    cmsRun $CFG
    
     
    cp DQM_V0001_R000000001__ZpTauTau__7_1_6__split.root $W_DIR
   
    exit


    
