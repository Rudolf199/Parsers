#include <algorithm>
#include <iostream>
#include <vector>

#include "Grammar.hpp"

class State {
 private:
  Rule rule_;
  size_t point_ = 0;  // non terminal or neterminal
  size_t start_ = 0;
  std::string left_;
  std::string right_;

 public:
  State(const Rule& rule_, const size_t& pos_, const size_t& start_)
      : rule_(rule_), point_(pos_), start_(start_) {
    left_ = rule_.left();
    right_ = rule_.right();
  }
  State() = default;
  Rule& rule() { return rule_; }
  Rule rule() const { return rule_; }
  size_t& start() { return start_; }
  size_t& point() { return point_; }
  size_t point() const { return point_; }
  size_t start() const { return start_; }
  bool succeeded() const { return point_ == right_.size(); }
  std::string current() const { return std::string(1, right_[point_]); }
  bool operator<(const State& other) const {
    if (this->rule() == other.rule()) {
      if (point_ == other.point()) {
        return start() < other.start();
      }
      return point() < other.point();
    }
    return this->rule() < other.rule();
  }
  bool operator==(const State& other) const {
    bool res = (rule_ == other.rule()) && (point_ == other.point()) &&
               (start_ == other.start());
    return res;
  }
  State(State&& other) = default;
  State& operator=(const State& other) = default;
  State& operator=(State&& other) = default;
  friend std::ostream& operator<<(std::ostream& out, const State& situation) {
    out << situation.rule_ << "\n";
    out << situation.right_ << '\n';
    out << situation.left_ << "\n";
    out << situation.start_ << "\n";
    out << situation.point_ << "\n";
    return out;
  }
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
  Grammar grammar() const;
  bool belongs() {
    build();
    std::string checkstr = def_start + "→" + grammar_.start();
    Rule check = checkstr;
    State sit(check, 1, 0);
    auto it = std::find(situations_[word_.size()].begin(),
                        situations_[word_.size()].end(), sit);
    return it != situations_[word_.size()].end();
  }
  ~Parser() = default;
  void clear() { situations_.clear(); }
  Parser() = default;
  Parser(const Grammar& grammar, const std::string& word)
      : grammar_(grammar), word_(word) {
    std::string checkstr = def_start + "→" + grammar_.start();
    def_first_ = checkstr;
    grammar_.insert(def_first_);
    grammar_.neterminals().insert(def_start);
  }
  void add(size_t j, const Rule& rule, size_t point, size_t index);
};

void Parser::build() {
  situations_.resize(word_.size() + 1);
  situations_[0].emplace(def_first_, 0, 0);
  int64_t size = -1;
  for (size_t i = 0; i < word_.size() + 1; ++i) {
    scan(i);
    size = -1;
    while (size != situations_[i].size()) {
      size = situations_[i].size();
      complete(i);
      predict(i);
    }
  }
}
void Parser::add(size_t j, const Rule& rule, size_t point, size_t index) {
  situations_[j].emplace(rule, point, index);
}
void Parser::scan(size_t j) {
  if (j == 0) {
    return;
  }
  for (const auto& elem : situations_[j - 1]) {  // after dot symbol a
    if (std::string(1, word_[j - 1]) == elem.current()) {
      add(j, elem.rule(), elem.point() + 1, elem.start());
    }
  }
}
void Parser::complete(size_t j) {
  for (const auto& situation : situations_[j]) {
    if (situation.succeeded()) {
      for (auto& prev : situations_[situation.start()]) {
        if (prev.current() == situation.rule().left()) {
          add(j, prev.rule(), prev.point() + 1, prev.start());
        }
      }  // starting from empty dot // for (A → α · Bβ, k) ∈ Di do
    }
  }
}
void Parser::predict(size_t j) {
  for (const auto& situation : situations_[j]) {
    std::string start = situation.current();
    for (const auto& elem : grammar_.starting_with(start)) {
      add(j, elem, 0, j);
    }
  }
}