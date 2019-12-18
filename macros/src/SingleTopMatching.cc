#include "../include/CentralInclude.h"


using namespace std;


int main(int argc, char* argv[]){

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------- declare files --------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------
  TFile* f_all = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.SingleTop_2018.root");
  TFile* f_mergedTop = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.SingleTop_mergedTop_2018.root");
  TFile* f_semimerged = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.SingleTop_semimerged_2018.root");
  TFile* f_notmerged = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.SingleTop_notmerged_2018.root");

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------

  TString histname = "ProbeJet_All_Pt400/mass_SD";
  TH1F* h_all = (TH1F*) f_all->Get(histname);
  TH1F* h_mergedTop = (TH1F*) f_mergedTop->Get(histname);
  TH1F* h_semimerged = (TH1F*) f_semimerged->Get(histname);
  TH1F* h_notmerged = (TH1F*) f_notmerged->Get(histname);


  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);

  TCanvas *a = new TCanvas("a", " ", 600, 600);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.1);
  h_all->SetTitle(" ");
  h_all->GetXaxis()->SetRangeUser(0, 300);
  h_all->GetYaxis()->SetTitleSize(0.06);
  h_all->GetXaxis()->SetTitleSize(0.05);
  h_all->GetZaxis()->SetTitleSize(0.05);
  h_all->GetXaxis()->SetTitleOffset(0.9);
  h_all->GetYaxis()->SetTitleOffset(1.1);
  h_all->GetZaxis()->SetTitleOffset(0.9);
  h_all->GetXaxis()->SetNdivisions(505);
  h_all->GetYaxis()->SetNdivisions(505);
  h_all->SetFillColor(15);
  h_all->SetLineColor(15);
  h_all->Draw("HIST");

  h_mergedTop->SetLineWidth(3);
  h_mergedTop->SetLineColor(kAzure+7);
  h_semimerged->SetLineWidth(3);
  h_semimerged->SetLineColor(kGreen-2);
  h_notmerged->SetLineWidth(3);
  h_notmerged->SetLineColor(kRed-2);

  h_notmerged->Draw("HIST SAME");
  h_semimerged->Draw("HIST SAME");
  h_mergedTop->Draw("HIST SAME");


  TLegend* leg = new TLegend(0.55, 0.65, 0.85, 0.85);
  leg->AddEntry(h_all, "all Single Top", "f");
  leg->AddEntry(h_mergedTop, "fully merged Single Top", "l");
  leg->AddEntry(h_semimerged, "semi-merged Single Top", "l");
  leg->AddEntry(h_notmerged, "not merged Single Top", "l");
  leg->SetBorderSize(0);
  leg->Draw();


  a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/Matching/SingleTop.pdf");


  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------
  return 0;
}
