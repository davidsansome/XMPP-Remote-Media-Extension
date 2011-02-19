#include "mediaplayerhandler.h"
#include "mediaplayerinterface.h"

#include <QtDebug>

struct MediaPlayerInterface::Private {
  Private()
    : handler_(NULL) {}

  MediaPlayerHandler* handler_;
};


MediaPlayerInterface::MediaPlayerInterface()
    : d(new Private) {
}

MediaPlayerInterface::~MediaPlayerInterface() {
}

void MediaPlayerInterface::StateChanged() {
  if (!d->handler_) {
    return;
  }

  d->handler_->StateChanged();
}

void MediaPlayerInterface::AlbumArtChanged() {
  if (!d->handler_) {
    return;
  }

  d->handler_->AlbumArtChanged();
}

void MediaPlayerInterface::Attach(MediaPlayerHandler* handler) {
  d->handler_ = handler;
}
