#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Rule {
 public:
  Rule() = default;
  Rule(const std::string& src);
  Rule(const Rule& other) = default;
  Rule(const char* str);
  friend std::istream& operator>>(std::istream& in, Rule& rule);
  Rule& operator=(const Rule& other);
  Rule& operator=(const std::string& src);
  Rule& operator=(const char* src);
  // friend std::ostream& operator<<(std::ostream& out, const Rule& rule);
  std::string& left() { return left_part_; }
  std::string& right() { return right_part_; }
  std::string left() const { return left_part_; }
  std::string right() const { return right_part_; }
  bool operator<(const Rule& other) const;
  bool operator==(const Rule& other) const;
  // Rule(Rule&& other) = default;

 private:
  std::string left_part_;
  std::string right_part_;
  inline static const std::string delimiter = "→";
};

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
  // friend std::ostream& operator<<(std::ostream& out, const Grammar& grammar);
  Grammar(const Grammar&) = default;
  Grammar(Grammar&&) = default;
  Grammar& operator=(const Grammar&) = default;
  Grammar& operator=(Grammar&&) = default;
  std::vector<Rule> starting_with(const std::string& start) const;

 private:
  std::set<std::string> alphabet_;
  std::vector<Rule> rules_;
  std::set<std::string> neterminals_;
  std::string start_;
  size_t size_ = 0;
};

class State {
 private:
  Rule rule_;
  size_t point_ = 0;  // non terminal or neterminal
  size_t start_ = 0;
  std::string left_;
  std::string right_;

 public:
  State(const Rule& rule_, const size_t& pos_, const size_t& start_);
  State() = default;
  State(const State& src) = default;
  Rule& rule() { return rule_; }
  Rule rule() const { return rule_; }
  size_t& start() { return start_; }
  size_t& point() { return point_; }
  size_t point() const { return point_; }
  size_t start() const { return start_; }
  bool succeeded() const { return point_ == right_.size(); }
  std::string current() const { return std::string(1, right_[point_]); }
  bool operator<(const State& other) const;
  bool operator==(const State& other) const;
  State(State&& other) = default;
  State& operator=(const State& other) = default;
  State& operator=(State&& other) = default;
  // friend std::ostream& operator<<(std::ostream& out, const State& situation);
};

class Parser {
 private:
  Grammar grammar_;
  std::string word_;
  inline const static std::string def_start = "Ñ";
  std::vector<std::set<State>> situations_;
  void scan(size_t j);
  void predict(size_t j);
  void complete(size_t j);
  Rule def_first_;
  void build();

 public:
  Grammar& grammar();  // to change
  const Grammar& grammar() const;
  const std::string& word() const;
  std::string& word(); 
  bool belongs();
  ~Parser() = default;
  void clear() { situations_.clear(); }
  Parser() = default;
  Parser(const Grammar& grammar, const std::string& word);
  void add(size_t j, const Rule& rule, size_t point, size_t index);
};