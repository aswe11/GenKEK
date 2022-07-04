//  Authors: Wooseung Jung

#ifndef HYPTPCFITTER_HH
#define HYPTPCFITTER_HH

//GenKEK
//#include "HypTPCTrack.hh"

//GenFit2
//#include <Exception.h>

#include <string>

class HypTPCFitter{

public:

  HypTPCFitter(const std::string& tgeo_file_name,
	       const bool m_is_const=false);
  ~HypTPCFitter(){ delete _fitter; }

  static HypTPCFitter* GetInstance(const std::string& tgeo_file_name,
				   const bool m_is_const = false);
  int get_verbosity() const { return verbosity; }
  void set_verbosity(int v){
    this->verbosity = v;
    if(verbosity >= 1) genfit::Exception::quiet(false);
    else genfit::Exception::quiet(true);
  }

private:
  /*!
   * Verbose control:
   * -1: Silent
   * 0: Minimum
   * 1: Errors only
   * 2: Errors and Warnings
   * 3: Verbose mode, long term debugging
   */
  int verbosity;
  /*!
   * Fitter control:
   * default & 0: KalmanFitterRefTrack()
   * 1: KalmanFitter()
   * 2: genfit::DAF(true) : DafRef
   * 3: genfit::DAF(false) : DafSimple
   */
  int GenFitFitter;
  TGeoManager* _tgeo_manager;
  genfit::AbsKalmanFitter* _fitter;

  ClassDef(HypTPCFitter, 1)

};  //class HypTPCFitter

#endif // genfit_HypTPCFitter_hh
