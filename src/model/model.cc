#include "model.h"

namespace ps {
Lexeme::Lexeme(const std::string name, Priority priority, Type type,
               double value)
    : name_(name), priority_(priority), type_(type), value_(value) {}

std::string Lexeme::GetName() { return name_; }
Priority Lexeme::GetPriority() { return priority_; }
Type Lexeme::GetType() { return type_; }
double Lexeme::GetValue() { return value_; }

Model::Model() { Model::MakeLexemeMap(lexeme_map_); }
void Model::MakeLexemeMap(std::map<std::string, Lexeme>& map) {
  map.insert(std::make_pair("x", Lexeme("x", kDefault, kXValue, 0.0)));
  map.insert(std::make_pair("(", Lexeme("(", kDefault, kLeftBracket, 0)));
  map.insert(std::make_pair(")", Lexeme(")", kDefault, kRightBracket, 0)));
  map.insert(std::make_pair("+", Lexeme("+", kLow, kBinaryOperator, 0)));
  map.insert(std::make_pair("-", Lexeme("-", kLow, kBinaryOperator, 0)));
  map.insert(std::make_pair("*", Lexeme("*", kMedium, kBinaryOperator, 0)));
  map.insert(std::make_pair("/", Lexeme("/", kMedium, kBinaryOperator, 0)));
  map.insert(std::make_pair("mod", Lexeme("mod", kMedium, kBinaryOperator, 0)));
  map.insert(
      std::make_pair("sin", Lexeme("sin", kFunction, kUnaryFunction, 0)));
  map.insert(
      std::make_pair("cos", Lexeme("cos", kFunction, kUnaryFunction, 0)));
  map.insert(
      std::make_pair("tan", Lexeme("tan", kFunction, kUnaryFunction, 0)));
  map.insert(
      std::make_pair("asin", Lexeme("asin", kFunction, kUnaryFunction, 0)));
  map.insert(
      std::make_pair("acos", Lexeme("acos", kFunction, kUnaryFunction, 0)));
  map.insert(
      std::make_pair("atan", Lexeme("atan", kFunction, kUnaryFunction, 0)));
  map.insert(
      std::make_pair("sqrt", Lexeme("sqrt", kFunction, kUnaryFunction, 0)));
  map.insert(std::make_pair("ln", Lexeme("ln", kFunction, kUnaryFunction, 0)));
  map.insert(
      std::make_pair("log", Lexeme("log", kFunction, kUnaryFunction, 0)));
  map.insert(std::make_pair("^", Lexeme("^", kHigh, kBinaryOperator, 0)));
}

void Model::Calculate(std::string& str, double x) {
  InitModel(str, x);
  ReadExpression();
  CheckExpression();
  ConvertToPostfix();
  result_ = CountResult(x_);
}

void Model::InitModel(std::string& expression, double x) {
  expression_ = expression;
  x_ = x;
  postfix_.clear();
  lexemes_.clear();
  lexemes_.push_back(Lexeme("begin", kDefault, kFirstSymbol, 0));
}

void Model::ReadExpression() {
  std::string lexeme_str;
  for (size_t i = 0; i < expression_.length();) {
    if (expression_[i] == '.' || isdigit(expression_[i])) {
      lexeme_str = ReadNumber(expression_, i);
      const char* number = lexeme_str.c_str();
      if (lexemes_.back().GetType() == kRightBracket ||
          lexemes_.back().GetType() == kXValue)
        lexemes_.push_back(Lexeme("*", kMedium, kBinaryOperator, 0));
      lexemes_.push_back(
          Lexeme(lexeme_str, kDefault, kNumber, strtod(number, nullptr)));
    } else if (isalpha(expression_[i])) {
      lexeme_str = ReadFunctionName(expression_, i);
      PushToQueue(lexeme_str);
    } else {
      lexeme_str = ReadTypeOperation(expression_, i);
      PushToQueue(lexeme_str);
    }
  }
  lexemes_.push_back(Lexeme("end", kDefault, kLastSymbol, 0));
}

std::string Model::ReadNumber(std::string& str, size_t& pos) {
  std::regex number_regex("^\\d*\\.?\\d*(e[+,-]?\\d+)?");
  std::sregex_iterator it(str.begin() + pos, str.end(), number_regex);
  std::smatch num_match = *it;
  pos += num_match.length();
  return num_match.str();
}

std::string Model::ReadFunctionName(std::string& str, size_t& pos) {
  if (str.at(pos) == 'x') {
    ++pos;
    return "x";
  }
  std::regex function_regex("^[a-z]+");
  std::sregex_iterator it(str.begin() + pos, str.end(), function_regex);
  std::smatch function_match = *it;
  pos += function_match.length();
  return function_match.str();
}

std::string Model::ReadTypeOperation(std::string& str, size_t& pos) {
  std::regex operator_regex("^\\W");
  std::sregex_iterator it(str.begin() + pos, str.end(), operator_regex);
  std::smatch operator_match = *it;
  pos += operator_match.length();
  return operator_match.str();
}

void Model::PushToQueue(std::string& str) {
  auto map_it = lexeme_map_.find(str);
  if (map_it == lexeme_map_.end()) {
    throw std::logic_error("Некорректный ввод: " + str);
  }
  Type type_prev = lexemes_.back().GetType();
  Type type_cur = (map_it->second).GetType();
  if (str == "-" || str == "+") {
    if (type_prev == kRightBracket || type_prev == kNumber ||
        type_prev == kXValue) {
      lexemes_.push_back(map_it->second);
    } else {
      if (str == "-") {
        lexemes_.push_back(Lexeme("-", kUnary, kUnaryOperator, 0));
      } else {
        lexemes_.push_back(Lexeme("+", kUnary, kUnaryOperator, 0));
      }
    }
  } else if (((type_prev == kXValue) &&
              (type_cur == kLeftBracket || type_cur == kUnaryFunction ||
               type_cur == kNumber || type_cur == kXValue)) ||
             ((type_prev == kNumber) &&
              (type_cur == kLeftBracket || type_cur == kUnaryFunction ||
               type_cur == kXValue)) ||
             ((type_prev == kRightBracket) &&
              (type_cur == kNumber || type_cur == kXValue ||
               type_cur == kUnaryFunction || type_cur == kLeftBracket))) {
    lexemes_.push_back(Lexeme("*", kMedium, kBinaryOperator, 0));
    lexemes_.push_back(map_it->second);
  } else {
    lexemes_.push_back(map_it->second);
  }
}

void Model::CheckExpression() {
  int bkt_count = 0;
  for (size_t i = 0; i < lexemes_.size() - 1; ++i) {
    if (lexemes_[i].GetType() == kRightBracket) --bkt_count;
    if (lexemes_[i].GetType() == kLeftBracket) ++bkt_count;
    if (bkt_count < 0)
      throw std::logic_error("Некорректная расстановка скобок");
    if (!adjacency_matrix_[lexemes_[i].GetType()][lexemes_[i + 1].GetType()]) {
      if (lexemes_[i].GetType() == kFirstSymbol)
        throw std::logic_error("Некорректный символ в начале строки");
      if (lexemes_[i + 1].GetType() == kLastSymbol)
        throw std::logic_error("Некорректный символ в конце строки");
      throw std::logic_error("Некорректный ввод: " + lexemes_[i].GetName() +
                             lexemes_[i + 1].GetName());
    }
  }
  if (bkt_count != 0) throw std::logic_error("Некорректная расстановка скобок");
}

void Model::ConvertToPostfix() {
  std::stack<Lexeme> tmp_stack;
  lexemes_.pop_front();
  while (lexemes_.front().GetType() != kLastSymbol) {
    Type cur_type = lexemes_.front().GetType();
    Priority cur_priority = lexemes_.front().GetPriority();
    std::string cur_name = lexemes_.front().GetName();
    if (cur_type == kNumber || cur_type == kXValue) {
      postfix_.push_back(lexemes_.front());
    }
    if (cur_type == kBinaryOperator || cur_type == kUnaryOperator) {
      while (!tmp_stack.empty() &&
             cur_priority <= tmp_stack.top().GetPriority() &&
             (tmp_stack.top().GetType() == kBinaryOperator ||
              tmp_stack.top().GetType() == kUnaryOperator)) {
        if (cur_name == "^" && (tmp_stack.top().GetName() == "^" ||
                                tmp_stack.top().GetType() == kUnaryOperator)) {
          break;
        } else {
          postfix_.push_back(tmp_stack.top());
          tmp_stack.pop();
        }
      }
      tmp_stack.push(lexemes_.front());
    }
    if (cur_type == kLeftBracket || cur_type == kUnaryFunction) {
      tmp_stack.push(lexemes_.front());
    }
    if (cur_type == kRightBracket) {
      while (!tmp_stack.empty() && tmp_stack.top().GetType() != kLeftBracket) {
        postfix_.push_back(tmp_stack.top());
        tmp_stack.pop();
      }
      tmp_stack.pop();
      if (!tmp_stack.empty() && tmp_stack.top().GetType() == kUnaryFunction) {
        postfix_.push_back(tmp_stack.top());
        tmp_stack.pop();
      }
    }
    lexemes_.pop_front();
  }
  lexemes_.pop_front();
  while (!tmp_stack.empty()) {
    postfix_.push_back(tmp_stack.top());
    tmp_stack.pop();
  }
}

double Model::CountResult(double& x) {
  double tmp_res = 0;
  std::stack<double> numbers;
  std::deque<Lexeme> tmp = postfix_;
  while (!tmp.empty()) {
    Type cur_type = tmp.front().GetType();
    std::string cur_name = tmp.front().GetName();
    if (cur_type == kNumber) {
      numbers.push(tmp.front().GetValue());
    } else if (cur_type == kXValue) {
      numbers.push(x);
    } else {
      double num = numbers.top();
      numbers.pop();
      if (cur_type == kBinaryOperator) {
        double num_2 = numbers.top();
        numbers.pop();
        if (cur_name == "+")
          tmp_res = num_2 + num;
        else if (cur_name == "-")
          tmp_res = num_2 - num;
        else if (cur_name == "*")
          tmp_res = num_2 * num;
        else if (cur_name == "/")
          tmp_res = num_2 / num;
        else if (cur_name == "^")
          tmp_res = pow(num_2, num);
        else if (cur_name == "mod")
          tmp_res = fmod(num_2, num);
      }
      if (cur_type == kUnaryOperator) {
        if (cur_name == "+")
          tmp_res = num;
        else if (cur_name == "-")
          tmp_res = -num;
      }
      if (cur_type == kUnaryFunction) {
        if (cur_name == "cos")
          tmp_res = cos(num);
        else if (cur_name == "sin")
          tmp_res = sin(num);
        else if (cur_name == "tan")
          tmp_res = tan(num);
        else if (cur_name == "acos")
          tmp_res = acos(num);
        else if (cur_name == "asin")
          tmp_res = asin(num);
        else if (cur_name == "atan")
          tmp_res = atan(num);
        else if (cur_name == "log")
          tmp_res = log10(num);
        else if (cur_name == "ln")
          tmp_res = log(num);
        else if (cur_name == "sqrt")
          tmp_res = sqrt(num);
      }
      numbers.push(tmp_res);
    }
    tmp.pop_front();
  }
  tmp_res = numbers.top();
  numbers.pop();
  return tmp_res;
}

double Model::GetResult() { return result_; }

GraphPoints Model::GetGraphPoints() { return points_; }

void Model::CalculateGraphPoints(std::string& str, double x_min, double x_max) {
  points_.x_val.clear();
  points_.y_val.clear();
  InitModel(str, 0);
  ReadExpression();
  CheckExpression();
  ConvertToPostfix();
  double step = (x_max - x_min) / 999;
  for (size_t i = 0; i < 1000; ++i) {
    points_.x_val.push_back(x_min + step * i);
    points_.y_val.push_back(CountResult(points_.x_val.back()));
  }
}

}  // namespace ps
