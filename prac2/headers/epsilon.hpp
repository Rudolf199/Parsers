#include <math.h>

#include <algorithm>
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
};