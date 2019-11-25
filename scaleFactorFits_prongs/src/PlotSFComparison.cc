#include "../include/CentralInclude.h"

using namespace std;

void SetSameCentralValue(TGraphAsymmErrors* stat, TGraphAsymmErrors* tot);
void AdjustXValues(vector<TGraphAsymmErrors*> graphs);



int main(int argc, char* argv[]){
  TString plotdir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_FSR_f_prongs/";
  vector<TString> jetcols = {"PUPPI", "PUPPI_btag"};
  // vector<TString> jetcols = {"PUPPI", "PUPPI_btag", "HOTVR"};
  vector<TString> wps_PUPPI = {"wp1", "wp2", "wp3", "wp4", "wp5"};
  vector<TString> wps_PUPPI_btag = {"wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_HOTVR = {""};
  // vector<TString> procnames = {"TTbar_mergedTop_2018"};
  // vector<TString> procnames = {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018"};
  vector<TString> procnames = {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018", "SingleTop_mergedTop_2018", "SingleTop_semimerged_2018", "SingleTop_notmerged_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};
  // vector<TString> procnames = {"mergedTop_2018", "semimergedTop_2018", "notmergedTop_2018", "QCD_2018", "DYJets_2018", "WJets_2018"};
  // vector<TString> procnames = {"mergedTop_2018", "semimergedTop_2018", "Backgrounds_2018"};
  vector<TGraphAsymmErrors*> comp_stat, comp_tot;
  for(auto jet: jetcols){
    vector<TString> wps;
    if(jet == "PUPPI") wps = wps_PUPPI;
    else if(jet == "PUPPI_btag") wps = wps_PUPPI_btag;
    else if(jet == "HOTVR") wps = wps_HOTVR;
    for(auto pname: procnames){
      vector<TGraphAsymmErrors*> SFs_stat, SFs_tot;
      for(auto wp: wps){
        TString filename = plotdir+"eff_hists_PUPPI_"+wp+".root";
        if(jet == "HOTVR") filename = plotdir+"eff_hists_HOTVR_"+wp+".root";
        TFile *file = new TFile(filename);
        SFs_stat.push_back((TGraphAsymmErrors*) file->Get("sf_"+pname+"_stat"));
        SFs_tot.push_back((TGraphAsymmErrors*) file->Get("sf_"+pname+"_tot"));
      }
      for(unsigned int i=0; i<SFs_stat.size(); i++) SetSameCentralValue(SFs_stat[i], SFs_tot[i]);
      AdjustXValues(SFs_stat);
      AdjustXValues(SFs_tot);

      // now plot here for every jet collection  
      double x[] = {0, 22};
      double y[] = {0.5, 2.0};

      TGraph* dummy = new TGraph(2, x, y);
      SetupGlobalStyle();
      TCanvas *c = new TCanvas("c", "c", 900, 600);
      gPad->SetLeftMargin(0.15);
      dummy->Draw("AP");
      dummy->SetTitle(" ");
      dummy->GetXaxis()->SetTickLength(0.0);
      dummy->GetXaxis()->SetLabelSize(0.0);
      dummy->GetYaxis()->SetTitle("scale factor");
      dummy->GetYaxis()->SetTitleOffset(1.2);
      dummy->SetMarkerColor(kWhite);

      vector<TLine*> l_ptbin;
      l_ptbin.push_back(new TLine( 6, 0.35,  6, 2.15));
      l_ptbin.push_back(new TLine(12, 0.35, 12, 2.15));
      l_ptbin.push_back(new TLine(18, 0.35, 18, 2.15));
      for(auto l: l_ptbin){
        l->SetLineColor(kBlack);
        l->SetLineWidth(2);
        l->Draw("SAME");
      }

      TLine* line = new TLine(0, 1, 24, 1);
      line->SetLineStyle(7);
      line->SetLineColor(13);
      line->SetLineWidth(3);
      line->Draw("SAME");

      vector<int> fill_stat = {797, kRed, kBlue, kGreen, kMagenta};
      vector<int> fill_sys = {794, kRed-2, kBlue-2, kGreen-2, kMagenta-2};
      for(unsigned int i=0; i<SFs_stat.size(); i++){
        SFs_stat[i]->SetFillColor(fill_stat[i]);
        SFs_tot[i]->SetFillColor(fill_sys[i]);
        SFs_stat[i]->SetMarkerColor(kBlack);
        SFs_stat[i]->SetMarkerStyle(8);
        SFs_stat[i]->SetMarkerSize(1);
        SFs_tot[i]->Draw("E2 SAME");
        SFs_stat[i]->Draw("E2 SAME");
        SFs_tot[i]->Draw("PX SAME");
      }

      TLegend* leg = new TLegend(0.72, 0.6, 0.85, 0.85);
      for(unsigned int i=0; i<SFs_stat.size(); i++){
        TString workingpoint = "wp ";
        int j=i+1;
        workingpoint += j;
        leg->AddEntry(SFs_stat[i], workingpoint, "f");
      }
      leg->Draw();

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

      TString cat;
      if(pname == "TTbar_mergedTop_2018") cat = "t#bar{t} merged Top";
      else if(pname == "TTbar_semimerged_2018") cat = "t#bar{t} semi-merged";
      else if(pname == "TTbar_notmerged_2018") cat = "t#bar{t} not merged";
      else if(pname == "SingleTop_mergedTop") cat = "Single t merged Top";
      else if(pname == "SingleTop_semimerged_2018") cat = "Single t semi-merged";
      else if(pname == "SingleTop_notmerged_2018") cat = "Single t not merged";
      else if(pname == "QCD_2018") cat = "QCD";
      else if(pname == "DYJets_2018") cat = "Z+jets";
      else if(pname == "WJets_2018") cat = "W+jets";

      TLatex* category = new TLatex(3.5, 24, cat);
      category->SetX(0.725);
      category->SetY(0.58);
      category->SetNDC();
      category->SetTextAlign(13);
      category->SetTextFont(42);
      category->SetTextSize(0.03);
      category->Draw();

      gPad->RedrawAxis();
      c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_prongs/SF_Comparison_"+jet+"_"+pname+".pdf");
      delete c;
    }
  }
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
  // return graphs;
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
