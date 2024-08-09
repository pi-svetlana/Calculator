#include "controller.h"

namespace ps {

Controller::Controller(Model* m, CreditModel* c)
    : calculator_model_(m), credit_model_(c) {}

double Controller::CalculateResult(std::string& str, double x) {
  calculator_model_->Calculate(str, x);
  return calculator_model_->GetResult();
}

GraphPoints Controller::CalculateGraph(std::string& str, double x_min,
                                       double x_max) {
  calculator_model_->CalculateGraphPoints(str, x_min, x_max);
  return calculator_model_->GetGraphPoints();
}

CreditData Controller::CalculeteCredit(double rubs, int months, double term,
                                       TypeCredit type) {
  credit_model_->CountCredit(rubs, months, term, type);
  return credit_model_->GetCreditData();
}

}  // namespace ps