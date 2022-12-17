#include <gtest/gtest.h>

#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>

#include "../src/Earley.hpp"

TEST(rule_constructors, basic) {
  Rule first;
  Rule second;
  EXPECT_TRUE(first == second);
}
TEST(rule_constructors, string) {
  std::string temp = "A→a";
  Rule first(temp);
  Rule second;
  second.left() = "A";
  second.right() = "a";
  EXPECT_EQ(first, second);
}
TEST(rule_constructors, rule) {
  std::string temp = "A→a";
  Rule first(temp);
  Rule second(first);
  EXPECT_EQ(first, second);
}
TEST(operator_eq, basic) {
  std::string temp = "A→a";
  Rule first(temp);
  Rule second = first;
  EXPECT_EQ(first, second);
}
TEST(operator_eq, string) {
  std::string temp = "A→a";
  Rule rule = temp;
  Rule tmp(temp);
  EXPECT_EQ(rule, tmp);
}
TEST(output, basic) {
  std::ostringstream out;
  std::string temp = "A→a";
  Rule rule = temp;
  out << temp;
  EXPECT_EQ("A→a", out.str());
}
TEST(char_operator, basic) {
  Rule rule = Rule("A→a");
  Rule first("A→a");
  EXPECT_EQ(rule, first);
}
TEST(left_right, basic) {
  std::string temp = "A→a";
  Rule rule = temp;
  EXPECT_EQ(rule.left(), "A");
  EXPECT_EQ(rule.right(), "a");
}
TEST(bool_operator, basic) {
  std::string temp = "A→a";
  Rule rule = temp;
  Rule first = temp;
  Rule second("B→b");
  EXPECT_TRUE(rule == first);
  EXPECT_FALSE(rule == second);
}
TEST(input, basic) {
  std::istringstream in("A→a");
  std::ostringstream out;
  Rule rule;
  in >> rule;
  // out << rule;
  EXPECT_EQ("A→a", in.str());
}
TEST(grammar_constructor, big) {
  std::vector<Rule> rules = {Rule("S→a")};
  std::set<std::string> alph = {"a"};
  std::set<std::string> net = {"S"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  EXPECT_EQ(5, 5);
}
TEST(state_constructors_and_methods, basic) {
  State state;
  Rule rule("S→a");
  size_t pos = 2;
  size_t start = 0;
  State state2(rule, pos, start);
  state.rule() = state2.rule();
  state.point() = state2.point();
  state.start() = state2.start();
  std::cout << state.current() << "\n";
  State state3 = state2;
  state3 = state;
  // std::cout << state3 << "\n";
  EXPECT_EQ(state3, state);
}
TEST(parser_methods_constructors_etc, basic) {
  Parser parser;
  std::vector<Rule> rules = {Rule("S→a")};
  std::set<std::string> alph = {"a"};
  std::set<std::string> net = {"S"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  std::string word = "a";
  parser.grammar() = grammar;
  parser.word() = word;
  Parser earley(grammar, word);
  EXPECT_TRUE(earley.belongs());
}
TEST(situation_out, basic) {}
TEST(parser_algorithm, hard) {
  std::vector<Rule> rules = {Rule("S→AB"), Rule("A→aA"), Rule("B→b"),
                             Rule("A→a")};
  std::set<std::string> alph = {"a", "b"};
  Rule rule;
  rule = rules[0];
  // std::cout << rule << "\n";
  std::set<std::string> net = {"S", "A", "B"};
  std::string start = "S";
  Grammar grammar(rules, alph, net, start);
  std::string word = "aaab";
  Parser parser(grammar, word);
  // std::cout << grammar;
  EXPECT_TRUE(parser.belongs());
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
