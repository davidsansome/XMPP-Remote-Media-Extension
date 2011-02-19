#include "handler.h"

#include <QtGlobal>

Handler::Handler()
    : client_(NULL) {
}

void Handler::Init(gloox::Client* client) {
  client_ = client;
}

void Handler::Reset() {
  client_ = NULL;
}
