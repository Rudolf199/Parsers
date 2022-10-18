#include "./addition.hpp"

#include <iostream>
DFA::DFA(std::vector<Transition> transitions, std::vector<State> states,
         std::string alphabet) {
  all_states_ = states;
  transitions_ = transitions;
  alphabet_ = alphabet;
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
  for (size_t i = 0; i < states.size(); ++i) {
    if (states[i].isStart) {
      starts++;
    }
    if (states[i].IsFinal) {
      finals++;
    }
  }
  if (starts != 1 || finals == 0) {
    std::cout << "You entered some cringe!\n";
    abort();
  }
  return states;
}
std::vector<Transition> TransitionsInit(int num_of_transitions,
                                        std::vector<State>& states,
                                        std::string alphabet) {
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
    if (path == '$') {
      std::cout << "What a cringe...\n";
      abort();
    }
    transition.path = path;
    states[start].path_counter++;
    states[start].paths += path;
    std::sort(states[start].paths.begin(), states[start].paths.end());
    if (states[start].path_counter > (int)alphabet.size()) {
      std::cout << "We are doing DFA here......\n";
      abort();
    }
    transitions.push_back(transition);
  }
  if (transitions.size() != states.size() * alphabet.size()) {
    std::cout << "Not a FDFA\n";
    abort();
  }
  return transitions;
}

void DFA::Inversion() {
  for (size_t i = 0; i < all_states_.size(); ++i) {
    if (all_states_[i].IsFinal) {
      all_states_[i].IsFinal = false;
    } else if (!all_states_[i].IsFinal) {
      all_states_[i].IsFinal = true;
    }
  }
}