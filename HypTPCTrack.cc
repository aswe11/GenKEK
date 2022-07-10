//  Authors: Wooseung Jung

//GenKEK
#include "HypTPCTrack.hh"

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
//TClonesArray *HypTPCTrack::_hitClusterArray = new TClonesArray("TPCLTrackHit");
//TClonesArray *HypTPCTrack::_genfitTrackArray = new TClonesArray("genfit::Track");

HypTPCTrack::HypTPCTrack(){

  std::cout<<"Hyptpctrack Constructor"<<std::endl;
  _hitClusterArray = new TClonesArray("HypTPCHit");
  std::cout<<"Hyptpctrack Constructor---1"<<std::endl;
  _genfitTrackArray = new TClonesArray("genfit::Track");
  std::cout<<"Hyptpctrack Constructor---2"<<std::endl;

  _measurementProducer = new genfit::MeasurementProducer<HypTPCHit, genfit::HypTPCSpacepointMeasurement>(_hitClusterArray);
  _measurementFactory = new genfit::MeasurementFactory<genfit::AbsMeasurement>();
  _measurementFactory -> addProducer(TPCDetID, _measurementProducer);

  std::cout<<"Measurementfactory is constructed"<<std::endl;
}

void HypTPCTrack::Init(){
  std::cout<<"init1"<<std::endl;
  _genfitTrackArray -> Delete();
  std::cout<<"init2"<<std::endl;
  _hitClusterArray -> Delete();
  std::cout<<"init3"<<std::endl;
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

  //GenFit Units : GeV/c, cm, kGauss
  const TVector3& res_vect = tp -> GetHit(0) -> GetResolutionVect();
  TMatrixDSym covSeed(6);
  covSeed.Zero();
  covSeed(0, 0) = res_vect.X() * res_vect.X()/100.;
  covSeed(1, 1) = res_vect.Y() * res_vect.Y()/100.;
  covSeed(2, 2) = res_vect.Z() * res_vect.Z()/100.;
  for (int iComp = 3; iComp < 6; iComp++)
    covSeed(iComp, iComp) = covSeed(iComp - 3, iComp - 3)
      /(3*nMeasurement*nMeasurement);
  //TVector3& momSeed = tp -> GetHit(0) -> GetMomentumHelix();
  TVector3 momSeed = tp -> GetHit(0) -> GetMomentumHelix();
  momSeed.SetMag(momSeed.Mag()/1000.);
  //TVector3& posSeed = tp -> GetHit(0) -> GetLocalCalPosHelix();
  TVector3 posSeed = tp -> GetHit(0) -> GetLocalCalPosHelix();
  posSeed.SetMag(posSeed.Mag()/10.);
  // set start values and pdg to cand
  trackCand.setCovSeed(covSeed);
  trackCand.setPosMomSeedAndPdgCode(posSeed, momSeed, pdg);
  //trackCand.setPosMomSeed(posSeed, momSeed, helixTrack -> Charge());

  new ((*_genfitTrackArray)[_genfitTrackArray -> GetEntriesFast()]) genfit::Track(trackCand, *_measurementFactory, new genfit::RKTrackRep(pdg));

}
