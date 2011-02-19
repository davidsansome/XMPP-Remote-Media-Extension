#include "connection.h"
#include "mediaplayerhandler.h"
#include "remotecontrolhandler.h"

#include <QSocketNotifier>
#include <QtDebug>

#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/disco.h>
#include <gloox/discohandler.h>
#include <gloox/iqhandler.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/rosterlistener.h>
#include <gloox/rostermanager.h>


struct Connection::Private : public gloox::ConnectionListener,
                             public gloox::MessageHandler,
                             public gloox::LogHandler,
                             public gloox::IqHandler,
                             public gloox::RosterListener,
                             public gloox::DiscoHandler {
  Private(Connection* parent)
    : parent_(parent),
      server_(kDefaultServer),
      jid_resource_(kDefaultJIDResource),
      jid_host_(kDefaultJIDHost),
      verbose_(false),
      media_player_(NULL),
      remote_control_(NULL),
      next_id_(1) {}

  static const char* kDefaultServer;
  static const char* kDefaultJIDResource;
  static const char* kDefaultJIDHost;
  static const char* kXmlNs;

  Connection* parent_;

  // Stuff the user sets before Connect()
  QString username_;
  QString password_;
  QString agent_name_;
  QString server_;
  QString jid_resource_;
  QString jid_host_;
  bool verbose_;

  // Interfaces
  MediaPlayerInterface* media_player_;
  RemoteControlInterface* remote_control_;
  QList<Handler*> handlers_;

  // Stuff that is valid when we're connected.
  QScopedPointer<gloox::Client> client_;
  QScopedPointer<QSocketNotifier> socket_notifier_;

  // After discovering a peer we query it to find its capabilities.  Only after
  // it replies to the query do we put it in peers_ and emit PeerFound().
  QList<Peer> peers_;
  QList<Peer> querying_peers_;

  bool has_peer(const QString& jid_resource) const;

  // Numeric ID -> XMPP ID mappings
  int next_id_;
  QMap<QString, int> id_map_;

  // gloox::MessageHandler
  void handleMessage(gloox::Stanza* stanza, gloox::MessageSession* session = 0);

  // gloox::ConnectionListener
  void onConnect();
  void onDisconnect(gloox::ConnectionError e);
  bool onTLSConnect(const gloox::CertInfo& info);

  // gloox::LogHandler
  void handleLog(gloox::LogLevel level, gloox::LogArea area,
                 const std::string& message);

  // gloox::IqHandler
  bool handleIq(gloox::Stanza* stanza);
  bool handleIqID(gloox::Stanza* stanza, int context);

  // gloox::RosterListener
  void handleItemAdded(const gloox::JID&) {}
  void handleItemSubscribed(const gloox::JID&) {}
  void handleItemRemoved(const gloox::JID&) {}
  void handleItemUpdated(const gloox::JID&) {}
  void handleItemUnsubscribed(const gloox::JID&) {}
  void handleRoster(const gloox::Roster&) {}
  void handleRosterPresence(const gloox::RosterItem& item, const std::string& resource, gloox::Presence presence, const std::string& msg);
  void handleSelfPresence(const gloox::RosterItem&, const std::string&, gloox::Presence, const std::string&) {}
  bool handleSubscriptionRequest(const gloox::JID&, const std::string&) { return false; }
  bool handleUnsubscriptionRequest(const gloox::JID&, const std::string&) { return false; }
  void handleNonrosterPresence(gloox::Stanza*) {}
  void handleRosterError(gloox::Stanza*) {}

  // gloox::DiscoHandler
  void handleDiscoInfoResult(gloox::Stanza* stanza, int context);
  void handleDiscoItemsResult(gloox::Stanza*, int) {}
  void handleDiscoError(gloox::Stanza* stanza, int context);
};

const char* Connection::Private::kDefaultServer = "talk.google.com";
const char* Connection::Private::kDefaultJIDResource = "xrmeagent";
const char* Connection::Private::kDefaultJIDHost = "gmail.com";
const char* Connection::Private::kXmlNs = "http://purplehatstands.com/xmlns/xrme";


Connection::Connection(QObject* parent)
    : QObject(parent),
      d(new Private(this)) {
}

Connection::~Connection() {
  qDeleteAll(d->handlers_);
}

void Connection::set_username(const QString& username) {
  d->username_ = username;
}

void Connection::set_password(const QString& password) {
  d->password_ = password;
}

void Connection::set_agent_name(const QString& agent_name) {
  d->agent_name_ = agent_name;
}

void Connection::set_server(const QString& server) {
  d->server_ = server;
}

void Connection::set_jid_resource(const QString& resource) {
  d->jid_resource_ = resource;
}

void Connection::set_jid_host(const QString& host) {
  d->jid_host_ = host;
}

void Connection::SetMediaPlayer(MediaPlayerInterface* interface) {
  if (d->media_player_) {
    qWarning() << "Connection::SetMediaPlayer: this connection already has a"
                  " MediaPlayerInterface set";
    return;
  }

  if (!interface) {
    qWarning() << "Connection::SetMediaPlayer called with NULL interface";
    return;
  }

  d->media_player_ = interface;
  d->handlers_ << new MediaPlayerHandler(interface);
}

void Connection::SetRemoteControl(RemoteControlInterface* interface) {
  if (d->media_player_) {
    qWarning() << "Connection::RemoteControlInterface: this connection already"
                  " has a RemoteControlInterface set";
    return;
  }

  if (!interface) {
    qWarning() << "Connection::SetRemoteControl called with NULL interface";
    return;
  }

  d->remote_control_ = interface;
  d->handlers_ << new RemoteControlHandler(interface);
}

void Connection::set_verbose(bool verbose) {
  d->verbose_ = verbose;
}

bool Connection::is_connected() const {
  return (d->client_ && d->client_->state() == gloox::StateConnected);
}

QString Connection::jid() const {
  if (is_connected()) {
    return QString::fromUtf8(d->client_->jid().full().c_str());
  }
  return QString();
}

bool Connection::Connect() {
  if (d->username_.isEmpty() || d->password_.isEmpty() || d->agent_name_.isEmpty()) {
    qWarning() << __PRETTY_FUNCTION__
               << ": A required field (username/password/agent_name) was empty";
    return false;
  }

  // Construct the JID - append the default host if the user didn't provide one
  QString jid = d->username_;
  if (!jid.contains('@')) {
    jid.append("@" + d->jid_host_);
  }
  jid.append("/" + d->jid_resource_);

  // Create a new connection
  d->client_.reset(new gloox::Client(gloox::JID(jid.toUtf8().constData()),
                                     d->password_.toUtf8().constData()));
  gloox::ConnectionTCPClient* connection = new gloox::ConnectionTCPClient(
        d->client_.data(), d->client_->logInstance(),
        d->server_.toUtf8().constData());
  d->client_->setConnectionImpl(connection);

  // Add listeners
  d->client_->registerConnectionListener(d.data());
  d->client_->registerMessageHandler(d.data());
  d->client_->registerIqHandler(d.data(), Private::kXmlNs);
  d->client_->rosterManager()->registerRosterListener(d.data());
  d->client_->logInstance().registerLogHandler(
        gloox::LogLevelDebug, gloox::LogAreaAll, d.data());

  // Setup disco
  d->client_->disco()->setIdentity("client", "bot");
  d->client_->disco()->setVersion(d->agent_name_.toUtf8().constData(), std::string());
  d->client_->disco()->addFeature(Private::kXmlNs);

  // Initialise the handlers
  foreach (Handler* handler, d->handlers_) {
    handler->Init(d->client_.data());
  }

  // Set presence
  d->client_->setPresence(gloox::PresenceAvailable, -128);

  // Connect
  if (!d->client_->connect(false)) {
    d->client_.reset();
    return false;
  }

  // Listen on the connection's socket
  d->socket_notifier_.reset(new QSocketNotifier(
        connection->socket(), QSocketNotifier::Read));
  connect(d->socket_notifier_.data(), SIGNAL(activated(int)),
          SLOT(SocketReadyReceive()));

  return true;
}

void Connection::Disconnect() {

}

void Connection::SocketReadyReceive() {
  d->client_->recv();
}

void Connection::Private::onConnect() {
  parent_->RefreshPeers();
  emit parent_->Connected();
}

void Connection::Private::handleMessage(gloox::Stanza* stanza,
                                        gloox::MessageSession* session) {
  qDebug() << Q_FUNC_INFO << stanza->xml().c_str();
}

void Connection::Private::onDisconnect(gloox::ConnectionError e) {
  foreach (Handler* handler, handlers_) {
    handler->Reset();
  }

  socket_notifier_->setEnabled(false);
  socket_notifier_.reset();
  client_.reset();
  emit parent_->Disconnected();
}

bool Connection::Private::onTLSConnect(const gloox::CertInfo& info) {
  return true;
}

void Connection::Private::handleLog(gloox::LogLevel level, gloox::LogArea area,
                     const std::string& message) {
  if (!verbose_) {
    return;
  }

  QString prefix = "---";
  if (area == gloox::LogAreaXmlIncoming) {
    prefix = "<<<";
  } else if (area == gloox::LogAreaXmlOutgoing) {
    prefix = ">>>";
  }

  qDebug() << "XMPP" << prefix.toAscii().constData() << message.c_str();
}

void Connection::RefreshPeers() {
  // Clear the lists
  d->peers_.clear();
  d->querying_peers_.clear();

  // Query presence
  qDebug() << "Sending presence query";
  d->client_->send(gloox::Stanza::createPresenceStanza(d->client_->jid().bareJID()));
}

bool Connection::Private::handleIq(gloox::Stanza* stanza) {
  qDebug() << __PRETTY_FUNCTION__ << stanza->xml().c_str();
}

bool Connection::Private::handleIqID(gloox::Stanza* stanza, int context) {
  qDebug() << __PRETTY_FUNCTION__ << context << stanza->xml().c_str();
}

Connection::Peer::Peer()
    : caps_(0) {
}

void Connection::Private::handleRosterPresence(
    const gloox::RosterItem& item, const std::string& resource,
    gloox::Presence presence, const std::string& msg) {
  if (item.jid() == client_->jid().bare()) {
    if (!has_peer(resource.c_str())) {
      qDebug() << "Got presence from" << resource.c_str();

      // This is a peer on our own bare JID, and we haven't seen it before
      gloox::JID full_jid(item.jid());
      full_jid.setResource(resource);

      Peer peer;
      peer.jid_resource_ = resource.c_str();
      querying_peers_ << peer;

      client_->disco()->getDiscoInfo(full_jid, std::string(), this, 0);
    }
  }
}

bool Connection::Private::has_peer(const QString& jid_resource) const {
  foreach (const Peer& peer, peers_) {
    if (peer.jid_resource_ == jid_resource) {
      return true;
    }
  }

  foreach (const Peer& peer, querying_peers_) {
    if (peer.jid_resource_ == jid_resource) {
      return true;
    }
  }
}

void Connection::Private::handleDiscoInfoResult(gloox::Stanza* stanza, int context) {
  // Is this from our own bare JID?
  if (stanza->from().bareJID() != client_->jid().bareJID()) {
    return;
  }

  qDebug() << "Got disco info from" << stanza->from().resource().c_str();

  // Are we currently querying this peer?
  int querying_peer_index = -1;
  for (int i=0 ; i<querying_peers_.count() ; ++i) {
    if (querying_peers_[i].jid_resource_ == stanza->from().resource().c_str()) {
      querying_peer_index = i;
      break;
    }
  }
  if (querying_peer_index == -1) {
    return;
  }

  // Remove this peer from the querying list and try to fill in his info.
  Peer peer = querying_peers_.takeAt(querying_peer_index);

  // Check for requried tags in the stanza.
  gloox::Tag* query = stanza->findChild("query");
  if (!query) {
    return;
  }

  gloox::Tag* identity = query->findChild("identity");
  gloox::Tag::TagList features = query->findChildren("feature");
  if (identity == NULL || features.size() == 0) {
    return;
  }

  // Fill in the name.
  peer.agent_name_ = identity->findAttribute("name").c_str();

  // Fill in the list of capabilities.
  foreach (gloox::Tag* feature, features) {
    const std::string feature_name = feature->findAttribute("var");
    if (feature_name == MediaPlayerHandler::kXmlNs) {
      peer.caps_ |= Peer::MediaPlayer;
    }
  }

  // No recognised capabilities?  Discard the peer.
  if (peer.caps_ == Peer::None) {
    return;
  }

  peers_ << peer;
  emit parent_->PeerFound(peer);

  qDebug() << "Peer found:" << peer.agent_name_ << peer.jid_resource_ << peer.caps_;
}

void Connection::Private::handleDiscoError(gloox::Stanza* stanza, int context) {
  // Remove this peer if we're currently querying it
  for (int i=0 ; i<querying_peers_.count() ; ++i) {
    if (querying_peers_[i].jid_resource_ == stanza->from().resource().c_str()) {
      querying_peers_.removeAt(i);
      return;
    }
  }
}
