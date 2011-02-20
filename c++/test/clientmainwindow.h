/* This file is part of the XMPP Remote Media Extension.
   Copyright 2010, David Sansome <me@davidsansome.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

#include <xrme/connection.h>
#include <xrme/remotecontrolinterface.h>

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
