
#ifndef HYPTPCTRACK_HH
#define HYPTPCTRACK_HH

#include <TMatrixDSymfwd.h>
#include <TVector3.h>

//STL
#include <map>
#include <memory>
#include <vector>
#include <iostream>

//GenFit

//class AbsTrackRep;
//class MeasuredStateOnPlane;
//class Track;

class HypTPCTrack : HypTPCFitter{

public:

  HypTPCTrack();
  HypTPCTrack* getInstance(){ return new HypTPCTrack(); }
  void Init();
  void AddHelixTrack(int pdg, TPCLocalTrackHelix *tp);

  genfit::Track* getTrack(int ith) { return (genfit::Track*) _hitClusterArray -> ConstructedAt(ith); }


private:

  int TPCDetID=0;
  TClonesArray *_hitClusterArray;
  TClonesArray *_genfitTrackArray;
  genfit::MeasurementFactory<genfit::AbsMeasurement> *_measurementFactory;
  genfit::MeasurementProducer<TPCLTrackHit, HypTPCSpacepointMeasurement> *_measurementProducer;

  //std::vector<unsigned int> _clusterIDs;
  //unsigned int _vertex_id;

  ClassDef(HypTPCTrack, 1)

}; //class HypTPCTrack.hh
