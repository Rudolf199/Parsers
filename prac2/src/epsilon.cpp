#include "../headers/epsilon.hpp"
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

int main() {
  std::cout << "Enter the number of states:\n";
  int number_of_states;
  std::cin >> number_of_states;
  std::string state_type;
  std::vector<State> states;
  std::vector<Transition> transitions;
  std::string name;
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
  std::cout << "\n------------------------STATES------------------\n";
  std::cout << "NAME    INDEX     TRASH      FINAL      START\n";
  for (int i = 0; i < number_of_states; ++i) {
    std::cout << states[i].name << "             " << states[i].index
              << "        " << states[i].IsTrash << "       "
              << states[i].IsFinal << "   " << states[i].isStart << "\n";
  }
  std::cout << "Enter number of transitions:\n";
  int num_of_transitions;
  std::cin >> num_of_transitions;
  for (int i = 0; i < num_of_transitions; ++i) {
    Transition transition = Transition();
    std::cout << "Enter start and end indexes for transition:\n";
    int start, end;
    std::cin >> start >> end;
    transition.start = start;
    transition.end = end;
    char path;
    std::cout << "Enter path for " << i << "-th transition";
    std::cin >> path;
    transition.path = path;
    transitions.push_back(transition);
  }
  std::cout << "--------------------Transitions----------------------\n";
  std::cout << "START          PATH           END\n";
  for (int i = 0; i < transitions.size(); ++i) {
    std::cout << transitions[i].start << "      " << transitions[i].path
              << "      " << transitions[i].end << "\n";
  }
  NFA nfa(transitions, states);
  std::cout << "--------------NFA--------------\n";
  std::cout << "\n------------------------STATES------------------\n";
  std::cout << "NAME    INDEX     TRASH      FINAL      START\n";
  for (int i = 0; i < nfa.all_states_.size(); ++i) {
    std::cout << nfa.all_states_[i].name << "      " << nfa.all_states_[i].index
              << "   " << nfa.all_states_[i].IsTrash << "          "
              << nfa.all_states_[i].IsFinal << "           "
              << nfa.all_states_[i].isStart << "\n";
  }
  std::cout << "--------------------Transitions----------------------\n";
  std::cout << "START          PATH           END\n";
  for (int i = 0; i < nfa.transitions_.size(); ++i) {
    std::cout << nfa.transitions_[i].start << "      "
              << nfa.transitions_[i].path << "      " << nfa.transitions_[i].end
              << "\n";
  }
  nfa.EpsilonsDelete();
  std::cout << "--------------NEWNFA--------------\n";
  std::cout << "\n------------------------STATES------------------\n";
  std::cout << "NAME    INDEX     TRASH      FINAL      START\n";
  for (int i = 0; i < nfa.all_states_.size(); ++i) {
    std::cout << nfa.all_states_[i].name << "           "
              << nfa.all_states_[i].index << "            "
              << nfa.all_states_[i].IsTrash << "           "
              << nfa.all_states_[i].IsFinal << "             "
              << nfa.all_states_[i].isStart << "\n";
  }
  std::cout << "--------------------Transitions----------------------\n";
  std::cout << "START          PATH           END\n";
  for (int i = 0; i < nfa.transitions_.size(); ++i) {
    std::cout << nfa.transitions_[i].start << "      "
              << nfa.transitions_[i].path << "      " << nfa.transitions_[i].end
              << "\n";
  }
  return 0;
}