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

MediaPlayerInterface::Metadata::Metadata()
    : track(0),
      disc(0),
      year(0),
      length_nanosec(0),
      rating(0.0) {
}

void MediaPlayerInterface::StateChanged() {
  if (!d->handler_) {
    qWarning() << "MediaPlayerInterface: You must add this interface to a"
                  " Connection before calling StateChanged()";
    return;
  }

  d->handler_->StateChanged();
}

void MediaPlayerInterface::Attach(MediaPlayerHandler* handler) {
  d->handler_ = handler;
}
