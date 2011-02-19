#include "clientmainwindow.h"
#include "ui_clientmainwindow.h"

#include "connection.h"

ClientMainWindow::ClientMainWindow(QWidget* parent)
    : QDialog(parent),
      ui_(new Ui_ClientMainWindow),
      connection_(new Connection(this))
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
  connect(connection_, SIGNAL(Disconnected()), SLOT(Disconnected()));
  connect(connection_, SIGNAL(PeerFound(Connection::Peer)), SLOT(PeerFound(Connection::Peer)));
  connect(connection_, SIGNAL(PeerRemoved(Connection::Peer)), SLOT(PeerRemoved(Connection::Peer)));
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

void ClientMainWindow::PeerRemoved(const Connection::Peer& peer) {
  int index = ui_->peers->findData(peer.jid_resource_);
  if (index != -1) {
    ui_->peers->removeItem(index);
  }
}

void ClientMainWindow::StateChanged(const QString& peer_jid_resource, const State& state) {
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
