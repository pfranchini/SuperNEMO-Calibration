//#############################################
//
// Plot vertexes for calibration source Bi207
// PF 2019
//
//#############################################

#include "TCanvas.h"
#include "TFile.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TTree.h"
#include <iostream>


int main(){

  /////////////////////////////////////////////////
  //TFile *f = new TFile("/home/hep/pfranchi/SuperNEMO/MC/20191119-1611/Bi207_source_calibration.root"); // Falaise 4.0.1
  //TFile *f = new TFile("/home/hep/pfranchi/SuperNEMO/MC/20191120-1441/Bi207_source_calibration.root"); // new geometry
  TFile *f = new TFile("/home/hep/pfranchi/SuperNEMO/MC/20191205-1626/calibBi207_source_calibration.root"); // new geometry 6 independent tracks
  //TFile *f = new TFile("/home/hep/pfranchi/SuperNEMO/MC/20200115-1204/Bi207_source_calibration.root"); // new geometry 6 independent tracks and new carrier dimensions

  Int_t nbins = 50;
  /////////////////////////////////////////////////

  TTree *t = (TTree*)f->Get("Sensitivity");

  TCanvas *c1 = new TCanvas("c1","Projections",1800,600);
  gStyle->SetOptStat(0);
  c1->Divide(3,1);

  TH2F *top_view   = new TH2F("top_view","Top view", nbins, -10, 10, nbins, -3000, 3000);
  TH2F *side_view  = new TH2F("side_view","Side view", nbins, -10, 10, nbins, -1600, 1600);
  TH2F *front_view = new TH2F("front_view","Front view", nbins, -3000, 3000, nbins, -1600, 1600);

  Double_t true_vertex_x;
  Double_t true_vertex_y;
  Double_t true_vertex_z;
  
  t->SetBranchAddress("true.vertex_x",&true_vertex_x);
  t->SetBranchAddress("true.vertex_y",&true_vertex_y);
  t->SetBranchAddress("true.vertex_z",&true_vertex_z);

  Long64_t nentries = t->GetEntries();
  for (Long64_t i=0;i<nentries;i++) {
    t->GetEntry(i);
    top_view->Fill(true_vertex_x,true_vertex_y);
    side_view->Fill(true_vertex_x,true_vertex_z);
    front_view->Fill(true_vertex_y,true_vertex_z);
    
  }

  top_view->GetXaxis()->SetTitle("True vertex X [mm]");
  top_view->GetYaxis()->SetTitle("True vertex Y [mm]");
  side_view->GetXaxis()->SetTitle("True vertex X [mm]");
  side_view->GetYaxis()->SetTitle("True vertex Z [mm]");
  front_view->GetXaxis()->SetTitle("True vertex Y [mm]");
  front_view->GetYaxis()->SetTitle("True vertex Z [mm]");

  c1->cd(1);
  gPad->SetGrid();
  top_view->Draw("colz");
  c1->cd(2);
  gPad->SetGrid();
  side_view->Draw("colz");
  c1->cd(3);
  gPad->SetGrid();
  front_view->Draw("colz");
  gPad->Modified(); gPad->Update();

  c1->SaveAs("calibration.png");
  system("display calibration.png");



  return 0;

}
