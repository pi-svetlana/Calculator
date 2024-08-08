#ifndef CALCULATOR_SRC_MODEL_MODEL_H_
#define CALCULATOR_SRC_MODEL_MODEL_H_

#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <regex>
#include <stack>
#include <vector>

namespace ps {

enum Priority { kDefault, kLow, kMedium, kHigh, kUnary, kFunction };

enum Type {
  kFirstSymbol,
  kLastSymbol,
  kNumber,
  kXValue,
  kBinaryOperator,
  kUnaryOperator,
  kUnaryFunction,
  kLeftBracket,
  kRightBracket,
  kNumTokenTypes
};

struct GraphPoints {
  std::vector<double> x_val;
  std::vector<double> y_val;
};

class Lexeme {
 public:
  Lexeme() {}
  Lexeme(const std::string name, Priority priority, Type type, double value);
  ~Lexeme() {}

  std::string GetName();
  Priority GetPriority();
  Type GetType();
  double GetValue();

 private:
  std::string name_;
  Priority priority_;
  Type type_;
  double value_;
};

class Model {
 public:
  Model();
  ~Model() {}

  void Calculate(std::string& str, double x);
  double GetResult();

  void CalculateGraphPoints(std::string& str, double x_min, double x_max);
  GraphPoints GetGraphPoints();

 private:
  std::string expression_;
  std::deque<Lexeme> lexemes_;
  std::deque<Lexeme> postfix_;
  double x_;
  double result_;
  std::map<std::string, Lexeme> lexeme_map_;
  struct GraphPoints points_;

  bool adjacency_matrix_[kNumTokenTypes][kNumTokenTypes] = {
      {0, 1, 1, 1, 0, 1, 1, 1, 0},  // kFirstSymbol
      {0, 0, 0, 0, 0, 0, 0, 0, 0},  // kLastSymbol
      {0, 1, 0, 0, 1, 0, 0, 0, 1},  // kNumber
      {0, 1, 0, 0, 1, 0, 0, 0, 1},  // kXValue
      {0, 0, 1, 1, 0, 1, 1, 1, 0},  // kBinaryOperator
      {0, 0, 1, 1, 0, 0, 1, 1, 0},  // kUnaryOperator
      {0, 0, 0, 0, 0, 0, 0, 1, 0},  // kUnaryFunction
      {0, 0, 1, 1, 0, 1, 1, 1, 0},  // kLeftBracket
      {0, 1, 0, 0, 1, 0, 0, 0, 1}   // kRightBracket
  };

  void MakeLexemeMap(std::map<std::string, Lexeme>& map);
  void InitModel(std::string& expression, double x);
  void ReadExpression();
  std::string ReadNumber(std::string& str, size_t& pos);
  std::string ReadFunctionName(std::string& str, size_t& pos);
  std::string ReadTypeOperation(std::string& str, size_t& pos);
  void PushToQueue(std::string& str);
  void CheckExpression();
  void ConvertToPostfix();
  double CountResult(double& x);
};
}  // namespace ps
#endif  // CALCULATOR_SRC_MODEL_MODEL_H_