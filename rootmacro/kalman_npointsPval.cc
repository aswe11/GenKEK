#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TChain.h>
#include <TStyle.h>
#include <TROOT.h>
#include <fstream>
#include <iostream>
#include <TPad.h>
#include <TCut.h>

#include "Genfit_Monitor.h"
using namespace std;

void kalman_npointsPval(){

  //gStyle -> SetOptStat(0);
  gStyle -> SetOptFit(101);
  gStyle -> SetStatH(0.3);
  gStyle -> SetStatW(0.2);
  gStyle -> SetStatX(1.0);
  gStyle -> SetStatY(0.85);

  Genfit_Monitor genfit_monitor;

  //Directories
  TString path = "/Volumes/data/genfit_uniform_p/";
  int mom = 300; //MeV/c
  //TString directory = "calc_p";
  //TString tag =Form("%dMeV_calcB_woEM",mom);
  TString directory = "calc_p_em";
  TString tag =Form("%dMeV_calcB_wEM",mom);
  //TString directory = "uniform_p_em";
  //TString tag =Form("%dMeV_uniformB_wEM",mom);
  //TString directory = "uniform_p";
  //TString tag =Form("%dMeV_uniformB_woEM",mom);
  TFile* tf = new TFile(Form("%s/%s/p%dMeV_GenfitHelixGeant4.root",path.Data(),directory.Data(),mom),"read");
  TTree* tr = (TTree*) tf -> Get("tpc");

  const int points = 18;
  TH1D *hist_pval[points];
  TCanvas *can = new TCanvas("can","can",600,600);
  can -> Divide(5,4);
  for(int j=0;j<points;j++){
    hist_pval[j] = new TH1D(Form("hist_pval_%d",j),"[GenFit] p-Value; p-Value; Counts [/0.01]", 100, 0., 1. );
    tr -> Project(Form("hist_pval_%d",j),"GFpval[0]",Form("GFntTpc>0&&nhtrack[0]==%d",8+j));
    can -> cd(j+1);
    hist_pval[j] -> Draw();
  }
  can -> SaveAs(Form("pdf/npoint/%s_pval.pdf",tag.Data()));

}//end
