#include "connection.h"

#include <QCoreApplication>
#include <QStringList>

int main(int argc, char** argv) {
  QCoreApplication a(argc, argv);

  if (a.arguments().count() != 3) {
    qFatal("Usage: %s username password", argv[0]);
  }

  Connection c;
  c.set_username(a.arguments()[1]);
  c.set_password(a.arguments()[2]);

  c.Connect();

  return a.exec();
}
