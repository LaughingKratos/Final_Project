#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include "survival_form.h"
#include "qmessagebox.h"
#include "QFile"
#include "QString"
#include "QTextStream"
//#include "fstream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("The Game");

    int any_score = 0;
    int easy_score = 0;
    int medium_score = 0;
    int hard_score = 0;

    QFile file("scores.txt");
    if (file.exists()){
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << any_score << '\n' << easy_score << '\n' << medium_score << '\n' << hard_score;
            file.close();
        }
        else{
            QMessageBox *ermsg = new QMessageBox;
            ermsg->setText("Error creating the file!");
            ermsg->exec();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    Form *pvp = new Form;
    pvp->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    Survival_Form *sur = new Survival_Form;
    sur->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    //read scores from file.................................................
    int any_score = 0;
    int easy_score = 0;
    int medium_score = 0;
    int hard_score = 0;

    QFile file("scores.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        in >> any_score >> easy_score >> medium_score >> hard_score;
        file.close();
    }
    //end of read scores from file.................................................

    QMessageBox *msg = new QMessageBox;
    msg->setText("Any Difficulty Mode: " + QString::number(any_score)   +
                 "\nEasy Mode: "         + QString::number(easy_score)  +
                 "\nMedium Mode: "       + QString::number(medium_score)+
                 "\nHard Mode: "         + QString::number(hard_score)) ;
    msg->exec();
}


void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox *about = new QMessageBox;
    about->setText("Aliraza Saker\nAbolfazl Noruzi\nSoren Soltanzadeh");
    about->exec();
}

