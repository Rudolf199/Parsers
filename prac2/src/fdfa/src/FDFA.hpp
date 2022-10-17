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
  DFA(std::vector<Transition> transitions_, std::vector<State> states);
  void PrintDFA();
  void MakeFDFA();
  std::string alphabet;
  void AddTrashState(int index, char path);
};
std::vector<Transition> TransitionsInit(int num_of_transitions, std::vector<State>& states);
std::vector<State> StatesInit(int number_of_states);
