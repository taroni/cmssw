cmssw
=====

CMS Offline Software
====

How to download, compile and run the code for the cluster splitting. 

1. Create the working area, move to src dir and set up the CMS environment
>scram p CMSSW CMSSW_7_1_0_pre9
>cd CMSSW_7_1_0_pre9/src
>cmsenv

2. Download the needed packages from the CMSSW official repository
>git cms-addpkg RecoLocalTracker/SubCollectionProducers
>git cms-addpkg RecoLocalTracker/SiStripRecHitConverter
>git cms-addpkg RecoLocalTracker/Records
>git cms-addpkg RecoLocalTracker/SiPixelRecHits
>git cms-addpkg CondFormats/DataRecord
>git cms-addpkg CondFormats/SiPixelObjects
>git cms-addpkg CalibTracker/Records
>git cms-addpkg CalibTracker/SiPixelESProducers
>git cms-addpkg CalibTracker/SiPixelErrorEstimation
>git cms-addpkg CondCore/SiPixelPlugins
>git cms-addpkg CondTools/SiPixel

3. Add my repository and pull the code
>git remote add silviaGit https://github.com/taroni/cmssw.git
>git pull silviaGit 71X_SLC6 

4. Unset some default options of gcc
>setenv USER_CXXFLAGS "-Wno-delete-non-virtual-dtor -Wno-error=unused-but-set-variable -Wno-error=unused-variable" 
or 
>export USER_CXXFLAGS="-Wno-delete-non-virtual-dtor -Wno-error=unused-but-set-variable -Wno-error=unused-variable" 
depending of your shell

5. Compile
>scram b -j 8 

6. Go to the test dir and run it
>cd RecoLocalTracker/SubCollectionProducers/test
>cmsRun run_split.py
