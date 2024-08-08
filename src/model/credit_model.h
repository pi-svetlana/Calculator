#ifndef CALCULATOR_SRC_MODEL_CREDIT_MODEL_H_
#define CALCULATOR_SRC_MODEL_CREDIT_MODEL_H_

#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

namespace ps {
enum TypeCredit { kAnnuity, kDifferentiated };

struct CreditData {
  double credit_total;
  double overpay;
  std::vector<double> payments;
};

class CreditModel {
 public:
  CreditModel() {}
  ~CreditModel() {}

  void CountCredit(double rubs, int months, double term, TypeCredit type);
  CreditData GetCreditData();

 private:
  CreditData credit_data_;
};

}  // namespace ps

#endif  // CALCULATOR_SRC_MODEL_CREDIT_MODEL_H_