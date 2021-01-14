#include "../include/CentralInclude.h"

using namespace std;
TString year = "2018";



int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  TString InputPath_PUPPI = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel/";

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


  std::vector<TString> Jets {"PUPPI"};

  for(auto jet: Jets){

    vector<TString> histnames = {
      "ProbeJet_pt300to400_all_pass/subDeepCSV_highest",
      "ProbeJet_pt400to480_all_pass/subDeepCSV_highest",
      "ProbeJet_pt480to600_all_pass/subDeepCSV_highest",
      "ProbeJet_pt600_all_pass/subDeepCSV_highest"
    };

    TString InputPath = InputPath_PUPPI;
    TFile *outputFile = new TFile("thetaFile_subbtag_incl_"+year+"_"+jet+".root","RECREATE");
    // first get and write data
    cout << "write data..." << endl;
    TString dataname = "uhh2.AnalysisModuleRunner.DATA.SingleMu_2018.root";
    if(year == "2016")      dataname.ReplaceAll("2018", "2016v3");
    else if(year == "2017") dataname.ReplaceAll("2018", "2017v2");
    TFile* f_data = new TFile(InputPath+dataname);
    TH1F* h_data = (TH1F*) f_data->Get(histnames[0]);
    for(int i=1; i<histnames.size(); i++){
      TH1F* h = (TH1F*) f_data->Get(histnames[i]);
      h_data->Add(h);
    }
    outputFile->cd();
    h_data->Rebin(4);
    h_data->Write("subbtag__DATA");
    for(auto mcname: MCNames){
      if(year == "2016")      mcname.ReplaceAll("2018", "2016v3");
      else if(year == "2017") mcname.ReplaceAll("2018", "2017v2");
      // then write nominal hist
      cout << "write " << mcname << "..." << endl;
      TFile* f_nom = new TFile(InputPath+"uhh2.AnalysisModuleRunner.MC."+mcname+".root");
      TH1F* h_nom = (TH1F*) f_nom->Get(histnames[0]);
      for(int i=1; i<histnames.size(); i++){
        TH1F* h = (TH1F*) f_nom->Get(histnames[i]);
        h_nom->Add(h);
      }
      outputFile->cd();
      h_nom->Rebin(4);
      h_nom->Write("subbtag__" + mcname);
      for(auto sys: systematics){
        // now do all systematics
        cout << "write " << mcname << "(" << sys[0] << ")" <<"..." << endl;
        TFile * f_sys = new TFile(InputPath+sys[1]+"/"+"uhh2.AnalysisModuleRunner.MC."+mcname+".root");
        TH1F* h_sys = (TH1F*) f_sys->Get(histnames[0]);
        for(int i=1; i<histnames.size(); i++){
          TH1F* h = (TH1F*) f_sys->Get(histnames[i]);
          h_sys->Add(h);
        }
        outputFile->cd();
        h_sys->Rebin(4);
        h_sys->Write("subbtag__" + mcname + "__" + sys[0]);
      }
    }
    outputFile->Close();
  }
  return 0;
}
