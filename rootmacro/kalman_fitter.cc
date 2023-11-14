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

void kalman_fitter(){

  //gStyle -> SetOptStat(0);
  gStyle -> SetOptFit(101);
  gStyle -> SetStatH(0.3);
  gStyle -> SetStatW(0.2);
  gStyle -> SetStatX(1.0);
  gStyle -> SetStatY(0.85);

  Genfit_Monitor genfit_monitor;

  //Directories
  const int file = 3;
  TString path = "/Volumes/data/kalman";
  TString directory[file] = {"res1","res3","res5"};
  TString daf_directory[file] = {"daf_res1","daf_res3","daf_res5"};

  double mean[6]={300,400,500,600,700,800}; double err_mean[6]={0};
  double sig[file][6]; double err_sig[file][6];
  double daf_sig[file][6]; double daf_err_sig[file][6];
  double dummy1;  double dummy2;
  int histnum1=203; int histnum2=303;
  TFile* tf[file][6]; TFile* daf_tf[file][6];
  for(int i=0;i<file;i++){
    for(int j=0;j<6;j++){
      tf[i][j] = new TFile(Form("%s/%s/p_%dMeV.root",path.Data(),directory[i].Data(),300+100*j),"read");
      TH1D *hist1 = (TH1D*) tf[i][j] -> Get(Form("h%d",histnum1));
      genfit_monitor.Fitting(hist1, dummy1, dummy2, sig[i][j], err_sig[i][j]);
      //sig[j]=std::floor(sig[j]*10000.)/10000.;
      sig[i][j]/=(mean[j]/1000);
      err_sig[i][j]/=(mean[j]/1000);
      genfit_monitor.can[2*6*i+2*j] -> SaveAs(Form("pdf/fitter/kalman_%dMeV_%s.pdf",300+100*j,directory[i].Data()));

      daf_tf[i][j] = new TFile(Form("%s/%s/p_%dMeV.root",path.Data(),daf_directory[i].Data(),300+100*j),"read");
      TH1D *hist2 = (TH1D*) daf_tf[i][j] -> Get(Form("h%d",histnum2));
      genfit_monitor.Fitting(hist2, dummy1, dummy2, daf_sig[i][j], daf_err_sig[i][j]);
      //daf_sig[j]=std::floor(daf_sig[j]*10000.)/10000.;
      daf_sig[i][j]/=(mean[j]/1000);
      daf_err_sig[i][j]/=(mean[j]/1000);
      genfit_monitor.can[2*6*i+2*j+1] -> SaveAs(Form("pdf/fitter/kalman_%dMeV_%s.pdf",300+100*j,daf_directory[i].Data()));
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

  TGraphErrors *gr[file]; TGraphErrors *daf_gr[file];
  legend1 -> AddEntry((TObject*)0,"Kalman Filter","");
  TString tag[file]={"NIM resolution ","3 times","5 times"};
  for(int i=0;i<file;i++){
    gr[i] = new TGraphErrors(6,mean,sig[i],err_mean,err_sig[i]);
    if(i<2) gr[i] -> SetMarkerColor(2);
    else gr[i] -> SetMarkerColor(4);
    gr[i] -> SetMarkerStyle(24+i%2);
    gr[i] -> SetMarkerSize(2);
    gr[i] -> SetTitle(", %s ;Momentum [GeV/#font[12]{c}];#Deltap/p");
    legend1 -> AddEntry(gr[i],Form("%s",tag[i].Data()),"p");
    mg -> Add(gr[i]);
  }

  legend2 -> AddEntry((TObject*)0,"DAF Filter","");
  for(int i=0;i<file;i++){
    daf_gr[i] = new TGraphErrors(6,mean,daf_sig[i],err_mean,daf_err_sig[i]);
    if(i<2) daf_gr[i] -> SetMarkerColor(1);
    else daf_gr[i] -> SetMarkerColor(7);
    daf_gr[i] -> SetMarkerStyle(24+i%2);
    daf_gr[i] -> SetMarkerSize(2);
    daf_gr[i] -> SetTitle(" ;Momentum [GeV/#font[12]{c}];#Deltap/p");
    legend2 -> AddEntry(daf_gr[i],Form("%s",tag[i].Data()),"p");
    mg -> Add(daf_gr[i]);
  }

  TCanvas *can = new TCanvas("can","can",1600,800);
  can -> cd();
  mg -> Draw("ap");
  mg -> GetHistogram() -> GetYaxis() -> SetRangeUser(0,0.04);
  legend1 -> Draw();
  legend2 -> Draw();
  gPad -> SetGrid();
  can -> SaveAs("pdf/fitter/Genfit_P_resolution.pdf");

}//end
