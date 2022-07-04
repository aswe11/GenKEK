//  Authors: Wooseung Jung

//GenKEK
#include "HypTPCFitter.hh"
#include "HypTPCFitProcess.hh"

//GenFit2
//#include <KalmanFitter.h>
//#include <KalmanFitterRefTrack.h>
//#include <AbsKalmanFitter.h>
//#include <DAF.h>
//#include <FieldManager.h>
//#include <MaterialEffects.h>
//#include <TGeoMaterialInterface.h>
//#include <FitStatus.h>

//k18-analyzer

//STL
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

#define LogDEBUG(exp) std::cout << "DEBUG: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl
#define LogERROR(exp) std::cout << "ERROR: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl
#define LogWARNING(exp) std::cout << "WARNING: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl

ClassImp(HypTPCFitProcess)

namespace{
  genfit::AbsKalmanFitter* _fitter = HypTPCFitter::GetFitter();
}

bool HypTPCFitProcess::FitCheck(genfit::Track* fittedTrack, genfit::AbsTrackRep* rep){

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

bool HypTPCFitProcess::FitCheck(genfit::Track* fittedTrack){

  if(!fittedTrack->getFitStatus()->isFitted()){
    if (verbosity >= 2) LogWARNING("Fitting is failed");
    return false;
  }
  if(!fittedTrack->getFitStatus()->isFitConverged()){
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

void HypTPCFitProcess::FitTracks(){

  int nTracks = HypTPCTrack::_genfitTrackArray -> GetEntriesFast();
  for(int i=0; i<nTracks; i++){
    ProcessTrack((genfit::Track*) HypTPCTrack::_genfitTrackArray -> ConstructedAt(i));
  }

}

bool HypTPCFitProcess::ProcessTrack(genfit::Track* Track){

  try{ HypTPCFitter::_fitter->processTrack(Track); }
  catch(genfit::Exception& e){
    if(verbosity >= 1){
      std::cerr << "HypTPCFitter::processTrack::Exception: \n";
      std::cerr << e.what();
      std::cerr << "Exception, next track" << std::endl;
    }
    return false;
  }

  if(!FitCheck(Track)) return false;
  return true;

}

bool HypTPCFitProcess::ProcessTrack(genfit::Track* Track, genfit::AbsTrackRep* rep){

  try{ HypTPCFitter::_fitter->processTrackWithRep(Track, rep, false); }
  catch(genfit::Exception& e){
    if(verbosity >= 1){
      std::cerr << "HypTPCFitter::processTrack::Exception: \n";
      std::cerr << e.what();
      std::cerr << "Exception, next track" << std::endl;
    }
    return false;
  }

  if(!FitCheck(Track, rep)) return false;
  return true;

}
