#include "../include/CentralInclude.h"


using namespace std;


int main(int argc, char* argv[]){

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------- declare files --------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------
  vector<TFile*> files;
  // files.push_back(new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root"));
  // files.push_back(new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar_2017v2.root"));
  files.push_back(new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar_2018.root"));


  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------

  TString histname_DeepCSV = "ProbeJet_All_Pt400/subDeepJet_highest";
  TString histname_DeepJet = "ProbeJet_All_Pt400/subDeepJet_highest";

  // get loose WP for all three years
  vector<double> wp_loose_DeepCSV = {
    // 0.2217,
    // 0.1522,
    0.1241};
  vector<double> wp_loose_DeepJet = {
    // 0.0614,
    // 0.0521,
    0.0494};


  vector<double> effi_DeepCSV, effi_DeepJet;
  for(unsigned int i=0; i<files.size(); i++){
    TH1F* h_DeepCSV = (TH1F*) files[i]->Get(histname_DeepCSV);
    TH1F* h_DeepJet = (TH1F*) files[i]->Get(histname_DeepJet);

    int BinLo_DeepCSV = h_DeepCSV->GetXaxis()->FindBin(wp_loose_DeepCSV[i]);
    int BinHi_DeepCSV = h_DeepCSV->GetXaxis()->FindBin(1.0);
    int BinLo_DeepJet = h_DeepJet->GetXaxis()->FindBin(wp_loose_DeepJet[i]);
    int BinHi_DeepJet = h_DeepJet->GetXaxis()->FindBin(1.0);

    effi_DeepCSV.push_back(h_DeepCSV->Integral(BinLo_DeepCSV, BinHi_DeepCSV)/h_DeepCSV->Integral(0, BinHi_DeepCSV));
    effi_DeepJet.push_back(h_DeepJet->Integral(BinLo_DeepJet, BinHi_DeepJet)/h_DeepJet->Integral(0, BinHi_DeepJet));
  }

  for(unsigned int i=0; i<effi_DeepCSV.size(); i++){
    cout << "---------------------------------" << endl;
    cout << "efficiency DeepCSV = " << effi_DeepCSV[i] << endl;
    cout << "efficiency DeepJet = " << effi_DeepJet[i] << endl;
  }

  return 0;
}
