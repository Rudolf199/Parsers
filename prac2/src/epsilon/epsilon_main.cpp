#include <iostream>
#include "./src/epsilon.hpp"

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
  PrintNFAStates(nfa);
  PrintNFATransitions(nfa);
  nfa.EpsilonsDelete();
  PrintNFAStates(nfa);
  PrintNFATransitions(nfa);
  return 0;
}