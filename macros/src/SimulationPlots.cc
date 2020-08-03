#include "../include/CentralInclude.h"


using namespace std;

int main(int argc, char* argv[]){

  vector<TString> years = {"2016", "2017", "2018"};

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

  vector<TString> jetcols = {"PUPPI"};
  // vector<TString> jetcols = {"PUPPI", "HOTVR"};

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------

  for(auto year: years){
    TString year_tag = "2018";
    if(year == "2016") year_tag = "2016v3";
    else if(year == "2017") year_tag = "2017v2";

    for(auto jet: jetcols){
      cout << "-------------------------"<< endl;
      cout << year << endl;
      cout << jet << endl;

      TString directory =  "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel/";
      if(jet == "HOTVR") directory = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel_HOTVR/";

      TFile* file = new TFile(directory+"uhh2.AnalysisModuleRunner.MC.TTbar_"+year_tag+".root");
      TFile* file_mis = new TFile(directory+"uhh2.AnalysisModuleRunner.MC.QCD_"+year_tag+".root");

      TString histname = "EfficiencyHists_"+ptstring+"_all";
      TH1F* h_tt_mass = (TH1F*) file->Get(histname+"/mass");
      TH1F* h_tt_tau = (TH1F*) file->Get(histname+"/tau32");
      TH1F* h_qcd_mass = (TH1F*) file_mis->Get(histname+"/mass");
      TH1F* h_qcd_tau = (TH1F*) file_mis->Get(histname+"/tau32");


      h_tt_mass->Scale(1/h_tt_mass->Integral());
      h_tt_tau->Scale(1/h_tt_tau->Integral());
      h_qcd_mass->Scale(1/h_qcd_mass->Integral());
      h_qcd_tau->Scale(1/h_qcd_tau->Integral());
      h_tt_mass->Rebin(4);
      h_tt_tau->Rebin(4);
      h_qcd_mass->Rebin(4);
      h_qcd_tau->Rebin(4);

      gStyle->SetOptStat(kFALSE);
      gStyle->SetPadTickY(1);
      gStyle->SetPadTickX(1);
      gStyle->SetLegendBorderSize(0);

      TCanvas *a = new TCanvas("a", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      h_tt_mass->SetTitle(" ");
      h_tt_mass->GetXaxis()->SetRangeUser(0, 300);
      h_tt_mass->GetYaxis()->SetRangeUser(0, 0.3);
      h_tt_mass->GetYaxis()->SetTitleSize(0.06);
      h_tt_mass->GetXaxis()->SetTitleSize(0.05);
      h_tt_mass->GetXaxis()->SetTitleOffset(0.9);
      h_tt_mass->GetYaxis()->SetTitleOffset(1.1);
      h_tt_mass->GetXaxis()->SetNdivisions(505);
      h_tt_mass->GetYaxis()->SetNdivisions(505);
      TString masstitle = "m_{SD} [GeV]";
      if(jet == "HOTVR") masstitle = "m_{jet} [GeV]";
      h_tt_mass->GetXaxis()->SetTitle(masstitle);
      h_tt_mass->GetYaxis()->SetTitle("#Delta N / N");

      h_tt_mass->SetLineWidth(3);
      h_tt_mass->SetLineColor(810);
      h_qcd_mass->SetLineWidth(3);
      h_qcd_mass->SetLineColor(867);

      h_tt_mass->Draw("HIST");
      h_qcd_mass->Draw("HIST SAME");

      TLegend *leg = new TLegend(0.65, 0.75, 0.85, 0.85);
      leg->AddEntry(h_tt_mass, "Top", "l");
      leg->AddEntry(h_qcd_mass, "QCD", "l");
      leg->Draw();

      TString t = "AK8 PUPPI";
      if(jet == "HOTVR") t = "HOTVR";
      TLatex* text = new TLatex(3.5, 24, t);
      text->SetX(0.70);
      text->SetY(0.7);
      text->SetNDC();
      text->SetTextAlign(13);
      text->SetTextFont(42);
      text->SetTextSize(0.03);
      text->Draw();
      a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/SimulationPlots/mjet_"+jet+"_"+year+".pdf");

      TCanvas *b = new TCanvas("b", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      h_tt_tau->SetTitle(" ");
      h_tt_tau->GetXaxis()->SetRangeUser(0, 1);
      h_tt_tau->GetYaxis()->SetRangeUser(0, 0.3);
      h_tt_tau->GetYaxis()->SetTitleSize(0.06);
      h_tt_tau->GetXaxis()->SetTitleSize(0.05);
      h_tt_tau->GetXaxis()->SetTitleOffset(0.9);
      h_tt_tau->GetYaxis()->SetTitleOffset(1.1);
      h_tt_tau->GetXaxis()->SetNdivisions(505);
      h_tt_tau->GetYaxis()->SetNdivisions(505);
      h_tt_tau->GetXaxis()->SetTitle("#tau_{32}");
      h_tt_tau->GetYaxis()->SetTitle("#Delta N / N");

      h_tt_tau->SetLineWidth(3);
      h_tt_tau->SetLineColor(810);
      h_qcd_tau->SetLineWidth(3);
      h_qcd_tau->SetLineColor(867);

      h_tt_tau->Draw("HIST");
      h_qcd_tau->Draw("HIST SAME");

      leg->Draw();

      text->Draw();
      b->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/SimulationPlots/tau32_"+jet+"_"+year+".pdf");
      delete a;
      delete b;
      delete h_tt_mass;
      delete h_tt_tau;
      delete h_qcd_mass;
      delete h_qcd_tau;

    }
  }





  return 0;
}
