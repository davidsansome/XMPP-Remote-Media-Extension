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

void MediaPlayerHandler::AlbumArtChanged() {
  qDebug() << __PRETTY_FUNCTION__;
}

void MediaPlayerHandler::Init(gloox::Client* client) {
  Handler::Init(client);

  client->registerIqHandler(this, kXmlNs);
  client->disco()->addFeature(kXmlNs);
}

bool MediaPlayerHandler::handleIq(gloox::Stanza* stanza) {
  gloox::Tag* query = stanza->findChild("query");
  if (!query) {
    return false;
  }

  if (stanza->hasChild("playpause")) {
    interface_->PlayPause();
  } else if (stanza->hasChild("stop")) {
    interface_->Stop();
  } else if (stanza->hasChild("previous")) {
    interface_->Previous();
  } else if (stanza->hasChild("next")) {
    interface_->Next();
  } else {
    qWarning() << "Unknown command received from"
               << stanza->from().resource().c_str()
               << stanza->xml().c_str();
    return false;
  }

  return true;
}
