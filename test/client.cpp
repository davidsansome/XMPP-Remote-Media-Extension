#include "connection.h"
#include "client.h"

#include <QCoreApplication>
#include <QObject>
#include <QStringList>
#include <QTimer>

Client::Client(const QString& username, const QString& password)
    : connection_(new Connection(this)) {
  connection_->set_username(username);
  connection_->set_password(password);
  connection_->set_agent_name("Test client");
  //connection_->set_verbose(true);

  if (!connection_->Connect()) {
    QCoreApplication::exit(1);
  }
}

void Client::GetPeers() {
}

int main(int argc, char** argv) {
  QCoreApplication a(argc, argv);

  if (a.arguments().count() != 3) {
    qFatal("Usage: %s username password", argv[0]);
  }

  Client c(a.arguments()[1], a.arguments()[2]);
  return a.exec();
}
