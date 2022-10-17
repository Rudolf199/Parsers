#include <gtest/gtest.h>

#include <iostream>

#include "../src/DFA.hpp"

TEST(DFA_TEST, dfa_transitions) {
  State state1 = {"0", 1, 0, 0, 0};
  State state2 = {"1", 0, 0, 1, 0};
  State state3 = {"2", 0, 1, 2, 0};
  std::string alphabet = "ab";
  Transition transition1 = {'a', 0, 0};
  Transition transition2 = {'b', 0, 0};
  Transition transition3 = {'a', 0, 1};
  Transition transition4 = {'b', 1, 2};
  std::vector<State> states = {state1, state2, state3};
  std::vector<Transition> transitions = {transition1, transition2, transition3,
                                         transition4};
  NFA nfa(transitions, states);
  DFA dfa1(transitions, states);
  EXPECT_EQ(false, dfa1.IsDFA(alphabet));
  Table table = nfa.MakeTable();
  DFA dfa = ConvertDFA(table);
  EXPECT_EQ(true, dfa.IsDFA(alphabet));
}