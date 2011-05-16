package com.purplehatstands.libxrme;

public interface PeerDiscoveryInterface {
  public class Peer {
    public final String full_jid_;
    public final String identity_;
    
    public Peer(String full_jid, String identity) {
      full_jid_ = full_jid;
      identity_ = identity;
    }
    
    @Override
    public String toString() {
      if (identity_ != null) {
        return identity_;
      }
      return full_jid_  ;
    }
  }
  
  
  void PeerFound(Peer peer);
}
