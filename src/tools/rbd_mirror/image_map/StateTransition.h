// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_RBD_MIRROR_IMAGE_MAP_STATE_TRANSITION_H
#define CEPH_RBD_MIRROR_IMAGE_MAP_STATE_TRANSITION_H

namespace rbd {
namespace mirror {
namespace image_map {

class StateTransition {
public:
  enum ActionType {
    ACTION_TYPE_ADD = 0,
    ACTION_TYPE_REMOVE,
    ACTION_TYPE_SHUFFLE,
  };

  enum State {
    STATE_UNASSIGNED = 0, // starting (initial) state
    STATE_ASSOCIATED,     // acquire image
    STATE_DISASSOCIATED,  // release image
    STATE_UPDATE_MAPPING, // update on-disk map
    STATE_REMOVE_MAPPING, // remove on-disk map
    STATE_COMPLETE,       // special state to invoke completion callback
  };

  struct Transition {
    State next_state;
    bool retry_on_error;

    Transition() {
    }
    Transition(State next_state, bool retry_on_error)
      : next_state(next_state),
        retry_on_error(retry_on_error) {
    }
  };

  static const Transition &transit(ActionType action_type, State state);

private:
  struct TransitionTable {
    // in: action + current_state
    ActionType action_type;
    State current_state;

    // out: Transition
    Transition transition;
  };

  // image transition table
  static const TransitionTable transition_table[];
};

std::ostream &operator<<(std::ostream &os, const StateTransition::ActionType &action_type);
std::ostream &operator<<(std::ostream &os, const StateTransition::State &state);

} // namespace image_map
} // namespace mirror
} // namespace rbd

#endif // CEPH_RBD_MIRROR_IMAGE_MAP_STATE_TRANSITION_H
