#include <iostream>

#include "./src/DFA.hpp"

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