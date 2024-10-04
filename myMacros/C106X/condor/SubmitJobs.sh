#!/bin/sh                                                                                                                                                                                    

#### Use: source SubmitJobs.sh

dataset=LowEGJet
queue=long

mkdir -p Log
SubmissionFile=${dataset}.submit
Input=${dataset}_data_files_aggrTMVA.txt
Output=/data_CMS/cms/kalipoliti/compact_trees/

echo "## Use: condor_submit -name llrt3condor ${SubmissionFile}" > $SubmissionFile
echo "" >> $SubmissionFile
echo "Universe   = vanilla" >> $SubmissionFile
echo "Executable = `pwd`/StepProcess.sh" >> $SubmissionFile
echo "+JobFlavour = microcentury" >> $SubmissionFile
# echo "transfer_input_files = Efficiency_all_dca.root" >> $SubmissionFile
echo "should_transfer_files = YES" >> $SubmissionFile
echo "Proxy_path=/afs/cern.ch/user/l/lkalipol/private/proxy/x509up_u137183" >> $SubmissionFile

# echo "transfer_output_files = DONE.txt" >> $SubmissionFile                                                                                                                                 
echo >> $SubmissionFile
Count=0

while IFS= read -r line; do
    echo "Arguments = \$(Proxy_path) `pwd` ${line} ${Output} ${Count}" >> $SubmissionFile
    echo 'Output    = Log/out.$(Process)' >> $SubmissionFile
    echo 'Error     = Log/err.$(Process)' >> $SubmissionFile
    echo 'Log       = Log/log.$(Process)' >> $SubmissionFile
    echo "T3Queue = ${queue}" >> $SubmissionFile
    echo 'WNTag=el7' >> $SubmissionFile
    echo '+SingularityCmd = ""' >> $SubmissionFile
    echo 'include : /opt/exp_soft/cms/t3/t3queue |' >> $SubmissionFile
    # echo 'Queue' >> $SubmissionFile
Count=$((Count+1))
done < $Input

#condor_submit $SubmissionFile

