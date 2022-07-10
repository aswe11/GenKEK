//  Authors: Wooseung Jung

#ifndef HYPTPCSPACEPOINTMEASUREMENT_HH
#define HYPTPCSPACEPOINTMEASUREMENT_HH

//GenFit2
#include <SpacepointMeasurement.h>
#include <TrackCandHit.h>

//k18-analyzer
#include "TPCLTrackHit.hh"
#include "HypTPCHit.hh"

//class TVector3;

namespace genfit {

  class HypTPCSpacepointMeasurement : public SpacepointMeasurement{

  public:
    HypTPCSpacepointMeasurement() : SpacepointMeasurement() {std::cout<<"constructor1"<<std::endl;}
    HypTPCSpacepointMeasurement(const HypTPCHit* dethit, const TrackCandHit* hit); //TPCHit Hitpos & Resolution
    ~HypTPCSpacepointMeasurement(){}

    virtual AbsMeasurement* clone() const { return new HypTPCSpacepointMeasurement(*this); }
    double GetCharge() { return fCharge; }

  private:
    double fCharge;

    ClassDef(HypTPCSpacepointMeasurement,1)
  };

} /* End of namespace genfit */
#endif // HypTPCSpacepointMeasurement_hh
