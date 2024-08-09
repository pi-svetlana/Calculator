#ifndef CALCULATOR_CONTROLLER_CONTROLLER_H_
#define CALCULATOR_CONTROLLER_CONTROLLER_H_

#include "../model/credit_model.h"
#include "../model/model.h"

namespace ps {
class Controller {
 public:
  Controller(Model* m, CreditModel* c);
  ~Controller() {}

  double CalculateResult(std::string& str, double x);

  GraphPoints CalculateGraph(std::string& str, double x_min, double x_max);

  CreditData CalculeteCredit(double rubs, int months, double term,
                             TypeCredit type);

 private:
  Model* calculator_model_;
  CreditModel* credit_model_;
};

}  // namespace ps

#endif  // CALCULATOR_CONTROLLER_CONTROLLER_H_
