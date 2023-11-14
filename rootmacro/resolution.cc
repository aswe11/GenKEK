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

void resolution(){

  //gStyle -> SetOptStat(0);
  gStyle -> SetOptFit(101);
  gStyle -> SetStatH(0.3);
  gStyle -> SetStatW(0.2);
  gStyle -> SetStatX(1.0);
  gStyle -> SetStatY(0.85);

  Genfit_Monitor genfit_monitor;

  //Directories
  const int file = 3;
  TString directory[file];
  directory[0] =  "/Volumes/data/kalman_filter/sim_res0"; //beam data directory
  directory[1] =  "/Volumes/data/kalman_filter/sim"; //beam data directory
  directory[2] =  "/Volumes/data/kalman_filter/sim_res3"; //beam data directory
  //directory[3] =  "/Volumes/data/kalman_filter/sim_res5"; //beam data directory

  double mean[6]={300,400,500,600,700,800}; double err_mean[6]={0};

  double sig[file][6]; double err_sig[file][6];
  double ref_sig[file][6]; double ref_err_sig[file][6];
  double dummy1;  double dummy2;
  int histnum1=203 ; int histnum2=303 ;
  //int time[file]={0,1,3,5};
  int time[file]={0,1,3};
  TFile* tf[file][6];
  for(int i=0;i<file;i++){
    for(int j=0;j<6;j++){
      tf[i][j] = new TFile(Form("%s/p_%dMeV.root",directory[i].Data(),300+100*j),"read");
      TH1D *hist1 = (TH1D*) tf[i][j] -> Get(Form("h%d",histnum1));
      genfit_monitor.Fitting(hist1, dummy1, dummy2, ref_sig[i][j], ref_err_sig[i][j]);
      ref_sig[i][j]=std::floor(ref_sig[i][j]*10000.)/10000.;
      ref_sig[i][j]/=(mean[j]/1000);
      ref_err_sig[i][j]/=(mean[j]/1000);
      genfit_monitor.can[12*i+2*j] -> SaveAs(Form("pdf/res/Helix_woEM_p_%dMeV_res%d.pdf",300+100*j,time[i]));

      TH1D *hist2 = (TH1D*) tf[i][j] -> Get(Form("h%d",histnum2));
      genfit_monitor.Fitting(hist2, dummy1, dummy2, sig[i][j], err_sig[i][j]);
      sig[i][j]=std::floor(sig[i][j]*10000.)/10000.;
      sig[i][j]/=(mean[j]/1000);
      err_sig[i][j]/=(mean[j]/1000);
      genfit_monitor.can[12*i+2*j+1] -> SaveAs(Form("pdf/res/genfit_woEM_p_%dMeV_res%d.pdf",300+100*j,time[i]));
    }
  }

  TMultiGraph *mg = new TMultiGraph();
  mg -> SetTitle(";Momentum [GeV/#font[12]{c}];#Deltap/p");

  TLegend *legend1 = new TLegend(0.65,0.72,0.85,0.95);
  legend1 -> SetFillStyle(0);
  legend1 -> SetLineWidth(0);
  legend1 -> SetTextSize(0.05);

  TLegend *legend2 = new TLegend(0.4,0.72,0.6,0.95);
  legend2 -> SetFillStyle(0);
  legend2 -> SetLineWidth(0);
  legend2 -> SetTextSize(0.05);

  TGraphErrors *gr[file]; TGraphErrors *ref_gr[file];
  legend1 -> AddEntry((TObject*)0,"Kalman Filter w/o EM","");
  //TString tag[file]={"no smearing","NIM resolution","3 times worse", "5 times worse"};
  TString tag[file]={"no smearing","NIM resolution","3 times worse"};
  for(int i=0;i<file;i++){
    gr[i] = new TGraphErrors(6,mean,sig[i],err_mean,err_sig[i]);
    gr[i] -> SetMarkerColor(kRed);
    gr[i] -> SetMarkerStyle(24+i);
    gr[i] -> SetMarkerSize(2);
    gr[i] -> SetTitle(", %s ;Momentum [GeV/#font[12]{c}];#Deltap/p");
    legend1 -> AddEntry(gr[i],Form("%s",tag[i].Data()),"p");
    mg -> Add(gr[i]);
    //if(i!=0) legend1 -> AddEntry(gr[i],Form("%s",tag[i].Data()),"p");
    //if(i!=0) mg -> Add(gr[i]);
  }

  legend2 -> AddEntry((TObject*)0,"Helix Tracking w/o EM","");
  for(int i=0;i<file;i++){
    ref_gr[i] = new TGraphErrors(6,mean,ref_sig[i],err_mean,ref_err_sig[i]);
    ref_gr[i] -> SetMarkerColor(kBlack);
    ref_gr[i] -> SetMarkerStyle(24+i);
    ref_gr[i] -> SetMarkerSize(2);
    ref_gr[i] -> SetTitle(" ;Momentum [GeV/#font[12]{c}];#Deltap/p");
    legend2 -> AddEntry(ref_gr[i],Form("%s",tag[i].Data()),"p");
    mg -> Add(ref_gr[i]);
    //if(i!=0) legend1 -> AddEntry(ref_gr[i],Form("%s",tag[i].Data()),"p");
    //if(i!=0) mg -> Add(ref_gr[i]);
  }

  TCanvas *can = new TCanvas("can","can",1600,800);
  can -> cd();
  mg -> Draw("ap");
  mg -> GetHistogram() -> GetYaxis() -> SetRangeUser(0,0.12);
  legend1 -> Draw();
  legend2 -> Draw();
  gPad -> SetGrid();
  can -> SaveAs("pdf/res/Genfit_P_resolution.pdf");

}//end
