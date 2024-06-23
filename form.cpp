#include "form.h"
#include "ui_form.h"
#include "qmessagebox.h"
#include "thread"
#include "QCloseEvent"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <ctime>
#include "QTimer"

QString category;
QString difficaulty;
int player1_score = 0;
int player2_score = 0;
int player1_tScore = 0;
int player2_tScore = 0;
int question_number = 0;
int currentQuestionCorrectAnswerIndex = 0;
int turn=0;
int golden_round_number = 0;
std::string uniqueToken = "";
void GetToken();

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

std::vector<std::string> Subjects;
std::vector<int> SubjctsId;
std::vector<QString> questions;
std::vector<std::vector<QString>> wrongAnswers;
std::vector<QString> trueAnswer;

std::string jsonData(std::string url) {
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

void Replacement(std::string& inp) {
    int pos;
    while ((pos = inp.find("&quot;")) != std::string::npos)
        inp.replace(pos, 6, "\"");
    while ((pos = inp.find("&amp;")) != std::string::npos)
        inp.replace(pos, 5, "&");
    while ((pos = inp.find("&#039;")) != std::string::npos)
        inp.replace(pos, 6, "\'");
}

void PvPRound(std::string difficultyLevel = "Any Difficulty", std::string subject = "Any Category") {
    questions.clear();
    trueAnswer.clear();
    wrongAnswers.clear();

resetToken:

    std::string url("https://opentdb.com/api.php?amount=5&type=multiple");
    if (uniqueToken != "") 
        url += "&token=" + uniqueToken;
    if (subject != "Any Category") {
        int subjectId;
        for (int i = 0; i < Subjects.size(); i++) {
            if (Subjects[i] == subject) {
                subjectId = SubjctsId[i];
                break;
            }
        }
        url += "&category=" + std::to_string(subjectId);
    } 
    if (difficultyLevel != "Any Difficulty")
        url += "&difficulty=" + difficultyLevel;

    std::string thisRoundItems = jsonData(url);

    if (thisRoundItems != "") {
        json js = json::parse(thisRoundItems);

        if (js["response_code"] == 0) {
            json res = js["results"];

            for (int i = 0; i < 5; i++) {
                std::string q = res[i]["question"];
                std::string ta = res[i]["correct_answer"];
                Replacement(q);
                Replacement(ta);

                questions.push_back(QString::fromStdString(q));
                trueAnswer.push_back(QString::fromStdString(ta));

                std::vector<QString> wa;
                json wajson = res[i]["incorrect_answers"];
                for (short j = 0; j < 3; j++) {
                    std::string temp = wajson[j];
                    Replacement(temp);
                    wa.push_back(QString::fromStdString(temp));
                }
                wrongAnswers.push_back(wa);
            }
        }
        else if (js["response_code"] == 1) {
            QMessageBox* msg = new QMessageBox;
            msg->setWindowTitle("Error!");
            msg->setText("Not enough questions on the server.");
            msg->exec();
        }
        else if (js["response_code"] == 3 || js["response_code"] == 4) {
            GetToken();
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

void GetSubjects() {
    const std::string url("https://opentdb.com/api_category.php");
    std::string response = jsonData(url);
    if (response == "") {
        return;
    }
    json subs = json::parse(response);

    for (int i = 0; i < subs["trivia_categories"].size(); i++) {
        Subjects.push_back(subs["trivia_categories"][i]["name"]);
        SubjctsId.push_back(subs["trivia_categories"][i]["id"]);
    }
}

void GetToken() {
    json tokenJ = json::parse(jsonData("https://opentdb.com/api_token.php?command=request"));
    if (tokenJ["response_code"] == 0)
        uniqueToken = tokenJ["token"];
}

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    setWindowTitle("Mode PVP");
    srand((unsigned)time(0));
    player1_score = 0;
    player2_score = 0;
    player1_tScore = 0;
    player2_tScore = 0;
    golden_round_number = 0;
    question_number = 0;
    turn=0;

    GetSubjects();
    if (Subjects.size() == 0) {
        close();
    }
    for (int i = 0; i < Subjects.size(); i++) {
        QString thisItem = QString::fromStdString(Subjects[i]);
        ui->comboBox->addItem(thisItem);
    }
    timer = new QTimer(this);
    ui->label_4->setStyleSheet("color:Blue");
    timerSec = new QTimer(this);
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    connect(timerSec, SIGNAL(timeout()), this, SLOT(UpdateCountdown()));

    if (Subjects.size() == 0) {
        QMessageBox* msg = new QMessageBox;
        msg->setWindowTitle("Error!");
        msg->setText("You might be disconnected.");
        msg->exec();
        close();
        return;
    }

    category = ui->comboBox->currentText();

    ui->radioButton_9->setChecked(true);
    ui->radioButton_5->setCheckable(true);
    ui->radioButton_6->setCheckable(true);
    ui->radioButton_7->setCheckable(true);
    ui->radioButton_8->setCheckable(true);

    if(ui->radioButton_1->isChecked()==true){difficaulty="Any Difficulty";}
    else if(ui->radioButton_2->isChecked()==true){difficaulty="easy";}
    else if(ui->radioButton_3->isChecked()==true){difficaulty="medium";}
    else if(ui->radioButton_4->isChecked()==true){difficaulty="hard";}
    ui->label_8->setText(QString::number(player1_score));
    ui->label_9->setText(QString::number(player2_score));
    ui->label_14->setText(QString::number(int(turn/2)+1 + golden_round_number));

    /*
    ui->label_5->setText((QString::number(player1_score))+"-"+QString::number(player2_score)+"-"+QString::number(turn)+"-"+QString::number(question_number));
    ui->radioButton_9->setChecked(true);
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);
    ui->radioButton_3->setChecked(false);
    ui->radioButton_4->setChecked(false);
    */

    question_number = 0;
    ui->frame->setEnabled(false);
    ui->frame_3->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->frame_2->setEnabled(true);
    //ui->frame_2->setHidden(false);
    ui->pushButton_2->setEnabled(true);

    PvPRound(difficaulty.toStdString(), category.toStdString());
    if (questions.size() == 0)
        return;

    ui->label_3->setText(questions[0]);
    int randomCorrect = rand() % (4);
    currentQuestionCorrectAnswerIndex = randomCorrect;
    
    switch (randomCorrect)
    {
    case 0:
        ui->radioButton_5->setText(trueAnswer[0]);
        ui->radioButton_6->setText(wrongAnswers[0][0]);
        ui->radioButton_7->setText(wrongAnswers[0][1]);
        ui->radioButton_8->setText(wrongAnswers[0][2]);
        break;
    case 1:
        ui->radioButton_6->setText(trueAnswer[0]);
        ui->radioButton_5->setText(wrongAnswers[0][0]);
        ui->radioButton_7->setText(wrongAnswers[0][1]);
        ui->radioButton_8->setText(wrongAnswers[0][2]);
        break;
    case 2:
        ui->radioButton_7->setText(trueAnswer[0]);
        ui->radioButton_6->setText(wrongAnswers[0][0]);
        ui->radioButton_5->setText(wrongAnswers[0][1]);
        ui->radioButton_8->setText(wrongAnswers[0][2]);
        break;
    case 3:
        ui->radioButton_8->setText(trueAnswer[0]);
        ui->radioButton_6->setText(wrongAnswers[0][0]);
        ui->radioButton_7->setText(wrongAnswers[0][1]);
        ui->radioButton_5->setText(wrongAnswers[0][2]);
        break;
    default:
        break;
    }

    connect(timer, &QTimer::timeout, this, &Form::on_pushButton_2_clicked);
    timer->start(30000);
    timerSec->start(1000);
}



void Form::on_pushButton_2_clicked()
{
    /*
    ui->radioButton_5->setChecked(false);
    ui->radioButton_6->setChecked(false);
    ui->radioButton_7->setChecked(false);
    ui->radioButton_8->setChecked(false);
    */
    //maks baray jabejayi gozine!!!!!!!!!!!!
    /*
    int flag = 0;
    if(ui->radioButton_5->isChecked()==false && ui->radioButton_6->isChecked()==false && ui->radioButton_7->isChecked()==false && ui->radioButton_8->isChecked()==false){
        ui->label_5->setText("Please choose an option!")
    }
    */

    timer->stop();
    timerSec->stop();
    question_number++;

    if (question_number >= 0 && question_number <= 5) {

        switch (currentQuestionCorrectAnswerIndex)
        {
        case 0:
            if (ui->radioButton_5->isChecked() == true) {
                if(turn % 2 == 0)
                    player1_score++;
                else
                    player2_score += 1;
            }
            break;
        case 1:
            if (ui->radioButton_6->isChecked() == true) {
                if (turn % 2 == 0)
                    player1_score++;
                else
                    player2_score += 1;
            }
            break;
        case 2:
            if (ui->radioButton_7->isChecked() == true) {
                if (turn % 2 == 0)
                    player1_score++;
                else
                    player2_score += 1;
            }
            break;
        case 3:
            if (ui->radioButton_8->isChecked() == true) {
                if (turn % 2 == 0)
                    player1_score++;
                else
                    player2_score += 1;
            }
            break;
        default:
            break;
        }
    }

    if (question_number >= 0 && question_number <= 4) {
        ui->label_3->setText(questions[question_number]);
        int randomCorrect = rand() % (4);
        int randomFalse = rand() % (3);
        currentQuestionCorrectAnswerIndex = randomCorrect;

        switch (randomCorrect)
        {
        case 0:
            ui->radioButton_5->setText(trueAnswer[question_number]);
            ui->radioButton_6->setText(wrongAnswers[question_number][randomFalse]);
            if (randomFalse == 0) {
                ui->radioButton_7->setText(wrongAnswers[question_number][1]);
                ui->radioButton_8->setText(wrongAnswers[question_number][2]);
            }
            else if (randomFalse == 1) {
                ui->radioButton_7->setText(wrongAnswers[question_number][0]);
                ui->radioButton_8->setText(wrongAnswers[question_number][2]);
            }
            else {
                ui->radioButton_7->setText(wrongAnswers[question_number][0]);
                ui->radioButton_8->setText(wrongAnswers[question_number][1]);
            }
            break;
        case 1:
            ui->radioButton_6->setText(trueAnswer[question_number]);
            ui->radioButton_5->setText(wrongAnswers[question_number][randomFalse]);
            if (randomFalse == 0) {
                ui->radioButton_7->setText(wrongAnswers[question_number][1]);
                ui->radioButton_8->setText(wrongAnswers[question_number][2]);
            }
            else if (randomFalse == 1) {
                ui->radioButton_7->setText(wrongAnswers[question_number][0]);
                ui->radioButton_8->setText(wrongAnswers[question_number][2]);
            }
            else {
                ui->radioButton_7->setText(wrongAnswers[question_number][0]);
                ui->radioButton_8->setText(wrongAnswers[question_number][1]);
            }
            break;
        case 2:
            ui->radioButton_7->setText(trueAnswer[question_number]);
            ui->radioButton_6->setText(wrongAnswers[question_number][randomFalse]);
            if (randomFalse == 0) {
                ui->radioButton_5->setText(wrongAnswers[question_number][1]);
                ui->radioButton_8->setText(wrongAnswers[question_number][2]);
            }
            else if (randomFalse == 1) {
                ui->radioButton_8->setText(wrongAnswers[question_number][0]);
                ui->radioButton_5->setText(wrongAnswers[question_number][2]);
            }
            else {
                ui->radioButton_5->setText(wrongAnswers[question_number][0]);
                ui->radioButton_8->setText(wrongAnswers[question_number][1]);
            }
            break;
        case 3:
            ui->radioButton_8->setText(trueAnswer[question_number]);
            ui->radioButton_6->setText(wrongAnswers[question_number][randomFalse]);
            if (randomFalse == 0) {
                ui->radioButton_7->setText(wrongAnswers[question_number][1]);
                ui->radioButton_5->setText(wrongAnswers[question_number][2]);
            }
            else if (randomFalse == 1) {
                ui->radioButton_5->setText(wrongAnswers[question_number][0]);
                ui->radioButton_7->setText(wrongAnswers[question_number][2]);
            }
            else {
                ui->radioButton_7->setText(wrongAnswers[question_number][0]);
                ui->radioButton_5->setText(wrongAnswers[question_number][1]);
            }
            break;
        default:
            break;
        }

        if (question_number == 4) {
            ui->pushButton_2->setText("Finish!");
        }
    }

    else if (question_number == 5 && turn % 2 == 0) {

        ui->label_3->setText("Question");
        ui->radioButton_5->setText("Option 1");
        ui->radioButton_6->setText("Option 2");
        ui->radioButton_7->setText("Option 3");
        ui->radioButton_8->setText("Option 4");
        ui->label_4->setText("Player 2");
        ui->pushButton_2->setText("Next");
        ui->pushButton->setEnabled(true);
        //std::this_thread::sleep_for(std::chrono::seconds(5));
        //ui->frame_2->setEnabled(false);
        //ui->frame_2->setVisible(false);
        ui->pushButton_2->setEnabled(false);

        ui->radioButton_9->setChecked(true);
        ui->radioButton_5->setCheckable(false);
        ui->radioButton_6->setCheckable(false);
        ui->radioButton_7->setCheckable(false);
        ui->radioButton_8->setCheckable(false);

        turn += 1;
    }
    else if (question_number == 5 && turn % 2 == 1 && turn < 7) {
        ui->label_3->setText("Question");
        ui->radioButton_5->setText("Option 1");
        ui->radioButton_6->setText("Option 2");
        ui->radioButton_7->setText("Option 3");
        ui->radioButton_8->setText("Option 4");
        ui->label_4->setText("Player 1");
        ui->pushButton_2->setText("Next");
        ui->frame->setEnabled(true);
        ui->frame_3->setEnabled(true);
        ui->pushButton->setEnabled(true);
        //ui->frame_2->setEnabled(false);
        //ui->frame_2->setVisible(false);
        ui->pushButton_2->setEnabled(false);

        ui->radioButton_9->setChecked(true);
        ui->radioButton_5->setCheckable(false);
        ui->radioButton_6->setCheckable(false);
        ui->radioButton_7->setCheckable(false);
        ui->radioButton_8->setCheckable(false);

        if (player1_score > player2_score) {
            player1_tScore++;
            ui->label_10->setText(QString::number(player1_tScore));
        }
        else if (player1_score < player2_score) {
            player2_tScore++;
            ui->label_11->setText(QString::number(player2_tScore));
        }
        else {
            player1_tScore++;
            ui->label_10->setText(QString::number(player1_tScore));
            player2_tScore++;
            ui->label_11->setText(QString::number(player2_tScore));
        }
        player1_score = 0;
        player2_score = 0;
        timer->stop();
        timer->disconnect();
        timerSec->stop();
        timerSec->disconnect();
        ui->label_12->setText("30");
        ui->label_12->setStyleSheet("color:black");

        turn += 1;
    }
    else {
        if (player1_score > player2_score) {
            player1_tScore++;
            ui->label_10->setText(QString::number(player1_tScore));
        }
        else if (player1_score < player2_score) {
            player2_tScore++;
            ui->label_11->setText(QString::number(player2_tScore));
        }
        else {
            player1_tScore++;
            ui->label_10->setText(QString::number(player1_tScore));
            player2_tScore++;
            ui->label_11->setText(QString::number(player2_tScore));
        }
        player1_score = 0;
        player2_score = 0;
        timer->stop();
        timer->disconnect();
        timerSec->stop();
        timerSec->disconnect();
        ui->label_12->setText("30");
        ui->label_12->setStyleSheet("color:black");

        if (question_number == 5 && turn % 2 == 1 && turn == 7 && player1_tScore == player2_tScore) {
        ui->label_3->setText("Question");
        ui->radioButton_5->setText("Option 1");
        ui->radioButton_6->setText("Option 2");
        ui->radioButton_7->setText("Option 3");
        ui->radioButton_8->setText("Option 4");
        ui->pushButton_2->setText("Next");
        ui->label_4->setText("Player 1");
        ui->pushButton->setEnabled(true);
        //ui->frame_2->setEnabled(false);
        //ui->frame_2->setHidden(true);
        ui->pushButton_2->setEnabled(false);

        ui->radioButton_9->setChecked(true);
        ui->radioButton_5->setCheckable(false);
        ui->radioButton_6->setCheckable(false);
        ui->radioButton_7->setCheckable(false);
        ui->radioButton_8->setCheckable(false);

        if (golden_round_number == 0) {
            ui->frame->setEnabled(true);
            ui->frame_3->setEnabled(true);
            ui->comboBox->setCurrentIndex(0);
            ui->radioButton_1->setChecked(true);
            category = "Any Category";
            difficaulty = "Any Difficulty";
            ui->frame->setEnabled(false);
            ui->frame_3->setEnabled(false);
        }

        turn -= 1;
        golden_round_number += 1;
    }
        else if (question_number == 5 && turn % 2 == 1 && turn == 7 && player1_tScore != player2_tScore) {

        QMessageBox* msg = new QMessageBox;
        ui->frame_2->setEnabled(false);
        msg->setWindowTitle("Result");
        if (player1_tScore > player2_tScore) {
            ui->pushButton_2->setText("Next");
            msg->setText("Player 1 win!");
            msg->exec();
        }
        else {
            ui->pushButton_2->setText("Next");
            msg->setText("Player 2 win!");
            msg->exec();
        }
        question_number = 0;
        turn = 0;
        player1_tScore = 0;
        player2_tScore = 0;
        player1_score = 0;
        player2_score = 0;
        golden_round_number = 0;
        close();
    }
    }
    
    ui->radioButton_9->setChecked(true);
    //ui->radioButton_5->setVisible(false);
    //ui->radioButton_9->set
    //if(question_number==4){
    //    std::this_thread::sleep_for(std::chrono::seconds(5));
    //}
    //ui->label_5->setText((QString::number(player1_score))+"-"+QString::number(player2_score)+"-"+QString::number(turn)+"-"+QString::number(question_number));
    ui->label_8->setText(QString::number(player1_score));
    ui->label_9->setText(QString::number(player2_score));
    ui->label_14->setText(QString::number(int(turn / 2) + 1 + golden_round_number));

    if (turn % 2 == 1)
        ui->label_4->setStyleSheet("color:Green");
    else
        ui->label_4->setStyleSheet("color:Blue");

    timer->start(30000);
    if(question_number != 5)
        timerSec->start(1000);
    ui->label_12->setText("30");
    ui->label_12->setStyleSheet("color:black");
}

void Form::UpdateCountdown() {
    timerSec->start(1000);
    double rtd = timer->remainingTime() / 1000.0;
    int rt = round(rtd);
    if (rt == 5) {
        ui->label_12->setStyleSheet("color:red");
    }
    ui->label_12->setText(QString::fromStdString(std::to_string(rt)));
}

/*
void closeEvent(QCloseEvent *event) {
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
