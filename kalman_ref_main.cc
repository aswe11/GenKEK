
namespace{
  const bool Const_field = true;
}
    /*!
     * Verbose control:
     * -1: Silient
     * 0: Minimum
     * 1: Errors only
     * 2: Errors and Warnings
     * 3: Verbose mode, long term debugging
     */

/*!
 * Fitter control:
 * default & 0: KalmanFitterRefTrack()
 * 1: KalmanFitter()
 * 2: genfit::DAF(true) : DafRef
 * 3: genfit::DAF(false) : DafSimple
 */


int main()
{

  //Initiallize Geometry, Field, Fitter
  HypTPCFitter* fitter = new HypTPCFitter("param/geometry/hyptpcGeo.gdml",Const_field);
  //-1: Silent, 0: Minimum, 1: Errors only, 2: Errors and Warnings, 3: Verbose mode, long term debugging(default)
  //fitter -> set_verbosity(0);

  fGenfitTest = new STGenfitTest2(fIsSamurai, fFieldXOffset, fFieldYOffset, fFieldZOffset);
  fGenfitTest -> SetTargetPlane(fTargetX*0.1, fTargetY*0.1, fTargetZ*0.1); // Target plane position unit mm -> cm

  //Initiallize the genfit track container
  HypTPCTrack* tracks = HypTPCTrack::GetInstance() -> Init();

  DCAna.TrackSearchTPCHelix();
  Int_t ntTpc = DCAna.GetNTracksTPCHelix();
  for(Int_t it=0; it<ntTpc; ++it){
    TPCLocalTrackHelix *tp = DCAna.GetTrackTPCHelix( it );
    if( !tp ) continue;

    int pid = ;
    tracks -> AddHelixTrack(pid, tp);

  }
  HypTPCFitProcess* fittedtracks = HypTPCFitProcess::GetInstance() -> FitTracks();

  return 1;
}
