#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

#include "connection.h"
#include "remotecontrolinterface.h"

#include <QDialog>

class Ui_ClientMainWindow;

class ClientMainWindow : public QDialog,
                         protected RemoteControlInterface {
  Q_OBJECT

public:
  ClientMainWindow(QWidget* parent = 0);
  ~ClientMainWindow();

protected:
  // RemoteControlInterface
  void StateChanged(const QString& peer_jid_resource, const State& state);
  void AlbumArtChanged(const QString& peer_jid_resource, const QImage& art);

private slots:
  // From the UI
  void Connect();
  void PlayPauseClicked();
  void StopClicked();
  void NextClicked();
  void PreviousClicked();
  void RefreshClicked();
  void UpdateStateClicked();

  // From Connection
  void Connected();
  void Disconnected(const QString& error);
  void PeerFound(const Connection::Peer& peer);
  void PeerRemoved(const Connection::Peer& peer);

private:
  Ui_ClientMainWindow* ui_;

  Connection* connection_;
};

#endif // CLIENTMAINWINDOW_H
