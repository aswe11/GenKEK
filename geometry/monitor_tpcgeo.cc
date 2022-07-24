void monitor_tpcgeo(){

  TGeoManager::Import("ver.2/hyptpcGeo.gdml");
  gGeoManager -> GetTopVolume() -> Draw("ogl");

}
