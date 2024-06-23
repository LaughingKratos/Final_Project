#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void UpdateCountdown();

private:
    Ui::Form *ui;
    QTimer *timer;
    QTimer* timerSec;
};

#endif // FORM_H
