#include "../include/CentralInclude.h"

using namespace std;

void HistCosmetics(TH1F* hist,TString histname);
TString year = "2018";

int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  TString InputPath = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel_HOTVR/";

  std::vector<TString> MCNames {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018", "SingleTop_mergedTop_2018", "SingleTop_semimerged_2018", "SingleTop_notmerged_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};

  std::vector< vector<TString> > systematics {
    {"Btag_bc__plus", "BTAG_bc_up"}, {"Btag_udsg__plus", "BTAG_udsg_up"},
    {"Btag_bc__minus", "BTAG_bc_down"}, {"Btag_udsg__minus", "BTAG_udsg_down"},
    {"MuonID__plus", "MUID_up"}, {"MuonID__minus", "MUID_down"},
    {"Trigger__plus", "MUTR_up"}, {"Trigger__minus", "MUTR_down"},
    {"PU__plus", "PU_up"}, {"PU__minus", "PU_down"},
    {"ScaleMuF__plus", "ScaleVariationMuF_up"},  {"ScaleMuF__minus", "ScaleVariationMuF_down"},
    {"ScaleMuR__plus", "ScaleVariationMuR_up"}, {"ScaleMuR__minus", "ScaleVariationMuR_down"},
    {"JEC__plus", "JEC_up"}, {"JEC__minus", "JEC_down"},
    {"JER__plus", "JER_up"}, {"JER__minus", "JER_down"},
    {"FSR__plus", "FSR_up_2"}, {"FSR__minus", "FSR_down_2"}
  };


  vector<TString> histdirs = {"ProbeJet_All_Pt200to250", "ProbeJet_All_Pt250to300", "ProbeJet_All_Pt300to400", "ProbeJet_All_Pt400"};
  vector<TString> histnames = {"min_pairmass", "Nsub", "rel_ptSub_ratio"};
  vector<TString> outnames = {"MinMass", "Nsub", "SubjetRatio"};


  for(unsigned int i=0; i<histnames.size(); i++){
    TFile *outputFile = new TFile("thetaFile_HOTVR_"+outnames[i]+"_"+year+".root","RECREATE");
    // first get and write data
    cout << "write data..." << endl;
    TString dataname = "uhh2.AnalysisModuleRunner.DATA.SingleMu_2018.root";
    if(year == "2016")      dataname.ReplaceAll("2018", "2016v3");
    else if(year == "2017") dataname.ReplaceAll("2018", "2017v2");
    TFile* f_data = new TFile(InputPath+dataname);
    TH1F* h_data = (TH1F*) f_data->Get(histdirs[0]+"/"+histnames[i]);
    for(unsigned int j=1; j<histdirs.size(); j++){
      TH1F* h = (TH1F*) f_data->Get(histdirs[j]+"/"+histnames[i]);
      h_data->Add(h);
    }
    HistCosmetics(h_data, outnames[i]);
    outputFile->cd();
    h_data->Write(outnames[i]+"__DATA");
    for(auto mcname: MCNames){
      if(year == "2016")      mcname.ReplaceAll("2018", "2016v3");
      else if(year == "2017") mcname.ReplaceAll("2018", "2017v2");
      // then write nominal hist
      cout << "write " << mcname << "..." << endl;
      TFile* f_nom = new TFile(InputPath+"uhh2.AnalysisModuleRunner.MC."+mcname+".root");
      TH1F* h_nom = (TH1F*) f_nom->Get(histdirs[0]+"/"+histnames[i]);
      for(unsigned int j=1; j<histdirs.size(); j++){
        TH1F* h = (TH1F*) f_nom->Get(histdirs[j]+"/"+histnames[i]);
        h_nom->Add(h);
      }
      HistCosmetics(h_nom, outnames[i]);
      outputFile->cd();
      h_nom->Write(outnames[i]+"__" + mcname);
      for(auto sys: systematics){
        // now do all systematics
        cout << "write " << mcname << "(" << sys[0] << ")" <<"..." << endl;
        TFile * f_sys = new TFile(InputPath+sys[1]+"/"+"uhh2.AnalysisModuleRunner.MC."+mcname+".root");
        TH1F* h_sys = (TH1F*) f_sys->Get(histdirs[0]+"/"+histnames[i]);
        for(unsigned int j=1; j<histdirs.size(); j++){
          TH1F* h = (TH1F*) f_sys->Get(histdirs[j]+"/"+histnames[i]);
          h_sys->Add(h);
        }
        HistCosmetics(h_sys,outnames[i]);
        outputFile->cd();
        h_sys->Write(outnames[i]+"__" + mcname + "__" + sys[0]);
      }
    }
    outputFile->Close();
  }
  return 0;
}

void HistCosmetics(TH1F* hist, TString histname){
  if(histname == "MinMass"){
    hist->Rebin(2);
    hist->GetXaxis()->SetRangeUser(0, 300);
  }
  else if(histname == "Nsub"){
    // hist->GetXaxis()->SetRangeUser(0, 500);
    // hist->GetXaxis()->SetTitle("Muon #it{p}_{T}");
    hist->SetTitle("Nsub");
  }
  else if(histname == "SubjetRatio"){
    hist->Rebin(2);
    hist->SetTitle("SubjetRatio");

    // hist->GetXaxis()->SetRangeUser(0, 1.5);
    // hist->GetXaxis()->SetTitle("#frac{Leading subjet #it{p}_{T}}{Probe jet #it{p}_{T}}");
  }
}
