//  Authors: Wooseung Jung

#ifndef HYPTPCFITPROCESS_HH
#define HYPTPCFITPROCESS_HH

//GenKEK
//#include "HypTPCTrack.hh"

//GenFit2
//#include <Exception.h>

#include <string>

class HypTPCFitProcess : HypTPCTrack{

public:

  HypTPCFitProcess(){}
  ~HypTPCFitProcess(){}
  HypTPCFitProcess* GetInstance(){ return new HypTPCFitProcess(); }
  void FitTracks();
  bool FitCheck(genfit::Track* fittedTrack);
  bool ProcessTrack(genfit::Track* fitTrack);
  bool ProcessTrack(HypTPCTrack* track, AbsTrackRep* rep);

private:

  ClassDef(HypTPCFitProcess, 1)

};  //class HypTPCFitProcess

#endif // genfit_HypTPCFitProcess_hh
