#ifndef XRME_COMMON_H
#define XRME_COMMON_H

#include <QString>

struct Metadata {
  Metadata();

  QString title;
  QString artist;
  QString album;
  QString albumartist;
  QString composer;
  QString genre;
  int track;
  int disc;
  int year;
  int length_millisec;
  double rating; // range 0.0 - 1.0
};

struct State {
  State();

  enum PlaybackState {
    PlaybackState_Stopped = 0,
    PlaybackState_Playing = 1,
    PlaybackState_Paused = 2,
  };

  PlaybackState playback_state;
  int position_millisec;
  double volume; // range 0.0 - 1.0
  bool can_go_next;
  bool can_go_previous;
  bool can_seek;
  Metadata metadata;
};

extern const char* kXmlnsXrme;
extern const char* kXmlnsXrmeMediaPlayer;
extern const char* kXmlnsXrmeRemoteControl;

#endif // XRME_COMMON_H
