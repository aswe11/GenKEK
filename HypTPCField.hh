//  Authors: Wooseung Jung

#ifndef HYPTPCFIELD_HH
#define HYPTPCFIELD_HH

class HypTPCField : public genfit::AbsBField
{
public:

  HypTPCField(bool is_constant_field=false);
  ~HypTPCField() override {}

  //return value at position
  TVector3 get(const TVector3& position) const override;

private:

  bool m_is_const;
  double const_field;

  ClassDef(HypTPCField, 1)
}; //class HypTPCField

#endif // genfit_HypTPCField_hh
