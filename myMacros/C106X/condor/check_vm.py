import os

jobs = []
contents = os.listdir("./log")
for fout in contents:
    if "out" not in fout: continue
    with open(os.path.join("log",fout)) as f:
        lines = f.readlines()
        hostname = lines[1]
        if "vm" in hostname:
            jobid = fout.split('_')[2].split('.')[0]
            os.system("cp log/log_3699092_{}.log /data_CMS/cms/kalipoliti/jobs_on_vm/".format(jobid))