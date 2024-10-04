import subprocess

### Use: create a txt file with a list of (data) files from eos

# out_name = "file_lists/LowEGJet_aggrTMVA_withMCJPCalibration.txt"
# base_dir = "root://eos.grif.fr//eos/grif/cms/llr/store/user/lkalipol/bJet2017G/LowEGJet/"
out_name = "file_lists/dijet_aggrTMVA_genParton.txt"
print("fout: ", out_name)
# base_dir = "root://eos.grif.fr//eos/grif/cms/llr/store/user/lkalipol/bJet2023/QCD_PthatGT15_TuneCH3_5p02TeV_herwig7/"
base_dir = "root://eos.grif.fr//eos/grif/cms/llr/store/user/lkalipol/bJet2023/QCD_pThat-15_Dijet_TuneCP5_5p02TeV-pythia8/"

out = subprocess.check_output("gfal-ls " + base_dir, shell=True, encoding="utf-8")
top_dirs = [base_dir + dir + "/" for dir in out.split() if "dijet" in dir]

# print(top_dirs)

with open(out_name, "w") as fout:
    for top_dir in top_dirs:
        filter = "dijet_aggrTMVA_genParton"
        if filter not in top_dir: continue
        print(top_dir)

        out = subprocess.check_output("gfal-ls " + top_dir, shell=True, encoding="utf-8")
        date_dirs = [top_dir + dir + "/" for dir in out.split() if "_" in dir]
        date_dir = date_dirs[0] # only one date_dir per top_dir

        out = subprocess.check_output("gfal-ls " + date_dir, shell=True, encoding="utf-8")
        bottom_dirs = [date_dir + dir + "/" for dir in out.split() if "000" in dir and "1000" not in dir]
        print(bottom_dirs)

        for bottom_dir in bottom_dirs:
            out = subprocess.check_output("gfal-ls " + bottom_dir, shell=True, encoding="utf-8")
            files = [bottom_dir + f for f in out.split() if "root" in f]

            for f in files:
                fout.write("%s\n" % f)

