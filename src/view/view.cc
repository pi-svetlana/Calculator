#include "view.h"

#include "./ui_view.h"

View::View(ps::Controller *c, QWidget *parent)
    : QWidget(parent), ui(new Ui::View), controller_(c) {
  ui->setupUi(this);

  QList<QPushButton *> functions = {ui->acos_btn, ui->asin_btn, ui->atan_btn,
                                    ui->cos_btn,  ui->log_btn,  ui->ln_btn,
                                    ui->sin_btn,  ui->sqrt_btn, ui->tan_btn};

  for (int i = 0; i < functions.length(); i++) {
    connect(functions[i], SIGNAL(clicked()), this,
            SLOT(add_func_name_to_expression()));
  }

  QList<QPushButton *> input_buttons = {
      ui->div_btn,   ui->dot_btn,       ui->eight_btn, ui->five_btn,
      ui->four_btn,  ui->left_brc_btn,  ui->minus_btn, ui->mod_btn,
      ui->mul_btn,   ui->nine_btn,      ui->one_btn,   ui->plus_btn,
      ui->pow_btn,   ui->right_brc_btn, ui->seven_btn, ui->six_btn,
      ui->three_btn, ui->two_btn,       ui->x_btn,     ui->zero_btn,
      ui->e_btn};

  for (int i = 0; i < input_buttons.length(); i++) {
    connect(input_buttons[i], SIGNAL(clicked()), this,
            SLOT(add_data_to_expression()));
  }

  QRegularExpressionValidator *expression_validator =
      new QRegularExpressionValidator(
          QRegularExpression(
              "[0-9ac-egil-oq-txAC-EGIL-OQ-TX\\-*/+^\\(\\)\\.]*"),
          this);

  ui->expression->setValidator(expression_validator);

  QRegularExpressionValidator *double_validator =
      new QRegularExpressionValidator(
          QRegularExpression("[-+]?[0-9]*[.]?[0-9]*"), this);

  ui->graph->setInteraction(QCP::iRangeDrag);
  ui->graph->setInteraction(QCP::iRangeZoom);

  ui->x_val->setValidator(double_validator);
  ui->x_max->setValidator(double_validator);
  ui->x_min->setValidator(double_validator);
  ui->y_max->setValidator(double_validator);
  ui->y_min->setValidator(double_validator);

  ui->graph->xAxis->setLabel("x");
  ui->graph->yAxis->setLabel("y");
}

View::~View() { delete ui; }

void View::add_func_name_to_expression() {
  QPushButton *btn = (QPushButton *)sender();
  QString text = ui->expression->text();
  text += btn->text() + "(";
  ui->expression->setText(text);
}

void View::add_data_to_expression() {
  QPushButton *btn = (QPushButton *)sender();
  QString text = ui->expression->text();
  text += btn->text();
  ui->expression->setText(text);
}

void View::on_backspace_btn_clicked() { ui->expression->backspace(); }

void View::on_ac_btn_clicked() {
  ui->expression->clear();
  ui->result->clear();
  ui->error->clear();
}

void View::on_equal_btn_clicked() {
  QString input_string = ui->expression->text().toLower();
  if (input_string.isEmpty()) return;
  std::string ready_expression = input_string.toStdString();

  QString x_value_string = ui->x_val->text();
  if (x_value_string.isEmpty()) {
    ui->x_val->setText("1");
    x_value_string = "1";
  }
  double x = x_value_string.toDouble();
  try {
    double result = controller_->CalculateResult(ready_expression, x);
    ui->result->setText(QString::number(result, 'g', 15));
  } catch (const std::exception &e) {
    ui->error->setText(QString::fromStdString(e.what()));
  }
}

void View::on_show_grath_btn_clicked() {
  ui->error->clear();
  QString x_min_str = ui->x_min->text();
  QString x_max_str = ui->x_max->text();
  QString y_min_str = ui->y_min->text();
  QString y_max_str = ui->y_max->text();

  if (x_min_str.isEmpty() || x_max_str.isEmpty() || y_min_str.isEmpty() ||
      y_max_str.isEmpty()) {
    ui->x_min->setText("-5");
    ui->x_max->setText("5");
    ui->y_min->setText("-5");
    ui->y_max->setText("5");
  }

  double x_min_val = x_min_str.toDouble();
  double x_max_val = x_max_str.toDouble();
  double y_min_val = y_min_str.toDouble();
  double y_max_val = y_max_str.toDouble();

  if ((x_min_val > 1000000 || x_min_val < -1000000 || x_max_val > 1000000 ||
       x_max_val < -1000000 || y_min_val > 1000000 || y_min_val < -1000000 ||
       y_max_val > 1000000 || y_max_val < -1000000) ||
      (x_min_val >= x_max_val || y_min_val >= y_max_val)) {
    ui->x_min->setText("-5");
    ui->x_max->setText("5");
    ui->y_min->setText("-5");
    ui->y_max->setText("5");
    x_min_val = y_min_val = -5;
    x_max_val = y_max_val = 5;
  }

  QString input_string = ui->expression->text().toLower();
  if (input_string.isEmpty()) return;
  std::string ready_expression = input_string.toStdString();

  try {
   ps::GraphPoints graph_points =
        controller_->CalculateGraph(ready_expression, x_min_val, x_max_val);
    QVector<double> x_points(graph_points.x_val.begin(),
                             graph_points.x_val.end());
    QVector<double> y_points(graph_points.y_val.begin(),
                             graph_points.y_val.end());

    ui->graph->clearGraphs();
    ui->graph->xAxis->setRange(x_min_val, x_max_val);
    ui->graph->yAxis->setRange(y_min_val, y_max_val);

    ui->graph->addGraph();
    ui->graph->graph(0)->setData(x_points, y_points);
    ui->graph->replot();
    x_points.clear();
    y_points.clear();

  } catch (const std::exception &e) {
    ui->error->setText(QString::fromStdString(e.what()));
  }
}

void View::on_expression_textChanged(const QString &arg1) {
  ui->error->clear();
}

void View::on_count_credit_btn_clicked() {
  ui->payment->clear();
  ui->credit_and_percent->clear();
  ui->percent_out->clear();
  ui->month_payment->clearContents();
  ui->month_payment->setRowCount(0);

  double credit_sum = ui->credit_sum->value();
  int credit_term = ui->credit_term->value();
  double credit_percent = ui->credit_percent->value();
  bool annuit = ui->annuity->isChecked();
  ps::TypeCredit credit_type;
  if (annuit)
    credit_type = ps::kAnnuity;
  else
    credit_type = ps::kDifferentiated;

  ps::CreditData credit_data = controller_->CalculeteCredit(
      credit_sum, credit_term, credit_percent, credit_type);
  if (annuit)
    ui->payment->setText(QString::number(credit_data.payments.front(), 'f', 2));
  else
    ui->payment->setText(QString::number(credit_data.payments.front(), 'f', 2) +
                         " ... " +
                         QString::number(credit_data.payments.back(), 'f', 2));
  ui->percent_out->setText(QString::number(credit_data.overpay, 'f', 2));
  ui->credit_and_percent->setText(
      QString::number(credit_data.credit_total, 'f', 2));
  if (!annuit) {
    ui->month_payment->setColumnCount(1);
    ui->month_payment->setShowGrid(true);
    QStringList headers;
    headers.append("Ежемесячный платеж");
    ui->month_payment->setHorizontalHeaderLabels(headers);
    ui->month_payment->horizontalHeader()->setStretchLastSection(true);
    ui->month_payment->setEditTriggers(QTableWidget::NoEditTriggers);
    for (int i = 0; i < credit_term; i++) {
      ui->month_payment->insertRow(i);
      ui->month_payment->setItem(i, 0,
                                 new QTableWidgetItem(QString::number(
                                     credit_data.payments.front(), 'f', 2)));
      credit_data.payments.erase(credit_data.payments.begin());
    }
  }
}
