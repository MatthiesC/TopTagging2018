#include "../include/CentralInclude.h"


using namespace std;


int main(int argc, char* argv[]){

  vector<TString> systematic = {"central", "TagEffi_3prong_up", "TagEffi_3prong_down", "TagEffi_2prong_up", "TagEffi_2prong_down", "TagEffi_1prong_up", "TagEffi_1prong_down"};
  vector<TString> passfail = {"pass", "fail"};
  vector<TString> mergedSelections = {"mergedTop", "semimerged", "notmerged"};

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------

  vector<vector<vector<TH1F*>>> h_tt, h_st;


  for(unsigned int i=0; i<systematic.size(); i++){
    vector<vector<TH1F*>> dummy;
    h_tt.push_back(dummy);
    h_st.push_back(dummy);
    for(unsigned int j=0; j<mergedSelections.size(); j++){
      vector<TH1F*> dummy2;
      h_tt[i].push_back(dummy2);
      h_st[i].push_back(dummy2);
      for(unsigned int k=0; k<passfail.size(); k++){
        TString location;
        if(systematic[i] == "central") location = dir;
        else location = dir+systematic[i]+"/";
        TFile* f_tt = new TFile(location+"uhh2.AnalysisModuleRunner.MC.TTbar"+"_"+mergedSelections[j]+"_2018.root");
        TFile* f_st = new TFile(location+"uhh2.AnalysisModuleRunner.MC.SingleTop"+"_"+mergedSelections[j]+"_2018.root");
        TH1F* tt = (TH1F*) f_tt->Get("ProbeJet_pt400to480_wp3_all_"+passfail[k]+"/mass_SD");
        TH1F* st = (TH1F*) f_st->Get("ProbeJet_pt400to480_wp3_all_"+passfail[k]+"/mass_SD");
        h_tt[i][j].push_back(tt);
        h_st[i][j].push_back(st);
      }
    }
  }


  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);


  for(unsigned int j=0; j<mergedSelections.size(); j++){
    for(unsigned int k=0; k<passfail.size(); k++){
      TCanvas *a = new TCanvas("a", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      h_tt[0][j][k]->SetTitle(" ");
      h_tt[0][j][k]->GetXaxis()->SetRangeUser(0, 300);
      h_tt[0][j][k]->GetYaxis()->SetRangeUser(0, h_tt[0][j][k]->GetMaximum()*1.5);
      h_tt[0][j][k]->GetYaxis()->SetTitleSize(0.06);
      h_tt[0][j][k]->GetXaxis()->SetTitleSize(0.05);
      h_tt[0][j][k]->GetZaxis()->SetTitleSize(0.05);
      h_tt[0][j][k]->GetXaxis()->SetTitleOffset(0.9);
      h_tt[0][j][k]->GetYaxis()->SetTitleOffset(1.1);
      h_tt[0][j][k]->GetZaxis()->SetTitleOffset(0.9);
      h_tt[0][j][k]->GetXaxis()->SetNdivisions(505);
      h_tt[0][j][k]->GetYaxis()->SetNdivisions(505);
      h_tt[0][j][k]->SetLineWidth(3);
      h_tt[0][j][k]->SetLineColor(13);
      h_tt[0][j][k]->Draw("HIST");

      h_tt[1][j][k]->SetLineWidth(3);
      h_tt[1][j][k]->SetLineColor(kAzure+7);
      h_tt[2][j][k]->SetLineWidth(3);
      h_tt[2][j][k]->SetLineColor(kRed-2);

      h_tt[1][j][k]->Draw("HIST SAME");
      h_tt[2][j][k]->Draw("HIST SAME");

      TLegend* leg = new TLegend(0.22, 0.65, 0.52, 0.85);
      leg->AddEntry(h_tt[0][j][k], "central", "l");
      leg->AddEntry(h_tt[1][j][k], "up", "l");
      leg->AddEntry(h_tt[2][j][k], "down", "l");
      leg->SetBorderSize(0);
      leg->Draw();

      a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/EffiVariations/TTbar_3prong_"+mergedSelections[j]+"_"+passfail[k]+".pdf");
      delete a;
    }
  }

  for(unsigned int j=0; j<mergedSelections.size(); j++){
    for(unsigned int k=0; k<passfail.size(); k++){
      TCanvas *a = new TCanvas("a", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      h_tt[0][j][k]->SetTitle(" ");
      h_tt[0][j][k]->GetXaxis()->SetRangeUser(0, 300);
      h_tt[0][j][k]->GetYaxis()->SetRangeUser(0, h_tt[0][j][k]->GetMaximum()*1.5);
      h_tt[0][j][k]->GetYaxis()->SetTitleSize(0.06);
      h_tt[0][j][k]->GetXaxis()->SetTitleSize(0.05);
      h_tt[0][j][k]->GetZaxis()->SetTitleSize(0.05);
      h_tt[0][j][k]->GetXaxis()->SetTitleOffset(0.9);
      h_tt[0][j][k]->GetYaxis()->SetTitleOffset(1.1);
      h_tt[0][j][k]->GetZaxis()->SetTitleOffset(0.9);
      h_tt[0][j][k]->GetXaxis()->SetNdivisions(505);
      h_tt[0][j][k]->GetYaxis()->SetNdivisions(505);
      h_tt[0][j][k]->SetLineWidth(3);
      h_tt[0][j][k]->SetLineColor(13);
      h_tt[0][j][k]->Draw("HIST");

      h_tt[3][j][k]->SetLineWidth(3);
      h_tt[3][j][k]->SetLineColor(kAzure+7);
      h_tt[4][j][k]->SetLineWidth(3);
      h_tt[4][j][k]->SetLineColor(kRed-2);

      h_tt[3][j][k]->Draw("HIST SAME");
      h_tt[4][j][k]->Draw("HIST SAME");

      TLegend* leg = new TLegend(0.22, 0.65, 0.52, 0.85);
      leg->AddEntry(h_tt[0][j][k], "central", "l");
      leg->AddEntry(h_tt[3][j][k], "up", "l");
      leg->AddEntry(h_tt[4][j][k], "down", "l");
      leg->SetBorderSize(0);
      leg->Draw();

      a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/EffiVariations/TTbar_2prong_"+mergedSelections[j]+"_"+passfail[k]+".pdf");
      delete a;
    }
  }

  for(unsigned int j=0; j<mergedSelections.size(); j++){
    for(unsigned int k=0; k<passfail.size(); k++){
      TCanvas *a = new TCanvas("a", " ", 600, 600);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.1);
      h_tt[0][j][k]->SetTitle(" ");
      h_tt[0][j][k]->GetXaxis()->SetRangeUser(0, 300);
      h_tt[0][j][k]->GetYaxis()->SetRangeUser(0, h_tt[0][j][k]->GetMaximum()*1.5);
      h_tt[0][j][k]->GetYaxis()->SetTitleSize(0.06);
      h_tt[0][j][k]->GetXaxis()->SetTitleSize(0.05);
      h_tt[0][j][k]->GetZaxis()->SetTitleSize(0.05);
      h_tt[0][j][k]->GetXaxis()->SetTitleOffset(0.9);
      h_tt[0][j][k]->GetYaxis()->SetTitleOffset(1.1);
      h_tt[0][j][k]->GetZaxis()->SetTitleOffset(0.9);
      h_tt[0][j][k]->GetXaxis()->SetNdivisions(505);
      h_tt[0][j][k]->GetYaxis()->SetNdivisions(505);
      h_tt[0][j][k]->SetLineWidth(3);
      h_tt[0][j][k]->SetLineColor(13);
      h_tt[0][j][k]->Draw("HIST");

      h_tt[5][j][k]->SetLineWidth(3);
      h_tt[5][j][k]->SetLineColor(kAzure+7);
      h_tt[6][j][k]->SetLineWidth(3);
      h_tt[6][j][k]->SetLineColor(kRed-2);

      h_tt[5][j][k]->Draw("HIST SAME");
      h_tt[6][j][k]->Draw("HIST SAME");

      TLegend* leg = new TLegend(0.22, 0.65, 0.52, 0.85);
      leg->AddEntry(h_tt[0][j][k], "central", "l");
      leg->AddEntry(h_tt[5][j][k], "up", "l");
      leg->AddEntry(h_tt[6][j][k], "down", "l");
      leg->SetBorderSize(0);
      leg->Draw();

      a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/EffiVariations/TTbar_1prong_"+mergedSelections[j]+"_"+passfail[k]+".pdf");
      delete a;
    }
  }


    for(unsigned int j=0; j<mergedSelections.size(); j++){
      for(unsigned int k=0; k<passfail.size(); k++){
        TCanvas *a = new TCanvas("a", " ", 600, 600);
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.1);
        h_st[0][j][k]->SetTitle(" ");
        h_st[0][j][k]->GetXaxis()->SetRangeUser(0, 300);
        h_st[0][j][k]->GetYaxis()->SetRangeUser(0, h_st[0][j][k]->GetMaximum()*1.5);
        h_st[0][j][k]->GetYaxis()->SetTitleSize(0.06);
        h_st[0][j][k]->GetXaxis()->SetTitleSize(0.05);
        h_st[0][j][k]->GetZaxis()->SetTitleSize(0.05);
        h_st[0][j][k]->GetXaxis()->SetTitleOffset(0.9);
        h_st[0][j][k]->GetYaxis()->SetTitleOffset(1.1);
        h_st[0][j][k]->GetZaxis()->SetTitleOffset(0.9);
        h_st[0][j][k]->GetXaxis()->SetNdivisions(505);
        h_st[0][j][k]->GetYaxis()->SetNdivisions(505);
        h_st[0][j][k]->SetLineWidth(3);
        h_st[0][j][k]->SetLineColor(13);
        h_st[0][j][k]->Draw("HIST");

        h_st[1][j][k]->SetLineWidth(3);
        h_st[1][j][k]->SetLineColor(kAzure+7);
        h_st[2][j][k]->SetLineWidth(3);
        h_st[2][j][k]->SetLineColor(kRed-2);

        h_st[1][j][k]->Draw("HIST SAME");
        h_st[2][j][k]->Draw("HIST SAME");

        TLegend* leg = new TLegend(0.22, 0.65, 0.52, 0.85);
        leg->AddEntry(h_st[0][j][k], "central", "l");
        leg->AddEntry(h_st[1][j][k], "up", "l");
        leg->AddEntry(h_st[2][j][k], "down", "l");
        leg->SetBorderSize(0);
        leg->Draw();

        a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/EffiVariations/SingleTop_3prong_"+mergedSelections[j]+"_"+passfail[k]+".pdf");
        delete a;
      }
    }

    for(unsigned int j=0; j<mergedSelections.size(); j++){
      for(unsigned int k=0; k<passfail.size(); k++){
        TCanvas *a = new TCanvas("a", " ", 600, 600);
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.1);
        h_st[0][j][k]->SetTitle(" ");
        h_st[0][j][k]->GetXaxis()->SetRangeUser(0, 300);
        h_st[0][j][k]->GetYaxis()->SetRangeUser(0, h_st[0][j][k]->GetMaximum()*1.5);
        h_st[0][j][k]->GetYaxis()->SetTitleSize(0.06);
        h_st[0][j][k]->GetXaxis()->SetTitleSize(0.05);
        h_st[0][j][k]->GetZaxis()->SetTitleSize(0.05);
        h_st[0][j][k]->GetXaxis()->SetTitleOffset(0.9);
        h_st[0][j][k]->GetYaxis()->SetTitleOffset(1.1);
        h_st[0][j][k]->GetZaxis()->SetTitleOffset(0.9);
        h_st[0][j][k]->GetXaxis()->SetNdivisions(505);
        h_st[0][j][k]->GetYaxis()->SetNdivisions(505);
        h_st[0][j][k]->SetLineWidth(3);
        h_st[0][j][k]->SetLineColor(13);
        h_st[0][j][k]->Draw("HIST");

        h_st[3][j][k]->SetLineWidth(3);
        h_st[3][j][k]->SetLineColor(kAzure+7);
        h_st[4][j][k]->SetLineWidth(3);
        h_st[4][j][k]->SetLineColor(kRed-2);

        h_st[3][j][k]->Draw("HIST SAME");
        h_st[4][j][k]->Draw("HIST SAME");

        TLegend* leg = new TLegend(0.22, 0.65, 0.52, 0.85);
        leg->AddEntry(h_st[0][j][k], "central", "l");
        leg->AddEntry(h_st[3][j][k], "up", "l");
        leg->AddEntry(h_st[4][j][k], "down", "l");
        leg->SetBorderSize(0);
        leg->Draw();

        a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/EffiVariations/SingleTop_2prong_"+mergedSelections[j]+"_"+passfail[k]+".pdf");
        delete a;
      }
    }

    for(unsigned int j=0; j<mergedSelections.size(); j++){
      for(unsigned int k=0; k<passfail.size(); k++){
        TCanvas *a = new TCanvas("a", " ", 600, 600);
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.1);
        h_st[0][j][k]->SetTitle(" ");
        h_st[0][j][k]->GetXaxis()->SetRangeUser(0, 300);
        h_st[0][j][k]->GetYaxis()->SetRangeUser(0, h_st[0][j][k]->GetMaximum()*1.5);
        h_st[0][j][k]->GetYaxis()->SetTitleSize(0.06);
        h_st[0][j][k]->GetXaxis()->SetTitleSize(0.05);
        h_st[0][j][k]->GetZaxis()->SetTitleSize(0.05);
        h_st[0][j][k]->GetXaxis()->SetTitleOffset(0.9);
        h_st[0][j][k]->GetYaxis()->SetTitleOffset(1.1);
        h_st[0][j][k]->GetZaxis()->SetTitleOffset(0.9);
        h_st[0][j][k]->GetXaxis()->SetNdivisions(505);
        h_st[0][j][k]->GetYaxis()->SetNdivisions(505);
        h_st[0][j][k]->SetLineWidth(3);
        h_st[0][j][k]->SetLineColor(13);
        h_st[0][j][k]->Draw("HIST");

        h_st[5][j][k]->SetLineWidth(3);
        h_st[5][j][k]->SetLineColor(kAzure+7);
        h_st[6][j][k]->SetLineWidth(3);
        h_st[6][j][k]->SetLineColor(kRed-2);

        h_st[5][j][k]->Draw("HIST SAME");
        h_st[6][j][k]->Draw("HIST SAME");

        TLegend* leg = new TLegend(0.22, 0.65, 0.52, 0.85);
        leg->AddEntry(h_st[0][j][k], "central", "l");
        leg->AddEntry(h_st[5][j][k], "up", "l");
        leg->AddEntry(h_st[6][j][k], "down", "l");
        leg->SetBorderSize(0);
        leg->Draw();

        a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TopTagging/EffiVariations/SingleTop_1prong_"+mergedSelections[j]+"_"+passfail[k]+".pdf");
        delete a;
      }
    }


  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------
  return 0;
}
