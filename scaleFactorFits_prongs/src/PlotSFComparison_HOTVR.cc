#include "../include/CentralInclude.h"

using namespace std;

void SetSameCentralValue(TGraphAsymmErrors* stat, TGraphAsymmErrors* tot);
void AdjustXValues(vector<TGraphAsymmErrors*> graphs, int ptbins, double offset);
void AdjustYValues(vector<TGraphAsymmErrors*> graphs, double shift, int ptbins);

int main(int argc, char* argv[]){

  TString plotdir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_FSR_f_prongs/";
  vector<TString> procnames = {"TTbar_mergedTop", "TTbar_semimerged", "TTbar_notmerged"};

  TString jet = "HOTVR";
  TString filename = "eff_hists_HOTVR_.root";

  vector<TGraphAsymmErrors*> SFmerged_stat_2018, SFmerged_tot_2018;
  vector<TGraphAsymmErrors*> SFsemimerged_stat_2018, SFsemimerged_tot_2018;
  vector<TGraphAsymmErrors*> SFnotmerged_stat_2018, SFnotmerged_tot_2018;

  vector<TGraphAsymmErrors*> SFmerged_stat_2017, SFmerged_tot_2017;
  vector<TGraphAsymmErrors*> SFsemimerged_stat_2017, SFsemimerged_tot_2017;
  vector<TGraphAsymmErrors*> SFnotmerged_stat_2017, SFnotmerged_tot_2017;

  vector<TGraphAsymmErrors*> SFmerged_stat_2016, SFmerged_tot_2016;
  vector<TGraphAsymmErrors*> SFsemimerged_stat_2016, SFsemimerged_tot_2016;
  vector<TGraphAsymmErrors*> SFnotmerged_stat_2016, SFnotmerged_tot_2016;

  TFile *file_2018 = new TFile(plotdir+"2018/"+filename);
  TFile *file_2017 = new TFile(plotdir+"2017/"+filename);
  TFile *file_2016 = new TFile(plotdir+"2016/"+filename);

  SFmerged_stat_2018.push_back((TGraphAsymmErrors*) file_2018->Get("sf_"+procnames[0]+"_2018_stat"));
  SFmerged_tot_2018.push_back((TGraphAsymmErrors*) file_2018->Get("sf_"+procnames[0]+"_2018_tot"));
  SFsemimerged_stat_2018.push_back((TGraphAsymmErrors*) file_2018->Get("sf_"+procnames[1]+"_2018_stat"));
  SFsemimerged_tot_2018.push_back((TGraphAsymmErrors*) file_2018->Get("sf_"+procnames[1]+"_2018_tot"));
  SFnotmerged_stat_2018.push_back((TGraphAsymmErrors*) file_2018->Get("sf_"+procnames[2]+"_2018_stat"));
  SFnotmerged_tot_2018.push_back((TGraphAsymmErrors*) file_2018->Get("sf_"+procnames[2]+"_2018_tot"));

  SFmerged_stat_2017.push_back((TGraphAsymmErrors*) file_2017->Get("sf_"+procnames[0]+"_2017v2_stat"));
  SFmerged_tot_2017.push_back((TGraphAsymmErrors*) file_2017->Get("sf_"+procnames[0]+"_2017v2_tot"));
  SFsemimerged_stat_2017.push_back((TGraphAsymmErrors*) file_2017->Get("sf_"+procnames[1]+"_2017v2_stat"));
  SFsemimerged_tot_2017.push_back((TGraphAsymmErrors*) file_2017->Get("sf_"+procnames[1]+"_2017v2_tot"));
  SFnotmerged_stat_2017.push_back((TGraphAsymmErrors*) file_2017->Get("sf_"+procnames[2]+"_2017v2_stat"));
  SFnotmerged_tot_2017.push_back((TGraphAsymmErrors*) file_2017->Get("sf_"+procnames[2]+"_2017v2_tot"));

  SFmerged_stat_2016.push_back((TGraphAsymmErrors*) file_2016->Get("sf_"+procnames[0]+"_2016v3_stat"));
  SFmerged_tot_2016.push_back((TGraphAsymmErrors*) file_2016->Get("sf_"+procnames[0]+"_2016v3_tot"));
  SFsemimerged_stat_2016.push_back((TGraphAsymmErrors*) file_2016->Get("sf_"+procnames[1]+"_2016v3_stat"));
  SFsemimerged_tot_2016.push_back((TGraphAsymmErrors*) file_2016->Get("sf_"+procnames[1]+"_2016v3_tot"));
  SFnotmerged_stat_2016.push_back((TGraphAsymmErrors*) file_2016->Get("sf_"+procnames[2]+"_2016v3_stat"));
  SFnotmerged_tot_2016.push_back((TGraphAsymmErrors*) file_2016->Get("sf_"+procnames[2]+"_2016v3_tot"));

  for(unsigned int i=0; i<SFmerged_stat_2018.size(); i++) SetSameCentralValue(SFmerged_stat_2018[i], SFmerged_tot_2018[i]);
  for(unsigned int i=0; i<SFsemimerged_stat_2018.size(); i++) SetSameCentralValue(SFsemimerged_stat_2018[i], SFsemimerged_tot_2018[i]);
  for(unsigned int i=0; i<SFnotmerged_stat_2018.size(); i++) SetSameCentralValue(SFnotmerged_stat_2018[i], SFnotmerged_tot_2018[i]);

  for(unsigned int i=0; i<SFmerged_stat_2017.size(); i++) SetSameCentralValue(SFmerged_stat_2017[i], SFmerged_tot_2017[i]);
  for(unsigned int i=0; i<SFsemimerged_stat_2017.size(); i++) SetSameCentralValue(SFsemimerged_stat_2017[i], SFsemimerged_tot_2017[i]);
  for(unsigned int i=0; i<SFnotmerged_stat_2017.size(); i++) SetSameCentralValue(SFnotmerged_stat_2017[i], SFnotmerged_tot_2017[i]);

  for(unsigned int i=0; i<SFmerged_stat_2016.size(); i++) SetSameCentralValue(SFmerged_stat_2016[i], SFmerged_tot_2016[i]);
  for(unsigned int i=0; i<SFsemimerged_stat_2016.size(); i++) SetSameCentralValue(SFsemimerged_stat_2016[i], SFsemimerged_tot_2016[i]);
  for(unsigned int i=0; i<SFnotmerged_stat_2016.size(); i++) SetSameCentralValue(SFnotmerged_stat_2016[i], SFnotmerged_tot_2016[i]);

  int ptbins = 6;
  double offset = 1.5;
  AdjustXValues(SFmerged_stat_2018, ptbins, -offset);
  AdjustXValues(SFmerged_tot_2018, ptbins, -offset);
  AdjustXValues(SFsemimerged_stat_2018, ptbins, -offset);
  AdjustXValues(SFsemimerged_tot_2018, ptbins, -offset);
  AdjustXValues(SFnotmerged_stat_2018, ptbins, -offset);
  AdjustXValues(SFnotmerged_tot_2018, ptbins, -offset);
  AdjustYValues(SFsemimerged_stat_2018, -2, ptbins);
  AdjustYValues(SFsemimerged_tot_2018, -2, ptbins);
  AdjustYValues(SFnotmerged_stat_2018, -4, ptbins);
  AdjustYValues(SFnotmerged_tot_2018, -4, ptbins);

  AdjustXValues(SFmerged_stat_2017, ptbins, 0);
  AdjustXValues(SFmerged_tot_2017, ptbins, 0);
  AdjustXValues(SFsemimerged_stat_2017, ptbins, 0);
  AdjustXValues(SFsemimerged_tot_2017, ptbins, 0);
  AdjustXValues(SFnotmerged_stat_2017, ptbins, 0);
  AdjustXValues(SFnotmerged_tot_2017, ptbins, 0);
  AdjustYValues(SFsemimerged_stat_2017, -2, ptbins);
  AdjustYValues(SFsemimerged_tot_2017, -2, ptbins);
  AdjustYValues(SFnotmerged_stat_2017, -4, ptbins);
  AdjustYValues(SFnotmerged_tot_2017, -4, ptbins);

  AdjustXValues(SFmerged_stat_2016, ptbins, offset);
  AdjustXValues(SFmerged_tot_2016, ptbins, offset);
  AdjustXValues(SFsemimerged_stat_2016, ptbins, offset);
  AdjustXValues(SFsemimerged_tot_2016, ptbins, offset);
  AdjustXValues(SFnotmerged_stat_2016, ptbins, offset);
  AdjustXValues(SFnotmerged_tot_2016, ptbins, offset);
  AdjustYValues(SFsemimerged_stat_2016, -2, ptbins);
  AdjustYValues(SFsemimerged_tot_2016, -2, ptbins);
  AdjustYValues(SFnotmerged_stat_2016, -4, ptbins);
  AdjustYValues(SFnotmerged_tot_2016, -4, ptbins);

  // now plot here for every jet collection
  double x[] = {0, 22};
  double y[] = {-2.9, 2.5};
  if(jet=="HOTVR") x[1] = 33;

  TGraph* dummy = new TGraph(2, x, y);
  SetupGlobalStyle();
  int width = 900;
  if(jet == "HOTVR") width = 1350;
  TCanvas *c = new TCanvas("c", "c", 900, 600);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  dummy->Draw("AP");
  dummy->SetTitle(" ");
  dummy->GetXaxis()->SetTickLength(0.0);
  dummy->GetXaxis()->SetLabelSize(0.0);
  dummy->GetYaxis()->SetLabelSize(0.0);
  dummy->GetYaxis()->SetTitle("scale factor");
  dummy->GetYaxis()->SetTitleOffset(1.2);
  dummy->GetYaxis()->SetTitleSize(0.05);
  dummy->GetYaxis()->SetNdivisions(524);
  dummy->SetMarkerColor(kWhite);

  // plot vertical lines for pt bins
  vector<TLine*> l_ptbin;
  double ymin = -3.6;
  double ymax = 1.5;
  if(jet == "HOTVR"){
    l_ptbin.push_back(new TLine( 6.0, ymin,  6.0, ymax));
    l_ptbin.push_back(new TLine(12.0, ymin, 12.0, ymax));
    l_ptbin.push_back(new TLine(18.0, ymin, 18.0, ymax));
    l_ptbin.push_back(new TLine(24.0, ymin, 24.0, ymax));
    l_ptbin.push_back(new TLine(30.0, ymin, 30.0, ymax));
  }
  else{
    l_ptbin.push_back(new TLine( 6, ymin,  6, ymax));
    l_ptbin.push_back(new TLine(12, ymin, 12, ymax));
    l_ptbin.push_back(new TLine(18, ymin, 18, ymax));
  }
  for(auto l: l_ptbin){
    l->SetLineColor(kBlack);
    l->SetLineWidth(2);
    l->Draw("SAME");
  }

  // plot horizontal lines
  vector<TLine*> lines;
  int xmax = 24;
  if(jet == "HOTVR") xmax = 36;
  lines.push_back(new TLine(0, 1, xmax, 1));
  lines.push_back(new TLine(0, -1, xmax, -1));
  lines.push_back(new TLine(0, -3, xmax, -3));
  for(auto line: lines){
    line->SetLineStyle(7);
    line->SetLineColor(13);
    line->SetLineWidth(3);
    line->Draw("SAME");
  }

  // plot SF
  vector<int> fill_stat = {797, kRed, kBlue, kGreen, kMagenta};
  vector<int> fill_sys = {794, kRed-2, kBlue-2, kGreen-2, kMagenta-2};
  for(unsigned int i=0; i<SFmerged_stat_2018.size(); i++){
    SFmerged_stat_2018[i]->SetFillColor(fill_stat[i]);
    SFmerged_tot_2018[i]->SetFillColor(fill_sys[i]);
    SFmerged_stat_2018[i]->SetMarkerColor(kBlack);
    SFmerged_stat_2018[i]->SetMarkerStyle(8);
    SFmerged_stat_2018[i]->SetMarkerSize(1);
    SFmerged_tot_2018[i]->Draw("E2 SAME");
    SFmerged_stat_2018[i]->Draw("E2 SAME");
    SFmerged_tot_2018[i]->Draw("PX SAME");

    SFmerged_stat_2017[i]->SetFillColor(fill_stat[i+1]);
    SFmerged_tot_2017[i]->SetFillColor(fill_sys[i+1]);
    SFmerged_stat_2017[i]->SetMarkerColor(kBlack);
    SFmerged_stat_2017[i]->SetMarkerStyle(8);
    SFmerged_stat_2017[i]->SetMarkerSize(1);
    SFmerged_tot_2017[i]->Draw("E2 SAME");
    SFmerged_stat_2017[i]->Draw("E2 SAME");
    SFmerged_tot_2017[i]->Draw("PX SAME");

    SFmerged_stat_2016[i]->SetFillColor(fill_stat[i+2]);
    SFmerged_tot_2016[i]->SetFillColor(fill_sys[i+2]);
    SFmerged_stat_2016[i]->SetMarkerColor(kBlack);
    SFmerged_stat_2016[i]->SetMarkerStyle(8);
    SFmerged_stat_2016[i]->SetMarkerSize(1);
    SFmerged_tot_2016[i]->Draw("E2 SAME");
    SFmerged_stat_2016[i]->Draw("E2 SAME");
    SFmerged_tot_2016[i]->Draw("PX SAME");
  }
  for(unsigned int i=0; i<SFsemimerged_stat_2018.size(); i++){
    SFsemimerged_stat_2018[i]->SetFillColor(fill_stat[i]);
    SFsemimerged_tot_2018[i]->SetFillColor(fill_sys[i]);
    SFsemimerged_stat_2018[i]->SetMarkerColor(kBlack);
    SFsemimerged_stat_2018[i]->SetMarkerStyle(8);
    SFsemimerged_stat_2018[i]->SetMarkerSize(1);
    SFsemimerged_tot_2018[i]->Draw("E2 SAME");
    SFsemimerged_stat_2018[i]->Draw("E2 SAME");
    SFsemimerged_tot_2018[i]->Draw("PX SAME");

    SFsemimerged_stat_2017[i]->SetFillColor(fill_stat[i+1]);
    SFsemimerged_tot_2017[i]->SetFillColor(fill_sys[i+1]);
    SFsemimerged_stat_2017[i]->SetMarkerColor(kBlack);
    SFsemimerged_stat_2017[i]->SetMarkerStyle(8);
    SFsemimerged_stat_2017[i]->SetMarkerSize(1);
    SFsemimerged_tot_2017[i]->Draw("E2 SAME");
    SFsemimerged_stat_2017[i]->Draw("E2 SAME");
    SFsemimerged_tot_2017[i]->Draw("PX SAME");

    SFsemimerged_stat_2016[i]->SetFillColor(fill_stat[i+2]);
    SFsemimerged_tot_2016[i]->SetFillColor(fill_sys[i+2]);
    SFsemimerged_stat_2016[i]->SetMarkerColor(kBlack);
    SFsemimerged_stat_2016[i]->SetMarkerStyle(8);
    SFsemimerged_stat_2016[i]->SetMarkerSize(1);
    SFsemimerged_tot_2016[i]->Draw("E2 SAME");
    SFsemimerged_stat_2016[i]->Draw("E2 SAME");
    SFsemimerged_tot_2016[i]->Draw("PX SAME");
  }
  for(unsigned int i=0; i<SFnotmerged_stat_2018.size(); i++){
    SFnotmerged_stat_2018[i]->SetFillColor(fill_stat[i]);
    SFnotmerged_tot_2018[i]->SetFillColor(fill_sys[i]);
    SFnotmerged_stat_2018[i]->SetMarkerColor(kBlack);
    SFnotmerged_stat_2018[i]->SetMarkerStyle(8);
    SFnotmerged_stat_2018[i]->SetMarkerSize(1);
    SFnotmerged_tot_2018[i]->Draw("E2 SAME");
    SFnotmerged_stat_2018[i]->Draw("E2 SAME");
    SFnotmerged_tot_2018[i]->Draw("PX SAME");

    SFnotmerged_stat_2017[i]->SetFillColor(fill_stat[i+1]);
    SFnotmerged_tot_2017[i]->SetFillColor(fill_sys[i+1]);
    SFnotmerged_stat_2017[i]->SetMarkerColor(kBlack);
    SFnotmerged_stat_2017[i]->SetMarkerStyle(8);
    SFnotmerged_stat_2017[i]->SetMarkerSize(1);
    SFnotmerged_tot_2017[i]->Draw("E2 SAME");
    SFnotmerged_stat_2017[i]->Draw("E2 SAME");
    SFnotmerged_tot_2017[i]->Draw("PX SAME");

    SFnotmerged_stat_2016[i]->SetFillColor(fill_stat[i+2]);
    SFnotmerged_tot_2016[i]->SetFillColor(fill_sys[i+2]);
    SFnotmerged_stat_2016[i]->SetMarkerColor(kBlack);
    SFnotmerged_stat_2016[i]->SetMarkerStyle(8);
    SFnotmerged_stat_2016[i]->SetMarkerSize(1);
    SFnotmerged_tot_2016[i]->Draw("E2 SAME");
    SFnotmerged_stat_2016[i]->Draw("E2 SAME");
    SFnotmerged_tot_2016[i]->Draw("PX SAME");
  }

  // Legend
  TLegend* leg = new TLegend(0.35, 0.73, 0.85, 0.88);
  leg->SetNColumns(3);
  leg->AddEntry(SFmerged_stat_2018[0], "#tau_{32}<0.56 (2018)", "f");
  leg->AddEntry(SFmerged_stat_2017[0], "#tau_{32}<0.56 (2017)", "f");
  leg->AddEntry(SFmerged_stat_2016[0], "#tau_{32}<0.56 (2016)", "f");
  leg->AddEntry((TObject*)0, "", "");
  leg->AddEntry((TObject*)0, "", "");
  leg->AddEntry((TObject*)0, "", "");

  leg->Draw();

  // jet label
  TString jettext = "HOTVR PUPPI";
  TLatex* jet_label = new TLatex(3.5, 24, jettext);
  jet_label->SetX(0.15);
  jet_label->SetY(0.907);
  jet_label->SetNDC();
  jet_label->SetTextAlign(11);
  jet_label->SetTextFont(42);
  jet_label->SetTextSize(0.035);
  jet_label->Draw();

  // lumi label
  TString lumitext = "(35.9 + 41.5 + 59.7) fb^{-1} (13 TeV)";
  TLatex* lumi_label = new TLatex(3.5, 24, lumitext);
  lumi_label->SetX(0.902);
  lumi_label->SetY(0.907);
  lumi_label->SetNDC();
  lumi_label->SetTextAlign(31);
  lumi_label->SetTextFont(42);
  lumi_label->SetTextSize(0.035);
  lumi_label->Draw();

  // custom X axis labels
  vector<double> xLabel = {0.23, 0.42, 0.61, 0.81};
  vector<double> yLabel = {0.148, 0.148, 0.148, 0.148};
  vector<TString> pttext;
  if(jet == "HOTVR"){
    xLabel = {0.21, 0.335, 0.46, 0.585, 0.71, 0.84};
    yLabel = {0.148, 0.148, 0.148, 0.148, 0.148, 0.148};
    pttext.push_back("200 < p_{T} < 250 GeV");
    pttext.push_back("250 < p_{T} < 300 GeV");
  }
  pttext.push_back("300 < p_{T} < 400 GeV");
  pttext.push_back("400 < p_{T} < 480 GeV");
  pttext.push_back("480 < p_{T} < 600 GeV");
  pttext.push_back("p_{T} > 600 GeV");

  vector<TLatex*> ptlabels;
  for(auto t: pttext) ptlabels.push_back(new TLatex(3.5, 24, t));

  for(int i=0; i<ptlabels.size(); i++){
    ptlabels[i]->SetX(xLabel[i]);
    ptlabels[i]->SetY(yLabel[i]);
    ptlabels[i]->SetNDC();
    ptlabels[i]->SetTextAlign(13);
    ptlabels[i]->SetTextFont(42);
    ptlabels[i]->SetTextSize(0.032);
    // if(jet == "HOTVR") ptlabels[i]->SetTextSize(0.022);
    ptlabels[i]->SetTextAngle(-25);
    ptlabels[i]->Draw();
  }

  // custom y axis labels
  double xsfLabel = 0.108;
  double ysfLabel = 0.166;

  vector<TString> sftext;
  sftext.push_back("0.5");
  sftext.push_back("1.0");
  sftext.push_back("1.5");

  for(unsigned int j=1; j<=3; j++){
    vector<TLatex*> sflabels;
    for(auto t: sftext) sflabels.push_back(new TLatex(3.5, 24, t));
    for(int i=0; i<sflabels.size(); i++){
      sflabels[i]->SetX(xsfLabel);
      sflabels[i]->SetY(ysfLabel);
      sflabels[i]->SetNDC();
      sflabels[i]->SetTextAlign(13);
      sflabels[i]->SetTextFont(42);
      sflabels[i]->SetTextSize(0.035);
      sflabels[i]->Draw();
      ysfLabel += 0.053;
    }
    ysfLabel += 0.072;
  }

  // labels for merged, semi merged, not merged
  vector<TString> toplabels = {"t#bar{t} merged Top", "t#bar{t} semimerged Top", "t#bar{t} not merged Top"};
  double ypos=0.72;
  for(auto label: toplabels){
    TLatex* text = new TLatex(3.5, 24, label);
    text->SetX(0.18);
    text->SetY(ypos);
    text->SetNDC();
    text->SetTextAlign(13);
    text->SetTextFont(42);
    text->SetTextSize(0.03);
    if(jet == "HOTVR"){
      text->SetX(0.165);
      text->SetTextSize(0.02);
      text->SetY(ypos-0.01);
    }
    text->Draw();
    ypos-=0.23;
  }

  // CMS label
  CMSLabel(true, 0.19, 0.87);

  gPad->RedrawAxis();

  c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_prongs/TopTaggingSF_HOTVR.pdf");
  delete c;


  return 0;
}

void AdjustXValues(vector<TGraphAsymmErrors*> graphs, int ptbins, double offset){
  int Nwps = graphs.size();
  double xposition = 0;
  if(Nwps == 1) xposition = 2; // skip fot HOTVR
  for(int ptbin=0; ptbin<ptbins; ptbin++){
    for(auto wp: graphs){
      xposition += 1;
      Double_t xval;
      Double_t yval;
      wp->GetPoint(ptbin, xval, yval);
      wp->SetPoint(ptbin, xposition+offset, yval);
      wp->SetPointEXlow(ptbin, 0.3);
      wp->SetPointEXhigh(ptbin, 0.3);
    }
    xposition += 1;
    if(Nwps == 4) xposition += 1; // skip one more for empty WP in CHS
    else if(Nwps == 1) xposition += 4; // skip fot HOTVR
  }
}

void AdjustYValues(vector<TGraphAsymmErrors*> graphs, double shift, int ptbins){
  for(int ptbin=0; ptbin<ptbins; ptbin++){
    for(auto wp: graphs){
      Double_t xval;
      Double_t yval;
      wp->GetPoint(ptbin, xval, yval);
      double newy = yval + shift;
      wp->SetPoint(ptbin, xval, newy);
    }
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
