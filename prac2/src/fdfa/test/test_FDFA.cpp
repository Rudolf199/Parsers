#include <iostream>
#include <gtest/gtest.h>
#include "../src/FDFA.hpp"



TEST(fdfa_tests, fdfa_test){
    State state_1 = {"0", 1, 0, 0, 1, 0, "a"};
    State state_2 = {"1", 0, 1, 1, 1, 0, "b"};
    Transition transition_1 = {'a', 0, 1};
    Transition transition_2 = {'b', 1, 0};
    std::string alphabet = "ab";
    std::vector<State> states = {state_1, state_2};
    std::vector<Transition> transitions = {transition_1, transition_2};
    DFA dfa(transitions, states);
    dfa.alphabet = alphabet;
    dfa.MakeFDFA();
    EXPECT_EQ(4, dfa.all_states_.size());
    EXPECT_EQ(8, dfa.transitions_.size());
}