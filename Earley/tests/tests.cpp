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
  out << rule;
  EXPECT_EQ(out.str(), in.str());
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
