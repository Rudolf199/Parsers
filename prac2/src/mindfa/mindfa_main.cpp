#include <iostream>
#include "./src/minDFA.hpp"

int main() {
  std::cout << "Enter the number of states:\n";
  int number_of_states;
  std::cin >> number_of_states;
  std::vector<State> states = StatesInit(number_of_states);
  std::cout << "Enter number of transitions:\n";
  int num_of_transitions;
  std::cin >> num_of_transitions;
  std::cout << "Enter alphabet\n";
  std::string alphabet;
  std::cin >> alphabet;
  std::vector<Transition> transitions =
      TransitionsInit(num_of_transitions, states, alphabet);
  DFA dfa(transitions, states, alphabet);
  dfa.PrintDFA();
  // dfa.Inversion();
  // dfa.PrintDFA();
  Groups classes = Groups();
  classes = dfa.MinDFAgroups();
  printclasses(classes);
  DFA mindfa = dfa.MinDFA(classes);
  mindfa.PrintDFA();
  return 0;
}