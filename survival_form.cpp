#include "survival_form.h"
#include "ui_survival_form.h"
#include "qmessagebox.h"
#include "QFile"
#include "QString"
#include "QTextStream"
#include "QCloseEvent"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "QTimer"

QString difficaultyy;
int question_nomber = 0;
int high_score = 0;
int score = 0;
std::string myToken;

using json = nlohmann::json;

namespace
{
    std::size_t callback(
        const char* in,
        std::size_t size,
        std::size_t num,
        std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}

std::string jsonData2(std::string url) {
    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    long httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
        return *httpData.get();
    else
        return "";
}

void Replacement2(std::string& inp) {
    int pos;
    while ((pos = inp.find("&quot;")) != std::string::npos)
        inp.replace(pos, 6, "\"");
    while ((pos = inp.find("&amp;")) != std::string::npos)
        inp.replace(pos, 5, "&");
    while ((pos = inp.find("&#039;")) != std::string::npos)
        inp.replace(pos, 6, "\'");
}

void GetToken2() {
    json tokenJ = json::parse(jsonData2("https://opentdb.com/api_token.php?command=request"));
    if (tokenJ["response_code"] == 0)
        myToken = tokenJ["token"];
}

Survival_Form::Survival_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Survival_Form)
{
    ui->setupUi(this);
    setWindowTitle("Survival");
    question_nomber = 0;
    high_score = 0;
    score = 0;
    GetToken2();
    timer = new QTimer(this);
    timerSec = new QTimer(this);
    connect(timerSec, SIGNAL(timeout()), this, SLOT(UpdateCountdown()));
}

Survival_Form::~Survival_Form()
{
    delete ui;
}

QString question;
std::vector<QString> wrongAnswers;
QString trueAnswer;
int randomCorrect;


void GetQuestion() {
    wrongAnswers.clear();
    resetToken:
    std::string url("https://opentdb.com/api.php?amount=1&type=multiple");
    url += "&token=" + myToken;
    if (difficaultyy != "Any Difficulty") {
        url += "&difficauly=" + difficaultyy.toStdString();
    }

    std::string thisRoundItem = jsonData2(url);

    if (thisRoundItem != "") {
        json js = json::parse(thisRoundItem);

        if (js["response_code"] == 0) {
            json res = js["results"];
            std::string qs = res[0]["question"];
            std::string coans = res[0]["correct_answer"];
            Replacement2(qs);
            Replacement2(coans);

            question = QString::fromStdString(qs);
            trueAnswer = QString::fromStdString(coans);
            json wajson = res[0]["incorrect_answers"];
            for (short j = 0; j < 3; j++) {
                std::string temp = wajson[j];
                Replacement2(temp);
                wrongAnswers.push_back(QString::fromStdString(temp));     
            }
        }
        else if (js["response_code"] == 1) {
            QMessageBox* msg = new QMessageBox;
            msg->setWindowTitle("Error!");
            msg->setText("Not enough questions on the server.");
            msg->exec();
        }
        else if (js["response_code"] == 3 || js["response_code"] == 4) {
            GetToken2();
            goto resetToken;
        }
        else {
            QMessageBox* msg = new QMessageBox;
            msg->setWindowTitle("Error!");
            msg->setText("An unknown error prevented us from starting the game.");
            msg->exec();
        }
    }
    else {
        goto resetToken;
    }
}

void Survival_Form::on_pushButton_clicked()
{
    if(ui->radioButton_1->isChecked()==true){difficaultyy="Any Difficulty";}
    else if(ui->radioButton_2->isChecked()==true){difficaultyy="easy";}
    else if(ui->radioButton_3->isChecked()==true){difficaultyy="medium";}
    else if(ui->radioButton_4->isChecked()==true){difficaultyy="hard";}

    //read high_score_Difficulty from file............................................
    QFile file("scores.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        if(difficaultyy=="Any Difficulty"){in >> high_score;}
        else if(difficaultyy=="easy"){in >> high_score >> high_score;}
        else if(difficaultyy=="medium"){in >> high_score >> high_score >> high_score;}
        else if(difficaultyy=="hard"){in >> high_score >> high_score >> high_score >> high_score;}
        file.close();
    }

    if(high_score<score){high_score = score;}
    ui->label_HSc->setText(QString::number(high_score));
    ui->label_Sc->setText(QString::number(score));
    //ui->label_wrongs->setText(QString::number((question_nomber+1)-score));

    ui->frame->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->frame_2->setEnabled(true);

    GetQuestion();

    ui->label_3->setText(question);

    randomCorrect = rand() % (4);

    switch (randomCorrect)
    {
    case 0:
        ui->radioButton_5->setText(trueAnswer);
        ui->radioButton_6->setText(wrongAnswers[0]);
        ui->radioButton_7->setText(wrongAnswers[1]);
        ui->radioButton_8->setText(wrongAnswers[2]);
        break;
    case 1:
        ui->radioButton_6->setText(trueAnswer);
        ui->radioButton_5->setText(wrongAnswers[0]);
        ui->radioButton_7->setText(wrongAnswers[1]);
        ui->radioButton_8->setText(wrongAnswers[2]);
        break;
    case 2:
        ui->radioButton_7->setText(trueAnswer);
        ui->radioButton_6->setText(wrongAnswers[0]);
        ui->radioButton_5->setText(wrongAnswers[1]);
        ui->radioButton_8->setText(wrongAnswers[2]);
        break;
    case 3:
        ui->radioButton_8->setText(trueAnswer);
        ui->radioButton_6->setText(wrongAnswers[0]);
        ui->radioButton_7->setText(wrongAnswers[1]);
        ui->radioButton_5->setText(wrongAnswers[2]);
        break;
    default:
        break;
    }

    connect(timer, &QTimer::timeout, this, &Survival_Form::on_pushButton_2_clicked);
    timer->start(15000);
    timerSec->start(1000);
}


void Survival_Form::on_pushButton_2_clicked()
{
    timer->stop();
    timerSec->stop();
    
    if (randomCorrect==0 && ui->radioButton_5->isChecked() == true) { score += 1; }
    else if (randomCorrect == 1 && ui->radioButton_6->isChecked() == true) { score += 1; }
    else if (randomCorrect == 2 && ui->radioButton_7->isChecked() == true) { score += 1; }
    else if (randomCorrect == 3 && ui->radioButton_8->isChecked() == true) { score += 1; }

    if (high_score < score) { high_score = score; }
    ui->label_HSc->setText(QString::number(high_score));
    ui->label_Sc->setText(QString::number(score));
    ui->label_wrongs->setText(QString::number((question_nomber + 1) - score));

    if (question_nomber - score == 2) {
        timer->stop();
        timer->disconnect();
        timerSec->stop();
        ui->frame_2->setEnabled(false);
        QMessageBox* msg = new QMessageBox;
        msg->setText("Your final score in " + difficaultyy + " Mode is " + QString::number(score));
        msg->exec();

        int any_score = 0;
        int easy_score = 0;
        int medium_score = 0;
        int hard_score = 0;

        QFile file("scores.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in >> any_score >> easy_score >> medium_score >> hard_score;
            file.close();
        }


        if (difficaultyy == "Any Difficulty") {
            if (score > any_score) {
                any_score = score;
            }
        }
        else if (difficaultyy == "easy") {
            if (score > easy_score) {
                easy_score = score;
            }
        }
        else if (difficaultyy == "medium") {
            if (score > medium_score) {
                medium_score = score;
            }
        }
        else if (difficaultyy == "hard") {
            if (score > hard_score) {
                hard_score = score;
            }
        }

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << any_score << '\n' << easy_score << '\n' << medium_score << '\n' << hard_score;
            file.close();
        }

        question_nomber = 0;
        high_score = 0;
        score = 0;
        close();
    }

    GetQuestion();

    ui->label_3->setText(question);

    randomCorrect = rand() % (4);

    switch (randomCorrect)
    {
    case 0:
        ui->radioButton_5->setText(trueAnswer);
        ui->radioButton_6->setText(wrongAnswers[0]);
        ui->radioButton_7->setText(wrongAnswers[1]);
        ui->radioButton_8->setText(wrongAnswers[2]);
        break;
    case 1:
        ui->radioButton_6->setText(trueAnswer);
        ui->radioButton_5->setText(wrongAnswers[0]);
        ui->radioButton_7->setText(wrongAnswers[1]);
        ui->radioButton_8->setText(wrongAnswers[2]);
        break;
    case 2:
        ui->radioButton_7->setText(trueAnswer);
        ui->radioButton_6->setText(wrongAnswers[0]);
        ui->radioButton_5->setText(wrongAnswers[1]);
        ui->radioButton_8->setText(wrongAnswers[2]);
        break;
    case 3:
        ui->radioButton_8->setText(trueAnswer);
        ui->radioButton_6->setText(wrongAnswers[0]);
        ui->radioButton_7->setText(wrongAnswers[1]);
        ui->radioButton_5->setText(wrongAnswers[2]);
        break;
    default:
        break;
    }

    ui->radioButton_9->setChecked(true);
    timer->start(15000);
    ui->label->setText("15");
    ui->label->setStyleSheet("color:black");
    timerSec->start(1000);
    question_nomber += 1;
}

void Survival_Form::UpdateCountdown() {
    timerSec->start(1000);
    double rtd = (timer->remainingTime() / 1000.0);
    int rt = round(rtd);
    if(rtd)
    if (rt == 5) {
        ui->label->setStyleSheet("color:red");
    }
    ui->label->setText(QString::fromStdString(std::to_string(rt)));
}

/*

static void closeEvent(QCloseEvent *event) {
    QMessageBox *exiting = new QMessageBox;
    exiting->setText("Are you sure you want to exit?");
    exiting->setStandardButtons( QMessageBox::Yes | QMessageBox::Cancel);
    exiting->setDefaultButton(QMessageBox::Cancel);
    exiting->setIcon(QMessageBox::Question);
    int result = exiting->exec();

    if (result != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}
*/
