#include "../include/CentralInclude.h"

using namespace std;

void SetSameCentralValue(TGraphAsymmErrors* stat, TGraphAsymmErrors* tot);
void AdjustXValues(vector<TGraphAsymmErrors*> graphs);
void AdjustYValues(vector<TGraphAsymmErrors*> graphs, double shift);
void WriteToFile(vector<TGraphAsymmErrors*> stat, vector<TGraphAsymmErrors*> tot, TString name);
ofstream outfile;
TString year = "2018";

int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];

  outfile.open("SF.txt");
  TFile* outroot = new TFile(year+"TopTaggingScaleFactors.root", "RECREATE");
  TFile* outroot_nomass = new TFile(year+"TopTaggingScaleFactors_NoMassCut.root", "RECREATE");

  TString plotdir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_FSR_f_prongs/"+year+"/";
  vector<TString> jetcols = {"PUPPI", "PUPPI_btag"};
  vector<TString> wps_PUPPI = {"wp1", "wp2", "wp3", "wp4", "wp5"};
  vector<TString> wps_PUPPI_btag = {"wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> procnames = {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018"};
  for(auto jet: jetcols){
    vector<TString> wps;
    if(jet == "PUPPI") wps = wps_PUPPI;
    else if(jet == "PUPPI_btag") wps = wps_PUPPI_btag;
    vector<TGraphAsymmErrors*> SFmerged_stat, SFmerged_tot;
    vector<TGraphAsymmErrors*> SFsemimerged_stat, SFsemimerged_tot;
    vector<TGraphAsymmErrors*> SFnotmerged_stat, SFnotmerged_tot;
    for(auto wp: wps){
      TString filename = plotdir+"eff_hists_PUPPI_"+wp+".root";
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
    AdjustXValues(SFmerged_stat);
    AdjustXValues(SFmerged_tot);
    AdjustXValues(SFsemimerged_stat);
    AdjustXValues(SFsemimerged_tot);
    AdjustXValues(SFnotmerged_stat);
    AdjustXValues(SFnotmerged_tot);
    TString btagflag = "";
    if(jet=="PUPPI_btag") btagflag = " (subjet b tag)";
    WriteToFile(SFmerged_stat, SFmerged_tot, "merged"+btagflag);
    WriteToFile(SFsemimerged_stat, SFsemimerged_tot, "semi merged"+btagflag);
    WriteToFile(SFnotmerged_stat, SFnotmerged_tot, "not merged"+btagflag);
    AdjustYValues(SFsemimerged_stat, -2);
    AdjustYValues(SFsemimerged_tot, -2);
    AdjustYValues(SFnotmerged_stat, -4);
    AdjustYValues(SFnotmerged_tot, -4);


    // now plot here for every jet collection
    double x[] = {0, 22};
    double y[] = {-2.9, 2.5};

    TGraph* dummy = new TGraph(2, x, y);
    SetupGlobalStyle();
    TCanvas *c = new TCanvas("c", "c", 900, 600);
    gPad->SetLeftMargin(0.15);
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
    l_ptbin.push_back(new TLine( 6, ymin,  6, ymax));
    l_ptbin.push_back(new TLine(12, ymin, 12, ymax));
    l_ptbin.push_back(new TLine(18, ymin, 18, ymax));
    for(auto l: l_ptbin){
      l->SetLineColor(kBlack);
      l->SetLineWidth(2);
      l->Draw("SAME");
    }

    // plot horizontal lines
    vector<TLine*> lines;
    lines.push_back(new TLine(0, 1, 24, 1));
    lines.push_back(new TLine(0, -1, 24, -1));
    lines.push_back(new TLine(0, -3, 24, -3));
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
    for(unsigned int i=0; i<SFmerged_stat.size(); i++){
      TString workingpoint = "#tau_{32} < ";
      workingpoint += tauvals[i];
      leg->AddEntry(SFmerged_stat[i], workingpoint, "f");
    }
    // if(jet == "PUPPI_btag")leg->AddEntry((TObject*)0, "+ subjet b tagging", "");
    leg->Draw();

    // subjet b tagging label
    if(jet == "PUPPI_btag"){
      TLatex* btag_label = new TLatex(3.5, 24, "+ subjet b tagging");
      btag_label->SetX(0.7);
      btag_label->SetY(0.775);
      btag_label->SetNDC();
      btag_label->SetTextAlign(13);
      btag_label->SetTextFont(42);
      btag_label->SetTextSize(0.035);
      btag_label->Draw();
    }

    // custom X axis labels
    vector<double> xLabel = {0.16, 0.35, 0.54, 0.75};
    vector<double> yLabel = {0.09, 0.09, 0.09, 0.09};
    vector<TString> pttext;
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
      ptlabels[i]->SetTextSize(0.03);
      ptlabels[i]->Draw();
    }

    // custom y axis labels
    double xsfLabel = 0.108;
    double ysfLabel = 0.11;

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
        ysfLabel += 0.06;
      }
      ysfLabel += 0.065;
    }

    // labels for merged, semi merged, not merged
    vector<TString> toplabels = {"t#bar{t} merged Top", "t#bar{t} semimerged Top", "t#bar{t} not merged Top"};
    double ypos=0.715;
    for(auto label: toplabels){
      TLatex* text = new TLatex(3.5, 24, label);
      text->SetX(0.18);
      text->SetY(ypos);
      text->SetNDC();
      text->SetTextAlign(13);
      text->SetTextFont(42);
      text->SetTextSize(0.03);
      text->Draw();
      ypos-=0.245;
    }

    // CMS label
    CMSLabel(true, 0.19, 0.87);

    gPad->RedrawAxis();
    TString pdfname = "2018_TopTaggingSF_AK8_PUPPI";
    if(jet=="PUPPI_btag") pdfname += "_subjet_btag";
    c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_prongs/"+year+"/"+pdfname+".pdf");
    delete c;

  }
  outfile.close();
  return 0;
}

void AdjustXValues(vector<TGraphAsymmErrors*> graphs){
  double xposition = 0;
  for(int ptbin=0; ptbin<4; ptbin++){
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
  }
}

void AdjustYValues(vector<TGraphAsymmErrors*> graphs, double shift){
  for(int ptbin=0; ptbin<4; ptbin++){
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
