#include <gtest/gtest.h>

#include <iostream>

#include "../src/addition.hpp"

TEST(addition_test, addition) {
  std::string alphabet = "ab";
  State state = {"0", 1, 0, 0, 2, 0, "ab"};
  State state_2 = {"1", 0, 0, 1, 2, 0, "ab"};
  State state_3 = {"2", 0, 1, 2, 2, 0, "ab"};
  Transition transition_1 = {'a', 0, 1};
  Transition transition_2 = {'b', 0, 1};
  Transition transition_3 = {'a', 1, 2};
  Transition transition_4 = {'b', 1, 2};
  Transition transition_5 = {'a', 2, 2};
  Transition transition_6 = {'b', 2, 2};
  std::vector<State> states = {state, state_2, state_3};
  std::vector<Transition> transitions = {transition_1, transition_2,
                                         transition_3, transition_4,
                                         transition_5, transition_6};

  DFA dfa(transitions, states, alphabet);
  EXPECT_EQ(1, dfa.Finals());
  dfa.Inversion();
  EXPECT_EQ(2, dfa.Finals());
}