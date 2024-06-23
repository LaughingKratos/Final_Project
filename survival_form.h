#ifndef SURVIVAL_FORM_H
#define SURVIVAL_FORM_H

#include <QWidget>

namespace Ui {
class Survival_Form;
}

class Survival_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Survival_Form(QWidget *parent = nullptr);
    ~Survival_Form();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void UpdateCountdown();

private:
    Ui::Survival_Form *ui;
    QTimer *timer;
    QTimer* timerSec;
};

#endif // SURVIVAL_FORM_H
