//  Authors: Wooseung Jung

//GenKEK
#include "HypTPCHit.hh"

//k18-analyzer
#include "TPCLTrackHit.hh"

ClassImp(HypTPCHit)

HypTPCHit::HypTPCHit(TPCLTrackHit& right)
  : m_hit(right)
{std::cout<<"hyptpchit const1"<<std::endl;}

HypTPCHit::~HypTPCHit(){}

TPCLTrackHit &HypTPCHit::GetHit() const{
  std::cout<<"hyptpchit gethit"<<std::endl;
  return m_hit;
}
