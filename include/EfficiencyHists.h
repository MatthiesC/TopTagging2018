#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/TopJet.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/TopJetIds.h"
#include "UHH2/common/include/JetIds.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/TTbarGen.h"

namespace uhh2examples {

class EfficiencyHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    EfficiencyHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;
    virtual void fill_probe(const uhh2::Event & ev, const TopJet & jet);
    virtual ~EfficiencyHists();

  private:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    bool isTTbar;

};

}
