#include <math.h>

#include <algorithm>
#include <exception>
#include <string>
#include <vector>
struct State {
  std::string name;
  bool isStart;
  bool IsFinal;
  int index;
  int path_counter;
  bool IsTrash;
  std::string paths;
};

struct Transition {
  char path;
  int start;
  int end;
};

class DFA {
 public:
  std::vector<State> all_states_;
  std::vector<Transition> transitions_;
  DFA(std::vector<Transition> transitions_, std::vector<State> states,
      std::string alphabet);
  void PrintDFA();
  std::string alphabet_;
  void Inversion();
  int Finals() {
    int counter = 0;
    for (size_t i = 0; i < all_states_.size(); ++i) {
      if (all_states_[i].IsFinal) {
        counter++;
      }
    }
    return counter;
  }
};
std::vector<Transition> TransitionsInit(int num_of_transitions,
                                        std::vector<State>& states,
                                        std::string alphabet);
std::vector<State> StatesInit(int number_of_states);
