# -*- coding: utf-8 -*-

import numpy
import ROOT
from array import array

import os
import sys

sys.path.append('./')
from PostFitUncertainties import *
from PostFitCorrelations import *
from SFcalculation import *
from ThetaPostFitPlot import *

outdir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/"

def run(fname_stat, fname_sys, outname_stat, outname_sys, calc_sfs = True, write_report = False):
    if write_report:report.reopen_file()

    year = '2017'
    inputpath = "thetaFiles_NoFSRnotmerged/"+year+"/mass_sub/"
    print 'use files in', inputpath

    model_stat = build_model_from_rootfile(inputpath+fname_stat, include_mc_uncertainties=True)
    model_stat.fill_histogram_zerobins()

    # if '300to400' in fname_sys and 'PUPPI' in fname_sys and 'wp3_btag' in fname_sys:
    #     model_sys = build_model_from_rootfile(inputpath+fname_sys,  histogram_filter=(lambda s:( not s.count('Btag') and not s.count('MUID') and not s.count('MUTR') and not s.count('PDF') and not s.count('PU')) ) , include_mc_uncertainties=True)
    # else:
    #     model_sys = build_model_from_rootfile(inputpath+fname_sys,  histogram_filter=(lambda s:( not s.count('Btag') and not s.count('MUID') and not s.count('MUTR')) ) , include_mc_uncertainties=True)

    model_sys = build_model_from_rootfile(inputpath+fname_sys,
    histogram_filter=(lambda s:(
    not s.count('MuonID')
    and not s.count('Trigger')
    and not s.count('PU')
    and not s.count('Btag')
    # and not s.count('JEC')
    # and not s.count('JER')
    # and not s.count('FSR')
    # and not s.count('ScaleMuR')
    # and not s.count('ScaleMuF')
    # and not s.count('TagEffi_3prong')
    # and not s.count('TagEffi_2prong')
    # and not s.count('TagEffi_1prong')
    ) ) , include_mc_uncertainties=True)


    model_sys.fill_histogram_zerobins()



    print '================================================='
    print fname_sys
    print '================================================='

    rate_unc_TTbar = math.log(1.2)
    rate_unc_TTbar_merged = math.log(1.2)
    rate_unc_TTbar_semimerged = math.log(1.2)
    rate_unc_SingleTop = math.log(1.2)
    rate_unc_WJets = math.log(1.2)
    rate_unc_DY = math.log(1.2)
    rate_unc_QCD = math.log(2.0)
    rate_unc_BKG = math.log(1.6)
    TagEffi_3prong_unc = 0.8
    TagEffi_2prong_unc = 0.5
    TagEffi_1prong_unc = 0.5

    # parameter tweaks PUPPI
    ############################################################################
    # wp1
    if '300to400' in fname_sys and 'PUPPI' in fname_sys and "wp1" in fname_sys:
        TagEffi_3prong_unc = 0.8
        TagEffi_2prong_unc = 0.7
        TagEffi_1prong_unc = 0.7
        rate_unc_QCD = math.log(1.8)
    if '400to480' in fname_sys and 'PUPPI' in fname_sys and "wp1" in fname_sys:
        TagEffi_3prong_unc = 0.5
        rate_unc_QCD = math.log(1.8)
    if '_600' in fname_sys and 'PUPPI' in fname_sys and "wp1" in fname_sys:
        rate_unc_QCD = math.log(1.8)
    ############################################################################
    #wp2
    if '300to400' in fname_sys and 'PUPPI' in fname_sys and "wp2" in fname_sys:
        TagEffi_3prong_unc = 0.5
        TagEffi_2prong_unc = 0.5
        TagEffi_1prong_unc = 0.5
        rate_unc_QCD = math.log(1.8)
        rate_unc_WJets = math.log(1.1)
    if '400to480' in fname_sys and 'PUPPI' in fname_sys and "wp2" in fname_sys:
        TagEffi_3prong_unc = 0.5
        rate_unc_QCD = math.log(1.8)
    if '480to600' in fname_sys and 'PUPPI' in fname_sys and "wp2" in fname_sys:
        TagEffi_3prong_unc = 0.7
        rate_unc_QCD = math.log(1.6)
        rate_unc_WJets = math.log(1.4)
    ############################################################################
    #wp3
    if '300to400' in fname_sys and 'PUPPI' in fname_sys and "wp3" in fname_sys:
        TagEffi_3prong_unc = 0.5
        TagEffi_2prong_unc = 0.5
        TagEffi_1prong_unc = 0.5
        rate_unc_QCD = math.log(1.6)
        rate_unc_WJets = math.log(1.5)
    if '400to480' in fname_sys and 'PUPPI' in fname_sys and "wp3" in fname_sys:
        TagEffi_3prong_unc = 0.5
        TagEffi_2prong_unc = 0.5
        TagEffi_1prong_unc = 0.5
        rate_unc_QCD = math.log(1.7)
    if '480to600' in fname_sys and 'PUPPI' in fname_sys and "wp3" in fname_sys:
        rate_unc_QCD = math.log(1.8)
    if '_600' in fname_sys and 'PUPPI' in fname_sys and "wp3" in fname_sys:
        rate_unc_QCD = math.log(1.6)
    ############################################################################
    #wp4
    if '300to400' in fname_sys and 'PUPPI' in fname_sys and "wp4" in fname_sys:
        TagEffi_3prong_unc = 0.5
        rate_unc_QCD = math.log(1.6)
        rate_unc_WJets = math.log(1.4)
    if '400to480' in fname_sys and 'PUPPI' in fname_sys and "wp4" in fname_sys:
        TagEffi_3prong_unc = 0.5
        rate_unc_QCD = math.log(1.6)
        rate_unc_WJets = math.log(1.4)
    ############################################################################
    #wp5
    if '300to400' in fname_sys and 'PUPPI' in fname_sys and "wp5" in fname_sys:
        TagEffi_3prong_unc = 0.5
        TagEffi_2prong_unc = 0.3
        TagEffi_1prong_unc = 0.3
        rate_unc_QCD = math.log(1.8)
    if '400to480' in fname_sys and 'PUPPI' in fname_sys and "wp5" in fname_sys:
        TagEffi_3prong_unc = 0.5
        rate_unc_QCD = math.log(1.6)
        rate_unc_WJets = math.log(1.4)
    ############################################################################
    ############################################################################
    ############################################################################
    #HOTVR
    if '200to250' in fname_sys and 'HOTVR' in fname_sys:
        rate_unc_QCD = math.log(1.6)
        rate_unc_WJets = math.log(1.4)
    if '250to300' in fname_sys and 'HOTVR' in fname_sys:
        rate_unc_QCD = math.log(1.6)
        rate_unc_WJets = math.log(1.4)
    ############################################################################
    ############################################################################
    ############################################################################
    # CHS
    # wp 2
    if '400to480' in fname_sys and 'CHS' in fname_sys and "wp2" in fname_sys:
        rate_unc_QCD = math.log(1.7)
    if '480to600' in fname_sys and 'CHS' in fname_sys and "wp2" in fname_sys:
        rate_unc_QCD = math.log(1.7)
    # wp 3
    if '400to480' in fname_sys and 'CHS' in fname_sys and "wp3" in fname_sys:
        rate_unc_QCD = math.log(1.7)
    # wp 4
    if '400to480' in fname_sys and 'CHS' in fname_sys and "wp4" in fname_sys:
        if 'btag' in fname_sys:
            rate_unc_QCD = math.log(1.6)
    # wp 5
    if '300to400' in fname_sys and 'CHS' in fname_sys and "wp5" in fname_sys:
        rate_unc_QCD = math.log(1.7)

    print "  - 3prong", TagEffi_3prong_unc
    print "  - 2prong", TagEffi_2prong_unc
    print "  - 2prong", TagEffi_1prong_unc
    print "  - merged rate", rate_unc_TTbar_merged
    print "  - semimerged rate", rate_unc_TTbar_semimerged
    print "  - background rate", rate_unc_BKG

    model_sys.distribution.set_distribution("TagEffi_3prong", 'gauss', 0.0, TagEffi_3prong_unc, [-float("inf"), float("inf")])
    model_sys.distribution.set_distribution("TagEffi_2prong", 'gauss', 0.0, TagEffi_2prong_unc, [-float("inf"), float("inf")])
    model_sys.distribution.set_distribution("TagEffi_1prong", 'gauss', 0.0, TagEffi_1prong_unc, [-float("inf"), float("inf")])

    # model_stat.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar, procname='mergedTop_2017v2',obsname='*')
    # model_sys.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar , procname='mergedTop_2017v2',obsname='*')
    # model_stat.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar, procname='semimergedTop_2017v2',obsname='*')
    # model_sys.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar , procname='semimergedTop_2017v2',obsname='*')
    # model_stat.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar, procname='notmergedTop_2017v2',obsname='*')
    # model_sys.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar , procname='notmergedTop_2017v2',obsname='*')

    model_stat.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar, procname='TTbar_mergedTop_2017v2',obsname='*')
    model_sys.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar , procname='TTbar_mergedTop_2017v2',obsname='*')
    model_stat.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar, procname='TTbar_semimerged_2017v2',obsname='*')
    model_sys.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar , procname='TTbar_semimerged_2017v2',obsname='*')
    model_stat.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar, procname='TTbar_notmerged_2017v2',obsname='*')
    model_sys.add_lognormal_uncertainty('TTbar_rate', rate_unc_TTbar , procname='TTbar_notmerged_2017v2',obsname='*')

    print 'add lognormal uncert BKG'
    model_sys.add_lognormal_uncertainty('SingleTop_rate', rate_unc_SingleTop, procname='SingleTop_mergedTop_2017v2',obsname='*')
    model_sys.add_lognormal_uncertainty('SingleTop_rate', rate_unc_SingleTop, procname='SingleTop_semimerged_2017v2',obsname='*')
    model_sys.add_lognormal_uncertainty('SingleTop_rate', rate_unc_SingleTop, procname='SingleTop_notmerged_2017v2',obsname='*')
    model_sys.add_lognormal_uncertainty('WJets_rate', rate_unc_WJets, procname='WJets_2017v2',obsname='*')
    model_sys.add_lognormal_uncertainty('DY_rate', rate_unc_DY, procname='DYJets_2017v2',obsname='*')
    model_sys.add_lognormal_uncertainty('QCD_rate', rate_unc_QCD, procname='QCD_2017v2',obsname='*')
    # model_sys.add_lognormal_uncertainty('BKG_rate', rate_unc_BKG, procname='Backgrounds_2017v2',obsname='*')

    for p in model_stat.get_processes('Mass_pass'):
        print(p)

    options = Options()
    options.set('minimizer', 'strategy', 'robust')

    # if '200to250' in fname_sys and 'HOTVR' in fname_sys:
    #     options.set('minimizer', 'minuit_tolerance_factor', '8')
    # if '250to300' in fname_sys and 'HOTVR' in fname_sys:
    #     options.set('minimizer', 'minuit_tolerance_factor', '6')

    n_name_stat = deepcopy(outname_stat)
    n_name_stat = n_name_stat.replace(".root","_")
    n_name_sys = deepcopy(outname_sys)
    n_name_sys = n_name_sys.replace(".root","_")

    print '============'
    print 'stat'
    print '============'
    mle_output_stat = mle(model_stat, input='data', n=100, with_covariance=True, chi2=True, options=options, signal_process_groups ={'background_only':[]})

    print '============'
    print 'sys'
    print '============'
    mle_output_sys = mle(model_sys, input='data', n=100, with_covariance=True, chi2=True, options=options, signal_process_groups ={'background_only':[]})

    print '============'
    print 'plot/write'
    print '============'

    PlotPostFitCorrelations(model_stat, mle_output_stat['background_only'], outdir+"fitResults_FSR_f_prongs/"+year+"/nuissance/Corr_"+n_name_stat)
    PlotPostFitCorrelations(model_sys, mle_output_sys['background_only'], outdir+"fitResults_FSR_f_prongs/"+year+"/nuissance/Corr_"+n_name_sys)
    writeOutputFile(inputpath+fname_stat,  outdir+"fitResults_FSR_f_prongs/"+year+"/mass_sub/"+outname_stat, mle_output_stat['background_only'], model_stat)
    writeOutputFile(inputpath+fname_sys,  outdir+"fitResults_FSR_f_prongs/"+year+"/mass_sub/"+outname_sys, mle_output_sys['background_only'], model_sys)

    if calc_sfs:
        sfs = SFcalculation(inputpath+fname_stat, inputpath+fname_sys, mle_output_stat['background_only'], mle_output_sys['background_only'], model_stat, model_sys)

        if 'CHS' in fname_stat:
            sfs.setMassWindow(105,220)
        elif 'PUPPI' in fname_stat:
            sfs.setMassWindow(105,210)
        else:
            sfs.setMassWindow(140,220)
        if not 'HOTVR' in fname_stat:
            print 'set extra mass files'
            sfs.setMassEffHists("thetaFiles_NoFSRnotmerged/"+year+"/mass_sub/fine/"+fname_stat);
            sfs.setMassEffHistsSys("thetaFiles_NoFSRnotmerged/"+year+"/mass_sub/fine/"+fname_sys);

        dictOut_stat = sfs.calcEfficiencies('stat')
        dictOut_sys = sfs.calcEfficiencies('sys')

    for pf_vals in mle_output_stat.itervalues():
        del pf_vals['__nll']
        del pf_vals['__cov']
    for pf_vals in mle_output_sys.itervalues():
        del pf_vals['__nll']
        del pf_vals['__cov']

    postfit_stat = ThetaPostFitPlot(mle_output_stat)
    postfit_stat.make_plots(outdir+"fitResults_FSR_f_prongs/"+year+"/nuissance/",n_name_stat)

    postfit_sys = ThetaPostFitPlot(mle_output_sys)
    postfit_sys.make_plots(outdir+"fitResults_FSR_f_prongs/"+year+"/nuissance/",n_name_sys)

    if calc_sfs:
        return [dictOut_stat, dictOut_sys]

###loop over taggers and wps

print 'starting script'
bins = array('d', [300, 400, 480, 600, 1100])
calculate_scaleFactors = True

#run("thetaFile_400_PUPPI_sys_.root", "Hists_400_PUPPI_sys.root")

# wps = ["_wp1", "_wp2", "_wp3", "_wp4", "_wp5", "_wp1_btag", "_wp2_btag", "_wp3_btag", "_wp4_btag", "_wp5_btag"]
wps = []
for wp in wps:
    print 'run PUPPI, ',wp
    d_300to400 = run("thetaFile_300to400_PUPPI_stat"+wp+".root", "thetaFile_300to400_PUPPI_sys"+wp+".root", "Hists_300to400_PUPPI_stat"+wp+".root", "Hists_300to400_PUPPI_sys"+wp+".root", calculate_scaleFactors)
    d_400to480 = run("thetaFile_400to480_PUPPI_stat"+wp+".root", "thetaFile_400to480_PUPPI_sys"+wp+".root", "Hists_400to480_PUPPI_stat"+wp+".root", "Hists_400to480_PUPPI_sys"+wp+".root", calculate_scaleFactors)
    d_480to600 = run("thetaFile_480to600_PUPPI_stat"+wp+".root", "thetaFile_480to600_PUPPI_sys"+wp+".root", "Hists_480to600_PUPPI_stat"+wp+".root", "Hists_480to600_PUPPI_sys"+wp+".root", calculate_scaleFactors)
    d_600 = run("thetaFile_600_PUPPI_stat"+wp+".root", "thetaFile_600_PUPPI_sys"+wp+".root", "Hists_600_PUPPI_stat"+wp+".root", "Hists_600_PUPPI_sys"+wp+".root", calculate_scaleFactors)

    if calculate_scaleFactors:
        dicts_stat = [d_300to400[0], d_400to480[0], d_480to600[0], d_600[0]]
        dicts_sys = [d_300to400[1], d_400to480[1], d_480to600[1], d_600[1]]
        WriteEffGraphs_separate(outdir+"ScaleFactors_FSR_f_prongs/2017/eff_hists_PUPPI"+wp+".root", dicts_stat, dicts_sys, bins)


wpsHOTVR = ["_"]
# wpsHOTVR = []
bins = array('d', [200, 250, 300, 400, 480, 600, 1100])
for wp in wpsHOTVR:
    print 'run HOTVR, ',wp
    d_200to250_HOTVR = run("thetaFile_200to250_HOTVR_stat"+wp+".root", "thetaFile_200to250_HOTVR_sys"+wp+".root", "Hists_200to250_HOTVR_stat"+wp+".root", "Hists_200to250_HOTVR_sys"+wp+".root", calculate_scaleFactors)
    d_250to300_HOTVR = run("thetaFile_250to300_HOTVR_stat"+wp+".root", "thetaFile_250to300_HOTVR_sys"+wp+".root", "Hists_250to300_HOTVR_stat"+wp+".root", "Hists_250to300_HOTVR_sys"+wp+".root", calculate_scaleFactors)
    d_300to400_HOTVR = run("thetaFile_300to400_HOTVR_stat"+wp+".root", "thetaFile_300to400_HOTVR_sys"+wp+".root", "Hists_300to400_HOTVR_stat"+wp+".root", "Hists_300to400_HOTVR_sys"+wp+".root", calculate_scaleFactors)
    d_400to480_HOTVR = run("thetaFile_400to480_HOTVR_stat"+wp+".root", "thetaFile_400to480_HOTVR_sys"+wp+".root", "Hists_400to480_HOTVR_stat"+wp+".root", "Hists_400to480_HOTVR_sys"+wp+".root", calculate_scaleFactors)
    d_480to600_HOTVR = run("thetaFile_480to600_HOTVR_stat"+wp+".root", "thetaFile_480to600_HOTVR_sys"+wp+".root", "Hists_480to600_HOTVR_stat"+wp+".root", "Hists_480to600_HOTVR_sys"+wp+".root", calculate_scaleFactors)
    d_600_HOTVR = run("thetaFile_600_HOTVR_stat"+wp+".root", "thetaFile_600_HOTVR_sys"+wp+".root", "Hists_600_HOTVR_stat"+wp+".root", "Hists_600_HOTVR_sys"+wp+".root", calculate_scaleFactors)

    if calculate_scaleFactors:
        dicts_stat_HOTVR = [d_200to250_HOTVR[0], d_250to300_HOTVR[0], d_300to400_HOTVR[0], d_400to480_HOTVR[0], d_480to600_HOTVR[0], d_600_HOTVR[0]]
        dicts_sys_HOTVR = [d_200to250_HOTVR[1], d_250to300_HOTVR[1], d_300to400_HOTVR[1], d_400to480_HOTVR[1], d_480to600_HOTVR[1], d_600_HOTVR[1]]
        WriteEffGraphs_separate(outdir+"ScaleFactors_FSR_f_prongs/2017/eff_hists_HOTVR"+wp+".root", dicts_stat_HOTVR, dicts_sys_HOTVR, bins)


# wpsCHS = ["_wp2", "_wp3", "_wp4", "_wp5", "_wp2_btag", "_wp3_btag", "_wp4_btag", "_wp5_btag"]
wpsCHS = []
bins = array('d', [300, 400, 480, 600, 1100])
for wp in wpsCHS:
    print 'run CHS, ',wp
    d_300to400_CHS = run("thetaFile_300to400_CHS_stat"+wp+".root", "thetaFile_300to400_CHS_sys"+wp+".root", "Hists_300to400_CHS_stat"+wp+".root", "Hists_300to400_CHS_sys"+wp+".root", calculate_scaleFactors)
    d_400to480_CHS = run("thetaFile_400to480_CHS_stat"+wp+".root", "thetaFile_400to480_CHS_sys"+wp+".root", "Hists_400to480_CHS_stat"+wp+".root", "Hists_400to480_CHS_sys"+wp+".root", calculate_scaleFactors)
    d_480to600_CHS = run("thetaFile_480to600_CHS_stat"+wp+".root", "thetaFile_480to600_CHS_sys"+wp+".root", "Hists_480to600_CHS_stat"+wp+".root", "Hists_480to600_CHS_sys"+wp+".root", calculate_scaleFactors)
    d_600_CHS = run("thetaFile_600_CHS_stat"+wp+".root", "thetaFile_600_CHS_sys"+wp+".root", "Hists_600_CHS_stat"+wp+".root", "Hists_600_CHS_sys"+wp+".root", calculate_scaleFactors)

    if calculate_scaleFactors:
        dicts_stat_CHS = [d_300to400_CHS[0], d_400to480_CHS[0], d_480to600_CHS[0], d_600_CHS[0]]
        dicts_sys_CHS = [d_300to400_CHS[1], d_400to480_CHS[1], d_480to600_CHS[1], d_600_CHS[1]]
        WriteEffGraphs_separate(outdir+"ScaleFactors_FSR_f_prongs/2017/eff_hists_CHS"+wp+".root", dicts_stat_CHS, dicts_sys_CHS, bins)
