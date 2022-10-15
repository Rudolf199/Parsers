#include <math.h>

#include <algorithm>
#include <string>
#include <vector>
#include <exception>
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
  DFA(std::vector<Transition> transitions_, std::vector<State> states, std::string alphabet);
  void PrintDFA();
  std::string alphabet_;
  void Inversion();
};
std::vector<Transition> TransitionsInit(int num_of_transitions, std::vector<State>& states, std::string alphabet);
std::vector<State> StatesInit(int number_of_states);
