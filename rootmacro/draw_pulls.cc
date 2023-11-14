void draw_pulls(){

  //TString path = "/Volumes/data/genfit_uniform_p/calc_p_em/";
  TString path = "/Volumes/data/genfit_uniform_p/nofield_p_em/";
  TString pdfpath = "pdf/pulls/";
  //TString tag= "p300MeV_GenfitHelixGeant4";
  TString tag= "p300MeV_GenfitLinearGeant4";
  TString file = path + tag + ".root";
  TString pdf = pdfpath + tag;

  TFile *tf = new TFile(file.Data(),"read");

  gStyle -> SetOptFit(1);
#if 1
  TCanvas* can1 = new TCanvas("can1","",1200,600);
  can1 -> Divide(3,2);
  TH1D *hist_pulls[5];
  for(int i=0;i<5;i++){
    hist_pulls[i] = (TH1D *) tf -> Get(Form("h%d",310+i));
    can1 -> cd(i+1);
    hist_pulls[i] -> Draw();
    hist_pulls[i] -> Fit("gaus");
  }
  TString pdf1 = pdf + "_pulls5d.pdf";
  can1 -> SaveAs(pdf1.Data());
#endif
#if 1
  TCanvas* can2 = new TCanvas("can2","",600,300);
  can2 -> Divide(2,1);
  TH1D *hist_chi = (TH1D *) tf -> Get("h304");
  TH1D *hist_pval = (TH1D *) tf -> Get("h315");
  can2 -> cd(1);
  hist_chi -> Draw();
  hist_chi -> GetXaxis() -> SetRangeUser(0,10);
  can2 -> cd(2);
  hist_pval -> Draw();
  //gPad -> SetLogy();
  TString pdf2 = pdf + "_chisqr_pval.pdf";
  can2 -> SaveAs(pdf2.Data());
#endif
#if 1
  TCanvas* can4 = new TCanvas("can4","",1500,400);
  can4 -> Divide(2,1);
  TH1D *hist_Presidual[2];
  can4 -> cd(1);
  hist_Presidual[0] = (TH1D *) tf -> Get("h203");
  hist_Presidual[0] -> Draw();
  can4 -> cd(2);
  hist_Presidual[1] = (TH1D *) tf -> Get("h303");
  hist_Presidual[1] -> Draw();
  TString pdf4 = pdf + "_dp.pdf";
  can4 -> SaveAs(pdf4.Data());
#endif
#if 1
  TCanvas* can5 = new TCanvas("can5","",1200,600);
  can5 -> Divide(3,2);
  TH1D *hist_pull6d[6];
  for(int i=0;i<6;i++){
    can5 -> cd(i+1);
    hist_pull6d[i] = (TH1D *) tf -> Get(Form("h%d",320+i));
    hist_pull6d[i] -> Draw();
    hist_pull6d[i] -> Fit("gaus");
  }
  TString pdf5 = pdf + "_pulls6d.pdf";
  can5 -> SaveAs(pdf5.Data());
#endif
#if 1
  TCanvas* can6 = new TCanvas("can6","",1200,600);
  can6 -> Divide(3,2);
  TH1D *hist_gfresi[6];
  for(int i=0;i<6;i++){
    can6 -> cd(i+1);
    hist_gfresi[i] = (TH1D *) tf -> Get(Form("h%d",326+i));
    hist_gfresi[i] -> Draw();
    //hist_gfresi[i] -> Fit("gaus");
  }
  TString pdf6 = pdf + "_gfresiduals.pdf";
  can6 -> SaveAs(pdf6.Data());
#endif
#if 1
  TCanvas* can7 = new TCanvas("can7","",1200,600);
  can7 -> Divide(3,2);
  TH1D *hist_k18resi[6];
  for(int i=0;i<6;i++){
    can7 -> cd(i+1);
    hist_k18resi[i] = (TH1D *) tf -> Get(Form("h%d",207+i));
    hist_k18resi[i] -> Draw();
    //hist_k18resi[i] -> Fit("gaus");
  }
  TString pdf7 = pdf + "_k18residuals.pdf";
  can7 -> SaveAs(pdf7.Data());
#endif

}
