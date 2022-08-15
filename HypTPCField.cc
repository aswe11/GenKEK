//  Authors: Wooseung Jung

//GenKEK
#include "HypTPCField.hh"

//k18-analyzer
#include "ConfMan.hh"
#include "FieldMan.hh"
#include "DCGeomMan.hh"
#include "ThreeVector.hh"

using namespace std;

namespace{
  const auto& gField  = FieldMan::GetInstance();
  const auto& valueHSHall = ConfMan::Get<Double_t>("HSFLDHALL");
  const auto&    gGeom = DCGeomMan::GetInstance();
  const int HSid = gGeom.DetectorId("HS");
}

// Interface of the K18 B-field map with GenFit

ClassImp(HypTPCField)

HypTPCField::HypTPCField(bool is_constant_field)
: genfit::AbsBField(),
  m_is_const(is_constant_field),
  const_field(-1*valueHSHall)
{}

//GenFit Units : GeV/c, ns, cm, kGauss
//K1.8Ana Units : GeV/c, ns, mm, T
TVector3 HypTPCField::get(const TVector3& position) const{

  TVector3 B;
  ThreeVector pos(10*position.X(),10*position.Y(),10*position.Z());  //cm -> mm
  ThreeVector posGlobal = gGeom.Local2GlobalPos(HSid,pos);
  if(m_is_const) B = TVector3(0.,const_field,0.);
  else B = gField.GetField(pos);

  return 10*B; //T -> kGauss
}
