#include "../include/CentralInclude.h"


using namespace std;

int main(int argc, char* argv[]){

  vector<TString> years = {"2016", "2017", "2018"};

  // vector<TString> jetcols = {"PUPPI"};
  vector<TString> jetcols = {"PUPPI", "HOTVR"};

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

      vector<TString> histnames = {
        "ProbeJet_pt300to400_wp2_all_pass",
        "ProbeJet_pt480to600_wp2_all_pass"
      };

      vector<TString> ptlabel = {
        "300 < #it{p}_{T} < 400 GeV",
        "480 < #it{p}_{T} < 600 GeV",
      };

      vector<Color_t> color = {798, kRed-2, kAzure+7};

      TString directory =  "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel/";
      if(jet == "HOTVR"){
        directory = "/nfs/dust/cms/user/schwarzd/TopTagging/PostSel_HOTVR/";

        histnames = {
          "ProbeJet_pt200to250_all_pass",
          "ProbeJet_pt300to400_all_pass",
          "ProbeJet_pt480to600_all_pass"
        };

        ptlabel = {
          "200 < #it{p}_{T} < 250 GeV",
          "300 < #it{p}_{T} < 400 GeV",
          "480 < #it{p}_{T} < 600 GeV",
        };
      }
      TFile* file = new TFile(directory+"uhh2.AnalysisModuleRunner.MC.TTbar_"+year_tag+".root");

      vector<TH1F*> h_tt_mass;
      for(auto histname: histnames){
        h_tt_mass.push_back((TH1F*) file->Get(histname+"/mass_sub"));
      }


      gStyle->SetOptStat(kFALSE);
      gStyle->SetPadTickY(1);
      gStyle->SetPadTickX(1);
      gStyle->SetLegendBorderSize(0);

      TCanvas *a = new TCanvas("a", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      double legy = 0.60;
      if(jet == "HOTVR") legy = 0.55;
      TLegend *leg = new TLegend(0.5, legy, 0.85, 0.85);
      for(unsigned int i=0; i<h_tt_mass.size(); i++){
        h_tt_mass[i]->Scale(1/h_tt_mass[i]->Integral());
        h_tt_mass[i]->Rebin(2);

        h_tt_mass[i]->SetTitle(" ");
        h_tt_mass[i]->GetXaxis()->SetRangeUser(0, 500);
        h_tt_mass[i]->GetYaxis()->SetRangeUser(0, 0.4);
        h_tt_mass[i]->GetYaxis()->SetTitleSize(0.06);
        h_tt_mass[i]->GetXaxis()->SetTitleSize(0.05);
        h_tt_mass[i]->GetXaxis()->SetTitleOffset(0.9);
        h_tt_mass[i]->GetYaxis()->SetTitleOffset(1.1);
        h_tt_mass[i]->GetXaxis()->SetNdivisions(505);
        h_tt_mass[i]->GetYaxis()->SetNdivisions(505);
        TString masstitle = "#it{m}_{SD} [GeV]";
        if(jet == "HOTVR") masstitle = "#it{m}_{jet} [GeV]";
        h_tt_mass[i]->GetXaxis()->SetTitle(masstitle);
        h_tt_mass[i]->GetYaxis()->SetTitle("#Delta N / N");
        h_tt_mass[i]->SetLineWidth(3);
        if(jet == "PUPPI") h_tt_mass[i]->SetLineColor(color[i+1]);
        else h_tt_mass[i]->SetLineColor(color[i]);

        leg->AddEntry(h_tt_mass[i], "t#bar{t} ("+ptlabel[i]+")", "l");
        h_tt_mass[i]->Draw("HIST SAME");
      }

      leg->Draw();

      TString t = "AK8 PUPPI";
      if(jet == "HOTVR") t = "HOTVR";
      TLatex* text = new TLatex(3.5, 24, t);
      text->SetX(0.20);
      text->SetY(0.85);
      text->SetNDC();
      text->SetTextAlign(13);
      text->SetTextFont(42);
      text->SetTextSize(0.03);
      text->Draw();
      a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/SimulationPlots/mjet_ptbins_"+year+"_"+jet+".pdf");


      delete a;


    }
  }





  return 0;
}
