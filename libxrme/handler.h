#ifndef LIBXRME_HANDLER_H
#define LIBXRME_HANDLER_H

namespace gloox {
  class Client;
}

class Handler {
public:
  Handler();
  virtual ~Handler() {}

  virtual void Init(gloox::Client* client);
  virtual void Reset();

protected:
  gloox::Client* client_;
};

#endif // LIBXRME_HANDLER_H
