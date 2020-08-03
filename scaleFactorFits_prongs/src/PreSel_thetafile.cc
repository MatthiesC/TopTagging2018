#include "../include/CentralInclude.h"

using namespace std;

void HistCosmetics(TH1F* hist,TString histname);
TString year = "2018";

int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  TString InputPath_PUPPI = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel/";
  TString InputPath_CHS = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel_CHS/";
  TString InputPath_HOTVR = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel_HOTVR/";

  std::vector<TString> MCNames {"TTbar_2018", "SingleTop_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};
  // std::vector<TString> MCNames {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018", "SingleTop_mergedTop_2018", "SingleTop_semimerged_2018", "SingleTop_notmerged_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};

  std::vector< vector<TString> > systematics {
    // {"Btag_bc__plus", "BTAG_bc_up"}, {"Btag_udsg__plus", "BTAG_udsg_up"},
    // {"Btag_bc__minus", "BTAG_bc_down"}, {"Btag_udsg__minus", "BTAG_udsg_down"},
    // {"MuonID__plus", "MUID_up"}, {"MuonID__minus", "MUID_down"},
    // {"Trigger__plus", "MUTR_up"}, {"Trigger__minus", "MUTR_down"},
    // {"PU__plus", "PU_up"}, {"PU__minus", "PU_down"},
    // {"ScaleMuF__plus", "ScaleVariationMuF_up"},  {"ScaleMuF__minus", "ScaleVariationMuF_down"},
    // {"ScaleMuR__plus", "ScaleVariationMuR_up"}, {"ScaleMuR__minus", "ScaleVariationMuR_down"},
    // {"JEC__plus", "JEC_up"}, {"JEC__minus", "JEC_down"},
    // {"JER__plus", "JER_up"}, {"JER__minus", "JER_down"},
    // {"FSR__plus", "FSR_up_2"}, {"FSR__minus", "FSR_down_2"}
  };


  std::vector<TString> Jets {"PUPPI"};

  for(auto jet: Jets){

    vector<TString> histnames = {"Event_sel/MET", "Muon_sel/pt_1"};
    vector<TString> outnames = {"MET", "Muon_pt"};
    TString InputPath = InputPath_PUPPI;
    for(unsigned int i=0; i<histnames.size(); i++){
      TFile *outputFile = new TFile("thetaFile_"+outnames[i]+"_"+year+".root","RECREATE");
      // first get and write data
      cout << "write data..." << endl;
      TString dataname = "uhh2.AnalysisModuleRunner.DATA.SingleMu_2018.root";
      if(year == "2016")      dataname.ReplaceAll("2018", "2016v3");
      else if(year == "2017") dataname.ReplaceAll("2018", "2017v2");
      TFile* f_data = new TFile(InputPath+dataname);
      TH1F* h_data = (TH1F*) f_data->Get(histnames[i]);
      HistCosmetics(h_data, outnames[i]);
      outputFile->cd();
      h_data->Write(outnames[i]+"__DATA");
      for(auto mcname: MCNames){
        if(year == "2016")      mcname.ReplaceAll("2018", "2016v3");
        else if(year == "2017") mcname.ReplaceAll("2018", "2017v2");
        // then write nominal hist
        cout << "write " << mcname << "..." << endl;
        TFile* f_nom = new TFile(InputPath+"uhh2.AnalysisModuleRunner.MC."+mcname+".root");
        TH1F* h_nom = (TH1F*) f_nom->Get(histnames[i]);
        HistCosmetics(h_nom, outnames[i]);
        outputFile->cd();
        h_nom->Write(outnames[i]+"__" + mcname);
        for(auto sys: systematics){
          // now do all systematics
          cout << "write " << mcname << "(" << sys[0] << ")" <<"..." << endl;
          TFile * f_sys = new TFile(InputPath+sys[1]+"/"+"uhh2.AnalysisModuleRunner.MC."+mcname+".root");
          TH1F* h_sys = (TH1F*) f_sys->Get(histnames[i]);
          HistCosmetics(h_sys,outnames[i]);
          outputFile->cd();
          h_sys->Write(outnames[i]+"__" + mcname + "__" + sys[0]);
        }
      }
      outputFile->Close();
    }
  }
  return 0;
}

void HistCosmetics(TH1F* hist, TString histname){
  hist->Rebin(2);
  if(histname == "MET"){
    hist->GetXaxis()->SetRangeUser(0, 500);
    hist->GetXaxis()->SetTitle("#it{p}_{T}^{miss}");
    hist->SetTitle("#it{p}_{T}^{miss}");
  }
  else if(histname == "Muon_pt"){
    hist->GetXaxis()->SetRangeUser(0, 500);
    hist->GetXaxis()->SetTitle("Muon #it{p}_{T}");
    hist->SetTitle("Muon #it{p}_{T}");
  }
}
