#ifndef LIBXRME_HANDLER_H
#define LIBXRME_HANDLER_H

class Connection;

namespace gloox {
  class Client;
}

class Handler {
public:
  Handler();
  virtual ~Handler() {}

  virtual void Init(Connection* connection, gloox::Client* client);
  virtual void Reset();

protected:
  Connection* connection_;
  gloox::Client* client_;
};

#endif // LIBXRME_HANDLER_H
