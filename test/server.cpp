/* This file is part of the XMPP Remote Media Extension.
   Copyright 2010, David Sansome <me@davidsansome.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "connection.h"
#include "mediaplayerinterface.h"

#include <QCoreApplication>
#include <QStringList>
#include <QtDebug>

class MediaPlayer : public xrme::MediaPlayerInterface {
public:
  void PlayPause() { qDebug() << __PRETTY_FUNCTION__; }
  void Stop() { qDebug() << __PRETTY_FUNCTION__; }
  void Next() { qDebug() << __PRETTY_FUNCTION__; }
  void Previous() { qDebug() << __PRETTY_FUNCTION__; }

  xrme::State state() const {
    xrme::State ret;
    ret.playback_state = xrme::State::PlaybackState_Playing;
    ret.position_millisec = 0;
    ret.volume = 0.5;
    ret.can_go_next = true;
    ret.can_go_previous = true;
    ret.can_seek = true;
    ret.metadata.title = "Penis Song";
    ret.metadata.artist = "Monty Python";
    ret.metadata.album = "Monty Python Sings";
    ret.metadata.track = 4;
    ret.metadata.disc = 1;
    ret.metadata.year = 2005;
    ret.metadata.length_millisec = 42000;
    ret.metadata.rating = 1.0;
    return ret;
  }

  QImage album_art() const {
    return QImage(":/trolltech/qmessagebox/images/qtlogo-64.png");
  }
};

int main(int argc, char** argv) {
  QCoreApplication a(argc, argv);

  if (a.arguments().count() != 3) {
    qFatal("Usage: %s username password", argv[0]);
  }

  xrme::Connection c;
  c.set_username(a.arguments()[1]);
  c.set_password(a.arguments()[2]);
  c.set_agent_name("Test server");
  //c.set_verbose(true);

  MediaPlayer player;
  c.SetMediaPlayer(&player);

  if (!c.Connect()) {
    return 1;
  }

  return a.exec();
}
