/** @brief Example class for a spacepoint measurement which can be created
 * from mySpacepointDetectorHit via the MeasurementFactory.
 *
 *  @author Johannes Rauch  (Technische Universit&auml;t M&uuml;nchen, original author)
 *
 */
//  Rearranged by: Wooseung Jung

#ifndef HYPTPCSPACEPOINTMEASUREMENT_HH
#define HYPTPCSPACEPOINTMEASUREMENT_HH

//k18-analyzer
//#include "TPCLTrackHit.hh"

class TVector3;

class HypTPCSpacepointMeasurement : public genfit::SpacepointMeasurement {

public:
  HypTPCSpacepointMeasurement() : genfit::SpacepointMeasurement() {}
  HypTPCSpacepointMeasurement(const TPCLTrackHit* dethit, const genfit::TrackCandHit* hit); //TPCHit Hitpos & Resolution

  virtual HypTPCSpacepointMeasurement* clone() const { return new HypTPCSpacepointMeasurement(*this); }
  double GetCharge() { return fCharge; }

private:
  double fCharge;

  ClassDef(HypTPCSpacepointMeasurement,1)
};

#endif // genfit_HypTPCSpacepointMeasurement_hh
