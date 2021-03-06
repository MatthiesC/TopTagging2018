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
#include <UHH2/common/include/JetIds.h>
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/TopPtReweight.h"
#include "UHH2/TopTagging2018/include/TopTaggingSelections.h"
#include "UHH2/TopTagging2018/include/ProbeJetHists.h"
#include <UHH2/common/include/Utils.h>
#include "UHH2/TopTagging2018/include/TopTaggingUtils.h"
#include "UHH2/TopTagging2018/include/TopJetCorrections.h"
#include "UHH2/HOTVR/include/HOTVRJetCorrectionModule.h"

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

class TTEfficiencyMainSelectionModule: public AnalysisModule {
public:

  explicit TTEfficiencyMainSelectionModule(Context & ctx);
  virtual bool process(Event & event) override;

private:

  //correctors
  std::unique_ptr<CommonModules> common;
  std::unique_ptr<TopJetCorrections> topjetCorr;
  std::unique_ptr<AnalysisModule> HOTVRCorr;
  std::unique_ptr<GenericJetResolutionSmearer> topjetJER_smearer;
  std::unique_ptr<TopJetGroomer> topjet_groomer;
  std::unique_ptr<uhh2::AnalysisModule> pileupRW;

  //cleaners
  std::unique_ptr<JetLeptonCleaner> jet_lepton_cleaner;
  std::unique_ptr<TopJetLeptonDeltaRCleaner> topjet_cleaner_dRlep;
  std::unique_ptr<JetCleaner> jet_cleaner;
  std::unique_ptr<TopJetCleaner> topjet_cleaner, topjet_cleaner_400;

  //reweighting and scale factors
  std::vector<std::unique_ptr<AnalysisModule>> reweighting_modules;
  std::unique_ptr<AnalysisModule> btagwAK8, subjet_btagwAK8;

  //selections
  std::unique_ptr<Selection> sel_badhcal;
  std::unique_ptr<uhh2::Selection> topjet_sel, ptW_sel, trigger_sel,  met_sel, bjetCloseToLepton_sel, twoDcut;
  std::unique_ptr<AndSelection> first_selection;
  std::unique_ptr<uhh2::Selection> hadronic_selection, lepton_jets_seletion, dilepton_selection, tau_jets_selection;
  std::unique_ptr<MergedSelection> merged_selection, mergedW_selection, mergedQB_selection, mergedEvent_Selection;
  std::unique_ptr<MassDiffSelection> massDiff_selection;
  std::unique_ptr<DPhiMuBSelection> dphi_selection;

  //histograms
  std::unique_ptr<BTagMCEfficiencyHists> hists_btag_loose_eff, hists_btag_medium_eff;
  std::vector<std::unique_ptr<uhh2::Hists>> hists_before_sel;
  std::vector<std::unique_ptr<uhh2::Hists>> hists_after_sel;
  std::unique_ptr<uhh2::Hists> hists_notrigger, hists_trigger;
  std::unique_ptr<ProbeJetHists> hists_all, hists_all_400, hists_all_400to550, hists_all_550 ,hists_tagged;
  std::unique_ptr<ProbeJetHists> hists_noTag, hists_tau32_wp1, hists_tau32_wp2, hists_tagged_wp1, hists_tagged_wp2;
  std::unique_ptr<ProbeJetHists> hists_merged, hists_mergedW, hists_mergedQB, hists_lightjets, hists_unmerged, hists_dilepton, hists_lepton_jets, hists_taujets, hists_hadronic;
  std::unique_ptr<ProbeJetHists> hists_merged_400, hists_mergedW_400, hists_mergedQB_400, hists_lightjets_400, hists_unmerged_400, hists_dilepton_400, hists_lepton_jets_400, hists_taujets_400, hists_hadronic_400;

  std::vector<std::unique_ptr<ProbeJetHists>> h_tau32, h_tau32_mass, h_tau32_btag, h_tau32_mass_btag, h_tau32_mass_btag_pt400to550, h_tau32_mass_btag_pt550, h_tau32_mass_btag_pt400;

  bool useHTT, usePUPPI, useHOTVR;
  string version;

  bool isMC;
  bool invert_merged_selection;

  Event::Handle<std::vector<Particle>> add_genjet;

};


TTEfficiencyMainSelectionModule::TTEfficiencyMainSelectionModule(Context & ctx){

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

  if(usePUPPI) cout << "use PUPPI topjets" << endl;
  else if(useHOTVR) cout << "use HOTVR topjets" << endl;
  else cout << "use CHS topjets" << endl;


  if(useHTT) cout << "run the HTT" << endl;
  else cout << "run CMS tagger" << endl;

  version = ctx.get("dataset_version", "");

  string merged = ctx.get("MergedSelection", "<not set>");



  //=============================
  // IDs
  //=============================

   MuonId muid = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(55., 2.4));
   ElectronId eleid = AndId<Electron>(ElectronID_Fall17_medium_noIso, PtEtaCut(55., 2.4));


  JetId jetid = AndId<Jet>(JetPFID(JetPFID::WP_TIGHT_CHS), PtEtaCut(30.0, 2.4));

  string PU_variation ="central";
  PU_variation = ctx.get("PU_variation","central");


  //=============================
  // corrections
  //=============================

  //int Nsubjet = -1;
  //TString CorrectSubjet = ctx.get("CorrectSubjet", "all");
  //if (CorrectSubjet == "first") Nsubjet = 0;
  //if (CorrectSubjet == "second") Nsubjet = 1;
  //if (CorrectSubjet == "anti") Nsubjet = -2;


  //common modules
  common.reset(new CommonModules());
  common->set_jet_id(jetid);
  common->set_muon_id(muid);
  common->set_electron_id(eleid);
  common->switch_jetlepcleaner(true);
  common->switch_metcorrection(true);
  common->switch_jetPtSorter();
  common->set_HTjetid(jetid);
  if(isMC) common->disable_metfilters();
  common->disable_mcpileupreweight();
  // common->disable_jersmear();           // disable until JEC available
  // common->disable_jec();                // disable until JEC available
  // common->switch_metcorrection(false);  // disable until JEC available
  // common->switch_jetlepcleaner(false);  // disable until JEC available
  common->init(ctx, PU_variation);
  cout << "common init" <<endl;

  if(!useHOTVR){
    if(usePUPPI) cout << "TopJet Corrections = PUPPI" << endl;
    else         cout << "TopJet Corrections = CHS" << endl;
    topjetCorr.reset(new TopJetCorrections());
    topjetCorr->init(ctx);
  }

  if(useHOTVR){
    cout << "TopJet Corrections = HOTVR" << endl;
    HOTVRCorr.reset(new HOTVRJetCorrectionModule(ctx));
  }

  //=============================
  // cleaners
  //=============================
  jet_cleaner.reset(new JetCleaner(ctx, 30., 2.4));

  double radius = 0.0;
  if(useHTT) radius = 1.5;
  else if(useHOTVR) radius = -2.0;
  else radius = 0.8;

  topjet_cleaner_dRlep.reset(new TopJetLeptonDeltaRCleaner(radius));

  if(useHTT) topjet_cleaner.reset(new TopJetCleaner(ctx, TopJetId(PtEtaCut(150., 2.4))));
  else topjet_cleaner.reset(new TopJetCleaner(ctx, TopJetId(PtEtaCut(170., 2.4))));
  topjet_cleaner_400.reset(new TopJetCleaner(ctx, TopJetId(PtEtaCut(400., 2.4))));

  //=============================
  //reweighting and scale factors
  //=============================
  if (((TString)version).Contains("TTT")){// == "TTbar_Incl" || version ==  "TTbar_700to1000" || version ==  "TTbar_1000toInf") {
    reweighting_modules.emplace_back(new TTbarGenProducer(ctx, "ttbargen", true));
    // reweighting_modules.emplace_back(new TopPtReweight(ctx, 0.156, -0.00137, "ttbargen", "weight_ttbar", true)); //8TeV
    // reweighting_modules.emplace_back(new TopPtReweight(ctx, 0.0615, -0.0005, "ttbargen", "weight_ttbar", true)); //13TeV
  }

  //  btagwAK8.reset(new MCBTagScaleFactor(ctx, CSVBTag::WP_LOOSE, "jets","central","mujets","incl","MCBtagEfficiencies"));
  //btagwAK8.reset(new MCBTagScaleFactor(ctx, CSVBTag::WP_MEDIUM, "jets","central","mujets","incl","MCBtagEfficiencies"));

  //=============================
  //selections
  //=============================

  sel_badhcal.reset(new BadHCALSelection(ctx));
  topjet_sel.reset(new NTopJetSelection(2));
  trigger_sel = uhh2::make_unique<TriggerSelection>("HLT_Mu50_v*");
  twoDcut.reset(new TwoDCut(.4, 25.));
  met_sel.reset(new METCut(50., std::numeric_limits<double>::infinity()));
  ptW_sel.reset(new PtWSelection(150.));
  // bjetCloseToLepton_sel.reset(new NMuonBTagSelection(1, 999, DeepCSVBTag(DeepCSVBTag::WP_MEDIUM) ));
  bjetCloseToLepton_sel.reset(new NMuonBTagSelection(1, 999, DeepJetBTag(DeepJetBTag::WP_MEDIUM) ));

  first_selection.reset(new AndSelection(ctx,"first selection"));
  first_selection->add<NMuonSelection>("Number of muons == 1",1,1);
  first_selection->add<NJetSelection>("Number of jets >= 2",2);
  first_selection->add<NTopJetSelection>("Number of topjets >= 1",1);

  //partonlevel decay channel slections
  hadronic_selection.reset( new DecayChannelSelection(ctx, "ttbargen", "hadronic"));
  lepton_jets_seletion.reset( new DecayChannelSelection(ctx, "ttbargen", "lepton_jets"));
  dilepton_selection.reset( new DecayChannelSelection(ctx, "ttbargen", "dilepton"));
  tau_jets_selection.reset( new DecayChannelSelection(ctx, "ttbargen", "tau_jets"));

  //double jet_radius = 0.;
  //if (useHTT) jet_radius = 1.5;
  //else jet_radius = 0.8;

  //merged selections
  //merged_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius));
  //mergedW_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oMergedW));
  //mergedQB_selection.reset( new MergedSelection(ctx, "ttbargen", jet_radius, MergedSelection::oBplusQ));


  //mass diff selection
  // massDiff_selection.reset( new MassDiffSelection() );
  // dphi_selection.reset(new DPhiMuBSelection(CSVBTag(CSVBTag::WP_MEDIUM), 1.2));


  //=============================
  //histograms
  //=============================
  // JetId id_btag_loose = BTag(BTag::DEEPCSV, BTag::WP_LOOSE);
  // JetId id_btag_medium = BTag(BTag::DEEPCSV, BTag::WP_MEDIUM);
  JetId id_btag_loose = BTag(BTag::DEEPJET, BTag::WP_LOOSE);
  JetId id_btag_medium = BTag(BTag::DEEPJET, BTag::WP_MEDIUM);


  hists_btag_loose_eff.reset(new BTagMCEfficiencyHists(ctx,"BTagLoose", id_btag_loose));
  hists_btag_medium_eff.reset(new BTagMCEfficiencyHists(ctx,"BTagMedium", id_btag_medium));


  // hists_subjet_btag_eff.reset(new BTagMCEfficiencyHists(ctx,"SubjetBTag",CSVBTag::WP_LOOSE, "topjets") );


  cout << "setup hists" << endl;
  hists_before_sel.emplace_back(new EventHists(ctx, "Event_presel"));
  hists_before_sel.emplace_back(new MuonHists(ctx, "Muon_presel"));
  hists_before_sel.emplace_back(new ElectronHists(ctx, "Electron_presel"));
  hists_before_sel.emplace_back(new JetHists(ctx, "Jet_presel"));
  hists_before_sel.emplace_back(new TopJetHists(ctx, "TopJet_presel"));

  hists_after_sel.emplace_back(new EventHists(ctx, "Event_sel"));
  hists_after_sel.emplace_back(new MuonHists(ctx, "Muon_sel"));
  hists_after_sel.emplace_back(new ElectronHists(ctx, "Electron_sel"));
  hists_after_sel.emplace_back(new JetHists(ctx, "Jet_sel"));
  hists_after_sel.emplace_back(new TopJetHists(ctx, "TopJet_sel"));

}



bool TTEfficiencyMainSelectionModule::process(Event & event) {

 //=============================
  //apply top pt reweighting
 //=============================

 // for (auto & rew : reweighting_modules) {
 //   rew->process(event);
  //}
  //  if (version == "TTbar_Incl" ) {
  //  if(!mttgen_sel->passes(event)) return false;
  //}

  //=============================
  //run corrections
  //=============================
  bool ok = common->process(event);
  if(!ok) return false;

  if(useHOTVR){
    HOTVRCorr->process(event);
  }
  else{
    topjetCorr->process(event);
  }

  //=============================
  //run cleaners
  //=============================
  jet_cleaner->process(event);
  topjet_cleaner->process(event);
  topjet_cleaner_dRlep->process(event); //remove topjets that overlap with a lepton (e/mu)

  //======================================
  //fill histograms before selection
  //======================================
  for(auto & h : hists_before_sel){
    h->fill(event);
  }

  //======================================
  //apply selections
  //======================================
  if(!trigger_sel->passes(event)) return false;
  if(!first_selection->passes(event)) return false;
  if(!twoDcut->passes(event)) return false;
  if(!met_sel->passes(event)) return false;
  if(!ptW_sel->passes(event)) return false;
  if(!sel_badhcal->passes(event)) return false;
  hists_btag_loose_eff->fill(event);
  hists_btag_medium_eff->fill(event);
  if(!bjetCloseToLepton_sel->passes(event)) return false;

  //======================================
  //fill histograms after selection
  //======================================

  for(auto & h : hists_after_sel){
    h->fill(event);
  }

  return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(TTEfficiencyMainSelectionModule)
