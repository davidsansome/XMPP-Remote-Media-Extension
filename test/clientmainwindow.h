#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

#include "connection.h"
#include "remotecontrolinterface.h"

#include <QDialog>

class Ui_ClientMainWindow;

class ClientMainWindow : public QDialog,
                         protected xrme::RemoteControlInterface {
  Q_OBJECT

public:
  ClientMainWindow(QWidget* parent = 0);
  ~ClientMainWindow();

protected:
  // xrme::RemoteControlInterface
  void StateChanged(const QString& peer_jid_resource, const xrme::State& state);
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

  // From xrme::Connection
  void Connected();
  void Disconnected(const QString& error);
  void PeerFound(const xrme::Connection::Peer& peer);
  void PeerRemoved(const xrme::Connection::Peer& peer);

private:
  Ui_ClientMainWindow* ui_;

  xrme::Connection* connection_;
};

#endif // CLIENTMAINWINDOW_H
