#!/usr/bin/python
import os
import sys

def replace_steer(PathRootFile, PlotName, year):
    PathPlots="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/"
    if year == "2018":
        filename="/nfs/dust/cms/user/schwarzd/SFramePlotter_TopTagging/toptagging_prongs"
    if year == "2017":
        filename="/nfs/dust/cms/user/schwarzd/SFramePlotter_TopTagging/toptagging_prongs_2017"
    if year == "2016":
        filename="/nfs/dust/cms/user/schwarzd/SFramePlotter_TopTagging/toptagging_prongs_2016"
    oldfile = open(filename+".steer",'r')       # open file for read
    newfile = open("toptagging_prongs_temp.steer", 'w') # create new file to write

    drawcms = False

    pattern1 = "fCycleName"
    pattern2 = "fOutputPsFile"
    pattern3 = "bForPrelim"
    pattern4 = "bIsHOTVR"

    for line in oldfile:
        line = line.strip('\r\n')  # it's always a good behave to strip what you read from files

        # if line matches a pattern, replace the line
        if pattern1 in line:
            line = 'fCycleName = "' + PathRootFile+'";'
        if pattern2 in line:
            line = 'fOutputPsFile = "'+PathPlots+PlotName+'.ps";'
        if pattern3 in line and not drawcms:
            line = 'bForPrelim = false;'
        if pattern4 in line and "HOTVR" in PlotName:
            line = 'bIsHOTVR = true;'
        newfile.write(line + '\n')

    # close both files
    oldfile.close()
    newfile.close()

    # now rename file to replace original
    # os.rename(filename+"_temp.steer", filename+".steer")

####### MAIN
year = "2018"
if len(sys.argv) > 1:
    year = sys.argv[1]

PathPlots="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/"
PathRootFile= "/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/TopTagging/scaleFactorFits_prongs/"
jets = ["PUPPI", "HOTVR"]
# jets = ["PUPPI"]
for jet in jets:
    rootname=PathRootFile+"Prefit_incl_mass_"+year+"_"+jet
    plotname="PreFit_incl_"+year+"_"+jet+"_sys"
    if jet == "PUPPI":
        rootname += "_wp2.root"
        plotname += "_wp2"
    if jet == "HOTVR":
        rootname += ".root"

    replace_steer(rootname, plotname, year)
    os.system("/nfs/dust/cms/user/schwarzd/SFramePlotter_TopTagging/bin/Plots -f toptagging_prongs_temp.steer")

    os.chdir(PathPlots)
    if jet == "PUPPI":
        os.system("epstopdf PreFit_incl_"+year+"_"+jet+"_sys_wp2_Main_Mass_pass.eps")
        os.system("epstopdf PreFit_incl_"+year+"_"+jet+"_sys_wp2_Main_Mass_fail.eps")
    if jet == "HOTVR":
        os.system("epstopdf PreFit_incl_"+year+"_"+jet+"_sys_Main_Mass_pass.eps")
        os.system("epstopdf PreFit_incl_"+year+"_"+jet+"_sys_Main_Mass_fail.eps")
