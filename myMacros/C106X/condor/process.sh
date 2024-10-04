dataset_version=$1
training_filelist=$2
test_filelist=$3
ncoffea=$4
config=$5


# mamba activate b_hive
# source /home/llr/cms/strazdas/b-hive/setup.sh

source /home/llr/cms/strazdas/.bashrc

echo $DATA_PATH

law run DatasetConstructorTask --dataset-version $dataset_version --training-filelist $training_filelist --test-filelist $test_filelist --coffea-worker $ncoffea --config $config