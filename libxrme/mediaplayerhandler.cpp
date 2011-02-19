#include "mediaplayerhandler.h"
#include "mediaplayerinterface.h"

#include <QtDebug>

#include <gloox/client.h>
#include <gloox/disco.h>

const char* MediaPlayerHandler::kXmlNs = "http://purplehatstands.com/xmlns/xrmemediaplayer";

MediaPlayerHandler::MediaPlayerHandler(MediaPlayerInterface* interface)
    : interface_(interface) {
  interface_->Attach(this);
}

void MediaPlayerHandler::StateChanged() {
  qDebug() << __PRETTY_FUNCTION__;
}

void MediaPlayerHandler::Init(gloox::Client* client) {
  client->registerIqHandler(this, kXmlNs);
  client->disco()->addFeature(kXmlNs);
}

bool MediaPlayerHandler::handleIq(gloox::Stanza* stanza) {
  qDebug() << __PRETTY_FUNCTION__ << stanza->xml().c_str();
  return true;
}

bool MediaPlayerHandler::handleIqID(gloox::Stanza* stanza, int context) {
  qDebug() << __PRETTY_FUNCTION__ << stanza->xml().c_str();
  return true;
}
