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

void kalman_npoints(){

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

  //const int points = 10;
  const int points = 18;
  double point[points]; double err_point[points];
  double sig[points]; double err_sig[points]; double sig_x[points]; double err_sig_x[points];
  TH1D *hist_residual_p[points]; TH1D *hist_residual_x[points];

  double dummy1, dummy2;
  TGraphErrors *gr = new TGraphErrors();
  gr -> SetTitle(";# of hits;#Deltap/p");
  gr -> SetMarkerColor(2);
  gr -> SetMarkerSize(2);
  gr -> SetMarkerStyle(24);
  TGraphErrors *grx = new TGraphErrors();
  grx -> SetTitle(";# of hits;#sigma_{x} [#mum]");
  grx -> SetMarkerColor(2);
  grx -> SetMarkerSize(2);
  grx -> SetMarkerStyle(24);

  for(int j=0;j<points;j++){
    point[j] = 8+j;
    err_point[j] = 0.;
    hist_residual_p[j] = new TH1D(Form("hist_residual_p_%d",j),"[GenFit] Reconstructed P Residual; P Residual [GeV/c]; Counts [/0.001 GeV/c]", 400, -0.2, 0.2 );
    //hist_pval[j] = new TH1D(Form("hist_pval_%d",j),"[GenFit] P-value; P-value; Counts [/0.001]", 1000, 0., 1.0 );
    tr -> Project(Form("hist_residual_p_%d",j),"GFresidual_p[0]",Form("GFntTpc>0&&nhtrack[0]==%d",8+j));
    //tr -> Project(Form("hist_pval_%d",j),"GFresidual_p[0]",Form("GFntTpc>0&&nhtrack[0]==%d",8+j));
    genfit_monitor.Fitting(hist_residual_p[j], dummy1, dummy2, sig[j], err_sig[j]);
    genfit_monitor.can[2*j] -> SaveAs(Form("pdf/npoint/%s_%dpoints.pdf",tag.Data(),8+j));
    gr -> SetPoint(j,8+j,sig[j]);
    gr -> SetPointError(0.,err_sig[j]);

    hist_residual_x[j] = new TH1D(Form("hist_residual_x_%d",j),"[GenFit] Reconstructed X Position Residual; X Position Residual [#mum]; Counts [/40 #mum]", 100, -2000, 2000 );
    tr -> Project(Form("hist_residual_x_%d",j),"10000.*GFresiduals[0][0]",Form("GFntTpc>0&&nhtrack[0]==%d",8+j));
    genfit_monitor.Fitting(hist_residual_x[j], dummy1, dummy2, sig_x[j], err_sig_x[j]);
    genfit_monitor.can[2*j+1] -> SaveAs(Form("pdf/npoint/x_residual_%s_%dpoints.pdf",tag.Data(),8+j));
    grx -> SetPoint(j,8+j,sig_x[j]); //cm -> um
    grx -> SetPointError(0.,err_sig_x[j]);
    //grx -> SetPoint(j,8+j,sig_x[j]);
    //grx -> SetPointError(0.,err_sig_x[j]);
  }

  TLegend *legend1 = new TLegend(0.65,0.71,0.85,0.94);
  legend1 -> SetFillStyle(0);
  legend1 -> SetLineWidth(0);
  legend1 -> SetTextSize(0.05);
  legend1 -> AddEntry(gr,tag.Data(),"p");

  TCanvas *can = new TCanvas("can","can",1600,800);
  can -> cd();
  gr -> Draw("ap");
  gr -> GetHistogram() -> GetYaxis() -> SetRangeUser(0,0.07);
  legend1 -> Draw();
  gPad -> SetGrid();
  can -> SaveAs(Form("pdf/npoint/npoints_%s.pdf",tag.Data()));

  TCanvas *canx = new TCanvas("canx","canx",1600,800);
  canx -> cd();
  grx -> Draw("ap");
  //grx -> GetHistogram() -> GetYaxis() -> SetRangeUser(0,500.);
  legend1 -> Draw();
  gPad -> SetGrid();
  canx -> SaveAs(Form("pdf/npoint/position_npoints_%s.pdf",tag.Data()));

}//end
