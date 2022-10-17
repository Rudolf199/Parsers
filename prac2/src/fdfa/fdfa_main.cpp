#include <iostream>
#include "./src/FDFA.hpp"

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