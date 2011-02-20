package com.purplehatstands.libxrme;

import org.jivesoftware.smack.XMPPConnection;

public abstract class Handler {
  public void Init(Connection connection, XMPPConnection client) {
    connection_ = connection;
    client_ = client;
  }
  
  public void Reset() {
    connection_ = null;
    client_ = null;
  }
  
  protected Connection connection_;
  protected XMPPConnection client_;
}
