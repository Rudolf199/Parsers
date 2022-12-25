#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Rule {
 public:
  Rule() = default;
  Rule(const std::string& src);
  Rule(const Rule& other) = default;
  explicit Rule(const char* str);
  friend std::istream& operator>>(std::istream& in, Rule& rule);
  Rule& operator=(const Rule& other);
  Rule& operator=(const std::string& src);
  Rule& operator=(const char* src);
  friend std::ostream& operator<<(std::ostream& out, const Rule& rule);
  std::string& left() { return left_part_; }
  std::string& right() { return right_part_; }
  std::string left() const { return left_part_; }
  std::string right() const { return right_part_; }
  bool operator<(const Rule& other) const;
  bool operator==(const Rule& other) const;
  Rule(Rule&& other) = default;

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
  void remove(size_t index);
  friend std::istream& operator>>(std::istream& in, Grammar& grammar);
  friend std::ostream& operator<<(std::ostream& out, const Grammar& grammar);
  Grammar(const Grammar&) = default;
  Grammar(Grammar&&) = default;
  Grammar& operator=(const Grammar&) = default;
  Grammar& operator=(Grammar&&) = default;
  std::vector<Rule> starting_with(const std::string& start) const;
  // std::vector<Rule&> starting_with(const std::string& start);
  std::vector<Rule> ending_with(const std::string& end) const;
  // std::vector<Rule&> ending_with(const std::string& end);
  // std::vector<size_t> ending_with_index(const std::string& end) const;
  // std::vector<size_t> starting_with_index(const std::string& start) const;
  std::vector<std::string> non_generating() const;
  std::vector<std::string> unreachable();
  void build_adj();
  void dfs(std::string& current);
  void add(const Rule& rule);
  std::vector<Rule> ending_with_single(const std::string& end) const;
  std::vector<Rule> get_mixed();
  std::vector<std::string> get_epsilons();
  std::vector<std::string> get_lonely();
  std::vector<Rule> get_long();
  static std::string remove_lower(std::string& str);
  static std::string remove_upper(const std::string& str);
  static bool is_mixed(const std::string& s);
  static bool is_long(const std::string& s);

 private:
  std::set<std::string> alphabet_;
  std::vector<Rule> rules_;
  std::set<std::string> neterminals_;
  std::string start_;
  size_t size_ = 0;
  std::map<std::string, bool> visited_;
  std::map<std::string, std::list<std::string>> adj_;
};

class Chomsky {
 public:
  Chomsky() = default;
  Chomsky(const std::set<std::string>& alphabet);
  Chomsky(const Grammar& grammar);
  Chomsky(const Grammar& grammar, bool epsilon);
  Chomsky(const Chomsky& other) { *this = other; }
  Grammar& get_grammar() { return grammar_; }
  Grammar get_grammar() const { return grammar_; }
  void remove_non_generating();
  void remove_unreachable();
  void remove_mixed();
  void remove_long();
  Rule ending_with(const std::string& end);
  void remove_epsilon();
  void remove_lonely();
  void bring_to_cnf();
  static bool is_upper(const std::string& s);
  static bool is_lower(const std::string& s);
  static bool is_mixed(const std::string& s);
  static bool is_long(const std::string& s);
  static std::string remove_lower(const std::string& str);
  static std::string remove_upper(const std::string& str);

 private:
  bool epsilon_word = true;
  std::vector<Rule> sub_rules_;
  std::vector<Rule> shorts_;
  off_t offset = 0;
  off_t long_set = 0;
  /*if (offset + 1 > add_neterminals.size()) {
    std::cout << "What kind of grammar is this\n";
    abort();
  }*/
  void make_short(Rule& src);
  Grammar grammar_;
  template <typename T>
  static std::vector<T> findDiff(std::vector<T> x, std::vector<T> y);
};

template <typename T>
std::vector<T> Chomsky::findDiff(std::vector<T> x,
                                 std::vector<T> y) {  // no-ref, no-const
  std::vector<T> diff;
  std::sort(x.begin(), x.end());
  std::sort(y.begin(), y.end());
  std::set_difference(x.begin(), x.end(), y.begin(), y.end(),
                      std::back_inserter(diff));
  return diff;
}

struct Data {
  std::string net;
  int i;
  int j;
  Data(const std::string& src, int i, int j) : net(src), i(i), j(j) {}
};
using tuple = std::tuple<std::string, int, int>;
using table = std::map<tuple, bool>;
class CYK {
 private:
  table T_;
  std::string word_;
  Chomsky cnf_;

 public:
  CYK() = default;
  CYK(const std::string& src, const Chomsky& other);
  bool parse();
};