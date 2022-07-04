//  Authors: Wooseung Jung

//GenKEK
#include "HypTPCTrack.hh"
#include "HypTPCSpacepointMeasurement.hh"

//GenFit2
#include <AbsFitterInfo.h>
#include <AbsHMatrix.h>
#include <AbsMeasurement.h>
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
#include <Track.h>
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

ClassImp(HypTPCTrack)

HypTPCTrack::HypTPCTrack(){

  _hitClusterArray = new TClonesArray("TPCLTrackHit");
  _genfitTrackArray = new TClonesArray("genfit::Track");

  _measurementProducer = new genfit::MeasurementProducer<TPCLTrackHit, genfit::HypTPCSpacepointMeasurement>(_hitClusterArray);
  _measurementFactory = new genfit::MeasurementFactory<genfit::AbsMeasurement>();
  _measurementFactory -> addProducer(TPCDetID, _measurementProducer);
}

void HypTPCTrack::Init(){
  fHitClusterArray -> Delete();
  fGenfitTrackArray -> Delete();
}

void HypTPCTrack::AddHelixTrack(int pdg, TPCLocalTrackHelix *tp){

  _hitClusterArray -> Delete();
  genfit::TrackCand trackCand;

  int nMeasurement = tp -> GetNHit();
  for(int i=0; i<nMeasurement; i++){
    new ((*_hitClusterArray)[i]) tp -> GetHit(i);
    trackCand.addHit(TPCDetID, i);
  }

  //GenFit Units : GeV/c, cm, kGauss
  //const TVector3& pos = dethit -> GetLocalHitPos();
  const TVector3& res_vect = dethit -> GetHit(0) -> GetResolutionVect();
  TMatrixDSym covSeed(6);
  covSeed.Zero();
  covSeed(0, 0) = res_vect.X() * res_vect.X()/100.;
  covSeed(1, 1) = res_vect.Y() * res_vect.Y()/100.;
  covSeed(2, 2) = res_vect.Z() * res_vect.Z()/100.;
  for (int iComp = 3; iComp < 6; iComp++)
    covSeed(iComp, iComp) = covSeed(iComp - 3, iComp - 3)
      /(3*nMeasurement*nMeasurement);

  // set start values and pdg to cand
  trackCand.setCovSeed(covSeed);
  //trackCand.setPosMomSeedAndPdgCode(posM, momM, pdg);
  //trackCand.setPosMomSeed(posSeed, momSeed, helixTrack -> Charge());

  iiiiiiiiiiiiiiiiiii;

  new ((*_genfitTrackArray)[_genfitTrackArray -> GetEntriesFast()]) genfit::Track(trackCand, *_measurementFactory, new genfit::RKTrackRep(pdg));

}
