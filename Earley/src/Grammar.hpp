#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Rule.hpp"

class Grammar {
 public:
  Grammar() = default;
  Grammar(const std::vector<Rule>& rules, const std::set<std::string>& alph,
          const std::set<std::string>& net, std::string start);  // done
  std::string start() const { return start_; }
  std::string& start() { return start_; }
  std::set<std::string> sigma() const { return alphabet_; }  // alphabet
  std::vector<Rule> rules() const { return rules_; }         // rules
  std::set<std::string> neterminals() const { return neterminals_; }
  std::set<std::string>& sigma() { return alphabet_; }  // alphabet
  std::vector<Rule>& rules() { return rules_; }         // rules
  std::set<std::string>& neterminals() { return neterminals_; }
  void insert(Rule& new_rule);
  friend std::istream& operator>>(std::istream& in, Grammar& grammar);
  friend std::ostream& operator<<(std::ostream& out, const Grammar& grammar);
  Grammar(const Grammar&) = default;
  Grammar(Grammar&&) = default;
  Grammar& operator=(const Grammar&) = default;
  Grammar& operator=(Grammar&&) = default;
  std::vector<Rule> starting_with(const std::string& start) const {
    std::vector<Rule> res;
    for (const auto& rule : rules_) {
      if (start == rule.left()) {
        res.push_back(rule);
      }
    }
    return res;
  }

 private:
  std::set<std::string> alphabet_;
  std::vector<Rule> rules_;
  std::set<std::string> neterminals_;
  std::string start_;
  size_t size_ = 0;
};

Grammar::Grammar(const std::vector<Rule>& rules,
                 const std::set<std::string>& alph,
                 const std::set<std::string>& net, std::string start) {
  rules_ = rules;
  alphabet_ = alph;
  size_ = rules_.size();
  neterminals_ = net;
  start_ = start;
}
std::istream& operator>>(std::istream& in, Grammar& grammar) {
  std::cout << "Enter alphabet size\n";
  size_t size = 0;
  in >> size;
  grammar.size_ = size;
  char symbol = 0;
  std::cout << "Enter alphabet\n";
  for (size_t i = 0; i < size; ++i) {
    in >> symbol;
    grammar.alphabet_.insert(std::string(1, symbol));
  }
  std::cout << "Neterminals set size?\n";
  in >> size;
  std::cout << "Enter neterminals\n";
  for (size_t i = 0; i < size; ++i) {
    in >> symbol;
    grammar.neterminals_.insert(std::string(1, symbol));
  }
  std::cout << "Which neterminal is start?\n";
  in >> symbol;
  grammar.start_ = std::string(1, symbol);
  std::cout << "How many rules are there?\n";
  in >> size;
  std::cout << "Enter rules !! use this symbol for delimiter!!! →\n";
  grammar.rules_ = std::vector<Rule>(size);
  for (auto& elem : grammar.rules_) {
    in >> elem;
  }
  return in;
}
std::ostream& operator<<(std::ostream& out, const Grammar& grammar) {
  out << "SIZE\n" << grammar.size_ << "\n";
  out << "START\n" << grammar.start_ << '\n';
  out << "ALPHABET\n";
  for (auto& elem : grammar.alphabet_) {
    out << elem << " ";
  }
  out << "\nNETERMINALS\n";
  for (auto& elem : grammar.neterminals_) {
    out << elem << " ";
  }
  out << "\nRULES\n";
  for (auto& elem : grammar.rules_) {
    out << elem << "\n";
  }
  return out;
}
void Grammar::insert(Rule& new_rule) {
  size_++;
  auto ps = neterminals_.find(new_rule.left());
  if (ps == neterminals_.end()) {
    neterminals_.insert(new_rule.left());
  }
  for (auto& elem : new_rule.right()) {
    auto aps = alphabet_.find(std::string(1, elem));
    if (aps == alphabet_.end() && islower(elem)) {
      std::cout << "ERROR : alphabet crack!!\n";
      abort();
    }
    auto nps = neterminals_.find(std::string(1, elem));
    if (nps == neterminals_.end()) {
      std::cout << "New neterminal !!! -> " << elem << "\n";
      neterminals_.insert(std::string(1, elem));
    }
  }
}