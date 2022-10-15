#include "../headers/FDFA.hpp"

#include <iostream>
DFA::DFA(std::vector<Transition> transitions, std::vector<State> states) {
  all_states_ = states;
  transitions_ = transitions;
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
    std::cout << all_states_[transitions_[i].start].name << "               "
              << transitions_[i].path << "               "
              << all_states_[transitions_[i].end].name << "\n";
  }
}
std::vector<State> StatesInit(int number_of_states) {
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
  int starts = 0, finals = 0;
  for(size_t i = 0; i < states.size(); ++i){
    if(states[i].isStart){
        starts++;
    }
    if(states[i].IsFinal){
        finals++;
    }
  }
  if(starts != 1 || finals == 0){
    std::cout<<"You entered some cringe!\n";
    abort();
  }
  return states;
}
std::vector<Transition> TransitionsInit(int num_of_transitions,
                                        std::vector<State>& states) {
  std::vector<Transition> transitions;
  std::cout << "Enter alphabet size \n";
  int alp_size;
  std::cin >> alp_size;
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
    if(path == '$'){
        std::cout<<"What a cringe...\n";
        abort();
    }
    transition.path = path;
    states[start].path_counter++;
    states[start].paths += path;
    std::sort(states[start].paths.begin(), states[start].paths.end());
    if (states[start].path_counter > alp_size) {
      std::cout << "We are doing DFA here......\n";
      abort();
    }
    transitions.push_back(transition);
  }
  return transitions;
}
void DFA::AddTrashState(int index, char path) {
  State trash = State();
  trash.IsTrash = true;
  trash.name = "trash-" + all_states_[index].name;
  trash.index = all_states_.size();
  trash.paths = alphabet;
  trash.path_counter = (int)alphabet.size();
  all_states_.push_back(trash);
  for (size_t i = 0; i < alphabet.size(); ++i) {
    Transition loop = Transition();
    loop.start = trash.index;
    loop.end = trash.index;
    loop.path = alphabet[i];
    transitions_.push_back(loop);
  }
  Transition connection = Transition();
  connection.path = path;
  connection.start = index;
  connection.end = trash.index;
  transitions_.push_back(connection);
}
void DFA::MakeFDFA() {
  for (size_t i = 0; i < all_states_.size(); ++i) {
    if (all_states_[i].path_counter < alphabet.size()) {
      for (size_t j = 0; j < alphabet.size(); ++j) {
        if (all_states_[i].paths[j] != alphabet[j]) {
          AddTrashState(i, alphabet[j]);
          all_states_[i].paths += alphabet[j];
          all_states_[i].path_counter++;
          std::sort(all_states_[i].paths.begin(), all_states_[i].paths.end());
          j = 0;
        }
      }
    }
  }
}

int main() {
  std::cout << "Enter the number of states:\n";
  int number_of_states;
  std::cin >> number_of_states;
  std::vector<State> states = StatesInit(number_of_states);
  std::cout << "Enter number of transitions:\n";
  int num_of_transitions;
  std::cin >> num_of_transitions;
  std::vector<Transition> transitions = TransitionsInit(num_of_transitions, states);
  DFA dfa(transitions, states);
  dfa.PrintDFA();
  std::cout<<"Enter alphabet\n";
  std::string alphabet;
  std::cin>> alphabet;
  dfa.alphabet = alphabet;
  dfa.MakeFDFA();
  dfa.PrintDFA();
  return 0;
}