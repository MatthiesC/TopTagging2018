#include "../include/CentralInclude.h"


using namespace std;


int main(int argc, char* argv[]){

  TFile* f_tt16 = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root");
  TFile* f_tt17 = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar_2017v2.root");
  TFile* f_tt17ul = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar_UL17.root");
  TFile* f_tt18 = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar_2018.root");
  // TFile* f_data = new TFile(dir+"uhh2.AnalysisModuleRunner.DATA.SingleMu_2018.root");

  TString histname = "ProbeJet_All_Pt400/tau32_scan";
  TH1F* h_tt16 = (TH1F*) f_tt16->Get(histname);
  TH1F* h_tt17 = (TH1F*) f_tt17->Get(histname);
  TH1F* h_tt17ul = (TH1F*) f_tt17ul->Get(histname);
  TH1F* h_tt18 = (TH1F*) f_tt18->Get(histname);

  // TH1F* h_data = (TH1F*) f_data->Get(histname);


  vector<double> wps = {0.40, 0.46, 0.54, 0.65, 0.80};

  cout << endl << endl;
  cout << "tau cut | efficiency ttbar 16 | efficiency ttbar 17 | efficiency ttbar 17UL | efficiency ttbar 18" << endl;
  cout << "-------------------------------------------------------------------------------------------------" << endl;

  for(auto wp: wps){
    int bin_lo = h_tt16->FindBin(0.0);
    int bin_hi = h_tt16->FindBin(wp);
    double effi_tt16 = h_tt16->Integral(bin_lo, bin_hi) / h_tt16->Integral();
    double effi_tt17 = h_tt17->Integral(bin_lo, bin_hi) / h_tt17->Integral();
    double effi_tt17ul = h_tt17ul->Integral(bin_lo, bin_hi) / h_tt17ul->Integral();
    double effi_tt18 = h_tt18->Integral(bin_lo, bin_hi) / h_tt18->Integral();

    // double effi_data = h_data->Integral(bin_lo, bin_hi) / h_data->Integral();
    cout << setw(7) << wp << " | " << setw(19) << effi_tt16 << " | " << setw(19) << effi_tt17 << " | ";
    cout << setw(21) << effi_tt17ul << " | " << setw(19) << effi_tt18 << endl;
  }
  cout << endl << endl;
  return 0;
}
