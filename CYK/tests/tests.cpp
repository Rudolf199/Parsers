#include <gtest/gtest.h>

#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>

#include "../src/cyk.hpp"

// →
TEST(mix_delete__Test, test) {
  std::vector<Rule> rules = {Rule("S→AABAA"), Rule("A→ABB"), Rule("B→bbb")};
  std::set<std::string> alph = {"a", "b"};
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  Chomsky chrome(grammar);
  chrome.remove_mixed();
  EXPECT_TRUE(chrome.get_grammar().get_mixed().empty());
}
TEST(const_char_test, test) {
  const char* text = "S→AAB";
  Rule rule = Rule(text);
  EXPECT_TRUE(5 == 5);
}

TEST(long_delete___Test, test) {
  std::vector<Rule> rules = {Rule("S→AAB"), Rule("A→a"), Rule("B→b")};
  std::set<std::string> alph = {"a", "b"};
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  Chomsky chrome(grammar);
  chrome.remove_long();
  EXPECT_TRUE(chrome.get_grammar().get_long().empty());
}
TEST(epsilon_delete__Test, test) {
  std::vector<Rule> rules = {Rule("S→AAB"), Rule("A→"), Rule("B→b")};
  std::set<std::string> alph = {"a", "b"};
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  Chomsky chrome(grammar);
  chrome.remove_epsilon();
  EXPECT_TRUE(chrome.get_grammar().get_epsilons().empty());
}
TEST(constructr, test){
  const char* text = "S→AAB";
  Rule rulek = Rule(text);
  EXPECT_EQ(rulek.right(), "AAB");
}
TEST(lonely_delete__Test, test) {
  std::vector<Rule> rules = {Rule("S→AAB"), Rule("A→B"), Rule("B→b")};
  std::set<std::string> alph = {"a", "b"};
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  Chomsky chrome(grammar);
  chrome.remove_lonely();
  EXPECT_TRUE(chrome.get_grammar().get_lonely().empty());
}
TEST(unreachable_delete___Test, test) {
  std::vector<Rule> rules = {Rule("S→B"), Rule("A→B"), Rule("B→b")};
  std::set<std::string> alph = {"a", "b"};
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  Chomsky chrome(grammar);
  chrome.remove_unreachable();
  EXPECT_TRUE(chrome.get_grammar().unreachable().empty());
}
TEST(non_generating__Test, test) {
  std::vector<Rule> rules = {Rule("S→AABC"), Rule("A→B"), Rule("B→b")};
  std::set<std::string> alph = {"a", "b"};
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  Chomsky chrome(grammar);
  chrome.remove_non_generating();
  EXPECT_TRUE(chrome.get_grammar().non_generating().empty());
}
TEST(algoritm_te__Test, test) {
  std::vector<Rule> rules = {Rule("S→AAB"), Rule("A→B"), Rule("B→b")};
  std::set<std::string> alph = {"a", "b"};
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  Chomsky chrome(grammar);
  chrome.bring_to_cnf();
  CYK parser("bbb", chrome);
  EXPECT_TRUE(parser.parse());
}
TEST(algorithm_te__Test, test) {
  std::vector<Rule> rules = {Rule("S→AAAB"), Rule("A→AA"), Rule("A→a"),
                             Rule("B→b")};
  std::set<std::string> alph = {"a", "b"};
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  Chomsky chrome(grammar);
  chrome.bring_to_cnf();
  CYK parser("aaab", chrome);
  EXPECT_TRUE(parser.parse());
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
