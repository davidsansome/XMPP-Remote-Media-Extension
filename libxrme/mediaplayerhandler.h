#ifndef LIBXRME_MEDIAPLAYERHANDLER_H
#define LIBXRME_MEDIAPLAYERHANDLER_H

#include "handler.h"

#include <gloox/iqhandler.h>

class MediaPlayerInterface;

class MediaPlayerHandler : public Handler,
                           public gloox::IqHandler {
public:
  MediaPlayerHandler(MediaPlayerInterface* interface);

  static const char* kXmlNs;

  void StateChanged();

  // Handler
  void Init(gloox::Client* client);

  // gloox::IqHandler
  bool handleIq(gloox::Stanza* stanza);
  bool handleIqID(gloox::Stanza* stanza, int context);

private:
  MediaPlayerInterface* interface_;
};

#endif // LIBXRME_MEDIAPLAYERHANDLER_H
