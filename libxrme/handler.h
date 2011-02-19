#ifndef LIBXRME_HANDLER_H
#define LIBXRME_HANDLER_H

namespace gloox {
  class Client;
}

class Handler {
public:
  virtual ~Handler() {}
  virtual void Init(gloox::Client* client) = 0;
};

#endif // LIBXRME_HANDLER_H
