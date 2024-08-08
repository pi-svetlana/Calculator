#include "credit_model.h"

namespace ps {
CreditData CreditModel::GetCreditData() { return credit_data_; }

void CreditModel::CountCredit(double rubs, int months, double rate,
                              TypeCredit type) {
  credit_data_.payments.clear();
  credit_data_.payments.reserve(months);
  credit_data_.credit_total = 0;
  if (type == kAnnuity) {
    double monthly_percent = rate / 100 / 12;
    double payment = rubs * (monthly_percent * pow(1 + monthly_percent, months)) /
                     (pow(1 + monthly_percent, months) - 1);
    payment = round(payment * 100) / 100;
    for (int i = 0; i < months; ++i) credit_data_.payments.push_back(payment);
    credit_data_.credit_total = payment * months;
    credit_data_.overpay = credit_data_.credit_total - rubs;
  } else {
    double tmp = rubs;
    double main_payment = tmp / static_cast<double>(months);
    double rate_part = 0.0;
    for (int i = 0; i < months; ++i) {
      rate_part = (tmp * rate) / 100 / 12;
      tmp -= main_payment;
      credit_data_.payments.push_back(round((main_payment + rate_part) * 100) /
                                      100);
    }
    for (int i = 0; i < months; ++i) {
      credit_data_.credit_total += credit_data_.payments[i];
    }
    credit_data_.overpay = credit_data_.credit_total - rubs;
  }
}

}  // namespace ps
