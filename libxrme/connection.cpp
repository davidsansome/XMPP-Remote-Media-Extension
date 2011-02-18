#include "connection.h"

#include <QSocketNotifier>
#include <QtDebug>

#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>


struct Connection::Private : public gloox::ConnectionListener,
                             public gloox::MessageHandler,
                             public gloox::LogHandler {
  Private(Connection* parent)
    : parent_(parent),
      server_(kDefaultServer),
      jid_resource_(kDefaultJIDResource),
      jid_host_(kDefaultJIDHost) {}

  static const char* kDefaultServer;
  static const char* kDefaultJIDResource;
  static const char* kDefaultJIDHost;

  Connection* parent_;

  QString username_;
  QString password_;
  QString server_;
  QString jid_resource_;
  QString jid_host_;

  QScopedPointer<gloox::Client> client_;
  QScopedPointer<QSocketNotifier> socket_notifier_;

  // gloox::MessageHandler
  void handleMessage(gloox::Stanza* stanza, gloox::MessageSession* session = 0);

  // gloox::ConnectionListener
  void onConnect();
  void onDisconnect(gloox::ConnectionError e);
  bool onTLSConnect(const gloox::CertInfo& info);

  // gloox::LogHandler
  void handleLog(gloox::LogLevel level, gloox::LogArea area,
                 const std::string& message);
};

const char* Connection::Private::kDefaultServer = "talk.google.com";
const char* Connection::Private::kDefaultJIDResource = "xmreagent";
const char* Connection::Private::kDefaultJIDHost = "gmail.com";


Connection::Connection(QObject* parent)
    : QObject(parent),
      d(new Private(this)) {
}

Connection::~Connection() {
}

void Connection::set_username(const QString& username) {
  d->username_ = username;
}

void Connection::set_password(const QString& password) {
  d->password_ = password;
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

QString Connection::jid() const {
  if (d->client_ && d->client_->state() == gloox::StateConnected) {
    return QString::fromUtf8(d->client_->jid().full().c_str());
  }
  return QString();
}

void Connection::Connect() {
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
  d->client_->logInstance().registerLogHandler(
        gloox::LogLevelDebug, gloox::LogAreaAll, d.data());

  // Set presence
  d->client_->setPresence(gloox::PresenceUnavailable, -128);

  // Connect
  d->client_->connect(false);

  // Listen on the connection's socket
  d->socket_notifier_.reset(new QSocketNotifier(
        connection->socket(), QSocketNotifier::Read));
  connect(d->socket_notifier_.data(), SIGNAL(activated(int)),
          SLOT(SocketReadyReceive()));
}

void Connection::Disconnect() {

}

void Connection::SocketReadyReceive() {
  d->client_->recv();
}

void Connection::Private::onConnect() {
  emit parent_->Connected();
  client_->login();
}

void Connection::Private::handleMessage(gloox::Stanza* stanza,
                                        gloox::MessageSession* session) {
  qDebug() << Q_FUNC_INFO << stanza->xml().c_str();
}

void Connection::Private::onDisconnect(gloox::ConnectionError e) {
  qDebug() << "Disconnected:" << e;
  socket_notifier_->setEnabled(false);
}

bool Connection::Private::onTLSConnect(const gloox::CertInfo& info) {
  return true;
}

void Connection::Private::handleLog(gloox::LogLevel level, gloox::LogArea area,
                     const std::string& message) {
  QString prefix = "---";
  if (area == gloox::LogAreaXmlIncoming) {
    prefix = "<<<";
  } else if (area == gloox::LogAreaXmlOutgoing) {
    prefix = ">>>";
  }

  //qDebug() << "XMPP" << prefix.toAscii().constData() << message.c_str();
}

