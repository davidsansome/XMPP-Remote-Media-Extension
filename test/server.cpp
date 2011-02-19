#include "connection.h"
#include "mediaplayerinterface.h"

#include <QCoreApplication>
#include <QStringList>
#include <QtDebug>

class MediaPlayer : public MediaPlayerInterface {
public:
  void PlayPause() { qDebug() << __PRETTY_FUNCTION__; }
  void Stop() { qDebug() << __PRETTY_FUNCTION__; }
  void Next() { qDebug() << __PRETTY_FUNCTION__; }
  void Previous() { qDebug() << __PRETTY_FUNCTION__; }

  PlaybackState playback_state() const { return PlaybackState_Playing; }
  double volume() const { return 0.5; }
  bool can_go_next() const { return true; }
  bool can_go_previous() const { return true; }
  bool can_seek() const { return true; }
  Metadata metadata() const {
    Metadata ret;
    ret.title = "Penis Song";
    ret.artist = "Monty Python";
    ret.album = "Monty Python Sings";
    ret.track = 4;
    ret.length_nanosec = 42000000000;
    ret.rating = 1.0;
    return ret;
  }
};

int main(int argc, char** argv) {
  QCoreApplication a(argc, argv);

  if (a.arguments().count() != 3) {
    qFatal("Usage: %s username password", argv[0]);
  }

  Connection c;
  c.set_username(a.arguments()[1]);
  c.set_password(a.arguments()[2]);
  c.set_agent_name("Test server");
  c.set_verbose(true);

  MediaPlayer player;
  c.SetMediaPlayer(&player);

  if (!c.Connect()) {
    return 1;
  }

  return a.exec();
}
