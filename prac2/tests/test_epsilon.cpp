#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "../headers/test_epislon.hpp"
#define CATCH_CONFIG_MAIN

TEST_CASE("Epsilons are merged", "[NFA]") {
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
  REQUIRE(nfa.EpsilonsAreMerged() == false);
  nfa.EpsilonsDelete();
  REQUIRE(nfa.EpsilonsAreMerged() == true);
}
