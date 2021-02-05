#include "UHH2/TopTagging2018/include/EfficiencyHists.h"


#include "TH1F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

EfficiencyHists::EfficiencyHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  TString vers = (TString) ctx.get("dataset_version", "");
  if(vers.Contains("TTbar") ) isTTbar = true;
  else isTTbar = false;
  if(isTTbar) h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");

  book<TH1F>("mass", "Soft drop mass [GeV]", 100, 0, 500);
  book<TH1F>("tau32", "Probe jet #tau_{3}/#tau_{2}", 100, 0, 1);
  book<TH1F>("pt", "Probe jet p_{T} [GeV]", 100, 0, 2000);
}

void EfficiencyHists::fill(const Event & event){
}

void EfficiencyHists::fill_probe(const Event & event, const TopJet & jet){
  double weight = event.weight;

  // if ttbar, only keep semilep events with matched jets
  if(isTTbar){
    const auto & ttbargen = event.get(h_ttbargen);
    if(!ttbargen.IsSemiLeptonicDecay()) return;

    GenParticle tHad;
    if(ttbargen.IsTopHadronicDecay()) tHad = ttbargen.Top();
    else if(ttbargen.IsAntiTopHadronicDecay()) tHad = ttbargen.Antitop();
    else return;

    if(deltaR(jet, tHad) > 0.6) return;
  }

  auto subjets = jet.subjets();
  sort_by_pt(subjets);

  LorentzVector subjet_sum(0,0,0,0);
  for (const auto s : subjets) {
    subjet_sum += s.v4();
  }
  double softdropmass = subjet_sum.M();
  hist("mass")->Fill(softdropmass, weight);
  hist("tau32")->Fill(jet.tau3()/jet.tau2(), weight);
  hist("pt")->Fill(jet.pt(), weight);

}


EfficiencyHists::~EfficiencyHists(){}
