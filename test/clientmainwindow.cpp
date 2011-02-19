#include "clientmainwindow.h"
#include "ui_clientmainwindow.h"

#include "connection.h"

ClientMainWindow::ClientMainWindow(QWidget* parent)
    : QDialog(parent),
      ui_(new Ui_ClientMainWindow),
      connection_(new Connection(this))
{
  ui_->setupUi(this);

  connect(ui_->connect, SIGNAL(clicked()), SLOT(Connect()));
  connect(ui_->playpause, SIGNAL(clicked()), SLOT(PlayPauseClicked()));
  connect(ui_->stop, SIGNAL(clicked()), SLOT(StopClicked()));
  connect(ui_->previous, SIGNAL(clicked()), SLOT(PreviousClicked()));
  connect(ui_->next, SIGNAL(clicked()), SLOT(NextClicked()));
  connect(ui_->refresh, SIGNAL(clicked()), SLOT(RefreshClicked()));

  connection_->set_agent_name("Test client");
  connection_->SetRemoteControl(this);

  connect(connection_, SIGNAL(Connected()), SLOT(Connected()));
  connect(connection_, SIGNAL(Disconnected()), SLOT(Disconnected()));
  connect(connection_, SIGNAL(PeerFound(Connection::Peer)), SLOT(PeerFound(Connection::Peer)));
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

void ClientMainWindow::Connected() {
  ui_->connect->setText("Disconnect");
  ui_->connect->setEnabled(true);
  ui_->remote_group->setEnabled(true);
  ui_->peers->clear();
}

void ClientMainWindow::Disconnected() {
  ui_->connect->setText("Connect");
  ui_->connect->setEnabled(true);
  ui_->remote_group->setEnabled(false);
}

void ClientMainWindow::PeerFound(const Connection::Peer& peer) {
  ui_->peers->addItem(peer.agent_name_, peer.jid_resource_);
}

void ClientMainWindow::StateChanged(const QString& peer_jid_resource, const State& state) {
}

void ClientMainWindow::AlbumArtChanged(const QString& peer_jid_resource, const QImage& art) {
}

