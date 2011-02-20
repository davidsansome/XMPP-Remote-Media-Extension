package com.purplehatstands.libxrme;

import java.util.List;

import org.jivesoftware.smack.PacketListener;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.filter.PacketTypeFilter;
import org.jivesoftware.smack.packet.IQ;
import org.jivesoftware.smack.packet.Packet;
import org.jivesoftware.smack.packet.XMPPError;
import org.jivesoftware.smackx.NodeInformationProvider;
import org.jivesoftware.smackx.ServiceDiscoveryManager;
import org.jivesoftware.smackx.packet.DiscoverInfo;

public class RemoteControlHandler extends Handler {
  public RemoteControlHandler(RemoteControlInterface remote_control) {
    iface_ = remote_control;
  }

  public void Init(Connection connection, XMPPConnection client) {
    super.Init(connection, client);
    
    // Advertise this service on the disco manager
    ServiceDiscoveryManager discoManager = ServiceDiscoveryManager.getInstanceFor(client);
    discoManager.addFeature(Common.XMLNS_XRME_REMOTECONTROL);
    
    // Listen for state IQs
    PacketTypeFilter filter = new PacketTypeFilter(State.class);
    PacketListener listener = new PacketListener() {
      public void processPacket(Packet packet) {
        State state = (State) packet;
        
        if (state != null && state.getType() == IQ.Type.SET) {
          String resource = state.getFrom();
          resource = resource.substring(resource.indexOf("/") + 1);
          iface_.StateChanged(resource, state);
        }
      }
    };
    client.addPacketListener(listener, filter);
    
    // Listen for album art IQs
    filter = new PacketTypeFilter(AlbumArt.class);
    listener = new PacketListener() {
      public void processPacket(Packet packet) {
        AlbumArt art = (AlbumArt) packet;
        
        if (art != null && art.getType() == IQ.Type.SET) {
          String resource = art.getFrom();
          resource = resource.substring(resource.indexOf("/") + 1);
          iface_.AlbumArtChanged(resource, art.image);
        }
      }
    };
    client.addPacketListener(listener, filter);
  }

  public void PlayPause(String peer_jid_resource) {
    // TODO Auto-generated method stub
    
  }

  public void Stop(String peer_jid_resource) {
    // TODO Auto-generated method stub
    
  }

  public void Next(String peer_jid_resource) {
    // TODO Auto-generated method stub
    
  }

  public void Previous(String peer_jid_resource) {
    // TODO Auto-generated method stub
    
  }

  public void QueryState(String peer_jid_resource) {
    // TODO Auto-generated method stub
    
  }

  private RemoteControlInterface iface_;
}
