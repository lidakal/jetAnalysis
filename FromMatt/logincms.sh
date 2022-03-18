export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source /cvmfs/cms.cern.ch/cmsset_default.sh 
#export CVSROOT=:ext:mnguyen@cmssw.cvs.cern.ch:/local/reps/CMSSW
### DPM
export DPNS_HOST=polgrid4.in2p3.fr
export DPM_HOST=polgrid4.in2p3.fr

#voms-proxy-init
#voms-proxy-init --voms cms --debug

### CRAB
#source /opt/exp_soft/cms/cms_ui_env_crab.sh
#SL6 workaround
#export LD_PRELOAD=/cvmfs/cms.cern.ch/slc5_amd64_gcc462/external/py2-cjson/1.0.5-cms/lib/python2.6/site-packages/cjson.so
#export PYTHONPATH=/cvmfs/cms.cern.ch/slc5_amd64_gcc462/external/py2-cjson/1.0.5-cms/lib/python2.6/site-packages/:$PYTHONPATH 

#if [ -z "$CRABDIR" ]; then source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.sh; fi                                                                                                 #echo "using crab: $CRABDIR"                                                                                                                                                         

#source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh  


source /cvmfs/cms.cern.ch/crab3/crab.sh
