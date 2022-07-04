//  Authors: Wooseung Jung

//GenFit2
#include <AbsBField.h>

//GenKEK
#include "HypTPCField.h"

//k18-analyzer
#include "ConfMan.hh"
#include "FieldMan.hh"
#include "ThreeVector.hh"

//ROOT
#include <TVector3.h>

using namespace std;

namespace{
  const auto& gField  = FieldMan::GetInstance();
  const auto& HS_field_0 = 0.9860;
  const auto& valueHSHall = ConfMan::Get<Double_t>("HSFLDHALL");
  const auto& valueHSCalc = ConfMan::Get<Double_t>("HSFLDCALC");
}

// Interface of the K18 B-field map with GenFit

ClassImp(HypTPCField)

HypTPCField::HypTPCField(bool is_constant_field=false)
: genfit::AbsBField(),
  m_is_const(is_constant_field),
  const_field(HS_field_0*HS_field_Hall/HS_field_Hall_calc)
{}

/* Getter for the magnetic field.
 *
 *  As Genfit uses kGauss, but we use T, we need to apply a factor 10 in the calculation.
 *  @param position   Position at which the magnetic field should be evaluated.
 */

TVector3 HypTPCField::get(const TVector3& position) const{
  TVector3 B;
  if(is_constant_field) B = ThreeVector(0.,0.,const_field);
  else B = gField.GetField(position);

  return B;
}
