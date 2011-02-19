#include "handler.h"

#include <QtGlobal>

namespace xrme {

Handler::Handler()
    : client_(NULL),
      connection_(NULL) {
}

void Handler::Init(Connection* connection, gloox::Client* client) {
  connection_ = connection;
  client_ = client;
}

void Handler::Reset() {
  connection_ = NULL;
  client_ = NULL;
}

} // namespace xrme
