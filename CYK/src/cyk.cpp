#include "cyk.hpp"

std::vector<std::string> add_neterminals = {"1", "2", "3", "4", "5", "6",
                                            "7", "8", "9", "0", "?", "#",
                                            "~", "*", "&", "@", "$"};
std::vector<std::string> for_longs = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
/////////////////////////////////////////

Rule::Rule(const char* str) {
  std::string string(str);
  *this = string;
}
Rule& Rule::operator=(const Rule& other) {
  left_part_ = other.left_part_;
  right_part_ = other.right_part_;
  return *this;
}
Rule& Rule::operator=(const std::string& src) {
  *this = Rule(src);
  return *this;
}
Rule& Rule::operator=(const char* src) {
  std::string name = src;
  return *this = name;
}
Rule::Rule(const std::string& src) {
  std::string start = src.substr(0, src.find(delimiter));
  left_part_ = start;
  right_part_ = src.substr(src.find(delimiter) + delimiter.size());
}
bool Rule::operator<(const Rule& other) const {
  if (left_part_ == other.left_part_) {
    return right_part_ < other.right_part_;
  }
  return left_part_ < other.left_part_;
}
bool Rule::operator==(const Rule& other) const {
  return (!(*this < other) && !(other < *this));
}
std::istream& operator>>(std::istream& in, Rule& rule) {
  std::string r;
  in >> r;
  rule = Rule(r);
  return in;
}
std::ostream& operator<<(std::ostream& out, const Rule& rule) {
  out << rule.left_part_ << rule.delimiter << rule.right_part_;
  return out;
}
std::vector<Rule> Grammar::get_mixed() {
  std::vector<Rule> rules;
  for (auto& rule : rules_) {
    if (is_mixed(rule.right())) {
      rules.push_back(rule);
    }
  }
  return rules;
}
void Chomsky::remove_epsilon() {
  std::vector<std::string> to_delete = grammar_.get_epsilons();
  // Parser parser(grammar_, "");
  if (to_delete.empty()) {
    return;
  }
  std::vector<Rule> to_add_later;
  if (epsilon_word) {
    std::string new_start = "Ñ";
    Rule new_rule = std::string(new_start + "→" + "");
    Rule new_start_rule = std::string(new_start + "→" + grammar_.start());
    // grammar_.add(new_start_rule);
    // grammar_.add(new_rule);
    to_add_later.push_back(new_start_rule);
    to_add_later.push_back(new_rule);
  }
  for (auto& elem : to_delete) {
    for (int i = 0; i < grammar_.rules().size(); ++i) {
      if (grammar_.rules()[i] == Rule(elem + "→" + "")) {
        grammar_.rules().erase(std::find(grammar_.rules().begin(),
                                         grammar_.rules().end(),
                                         Rule(elem + "→" + "")));
        break;
      }
    }
    for (auto& rule : grammar_.ending_with(elem)) {
      std::string new_right = rule.right();
      new_right.erase(std::remove(new_right.begin(), new_right.end(), elem[0]),
                      new_right.end());
      Rule temp = std::string(rule.left() + "→" + new_right);
      std::cout << temp << "\n";
      grammar_.add(temp);
    }
  }
  remove_epsilon();
  for (auto& elem : to_add_later) {
    grammar_.add(elem);
  }
  std::cout << "result : \n" << grammar_;
}
void Chomsky::remove_lonely() {
  std::vector<std::string> res = grammar_.get_lonely();
  for (auto& elem : res) {
    std::cout << " lonely\n" << elem << "\n";
  }
  for (size_t j = 0; j < res.size(); ++j) {
    for (size_t i = 0; i < grammar_.ending_with(res[j]).size(); ++i) {
      // for (auto& elem : grammar_.ending_with(trash)) {
      char cpy = grammar_.starting_with(res[j])[0].right()[0];
      Rule temp = grammar_.ending_with(res[j])[i];
      std::string current = temp.right();
      std::replace(current.begin(), current.end(), res[j][0], cpy);
      // temp.right() = current;
      auto it =
          std::find(grammar_.rules().begin(), grammar_.rules().end(), temp);
      (*it).right() = current;
      grammar_.rules().erase(std::find(grammar_.rules().begin(),
                                       grammar_.rules().end(),
                                       grammar_.starting_with(res[j])[0]));
      grammar_.neterminals().erase(grammar_.neterminals().find(res[j]));
      i--;
    }
  }
  std::cout << "result\n" << grammar_;
}
void Chomsky::make_short(Rule& src) {
  if (src.right().size() <= 2) {
    return;
  }
  if (long_set > for_longs.size()) {
    // std::cout << " too many nets, not enough alphabet SEGV\n";
    // abort();
  }
  std::cout << "current: " << src << "\n";
  Rule cpy = src;
  auto fx = std::find(grammar_.rules().begin(), grammar_.rules().end(), src);
  std::string temp = src.right();
  Rule fix = Rule(src.left() + "→" + for_longs[long_set] + temp.back());
  std::cout << fix << "\n";
  src = fix;
  std::cout << "after fix " << src << '\n';
  *fx = fix;
  std::cout << "iterator after fix" << *fx << "\n";
  if (std::find(grammar_.rules().begin(), grammar_.rules().end(), cpy) !=
      grammar_.rules().end()) {
    // std::cout << "its still here"
    //           << *std::find(grammar_.rules().begin(), grammar_.rules().end(),
    //                         cpy)
    //          << "\n";
  }
  temp.pop_back();
  Rule new_rule_cpy(for_longs[long_set] + "→" + temp);
  Rule& new_rule = new_rule_cpy;
  grammar_.neterminals().emplace(for_longs[long_set]);
  // auto it = std::find(grammar_.rules().begin(), grammar_.rules().end(), cpy);
  grammar_.rules().push_back(new_rule);
  long_set++;
  make_short(new_rule);
}
void Chomsky::remove_long() {
  for (size_t i = 0; i < grammar_.rules().size(); ++i) {
    if (is_long(grammar_.rules()[i].right())) {
      Rule& current = grammar_.rules()[i];
      make_short(current);
      i = 0;
    }
  }
  std::cout << "result : \n" << grammar_;
}
Rule Chomsky::ending_with(const std::string& end) {
  for (const auto& rule : sub_rules_) {
    if (rule.right() == end) {
      return rule;
    }
  }
  Rule rule;
  return rule;
}
Chomsky::Chomsky(const std::set<std::string>& alphabet) {
  size_t i = 0;
  for (const auto& elem : alphabet) {
    if (i > add_neterminals.size()) {
      // std::cout << "What kind of grammar is this\n";
      // abort();
    }
    Rule rule(add_neterminals[i] + "→" + elem);
    sub_rules_.push_back(rule);
    ++i;
  }
  std::cout << "Does this grammar reach epsilon word?\nPlease check "
               "twice!!\ntype YES or NO\n";
  std::string str;
  std::cin >> str;
  epsilon_word = (str == "YES");
}
Chomsky::Chomsky(const Grammar& grammar, bool epsilon) {
  grammar_ = grammar;
  size_t i = 0;
  for (const auto& elem : grammar.sigma()) {
    if (i > add_neterminals.size()) {
      // std::cout << "What kind of grammar is this\n";
      // abort();
    }
    Rule rule(add_neterminals[i] + "→" + elem);
    sub_rules_.push_back(rule);
    ++i;
  }
  std::vector<std::string> temp;
  for (auto& elem : grammar_.neterminals()) {
    temp.push_back(elem);
  }
  for_longs = findDiff(for_longs, temp);
  std::cout << "Does this grammar reach epsilon word?\nPlease check "
               "twice!!\ntype YES or NO\n";
  std::string str;
  std::cin >> str;
  epsilon_word = epsilon;
}
Chomsky::Chomsky(const Grammar& grammar) {
  grammar_ = grammar;
  size_t i = 0;
  for (const auto& elem : grammar.sigma()) {
    if (i > add_neterminals.size()) {
      // std::cout << "What kind of grammar is this\n";
      // abort();
    }
    Rule rule(add_neterminals[i] + "→" + elem);
    sub_rules_.push_back(rule);
    ++i;
  }
  std::vector<std::string> temp;
  for (auto& elem : grammar_.neterminals()) {
    temp.push_back(elem);
  }
  for_longs = findDiff(for_longs, temp);
  std::cout << "Does this grammar reach epsilon word?\nPlease check "
               "twice!!\ntype YES or NO\n";
  std::string str;
  std::cin >> str;
  epsilon_word = (str == "YES");
}

void Chomsky::remove_unreachable() {
  std::vector<std::string> trash = grammar_.unreachable();
  if (trash.empty()) {
    return;
  }
  std::vector<Rule> trash_rules;
  std::vector<Rule> trash_rules_2;
  for (const auto& elem : trash) {
    for (const auto& rule : grammar_.starting_with(elem)) {
      trash_rules.push_back(rule);
    }
  }
  for (const auto& elem : trash) {
    for (const auto& rule : grammar_.ending_with(elem)) {
      trash_rules_2.push_back(rule);
    }
  }
  trash_rules.erase(std::unique(trash_rules.begin(), trash_rules.end()),
                    trash_rules.end());
  trash_rules_2.erase(std::unique(trash_rules_2.begin(), trash_rules_2.end()),
                      trash_rules_2.end());
  grammar_.rules() = findDiff(grammar_.rules(), trash_rules);
  grammar_.rules() = findDiff(grammar_.rules(), trash_rules_2);
  std::set<std::string> temp;
  for (auto& elem : trash) {
    temp.emplace(elem);
  }
  std::set<std::string> new_temp;
  std::set_difference(grammar_.neterminals().begin(),
                      grammar_.neterminals().end(), temp.begin(), temp.end(),
                      std::inserter(new_temp, new_temp.end()));
  grammar_.neterminals() = new_temp;
  trash = grammar_.unreachable();
  if (!trash.empty()) {
    remove_unreachable();
  }
  std::cout << "RESULT :\n";
  std::cout << grammar_;
}
void Chomsky::remove_mixed() {
  std::vector<Rule> to_add;
  // for (auto& rule : grammar_.rules()) {
  for (size_t i = 0; i < grammar_.rules().size(); ++i) {
    Rule& rule = grammar_.rules()[i];
    if (is_mixed(rule.right())) {
      // for (auto& symbol : rule.right()) {
      std::string& right = rule.right();
      for (size_t j = 0; j < right.size(); ++j) {
        char temp = right[j];
        if (islower(temp)) {
          if (to_add.end() != std::find(to_add.begin(), to_add.end(),
                                        ending_with(std::string(1, temp)))) {
            std::replace(rule.right().begin(), rule.right().end(), temp,
                         ending_with(std::string(1, temp)).left()[0]);
            j = -1;
            continue;
          }
          char new_symbol = ending_with(std::string(1, temp)).left()[0];
          std::replace(rule.right().begin(), rule.right().end(), temp,
                       new_symbol);
          to_add.push_back(ending_with(std::string(1, temp)));
          j = -1;
          // grammar_.add(ending_with(std::string(1, symbol)));
        }
      }
    }
  }
  for (const auto& it : to_add) {
    grammar_.add(it);
  }
  std::cout << "result : \n";
  std::cout << grammar_;
}
void Chomsky::remove_non_generating() {
  std::vector<std::string> trash = grammar_.non_generating();
  if (trash.empty()) {
    return;
  }
  if (std::find(trash.begin(), trash.end(), grammar_.start()) != trash.end()) {
    // std::cout << "cringe grammar try better\n";
    // abort();
  }
  std::vector<Rule> trash_rules;
  for (const auto& elem : trash) {
    for (const auto& rule : grammar_.ending_with(elem)) {
      trash_rules.push_back(rule);
    }
  }
  trash_rules.erase(std::unique(trash_rules.begin(), trash_rules.end()),
                    trash_rules.end());
  grammar_.rules() = findDiff(grammar_.rules(), trash_rules);
  std::set<std::string> temp;
  for (auto& elem : trash) {
    temp.emplace(elem);
  }
  std::set<std::string> new_temp;
  std::set_difference(grammar_.neterminals().begin(),
                      grammar_.neterminals().end(), temp.begin(), temp.end(),
                      std::inserter(new_temp, new_temp.end()));
  grammar_.neterminals() = new_temp;
  trash = grammar_.non_generating();
  if (!trash.empty()) {
    remove_non_generating();
  }
  std::cout << "RESULT :\n";
  std::cout << grammar_;
}

bool Chomsky::is_upper(const std::string& s) {
  return std::all_of(s.begin(), s.end(),
                     [](unsigned char c) { return std::isupper(c); });
}
bool Chomsky::is_lower(const std::string& s) {
  return std::all_of(s.begin(), s.end(),
                     [](unsigned char c) { return std::islower(c); });
}
bool Chomsky::is_mixed(const std::string& s) {
  return (remove_upper(s).size() != s.size()) && (!remove_upper(s).empty());
}
bool Chomsky::is_long(const std::string& s) { return s.size() > 2; }
std::string Chomsky::remove_lower(const std::string& str) {
  const std::regex pattern("[a-z]");
  return regex_replace(str, pattern, "");
}
std::vector<Rule> Grammar::get_long() {
  std::vector<Rule> rules;
  for (auto& rule : rules_) {
    if (is_long(rule.right())) {
      rules.push_back(rule);
    }
  }
  return rules;
}
bool Grammar::is_long(const std::string& s) { return s.size() > 2; }
std::string Chomsky::remove_upper(const std::string& str) {
  const std::regex pattern("[A-Z]");
  return regex_replace(str, pattern, "");
}
void Chomsky::bring_to_cnf() {
  bool is_ok = true;
  for (auto& rule : grammar_.rules()) {
    if (is_mixed(rule.right())) {
      is_ok = false;
    }
    if (is_long(rule.right())) {
      is_ok = false;
    }
    if (rule.right() == "" && rule.left() != grammar_.start()) {
      is_ok = false;
    }
    if (rule.right().size() == 1 && is_upper(rule.right())) {
      is_ok = false;
    }
    if (!grammar_.unreachable().empty()) {
      is_ok = false;
    }
    if (!grammar_.non_generating().empty()) {
      is_ok = false;
    }
  }
  if (is_ok) {
    return;
  }
  remove_non_generating();
  remove_unreachable();
  remove_mixed();
  remove_long();
  remove_epsilon();
  remove_lonely();
}

CYK::CYK(const std::string& src, const Chomsky& other)
    : word_(src), cnf_(other) {
  cnf_.bring_to_cnf();
}
bool CYK::parse() {
  for (auto& rule : cnf_.get_grammar().rules()) {
    if (rule.right().size() == 1) {
      for (size_t i = 0; i < word_.size(); ++i) {
        std::cout << "current letter " << word_[i] << "\n";
        if (rule.right()[0] == word_[i]) {
          T_[tuple(rule.left(), i, i + 1)] = true;
        } else {
          if (T_[tuple(rule.left(), i, i + 1)] != true)
            T_[tuple(rule.left(), i, i + 1)] = false;
        }
      }
    }
  }
  for (auto& elem : T_) {
    std::cout << std::get<0>(elem.first) << " " << std::get<1>(elem.first)
              << " " << std::get<2>(elem.first) << " value " << elem.second
              << "\n";
  }
  std::reverse(cnf_.get_grammar().rules().begin(),
               cnf_.get_grammar().rules().end());
  for (size_t len = 2; len <= word_.size(); ++len) {
    for (size_t i = 0; i <= word_.size() - len; ++i) {
      size_t j = i + len;
      for (size_t k = i + 1; k <= j - 1; ++k) {
        for (auto& rule : cnf_.get_grammar().rules()) {
          if (rule.right().size() == 2) {
            std::cout << "possible posotions\n";
            for (size_t r = 0; r <= word_.size(); ++r) {
              std::cout << r << " ";
            }
            std::cout << '\n';
            std::cout << rule << "\n";
            std::string B = std::string(1, rule.right()[0]);
            std::string C = std::string(1, rule.right()[1]);
            std::string A = rule.left();
            std::cout << "B : " << B << "\nC : " << C << "\ni : " << i
                      << "\nj : " << j << "\nk : " << k << "\n";
            std::cout << "checking if " << A << "|-" << word_[i] << ".."
                      << word_[j] << "from " << i << " to " << j << "\n";
            std::cout << "T_[tuple(B, i, k)] " << T_[tuple(B, i, k)] << "\n";
            std::cout << "T_[tuple(C, k, j)] " << T_[tuple(C, k, j)] << "\n";
            if (T_[tuple(B, i, k)] && T_[tuple(C, k, j)]) {
              T_[tuple(A, i, j)] = true;
            } else {
              if (T_[tuple(A, i, j)] != true) T_[tuple(A, i, j)] = false;
            }
          }
        }
      }
    }
  }
  for (auto& elem : T_) {
    std::cout << std::get<0>(elem.first) << " " << std::get<1>(elem.first)
              << " " << std::get<2>(elem.first) << " value " << elem.second
              << "\n";
  }
  return T_[tuple(cnf_.get_grammar().start(), 0, word_.size())];
}

std::vector<std::string> Grammar::get_lonely() {
  std::vector<std::string> res;
  for (auto& elem : rules_) {
    if (elem.right().size() == 1 && isupper(elem.right()[0])) {
      if (starting_with(elem.left()).size() == 1) {
        res.push_back(elem.left());
      }
    }
  }
  return res;
}
std::vector<std::string> Grammar::get_epsilons() {
  std::vector<std::string> res;
  for (auto& elem : rules_) {
    if (elem.right() == "") {
      res.push_back(elem.left());
    }
  }
  return res;
}
void Grammar::add(const Rule& rule) {
  if (rules_.end() != std::find(rules_.begin(), rules_.end(), rule)) {
    return;
  }
  neterminals_.emplace(rule.left());
  rules_.push_back(rule);
}

std::vector<Rule> Grammar::ending_with_single(const std::string& end) const {
  std::vector<Rule> res;
  for (auto& rule : rules_) {
    if (rule.right() == end) {
      res.push_back(rule);
    }
  }
  return res;
}
bool Grammar::is_mixed(const std::string& s) {
  return (remove_upper(s).size() != s.size()) && (!remove_upper(s).empty());
}
std::string Grammar::remove_upper(const std::string& str) {
  const std::regex pattern("[A-Z]");
  return regex_replace(str, pattern, "");
}
void Grammar::build_adj() {
  for (auto& net : neterminals_) {
    for (auto& elem : starting_with(net)) {
      std::vector<std::string> temp;
      for (auto& symb : remove_lower(elem.right())) {
        temp.push_back(std::string(1, symb));
      }
      temp.erase(std::unique(temp.begin(), temp.end()), temp.end());
      for (auto& it : temp) {
        adj_[net].push_back(it);
      }
    }
  }
}

std::string Grammar::remove_lower(std::string& str) {
  const std::regex pattern("[a-z]");
  return regex_replace(str, pattern, "");
}

void Grammar::dfs(std::string& current) {
  visited_[current] = true;
  std::cout << "Currently at " << current << "...\n";
  for (auto it = adj_[current].begin(); it != adj_[current].end(); ++it) {
    if (!visited_[*it]) {
      dfs(*it);
    }
  }
}

std::vector<std::string> Grammar::unreachable() {
  std::vector<std::string> res;
  build_adj();
  dfs(start_);
  for (const auto& net : neterminals_) {
    if (visited_.count(net) == 0) {
      res.push_back(net);
    }
  }
  return res;
}

std::vector<Rule> Grammar::ending_with(const std::string& end) const {
  std::vector<Rule> res;
  for (const auto& rule : rules_) {
    if (rule.right().find(end) != std::string::npos) {
      res.push_back(rule);
    }
  }
  return res;
}

std::vector<std::string> Grammar::non_generating() const {
  std::vector<std::string> res;
  std::vector<std::string> lefts;
  for (const auto& start : rules_) {
    lefts.push_back(start.left());
  }
  lefts.erase(std::unique(lefts.begin(), lefts.end()), lefts.end());
  for (const auto& net : neterminals_) {
    if (std::find(lefts.begin(), lefts.end(), net) == lefts.end()) {
      res.push_back(net);
    }
  }
  return res;
}

std::vector<Rule> Grammar::starting_with(const std::string& start) const {
  std::vector<Rule> res;
  for (const auto& rule : rules_) {
    if (start == rule.left()) {
      res.push_back(rule);
    }
  }
  return res;
}

/*std::vector<size_t> Grammar::starting_with_index(
    const std::string& start) const {
  std::vector<size_t> res;
  size_t i = 0;
  for (const auto& rule : rules_) {
    if (start == rule.left()) {
      res.push_back(i);
    }
    ++i;
  }
  return res;
}

std::vector<size_t> Grammar::ending_with_index(const std::string& end) const {
  std::vector<size_t> res;
  size_t i = 0;
  for (const auto& rule : rules_) {
    if (rule.right().find(end) != std::string::npos) {
      res.push_back(i);
    }
    ++i;
  }
  return res;
}*/

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
      // std::cout << "ERROR : alphabet crack!!\n";
      // abort();
    }
    auto nps = neterminals_.find(std::string(1, elem));
    if (nps == neterminals_.end()) {
      // std::cout << "New neterminal !!! -> " << elem << "\n";
      neterminals_.insert(std::string(1, elem));
    }
  }
}

int main() {
  Grammar grammar;
  std::cin >> grammar;
  Chomsky chrome(grammar, false);
  Grammar grammar_copy(grammar.rules(), grammar.sigma(), grammar.neterminals(),
                       grammar.start());
  Rule rule(std::string("A→CBD"));
  grammar_copy.insert(rule);
  const char* text = "S→AAB";
  Rule rulek = Rule(text);
  std::vector<Rule> ruless = {Rule("S→AAB"), Rule("A→"), Rule("B→b")};
  std::set<std::string> alphs = {"a", "b"};
  std::set<std::string> nets = {"S", "A", "B"};
  std::string starts = "S";
  Grammar grammars(ruless, alphs, nets, starts);
  Chomsky chromes(grammars, true);
  chromes.remove_epsilon();
  std::vector<Rule> rulesk = {Rule("S→AAB"), Rule("A→B"), Rule("B→b")};
  // std::set<std::string> alphs = {"a", "b"};
  // std::set<std::string> nets = {"S", "A", "B"};
  // std::string starts = "S";
  Grammar grammarsk(rulesk, alphs, nets, starts);
  Chomsky chromesk(grammarsk);
  chromesk.remove_lonely();
  std::vector<Rule> rules_with_unreachable = {Rule("S→AAB"), Rule("A→B"),
                                              Rule("B→b"), Rule("K→b")};
  std::set<std::string> alphs_with_unreachable = {"a", "b"};
  std::set<std::string> nets_with_unreachable = {"S", "A", "B", "K"};
  std::string starts_with_unreachable = "S";
  Grammar grammar_with_unreachable(
      rules_with_unreachable, alphs_with_unreachable, nets_with_unreachable,
      starts_with_unreachable);
  Chomsky chromes_with_unr(grammar_with_unreachable);
  chromes_with_unr.remove_unreachable();
  std::vector<Rule> rules_with_non = {Rule("S→AABK"), Rule("A→B"), Rule("B→b")};
  std::set<std::string> alphs_with_non = {"a", "b"};
  std::set<std::string> nets_with_non = {"S", "A", "B", "K"};
  std::string starts_with_non = "S";
  Grammar grammar_with_non(rules_with_non, alphs_with_non, nets_with_non,
                           starts_with_non);
  Chomsky chromes_with_non(grammar_with_non);
  chromes_with_non.remove_non_generating();
  std::vector<Rule> trash = grammar_copy.ending_with("B");
  std::vector<Rule> trash2 = grammar_copy.ending_with_single("b");
  std::vector<Rule> trash3 = grammar_copy.get_mixed();
  chrome.bring_to_cnf();
  CYK parser("aaab", chrome);
  std::cout << parser.parse();
  Chomsky chr(alphs);
  Rule rl = chr.ending_with("a");
  Rule rl2;
  rl2 = rl;
  std::string str1 = "AAAAAaaaaa";
  bool tmp1 = grammar_copy.is_long(str1);
  bool tmp2 = chr.is_lower(str1);
  str1 = grammar_copy.remove_lower(str1);
  auto trsh = grammar_copy.get_long();
  return 0;
}
/*S→ABCD
A→CBD
B→CD
D→C
C→aab*/

/*S→aA
A→B
B→C
C→D
D→ab*/

/*S→AAAB
A→AA
A→a
B→b*/
