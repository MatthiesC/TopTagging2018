#include "UHH2/TopTagging2018/include/TopTaggingSelections.h"
#include "UHH2/TopTagging2018/include/TopTaggingUtils.h"
#include "UHH2/core/include/Event.h"


#include <stdexcept>

//using namespace uhh2examples;
using namespace std;
using namespace uhh2;


DijetSelection::DijetSelection(float dphi_min_, float third_frac_max_): dphi_min(dphi_min_), third_frac_max(third_frac_max_){}

bool DijetSelection::passes(const Event & event){
  assert(event.jets); // if this fails, it probably means jets are not read in
  if(event.jets->size() < 2) return false;
  const auto & jet0 = event.jets->at(0);
  const auto & jet1 = event.jets->at(1);
  auto dphi = deltaPhi(jet0, jet1);
  if(dphi < dphi_min) return false;
  if(event.jets->size() == 2) return true;
  const auto & jet2 = event.jets->at(2);
  auto third_jet_frac = jet2.pt() / (0.5 * (jet0.pt() + jet1.pt()));
  return third_jet_frac < third_frac_max;
}

HTCut::HTCut(float minHT_, float maxHT_):
minHT(minHT_), maxHT(maxHT_){}

bool HTCut::passes(const Event& event){
  double ht = 0.;
  assert(event.jets);
  for(const auto jet : *event.jets){
    ht += jet.pt();
  }
  if(ht > minHT && ht < maxHT) return true;
  return false;
}

PtWSelection::PtWSelection(double minPt_, double maxPt_):
minPt(minPt_), maxPt(maxPt_){}

bool PtWSelection::passes(const Event& event){

  TVector2 muon;
  muon.SetMagPhi(event.muons->at(0).pt(), event.muons->at(0).phi());

  TVector2 met;
  met.SetMagPhi(event.met->pt(), event.met->phi());

  TVector2 W = muon+met;

  double ptW = W.Mod();

  if(ptW > minPt && ptW < maxPt) return true;
  return false;
}


HTlepCut::HTlepCut(double minHTLep_, double maxHTLep_, bool useMuons_, bool useElectrons_):
minHTLep(minHTLep_), maxHTLep(maxHTLep_), useMuons(useMuons_), useElectrons(useElectrons_){}

bool HTlepCut::passes(const Event& event){
  double ptlep = 0;

  if(useMuons){
    assert(event.muons);
    for(const auto muon : *event.muons){
      if(muon.pt() > ptlep) ptlep = muon.pt();
    }
  }
  if(useElectrons){
    for(const auto ele : *event.electrons){
      if(ele.pt() > ptlep) ptlep = ele.pt();
    }
  }

  double htlep = ptlep + event.met->pt();

  if(htlep > minHTLep && htlep < maxHTLep) return true;
  return false;
}



METCut::METCut(float minMet_, float maxMet_):
minMet(minMet_), maxMet(maxMet_) {}

bool METCut::passes(const Event & event){

  assert(event.met);

  float MET = event.met->pt();
  return (MET > minMet) && (MET < maxMet);
}


bool TwoDCut::passes(const Event & event){

  assert(event.muons && event.electrons && event.jets);
  /*  if((event.muons->size()+event.electrons->size()) != 1){
  std::cout << "N_elec=" << event.electrons->size() << "N_muon=" << event.muons->size() << "\n @@@ WARNING -- TwoDCut::passes -- unexpected number of muons+electrons in the event (!=1). returning 'false'\n";
  return false;
}*/

float drmin, ptrel;
if(event.muons->size()) std::tie(drmin, ptrel) = drmin_pTrel(event.muons->at(0), *event.jets);
else std::tie(drmin, ptrel) = drmin_pTrel(event.electrons->at(0), *event.jets);

return (drmin > min_deltaR) || (ptrel > min_pTrel);
}



NMuonBTagSelection::NMuonBTagSelection(int min_nbtag, int max_nbtag, JetId btag, double ptmin, double etamax )
{
  m_min_nbtag=min_nbtag;
  m_max_nbtag=max_nbtag;
  m_btag=btag;
  m_ptmin=ptmin;
  m_etamax=etamax;
}

bool NMuonBTagSelection::passes(const Event & event)
{
  int nbtag=0;

  //Assumes to have only one muon
  std::vector<Jet>* jets = event.jets;
  std::vector<Muon>* muons = event.muons;
  for(unsigned int i=0; i<event.jets->size(); ++i) {
    int jettagged=0;
    Jet jet=jets->at(i);
    if (m_btag(jet, event)) jettagged=1;

    if(muons->size() != 1){
      std::cout << "ATTENTION!!! muon size " << muons->size() << std::endl;
    }

    double deltaphi=deltaPhi(jet,muons->at(0));
    double pi = 3.14159265359;
    if(jettagged&&(deltaphi<(2*pi/3))&&(jet.pt()>m_ptmin)&&(fabs(jet.eta())<m_etamax)){

      nbtag++;

    }
  }

  if(nbtag<m_min_nbtag) return false;
  if(nbtag>m_max_nbtag) return false;
  return true;
}


MergedSelection::MergedSelection( uhh2::Context& ctx, const std::string ttbarGen_name_, double radius_, mergingOpt opt_): ttbarGen_name(ttbarGen_name_), radius(radius_), opt(opt_) {
  string version = ctx.get("dataset_version", "");
  TString vers = (TString)version;
  if ( vers.Contains("TTbar") ) isTTbar = true;
  else isTTbar = false;
  if(isTTbar) h_ttbarGen = ctx.get_handle<TTbarGen>(ttbarGen_name);

  if(isTTbar) cout << "use ttbar matching" << endl;
  else cout << "use Single Top matching" << endl;
  // cout << opt << endl;
}

bool MergedSelection::passes(const Event & event)
{
  return false;
}

bool MergedSelection::passes_probe(const uhh2::Event &event, const TopJet &probe_jet) {

  // use other function if MC is not ttbar
  if(!isTTbar){
    bool pass = passes_probe_ST(event, probe_jet);
    return pass;
  }

  // this is needed for HOTVR jets
  if (radius < 0){
    double Rmin = 0.1;
    double Rmax = 1.5;
    double rho = 600.;
    double reff = rho/(probe_jet.pt()*probe_jet.JEC_factor_raw());
    if( reff <  Rmin ) radius = Rmin;
    else if( reff >  Rmax ) radius = Rmax;
    else radius = reff;
  }

  // const TTbarGen& ttbarGen = !ttbarGen_name.empty() ? event.get(h_ttbarGen) : TTbarGen(*event.genparticles,true);
  const auto & ttbarGen = event.get(h_ttbarGen);


  if(ttbarGen.IsSemiLeptonicDecay()) {
    GenParticle bHad = ttbarGen.BHad();
    GenParticle q1 = ttbarGen.Q1();
    GenParticle q2 = ttbarGen.Q2();
    if(opt == oFullyMerged){
      if( deltaR(probe_jet.v4(), bHad.v4()) < radius
      && deltaR(probe_jet.v4(), q1.v4()) < radius
      && deltaR(probe_jet.v4(), q2.v4()) < radius) {
        return true;
      }
    }
    if(opt == oMergedW || opt == oSemiMerged) {
      if( deltaR(probe_jet.v4(), bHad.v4()) > radius
      && deltaR(probe_jet.v4(), q1.v4()) < radius
      && deltaR(probe_jet.v4(), q2.v4()) < radius) {
        return true;
      }
    }
    if(opt == oBplusQ || opt == oSemiMerged) {
      if( deltaR(probe_jet.v4(), bHad.v4()) < radius){
        if (deltaR(probe_jet.v4(), q1.v4()) < radius && deltaR(probe_jet.v4(), q2.v4()) > radius) return true;
        if (deltaR(probe_jet.v4(), q1.v4()) > radius && deltaR(probe_jet.v4(), q2.v4()) < radius) return true;
      }
    }
    if(opt == oLight|| opt == oNotMerged) {
      unsigned int N = 0;
      if( deltaR(probe_jet.v4(), bHad.v4()) < radius ) N++;
      if( deltaR(probe_jet.v4(), q1.v4()) < radius) N++;
      if( deltaR(probe_jet.v4(), q2.v4()) < radius) N++;
      if( N <= 1) return true;
    }
  }
  else if(opt == oBkg || opt == oNotMerged) {
    return true;
  }
  return false;
}


bool MergedSelection::passes_probe_ST(const uhh2::Event &event, const TopJet &probe_jet) {

  // this is needed for HOTVR jets
  if (radius < 0){
    double Rmin = 0.1;
    double Rmax = 1.5;
    double rho = 600.;
    double reff = rho/probe_jet.pt();
    if( reff <  Rmin ) radius = Rmin;
    else if( reff >  Rmax ) radius = Rmax;
    else radius = reff;
  }

  vector<GenParticle> genparts = *event.genparticles;

  // cout << "---------------------" << endl;
  // cout << " number genparts = " << genparts.size() << endl;

  GenParticle top;
  int Ntop = 0;
  for(auto gp: genparts){
    if(abs(gp.pdgId()) == 6){
      top = gp;
      Ntop ++;
    }
  }
  if(Ntop != 1){
    cout << "no Top found" << endl;
    if(opt == oNotMerged) return true;
    else return false;
  }

  auto w = top.daughter(&genparts, 1);
  auto b = top.daughter(&genparts, 2);
  if(!w || !b){
    throw runtime_error("top has not ==2 daughters");
    return false;
  }
  if(abs(w->pdgId()) != 24){
    std::swap(w, b);
  }

  /* It rarely happens that the list of genparts contains 4 or more (instead of 2) particles which reckon the same top
  as their mother although each particle including the tops can just have two daughters. E.g. if the top emits a photon
  before decaying into b and W, this photon may split up into two leptons which reckon the top as their mother, too.
  Therefore, it may happen that those leptons are considered as the top daughters whereas b and W are "ignored" and cannot
  be found. This workaround fixes that issue: */
  if(abs(w->pdgId()) != 24) {
    for(unsigned int j = 0; j < genparts.size(); ++j) {
      const GenParticle & gp = genparts[j];
      auto m1 = gp.mother(&genparts, 1);
      auto m2 = gp.mother(&genparts, 2);
      bool has_top_mother = ((m1 && m1->pt() == top.pt()) || (m2 && m2->pt() == top.pt()));
      if(has_top_mother && (abs(gp.pdgId()) == 24)) {
        w = &gp;
        break;
      }
    }
  }
  if(abs(w->pdgId()) != 24){
    throw runtime_error("MergedSelection: top has no W daughter");
    return false;
  }

  // NOTE: here, we could skip over intermediate W bosons. However,
  // this Pythia8-related problem is now fixed when creating ntuples already,
  // so this should not be necessary.

  /* Do a similar workaround as above if the expected b daughter has not been found yet */
  if(abs(b->pdgId()) != 5 && abs(b->pdgId()) != 3 && abs(b->pdgId()) != 1) {
    for(unsigned int j = 0; j < genparts.size(); ++j) {
      const GenParticle & gp = genparts[j];
      auto m1 = gp.mother(&genparts, 1);
      auto m2 = gp.mother(&genparts, 2);
      bool has_top_mother = ((m1 && m1->pt() == top.pt()) || (m2 && m2->pt() == top.pt()));
      if(has_top_mother && (abs(gp.pdgId()) == 5 || abs(gp.pdgId()) == 3 || abs(gp.pdgId()) == 1)) {
        b = &gp;
        break;
      }
    }
  }
  if(abs(b->pdgId()) != 5 && abs(b->pdgId()) != 3   && abs(b->pdgId()) != 1){
    throw runtime_error("MergedSelection: top has no b daughter");
    return false;
  }

  // now get W daughters:
  int n_wdau=0;

  auto wd1 = w->daughter(&genparts, 1);
  auto wd2 = w->daughter(&genparts, 2);

  while (n_wdau!=2){
    if(wd1 && !wd2){
      w = wd1;
      wd1 = w->daughter(&genparts, 1);
      wd2 = w->daughter(&genparts, 2);
    }
    else if(wd1 && wd2){
      n_wdau=2;
    }

    else{
      throw runtime_error("MergedSelection: W has no daughters");
      return false;
    }
  }
  if(!wd1 || !wd2){
    throw runtime_error("MergedSelection: W has not ==2 daughters");
    return false;
  }

  int id1 = wd1->pdgId();
  int id2 = wd2->pdgId();

  // check if W from top decays into leptons
  if(id1 == 11 || id1 == 12 || id1 == 13 ||id1 == 14 || id1 == 15 || id1 == 16 || id2 == 11 || id2 == 12 || id2 == 13 ||id2 == 14 || id2 == 15 || id2 == 16){
    GenParticle otherW;
    int NotherW = 0;
    for(auto gp: genparts){
      if(abs(gp.pdgId()) == 24 && !is_same_particle(gp, *w)){
        otherW = gp;
        NotherW++;
      }
    }
    // cout << "other W number " << NotherW << endl;
    if(NotherW == 0){
      // if no other W is found, event is declared not matched
      if(opt == oNotMerged) return true;
      else return false;
    }
    else{
      // if one is found, overwrite W daughters with daughters from new W
      wd1 = otherW.daughter(&genparts, 1);
      wd2 = otherW.daughter(&genparts, 2);
      id1 = wd1->pdgId();
      id2 = wd2->pdgId();
      if(id1 == 11 || id1 == 12 || id1 == 13 ||id1 == 14 || id1 == 15 || id1 == 16 || id2 == 11 || id2 == 12 || id2 == 13 ||id2 == 14 || id2 == 15 || id2 == 16){
        // if other W also decays into leptons, declare not matched
        if(opt == oNotMerged) return true;
        else return false;
      }
    }
  }


  GenParticle bHad = *b;
  GenParticle q1 = *wd1;
  GenParticle q2 = *wd2;
  if(opt == oFullyMerged){
    if( deltaR(probe_jet.v4(), bHad.v4()) < radius
    && deltaR(probe_jet.v4(), q1.v4()) < radius
    && deltaR(probe_jet.v4(), q2.v4()) < radius) {
      return true;
    }
  }
  if(opt == oMergedW || opt == oSemiMerged) {
    if( deltaR(probe_jet.v4(), bHad.v4()) > radius
    && deltaR(probe_jet.v4(), q1.v4()) < radius
    && deltaR(probe_jet.v4(), q2.v4()) < radius) {
      return true;
    }
  }
  if(opt == oBplusQ || opt == oSemiMerged) {
    if( deltaR(probe_jet.v4(), bHad.v4()) < radius){
      if (deltaR(probe_jet.v4(), q1.v4()) < radius && deltaR(probe_jet.v4(), q2.v4()) > radius) return true;
      if (deltaR(probe_jet.v4(), q1.v4()) > radius && deltaR(probe_jet.v4(), q2.v4()) < radius) return true;
    }
  }
  if(opt == oLight|| opt == oNotMerged) {
    unsigned int N = 0;
    if( deltaR(probe_jet.v4(), bHad.v4()) < radius ) N++;
    if( deltaR(probe_jet.v4(), q1.v4()) < radius) N++;
    if( deltaR(probe_jet.v4(), q2.v4()) < radius) N++;
    if( N <= 1) return true;
  }

  return false;
}

bool MergedSelection::is_same_particle(GenParticle p1, GenParticle p2){
  // cout << "pt1 = " << p1.pt() << endl;
  // cout << "pt2 = " << p2.pt() << endl;
  double epsilon = 0.001;
  if(fabs(p1.pt()-p2.pt()) < epsilon) return true;
  else return false;
}

DecayChannelSelection::DecayChannelSelection( uhh2::Context& ctx,  const std::string ttbarGen_name_, TString channel_): ttbarGen_name(ttbarGen_name_), channel(channel_) {
  h_ttbarGen = ctx.get_handle<TTbarGen>(ttbarGen_name);
}

bool DecayChannelSelection::passes(const uhh2::Event &event) {

  const TTbarGen& ttbarGen = !ttbarGen_name.empty() ? event.get(h_ttbarGen) : TTbarGen(*event.genparticles,false);

  if( ttbarGen.DecayChannel() != TTbarGen::e_notfound) {
    if(channel == "dilepton") {
      if (ttbarGen.DecayChannel() == TTbarGen::e_mumu ||
      ttbarGen.DecayChannel() == TTbarGen::e_ee ||
      ttbarGen.DecayChannel() == TTbarGen::e_tautau ||
      ttbarGen.DecayChannel() == TTbarGen::e_emu ||
      ttbarGen.DecayChannel() == TTbarGen::e_etau ||
      ttbarGen.DecayChannel() == TTbarGen::e_mutau ) {
        return true;
      }
    }
    else if(channel == "lepton_jets") {
      if (ttbarGen.IsSemiLeptonicDecay() &&
      !(ttbarGen.DecayChannel() == TTbarGen::e_tauhad) ) {
        return true;
      }
    }
    else if(channel == "hadronic") {
      if (ttbarGen.DecayChannel() == TTbarGen::e_had) {
        return true;
      }
    }
    else if(channel == "tau_jets") {
      if (ttbarGen.DecayChannel() == TTbarGen::e_tauhad) {
        return true;
      }
    }
    else{
      std::cout << "NO proper decay channel selected: all events will be rejected by the decay channel selection" << std::endl;
    }
  }
  return false;
}

MassDiffSelection::MassDiffSelection(JetId btag):  btag_(btag) {}

bool MassDiffSelection::passes(const uhh2::Event &event){
  std::cout << "passes not used. reject all events!" << std::endl;
  return false;
}

bool MassDiffSelection::passes_probe(const uhh2::Event &event, const TopJet &probeJet){
  //get the muon
  Muon mu = event.muons->at(0);

  //get the bjet
  Jet bjet;

  bool bjet_found = false;
  bool b_candidate_found = false;

  double max_pt = 0.;
  std::vector<Jet> *ak4jets = event.jets;
  double pi = 3.14159265359;

  for( const auto & ak4jet : *ak4jets){
    Jet b_candidate;
    if( btag_(ak4jet, event) && (deltaPhi(ak4jet,mu) < (2*pi/3)) ){
      b_candidate = ak4jet;
      b_candidate_found = true;
    }
    if(b_candidate_found){
      if( b_candidate.pt() > max_pt){
        bjet = b_candidate;
        max_pt = b_candidate.pt();
        bjet_found = true;
      }
    }
  }

  if(!b_candidate_found) std::cout << "No Bjet candidate found in event:  " << event.event << std::endl;
  if(!bjet_found) std::cout << "No Bjet found in event: " << event.event << std::endl;

  double mProbe_mLep_bjet = probeJet.v4().M()/(bjet.v4()+mu.v4()).M();

  if(bjet_found && mProbe_mLep_bjet > 1.) return true;
  return false;

}


DPhiMuBSelection::DPhiMuBSelection( JetId btag, double dPhiMin):  btag_(btag), dPhiMin_(dPhiMin) {}

bool DPhiMuBSelection::passes(const uhh2::Event &event){
  std::cout << "passes not used. reject all events!" << std::endl;
  return false;
}

bool DPhiMuBSelection::passes_probe(const uhh2::Event &event, const TopJet &probeJet){
  //get the muon
  Muon mu = event.muons->at(0);

  //get the bjet
  Jet bjet;

  bool bjet_found = false;
  bool b_candidate_found = false;

  double max_pt = 0.;
  std::vector<Jet> *ak4jets = event.jets;
  double pi = 3.14159265359;

  for( const auto & ak4jet : *ak4jets){
    Jet b_candidate;
    if( btag_(ak4jet, event) && (deltaPhi(ak4jet,mu) < (2*pi/3)) ){
      b_candidate = ak4jet;
      b_candidate_found = true;
    }
    if(b_candidate_found){
      if( b_candidate.pt() > max_pt){
        bjet = b_candidate;
        max_pt = b_candidate.pt();
        bjet_found = true;
      }
    }
  }

  if(!b_candidate_found) std::cout << "No Bjet candidate found in event:  " << event.event << std::endl;
  if(!bjet_found) std::cout << "No Bjet found in event: " << event.event << std::endl;

  double dPhi = deltaPhi(probeJet, mu);

  if(bjet_found && dPhi >  dPhiMin_) return true;
  return false;

}

LeadingAddJetSelection::LeadingAddJetSelection( JetId btag, double ptMin): btag_(btag), ptMin_(ptMin) {}

bool LeadingAddJetSelection::passes(const uhh2::Event &event){
  std::cout << "passes not used. reject all events! please use passes_probe instead" << std::endl;
  return false;
}

bool LeadingAddJetSelection::passes_probe(const uhh2::Event &event, const TopJet &probeJet){
  Jet bjet;

  bool bjet_found = GetLeadingBjetLepHem(event, bjet, btag_ );
  if(!bjet_found) return false;

  for( const auto & jet : *event.jets){
    if(deltaR(jet, probeJet) > 0.8 && deltaR(jet, bjet) > 0.1){
      if(jet.pt() > ptMin_) return true;
    }
  }
  return false;
}

BadHCALSelection::BadHCALSelection(Context &ctx, long int seed) {
  m_seed = seed;
  m_rng = new TRandomMixMax();
  m_rng->SetSeed(m_seed);
  year = extract_year(ctx);
}

bool BadHCALSelection::passes(const Event &event) {

  if (year != Year::is2018) return true;

  // check if event should be removed:
  // for data: if event is affected by HEM15/16
  // for mc: draw random sample according to lumi ratio of affected data
  if ((event.isRealData && event.run >= m_runnumber) || (!event.isRealData && m_rng->Uniform() < m_lumi_ratio)){
    for (const Electron & e : *event.electrons){
      if (e.eta() < m_interval_eta && e.phi() > m_interval_phi_low && e.phi() < m_interval_phi_high) return false;
    }
    for (const Jet & j : *event.jets){
      if (j.eta() < m_interval_eta && j.phi() > m_interval_phi_low && j.phi() < m_interval_phi_high) return false;
    }
  }
  return true;
}

/*
HadronicTopSelection::HadronicTopSelection( uhh2::Context& ctx, const std::string ttbarGen_name_): ttbarGen_name(ttbarGen_name_) {
h_ttbarGen = ctx.get_handle<TTbarGen>(ttbarGen_name);
}

bool HadronicTopSelection::passes(const uhh2::Event &event) {
return false;
}
bool HadronicTopSelection::passes_jet(const uhh2::Event &event, const TopJet &jet) {

// const TTbarGen& ttbarGen = !ttbarGen_name.empty() ? event.get(h_ttbarGen) : TTbarGen(*event.genparticles,true);
const auto & ttbarGen = event.get(h_ttbarGen);

vector<GenParticle> tops;
double radius = 0.6;

if(IsTopHadronicDecay()) {
GenParticle b = ttbarGen.bTop();
GenParticle q1 = ttbarGen.Wdecay1();
GenParticle q2 = ttbarGen.Wdecay2();
if( deltaR(b.v4(),q1.v4()) < radius
&& deltaR(b.v4(), q2.v4()) < radius
&& deltaR(q1.v4(), q2.v4()) < radius) {
tops.push_back(ttbarGen.Top());
}
}
if(IsAntiTopHadronicDecay()) {
GenParticle b = ttbarGen.bAntitop();
GenParticle q1 = ttbarGen.WMinusdecay1();
GenParticle q2 = ttbarGen.WMinusdecay2();
if( deltaR(b.v4(),q1.v4()) < radius
&& deltaR(b.v4(), q2.v4()) < radius
&& deltaR(q1.v4(), q2.v4()) < radius) {
tops.push_back(ttbarGen.Antitop());
}
}

for( const auto & top : tops){
if(deltaR(jet.v4(),top.v4()) < radius) return true;
}

return false;
}


vector<GenParticles> GetMergedHadronicTops(TTbarGen ttbarGen,
*/

//};
