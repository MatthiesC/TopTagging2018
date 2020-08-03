#include "../include/CentralInclude.h"

using namespace std;

void PlotEffi(TString dir, TString jet, TString wp,  TString mass);
void SetSameCentralValue(TGraphAsymmErrors* stat, TGraphAsymmErrors* tot);
vector<double> GetMinMaxEffis(TString dir, TString jet, TString wp,  bool misstag);

TString year = "2018";


int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  TString plotdir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_FSR_f_prongs/"+year+"/";
  // vector<TString> jetcols = {"PUPPI"};
  vector<TString> jetcols = {"PUPPI", "HOTVR", "CHS"};
  vector<TString> wps_PUPPI = {"wp1", "wp2", "wp3", "wp4", "wp5", "wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_CHS = {"wp2", "wp3", "wp4", "wp5", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_HOTVR = {""};
  vector<TString> mass_switch = {"NoMass", "Mass", "MassOnly"};


  for(auto jet: jetcols){
    vector<TString> wps;
    if(jet == "PUPPI")      wps = wps_PUPPI;
    else if(jet == "HOTVR") wps = wps_HOTVR;
    else if(jet == "CHS")   wps = wps_CHS;

    vector<vector<double>> effi, misstag;
    for(auto wp: wps){
      vector<double> e = GetMinMaxEffis(plotdir, jet, wp, false);
      vector<double> m = GetMinMaxEffis(plotdir, jet, wp, true);
      effi.push_back(e);
      misstag.push_back(m);
      for(auto mass: mass_switch){
        PlotEffi(plotdir, jet, wp, mass);
      }
    }
    for(int i=0; i<wps.size(); i++){
      cout << wps[i] << "  effi = " << effi[i][0] <<  "--" << effi[i][1] << "    misstag = " << misstag[i][0] <<  "--" << misstag[i][1] << endl;
    }
  }
}

vector<double> GetMinMaxEffis(TString dir, TString jet, TString wp, bool misstag){
  TString filename = dir+"eff_hists_"+jet+"_"+wp+".root";
  TFile *file = new TFile(filename);
  TString pname = "TTbar_mergedTop_2018";
  if(misstag) pname = "TTbar_notmerged_2018";
  // if(misstag) pname = "QCD_2018";

  TGraphAsymmErrors* pre_tot = (TGraphAsymmErrors*) file->Get("effPreFit_"+pname+"_tot");
  TGraphAsymmErrors* post_tot = (TGraphAsymmErrors*) file->Get("effPostFit_"+pname+"_tot");

  const int Npoints = pre_tot->GetN();
  Double_t* y_pre = pre_tot->GetY();
  Double_t* y_post = post_tot->GetY();
  double ymin = 100, ymax = 0;
  for(int i=0; i<Npoints; i++){
    if(y_pre[i] < ymin) ymin = y_pre[i];
    if(y_pre[i] > ymax) ymax = y_pre[i];
  }
  for(int i=0; i<Npoints; i++){
    if(y_post[i] < ymin) ymin = y_post[i];
    if(y_post[i] > ymax) ymax = y_post[i];
  }
  vector<double> minmax = {ymin, ymax};
  return minmax;
}


void PlotEffi(TString dir, TString jet, TString wp, TString mass){
  TString filename = dir+"eff_hists_"+jet+"_"+wp+".root";
  TFile *file = new TFile(filename);
  vector<TString> procnames = {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018"};
  // vector<TString> procnames = {"mergedTop_2018", "semimergedTop_2018", "notmergedTop_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};
  // vector<TString> procnames = {"mergedTop_2018", "semimergedTop_2018", "Backgrounds_2018"};

  vector<TGraphAsymmErrors*> pre_stat, pre_tot, post_stat, post_tot;
  for(auto pname: procnames){
    if(year == "2016")      pname.ReplaceAll("2018", "2016v3");
    else if(year == "2017") pname.ReplaceAll("2018", "2017v2");
    if(mass== "NoMass"){
      pre_stat.push_back((TGraphAsymmErrors*) file->Get("effPreFit_NoMass_"+pname+"_stat"));
      pre_tot.push_back((TGraphAsymmErrors*) file->Get("effPreFit_NoMass_"+pname+"_tot"));
      post_stat.push_back((TGraphAsymmErrors*) file->Get("effPostFit_NoMass_"+pname+"_stat"));
      post_tot.push_back((TGraphAsymmErrors*) file->Get("effPostFit_NoMass_"+pname+"_tot"));
    }
    else if(mass== "MassOnly"){
      pre_stat.push_back((TGraphAsymmErrors*) file->Get("effPreFit_Mass_"+pname+"_stat"));
      pre_tot.push_back((TGraphAsymmErrors*) file->Get("effPreFit_Mass_"+pname+"_tot"));
      post_stat.push_back((TGraphAsymmErrors*) file->Get("effPostFit_Mass_"+pname+"_stat"));
      post_tot.push_back((TGraphAsymmErrors*) file->Get("effPostFit_Mass_"+pname+"_tot"));
    }
    else{
      pre_stat.push_back((TGraphAsymmErrors*) file->Get("effPreFit_"+pname+"_stat"));
      pre_tot.push_back((TGraphAsymmErrors*) file->Get("effPreFit_"+pname+"_tot"));
      post_stat.push_back((TGraphAsymmErrors*) file->Get("effPostFit_"+pname+"_stat"));
      post_tot.push_back((TGraphAsymmErrors*) file->Get("effPostFit_"+pname+"_tot"));
    }
  }

  SetupGlobalStyle();
  for(unsigned int i=0; i<pre_stat.size(); i++){
    SetSameCentralValue(pre_stat[i], pre_tot[i]);
    SetSameCentralValue(post_stat[i], post_tot[i]);

    TCanvas *c = new TCanvas("c", "c", 600, 600);
    gPad->SetLeftMargin(0.15);
    pre_tot[i]->Draw("AP");
    pre_tot[i]->SetTitle(" ");
    pre_tot[i]->GetXaxis()->SetTitle("Probe jet #it{p}_{T}");
    pre_tot[i]->GetYaxis()->SetTitle("Efficiency");
    pre_tot[i]->GetXaxis()->SetTitleOffset(1.1);
    pre_tot[i]->GetYaxis()->SetTitleOffset(1.2);
    double xmin;
    if(jet == "PUPPI") xmin = 300;
    else xmin = 200;
    pre_tot[i]->GetXaxis()->SetRangeUser(xmin,1000.);
    pre_tot[i]->GetYaxis()->SetRangeUser(0.,1.);
    pre_tot[i]->SetMarkerColor(kBlack);
    pre_tot[i]->SetMarkerStyle(8);
    pre_tot[i]->SetMarkerSize(1);
    pre_tot[i]->Draw("E1 SAME");
    // pre_stat[i]->SetMarkerColor(kBlack);
    // pre_stat[i]->SetMarkerStyle(8);
    // pre_stat[i]->SetMarkerSize(1);
    // pre_stat[i]->Draw("E1 SAME");
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

    TLegend *leg = new TLegend(0.4, 0.65, 0.85, 0.85);
    if(mass == "MassOnly") leg = new TLegend(0.4, 0.25, 0.85, 0.45);
    TString laglabel = "Total tagging efficiency";
    if(mass == "NoMass") laglabel = "Efficiency excluding mass window";
    else if(mass == "MassOnly") laglabel = "Efficiency of mass window";
    leg->AddEntry((TObject*)0, laglabel, "");
    leg->AddEntry(pre_tot[i], "Pre fit", "pl");
    leg->AddEntry(post_stat[i], "Post fit", "pl");
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

    TString add = "";
    if(mass == "NoMass") add = "NoMass_";
    else if(mass == "MassOnly") add = "MassOnly_";
    c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_prongs/"+year+"/Effi_"+add+jet+"_"+wp+"_"+procname+".pdf");
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
