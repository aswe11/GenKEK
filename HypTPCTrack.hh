//  Authors: Wooseung Jung

#ifndef HYPTPCTRACK_HH
#define HYPTPCTRACK_HH

#include <TMatrixDSymfwd.h>
#include <TVector3.h>

//STL
#include <map>
#include <memory>
#include <vector>
#include <iostream>

//GenKEK
#include "HypTPCSpacepointMeasurement.hh"
#include "HypTPCFitter.hh"
#include "HypTPCFitProcess.hh"

//k18-analyzer
#include "TPCLocalTrackHelix.hh"
#include "TPCLocalTrack.hh"

//GenFit
#include <AbsMeasurement.h>
#include <Track.h>
#include <MeasurementFactory.h>
#include <MeasurementProducer.h>

//ROOT
#include <TClonesArray.h>
//class AbsTrackRep;
//class MeasuredStateOnPlane;
//class Track;

//class HypTPCFitter;
//class HypTPCFitProcess;

class HypTPCTrack: public HypTPCFitter, public HypTPCFitProcess{

public:

  HypTPCTrack();
  HypTPCTrack* getInstance(){ return new HypTPCTrack(); }
  void Init();
  void AddHelixTrack(int pdg, TPCLocalTrackHelix *tp);


  genfit::Track* GetTrack(int ith){ return (genfit::Track*) _genfitTrackArray -> ConstructedAt(ith); }

  /*!
   * Verbose control:
   * -1: Silent, 0: Minimum
   * 1: Errors only, 2: Errors and Warnings
   * 3: Verbose mode, long term debugging (default)
   */
  int Get_verbosity() const override;
  void Set_verbosity(int v) override;
  void FitTracks() override;
  //Process all AbsTrackReps.
  bool ProcessTrack(genfit::Track* Track) override;
  bool FitCheck(genfit::Track* fittedTrack) override; //check fitstatus of CardinalRep
  //Process Track with one AbsTrackRep of the Track
  bool ProcessTrack(genfit::Track* Track, genfit::AbsTrackRep* rep) override;
  bool FitCheck(genfit::Track* fittedTrack, genfit::AbsTrackRep* rep) override;

  static TClonesArray *_hitClusterArray;
  static TClonesArray *_genfitTrackArray;

private:

  int TPCDetID=0;
  genfit::MeasurementFactory<genfit::AbsMeasurement> *_measurementFactory;
  genfit::MeasurementProducer<TPCLTrackHit, HypTPCSpacepointMeasurement> *_measurementProducer;

  ClassDef(HypTPCTrack, 1)

}; //class HypTPCTrack.hh

#endif // HypTPCTrack_hh
