#ifndef LIBXRME_MEDIAPLAYERINTERFACE_H
#define LIBXRME_MEDIAPLAYERINTERFACE_H

#include <QImage>
#include <QScopedPointer>

class MediaPlayerHandler;

class MediaPlayerInterface {
public:
  MediaPlayerInterface();
  virtual ~MediaPlayerInterface();

  enum PlaybackState {
    PlaybackState_Stopped = 0,
    PlaybackState_Playing = 1,
    PlaybackState_Paused = 2,
  };

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
    quint64 length_nanosec;
    double rating; // range 0.0 - 1.0

    QImage art;
  };

  // Control playback
  virtual void PlayPause() = 0;
  virtual void Stop() = 0;
  virtual void Next() = 0;
  virtual void Previous() = 0;

  // Query the current state of the player.  StateChanged() should be called
  // when any of these values change.
  virtual PlaybackState playback_state() const = 0;
  virtual double volume() const = 0; // range 0.0 - 1.0
  virtual bool can_go_next() const = 0;
  virtual bool can_go_previous() const = 0;
  virtual bool can_seek() const = 0;
  virtual Metadata metadata() const = 0;

protected:
  // Call this when the value of any of the above getter methods have changed.
  virtual void StateChanged();

private:
  Q_DISABLE_COPY(MediaPlayerInterface);
  friend class MediaPlayerHandler;

  void Attach(MediaPlayerHandler* handler);

  struct Private;
  QScopedPointer<Private> d;
};

#endif // LIBXRME_MEDIAPLAYERINTERFACE_H
