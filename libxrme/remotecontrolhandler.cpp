/* This file is part of Clementine.
   Copyright 2010, David Sansome <me@davidsansome.com>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mediaplayerhandler.h"
#include "remotecontrolinterface.h"
#include "remotecontrolhandler.h"

#include <QtDebug>

#include <gloox/client.h>
#include <gloox/disco.h>

const char* RemoteControlHandler::kXmlNs = "http://purplehatstands.com/xmlns/xrmeremotecontrol";

RemoteControlHandler::RemoteControlHandler(RemoteControlInterface* interface)
    : interface_(interface) {
  interface_->Attach(this);
}

void RemoteControlHandler::Init(gloox::Client* client) {
  Handler::Init(client);

  client->registerIqHandler(this, kXmlNs);
  client->disco()->addFeature(kXmlNs);
}

void RemoteControlHandler::SendIQ(const QString& jid_resource,
                                  gloox::StanzaSubType type,
                                  const QString& command) {
  if (!client_) {
    return;
  }

  gloox::JID to(client_->jid());
  to.setResource(jid_resource.toUtf8().constData());

  gloox::Tag* stanza = gloox::Stanza::createIqStanza(
        to, client_->getID(), type, MediaPlayerHandler::kXmlNs);
  new gloox::Tag(stanza, command.toUtf8().constData());

  client_->send(stanza);
}

void RemoteControlHandler::PlayPause(const QString& jid_resource) {
  SendIQ(jid_resource, gloox::StanzaIqSet, "playpause");
}

void RemoteControlHandler::Stop(const QString& jid_resource) {
  SendIQ(jid_resource, gloox::StanzaIqSet, "stop");
}

void RemoteControlHandler::Next(const QString& jid_resource) {
  SendIQ(jid_resource, gloox::StanzaIqSet, "next");
}

void RemoteControlHandler::Previous(const QString& jid_resource) {
  SendIQ(jid_resource, gloox::StanzaIqSet, "previous");
}

void RemoteControlHandler::QueryState(const QString& jid_resource) {
  SendIQ(jid_resource, gloox::StanzaIqGet, "querystate");
}

bool RemoteControlHandler::handleIq(gloox::Stanza* stanza) {
  qDebug() << __PRETTY_FUNCTION__ << stanza->xml().c_str();
  return true;
}

bool RemoteControlHandler::handleIqID(gloox::Stanza* stanza, int context) {
  qDebug() << __PRETTY_FUNCTION__ << stanza->xml().c_str();
  return true;
}
