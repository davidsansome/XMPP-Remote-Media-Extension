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

  State state() const {
    State ret;
    ret.playback_state = State::PlaybackState_Playing;
    ret.position_nanosec = 0;
    ret.volume = 0.5;
    ret.can_go_next = true;
    ret.can_go_previous = true;
    ret.can_seek = true;
    ret.metadata.title = "Penis Song";
    ret.metadata.artist = "Monty Python";
    ret.metadata.album = "Monty Python Sings";
    ret.metadata.track = 4;
    ret.metadata.length_nanosec = 42000000000;
    ret.metadata.rating = 1.0;
    return ret;
  }

  QImage album_art() const {
    return QImage();
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
