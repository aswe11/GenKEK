#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TF1.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TString.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <fstream>
#include <math.h>
#include <iostream>

static int cannum=0;
class Genfit_Monitor{

 private:

 public:

  void Fitting(TH1D *hist, double &mean, double &error_mean, double &sigma, double &error_sigma);
  TCanvas *can[100];

};


void Genfit_Monitor::Fitting(TH1D *hist, double &mean, double &error_mean, double &sigma, double &error_sigma){

  can[cannum] = new TCanvas(Form("can%d",cannum),Form("can%d",cannum),900,600);
  TF1 *func = new TF1("func","gaus",-3000,3000);
  double std = 0.5*hist -> GetStdDev();
  func -> SetRange(-std,std);
  can[cannum] -> cd();
  hist -> Draw();
  hist -> Fit(func,"REQ");

  double range = 1.2*func -> GetParameter(2);
  double center = func -> GetParameter(1);
  func -> SetRange(center-range, center+range);
  hist -> Fit(func,"REQ");
  mean = func -> GetParameter(1);
  error_mean = func -> GetParError(1);
  sigma = func -> GetParameter(2);
  error_sigma = func -> GetParError(2);

  cannum++;
}
