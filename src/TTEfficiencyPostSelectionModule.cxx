#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/EventHists.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/TTbarGen.h"
#include "UHH2/common/include/AdditionalSelections.h"
#include <UHH2/common/include/JetCorrections.h>
#include <UHH2/common/include/ObjectIdUtils.h>
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/ElectronIds.h>
#include <UHH2/common/include/TopJetIds.h>
#include <UHH2/common/include/JetIds.h>
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/TopPtReweight.h"
#include "UHH2/TopTagging2018/include/TopTaggingSelections.h"
#include "UHH2/TopTagging2018/include/ProbeJetHists.h"
#include "UHH2/TopTagging2018/include/EfficiencyHists.h"
#include <UHH2/common/include/Utils.h>
#include "UHH2/TopTagging2018/include/TopTaggingUtils.h"
#include "UHH2/HOTVR/include/HOTVRIds.h"

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

class TTEfficiencyPostSelectionModule: public AnalysisModule {
public:

  explicit TTEfficiencyPostSelectionModule(Context & ctx);
  virtual bool process(Event & event) override;

private:

  //correctors
  std::unique_ptr<CommonModules> common;
  std::unique_ptr<uhh2::AnalysisModule> pileupRW;
  std::unique_ptr<TopJetCleaner> topjet_cleaner;

  //reweighting and scale factors
  std::vector<std::unique_ptr<AnalysisModule>> reweighting_modules;
  std::unique_ptr<uhh2::AnalysisModule> muo_tight_SF, muo_trigger_SF_before, muo_trigger_SF_after;
  std::unique_ptr<uhh2::AnalysisModule> sf_btag;

  std::unique_ptr<AnalysisModule> btagwAK8, subjet_btagwAK8;
  std::unique_ptr<AnalysisModule> subjet_btagwAK8_300to400, subjet_btagwAK8_400to480, subjet_btagwAK8_480to600, subjet_btagwAK8_600;
  std::unique_ptr<uhh2::AnalysisModule> scale_variation;
  std::unique_ptr<uhh2::AnalysisModule> ps_weights;
  std::unique_ptr<GenericJetResolutionSmearer> topjetJER_smearer;

  //selections
  std::unique_ptr<Selection> sel_badhcal;
  TopJetId hotvr_tag;
  // HOTVRTopTag hotvr_tag;

  std::unique_ptr<uhh2::Selection> hadronic_selection, lepton_jets_seletion, dilepton_selection, tau_jets_selection;
  std::unique_ptr<MergedSelection> merged_selection, mergedW_selection, mergedQB_selection, mergedEvent_selection;
  std::unique_ptr<MassDiffSelection> massDiff_selection;
  std::unique_ptr<DPhiMuBSelection> dphi_selection;
  std::unique_ptr<LeadingAddJetSelection> addJet_selection;
  std::unique_ptr<uhh2::Selection> ptW_sel;

  //histograms
  std::vector<std::unique_ptr<uhh2::Hists>> hists_before_sel;
  std::vector<std::unique_ptr<uhh2::Hists>> hists_after_sel;
  std::unique_ptr<uhh2::Hists> hists_notrigger, hists_trigger;
  std::unique_ptr<ProbeJetHists> hists_all, hists_all_200to250, hists_all_250to300, hists_all_300to400, hists_all_400, hists_all_400to550, hists_all_550 ,hists_tagged;
  std::unique_ptr<ProbeJetHists>  hists_dilepton, hists_lepton_jets, hists_taujets, hists_hadronic, hists_dilepton_400, hists_lepton_jets_400, hists_taujets_400, hists_hadronic_400;
  std::vector<std::vector<std::unique_ptr<ProbeJetHists>>> h_probe_all_pass, h_probe_mass_pass, h_probe_btag_pass, h_probe_mass_btag_pass;
  std::vector<std::vector<std::unique_ptr<ProbeJetHists>>> h_probe_all_fail, h_probe_mass_fail, h_probe_btag_fail, h_probe_mass_btag_fail;

  std::vector<std::unique_ptr<EfficiencyHists>> h_effi_300to500_all, h_effi_300to500_pass, h_effi_300to500_fail, h_effi_300to500_btag_pass, h_effi_300to500_btag_fail;
  std::vector<std::unique_ptr<EfficiencyHists>> h_effi_400to800_all, h_effi_400to800_pass, h_effi_400to800_fail, h_effi_400to800_btag_pass, h_effi_400to800_btag_fail;
  std::vector<std::unique_ptr<EfficiencyHists>> h_effi_480to600_all, h_effi_480to600_pass, h_effi_480to600_fail, h_effi_480to600_btag_pass, h_effi_480to600_btag_fail;
  std::vector<std::unique_ptr<EfficiencyHists>> h_effi_1000to2000_all, h_effi_1000to2000_pass, h_effi_1000to2000_fail, h_effi_1000to2000_btag_pass, h_effi_1000to2000_btag_fail;

  //variables and functions
  string mass_scale;
  bool useHTT, usePUPPI, useHOTVR;
  string version;
  bool fill_PDF;
  bool isMC;
  bool invert_merged_selection;
  bool merged_category;
  bool isTTbar;
  bool isSingleTop;

  const std::vector<double> pt_bins{-1,-400 , 200, 250, 300, 400, 480, 600};

  bool get_pt_cut(unsigned int bin, const double pt);
  bool get_tau32_cut(unsigned int bin, const double tau32, const std::vector<double> wps);
};


TTEfficiencyPostSelectionModule::TTEfficiencyPostSelectionModule(Context & ctx){

  //=============================
  // access configuaration values
  //=============================

  string testvalue = ctx.get("TestKey", "<not set>");
  cout << "TestKey in the configuration was: " << testvalue << endl;

  string triggerName = ctx.get("Trigger", "<not set>");

  isMC = (ctx.get("dataset_type") == "MC");

  useHTT = (ctx.get("useHTT", "<not set>") == "TRUE");
  usePUPPI = (ctx.get("usePUPPI", "<not set>") == "TRUE");
  useHOTVR = (ctx.get("useHOTVR", "<not set>") == "TRUE");

  fill_PDF = (ctx.get("fill_PDF", "FALSE") == "TRUE");

  if(usePUPPI) cout << "use PUPPI topjets" << endl;
  else cout << "use CHS topjets" << endl;

  if(useHTT) cout << "run the HTT" << endl;
  else cout << "run CMS tagger" << endl;

  version = ctx.get("dataset_version", "");

  string merged = ctx.get("MergedSelection", "<not set>");
  TString vers = (TString)version;
  if(vers.Contains("mergedTop")) merged = "mergedTop";
  if(vers.Contains("mergedW")) merged = "mergedW";
  if(vers.Contains("mergedQB")) merged = "mergedQB";
  if(vers.Contains("light")) merged = "light";
  if(vers.Contains("bkg")) merged = "bkg";
  if(vers.Contains("notmerged")) merged = "notmerged";
  if(vers.Contains("semimerged")) merged = "semimerged";

  JetId jetid = AndId<Jet>(JetPFID(JetPFID::WP_TIGHT_CHS), PtEtaCut(30.0, 2.4));
  TopJetId topjetid = AndId<TopJet>(JetPFID(JetPFID::WP_TIGHT_CHS), PtEtaCut(150., 2.4));
  topjet_cleaner.reset(new TopJetCleaner(ctx,topjetid));

  string PU_variation = "central";
  PU_variation = ctx.get("PU_variation","central");

  string BTag_variation = "central";
  BTag_variation = ctx.get("BTag_variation","central");

  string SubjetBTag_variation = "central";
  SubjetBTag_variation = ctx.get("SubjetBTag_variation","central");

  string MuonID_variation = "none";
  MuonID_variation = ctx.get("MuonID_variation","none");

  string MuonTrigger_variation = "none";
  MuonTrigger_variation = ctx.get("MuonTrigger_variation","none");

  string PS_variation = "central";
  PS_variation = ctx.get("PS_variation", "central");

  string TagEffi_variation = "central";
  TagEffi_variation = ctx.get("TagEffi_variation", "central");


  bool TopPtReweighting = false;
  TopPtReweighting = (ctx.get("TopPtReweight","FALSE")== "TRUE");


  if ( vers.Contains("TTbar") ) isTTbar = true;
  else                          isTTbar = false;

  if ( vers.Contains("ST") ) isSingleTop = true;
  else                       isSingleTop = false;



  Year year = extract_year(ctx);
  //===========================
  //setup corrections
  //===========================

  common.reset(new CommonModules());
  common->set_HTjetid(jetid);
  common->disable_jec();
  common->disable_jersmear();
  common->disable_lumisel();
  common->disable_metfilters();
  common->disable_pvfilter();
  common->disable_jetpfidfilter();
  common->init(ctx, PU_variation);
  cout << "common init" <<endl;

  sel_badhcal.reset(new BadHCALSelection(ctx));

  //==============================
  //reweighting and scale factors
  //==============================

  // if (version == "TTbar_Incl" || version ==  "TTbar_700to1000" || version ==  "TTbar_1000toInf") {
  if ( vers.Contains("TTbar") ) {
    reweighting_modules.emplace_back(new TTbarGenProducer(ctx, "ttbargen", true));
    // reweighting_modules.emplace_back(new TopPtReweight(ctx, 0.156, -0.00137, "ttbargen", "weight_ttbar", true)); //8TeV
    if(TopPtReweighting) reweighting_modules.emplace_back(new TopPtReweight(ctx, 0.0615, -0.0005, "ttbargen", "weight_ttbar", true)); //13TeV
  }

  ps_weights.reset(new PartonShowerWeight(ctx, PS_variation));

  BTag::algo btag_algo = BTag::DEEPJET;
  BTag::wp btag_wp = BTag::WP_MEDIUM;
  cout << "before btag sf module" << endl;
  sf_btag.reset(new MCBTagScaleFactor(ctx, btag_algo, btag_wp, "jets", BTag_variation, "mujets", "incl", "MCBtagEfficiencies"));
  cout << "after btag sf module" << endl;

  TString muonID_file, muonTR_file;
  if     (year == Year::is2016v3){
    muo_tight_SF.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root","NUM_TightID_DEN_genTracks_eta_pt",1, "tightID", false, MuonID_variation));
    muo_trigger_SF_before.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonTrigger_EfficienciesAndSF_average_RunBtoH.root","IsoMu50_OR_IsoTkMu50_PtEtaBins",1, "muonTrigger", false, MuonTrigger_variation));
    muo_trigger_SF_before.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonTrigger_EfficienciesAndSF_average_RunBtoH.root","IsoMu50_OR_IsoTkMu50_PtEtaBins",1, "muonTrigger", false, MuonTrigger_variation));
  }
  else if(year == Year::is2017v2){
    muo_tight_SF.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2017/MuonID_94X_RunBCDEF_SF_ID.root","NUM_TightID_DEN_genTracks_pt_abseta",0., "tightID", true, MuonID_variation));
    muo_trigger_SF_before.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2017/MuonTrigger_EfficienciesAndSF_RunBtoF_Nov17Nov2017.root","Mu50_PtEtaBins",0., "muonTrigger", false, MuonTrigger_variation));
    muo_trigger_SF_after.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2017/MuonTrigger_EfficienciesAndSF_RunBtoF_Nov17Nov2017.root","Mu50_PtEtaBins",0., "muonTrigger", false, MuonTrigger_variation));
  }
  else if(year == Year::is2018){
    muo_tight_SF.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2018/Muon_ID_SF_RunABCD.root","NUM_TightID_DEN_TrackerMuons_pt_abseta",0., "tightID", true, MuonID_variation));
    muo_trigger_SF_before.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2018/Muon_Trigger_Eff_SF_AfterMuonHLTUpdate.root","Mu50_OR_OldMu100_OR_TkMu100_PtEtaBins",0., "muonTrigger", false,  MuonTrigger_variation));
    muo_trigger_SF_after.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW10/CMSSW_10_2_10/src/UHH2/common/data/2018/Muon_Trigger_Eff_SF_BeforeMuonHLTUpdate.root","Mu50_OR_OldMu100_OR_TkMu100_PtEtaBins",0., "muonTrigger", false,  MuonTrigger_variation));
  }
  else{
    cout << "[ERROR] Year is not found!" << endl;
  }



  scale_variation.reset(new MCScaleVariation(ctx));


  //==========================================
  //partonlevel decay channel slections
  //==========================================
  hadronic_selection.reset( new DecayChannelSelection(ctx, "ttbargen", "hadronic"));
  lepton_jets_seletion.reset( new DecayChannelSelection(ctx, "ttbargen", "lepton_jets"));
  dilepton_selection.reset( new DecayChannelSelection(ctx, "ttbargen", "dilepton"));
  tau_jets_selection.reset( new DecayChannelSelection(ctx, "ttbargen", "tau_jets"));

  //---------
  //matching
  //---------
  double jet_radius = 0.;
  if (useHTT) jet_radius = 1.5;
  if (useHOTVR) jet_radius = -1.0;
  else jet_radius = 0.8;

  merged_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius));
  mergedW_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oMergedW));
  mergedQB_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oBplusQ));

  merged_category = true;
  if (merged == "mergedTop" || merged == "notmergedTop") mergedEvent_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius));
  else if (merged == "mergedW") mergedEvent_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oMergedW));
  else if (merged == "mergedQB") mergedEvent_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oBplusQ));
  else if (merged == "light") mergedEvent_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oLight));
  else if (merged == "bkg") mergedEvent_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oBkg));
  else if (merged == "notmerged") mergedEvent_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oNotMerged));
  else if (merged == "semimerged") mergedEvent_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oSemiMerged));
  else merged_category = false;

  invert_merged_selection = false;
  if (merged == "notmergedTop") {invert_merged_selection = true; merged_category = false;}

  if(!(vers.Contains("TTbar")) && !(vers.Contains("ST")) ){
    merged_category = false;
    invert_merged_selection = false;
  }

  //===========================
  // HOTVR tag
  //===========================
  // deactivate mass cut, other values are standard
  double fpt_max   = 0.8;    // maximum pt fraction of leading subjet
  double m_min     = 0.;   // minimum topjet mass
  double m_max     = 10000.;   // maximum topjet mass
  double mpair_min = 50.;    // minimum pairwise mass of first three subjets
  double tau32_max = 0.56;   // maximum nsubjetiness tau_3/2
  hotvr_tag = AndId<TopJet>(HOTVRTopTag(fpt_max, m_min, m_max, mpair_min), Tau32Groomed(tau32_max));

  //===========================
  //additional selections
  //===========================

  ptW_sel.reset(new PtWSelection(250.)); /// just used comaprisons to the old pythia tune

  massDiff_selection.reset( new MassDiffSelection() );
  dphi_selection.reset(new DPhiMuBSelection( CSVBTag(CSVBTag::WP_MEDIUM), 1.2));
  addJet_selection.reset(new LeadingAddJetSelection( CSVBTag(CSVBTag::WP_MEDIUM), 50));


  //===========================
  //histograms
  //===========================
  hists_after_sel.emplace_back(new EventHists(ctx, "Event_sel"));
  hists_after_sel.emplace_back(new MuonHists(ctx, "Muon_sel"));
  hists_after_sel.emplace_back(new ElectronHists(ctx, "Electron_sel"));
  hists_after_sel.emplace_back(new JetHists(ctx, "Jet_sel"));
  hists_after_sel.emplace_back(new TopJetHists(ctx, "TopJet_sel"));

  std::vector<TString> wps;
  std::vector<TString> wps_PUPPI{"", "_wp1", "_wp2", "_wp3", "_wp4","_wp5"};
  std::vector<TString> wps_CHS{"", "_wp1", "_wp2", "_wp3", "_wp4","_wp5"};
  std::vector<TString> wps_HOTVR{""};

  if(usePUPPI) wps = wps_PUPPI;
  else if(useHOTVR) wps = wps_HOTVR;
  else wps = wps_CHS;

  TString name = "ProbeJet";

  h_probe_all_pass.resize(pt_bins.size());
  h_probe_mass_pass.resize(pt_bins.size());
  h_probe_btag_pass.resize(pt_bins.size());
  h_probe_mass_btag_pass.resize(pt_bins.size());
  // h_probe_btag_NoSF_pass.resize(pt_bins.size());

  h_probe_all_fail.resize(pt_bins.size());
  h_probe_mass_fail.resize(pt_bins.size());
  h_probe_btag_fail.resize(pt_bins.size());
  h_probe_mass_btag_fail.resize(pt_bins.size());
  // h_probe_btag_NoSF_fail.resize(pt_bins.size());

  for(unsigned int bin = 0; bin < pt_bins.size(); ++bin){
    TString ptString = "";
    if(pt_bins.at(bin) == -1) ptString = "ptInclusive";
    else if(pt_bins.at(bin) >= 0 && bin+1 < pt_bins.size() && pt_bins.at(bin+1) >= 0) ptString = "pt"+TString::Format("%3.0f",pt_bins.at(bin))+"to"+TString::Format("%3.0f",pt_bins.at(bin+1));
    else ptString = "pt"+TString::Format("%3.0f",fabs(pt_bins.at(bin)));

    for(unsigned int b = 0; b < wps.size(); ++b){
      h_probe_all_pass.at(bin).emplace_back(new ProbeJetHists(ctx, (name+"_"+ptString+wps.at(b)+"_all_pass").Data(), TagEffi_variation));
      h_probe_btag_pass.at(bin).emplace_back(new ProbeJetHists(ctx,  (name+"_"+ptString+wps.at(b)+"_btag_pass").Data(), TagEffi_variation));

      h_probe_all_fail.at(bin).emplace_back(new ProbeJetHists(ctx, (name+"_"+ptString+wps.at(b)+"_all_fail").Data(), TagEffi_variation));
      h_probe_btag_fail.at(bin).emplace_back(new ProbeJetHists(ctx,  (name+"_"+ptString+wps.at(b)+"_btag_fail").Data(), TagEffi_variation));

      h_effi_300to500_all.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_300to500"+wps.at(b)+"_all").Data()));
      h_effi_300to500_pass.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_300to500"+wps.at(b)+"_pass").Data()));
      h_effi_300to500_fail.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_300to500"+wps.at(b)+"_fail").Data()));
      h_effi_300to500_btag_pass.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_300to500"+wps.at(b)+"_btag_pass").Data()));
      h_effi_300to500_btag_fail.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_300to500"+wps.at(b)+"_btag_fail").Data()));

      h_effi_400to800_all.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_400to800"+wps.at(b)+"_all").Data()));
      h_effi_400to800_pass.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_400to800"+wps.at(b)+"_pass").Data()));
      h_effi_400to800_fail.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_400to800"+wps.at(b)+"_fail").Data()));
      h_effi_400to800_btag_pass.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_400to800"+wps.at(b)+"_btag_pass").Data()));
      h_effi_400to800_btag_fail.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_400to800"+wps.at(b)+"_btag_fail").Data()));

      h_effi_480to600_all.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_480to600"+wps.at(b)+"_all").Data()));
      h_effi_480to600_pass.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_480to600"+wps.at(b)+"_pass").Data()));
      h_effi_480to600_fail.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_480to600"+wps.at(b)+"_fail").Data()));
      h_effi_480to600_btag_pass.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_480to600"+wps.at(b)+"_btag_pass").Data()));
      h_effi_480to600_btag_fail.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_480to600"+wps.at(b)+"_btag_fail").Data()));

      h_effi_1000to2000_all.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_1000to2000"+wps.at(b)+"_all").Data()));
      h_effi_1000to2000_pass.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_1000to2000"+wps.at(b)+"_pass").Data()));
      h_effi_1000to2000_fail.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_1000to2000"+wps.at(b)+"_fail").Data()));
      h_effi_1000to2000_btag_pass.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_1000to2000"+wps.at(b)+"_btag_pass").Data()));
      h_effi_1000to2000_btag_fail.emplace_back(new EfficiencyHists(ctx, ("EfficiencyHists_1000to2000"+wps.at(b)+"_btag_fail").Data()));

    }
  }

  hists_all.reset(new ProbeJetHists(ctx, "ProbeJet_All"));
  hists_all_200to250.reset(new ProbeJetHists(ctx, "ProbeJet_All_Pt200to250"));
  hists_all_250to300.reset(new ProbeJetHists(ctx, "ProbeJet_All_Pt250to300"));
  hists_all_300to400.reset(new ProbeJetHists(ctx, "ProbeJet_All_Pt300to400"));
  hists_all_400.reset(new ProbeJetHists(ctx, "ProbeJet_All_Pt400"));
  hists_all_400to550.reset(new ProbeJetHists(ctx, "ProbeJet_All_Pt400to550"));
  hists_all_550.reset(new ProbeJetHists(ctx, "ProbeJet_All_Pt550"));

  hists_dilepton.reset(new ProbeJetHists(ctx, "ProbeJet_all_dilepton"));
  hists_lepton_jets.reset(new ProbeJetHists(ctx, "ProbeJet_all_lepton_jets"));
  hists_taujets.reset(new ProbeJetHists(ctx, "ProbeJet_all_tau_jets"));
  hists_hadronic.reset(new ProbeJetHists(ctx, "ProbeJet_all_hadronic"));

  hists_dilepton_400.reset(new ProbeJetHists(ctx, "ProbeJet_all_dilepton_Pt400"));
  hists_lepton_jets_400.reset(new ProbeJetHists(ctx, "ProbeJet_all_lepton_jets_Pt400"));
  hists_taujets_400.reset(new ProbeJetHists(ctx, "ProbeJet_all_tau_jets_Pt400"));
  hists_hadronic_400.reset(new ProbeJetHists(ctx, "ProbeJet_all_hadronic_Pt400"));
}



bool TTEfficiencyPostSelectionModule::process(Event & event) {

  //========================================
  //corrections, reweighting, scale factors
  //========================================

  //run corrections
  bool ok = common->process(event);
  if(!ok) return false;

  if(!sel_badhcal->passes(event)) return false;


  //if(!ptW_sel->passes(event)) return false; // uncomment for comparisons with the old pythia tune

  //reject events with
  if(isMC && (event.genInfo->pileup_TrueNumInteractions() < 10 || event.genInfo->pileup_TrueNumInteractions() > 75)) return false;

  topjet_cleaner->process(event);

  //apply top pt reweighting
  for (auto & rew : reweighting_modules) {
    rew->process(event);
  }

  //apply muon b tagging scale factors
  // btagwAK8->process(event);
  const int hlt_runnr = 316361;
  if(event.run < hlt_runnr) muo_trigger_SF_before->process(event);
  else                      muo_trigger_SF_after->process(event);
  muo_tight_SF->process(event);
  sf_btag->process(event);
  scale_variation->process(event);

  if(isTTbar) ps_weights->process(event);

  for(auto & h : hists_after_sel){
    h->fill(event);
  }

  //=====================
  //get the probe jet
  //=====================

  std::vector<TopJet>* topjets = event.topjets;
  std::vector<Muon>* muons = event.muons;
  TopJet probe_jet;
  bool probejet_found = false;

  for(auto & topjet : *topjets){
    double pi = 3.14159265359;
    double delPhi = deltaPhi(topjet, muons->at(0));
    if(delPhi > ((2./3.)*pi)){
      probe_jet = topjet;
      probejet_found = true;
      break;
    }
  }

  if(!probejet_found){
    //   cout << "WARNING: no probe jet found"<< endl;
    return false;
  }

  //throw away all other AK8Jets for Subjet-Btagging SFs
  std::vector<TopJet> new_topjets;
  new_topjets.push_back(probe_jet);
  std::swap(new_topjets, *event.topjets);

  if(invert_merged_selection){
    if(mergedEvent_selection->passes_probe(event, probe_jet)) return false;
  }
  else if (merged_category){
    if(!mergedEvent_selection->passes_probe(event, probe_jet)) return false;
  }


  //=================================
  //define working points and tags
  //=================================

  double probejet_tau32 = probe_jet.tau3()/probe_jet.tau2();

  //get a subjet b tag on the probe jet and the probe jet mass from the subjets
  bool subjet_btag = false;

  for(const auto & subjet : probe_jet.subjets()){
    JetId btag = DeepCSVBTag(DeepCSVBTag::WP_LOOSE);
    // JetId btag = DeepJetBTag(DeepJetBTag::WP_LOOSE);
    if( btag(subjet, event) ) subjet_btag = true;
  }

  LorentzVector subjet_sum(0,0,0,0);
  for(const auto & subjet : probe_jet.subjets()){
    subjet_sum += subjet.v4();
  }

  double probejet_mass = subjet_sum.M();
  double probejet_pt = probe_jet.pt();

  bool mass_cut = false;
  if(usePUPPI && probejet_mass > 105 && probejet_mass < 210) mass_cut = true;
  else if(!usePUPPI && useHOTVR && probejet_mass > 140 && probejet_mass < 220) mass_cut = true;
  else if(!usePUPPI && !useHOTVR && probejet_mass > 105 && probejet_mass < 220) mass_cut = true;

  if(probejet_mass < 10) return false; //mild mass cut on the denominator


  //===================
  //fill histograms
  //===================

  hists_all->fill_probe(event, probe_jet);

  if(probejet_pt > 200 && probejet_pt < 250) hists_all_200to250->fill_probe(event, probe_jet);
  if(probejet_pt > 250 && probejet_pt < 300) hists_all_250to300->fill_probe(event, probe_jet);
  if(probejet_pt > 300 && probejet_pt < 400) hists_all_300to400->fill_probe(event, probe_jet);
  if(probejet_pt > 400) hists_all_400->fill_probe(event, probe_jet);
  if(probejet_pt > 400 && probejet_pt < 550) hists_all_400to550->fill_probe(event, probe_jet);
  if(probejet_pt > 550) hists_all_550->fill_probe(event, probe_jet);


  // if (version == "TTbar_Incl" || version ==  "TTbar_700to1000" || version ==  "TTbar_1000toInf") {
  if ( ((TString)version).Contains("TTbar") ){
    if( tau_jets_selection->passes(event) ) {
      hists_taujets->fill_probe(event, probe_jet);
      if(probejet_pt > 400) hists_taujets_400->fill_probe(event, probe_jet);
    }
    if( lepton_jets_seletion->passes(event) ) {
      hists_lepton_jets->fill_probe(event, probe_jet);
      if(probejet_pt > 400) hists_lepton_jets_400->fill_probe(event, probe_jet);
    }
    if( dilepton_selection->passes(event) ) {
      hists_dilepton->fill_probe(event, probe_jet);
      if(probejet_pt > 400) hists_dilepton_400->fill_probe(event, probe_jet);
    }
    if( hadronic_selection->passes(event) ) {
      hists_hadronic->fill_probe(event, probe_jet);
      if(probejet_pt > 400)  hists_hadronic_400->fill_probe(event, probe_jet);
    }
  }


  //===================
  //new working points
  //===================

  std::vector<bool> toptag, toptag_mass, toptag_btag, toptag_mass_btag;

  std::vector<double> tau32_wps_CHS_new           {-1, 0.40, 0.50, 0.57, 0.67, 0.81};
  std::vector<double> tau32_wps_PUPPI_new         {-1, 0.40, 0.46, 0.54, 0.65, 0.80};
  std::vector<double> tau32_wps_HOTVR_new         {-1};

  std::vector<double> tau32_wps_new;
  std::vector<double> fRec_wps_new;
  if(usePUPPI) tau32_wps_new = tau32_wps_PUPPI_new;
  else if(useHOTVR) tau32_wps_new = tau32_wps_HOTVR_new;
  else tau32_wps_new = tau32_wps_CHS_new;

  bool pass_hotvr = false;
  if(hotvr_tag(probe_jet, event)) pass_hotvr = true;

  for(unsigned int wp = 0; wp < tau32_wps_new.size(); ++wp){

    bool tau_cut = get_tau32_cut(wp, probejet_tau32, tau32_wps_new);

    // PUPPI
    if(usePUPPI){
      // tau cut
      if(tau_cut) toptag.push_back(true);
      else toptag.push_back(false);
      // tau + mass
      if(tau_cut && mass_cut) toptag_mass.push_back(true);
      else toptag_mass.push_back(false);
      // tau + subjet btag
      if(tau_cut && subjet_btag) toptag_btag.push_back(true);
      else toptag_btag.push_back(false);
      // tau + subjet btag + mass
      if(tau_cut && mass_cut && subjet_btag) toptag_mass_btag.push_back(true);
      else toptag_mass_btag.push_back(false);
    }

    // CHS
    if(!usePUPPI && !useHOTVR){
      // tau cut
      if(tau_cut) toptag.push_back(true);
      else toptag.push_back(false);
      // tau + mass
      if(tau_cut && mass_cut) toptag_mass.push_back(true);
      else toptag_mass.push_back(false);
      // tau + subjet btag
      if(tau_cut && subjet_btag) toptag_btag.push_back(true);
      else toptag_btag.push_back(false);
      // tau + subjet btag + mass
      if(tau_cut && mass_cut && subjet_btag) toptag_mass_btag.push_back(true);
      else toptag_mass_btag.push_back(false);
    }

    // HOTVR
    if(useHOTVR){
      // top tag
      if(pass_hotvr) toptag.push_back(true);
      else toptag.push_back(false);
      if(pass_hotvr && mass_cut) toptag_mass.push_back(true);
      else toptag_mass.push_back(false);
      // return false for additional requirements
      toptag_btag.push_back(false);
      toptag_mass_btag.push_back(false);
    }
  }


  //=======================
  //fill the new histograms
  //=======================

  for(unsigned int bin = 0; bin < pt_bins.size(); ++bin){
    for(unsigned int wp = 0; wp < tau32_wps_new.size(); ++wp){

      bool pt_cut = get_pt_cut(bin, probejet_pt);
      if(pt_cut){
        if(toptag.at(wp)) h_probe_all_pass.at(bin).at(wp)->fill_probe(event, probe_jet);
        else h_probe_all_fail.at(bin).at(wp)->fill_probe(event, probe_jet);

        //	if(toptag_mass.at(wp)) h_probe_mass_pass.at(bin).at(wp)->fill_probe(event, probe_jet);
        //	else h_probe_mass_fail.at(bin).at(wp)->fill_probe(event, probe_jet);
      }

    }
  }

  // fill hists to calculate efficiencies
  for(unsigned int wp = 0; wp < tau32_wps_new.size(); ++wp){
    if(probejet_pt > 300 && probejet_pt < 500){
      h_effi_300to500_all.at(wp)->fill_probe(event, probe_jet);
      // is tagged?
      if(toptag_mass.at(wp)){
        h_effi_300to500_pass.at(wp)->fill_probe(event, probe_jet);
      }
      else h_effi_300to500_fail.at(wp)->fill_probe(event, probe_jet);
      // tagged with subjet btag?
      if(toptag_mass_btag.at(wp)){
        h_effi_300to500_btag_pass.at(wp)->fill_probe(event, probe_jet);
      }
      else h_effi_300to500_btag_fail.at(wp)->fill_probe(event, probe_jet);
    }

    if(probejet_pt > 400 && probejet_pt < 800){
      h_effi_400to800_all.at(wp)->fill_probe(event, probe_jet);
      // is tagged?
      if(toptag_mass.at(wp)){
        h_effi_400to800_pass.at(wp)->fill_probe(event, probe_jet);
      }
      else h_effi_400to800_fail.at(wp)->fill_probe(event, probe_jet);
      // tagged with subjet btag?
      if(toptag_mass_btag.at(wp)){
        h_effi_400to800_btag_pass.at(wp)->fill_probe(event, probe_jet);
      }
      else h_effi_400to800_btag_fail.at(wp)->fill_probe(event, probe_jet);
    }

    if(probejet_pt > 480 && probejet_pt < 600){
      h_effi_480to600_all.at(wp)->fill_probe(event, probe_jet);
      // is tagged?
      if(toptag_mass.at(wp)){
        h_effi_480to600_pass.at(wp)->fill_probe(event, probe_jet);
      }
      else h_effi_480to600_fail.at(wp)->fill_probe(event, probe_jet);
      // tagged with subjet btag?
      if(toptag_mass_btag.at(wp)){
        h_effi_480to600_btag_pass.at(wp)->fill_probe(event, probe_jet);
      }
      else h_effi_480to600_btag_fail.at(wp)->fill_probe(event, probe_jet);
    }

    if(probejet_pt > 1000 && probejet_pt < 2000){
      h_effi_1000to2000_all.at(wp)->fill_probe(event, probe_jet);
      // is tagged?
      if(toptag_mass.at(wp)){
        h_effi_1000to2000_pass.at(wp)->fill_probe(event, probe_jet);
      }
      else h_effi_1000to2000_fail.at(wp)->fill_probe(event, probe_jet);
      // tagged with subjet btag?
      if(toptag_mass_btag.at(wp)){
        h_effi_1000to2000_btag_pass.at(wp)->fill_probe(event, probe_jet);
      }
      else h_effi_1000to2000_btag_fail.at(wp)->fill_probe(event, probe_jet);
    }
  }

  //subjet_btagwAK8->process(event);

  // if(probejet_pt > 300 && probejet_pt < 400 ) subjet_btagwAK8_300to400->process(event);
  // if(probejet_pt >= 400 && probejet_pt < 480 ) subjet_btagwAK8_400to480->process(event);
  // if(probejet_pt >= 480 && probejet_pt < 600 ) subjet_btagwAK8_480to600->process(event);
  // if(probejet_pt >= 600 ) subjet_btagwAK8_600->process(event);


  for(unsigned int bin = 0; bin < pt_bins.size(); ++bin){
    for(unsigned int wp = 0; wp < tau32_wps_new.size(); ++wp){

      bool pt_cut = get_pt_cut(bin, probejet_pt);
      if(pt_cut){
        if(toptag_btag.at(wp)) h_probe_btag_pass.at(bin).at(wp)->fill_probe(event, probe_jet);
        else h_probe_btag_fail.at(bin).at(wp)->fill_probe(event, probe_jet);

        //	if(toptag_mass_btag.at(wp)) h_probe_mass_btag_pass.at(bin).at(wp)->fill_probe(event, probe_jet);
        //	else h_probe_mass_btag_fail.at(bin).at(wp)->fill_probe(event, probe_jet);
      }

    }
  }

  // do not save analysis tree
  return false;
}

bool TTEfficiencyPostSelectionModule::get_pt_cut(unsigned int bin, const double pt){
  if(bin > pt_bins.size()) return false;

  bool m_pt_cut = false;
  if(pt_bins.at(bin) == -1) m_pt_cut = true;
  else if(pt_bins.at(bin) >= 0 && bin+1 < pt_bins.size() && pt_bins.at(bin+1) >= 0){
    if( pt > pt_bins.at(bin) && pt < pt_bins.at(bin+1) ) m_pt_cut = true;
  }
  else{
    if( pt > fabs(pt_bins.at(bin)) ) m_pt_cut = true;
  }
  return m_pt_cut;
}

bool TTEfficiencyPostSelectionModule::get_tau32_cut(unsigned int bin, const double tau32, const std::vector<double> wps){
  if(bin > wps.size()) return false;

  bool m_tau_cut = false;
  if(wps.at(bin) < 0) m_tau_cut = true;
  else{
    if( tau32 < wps.at(bin)) m_tau_cut = true;
  }
  return m_tau_cut;
}

UHH2_REGISTER_ANALYSIS_MODULE(TTEfficiencyPostSelectionModule)
