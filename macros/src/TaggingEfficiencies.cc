#include "../include/CentralInclude.h"


using namespace std;

TH1F* GetEfficiency(vector<TH1F*> h_pass, vector<TH1F*> h_fail, vector<int> pt_bins);

int main(int argc, char* argv[]){

  TString dir_PUPPI = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel/";
  TString dir_HOTVR = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel_HOTVR/";

  TFile* tt_PUPPI = new TFile(dir_PUPPI+"uhh2.AnalysisModuleRunner.MC.TTbar_mergedTop_2018.root");
  TFile* tt_HOTVR = new TFile(dir_HOTVR+"uhh2.AnalysisModuleRunner.MC.TTbar_mergedTop_2018.root");

  TFile* qcd_PUPPI = new TFile(dir_PUPPI+"uhh2.AnalysisModuleRunner.MC.QCD_2018.root");
  TFile* qcd_HOTVR = new TFile(dir_HOTVR+"uhh2.AnalysisModuleRunner.MC.QCD_2018.root");

  vector<TString> wps_PUPPI = {"wp1","wp2", "wp3", "wp4", "wp5", "wp1_btag","wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_HOTVR = {"HOTVR"};

  vector<int> pt_bins = {200, 250, 300, 400, 480, 600};

  vector<TString> jetcols = {"PUPPI", "HOTVR"};
  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------

  for(auto jet: jetcols){
    TFile *file, *file_mis;
    vector<TString> wps;
    if(jet == "PUPPI"){
      wps = wps_PUPPI;
      file = tt_PUPPI;
      file_mis = qcd_PUPPI;
    }
    else{
      wps = wps_HOTVR;
      file = tt_HOTVR;
      file_mis = qcd_HOTVR;
    }
    for(auto wp: wps){
      vector<TH1F*> h_pass, h_fail, h_pass_mis, h_fail_mis;
      for(unsigned int i=0; i<pt_bins.size(); i++){
        TString histname = "ProbeJet_pt";
        if(i < pt_bins.size()-1) histname += to_string(pt_bins[i])+"to"+to_string(pt_bins[i+1]);
        else histname += to_string(pt_bins[i]);
        if(wp != "HOTVR") histname += "_"+wp;
        if(!wp.Contains("btag")) histname += "_all";
        h_pass.push_back((TH1F*) file->Get(histname+"_pass/mass_sub"));
        h_fail.push_back((TH1F*) file->Get(histname+"_fail/mass_sub"));
        h_pass_mis.push_back((TH1F*) file_mis->Get(histname+"_pass/mass_sub"));
        h_fail_mis.push_back((TH1F*) file_mis->Get(histname+"_fail/mass_sub"));
      }
      TH1F* effi = GetEfficiency(h_pass, h_fail, pt_bins);
      TH1F* mist = GetEfficiency(h_pass_mis, h_fail_mis, pt_bins);
      gStyle->SetOptStat(kFALSE);
      gStyle->SetPadTickY(1);
      gStyle->SetPadTickX(1);

      TCanvas *a = new TCanvas("a", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      effi->SetTitle(" ");
      effi->GetXaxis()->SetRangeUser(0, 800);
      effi->GetYaxis()->SetRangeUser(0, 1);
      effi->GetYaxis()->SetTitleSize(0.06);
      effi->GetXaxis()->SetTitleSize(0.05);
      effi->GetZaxis()->SetTitleSize(0.05);
      effi->GetXaxis()->SetTitleOffset(0.9);
      effi->GetYaxis()->SetTitleOffset(1.1);
      effi->GetZaxis()->SetTitleOffset(0.9);
      effi->GetXaxis()->SetNdivisions(505);
      effi->GetYaxis()->SetNdivisions(505);
      effi->GetXaxis()->SetTitle("probe-jet p_{T}");
      effi->GetYaxis()->SetTitle("tag efficiency");
      effi->Draw("E1");
      TString t = jet;
      if(jet == "PUPPI") t = "#splitline{"+jet+"}{"+wp+"}";
      TLatex* text = new TLatex(3.5, 24, t);
      text->SetX(0.73);
      text->SetY(0.83);
      text->SetNDC();
      text->SetTextAlign(13);
      text->SetTextFont(42);
      text->SetTextSize(0.04);
      text->Draw();
      a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/Efficiency/effi_"+jet+"_"+wp+".pdf");
      delete a;
      delete effi;
      TCanvas *b = new TCanvas("b", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      mist->SetTitle(" ");
      mist->GetXaxis()->SetRangeUser(0, 800);
      mist->GetYaxis()->SetRangeUser(0, 0.1);
      mist->GetYaxis()->SetTitleSize(0.06);
      mist->GetXaxis()->SetTitleSize(0.05);
      mist->GetZaxis()->SetTitleSize(0.05);
      mist->GetXaxis()->SetTitleOffset(0.9);
      mist->GetYaxis()->SetTitleOffset(1.1);
      mist->GetZaxis()->SetTitleOffset(0.9);
      mist->GetXaxis()->SetNdivisions(505);
      mist->GetYaxis()->SetNdivisions(505);
      mist->GetXaxis()->SetTitle("probe-jet p_{T}");
      mist->GetYaxis()->SetTitle("mistag rate");
      mist->Draw("E1");
      text->Draw();
      b->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/Efficiency/mistag_"+jet+"_"+wp+".pdf");
      delete b;
      delete mist;
    }
  }





  return 0;
}

TH1F* GetEfficiency(vector<TH1F*> h_pass, vector<TH1F*> h_fail, vector<int> pt_bins){
  // convert to double
  vector<double> bins;
  for(auto p: pt_bins) bins.push_back(p);
  bins.push_back(800); // to get some upper boundary of last bin
  TH1F* effi = new TH1F("effi", "jet p_{T}", bins.size()-1, &bins[0]);
  for(unsigned int i=0; i<bins.size()-1; i++){
    int lower = 1;
    int upper = h_pass[i]->GetSize()-2;
    double n_pass = h_pass[i]->Integral();
    double n_fail = h_fail[i]->Integral();
    double n_sum = n_pass + n_fail;
    double efficiency = n_pass/n_sum;
    double error = sqrt(efficiency*(1-efficiency)/n_sum);
    effi->SetBinContent(i+1, efficiency);
    effi->SetBinError(i+1, error);
  }
  return effi;
}
