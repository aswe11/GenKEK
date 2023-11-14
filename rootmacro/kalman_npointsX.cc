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

void kalman_npointsX(){

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
  double point[points]; double err_point[points];
  double sig[points]; double err_sig[points]; double sig_x[points]; double err_sig_x[points];
  TH1D *hist_residual_p[points]; TH1D *hist_residual_x[points];

  double dummy1, dummy2;
  TGraphErrors *gr = new TGraphErrors();
  gr -> SetTitle(";# of hits;#sigma_{x} [#mum]");
  gr -> SetMarkerColor(2);
  gr -> SetMarkerSize(2);
  gr -> SetMarkerStyle(24);

  TGraphErrors *grx = new TGraphErrors();
  grx -> SetTitle(";# of hits;#sigma_{x} [#mum]");
  grx -> SetMarkerColor(kBlue);
  grx -> SetMarkerSize(2);
  grx -> SetMarkerStyle(24);
  TCanvas *can1[points];   TCanvas *can2[points];
  for(int j=0;j<points;j++){
    point[j] = 8+j;
    err_point[j] = 0.;
    hist_residual_p[j] = new TH1D(Form("hist_residual_x_%d",j),"[GenFit] Reconstructed X Position Residual; X Position Residual [#mum]; Counts [/10 #mum]", 400, -2000, 2000 );
    tr -> Project(Form("hist_residual_x_%d",j),"10000.*GFresiduals[0][3]",Form("GFntTpc>0&&nhtrack[0]==%d",8+j));
    can1[j] = new TCanvas(Form("can1%d",j),"",600,600);
    can1[j] -> cd();
    hist_residual_p[j] -> Draw();
    sig[j] = hist_residual_p[j] -> GetStdDev();
    err_sig[j] = hist_residual_p[j] -> GetStdDevError();
    can1[j] -> SaveAs(Form("pdf/npoint/%s_X_GF_%dpoints.pdf",tag.Data(),8+j));
    gr -> SetPoint(j,8+j,sig[j]);
    gr -> SetPointError(j,0.,err_sig[j]);

    hist_residual_x[j] = new TH1D(Form("hist_residual_k18_%d",j),"[K1.8] Reconstructed X Position Residual; X Position Residual [#mum]; Counts [/10 #mum]", 400, -2000, 2000 );
    //tr -> Project(Form("hist_residual_k18_%d",j),"1000.*residual_x[0][0]",Form("ntTpc>0&&nhtrack[0]==%d",8+j));
    tr -> Project(Form("hist_residual_k18_%d",j),"1000.*(calpos_x[0][0]-g4pos_x[0][0])",Form("ntTpc>0&&nhtrack[0]==%d",8+j));
    can2[j] = new TCanvas(Form("can2%d",j),"",600,600);
    can2[j] -> cd();
    hist_residual_x[j] -> Draw();
    sig_x[j] = hist_residual_x[j] -> GetStdDev();
    err_sig_x[j] = hist_residual_x[j] -> GetStdDevError();
    can2[j] -> SaveAs(Form("pdf/npoint/%s_X_K18_%dpoints.pdf",tag.Data(),8+j));
    grx -> SetPoint(j,8+j,sig_x[j]);
    grx -> SetPointError(j,0.,err_sig_x[j]);
  }

  TLegend *legend1 = new TLegend(0.65,0.71,0.85,0.94);
  legend1 -> SetFillStyle(0);
  legend1 -> SetLineWidth(0);
  legend1 -> SetTextSize(0.05);
  legend1 -> AddEntry(gr,"w/ Kalman Filter","p");
  legend1 -> AddEntry(grx,"w0/ Kalman Filter","p");

  TMultiGraph *mg = new TMultiGraph();
  mg -> SetTitle(";# of hits;#sigma_{x} [#mum]");
  mg -> Add(gr);
  mg -> Add(grx);
  TCanvas *can = new TCanvas("can","can",1600,800);
  can -> cd();
  mg -> Draw("ap");
  mg -> GetHistogram() -> GetYaxis() -> SetRangeUser(0,350);
  legend1 -> Draw();
  gPad -> SetGrid();
  can -> SaveAs(Form("pdf/npoint/X_npoints_%s.pdf",tag.Data()));

}//end
