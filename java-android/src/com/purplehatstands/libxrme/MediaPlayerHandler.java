package com.purplehatstands.libxrme;

import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smackx.ServiceDiscoveryManager;

public class MediaPlayerHandler extends Handler {
  public MediaPlayerHandler(MediaPlayerInterface media_player) {
    iface_ = media_player;
  }
  
  public void Init(Connection connection, XMPPConnection client) {
    super.Init(connection, client);
    
    ServiceDiscoveryManager discoManager = ServiceDiscoveryManager.getInstanceFor(client);
    discoManager.addFeature(Common.XMLNS_XRME_MEDIAPLAYER);
  }

  public void AlbumArtChanged() {
    // TODO Auto-generated method stub
    
  }

  public void StateChanged() {
    // TODO Auto-generated method stub
    
  }

  MediaPlayerInterface iface_;
}
