//  Authors: Wooseung Jung

//GenKEK
#include "HypTPCFitter.hh"
#include "HypTPCFitProcess.hh"
#include "HypTPCTrack.hh"

//GenFit2
//#include <KalmanFitter.h>
//#include <KalmanFitterRefTrack.h>
//#include <AbsKalmanFitter.h>
//#include <DAF.h>
//#include <FieldManager.h>
//#include <MaterialEffects.h>
//#include <TGeoMaterialInterface.h>
//#include <FitStatus.h>
#include <Track.h>
#include <Exception.h>

//k18-analyzer

#include <cassert>
#include <cstddef>
#include <iostream>

#define LogDEBUG(exp) std::cout << "DEBUG: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl
#define LogERROR(exp) std::cout << "ERROR: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl
#define LogWARNING(exp) std::cout << "WARNING: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl

ClassImp(HypTPCFitProcess)

bool HypTPCFitProcess::FitCheck(genfit::Track* fittedTrack){

  if(!fittedTrack->getFitStatus(rep)->isFitted()){
    if (verbosity >= 2) LogWARNING("Fitting is failed");
    return false;
  }
  if(!fittedTrack->getFitStatus(rep)->isFitConverged()){
    if (verbosity >= 2) LogWARNING("Fit is not converged");
    return false;
  }
  try{fittedTrack->checkConsistency();}
  catch(genfit::Exception& e){
    if(verbosity >= 2){
      std::cerr << "genfit::Track::checkConsistency() failed!" << std::endl;
      std::cerr << e.what();
    }
    return false;
  }
  return true;

}

void HypTPCTrack::FitTracks(){

  int nTracks = _genfitTrackArray -> GetEntriesFast();
  for(int i=0; i<nTracks; i++){
    ProcessTrack(_genfitTrackArray -> ConstructedAt(i));
  }

}

bool HypTPCFitProcess::ProcessTrack(genfit::Track* fitTrack){

  try{ _fitter->processTrack(fitTrack); }
  catch(genfit::Exception& e){
    if(verbosity >= 1){
      std::cerr << "genfit::HypTPCFitter::processTrack::Exception: \n";
      std::cerr << e.what();
      std::cerr << "Exception, next track" << std::endl;
    }
    return false;
  }

  if(!FitCheck(fitTrack)) return false;
  return true;

}

bool HypTPCFitProcess::ProcessTrack(HypTPCTrack* track, AbsTrackRep* rep){

  try{ _fitter->processTrackWithRep(fitTrack, rep, false); }
  catch(genfit::Exception& e){
    if(verbosity >= 1){
      std::cerr << "genfit::HypTPCFitter::processTrack::Exception: \n";
      std::cerr << e.what();
      std::cerr << "Exception, next track" << std::endl;
    }
    return false;
  }

  if(!FitCheck(fitTrack)) return false;
  return true;

}
