#include "../include/CentralInclude.h"

using namespace std;

void HistCosmetics(TH1F* hist, int color);
TString year = "2018";

int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  TString InputPath_PUPPI = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel/";
  TString InputPath_HOTVR = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel_HOTVR/";

  std::vector<TString> MCNames {"TTbar_2018"};

  vector<TString> systematics {
    "BTAG_bc", "BTAG_udsg",
    "MUID",
    "MUTR",
    "PU",
    "ScaleVariationMuF",
    "ScaleVariationMuR",
    "JEC",
    "JER",
    "FSR"
  };


  std::vector<TString> Jets {"PUPPI", "HOTVR"};
  // std::vector<TString> Jets {"HOTVR"};

  for(auto jet: Jets){
    for(auto sys: systematics){
      for(auto mcname: MCNames){
        if(year == "2016")      mcname.ReplaceAll("2018", "2016v3");
        else if(year == "2017") mcname.ReplaceAll("2018", "2017v2");
        TCanvas * c = new TCanvas("","",600,600);
        TString histname = "ProbeJet_All_Pt400/mass_sub";
        TString InputPath = InputPath_PUPPI;
        if(jet == "HOTVR"){
          InputPath = InputPath_HOTVR;
        }
        // draw nominal hist
        TFile* f_nom = new TFile(InputPath+"uhh2.AnalysisModuleRunner.MC."+mcname+".root");
        TH1F* h_nom = (TH1F*) f_nom->Get(histname);
        HistCosmetics(h_nom, 13);
        h_nom->Draw("HIST");
        // now do all systematics
        TString up = "_up";
        TString down = "_down";
        if(sys == "FSR"){
          up += "_2";
          down += "_2";
        }
        TFile * f_up = new TFile(InputPath+sys+up+"/uhh2.AnalysisModuleRunner.MC."+mcname+".root");
        TFile * f_down = new TFile(InputPath+sys+down+"/uhh2.AnalysisModuleRunner.MC."+mcname+".root");
        TH1F* h_up = (TH1F*) f_up->Get(histname);
        TH1F* h_down = (TH1F*) f_down->Get(histname);
        HistCosmetics(h_up, kRed-2);
        HistCosmetics(h_down, kAzure+7);
        h_up->Draw("HIST SAME");
        h_down->Draw("HIST SAME");
        c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/SysVariations/"+year+"/"+sys+"_"+mcname+"_"+jet+".pdf");
        delete c;
      }
    }
  }
  return 0;
}

void HistCosmetics(TH1F* hist, int color){
  // hist->Rebin(5);
  hist->GetXaxis()->SetRangeUser(0, 400);
  hist->SetLineWidth(3);
  hist->SetLineColor(color);
}
