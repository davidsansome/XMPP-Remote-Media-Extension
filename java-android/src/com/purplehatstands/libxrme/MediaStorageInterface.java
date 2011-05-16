package com.purplehatstands.libxrme;

public abstract class MediaStorageInterface {
  public void GetArtists(String peer_jid_resource) {
    if (handler_ != null) {
      handler_.GetArtists(peer_jid_resource);
    }
  }
  
  public void Attach(MediaStorageHandler handler) {
    handler_ = handler;
  }

  private MediaStorageHandler handler_;
}
