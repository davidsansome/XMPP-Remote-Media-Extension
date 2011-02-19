#include "common.h"

namespace xrme {

const char* kXmlnsXrme = "http://purplehatstands.com/xmlns/xrme";
const char* kXmlnsXrmeMediaPlayer = "http://purplehatstands.com/xmlns/xrme/mediaplayer";
const char* kXmlnsXrmeRemoteControl = "http://purplehatstands.com/xmlns/xrme/remotecontrol";

Metadata::Metadata()
    : track(0),
      disc(0),
      year(0),
      length_millisec(0),
      rating(0.0) {
}

State::State()
    : playback_state(PlaybackState_Stopped),
      position_millisec(0),
      volume(0.0),
      can_go_next(false),
      can_go_previous(false),
      can_seek(false) {
}

} // namespace xrme
