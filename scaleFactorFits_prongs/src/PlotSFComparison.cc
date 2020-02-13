#include "../include/CentralInclude.h"

using namespace std;

void SetSameCentralValue(TGraphAsymmErrors* stat, TGraphAsymmErrors* tot);
void AdjustXValues(vector<TGraphAsymmErrors*> graphs, int ptbins);
void AdjustYValues(vector<TGraphAsymmErrors*> graphs, double shift, int ptbins);
void WriteToFile(vector<TGraphAsymmErrors*> stat, vector<TGraphAsymmErrors*> tot, TString name);
ofstream outfile;
TString year = "2018";

int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  outfile.open("SF.txt");
  // TFile* outroot = new TFile(year+"TopTaggingScaleFactors.root", "RECREATE");
  // TFile* outroot_nomass = new TFile(year+"TopTaggingScaleFactors_NoMassCut.root", "RECREATE");

  TString plotdir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_FSR_f_prongs/"+year+"/";
  // vector<TString> jetcols = {"HOTVR"};
  vector<TString> jetcols = {"PUPPI", "PUPPI_btag", "CHS", "CHS_btag", "HOTVR"};
  vector<TString> wps_PUPPI = {"wp1", "wp2", "wp3", "wp4", "wp5"};
  vector<TString> wps_PUPPI_btag = {"wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_CHS = {"wp2", "wp3", "wp4", "wp5"};
  vector<TString> wps_CHS_btag = {"wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_HOTVR = {""};
  vector<TString> procnames = {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018"};
  for(auto jet: jetcols){
    vector<TString> wps;
    if(jet == "PUPPI") wps = wps_PUPPI;
    else if(jet == "PUPPI_btag") wps = wps_PUPPI_btag;
    else if(jet == "CHS") wps = wps_CHS;
    else if(jet == "CHS_btag") wps = wps_CHS_btag;
    else if(jet == "HOTVR") wps = wps_HOTVR;
    vector<TGraphAsymmErrors*> SFmerged_stat, SFmerged_tot;
    vector<TGraphAsymmErrors*> SFsemimerged_stat, SFsemimerged_tot;
    vector<TGraphAsymmErrors*> SFnotmerged_stat, SFnotmerged_tot;
    for(auto wp: wps){
      TString filename;
      if(jet.Contains("PUPPI")) filename = plotdir+"eff_hists_PUPPI_"+wp+".root";
      else if (jet.Contains("CHS")) filename = plotdir+"eff_hists_CHS_"+wp+".root";
      else if (jet.Contains("HOTVR")) filename = plotdir+"eff_hists_HOTVR_"+wp+".root";
      TFile *file = new TFile(filename);

      if(year == "2016"){
        procnames[0].ReplaceAll("2018", "2016v3");
        procnames[1].ReplaceAll("2018", "2016v3");
        procnames[2].ReplaceAll("2018", "2016v3");
      }
      else if(year == "2017"){
        procnames[0].ReplaceAll("2018", "2017v2");
        procnames[1].ReplaceAll("2018", "2017v2");
        procnames[2].ReplaceAll("2018", "2017v2");
      }

      SFmerged_stat.push_back((TGraphAsymmErrors*) file->Get("sf_"+procnames[0]+"_stat"));
      SFmerged_tot.push_back((TGraphAsymmErrors*) file->Get("sf_"+procnames[0]+"_tot"));
      SFsemimerged_stat.push_back((TGraphAsymmErrors*) file->Get("sf_"+procnames[1]+"_stat"));
      SFsemimerged_tot.push_back((TGraphAsymmErrors*) file->Get("sf_"+procnames[1]+"_tot"));
      SFnotmerged_stat.push_back((TGraphAsymmErrors*) file->Get("sf_"+procnames[2]+"_stat"));
      SFnotmerged_tot.push_back((TGraphAsymmErrors*) file->Get("sf_"+procnames[2]+"_tot"));
    }
    for(unsigned int i=0; i<SFmerged_stat.size(); i++) SetSameCentralValue(SFmerged_stat[i], SFmerged_tot[i]);
    for(unsigned int i=0; i<SFsemimerged_stat.size(); i++) SetSameCentralValue(SFsemimerged_stat[i], SFsemimerged_tot[i]);
    for(unsigned int i=0; i<SFnotmerged_stat.size(); i++) SetSameCentralValue(SFnotmerged_stat[i], SFnotmerged_tot[i]);
    int ptbins = 4;
    if(jet == "HOTVR") ptbins = 6;
    AdjustXValues(SFmerged_stat, ptbins);
    AdjustXValues(SFmerged_tot, ptbins);
    AdjustXValues(SFsemimerged_stat, ptbins);
    AdjustXValues(SFsemimerged_tot, ptbins);
    AdjustXValues(SFnotmerged_stat, ptbins);
    AdjustXValues(SFnotmerged_tot, ptbins);
    // TString btagflag = "";
    // if(jet=="PUPPI_btag" || jet == "CHS_btag") btagflag = " (subjet b tag)";
    // WriteToFile(SFmerged_stat, SFmerged_tot, "merged"+btagflag);
    // WriteToFile(SFsemimerged_stat, SFsemimerged_tot, "semi merged"+btagflag);
    // WriteToFile(SFnotmerged_stat, SFnotmerged_tot, "not merged"+btagflag);
    AdjustYValues(SFsemimerged_stat, -2, ptbins);
    AdjustYValues(SFsemimerged_tot, -2, ptbins);
    AdjustYValues(SFnotmerged_stat, -4, ptbins);
    AdjustYValues(SFnotmerged_tot, -4, ptbins);


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
    for(unsigned int i=0; i<SFmerged_stat.size(); i++){
      SFmerged_stat[i]->SetFillColor(fill_stat[i]);
      SFmerged_tot[i]->SetFillColor(fill_sys[i]);
      SFmerged_stat[i]->SetMarkerColor(kBlack);
      SFmerged_stat[i]->SetMarkerStyle(8);
      SFmerged_stat[i]->SetMarkerSize(1);
      SFmerged_tot[i]->Draw("E2 SAME");
      SFmerged_stat[i]->Draw("E2 SAME");
      SFmerged_tot[i]->Draw("PX SAME");
    }
    for(unsigned int i=0; i<SFsemimerged_stat.size(); i++){
      SFsemimerged_stat[i]->SetFillColor(fill_stat[i]);
      SFsemimerged_tot[i]->SetFillColor(fill_sys[i]);
      SFsemimerged_stat[i]->SetMarkerColor(kBlack);
      SFsemimerged_stat[i]->SetMarkerStyle(8);
      SFsemimerged_stat[i]->SetMarkerSize(1);
      SFsemimerged_tot[i]->Draw("E2 SAME");
      SFsemimerged_stat[i]->Draw("E2 SAME");
      SFsemimerged_tot[i]->Draw("PX SAME");
    }
    for(unsigned int i=0; i<SFnotmerged_stat.size(); i++){
      SFnotmerged_stat[i]->SetFillColor(fill_stat[i]);
      SFnotmerged_tot[i]->SetFillColor(fill_sys[i]);
      SFnotmerged_stat[i]->SetMarkerColor(kBlack);
      SFnotmerged_stat[i]->SetMarkerStyle(8);
      SFnotmerged_stat[i]->SetMarkerSize(1);
      SFnotmerged_tot[i]->Draw("E2 SAME");
      SFnotmerged_stat[i]->Draw("E2 SAME");
      SFnotmerged_tot[i]->Draw("PX SAME");
    }

    // Legend
    TLegend* leg = new TLegend(0.35, 0.73, 0.85, 0.88);
    leg->SetNColumns(3);
    vector<TString> tauvals = {"0.40", "0.46", "0.54", "0.65", "0.80"};
    if(jet.Contains("CHS")){
      tauvals = {"0.5", "0.57", "0.67", "0.81"};
      leg->AddEntry((TObject*)0, "", "");
    }
    else if(jet.Contains("HOTVR")){
      tauvals = {"0.56"};
    }
    for(unsigned int i=0; i<SFmerged_stat.size(); i++){
      TString workingpoint = "#tau_{32} < ";
      workingpoint += tauvals[i];
      leg->AddEntry(SFmerged_stat[i], workingpoint, "f");
    }
    if(jet.Contains("HOTVR")){
      leg->AddEntry((TObject*)0, "", "");
      leg->AddEntry((TObject*)0, "", "");
      leg->AddEntry((TObject*)0, "", "");
      leg->AddEntry((TObject*)0, "", "");
    }
    leg->Draw();

    // subjet b tagging label
    if(jet == "PUPPI_btag" || jet == "CHS_btag" ){
      TLatex* btag_label = new TLatex(3.5, 24, "+ subjet b tagging");
      btag_label->SetX(0.7);
      btag_label->SetY(0.775);
      btag_label->SetNDC();
      btag_label->SetTextAlign(13);
      btag_label->SetTextFont(42);
      btag_label->SetTextSize(0.035);
      btag_label->Draw();
    }

    // jet label
    TString jettext = "AK8 PUPPI";
    if(jet.Contains("CHS")) jettext = "AK8 CHS";
    else if(jet.Contains("HOTVR")) jettext = "HOTVR";
    TLatex* jet_label = new TLatex(3.5, 24, jettext);
    jet_label->SetX(0.15);
    jet_label->SetY(0.907);
    jet_label->SetNDC();
    jet_label->SetTextAlign(11);
    jet_label->SetTextFont(42);
    jet_label->SetTextSize(0.035);
    jet_label->Draw();

    // lumi label
    TString lumitext = "59.7 fb^{-1} (13 TeV)";
    if(year == "2017") lumitext = "41.5 fb^{-1} (13 TeV)";
    else if(year == "2016") lumitext = "35.9 fb^{-1} (13 TeV)";
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
    TString pdfname;
    if(jet.Contains("PUPPI")) pdfname = year+"_TopTaggingSF_AK8_PUPPI";
    else if(jet.Contains("CHS")) pdfname = year+"_TopTaggingSF_AK8_CHS";
    else if(jet.Contains("HOTVR")) pdfname = year+"_TopTaggingSF_HOTVR";
    if(jet=="PUPPI_btag" || jet=="CHS_btag") pdfname += "_subjet_btag";
    c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_prongs/"+year+"/"+pdfname+".pdf");
    delete c;

  }
  outfile.close();
  return 0;
}

void AdjustXValues(vector<TGraphAsymmErrors*> graphs, int ptbins){
  int Nwps = graphs.size();
  double xposition = 0;
  if(Nwps == 1) xposition = 2; // skip fot HOTVR
  for(int ptbin=0; ptbin<ptbins; ptbin++){
    for(auto wp: graphs){
      xposition += 1;
      Double_t xval;
      Double_t yval;
      wp->GetPoint(ptbin, xval, yval);
      wp->SetPoint(ptbin, xposition, yval);
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

void WriteToFile(vector<TGraphAsymmErrors*> stat, vector<TGraphAsymmErrors*> tot, TString name){
  cout << "write " << name << "..." << endl;
  vector<TString> ptbins = {"300 < p_{T} < 400", "400 < p_{T} < 480", "480 < p_{T} < 600", "p_{T} > 600"};
  for(int wp=1; wp<= stat.size(); wp++){
    outfile << name << " WP" << wp << endl;
    const int Npoints = tot[wp-1]->GetN();
    Double_t* y = tot[wp-1]->GetY();
    for(int i=0; i<Npoints; i++){
      double val = y[i];
      double stat_lo = stat[wp-1]->GetErrorYlow(i);
      double stat_hi = stat[wp-1]->GetErrorYhigh(i);
      double tot_lo = tot[wp-1]->GetErrorYlow(i);
      double tot_hi = tot[wp-1]->GetErrorYhigh(i);
      if(stat_hi != stat_lo) cout << "stat uncertainty is assumed to be symmetric but is not!" << endl;
      if(tot_hi != tot_lo) cout << "tot uncertainty is assumed to be symmetric but is not!" << endl;
      outfile << "   " << ptbins[i] << "     ";
      if(i == Npoints-1) outfile << "      ";
      outfile << setprecision(4);
      outfile << val << " +-" << stat_lo << " (stat)";
      outfile << " +-" << tot_lo << " (tot)";
      outfile << endl;
    }
  }
  return;
}
