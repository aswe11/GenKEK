/* Copyright 2008-2010, Technische Universitaet Muenchen,
   Authors: Christian Hoeppner & Sebastian Neubert & Johannes Rauch

   This file is part of GENFIT.

   GENFIT is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   GENFIT is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with GENFIT.  If not, see <http://www.gnu.org/licenses/>.
*/
/*!
 *  \file		SpacepointMeasurement.cc
 *  \brief		Handles the palnar type of measurements.
 *  \author		Haiwang Yu <yuhw@nmsu.edu>
 */
//  Rearranged by: Wooseung Jung

//GenKEK
#include "HypTPCSpacepointMeasurement.h"

//k18-analyzer
//#include "TPCHit.hh"
#include "TPCLTrackHit.hh"

//GenFit2
#include <SpacepointMeasurement.h>
#include <TrackCandHit.h>

//ROOT
#include <TMatrixDSymfwd.h>                // for TMatrixDSym
#include <TVector3.h>

ClassImp(HypTPCSpacepointMeasurement)

HypTPCSpacepointMeasurement::HypTPCSpacepointMeasurement(const TPCLTrackHit* dethit, const genfit::TrackCandHit* hit)
: genfit::SpacepointMeasurement()
{
  const TVector3& res_vect = dethit -> GetResolutionVect();
  const TVector3& pos = dethit -> GetLocalHitPos();

  //GenFit Units : GeV/c, cm, kGauss
  int nDim = 3;
  TMatrixDSym hitCov(nDim);
  hitCov.Zero();
  hitCov(0, 0) = res_vect.X() * res_vect.X()/100.;
  hitCov(1, 1) = res_vect.Y() * res_vect.Y()/100.;
  hitCov(2, 2) = res_vect.Z() * res_vect.Z()/100.;

  TVector3 pos = dethit->GetPosition();
  rawHitCoords_(0) = pos.X()/10.;
  rawHitCoords_(1) = pos.Y()/10.;
  rawHitCoords_(2) = pos.Z()/10.;

  rawHitCov_ = cov;
  detId_ = hit -> getDetId();
  hitId_ = hit -> getHitId();

  //TPCHit *trackhit =  detHit -> GetHit();
  //fCharge = trackhit -> GetCDe();

  this -> initG();
}
