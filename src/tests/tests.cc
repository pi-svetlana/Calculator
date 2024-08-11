#include <gtest/gtest.h>

#include "../controller/controller.h"
#include "../model/model.h"

namespace ps {

TEST(Calculator, Test_1) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "9+)+9-(-9";
  ASSERT_ANY_THROW(contr.CalculateResult(str, 8));
}

TEST(Calculator, Test_2) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "cosin(x)";
  ASSERT_ANY_THROW(contr.CalculateResult(str, 8));
}

TEST(Calculator, Test_3) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "7mod";
  ASSERT_ANY_THROW(contr.CalculateResult(str, 8));
}

TEST(Calculator, Test_4) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "cos9";
  ASSERT_ANY_THROW(contr.CalculateResult(str, 8));
}

TEST(Calculator, Test_5) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "*-8";
  ASSERT_ANY_THROW(contr.CalculateResult(str, 8));
}

TEST(Calculator, Test_6) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "-89+*90";
  ASSERT_ANY_THROW(contr.CalculateResult(str, 8));
}

TEST(Calculator, Test_7) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "2xcos(34)+-84++43";
  double my_res = contr.CalculateResult(str, 8);
  double result = 2 * 8 * cos(34) - 84 + 43;
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Calculator, Test_8) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "sin(0.5)tan(1.3)";
  double my_res = contr.CalculateResult(str, 8);
  double result = sin(0.5) * tan(1.3);
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Calculator, Test_9) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "-2^-3^2";
  double my_res = contr.CalculateResult(str, 8);
  double result = -1 * pow(2, -1 * pow(3, 2));
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Calculator, Test_10) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "1e+3x+3mod2";
  double my_res = contr.CalculateResult(str, 8);
  double result = 1000 * 8 + fmod(3, 2);
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Calculator, Test_11) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "asin(0.3)atan(.4)";
  double my_res = contr.CalculateResult(str, 8);
  double result = asin(0.3) * atan(0.4);
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Calculator, Test_12) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "acos(0.354)sqrt(27)";
  double my_res = contr.CalculateResult(str, 8);
  double result = acos(0.354) * sqrt(27);
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Calculator, Test_13) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "ln(0.345)*log(0.645)";
  double my_res = contr.CalculateResult(str, 8);
  double result = log(0.345) * log10(0.645);
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Calculator, Test_14) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "-67*+45/-11";
  double my_res = contr.CalculateResult(str, 8);
  double result = (-67.0) * 45 / (-11);
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Calculator, Test_15) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "(56.789/7.5)(23-45.2)";
  double my_res = contr.CalculateResult(str, 8);
  double result = (56.789 / 7.5) * (23 - 45.2);
  ASSERT_NEAR(my_res, result, 1e-6);
}

TEST(Graphics, Test_1) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "3xcos(0.345)";
  GraphPoints g_points = contr.CalculateGraph(str, -5, 5);
  double result_min = 3 * (-5) * cos(0.345);
  double result_max = 3 * 5 * cos(0.345);
  ASSERT_NEAR(result_min, g_points.y_val.front(), 1e-6);
  ASSERT_NEAR(result_max, g_points.y_val.back(), 1e-6);
}

TEST(Graphics, Test_2) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  std::string str = "2tan(x)cos(0.345)";
  GraphPoints g_points = contr.CalculateGraph(str, -5, 5);
  double result_min = 2 * tan(-5) * cos(0.345);
  double result_max = 2 * tan(5) * cos(0.345);
  ASSERT_NEAR(result_min, g_points.y_val.front(), 1e-6);
  ASSERT_NEAR(result_max, g_points.y_val.back(), 1e-6);
}

TEST(CreditCalculator, Test_1) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  CreditData data = contr.CalculeteCredit(100000, 10, 10, kAnnuity);
  ASSERT_NEAR(data.credit_total, 104640.40, 1e-2);
  ASSERT_NEAR(data.overpay, 4640.40, 1e-2);
  ASSERT_NEAR(data.payments.front(), 10464.04, 1e-2);
}

TEST(CreditCalculator, Test_2) {
  Model mod;
  CreditModel c_mod;
  Controller contr(&mod, &c_mod);
  CreditData data = contr.CalculeteCredit(100000, 10, 10, kDifferentiated);
  ASSERT_NEAR(data.credit_total, 104583.33, 1e-2);
  ASSERT_NEAR(data.overpay, 4583.33, 1e-2);
  ASSERT_NEAR(data.payments.front(), 10833.33, 1e-2);
  ASSERT_NEAR(data.payments.back(), 10083.33, 1e-2);
}

}  // namespace ps
