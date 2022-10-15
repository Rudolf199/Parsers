#include "../headers/DFA.hpp"

#include <iostream>
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
void NFA::MergeVertex(State& start, State& second) {
  for (size_t i = 0; i < transitions_.size(); ++i) {
    if (transitions_[i].start == second.index) {
      if (transitions_[i].path != '$') {
        Transition transition = Transition();
        transition.start = start.index;
        transition.end = transitions_[i].end;
        transition.path = transitions_[i].path;
        transitions_.push_back(transition);
      }
    }
  }
  if (second.IsFinal) {
    start.IsFinal = true;
  }
  second.isStart = false;
}
void NFA::MergeStarts() {
  for (size_t i = 0; i < transitions_.size(); ++i) {
    if (transitions_[i].path == '$') {
      MergeVertex(all_states_[transitions_[i].start],
                  all_states_[transitions_[i].end]);
      transitions_.erase(transitions_.begin() + i);
      i = 0;
    }
  }
}
void NFA::EpsilonsDelete() {
  for (size_t i = 0; i < transitions_.size(); ++i) {
    if (transitions_[i].path == '$') {
      EpsilonMerge(all_states_[transitions_[i].start],
                   all_states_[transitions_[i].end]);
      transitions_.erase(transitions_.begin() + i);
      i = 0;
    }
  }
}
void NFA::SingleStartTransitions(State state) {
  for (size_t i = 0; i < all_states_.size(); ++i) {
    if (all_states_[i].isStart) {
      Transition new_trans = Transition();
      new_trans.start = state.index;
      new_trans.end = all_states_[i].index;
      new_trans.path = '$';
      transitions_.push_back(new_trans);
      all_states_[i].isStart = false;
      i = 0;
    }
  }
  all_states_.push_back(state);
}
void NFA::DeleteDuplicates() {
  for (size_t i = 0; i < transitions_.size(); ++i) {
    for (size_t j = i + 1; j < transitions_.size(); ++j) {
      if (transitions_[i].end == transitions_[j].end &&
          transitions_[i].start == transitions_[j].start &&
          transitions_[i].path == transitions_[j].path) {
        transitions_.erase(transitions_.begin() + j);
        j = 0;
        i = 0;
      }
    }
  }
}
State NFA::SingleStart() {
  int index = all_states_.size();
  std::string name = "n0";
  State start_state = State();
  start_state.index = index;
  start_state.IsFinal = false;
  start_state.isStart = true;
  start_state.IsTrash = false;
  start_state.name = name;
  return start_state;
}
void PrintNFATransitions(NFA nfa) {
  std::cout << "--------------------Transitions----------------------\n";
  std::cout << "START          PATH           END\n";
  for (size_t i = 0; i < nfa.transitions_.size(); ++i) {
    std::cout << nfa.transitions_[i].start << "               "
              << nfa.transitions_[i].path << "               "
              << nfa.transitions_[i].end << "\n";
  }
}
void PrintNFAStates(NFA nfa) {
  std::cout << "--------------NFA--------------\n";
  std::cout << "\n------------------------STATES------------------\n";
  std::cout << "NAME    INDEX     TRASH      FINAL      START\n";
  for (size_t i = 0; i < nfa.all_states_.size(); ++i) {
    std::cout << nfa.all_states_[i].name << "         "
              << nfa.all_states_[i].index << "         "
              << nfa.all_states_[i].IsTrash << "        "
              << nfa.all_states_[i].IsFinal << "         "
              << nfa.all_states_[i].isStart << "\n";
  }
}
std::string Alphabet() {
  std::cout << "Please, enter size of alphabet\n";
  std::string alphabet;
  int amount;
  std::cin >> amount;
  char member;
  for (int i = 0; i < amount; ++i) {
    std::cout << "Please enter symbols\n";
    std::cin >> member;
    alphabet += member;
    // alphabet.push_back(member);
  }
  return alphabet;
}
// DFA ConvertDFA(NFA nfa) {}
std::vector<State> PlayGround(int number_of_states) {
  std::string state_type, name;
  std::vector<State> states;
  for (int i = 0; i < number_of_states; ++i) {
    State state = State();
    std::cout << "Name of " << i << " state:\n";
    std::cin >> name;
    state.name = name;
    std::cout << "Is is final state or start or trash state?(type 'final' or "
                 "'start' or 'both or 'trash' or 'none'):\n";
    std::cin >> state_type;
    state.IsFinal = false;
    state.IsTrash = false;
    state.isStart = false;
    if (state_type == "final") {
      state.IsFinal = true;
    }
    if (state_type == "start") {
      state.isStart = true;
    }
    if (state_type == "both") {
      state.IsFinal = true;
      state.isStart = true;
    }
    if (state_type == "trash") {
      state.IsTrash = true;
    }
    if (state_type == "none") {
      state.IsFinal = false;
      state.isStart = false;
      state.IsTrash = false;
    }
    state.index = i;
    states.push_back(state);
  }
  return states;
}

std::vector<Transition> TransitionsInit(int num_of_transitions) {
  std::vector<Transition> transitions;
  for (int i = 0; i < num_of_transitions; ++i) {
    Transition transition = Transition();
    std::cout << "Enter start and end indexes for transition:\n";
    int start, end;
    std::cin >> start >> end;
    transition.start = start;
    transition.end = end;
    char path;
    std::cout << "Enter path for " << i << "-th transition\n";
    std::cin >> path;
    transition.path = path;
    transitions.push_back(transition);
  }
  return transitions;
}
void NFADisplay(NFA nfa) {
  PrintNFAStates(nfa);
  PrintNFATransitions(nfa);
}
bool NFA::StartCount() {
  int counter = 0;
  for (size_t i = 0; i < all_states_.size(); ++i) {
    if (all_states_[i].isStart) {
      ++counter;
    }
  }
  if (counter > 1) {
    return true;
  }
  return false;
}
void NFA::DeleteDulls() {
  for (size_t i = 0; i < all_states_.size(); ++i) {
    int income = 0;
    for (size_t j = 0; j < transitions_.size(); ++j) {
      if (transitions_[j].end == all_states_[i].index) {
        ++income;
      }
    }
    std::cout << income << " incomes\n";
    if (income == 0 && all_states_[i].isStart == false) {
      int temp = all_states_[i].index;
      all_states_.erase(all_states_.begin() + i);
      for (size_t j = 0; j < transitions_.size(); ++j) {
        if (transitions_[j].start == temp) {
          transitions_.erase(transitions_.begin() + j);
          j = 0;
        }
      }
      i = 0;
    }
  }
}
State NFA::FindStart() {
  State state = State();
  for (size_t i = 0; i < all_states_.size(); ++i) {
    if (all_states_[i].isStart) {
      state = all_states_[i];
      return state;
    }
  }
  return state;
}
TableNode NFA::Outcomes(TableNode node, char path) {
  TableNode res = TableNode();
  // State state = State();
  // state.name = str;
  // std::string str;
  for (size_t j = 0; j < node.states.size(); ++j) {
    for (size_t i = 0; i < transitions_.size(); ++i) {
      if (transitions_[i].path == path &&
          transitions_[i].start == node.states[j].index) {
        res.states.push_back(all_states_[transitions_[i].end]);
        res.name = res.name + all_states_[transitions_[i].end].name;
      }
    }
  }
  std::sort(res.name.begin(), res.name.end());
  return res;
}
Table NFA::MakeTable() {
  Table table = Table();
  std::string alphabet = Alphabet();
  table.alphabet = alphabet;
  for (size_t i = 0; i <= alphabet.size(); ++i) {
    Col column = Col();
    table.columns.push_back(column);
  }
  State start = FindStart();
  TableNode init = TableNode();
  init.states.push_back(start);
  init.name = start.name;
  table.columns[0].nodes.push_back(init);
  for (size_t i = 1; i < table.columns.size(); ++i) {
    TableNode temp = TableNode();
    temp = Outcomes(table.columns[0].nodes[0], alphabet[i - 1]);
    table.columns[i].nodes.push_back(temp);
  }
  std::deque<TableNode> deque;
  for (size_t i = 1; i < table.columns.size(); ++i) {
    deque.push_back(table.columns[i].nodes[0]);
  }
  int current = 1;
  while (deque.size() != 0) {
    TableNode start = deque.front();
    table.columns[0].nodes.push_back(start);
    for (size_t i = 1; i < table.columns.size(); ++i) {
      TableNode temp = TableNode();
      temp = Outcomes(table.columns[0].nodes[current], alphabet[i - 1]);
      table.columns[i].nodes.push_back(temp);
      bool IsntThere = true;
      for (size_t j = 0; j < deque.size(); ++j) {
        if (deque[j].name == temp.name) {
          IsntThere = false;
          break;
        }
      }
      for (size_t j = 0; j < table.columns[0].nodes.size(); ++j) {
        if (table.columns[0].nodes[j].name == temp.name) {
          IsntThere = false;
        }
      }
      if (IsntThere && temp.name != "") {
        deque.push_back(temp);
      }
    }
    deque.pop_front();
    current++;
  }
  return table;
}
void PrintTable(Table table) {
  for (size_t i = 0; i < table.columns.size(); ++i) {
    std::cout << "Column " << i << "\n";
    for (size_t j = 0; j < table.columns[i].nodes.size(); j++) {
      std::cout << table.columns[i].nodes[j].name << "\n";
    }
    std::cout << "--------\n";
  }
}
int FindIndex(TableNode node, std::vector<State> states) {
  for (size_t i = 0; i < states.size(); ++i) {
    if (node.name == states[i].name) {
      return i;
    }
  }
  return 0;
}
DFA::DFA(std::vector<Transition> transitions, std::vector<State> states) {
  all_states_ = states;
  transitions_ = transitions;
}
DFA ConvertDFA(Table table) {
  std::vector<State> states;
  std::vector<Transition> transitions;
  State start = State();
  start.isStart = true;
  start.name = table.columns[0].nodes[0].name;
  states.push_back(start);
  for (size_t i = 1; i < table.columns[0].nodes.size(); ++i) {
    State state = State();
    if (table.columns[0].nodes[i].name == start.name) {
      for (size_t k = 0; k < table.columns.size(); ++k) {
        table.columns[k].nodes.erase(table.columns[k].nodes.begin() + i);
      }
      i = i - 1;
      continue;
    }
    state.index = i;
    state.name = table.columns[0].nodes[i].name;
    for (size_t j = 0; j < table.columns[0].nodes[i].states.size(); ++j) {
      if (table.columns[0].nodes[i].states[j].IsFinal) {
        state.IsFinal = true;
      }
    }
    states.push_back(state);
  }
  for (size_t i = 0; i < table.columns[0].nodes.size(); ++i) {
    for (size_t j = 1; j < table.columns.size(); ++j) {
      Transition transition = Transition();
      transition.path = table.alphabet[j - 1];
      transition.start = i;
      transition.end = FindIndex(table.columns[j].nodes[i], states);
      transitions.push_back(transition);
    }
  }
  DFA dfa(transitions, states);
  return dfa;
}
void DFA::DeleteDuplicates() {
  for (size_t i = 0; i < transitions_.size(); ++i) {
    for (size_t j = i + 1; j < transitions_.size(); ++j) {
      if (transitions_[i].end == transitions_[j].end &&
          transitions_[i].start == transitions_[j].start &&
          transitions_[i].path == transitions_[j].path) {
        transitions_.erase(transitions_.begin() + j);
        j = 0;
        i = 0;
      }
    }
  }
}
void DFA::PrintDFA() {
  std::cout << "--------------DFA--------------\n";
  std::cout << "\n------------------------STATES------------------\n";
  std::cout << "NAME    INDEX     TRASH      FINAL      START\n";
  for (size_t i = 0; i < all_states_.size(); ++i) {
    std::cout << all_states_[i].name << "         " << all_states_[i].index
              << "         " << all_states_[i].IsTrash << "        "
              << all_states_[i].IsFinal << "         " << all_states_[i].isStart
              << "\n";
  }
  std::cout << "--------------------Transitions----------------------\n";
  std::cout << "START          PATH           END\n";
  for (size_t i = 0; i < transitions_.size(); ++i) {
    std::cout << transitions_[i].start << "               "
              << transitions_[i].path << "               "
              << transitions_[i].end << "\n";
  }
}
int main() {
  std::cout << "Enter the number of states:\n";
  int number_of_states;
  std::cin >> number_of_states;
  std::vector<State> states = PlayGround(number_of_states);
  std::cout << "Enter number of transitions:\n";
  int num_of_transitions;
  std::cin >> num_of_transitions;
  std::vector<Transition> transitions = TransitionsInit(num_of_transitions);
  NFA nfa(transitions, states);
  // NFADisplay(nfa);
  nfa.EpsilonsDelete();
  NFADisplay(nfa);
  std::cout << "start count " << nfa.StartCount() << "\n";
  if (nfa.StartCount()) {
    nfa.SingleStartTransitions(nfa.SingleStart());
    nfa.MergeStarts();
  }
  nfa.DeleteDuplicates();
  nfa.DeleteDulls();
  NFADisplay(nfa);
  Table table = nfa.MakeTable();
  PrintTable(table);
  std::vector<State> temp_states;
  std::vector<Transition> temp_transitions;
  DFA dfa(temp_transitions, temp_states);
  dfa = ConvertDFA(table);
  dfa.PrintDFA();
  return 0;
}