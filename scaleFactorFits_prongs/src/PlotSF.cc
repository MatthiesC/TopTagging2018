#include "../include/CentralInclude.h"

using namespace std;

void PlotSF(TString dir, TString jet, TString wp);
void SetSameCentralValue(TGraphAsymmErrors* stat, TGraphAsymmErrors* tot);
TString year = "2018";



int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  TString plotdir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_FSR_f_prongs/"+year+"/";
  // vector<TString> jetcols = {"HOTVR"};
  vector<TString> jetcols = {"PUPPI", "HOTVR", "CHS"};
  vector<TString> wps_PUPPI = {"wp1", "wp2", "wp3", "wp4", "wp5", "wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_CHS = {"wp2", "wp3", "wp4", "wp5", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_HOTVR = {""};


  for(auto jet: jetcols){
    vector<TString> wps;
    if(jet == "PUPPI")      wps = wps_PUPPI;
    else if(jet == "HOTVR") wps = wps_HOTVR;
    else if(jet == "CHS")   wps = wps_CHS;
    for(auto wp: wps){
      PlotSF(plotdir, jet, wp);
    }
  }
}

void PlotSF(TString dir, TString jet, TString wp){
  TString filename = dir+"eff_hists_"+jet+"_"+wp+".root";
  TFile *file = new TFile(filename);
  vector<TString> procnames = {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018", "SingleTop_mergedTop_2018", "SingleTop_semimerged_2018", "SingleTop_notmerged_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};
  // vector<TString> procnames = {"mergedTop_2018", "semimergedTop_2018", "notmergedTop_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};
  // vector<TString> procnames = {"mergedTop_2018", "semimergedTop_2018", "Backgrounds_2018"};
  vector<TGraphAsymmErrors*> SFs_stat, SFs_tot;
  for(auto pname: procnames){
    if(year == "2016")      pname.ReplaceAll("2018", "2016v3");
    else if(year == "2017") pname.ReplaceAll("2018", "2017v2");
    SFs_stat.push_back((TGraphAsymmErrors*) file->Get("sf_"+pname+"_stat"));
    SFs_tot.push_back((TGraphAsymmErrors*) file->Get("sf_"+pname+"_tot"));
  }
  SetupGlobalStyle();
  for(unsigned int i=0; i<SFs_stat.size(); i++){
    SetSameCentralValue(SFs_stat[i], SFs_tot[i]);
    TCanvas *c = new TCanvas("c", "c", 600, 600);
    gPad->SetLeftMargin(0.15);
    SFs_stat[i]->Draw("AP");
    SFs_stat[i]->SetTitle(" ");
    SFs_stat[i]->GetXaxis()->SetTitle("Probe jet #it{p}_{T}");
    SFs_stat[i]->GetYaxis()->SetTitle("Scale factor");
    SFs_stat[i]->GetXaxis()->SetTitleOffset(1.1);
    SFs_stat[i]->GetYaxis()->SetTitleOffset(1.2);
    double xmin;
    if(jet == "PUPPI") xmin = 300;
    else xmin = 200;
    SFs_stat[i]->GetXaxis()->SetRangeUser(xmin,1000.);
    SFs_stat[i]->GetYaxis()->SetRangeUser(0.,2.);
    SFs_tot[i]->SetFillColor(16);
    SFs_tot[i]->Draw("E2 SAME");
    SFs_stat[i]->SetFillColor(797);
    SFs_stat[i]->SetMarkerColor(kBlack);
    SFs_stat[i]->SetMarkerStyle(8);
    SFs_stat[i]->SetMarkerSize(1);
    SFs_stat[i]->Draw("E2 SAME");
    SFs_stat[i]->Draw("PX SAME");
    TLegend *leg = new TLegend(0.5, 0.6, 0.85, 0.85);
    leg->AddEntry(SFs_stat[i], "Scale factor", "p");
    leg->AddEntry(SFs_stat[i], "Stat uncertainty", "f");
    leg->AddEntry(SFs_tot[i], "Total uncertainty", "f");
    TString leglabel2 = procnames[i];
    if(procnames[i].Contains("TTbar_mergedTop")) leglabel2 = "t#bar{t} merged top";
    if(procnames[i].Contains("TTbar_semimerged")) leglabel2 = "t#bar{t} semimerged top";
    if(procnames[i].Contains("TTbar_notmerged")) leglabel2 = "t#bar{t} unmerged top";
    leg->AddEntry((TObject*)0, leglabel2, "");
    leg->Draw();
    gPad->RedrawAxis();

    TString jettext = "AK8 PUPPI";
    if(jet.Contains("HOTVR")) jettext = "HOTVR PUPPI";
    TLatex* jet_label = new TLatex(3.5, 24, jettext);
    jet_label->SetX(0.15);
    jet_label->SetY(0.915);
    jet_label->SetNDC();
    jet_label->SetTextAlign(11);
    jet_label->SetTextFont(42);
    jet_label->SetTextSize(0.045);
    jet_label->Draw();

    TString procname = procnames[i];
    if(year == "2016")      procname.ReplaceAll("2018", "2016v3");
    else if(year == "2017") procname.ReplaceAll("2018", "2017v2");
    c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_prongs/"+year+"/SF_"+jet+"_"+wp+"_"+procname+".pdf");
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
