#include "../include/CentralInclude.h"

using namespace std;
TString year = "2018";

vector<TH1F*> ConvertToHist(TGraphAsymmErrors* sf, TString jet);


int main(int argc, char* argv[]){
  if(argc > 1) year = argv[1];
  TFile* outfile = new TFile(year+"TopTaggingScaleFactors.root", "RECREATE");
  TFile* outfile_nomass = new TFile(year+"TopTaggingScaleFactors_NoMassCut.root", "RECREATE");

  TString dir="/afs/desy.de/user/s/schwarzd/Plots/TopTagging/ScaleFactors_FSR_f_prongs/"+year+"/";
  // vector<TString> jetcols = {"PUPPI", "PUPPI_btag", "CHS", "CHS_btag", "HOTVR"};
  vector<TString> jetcols = {"PUPPI", "PUPPI_btag", "HOTVR"};
  vector<TString> wps_HOTVR = {""};
  vector<TString> wps_PUPPI = {"wp1", "wp2", "wp3", "wp4", "wp5"};
  vector<TString> wps_PUPPI_btag = {"wp1_btag", "wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> wps_CHS = {"wp2", "wp3", "wp4", "wp5"};
  vector<TString> wps_CHS_btag = {"wp2_btag", "wp3_btag", "wp4_btag", "wp5_btag"};
  vector<TString> procnames = {"TTbar_mergedTop_2018", "TTbar_semimerged_2018", "TTbar_notmerged_2018"};

  for(auto jet: jetcols){
    vector<TString> wps;
    if(jet == "PUPPI") wps = wps_PUPPI;
    else if(jet == "PUPPI_btag"){
      wps = wps_PUPPI_btag;
      jet = "PUPPI";
    }
    else if(jet == "CHS"){
      wps = wps_CHS;
      jet = "CHS";
    }
    else if(jet == "CHS_btag"){
      wps = wps_CHS_btag;
      jet = "CHS";
    }
    else if(jet == "HOTVR") wps = wps_HOTVR;
    for(auto wp: wps){
      TString subdir;
      if(jet == "HOTVR") subdir = "HOTVR";
      else subdir = jet+"_"+wp;
      outfile->mkdir(subdir);
      outfile_nomass->mkdir(subdir);
      TString filename = dir+"eff_hists_"+jet+"_"+wp+".root";
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
      vector<TH1F*> h_SFmerged = ConvertToHist((TGraphAsymmErrors*) file->Get("sf_"+procnames[0]+"_tot"), jet);
      vector<TH1F*> h_SFsemimerged = ConvertToHist((TGraphAsymmErrors*) file->Get("sf_"+procnames[1]+"_tot"), jet);
      vector<TH1F*> h_SFnotmerged = ConvertToHist((TGraphAsymmErrors*) file->Get("sf_"+procnames[2]+"_tot"), jet);
      vector<TH1F*> h_SFmerged_nomass = ConvertToHist((TGraphAsymmErrors*) file->Get("sf_NoMass_"+procnames[0]+"_tot"), jet);
      vector<TH1F*> h_SFsemimerged_nomass = ConvertToHist((TGraphAsymmErrors*) file->Get("sf_NoMass_"+procnames[1]+"_tot"), jet);
      vector<TH1F*> h_SFnotmerged_nomass = ConvertToHist((TGraphAsymmErrors*) file->Get("sf_NoMass_"+procnames[2]+"_tot"), jet);
      vector<TString> histname = {"nominal", "up", "down"};
      for(unsigned int i=0; i<h_SFmerged.size(); i++){
        outfile->cd(subdir);
        h_SFmerged[i]->SetDirectory(gDirectory);
        h_SFmerged[i]->Write("sf_mergedTop_"+histname[i]);
        outfile_nomass->cd(subdir);
        h_SFmerged_nomass[i]->SetDirectory(gDirectory);
        h_SFmerged_nomass[i]->Write("sf_mergedTop_"+histname[i]);
      }
      for(unsigned int i=0; i<h_SFsemimerged.size(); i++){
        outfile->cd(subdir);
        h_SFsemimerged[i]->SetDirectory(gDirectory);
        h_SFsemimerged[i]->Write("sf_semimerged_"+histname[i]);
        outfile_nomass->cd(subdir);
        h_SFsemimerged_nomass[i]->SetDirectory(gDirectory);
        h_SFsemimerged_nomass[i]->Write("sf_semimerged_"+histname[i]);
      }
      for(unsigned int i=0; i<h_SFnotmerged.size(); i++){
        outfile->cd(subdir);
        h_SFnotmerged[i]->SetDirectory(gDirectory);
        h_SFnotmerged[i]->Write("sf_notmerged_"+histname[i]);
        outfile_nomass->cd(subdir);
        h_SFnotmerged_nomass[i]->SetDirectory(gDirectory);
        h_SFnotmerged_nomass[i]->Write("sf_notmerged_"+histname[i]);
      }
    }
  }
  outfile->Close();
  outfile_nomass->Close();

  return 0;
}


vector<TH1F*> ConvertToHist(TGraphAsymmErrors* sf, TString jet){
  vector<double> ptbins;
  vector<double> ptbins_PUPPI = {300, 400, 480, 600, 1100, 5000};
  vector<double> ptbins_HOTVR = {200, 250, 300, 400, 480, 600, 1100, 5000};
  if(jet.Contains("HOTVR")) ptbins = ptbins_HOTVR;
  else ptbins = ptbins_PUPPI;

  TH1F* nominal = new TH1F("nominal", "Probe jet p_{T} [GeV]", ptbins.size()-1, &ptbins[0]);
  TH1F* up = new TH1F("up", "Probe jet p_{T} [GeV]", ptbins.size()-1, &ptbins[0]);
  TH1F* down = new TH1F("down", "Probe jet p_{T} [GeV]", ptbins.size()-1, &ptbins[0]);

  Double_t* values = sf->GetY();
  const int Npoints = sf->GetN();

  if(Npoints+1 != ptbins.size()-1){
    throw std::invalid_argument( "unexpected number of pt bins!" );
  }

  for(int i=0; i<Npoints; i++){
    int bin = i+1;
    double c = values[i];
    double u = c + sf->GetErrorYhigh(i);
    double d = c - sf->GetErrorYlow(i);
    nominal->SetBinContent(bin, c);
    up->SetBinContent(bin, u);
    down->SetBinContent(bin, d);
  }
  // now set double uncert in last bin
  double c = values[Npoints-1];
  double u = c + 2*sf->GetErrorYhigh(Npoints-1);
  double d = c - 2*sf->GetErrorYlow(Npoints-1);
  nominal->SetBinContent(ptbins.size()-1, c);
  up->SetBinContent(ptbins.size()-1, u);
  down->SetBinContent(ptbins.size()-1, d);
  //

  vector<TH1F*> hists = {nominal, up, down};
  for(auto h: hists){
    h->SetTitle("");
    h->GetXaxis()->SetTitle("Probe jet p_{T} [GeV]");
  }
  return hists;
}
