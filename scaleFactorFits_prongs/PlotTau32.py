#!/usr/bin/python
import os

def replace_steer(PathRootFile, PlotName):
    PathPlots="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/PreFit_prongs/"
    filename="/nfs/dust/cms/user/schwarzd/SFramePlotter_TopTagging/toptagging_prongs"
    oldfile = open(filename+".steer",'r')       # open file for read
    newfile = open("toptagging_prongs_temp.steer", 'w') # create new file to write

    pattern1 = "fCycleName"
    pattern2 = "fOutputPsFile"
    for line in oldfile:
        line = line.strip('\r\n')  # it's always a good behave to strip what you read from files

        # if line matches a pattern, replace the line
        if pattern1 in line:
            line = 'fCycleName = "' + PathRootFile+'";'
        if pattern2 in line:
            line = 'fOutputPsFile = "'+PathPlots+PlotName+'.ps";'
        newfile.write(line + '\n')

    # close both files
    oldfile.close()
    newfile.close()

    # now rename file to replace original
    # os.rename(filename+"_temp.steer", filename+".steer")

#######
PathPlots="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/PreFit_prongs/"
PathRootFile= "/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/TopTagging/scaleFactorFits_prongs/"
jets = ["PUPPI", "HOTVR"]

for jet in jets:
    rootname=PathRootFile+"thetaFile_tau32_"+jet+".root"
    plotname="PreFit_tau32_"+jet
    replace_steer(rootname, plotname)
    os.system("/nfs/dust/cms/user/schwarzd/SFramePlotter_TopTagging/bin/Plots -f toptagging_prongs_temp.steer")
    os.chdir(PathPlots)
    os.system("epstopdf PreFit_tau32_"+jet+"_Main_tau32.eps")
