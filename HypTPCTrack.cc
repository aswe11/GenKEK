//  Authors: Wooseung Jung

//GenKEK
#include "HypTPCTrack.hh"
#include "HypTPCHit.hh"

//k18-analyzer
#include "TPCLTrackHit.hh"

//GenFit
#include <AbsFitterInfo.h>
#include <AbsHMatrix.h>
#include <AbsTrackRep.h>
#include <DetPlane.h>
#include <Exception.h>
#include <FitStatus.h>
#include <KalmanFittedStateOnPlane.h>
#include <KalmanFitterInfo.h>
#include <MeasuredStateOnPlane.h>
#include <MeasurementOnPlane.h>
#include <SharedPlanePtr.h>
#include <Tools.h>
#include <TrackPoint.h>
#include <RKTrackRep.h>

//ROOT
#include <TMatrixDfwd.h>                      // for TMatrixD
#include <TMatrixDSymfwd.h>                   // for TMatrixDSym
#include <TMatrixT.h>                         // for TMatrixT
#include <TMatrixTSym.h>                      // for TMatrixTSym
#include <TVectorDfwd.h>                      // for TVectorD
#include <TVector3.h>                         // for TVector3
#include <TVectorT.h>                         // for TVectorT, operator-

//STL
#include <iostream>

ClassImp(HypTPCTrack)

TClonesArray *HypTPCTrack::_hitClusterArray = nullptr;
TClonesArray *HypTPCTrack::_genfitTrackArray = nullptr;

HypTPCTrack::HypTPCTrack(){

  _hitClusterArray = new TClonesArray("HypTPCHit");
  _genfitTrackArray = new TClonesArray("genfit::Track");
  _measurementProducer = new genfit::MeasurementProducer<HypTPCHit, genfit::HypTPCSpacepointMeasurement>(_hitClusterArray);
  _measurementFactory = new genfit::MeasurementFactory<genfit::AbsMeasurement>();
  _measurementFactory -> addProducer(TPCDetID, _measurementProducer);
  std::cout<<"GenFit : HypTPCTrack container is ready"<<std::endl;

}

void HypTPCTrack::Init(){

  _genfitTrackArray -> Delete();
  _hitClusterArray -> Delete();

}

genfit::Track* HypTPCTrack::GetTrack(int ith) const {

  return (genfit::Track*) _genfitTrackArray -> ConstructedAt(ith);

}

void HypTPCTrack::AddReps(int ith, int pdg){

  genfit::Track* tr = GetTrack(ith);
  tr -> addTrackRep(new genfit::RKTrackRep(pdg));

}

void HypTPCTrack::AddHelixTrack(int pdg, TPCLocalTrackHelix *tp){

  _hitClusterArray -> Delete();
  genfit::TrackCand trackCand;

  int nMeasurement = tp -> GetNHit();
  for(int i=0; i<nMeasurement; i++){
    TPCLTrackHit *point = tp -> GetHit(i);
    new ((*_hitClusterArray)[i]) HypTPCHit(*point);
    trackCand.addHit(TPCDetID, i);
  }

  //GenFit Units : GeV/c, ns, cm, kGauss
  //K1.8Ana Units : GeV/c, ns, mm, T
  const TVector3& res_vect = tp -> GetHit(0) -> GetResolutionVect();
  TMatrixDSym covSeed(6);
  covSeed.Zero();
  /*
  covSeed(0, 0) = 0.01;
  covSeed(1, 1) = 0.01;
  covSeed(2, 2) = 0.04;
  for (int iComp = 3; iComp < 6; iComp++) covSeed(iComp, iComp) = covSeed(iComp - 3, iComp - 3);
  */
  covSeed(0, 0) = res_vect.X() * res_vect.X()/100.;
  covSeed(1, 1) = res_vect.Y() * res_vect.Y()/100.;
  covSeed(2, 2) = res_vect.Z() * res_vect.Z()/100.;
  for (int iComp = 3; iComp < 6; iComp++) covSeed(iComp, iComp) = covSeed(iComp - 3, iComp - 3)/(3*nMeasurement*nMeasurement);

  TVector3 momSeed = tp -> GetHit(0) -> GetMomentumHelix(); //GeV/c
  TVector3 posSeed = tp -> GetHit(0) -> GetLocalCalPosHelix();
  posSeed.SetMag(posSeed.Mag()/10.); //mm -> cm
  // set start values and pdg to cand
  trackCand.setCovSeed(covSeed);
  trackCand.setPosMomSeedAndPdgCode(posSeed, momSeed, pdg);
  trackCand.setTimeSeed(0.); //set defualt _time=0.;
  //covSeed.Print();
  //trackCand.setPosMomSeed(posSeed, momSeed, helixTrack -> Charge());

  new ((*_genfitTrackArray)[_genfitTrackArray -> GetEntriesFast()]) genfit::Track(trackCand, *_measurementFactory, new genfit::RKTrackRep(pdg));

}

void HypTPCTrack::AddHelixTrack(std::vector<int> pdg, TPCLocalTrackHelix *tp){

  int Ntracks = GetNTrack();
  int Nreps = pdg.size();
  for(int i=0;i<Nreps;i++){
    int pid = pdg.at(i);
    if(i==0) AddHelixTrack(pid, tp);
    else AddReps(Ntracks, pid);
  }

}

int HypTPCTrack::GetNHit(int ith) const {

  genfit::Track* tr = GetTrack(ith);
  int num =(int) tr -> getNumPoints();
  return num;

}
