#include "../include/CentralInclude.h"


using namespace std;

TH1F* GetEfficiency(vector<TH1F*> h_pass, vector<TH1F*> h_fail, vector<int> pt_bins);
void WriteTable(vector<double> effis, vector<double> mistags, vector<TString> wps);

int main(int argc, char* argv[]){

  TString dir_PUPPI = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel/";
  TString dir_HOTVR = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel_HOTVR/";

  TFile* tt_PUPPI = new TFile(dir_PUPPI+"uhh2.AnalysisModuleRunner.MC.TTbar_2018.root");
  TFile* tt_HOTVR = new TFile(dir_HOTVR+"uhh2.AnalysisModuleRunner.MC.TTbar_2018.root");

  TFile* qcd_PUPPI = new TFile(dir_PUPPI+"uhh2.AnalysisModuleRunner.MC.QCD_2018.root");
  TFile* qcd_HOTVR = new TFile(dir_HOTVR+"uhh2.AnalysisModuleRunner.MC.QCD_2018.root");

  vector<TString> wps_PUPPI = {"wp1","wp2", "wp3", "wp4", "wp5", "wp1_btag","wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_HOTVR = {" "};

  vector<int> pt_bins;
  TString ptstring;
  if((TString)argv[1] == "300"){
    pt_bins = {300, 500};
    ptstring = "300to500";
  }
  else if((TString)argv[1] == "400"){
    pt_bins = {400, 800};
    ptstring = "400to800";
  }
  else if((TString)argv[1] == "480"){
    pt_bins = {480, 600};
    ptstring = "480to600";
  }
  else if((TString)argv[1] == "1000"){
    pt_bins = {1000, 2000};
    ptstring = "1000to2000";
  }
  else{
    cout << "argument not valid, try '300', '480' or '1000'" << endl;
    return 1;
  }

  // vector<TString> jetcols = {"PUPPI"};
  vector<TString> jetcols = {"PUPPI", "HOTVR"};

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------

  for(auto jet: jetcols){
    vector<double> efficiencies, mistags;
    cout << "-------------------------"<< endl;
    cout << jet << endl;

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
      TString histname = "EfficiencyHists_"+ptstring+"_"+wp;
      if(jet == "HOTVR") histname = "EfficiencyHists_"+ptstring;

      h_pass.push_back((TH1F*) file->Get(histname+"_pass/mass"));
      h_fail.push_back((TH1F*) file->Get(histname+"_fail/mass"));
      h_pass_mis.push_back((TH1F*) file_mis->Get(histname+"_pass/mass"));
      h_fail_mis.push_back((TH1F*) file_mis->Get(histname+"_fail/mass"));

      TH1F* effi = GetEfficiency(h_pass, h_fail, pt_bins);
      TH1F* mist = GetEfficiency(h_pass_mis, h_fail_mis, pt_bins);

      efficiencies.push_back(effi->GetBinContent(1));
      mistags.push_back(mist->GetBinContent(1));

      gStyle->SetOptStat(kFALSE);
      gStyle->SetPadTickY(1);
      gStyle->SetPadTickX(1);

      TCanvas *a = new TCanvas("a", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      effi->SetTitle(" ");
      effi->GetXaxis()->SetRangeUser(300, 500);
      effi->GetYaxis()->SetRangeUser(0, 1);
      effi->GetYaxis()->SetTitleSize(0.06);
      effi->GetXaxis()->SetTitleSize(0.05);
      effi->GetZaxis()->SetTitleSize(0.05);
      effi->GetXaxis()->SetTitleOffset(0.9);
      effi->GetYaxis()->SetTitleOffset(1.1);
      effi->GetZaxis()->SetTitleOffset(0.9);
      effi->GetXaxis()->SetNdivisions(505);
      effi->GetYaxis()->SetNdivisions(505);
      effi->GetXaxis()->SetTitle("Probe jet #it{p}_{T} [GeV]");
      effi->GetYaxis()->SetTitle("Efficiency");
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
      if(jet=="HOTVR") a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/Efficiency/2018/effi_"+jet+".pdf");
      else a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/Efficiency/2018/effi_"+jet+"_"+wp+".pdf");
      delete a;
      delete effi;
      TCanvas *b = new TCanvas("b", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      mist->SetTitle(" ");
      mist->GetXaxis()->SetRangeUser(300, 500);
      mist->GetYaxis()->SetRangeUser(0, 0.1);
      mist->GetYaxis()->SetTitleSize(0.06);
      mist->GetXaxis()->SetTitleSize(0.05);
      mist->GetZaxis()->SetTitleSize(0.05);
      mist->GetXaxis()->SetTitleOffset(0.9);
      mist->GetYaxis()->SetTitleOffset(1.1);
      mist->GetZaxis()->SetTitleOffset(0.9);
      mist->GetXaxis()->SetNdivisions(505);
      mist->GetYaxis()->SetNdivisions(505);
      mist->GetXaxis()->SetTitle("Probe jet #it{p}_{T} [GeV]");
      mist->GetYaxis()->SetTitle("Mistag rate");
      mist->Draw("E1");
      text->Draw();
      if(jet=="HOTVR") b->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/Efficiency/2018/mistag_"+jet+".pdf");
      else b->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/Efficiency/2018/mistag_"+jet+"_"+wp+".pdf");
      delete b;
      delete mist;
    }



    WriteTable(efficiencies, mistags, wps);
  }





  return 0;
}

TH1F* GetEfficiency(vector<TH1F*> h_pass, vector<TH1F*> h_fail, vector<int> pt_bins){
  // convert to double
  vector<double> bins;
  for(auto p: pt_bins) bins.push_back(p);
  TH1F* effi = new TH1F("effi", "jet p_{T}", bins.size()-1, &bins[0]);
  for(unsigned int i=0; i<bins.size()-1; i++){
    int lower = 1;
    int upper = h_pass[i]->GetSize()-2;
    double n_pass = h_pass[i]->Integral();
    double n_fail = h_fail[i]->Integral();
    double n_sum = n_pass + n_fail;
    double efficiency = n_pass/n_sum;
    // cout << "pass = " << n_pass << endl;
    // cout << "fail = " << n_fail << endl;
    // cout << "effi = " << efficiency << endl;
    double error = sqrt(efficiency*(1-efficiency)/n_sum);
    effi->SetBinContent(i+1, efficiency);
    effi->SetBinError(i+1, error);
  }
  return effi;
}

void WriteTable(vector<double> effis, vector<double> mistags, vector<TString> wps){
  vector<TString> taucut = {"0.40", "0.46", "0.54", "0.65", "0.80"};
  cout << "\\begin{tabular}{l | r | r}" << endl;
  cout << "& Efficiency signal & Efficiency background \\\\" << endl;
  cout << "\\hline" << endl;
  for(int i=0; i<effis.size(); i++){
    if(!wps[i].Contains("btag")) cout << "$\\tau_{32} < " << std::setprecision(4) << taucut[i] << "$ + mass & " << effis[i] << " & "  << mistags[i] << "\\\\" << endl;
    else cout << "$\\tau_{32} < " << taucut[i-5] << "$ + mass + subjet b tag & " << std::setprecision(4) << effis[i] << " & "  << mistags[i] << "\\\\" << endl;
  }
  cout << "\\end{tabular}" << endl;

}
