#ifndef LIBXRME_MEDIAPLAYERHANDLER_H
#define LIBXRME_MEDIAPLAYERHANDLER_H

#include "handler.h"

#include <gloox/iqhandler.h>

namespace xrme {

class MediaPlayerInterface;

class MediaPlayerHandler : public Handler,
                           public gloox::IqHandler {
public:
  MediaPlayerHandler(MediaPlayerInterface* interface);

  static const int kMaxAlbumArtSize = 300;

  void StateChanged();
  void AlbumArtChanged();

  // Handler
  void Init(Connection* connection, gloox::Client* client);

  // gloox::IqHandler
  bool handleIq(gloox::Stanza* stanza);
  bool handleIqID(gloox::Stanza*, int) {}

private:
  MediaPlayerInterface* interface_;
};

} // namespace xrme

#endif // LIBXRME_MEDIAPLAYERHANDLER_H
