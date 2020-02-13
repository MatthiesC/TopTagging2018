#!/usr/bin/python
import os
import sys

def write_latex():
    jets = ["PUPPI", "CHS", "HOTVR"]
    years = ["2016", "2017"]
    outfile = open("Comparison.tex","w")
    outfile.write("\\documentclass{beamer}\n")
    outfile.write("\\usepackage[english]{babel}\n")
    outfile.write("\\usepackage{graphicx} \n")
    outfile.write("\\usepackage{epstopdf}\n")
    outfile.write("\\usetheme{Copenhagen}\n")
    outfile.write("\\usecolortheme{beaver}\n")
    outfile.write("\\begin{document}\n")
    for year in years:
        for jet in jets:
            if "HOTVR" in jet:
                wps = [""];
                if year == "2017":
                    continue
            if "PUPPI" in jet:
                wps = ["wp1", "wp2", "wp3", "wp4", "wp5", "wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"]
            if "CHS" in jet:
                wps = ["wp2", "wp3", "wp4", "wp5", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"]
            for wp in wps:
                # begin new frame
                outfile.write("%--------------------------------------------\n")
                outfile.write("\\begin{frame}\n")
                wpname = wp
                if "_btag" in wpname:
                    wpname = wpname.replace("_btag", " btag")
                outfile.write("\\frametitle{"+year+", "+jet+", "+wpname+"}\n")
                outfile.write("\\begin{columns}\n")
                #----1st column
                #TTbar merged
                filename = year+"_"+jet+"_"+wp+"_mergedTop"
                outfile.write("\\begin{column}{.33\\textwidth}\n")
                outfile.write("\\centering\n")
                outfile.write("TTbar merged \\newline \n")
                outfile.write("\\includegraphics[width=\\textwidth]{")
                outfile.write(filename+".pdf")
                outfile.write("} \n")
                outfile.write("\\end{column}\n")
                #----2nd column
                #TTbar semimerged
                filename = year+"_"+jet+"_"+wp+"_semimerged"
                outfile.write("\\begin{column}{.33\\textwidth}\n")
                outfile.write("\\centering\n")
                outfile.write("TTbar semimerged  \\newline \n")
                outfile.write("\\includegraphics[width=\\textwidth]{")
                outfile.write(filename+".pdf")
                outfile.write("} \n")
                outfile.write("\\end{column}\n")
                #----3rd column
                #TTbar not merged
                filename = year+"_"+jet+"_"+wp+"_notmerged"
                # filename = "SF_"+jet+"_"+wp+"_notmergedTop_"+yearstring
                outfile.write("\\begin{column}{.33\\textwidth}\n")
                outfile.write("\\centering\n")
                outfile.write("TTbar not merged  \\newline \n")
                outfile.write("\\includegraphics[width=\\textwidth]{")
                outfile.write(filename+".pdf")
                outfile.write("} \n")
                outfile.write("\\end{column}\n")
                # end frame
                outfile.write("\\end{columns}\n")
                outfile.write("\\end{frame}\n")
    #end document
    outfile.write("\\end{document}")
    outfile.close()



####### MAIN
PathPlots="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/CompareToTorben/"
os.system("./CompareToTorben")
os.chdir(PathPlots)
write_latex()
os.system("pdflatex Comparison.tex")
