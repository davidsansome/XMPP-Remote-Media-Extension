#include "remotecontrolhandler.h"
#include "remotecontrolinterface.h"

namespace xrme {

struct RemoteControlInterface::Private {
  Private()
    : handler_(NULL) {}

  RemoteControlHandler* handler_;
};


RemoteControlInterface::RemoteControlInterface()
    : d(new Private) {
}

RemoteControlInterface::~RemoteControlInterface() {
}

void RemoteControlInterface::PlayPause(const QString& peer_jid_resource) {
  if (d->handler_) {
    d->handler_->PlayPause(peer_jid_resource);
  }
}

void RemoteControlInterface::Stop(const QString& peer_jid_resource) {
  if (d->handler_) {
    d->handler_->Stop(peer_jid_resource);
  }
}

void RemoteControlInterface::Next(const QString& peer_jid_resource) {
  if (d->handler_) {
    d->handler_->Next(peer_jid_resource);
  }
}

void RemoteControlInterface::Previous(const QString& peer_jid_resource) {
  if (d->handler_) {
    d->handler_->Previous(peer_jid_resource);
  }
}

void RemoteControlInterface::QueryState(const QString& peer_jid_resource) {
  if (d->handler_) {
    d->handler_->QueryState(peer_jid_resource);
  }
}

void RemoteControlInterface::Attach(RemoteControlHandler* handler) {
  d->handler_ = handler;
}

} // namespace xrme
