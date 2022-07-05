//  Authors: Wooseung Jung

#ifndef HYPTPCFITPROCESS_HH
#define HYPTPCFITPROCESS_HH

//GenFit
#include <Track.h>
#include <AbsTrackRep.h>
#include <Exception.h>

class HypTPCFitProcess{

public:

  HypTPCFitProcess(): verbosity(3){}
  virtual ~HypTPCFitProcess(){}
  HypTPCFitProcess* GetInstance(){ return new HypTPCFitProcess(); }

  virtual int Get_verbosity() const { return verbosity; }
  virtual void Set_verbosity(int v){
    this->verbosity = v;
    if(verbosity >= 1) genfit::Exception::quiet(false);
    else genfit::Exception::quiet(true);
  }

  virtual void FitTracks();

  //Process all AbsTrackReps.
  virtual bool ProcessTrack(genfit::Track* Track);
  virtual bool FitCheck(genfit::Track* fittedTrack); //check fitstatus of CardinalRep

  //Process Track with one AbsTrackRep of the Track
  virtual bool ProcessTrack(genfit::Track* Track, genfit::AbsTrackRep* rep);
  virtual bool FitCheck(genfit::Track* fittedTrack, genfit::AbsTrackRep* rep);

private:

  int verbosity;
  ClassDef(HypTPCFitProcess, 1)

};  //class HypTPCFitProcess

#endif // genfit_HypTPCFitProcess_hh
