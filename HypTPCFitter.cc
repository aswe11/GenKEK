//  Authors: Wooseung Jung

//GenKEK
#include "HypTPCFitter.hh"
#include "HypTPCField.hh"
#include "HypTPCTrack.hh"

//ROOT
#include <TGeoManager.h>
#include <RVersion.h>                      // for ROOT_VERSION, ROOT_VERSION...

//GenFit2
#include <KalmanFitter.h>
#include <KalmanFitterRefTrack.h>
#include <AbsKalmanFitter.h>
#include <DAF.h>
#include <FieldManager.h>
#include <FitStatus.h>
#include <MaterialEffects.h>
#include <TGeoMaterialInterface.h>
#include <Track.h>
#include <Exception.h>

//k18-analyzer
#include <UserParamMan.hh>

#include <cassert>
#include <cstddef>
#include <iostream>

#define LogDEBUG(exp) std::cout << "DEBUG: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl
#define LogERROR(exp) std::cout << "ERROR: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl
#define LogWARNING(exp) std::cout << "WARNING: " << __FILE__ << ": " << __LINE__ << ": " << exp << std::endl

namespace{ const auto& gUser = UserParamMan::GetInstance(); }

ClassImp(HypTPCFitter)

HypTPCFitter::HypTPCFitter(const std::string& tgeo_file_name, const bool m_is_const)
: verbosity(3),GenFitFitter(-1)
{
  _tgeo_manager = new TGeoManager("Geometry", "HypTPC geometry");
  TGeoManager::Import(tgeo_file_name.data());

  genfit::FieldManager::getInstance()->init(new HypTPCField(m_is_const));
  genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());

  const int GenFitFitter = gUser.GetParameter("Fitter");
  const unsigned int MinIter = gUser.GetParameter("nIteration",0);
  const unsigned int MaxIter = gUser.GetParameter("nIteration",1);

  // init fitter
  if(GenFitFitter==0) _fitter = new genfit::KalmanFitterRefTrack();
  else if(GenFitFitter==1) _fitter = new genfit::KalmanFitter();
  else if(GenFitFitter==2) _fitter = new genfit::DAF(true);
  else if(GenFitFitter==3) _fitter = new genfit::DAF(false);
  else _fitter = new genfit::KalmanFitterRefTrack();
  _fitter -> setMaxIterations(MaxIter);
  _fitter -> setMinIterations(MinIter);

  genfit::Exception::quiet(true);
}

HypTPCFitter* HypTPCFitter::GetInstance(const std::string& tgeo_file_name, const bool m_is_const = false, const std::string& track_rep_choice){

  return new HypTPCFitter(tgeo_file_name, m_is_const);
}
