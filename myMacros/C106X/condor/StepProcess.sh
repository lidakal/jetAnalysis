#!/bin/sh --login  

#### Use: with submitAllNtuples or SubmitJobs.sh

echo "hostname:"
hostname

export X509_USER_PROXY=$1
voms-proxy-info -all
voms-proxy-info -all -file $1

#BSUB -q 1nh                                                                                                                                     

WorkDir=$2
File=$3
Output=$4
ID=$5

echo proxy=$1
echo WorkDir=$2
echo File=$3
echo Output=$4
echo ID=$5

source /opt/exp_soft/llr/root/v6.24.04-el7-gcc9xx-py370/etc/init.sh

source /cvmfs/cms.cern.ch/cmsset_default.sh
# cd /home/llr/cms/kalipoliti/CMSSW_12_6_0_patch1/src
cd /home/llr/cms/kalipoliti/C10630p1_miniAOD/src
eval `scramv1 runtime -sh`
# cmsenv
cd $WorkDir
# source /opt/exp_soft/llr/root/v6.14.04-el7-gcc71-py37/etc/init.sh

echo root bin is: 
which root
echo scram arch is: $SCRAM_ARCH
echo Input files are: $File

echo root -l -b -q $WorkDir'/create_new_tree.C("'${File}'", "'${Output}'/test_file_'${ID}'.root")'
root -l -b -q $WorkDir'/create_new_tree.C("'${File}'", "'${Output}'/test_file_'${ID}'.root")'
 
# cp test_file.root ${Output}/test_file_${ID}.root
# cp test_file.root test_file_${ID}.root

# rm test_file.root
