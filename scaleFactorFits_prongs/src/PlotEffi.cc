#include "../include/CentralInclude.h"

using namespace std;

void PlotEffi(TString dir, TString jet, TString wp);
void SetSameCentralValue(TGraphAsymmErrors* stat, TGraphAsymmErrors* tot);
TString year = "2018";



int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  TString plotdir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_FSR_f_prongs/"+year+"/";
  // vector<TString> jetcols = {"PUPPI"};
  vector<TString> jetcols = {"PUPPI", "HOTVR"};
  vector<TString> wps_PUPPI = {"wp1", "wp2", "wp3", "wp4", "wp5", "wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_HOTVR = {""};


  for(auto jet: jetcols){
    vector<TString> wps;
    if(jet == "PUPPI")      wps = wps_PUPPI;
    else if(jet == "HOTVR") wps = wps_HOTVR;
    for(auto wp: wps){
      PlotEffi(plotdir, jet, wp);
    }
  }
}

void PlotEffi(TString dir, TString jet, TString wp){
  TString filename = dir+"eff_hists_"+jet+"_"+wp+".root";
  TFile *file = new TFile(filename);
  vector<TString> procnames = {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018", "SingleTop_mergedTop_2018", "SingleTop_semimerged_2018", "SingleTop_notmerged_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};
  // vector<TString> procnames = {"mergedTop_2018", "semimergedTop_2018", "notmergedTop_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};
  // vector<TString> procnames = {"mergedTop_2018", "semimergedTop_2018", "Backgrounds_2018"};

  vector<TGraphAsymmErrors*> pre_stat, pre_tot, post_stat, post_tot;
  for(auto pname: procnames){
    if(year == "2016")      pname.ReplaceAll("2018", "2016v3");
    else if(year == "2017") pname.ReplaceAll("2018", "2017v2");
    pre_stat.push_back((TGraphAsymmErrors*) file->Get("effPreFit_"+pname+"_stat"));
    pre_tot.push_back((TGraphAsymmErrors*) file->Get("effPreFit_"+pname+"_tot"));
    post_stat.push_back((TGraphAsymmErrors*) file->Get("effPostFit_"+pname+"_stat"));
    post_tot.push_back((TGraphAsymmErrors*) file->Get("effPostFit_"+pname+"_tot"));
  }

  SetupGlobalStyle();
  for(unsigned int i=0; i<pre_stat.size(); i++){
    SetSameCentralValue(pre_stat[i], pre_tot[i]);
    SetSameCentralValue(post_stat[i], post_tot[i]);

    TCanvas *c = new TCanvas("c", "c", 600, 600);
    gPad->SetLeftMargin(0.15);
    pre_stat[i]->Draw("AP");
    pre_stat[i]->SetTitle(" ");
    pre_stat[i]->GetXaxis()->SetTitle("probe jet p_{T}");
    pre_stat[i]->GetYaxis()->SetTitle("efficiency");
    pre_stat[i]->GetXaxis()->SetTitleOffset(1.1);
    pre_stat[i]->GetYaxis()->SetTitleOffset(1.2);
    double xmin;
    if(jet == "PUPPI") xmin = 300;
    else xmin = 200;
    pre_stat[i]->GetXaxis()->SetRangeUser(xmin,1000.);
    pre_stat[i]->GetYaxis()->SetRangeUser(0.,1.);
    pre_tot[i]->SetMarkerColor(kBlack);
    pre_tot[i]->SetMarkerStyle(8);
    pre_tot[i]->SetMarkerSize(1);
    pre_tot[i]->Draw("E1 SAME");
    pre_stat[i]->SetMarkerColor(kBlack);
    pre_stat[i]->SetMarkerStyle(8);
    pre_stat[i]->SetMarkerSize(1);
    pre_stat[i]->Draw("E1 SAME");
    post_tot[i]->SetMarkerColor(kRed);
    post_tot[i]->SetLineColor(kRed);
    post_tot[i]->SetMarkerStyle(8);
    post_tot[i]->SetMarkerSize(1);
    post_tot[i]->Draw("P E1 SAME");
    post_stat[i]->SetMarkerColor(kRed);
    post_stat[i]->SetLineColor(kRed);
    post_stat[i]->SetMarkerStyle(8);
    post_stat[i]->SetMarkerSize(1);
    post_stat[i]->Draw("P E1 SAME");

    TLegend *leg = new TLegend(0.5, 0.6, 0.85, 0.85);
    leg->AddEntry(pre_stat[i], "pre fit", "pl");
    leg->AddEntry(post_stat[i], "post fit", "pl");
    leg->Draw();
    gPad->RedrawAxis();
    c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_prongs/"+year+"/Effi_"+jet+"_"+wp+"_"+procnames[i]+".pdf");
    delete c;
  }

}


void SetSameCentralValue(TGraphAsymmErrors* stat, TGraphAsymmErrors* tot){
  const int Npoints = tot->GetN();
  Double_t* x = tot->GetX();
  Double_t* y = tot->GetY();
  for(int i=0; i<Npoints; i++){
    stat->SetPoint(i, x[i], y[i]);
  }
  return;
}
