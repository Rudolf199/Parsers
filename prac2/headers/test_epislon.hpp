#include <math.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
struct State {
  std::string name;
  bool isStart;
  bool IsFinal;
  int index;
  bool IsTrash;
};

struct Transition {
  char path;
  int start;
  int end;
};
class NFA {
 public:
  std::vector<State> all_states_;
  std::vector<Transition> transitions_;
  NFA(std::vector<Transition> transitions, std::vector<State> states);
  void EpsilonMerge(State& first, State& second);
  void EpsilonsDelete();
  bool EpsilonsAreMerged() {
    for (size_t i = 0; i < transitions_.size(); ++i) {
      if (transitions_[i].path == '$') {
        return false;
      }
    }
    return true;
  }
};
NFA::NFA(std::vector<Transition> transitions, std::vector<State> states) {
  all_states_ = states;
  transitions_ = transitions;
}
void NFA::EpsilonMerge(State& first, State& second) {
  for (size_t i = 0; i < transitions_.size(); ++i) {
    if (transitions_[i].start == second.index) {
      if (transitions_[i].path != '$') {
        Transition transition = Transition();
        transition.start = first.index;
        transition.end = transitions_[i].end;
        transition.path = transitions_[i].path;
        transitions_.push_back(transition);
      }
    }
  }
  if (second.IsFinal) {
    first.IsFinal = true;
  }
  if (first.isStart) {
    second.isStart = true;
  }
}
void NFA::EpsilonsDelete() {
  for (int i = 0; i < transitions_.size(); ++i) {
    if (transitions_[i].path == '$') {
      EpsilonMerge(all_states_[transitions_[i].start],
                   all_states_[transitions_[i].end]);
      transitions_.erase(transitions_.begin() + i);
      i = 0;
    }
  }
}