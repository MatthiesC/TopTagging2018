#include "../include/CentralInclude.h"

using namespace std;

void PlotComparison(vector<TH1F*> h_new, vector<TH1F*> h_old, TString plotname);
void IsCompatible(vector<TH1F*> h_new, vector<TH1F*> h_old, TString year, TString jet, TString wp, TString process);
TGraphAsymmErrors* ConvertToGraph(vector<TH1F*> hists);



int main(int argc, char* argv[]){

  vector<TString> jetcols = {"PUPPI", "HOTVR"};
  vector<TString> years = {"2016", "2017"};
  vector<TString> wps_PUPPI = {"wp1", "wp2", "wp3", "wp4", "wp5", "wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_CHS = {"wp2", "wp3", "wp4", "wp5", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_HOTVR = {""};
  vector<TString> procnames = {"mergedTop", "semimerged", "notmerged"};


  for(auto year: years){
    TString filename = year+"TopTaggingScaleFactors.root";
    TFile *file_new = new TFile(filename);
    TFile *file_old = new TFile("TorbenSF/"+filename);
    for(auto jet: jetcols){
      if(jet == "HOTVR" && year == "2017") continue;
      vector<TString> wps;
      if(jet == "PUPPI")      wps = wps_PUPPI;
      else if(jet == "HOTVR") wps = wps_HOTVR;
      else if(jet == "CHS")   wps = wps_CHS;
      for(auto wp: wps){
        for(auto process: procnames){
          vector<TH1F*> h_new, h_old;
          if(jet == "HOTVR"){
            h_new.push_back((TH1F*) file_new->Get(jet+"/sf_"+process+"_nominal"));
            h_new.push_back((TH1F*) file_new->Get(jet+"/sf_"+process+"_up"));
            h_new.push_back((TH1F*) file_new->Get(jet+"/sf_"+process+"_down"));
            h_old.push_back((TH1F*) file_old->Get(jet+"/sf_"+process+"_nominal"));
            h_old.push_back((TH1F*) file_old->Get(jet+"/sf_"+process+"_up"));
            h_old.push_back((TH1F*) file_old->Get(jet+"/sf_"+process+"_down"));
          }
          else{
            h_new.push_back((TH1F*) file_new->Get(jet+"_"+wp+"/sf_"+process+"_nominal"));
            h_new.push_back((TH1F*) file_new->Get(jet+"_"+wp+"/sf_"+process+"_up"));
            h_new.push_back((TH1F*) file_new->Get(jet+"_"+wp+"/sf_"+process+"_down"));
            h_old.push_back((TH1F*) file_old->Get(jet+"_"+wp+"/sf_"+process+"_nominal"));
            h_old.push_back((TH1F*) file_old->Get(jet+"_"+wp+"/sf_"+process+"_up"));
            h_old.push_back((TH1F*) file_old->Get(jet+"_"+wp+"/sf_"+process+"_down"));
          }
          TString plotname = year+"_"+jet+"_"+wp+"_"+process;
          PlotComparison(h_new, h_old, plotname);
          IsCompatible(h_new, h_old, year, jet, wp, process);
        }
      }
    }
  }
}

void PlotComparison(vector<TH1F*> h_new, vector<TH1F*> h_old, TString plotname){

  TGraphAsymmErrors* SF_new = ConvertToGraph(h_new);
  TGraphAsymmErrors* SF_old = ConvertToGraph(h_old);

  SetupGlobalStyle();
  TCanvas *c = new TCanvas("c", "c", 600, 600);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.12);
  SF_new->Draw("AP");
  SF_new->SetTitle(" ");
  SF_new->GetXaxis()->SetTitle("Probe jet #it{p}_{T} [GeV]");
  SF_new->GetYaxis()->SetTitle("Scale factor");
  SF_new->GetXaxis()->SetTitleOffset(1.1);
  SF_new->GetYaxis()->SetTitleOffset(1.2);
  SF_new->GetYaxis()->SetTitleSize(0.06);
  SF_new->GetXaxis()->SetTitleSize(0.05);
  double xmin;
  if(plotname.Contains("HOTVR")) xmin = 200;
  else xmin = 300;
  SF_new->GetXaxis()->SetRangeUser(xmin,1500.);
  SF_new->GetYaxis()->SetRangeUser(0., 4.);

  SF_old->SetMarkerColor(1);
  SF_old->SetLineColor(16);
  SF_old->SetFillColor(16);
  SF_old->SetMarkerStyle(8);
  SF_old->SetMarkerSize(1);
  SF_old->Draw("P E2 SAME");
  SF_new->SetMarkerColor(kRed);
  SF_new->SetLineColor(kRed);
  SF_new->SetMarkerStyle(8);
  SF_new->SetMarkerSize(1);
  SF_new->Draw("E1 P SAME");
  TLegend *leg = new TLegend(0.2, 0.65, 0.75, 0.85);
  leg->AddEntry(SF_new, "This analysis", "pe");
  leg->AddEntry(SF_old, "JINST 15 (2020), no. 6, P06005", "pf");
  TString leglabel = "";
  if(plotname.Contains("mergedTop")) leglabel = "t#bar{t} merged top";
  if(plotname.Contains("semimerged")) leglabel = "t#bar{t} semimerged top";
  if(plotname.Contains("notmerged")) leglabel = "t#bar{t} unmerged top";
  leg->AddEntry((TObject*)0, leglabel, "");
  leg->Draw();
  gPad->RedrawAxis();

  TString jettext = "AK8 PUPPI";
  if(plotname.Contains("HOTVR")) jettext = "HOTVR PUPPI";
  TLatex* jet_label = new TLatex(3.5, 24, jettext);
  jet_label->SetX(0.15);
  jet_label->SetY(0.915);
  jet_label->SetNDC();
  jet_label->SetTextAlign(11);
  jet_label->SetTextFont(42);
  jet_label->SetTextSize(0.045);
  jet_label->Draw();

  c->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/CompareToTorben/"+plotname+".pdf");
  delete c;

}


TGraphAsymmErrors* ConvertToGraph(vector<TH1F*> hists){
  const int Nbins = hists[0]->GetSize()-2;
  vector<double> x, y,exl,exh,eyl,eyh;
  for(int bin=1; bin<=Nbins;bin++){
    double pt = hists[0]->GetXaxis()->GetBinCenter(bin);
    double sf = hists[0]->GetBinContent(bin);
    if(bin != Nbins){
      x.push_back(pt);
      exl.push_back(hists[0]->GetXaxis()->GetBinWidth(bin)/2);
      exh.push_back(hists[0]->GetXaxis()->GetBinWidth(bin)/2);
    }
    else{
      x.push_back(1300);
      exl.push_back(200);
      exh.push_back(200);
    }
    y.push_back(sf);
    eyl.push_back(fabs(hists[1]->GetBinContent(bin)-sf));
    eyh.push_back(fabs(hists[2]->GetBinContent(bin)-sf));
  }
  TGraphAsymmErrors* graph = new TGraphAsymmErrors(Nbins, &x[0], &y[0], &exl[0], &exh[0], &eyl[0], &eyh[0]);
  return graph;
}


void IsCompatible(vector<TH1F*> h_new, vector<TH1F*> h_old, TString year, TString jet, TString wp, TString process){
  if(jet == "HOTVR") return; // comparing HOTVR is complicated because of additional bins
  const int Nbins = h_new[0]->GetSize()-2;
  for(int bin=1; bin<=Nbins;bin++){
    double sf_new = h_new[0]->GetBinContent(bin);
    double sf_old = h_old[0]->GetBinContent(bin);
    double up_new = fabs(h_new[1]->GetBinContent(bin) - sf_new);
    double up_old = fabs(h_old[1]->GetBinContent(bin) - sf_old);
    double down_new = fabs(h_new[2]->GetBinContent(bin) - sf_new);
    double down_old = fabs(h_old[2]->GetBinContent(bin) - sf_old);
    // if(fabs(up_new-down_new) > 0.0001){
    //   cout << "up/down variations of new SF not the same!" << endl;
    // }
    // if(fabs(up_old-down_old) > 0.0001){
    //   cout << "up/down variations of old SF not the same: " << endl;
    //   cout << year << ", " << jet << ", " << wp << ", " << bin << ", " << process << endl;
    //   cout << up_old << " != " << down_old << endl;
    //   cout << h_old[0]->GetBinContent(bin) << endl;
    //   cout << h_old[1]->GetBinContent(bin) << endl;
    //   cout << h_old[2]->GetBinContent(bin) << endl;
    // }

    double diff = sf_new-sf_old;
    bool compatible = false;
    if(diff>0){
      if(fabs(diff) < sqrt(down_new*down_new + up_old*up_old)) compatible = true;
    }
    else if(diff<0){
      if(fabs(diff) < sqrt(up_new*up_new + down_old*down_old)) compatible = true;
    }
    if(!compatible && process == "mergedTop"){
      cout << "==============================================================="<< endl;
      cout << "!! NOT COMPATIBLE !!" << endl;
      cout << "  -    year: " << year << endl;
      cout << "  -     jet: " << jet << endl;
      cout << "  -      wp: " << wp << endl;
      cout << "  -   ptbin: " << bin << endl;
      cout << "  - process: " << process << endl;
      cout << "==============================================================="<< endl;
    }
  }



}
