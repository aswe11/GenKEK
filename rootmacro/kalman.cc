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

void kalman(){

  //gStyle -> SetOptStat(0);
  gStyle -> SetOptFit(101);
  gStyle -> SetStatH(0.3);
  gStyle -> SetStatW(0.2);
  gStyle -> SetStatX(1.0);
  gStyle -> SetStatY(0.85);

  Genfit_Monitor genfit_monitor;

  //Directories
  const int file = 4;
  TString base="/Volumes/data/genfit_uniform_p/";
  TString directory[file];
  directory[0] = "calc_p_em"; //beam data directory
  directory[1] = "uniform_p_em"; //beam data directory
  directory[2] = "calc_p"; //beam data directory
  directory[3] = "uniform_p"; //beam data directory
  TString tag[file]={"Calc. B-field w/ EM","Uniform. B-field w/ EM","Calc. B-field w/o EM","Uniform. B-field w/o EM"};
  double mean[6]={300,400,500,600,700,800}; double err_mean[6]={0};

  double sig[file][6]; double err_sig[file][6];
  double ref_sig[file][6]; double ref_err_sig[file][6];
  double dummy1;  double dummy2;
  int histnum1=203; int histnum2=303;
  TFile* tf[file][6];
  for(int i=0;i<file;i++){
    for(int j=0;j<6;j++){
      tf[i][j] = new TFile(Form("%s%s/p%dMeV_GenfitHelixGeant4.root",base.Data(),directory[i].Data(),300+100*j),"read");
      TH1D *hist1 = (TH1D*) tf[i][j] -> Get(Form("h%d",histnum1));
      genfit_monitor.Fitting(hist1, dummy1, dummy2, ref_sig[i][j], ref_err_sig[i][j]);
      ref_sig[i][j]/=(mean[j]/1000);
      ref_err_sig[i][j]/=(mean[j]/1000);
      genfit_monitor.can[12*i+2*j] -> SaveAs(Form("pdf/kalman/Helix_%s_%dMeV.pdf",directory[i].Data(),300+100*j));

      TH1D *hist2 = (TH1D*) tf[i][j] -> Get(Form("h%d",histnum2));
      genfit_monitor.Fitting(hist2, dummy1, dummy2, sig[i][j], err_sig[i][j]);
      sig[i][j]/=(mean[j]/1000);
      err_sig[i][j]/=(mean[j]/1000);
      genfit_monitor.can[12*i+2*j+1] -> SaveAs(Form("pdf/kalman/Genfit_%s_%dMeV.pdf",directory[i].Data(),300+100*j));
    }
  }

  TMultiGraph *mg = new TMultiGraph();
  mg -> SetTitle(";Momentum [GeV/#font[12]{c}];#Deltap/p");

  TLegend *legend1 = new TLegend(0.65,0.71,0.85,0.94);
  legend1 -> SetFillStyle(0);
  legend1 -> SetLineWidth(0);
  legend1 -> SetTextSize(0.05);

  TLegend *legend2 = new TLegend(0.3,0.71,0.5,0.94);
  legend2 -> SetFillStyle(0);
  legend2 -> SetLineWidth(0);
  legend2 -> SetTextSize(0.05);

  TGraphErrors *gr[file]; TGraphErrors *ref_gr[file];
  legend1 -> AddEntry((TObject*)0,"Kalman Filter","");

  for(int i=0;i<file;i++){
    gr[i] = new TGraphErrors(6,mean,sig[i],err_mean,err_sig[i]);
    if(i<2) gr[i] -> SetMarkerColor(2);
    else gr[i] -> SetMarkerColor(4);
    gr[i] -> SetMarkerStyle(24+i%2);
    gr[i] -> SetMarkerSize(2);
    gr[i] -> SetTitle(";Momentum [GeV/#font[12]{c}];#Deltap/p");
    legend1 -> AddEntry(gr[i],Form("%s",tag[i].Data()),"p");
    mg -> Add(gr[i]);
  }

  legend2 -> AddEntry((TObject*)0,"Helix Tracking","");
  for(int i=0;i<file;i++){
    ref_gr[i] = new TGraphErrors(6,mean,ref_sig[i],err_mean,ref_err_sig[i]);
    if(i<2) ref_gr[i] -> SetMarkerColor(1);
    else ref_gr[i] -> SetMarkerColor(7);
    ref_gr[i] -> SetMarkerStyle(24+i%2);
    ref_gr[i] -> SetMarkerSize(2);
    ref_gr[i] -> SetTitle(" ;Momentum [GeV/#font[12]{c}];#Deltap/p");
    legend2 -> AddEntry(ref_gr[i],Form("%s",tag[i].Data()),"p");
    mg -> Add(ref_gr[i]);
  }

  TCanvas *can = new TCanvas("can","can",1600,800);
  can -> cd();
  mg -> Draw("ap");
  mg -> GetHistogram() -> GetYaxis() -> SetRangeUser(0,0.04);
  legend1 -> Draw();
  legend2 -> Draw();
  gPad -> SetGrid();
  can -> SaveAs("pdf/kalman/Genfit_P_resolution.pdf");

}//end
