#include "common.h"

Metadata::Metadata()
    : track(0),
      disc(0),
      year(0),
      length_nanosec(0),
      rating(0.0) {
}

State::State()
    : playback_state(PlaybackState_Stopped),
      volume(0.0),
      can_go_next(false),
      can_go_previous(false),
      can_seek(false) {
}
