#include <algorithm>
#include <iostream>
#include <string>
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
