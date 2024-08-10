#include <QApplication>

#include "view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  ps::Model calc_model;
  ps::CreditModel credit_model;
  ps::Controller calc_controller(&calc_model, &credit_model);

  View w(&calc_controller);
  w.show();
  return a.exec();
}
