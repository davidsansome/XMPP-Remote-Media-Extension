package com.purplehatstands.libxrme;

public abstract class MediaPlayerInterface {
  public abstract void PlayPause();
  public abstract void Stop();
  public abstract void Next();
  public abstract void Previous();
  
  public abstract State state();
  // TODO public abstract QImage album_art();
  
  protected void StateChanged() {
    if (handler_ != null) {
      handler_.StateChanged();
    }
  }
  
  protected void AlbumArtChanged() {
    if (handler_ != null) {
      handler_.AlbumArtChanged();
    }
  }
  
  public void Attach(MediaPlayerHandler handler) {
    handler_ = handler;
  }
  
  private MediaPlayerHandler handler_;
}
