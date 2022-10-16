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
  std::string friends;
  int current;
};
struct Group {
  std::vector<State> states;
  std::string names;
  int index;
  std::vector<int> indexes;
};
struct Groups {
  std::vector<Group> groups;
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
  void Partition(char path, Groups& classes, int& movements, size_t& size);
  Groups MinDFAgroups();
  int WhereItGoes(int index, char path);
  int FindGroupIndex(int index, Groups& classes);
  void ClearName(std::string& names, std::string name);
  void NewGroups(Group& kickout, Groups& classes);
  bool AllInOne(Group group);
  DFA MinDFA(Groups classes);
};
std::vector<Transition> TransitionsInit(int num_of_transitions,
                                        std::vector<State>& states,
                                        std::string alphabet);
std::vector<State> StatesInit(int number_of_states);
void DeleteIndex(std::vector<int>& vect, int index);
void printclasses(Groups classes);