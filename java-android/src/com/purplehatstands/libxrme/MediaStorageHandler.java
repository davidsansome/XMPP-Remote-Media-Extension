package com.purplehatstands.libxrme;

import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.packet.IQ;
import org.jivesoftware.smack.packet.Packet;
import org.jivesoftware.smackx.ServiceDiscoveryManager;

import android.util.Log;

public class MediaStorageHandler extends Handler {
  private static final String TAG = "MediaStorageHandler";
  
  public MediaStorageHandler(MediaStorageInterface iface) {
    Log.d(TAG, "Constructor");
    iface_ = iface;
    iface_.Attach(this);
  }
  
  @Override
  public void Init(Connection connection, XMPPConnection client) {
    Log.d(TAG, "Init");
    super.Init(connection, client);
    
    ServiceDiscoveryManager discoManager = ServiceDiscoveryManager.getInstanceFor(client);
    discoManager.addFeature(Common.XMLNS_XRME_MEDIASTORAGE);
  }
  
  public void GetArtists(String peer_jid_resource) {
    Log.d(TAG, "Sending GetArtists()");
    Packet iqPacket = new MediaStoragePacket("get_artists");
    iqPacket.setTo(peer_jid_resource);
    client_.sendPacket(iqPacket);
  }
  
  private class MediaStoragePacket extends IQ {
    private final String command_;

    MediaStoragePacket(String command) {
      command_ = command;
      setType(IQ.Type.SET);
    }

    @Override
    public String getChildElementXML() {
      return "<xrme xmlns=\"" + Common.XMLNS_XRME_MEDIASTORAGE + "\">" + "<"
          + command_ + "/></xrme>";

    }

  }
  
  MediaStorageInterface iface_;
}
