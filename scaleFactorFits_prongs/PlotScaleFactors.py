#!/usr/bin/python
import os
import sys

def write_latex(PathPlots, jets, wps_PUPPI, wps_HOTVR, year):
    outfile = open(PathPlots+"SFLatex.tex","w")
    outfile.write("\\documentclass[aspectratio=169]{beamer}\n")
    outfile.write("\\usepackage[english]{babel}\n")
    outfile.write("\\usepackage{graphicx} \n")
    outfile.write("\\usepackage{epstopdf}\n")
    outfile.write("\\usetheme{Copenhagen}\n")
    outfile.write("\\usecolortheme{beaver}\n")
    outfile.write("\\begin{document}\n")
    yearstring = "2018"
    if year == "2016":
        yearstring = "2016v3"
    if year == "2017":
        yearstring = "2017v2"
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
            filename = "SF_"+jet+"_"+wp+"_TTbar_mergedTop_"+yearstring
            outfile.write("\\begin{column}{.33\\textwidth}\n")
            outfile.write("\\centering\n")
            outfile.write("\\tiny TTbar merged\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.75\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            # SingleTop merged
            filename = "SF_"+jet+"_"+wp+"_SingleTop_mergedTop_"+yearstring
            outfile.write("\\centering\n")
            outfile.write("\\tiny Single Top merged\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.75\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            outfile.write("\\end{column}\n")
            #----2nd column
            #TTbar semimerged
            filename = "SF_"+jet+"_"+wp+"_TTbar_semimerged_"+yearstring
            # filename = "SF_"+jet+"_"+wp+"_semimergedTop_"+yearstring
            outfile.write("\\begin{column}{.33\\textwidth}\n")
            outfile.write("\\centering\n")
            outfile.write("\\tiny TTbar semimerged\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.75\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            # SingleTop semimerged
            filename = "SF_"+jet+"_"+wp+"_SingleTop_semimerged_"+yearstring
            outfile.write("\\centering\n")
            outfile.write("\\tiny Single Top semimerged\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.75\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            outfile.write("\\end{column}\n")
            #----3rd column
            #TTbar not merged
            filename = "SF_"+jet+"_"+wp+"_TTbar_notmerged_"+yearstring
            # filename = "SF_"+jet+"_"+wp+"_notmergedTop_"+yearstring
            outfile.write("\\begin{column}{.33\\textwidth}\n")
            outfile.write("\\centering\n")
            outfile.write("\\tiny TTbar not merged\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.75\\textwidth]{")
            outfile.write(filename+".pdf")
            outfile.write("} \n")
            outfile.write("\\end{figure}\n")
            # SingleTop semimerged
            filename = "SF_"+jet+"_"+wp+"_SingleTop_notmerged_"+yearstring
            outfile.write("\\centering\n")
            outfile.write("\\tiny Single Top not merged\n")
            outfile.write("\\begin{figure}\n")
            outfile.write("\\includegraphics[width=.75\\textwidth]{")
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
year = "2018"
if len(sys.argv) > 1:
    year = sys.argv[1]

PathPlots="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_prongs/"+year+"/"
jets = ["PUPPI", "HOTVR"]
# jets = ["PUPPI"]
# jets = ["HOTVR"]
wps_PUPPI = ["wp1", "wp2", "wp3", "wp4", "wp5", "wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"]
wps_HOTVR = [""]

print "create SF plots..."
os.system("./PlotSF "+year)
print "create Effi plots..."
os.system("./PlotEffi "+year)
print "creating latex document..."
os.chdir(PathPlots)
write_latex(PathPlots, jets, wps_PUPPI, wps_HOTVR, year)
os.system("pdflatex SFLatex.tex")
