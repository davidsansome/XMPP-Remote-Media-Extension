#ifndef LIBXMRE_CONNECTION_H
#define LIBXMRE_CONNECTION_H

#include <QObject>
#include <QScopedPointer>
#include <QString>

class Connection : public QObject {
  Q_OBJECT

public:
  Connection(QObject* parent = NULL);
  ~Connection();

  // The username and password must be set before calling Connect()
  void set_username(const QString& username);
  void set_password(const QString& password);

  // Sets the hostname of the XMPP server to connect to.  Defaults to
  // "talk.google.com".
  void set_server(const QString& server);

  // Sets the resource string to use in the JID.  Defaults to "xmreagent".
  void set_jid_resource(const QString& resource);

  // Sets the host part to append to the JID if the user didn't specify one in
  // the username.  The host part is the part of the JID after the @ and before
  // the /, eg: "username@host/resource".  Defaults to "gmail.com".
  void set_jid_host(const QString& host);

  // Returns the user's actual JID.  This is only valid after Connected() is
  // emitted.  Before the connection is complete it will return a null QString.
  QString jid() const;

public slots:
  // Starts connecting and returns immediately.  Will emit Connected() or
  // Disconnected() later.  The username and password must already be set.
  void Connect();

  // Disconnects immediately and emits Disconnected().
  void Disconnect();

signals:
  void Connected();
  void Disconnected();

private slots:
  void SocketReadyReceive();

private:
  struct Private;
  friend struct Private;
  QScopedPointer<Private> d;
};

#endif
