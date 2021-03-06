#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include <iomanip>
#include <locale>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TMath.h>

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <TF1.h>

// global variables





int main(int argc, char *argv[])
{ 

  // for output tree
  TFile *fOUT = new TFile("TMVA_input.root","RECREATE");
  float ax,ay,low,high;

  TTree t1("t1","a tree");
  t1.Branch("ax",&ax,"ax/F");
  t1.Branch("ay",&ay,"ay/F");
  t1.Branch("low",&low,"low/F");
  t1.Branch("high",&high,"high/F");

  // for random number generator
  TRandom r;
  gRandom->SetSeed();

  // for function histogram
  float xmin=1.;
  float xmax = 10.;
  const int nbin=5000;  

    
  TH1F *histo1 = new TH1F("histo1","s",nbin,xmin,xmax);
  histo1->SetMarkerStyle(21);
  // plot function
  for(int i=0;i<nbin;i++ ) {
    ax=float(i)*(xmax-xmin)/float(nbin)+xmin;
    ay=TMath::Log10(ax);
    histo1->SetBinContent(i,ay);
  }


  // for toy data
  const int ntoy =10000;
  const int ngap=40;
  for (int i=0;i<ntoy;i++) {
    // pick a random bin
    int j = r.Integer(nbin-2*ngap) +2*ngap;
    ax=float(j)*(xmax-xmin)/float(nbin)+xmin;
    // get value at that bin
    ay=histo1->GetBinContent(j);
    // get value at two neighboring bins
    low=histo1->GetBinContent(j-ngap);
    high=histo1->GetBinContent(j+ngap);
    // smear
    low=low+r.Gaus(0.,0.01*low);
    high=high+r.Gaus(0.,0.01*low);
    //
    std::cout<<" debug: i="<<i<<" ax="<<ax<<" ay="<<ay<<" low="<<low<<" high="<<high<<std::endl;
    t1.Fill();
  }
  fOUT->Write();


  //make some plots
  TCanvas* canv = new TCanvas("Fig","Fig",50,50,800,600);
  canv->SetFillColor(0);
  canv->SetBorderMode(0);
  canv->SetFrameFillStyle(0);
  canv->SetFrameBorderMode(0);
  canv->SetTickx(0);
  canv->SetTicky(0);
  histo1->Draw();
  canv->Print("c1.png",".png");

}
