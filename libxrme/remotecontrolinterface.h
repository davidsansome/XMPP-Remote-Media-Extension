#ifndef XRME_REMOTECONTROLINTERFACE_H
#define XRME_REMOTECONTROLINTERFACE_H

#include "common.h"
#include "connection.h"

class QImage;

namespace xrme {

class RemoteControlHandler;

class RemoteControlInterface {
public:
  RemoteControlInterface();
  virtual ~RemoteControlInterface();

  // All functions here will work asynchronously and return immediately.

  // Call these to control the playback of a MediaPlayer.
  void PlayPause(const QString& peer_jid_resource);
  void Stop(const QString& peer_jid_resource);
  void Next(const QString& peer_jid_resource);
  void Previous(const QString& peer_jid_resource);

  // Call this to query the MediaPlayer.  StateChanged() will be called later.
  void QueryState(const QString& peer_jid_resource);

  // Called whenever the MediaPlayer's state changes.
  virtual void StateChanged(const QString& peer_jid_resource,
                            const State& state) = 0;

  // Called whenever the MediaPlayer's album art changes.
  virtual void AlbumArtChanged(const QString& peer_jid_resource,
                               const QImage& art) = 0;

private:
  Q_DISABLE_COPY(RemoteControlInterface);
  friend class RemoteControlHandler;

  void Attach(RemoteControlHandler* handler);

  struct Private;
  QScopedPointer<Private> d;
};

} // namespace xrme

#endif // XRME_REMOTECONTROLINTERFACE_H
