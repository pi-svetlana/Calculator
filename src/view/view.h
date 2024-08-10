#ifndef VIEW_H
#define VIEW_H

// #include <QValidator>
#include <QWidget>

#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QWidget {
  Q_OBJECT

 public:
  View(ps::Controller *c, QWidget *parent = nullptr);
  ~View();

 private slots:
  void add_func_name_to_expression();
  void add_data_to_expression();
  void on_backspace_btn_clicked();
  void on_ac_btn_clicked();
  void on_equal_btn_clicked();
  void on_show_grath_btn_clicked();
  void on_expression_textChanged(const QString &arg1);
  void on_count_credit_btn_clicked();

 private:
  Ui::View *ui;
  void ConnectSignals();
  ps::Controller *controller_;
};

#endif  // VIEW_H
