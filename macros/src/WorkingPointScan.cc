#include "../include/CentralInclude.h"


using namespace std;


int main(int argc, char* argv[]){

  TFile* f_tt = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar_2018.root");
  TFile* f_data = new TFile(dir+"uhh2.AnalysisModuleRunner.DATA.SingleMu_2018.root");

  TString histname = "ProbeJet_All_Pt400/tau32_scan";
  TH1F* h_tt = (TH1F*) f_tt->Get(histname);
  TH1F* h_data = (TH1F*) f_data->Get(histname);


  vector<double> wps = {0.40, 0.46, 0.54, 0.65, 0.80};

  cout << endl << endl;
  cout << "tau cut | efficiency ttbar | efficiency data" << endl;
  cout << "--------------------------------------------" << endl;

  for(auto wp: wps){
    int bin_lo = h_tt->FindBin(wp);
    int bin_hi = h_tt->FindBin(1.0);
    double effi_tt = h_tt->Integral(bin_lo, bin_hi) / h_tt->Integral();
    double effi_data = h_data->Integral(bin_lo, bin_hi) / h_data->Integral();
    cout << setw(7) << wp << " | " << setw(16) << effi_tt << " | " << setw(16) << effi_data << endl;
  }
  cout << endl << endl;
  return 0;
}
