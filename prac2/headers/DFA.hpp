#include <math.h>

#include <algorithm>
#include <deque>
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
struct TableNode {
  std::vector<State> states;
  std::string name;
};
struct Col {
  std::vector<TableNode> nodes;
};
struct Table {
  std::vector<Col> columns;
  std::string alphabet;
};

class NFA {
 public:
  std::vector<State> all_states_;
  std::vector<Transition> transitions_;
  NFA(std::vector<Transition> transitions, std::vector<State> states);
  void EpsilonMerge(State& first, State& second);
  void EpsilonsDelete();
  void MergeStarts();
  void MergeVertex(State& start, State& second);
  State SingleStart();
  void SingleStartTransitions(State state);
  void DeleteDulls();
  void DeleteDuplicates();
  bool StartCount();
  State FindStart();
  Table MakeTable();
  TableNode Outcomes(TableNode node, char path);
};
class DFA {
 public:
  std::vector<State> all_states_;
  std::vector<Transition> transitions_;
  DFA(std::vector<Transition> transitions_, std::vector<State> states);
  void DeleteDuplicates();
  void PrintDFA();
};
DFA ConvertDFA(Table table);
std::string Alphabet();
std::vector<Transition> TransitionsInit(int num_of_transitions);
std::vector<State> PlayGround(int number_of_states);
void PrintNFAStates(NFA nfa);
void PrintNFATransitions(NFA nfa);
void NFADisplay(NFA nfa);
int FindIndex(TableNode node, std::vector<State> states);