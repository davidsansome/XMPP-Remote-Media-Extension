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

#include "clientmainwindow.h"
#include "ui_clientmainwindow.h"

#include <xrme/connection.h>

#include <QMessageBox>

ClientMainWindow::ClientMainWindow(QWidget* parent)
    : QDialog(parent),
      ui_(new Ui_ClientMainWindow),
      connection_(new xrme::Connection(this))
{
  ui_->setupUi(this);
  QPushButton* update_state =
      ui_->buttonBox->addButton("Update state", QDialogButtonBox::ResetRole);

  connect(ui_->connect, SIGNAL(clicked()), SLOT(Connect()));
  connect(ui_->playpause, SIGNAL(clicked()), SLOT(PlayPauseClicked()));
  connect(ui_->stop, SIGNAL(clicked()), SLOT(StopClicked()));
  connect(ui_->previous, SIGNAL(clicked()), SLOT(PreviousClicked()));
  connect(ui_->next, SIGNAL(clicked()), SLOT(NextClicked()));
  connect(ui_->refresh, SIGNAL(clicked()), SLOT(RefreshClicked()));
  connect(update_state, SIGNAL(clicked()), SLOT(UpdateStateClicked()));

  connection_->set_agent_name("Test client");
  connection_->SetRemoteControl(this);

  connect(connection_, SIGNAL(Connected()), SLOT(Connected()));
  connect(connection_, SIGNAL(Disconnected(QString)), SLOT(Disconnected(QString)));
  connect(connection_, SIGNAL(PeerFound(xrme::Connection::Peer)), SLOT(PeerFound(xrme::Connection::Peer)));
  connect(connection_, SIGNAL(PeerRemoved(xrme::Connection::Peer)), SLOT(PeerRemoved(xrme::Connection::Peer)));
}

ClientMainWindow::~ClientMainWindow() {
  delete ui_;
}

void ClientMainWindow::Connect() {
  ui_->connect->setEnabled(false);

  if (connection_->is_connected()) {
    connection_->Disconnect();
  } else {
    connection_->set_username(ui_->username->text());
    connection_->set_password(ui_->password->text());
    if (!connection_->Connect()) {
      ui_->connect->setEnabled(true);
    }
  }
}

void ClientMainWindow::PlayPauseClicked() {
  if (ui_->peers->currentIndex() != -1) {
    PlayPause(ui_->peers->itemData(ui_->peers->currentIndex()).toString());
  }
}

void ClientMainWindow::StopClicked() {
  if (ui_->peers->currentIndex() != -1) {
    Stop(ui_->peers->itemData(ui_->peers->currentIndex()).toString());
  }
}

void ClientMainWindow::NextClicked() {
  if (ui_->peers->currentIndex() != -1) {
    Next(ui_->peers->itemData(ui_->peers->currentIndex()).toString());
  }
}

void ClientMainWindow::PreviousClicked() {
  if (ui_->peers->currentIndex() != -1) {
    Previous(ui_->peers->itemData(ui_->peers->currentIndex()).toString());
  }
}

void ClientMainWindow::RefreshClicked() {
  ui_->peers->clear();
  connection_->RefreshPeers();
}

void ClientMainWindow::UpdateStateClicked() {
  if (ui_->peers->currentIndex() != -1) {
    QueryState(ui_->peers->itemData(ui_->peers->currentIndex()).toString());
  }
}

void ClientMainWindow::Connected() {
  ui_->connect->setText("Disconnect");
  ui_->connect->setEnabled(true);
  ui_->remote_group->setEnabled(true);
  ui_->current_track_group->setEnabled(true);
  ui_->peers->clear();
}

void ClientMainWindow::Disconnected(const QString& error) {
  ui_->connect->setText("Connect");
  ui_->connect->setEnabled(true);
  ui_->remote_group->setEnabled(false);
  ui_->current_track_group->setEnabled(false);

  if (!error.isNull()) {
    QMessageBox::warning(this, "Disconnected", error);
  }
}

void ClientMainWindow::PeerFound(const xrme::Connection::Peer& peer) {
  ui_->peers->addItem(peer.agent_name_, peer.jid_resource_);
}

void ClientMainWindow::PeerRemoved(const xrme::Connection::Peer& peer) {
  int index = ui_->peers->findData(peer.jid_resource_);
  if (index != -1) {
    ui_->peers->removeItem(index);
  }
}

void ClientMainWindow::StateChanged(const QString& peer_jid_resource, const xrme::State& state) {
  ui_->title->setText(state.metadata.title);
  ui_->artist->setText(state.metadata.artist);
  ui_->album->setText(state.metadata.album);
  ui_->albumartist->setText(state.metadata.albumartist);
  ui_->composer->setText(state.metadata.composer);
  ui_->genre->setText(state.metadata.genre);
  ui_->track->setText(QString::number(state.metadata.track));
  ui_->disc->setText(QString::number(state.metadata.disc));
  ui_->year->setText(QString::number(state.metadata.year));
  ui_->length->setText(QString::number(state.metadata.length_millisec));
  ui_->rating->setText(QString::number(state.metadata.rating));
}

void ClientMainWindow::AlbumArtChanged(const QString& peer_jid_resource, const QImage& art) {
  ui_->art->setPixmap(QPixmap::fromImage(art));
}

