//############################################
//
// Plot positions from 3D survey ascii file
//
//############################################


#include "TCanvas.h"
#include "TFile.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TTree.h"
#include "TSpectrum2.h"
#include "TSpectrum.h"
#include <iostream>


int main(){

  //Int_t nbins = 500;
  Float_t x_min = 850;
  Float_t x_max = 890;
  Float_t y_min = -12400;
  Float_t y_max = -7000;
  Float_t z_min = 10400;
  Float_t z_max = 13500;
  
  gStyle->SetOptStat(0);
  TCanvas *c1 = new TCanvas("c1","Projections",1800,600);
  c1->Divide(3,1);

  TCanvas *c2 = new TCanvas("c2","Positions",1800,600);
  c2->Divide(2,1);

  //TFile *f = new TFile("basic2.root","RECREATE");

  TH1F *Y   = new TH1F("Y","Y", y_max-y_min, y_min, y_max);
  TH1F *Z   = new TH1F("Z","Z", z_max-z_min, z_min, z_max);
 
  TH2F *top_view   = new TH2F("top_view","Top view", x_max-x_min, x_min, x_max, y_max-y_min, y_min, y_max);
  TH2F *side_view  = new TH2F("side_view","Side view", x_max-x_min, x_min, x_max, z_max-z_min, z_min, z_max);
  TH2F *front_view = new TH2F("front_view","Front view", (y_max-y_min)/1, y_min, y_max, (z_max-z_min)/1, z_min, z_max);

  //TH2F *h1 = new TH1F("h1","x distribution",100,-4,4);
  TTree *t = new TTree("ntuple","data from ascii file");
  Long64_t nlines = t->ReadFile("bi_source_position.dat","_y:_x:_z");
  std::cout << "Found points: " << nlines << std::endl;

  Float_t x;
  Float_t y;
  Float_t z;

  t->SetBranchAddress("_x",&x);
  t->SetBranchAddress("_y",&y);
  t->SetBranchAddress("_z",&z);

  Long64_t nentries = t->GetEntries();
  for (Long64_t i=0;i<nentries;i++) {
    t->GetEntry(i);
    top_view->Fill(x,y);
    side_view->Fill(x,z);
    front_view->Fill(y,z);
    Y->Fill(y);
    Z->Fill(z);
  }

  top_view->GetXaxis()->SetTitle("X [mm]");
  top_view->GetYaxis()->SetTitle("Y [mm]");
  side_view->GetXaxis()->SetTitle("X [mm]");
  side_view->GetYaxis()->SetTitle("Z [mm]");
  front_view->GetXaxis()->SetTitle("Y [mm]");
  front_view->GetYaxis()->SetTitle("Z [mm]");

  Y->GetXaxis()->SetTitle("Y [mm]");
  Z->GetXaxis()->SetTitle("Z [mm]");

  c2->cd(2);
  Z->Draw();
  c2->cd(1);
  Y->Draw();

  c2->SaveAs("position2.png");

  // Find peaks in YZ

  TSpectrum2 *sYZ = new TSpectrum2(100);
  TH2F *YZ = (TH2F*)front_view->Clone("YZ");
  //Int_t nfound_YZ = sYZ->Search(front_view,5,"col,noMarkov");
  Int_t nfound_YZ = sYZ->Search(YZ,10,"col,noMarkov");

  std::cout << "YZ peaks: " << nfound_YZ << std::endl;
  Float_t *xpeaks = sYZ->GetPositionX();
  Float_t *ypeaks = sYZ->GetPositionY();

  for ( Int_t p=0;p<nfound_YZ;p++) {
    std::cout << xpeaks[p] << "\t" << ypeaks[p] << std::endl;
  }

  c1->cd(1);
  top_view->Draw("colz");
  c1->cd(2);  
  side_view->Draw("colz");
  c1->cd(3);
  front_view->Draw("colz");
  c1->SaveAs("position1.png");

  // Find peaks
  
  TSpectrum *sY = new TSpectrum(20);
  TSpectrum *sZ = new TSpectrum(40);

  TH1F *YY = (TH1F*)Y->Clone("YY");
  TH1F *ZZ = (TH1F*)Z->Clone("ZZ");

  Int_t nfound_Y = sY->Search(YY,1,"new");
  Int_t nfound_Z = sZ->Search(ZZ,1,"new");

  std::cout << "Y peaks: " << nfound_Y << std::endl;
  Float_t *peaks_Y = sY->GetPositionX();
  for ( Int_t p=0;p<nfound_Y;p++) {
    std::cout << peaks_Y[p] <<std::endl;
  }
  
  std::cout << "Z peaks: " << nfound_Z << std::endl;
  Float_t *peaks_Z = sZ->GetPositionX();
  for ( Int_t p=0;p<nfound_Z;p++) {
    std::cout << peaks_Z[p] <<std::endl;
  }
  
 

  system("display position*.png");




}
