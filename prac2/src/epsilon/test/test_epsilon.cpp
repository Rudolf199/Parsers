#include <gtest/gtest.h>

#include <iostream>

#include "../src/epsilon.hpp"

TEST(Epsilon_test, Epsilons_are_merged) {
  State state1 = {"0", true, false, 0, false};
  State state2 = {"1", false, false, 1, false};
  State state3 = {"2", false, true, 2, false};
  std::vector<State> states;
  states.push_back(state1);
  states.push_back(state2);
  states.push_back(state3);
  std::vector<Transition> transitions;
  Transition transition1 = {'$', 0, 1};
  Transition transition2 = {'a', 1, 2};
  transitions.push_back(transition1);
  transitions.push_back(transition2);
  NFA nfa(transitions, states);
  EXPECT_EQ(false, nfa.EpsilonsAreMerged());
  nfa.EpsilonsDelete();
  EXPECT_EQ(true, nfa.EpsilonsAreMerged());
}
