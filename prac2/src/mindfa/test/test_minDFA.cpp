#include <gtest/gtest.h>

#include <iostream>

#include "../src/minDFA.hpp"

TEST(mindfa_test, mindfa) {
  State state_1 = {"0", 1, 0, 0, 2, 0, "ab", "12", 0};
  State state_2 = {"1", 0, 0, 1, 2, 0, "ab", "3", 0};
  State state_3 = {"2", 0, 0, 2, 2, 0, "ab", "3", 0};
  State state_4 = {"3", 0, 1, 3, 2, 0, "ab", "3", 0};
  std::vector<State> states = {state_1, state_2, state_3, state_4};
  std::vector<Transition> transitions = {{'a', 0, 1}, {'b', 0, 2}, {'a', 1, 3},
                                         {'b', 1, 3}, {'a', 2, 3}, {'b', 2, 3},
                                         {'a', 3, 3}, {'b', 3, 3}};
  std::string alphabet = "ab";
  DFA dfa(transitions, states, alphabet);
  Groups classes = dfa.MinDFAgroups();
  DFA mindfa = dfa.MinDFA(classes);
  EXPECT_EQ(3, mindfa.all_states_.size());
}