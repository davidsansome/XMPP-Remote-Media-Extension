#ifndef LIBXRME_HANDLER_H
#define LIBXRME_HANDLER_H

namespace gloox {
  class Client;
}

namespace xrme {
class Connection;

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

} // namespace xrme

#endif // LIBXRME_HANDLER_H
