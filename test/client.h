#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class Connection;

class Client : public QObject {
  Q_OBJECT

public:
  Client(const QString& username, const QString& password);

public slots:
  void GetPeers();

private:
  Connection* connection_;
};

#endif // CLIENT_H
