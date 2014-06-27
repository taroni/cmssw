cmssw
=====

CMS Offline Software
====

How to download, compile and run the code for the cluster splitting. 

1. Create the working area, move to src dir and set up the CMS environment
>scram p CMSSW CMSSW_7_1_0_pre9  <br />
>cd CMSSW_7_1_0_pre9/src  <br />
>cmsenv <br />

2. Download the needed packages from the CMSSW official repository
>git cms-addpkg RecoLocalTracker/SubCollectionProducers <br />
>git cms-addpkg RecoLocalTracker/SiStripRecHitConverter <br />
>git cms-addpkg RecoLocalTracker/Records <br />
>git cms-addpkg RecoLocalTracker/SiPixelRecHits <br />
>git cms-addpkg CondFormats/DataRecord <br />
>git cms-addpkg CondFormats/SiPixelObjects <br />
>git cms-addpkg CalibTracker/Records <br />
>git cms-addpkg CalibTracker/SiPixelESProducers <br />
>git cms-addpkg CalibTracker/SiPixelErrorEstimation <br />
>git cms-addpkg CondCore/SiPixelPlugins <br />
>git cms-addpkg CondTools/SiPixel <br />
 
3. Add my repository and pull the code
>git remote add silviaGit https://github.com/taroni/cmssw.git <br />
>git pull silviaGit 71X_SLC6_sqlitefile  <br />

4. Unset some default options of gcc
>setenv USER_CXXFLAGS "-Wno-delete-non-virtual-dtor -Wno-error=unused-but-set-variable -Wno-error=unused-variable"  <br />
or 
>export USER_CXXFLAGS="-Wno-delete-non-virtual-dtor -Wno-error=unused-but-set-variable -Wno-error=unused-variable"  <br />
depending of your shell

5. Compile
>scram b -j 8  <br />

6. Go to the test dir and run it
>cd RecoLocalTracker/SubCollectionProducers/test <br />
>cmsRun run_split.py <br />
