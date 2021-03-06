#!/usr/bin/python
import os

def write_latex(PathPlots, jets, wps_PUPPI, wps_HOTVR):
    outfile = open(PathPlots+"SFLatex.tex","w")
    outfile.write("\\documentclass[aspectratio=169]{beamer}\n")
    outfile.write("\\usepackage[english]{babel}\n")
    outfile.write("\\usepackage{graphicx} \n")
    outfile.write("\\usepackage{epstopdf}\n")
    outfile.write("\\usetheme{Copenhagen}\n")
    outfile.write("\\usecolortheme{beaver}\n")
    outfile.write("\\begin{document}\n")
    for jet in jets:
        if "HOTVR" in jet:
            wps = wps_HOTVR
        if "PUPPI" in jet:
            wps = wps_PUPPI
        for wp in wps:
            # begin new frame
            outfile.write("%--------------------------------------------\n")
            outfile.write("\\begin{frame}\n")
            wpname = wp
            if "_btag" in wpname:
                wpname = wpname.replace("_btag", " btag")
            outfile.write("\\frametitle{"+jet+", "+wpname+"}\n")
            outfile.write("\\begin{columns}\n")
            #----1st column
            #TTbar merged
            filename = "SF_"+jet+"_"+wp+"_TTbar_mergedTop_2018"
            outfile.write("\\begin{column}{.33\\textwidth}\n")
            outfile.write("\\centering\n")
            outfile.write("TTbar merged Top\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.8\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            # SingleTop
            filename = "SF_"+jet+"_"+wp+"_SingleTop_2018"
            outfile.write("\\centering\n")
            outfile.write("Single Top\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.8\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            outfile.write("\\end{column}\n")
            #----2nd column
            #TTbar semi merged
            filename = "SF_"+jet+"_"+wp+"_TTbar_semimerged_2018"
            outfile.write("\\begin{column}{.33\\textwidth}\n")
            outfile.write("\\centering\n")
            outfile.write("TTbar semi merged\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.8\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            # WJets
            filename = "SF_"+jet+"_"+wp+"_WJets_2018"
            outfile.write("\\centering\n")
            outfile.write("W+Jets\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.8\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            outfile.write("\\end{column}\n")
            #----3rd column
            #TTbar not merged
            filename = "SF_"+jet+"_"+wp+"_TTbar_notmerged_2018"
            outfile.write("\\begin{column}{.33\\textwidth}\n")
            outfile.write("\\centering\n")
            outfile.write("TTbar not merged\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.8\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            # DY
            filename = "SF_"+jet+"_"+wp+"_DYJets_2018"
            outfile.write("\\centering\n")
            outfile.write("Z+Jets\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.8\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            outfile.write("\\end{column}\n")
            # end frame
            outfile.write("\\end{columns}\n")
            outfile.write("\\end{frame}\n")
    #end document
    outfile.write("\\end{document}")
    outfile.close()



####### MAIN
PathPlots="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors/"
jets = ["PUPPI", "HOTVR"]
wps_PUPPI = ["wp1", "wp2", "wp3", "wp4", "wp5", "wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"]
wps_HOTVR = [""]

print "create SF plots..."
os.system("./PlotSF")
print "creating latex document..."
os.chdir(PathPlots)
write_latex(PathPlots, jets, wps_PUPPI, wps_HOTVR)
os.system("pdflatex SFLatex.tex")
