package com.purplehatstands.libxrme;

import android.graphics.Bitmap;

public abstract class RemoteControlInterface {
  public abstract void StateChanged(String peer_jid_resource, State state);
  public abstract void AlbumArtChanged(String peer_jid_resource, Bitmap image);
  
  protected void PlayPause(String peer_jid_resource) {
    if (handler_ != null) {
      handler_.PlayPause(peer_jid_resource);
    }
  }
  
  protected void Stop(String peer_jid_resource) {
    if (handler_ != null) {
      handler_.Stop(peer_jid_resource);
    }
  }
  
  protected void Next(String peer_jid_resource) {
    if (handler_ != null) {
      handler_.Next(peer_jid_resource);
    }
  }

  protected void Previous(String peer_jid_resource) {
    if (handler_ != null) {
      handler_.Previous(peer_jid_resource);
    }
  }
  
  protected void QueryState(String peer_jid_resource) {
    if (handler_ != null) {
      handler_.QueryState(peer_jid_resource);
    }
  }
  
  public void Attach(RemoteControlHandler handler) {
    handler_ = handler;
  }
  
  private RemoteControlHandler handler_;
}
