#include "form.h"
#include "ui_form.h"
#include "qmessagebox.h"
#include "thread"
#include "QCloseEvent"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

QString category;
QString difficaulty;
int player1_score = 0;
int player2_score = 0;
int question_number = 0;
int turn=0;
int golden_round_number = 0;

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

void PvPRound(std::string difficultyLevel = "", std::string subject = "Any Category") {
    std::string uniqueToken = "";

    std::string url("https://opentdb.com/api.php?amount=5");
    if (subject != "Any Category") {
        int subjectId;
        for (int i = 0; i < Subjects.size(); i++) {
            if (Subjects[i] == subject) {
                subjectId = SubjctsId[i];
            }
        }
        url += "&category=" + subjectId;
    } 
    if (difficultyLevel != "")
        url += "&difficulty=" + difficultyLevel;

    std::string thisRoundItems = jsonData(url);

    if (thisRoundItems != "") {
        json js = json::parse(thisRoundItems);

        if (js["response_code"] == 0) {
            json res = js["results"];

            for (int i = 0; i < 5; i++) {
                questionsVec.push_back(res[i]["question"]);
                trueAnswer.push_back(res[i]["correct_answer"]);

                std::vector<std::string> wa;
                json wajson = res[i]["incorrect_answers"];
                for (short j = 0; j < 3; j++) {
                    wa.push_back(wajson[j]);
                }
                wrongAnswers.push_back(wa);
            }
        }
    }
    else {
        
    }
}

void GetSubjects() {
    const std::string url("https://opentdb.com/api_category.php");
    json subs = json::parse(jsonData(url));

    for (int i = 0; i < subs["trivia_categories"].size(); i++) {
        Subjects.push_back(subs["trivia_categories"][i]["name"]);
        SubjctsId.push_back(subs["trivia_categories"][i]["id"]);
    }
}

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    setWindowTitle("Mode PVP");
    player1_score = 0;
    player2_score = 0;
    question_number = 0;
    turn=0;

    GetSubjects();
    for (int i = 0; i < Subjects.size(); i++) {
        QString thisItem = QString::fromStdString(Subjects[i]);
        ui->comboBox->addItem(thisItem);
    }
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{

    category = ui->comboBox->currentText();

    ui->radioButton_9->setChecked(true);
    ui->radioButton_5->setCheckable(true);
    ui->radioButton_6->setCheckable(true);
    ui->radioButton_7->setCheckable(true);
    ui->radioButton_8->setCheckable(true);

    if(ui->radioButton_1->isChecked()==true){difficaulty="Any Difficulty";}
    else if(ui->radioButton_2->isChecked()==true){difficaulty="Easy";}
    else if(ui->radioButton_3->isChecked()==true){difficaulty="Medium";}
    else if(ui->radioButton_4->isChecked()==true){difficaulty="Hard";}
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

    ui->label_3->setText(questions[0][0]);
    ui->radioButton_5->setText(questions[0][1]);
    ui->radioButton_6->setText(questions[0][2]);
    ui->radioButton_7->setText(questions[0][3]);
    ui->radioButton_8->setText(questions[0][4]);

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

    if(question_number>=0 && question_number<=4){

        if(questions[question_number][5]==questions[question_number][1] && ui->radioButton_5->isChecked()==true && turn%2==0){player1_score+=1;}
        else if(questions[question_number][5]==questions[question_number][1] && ui->radioButton_5->isChecked()==true && turn%2==1){player2_score+=1;}
        else if(questions[question_number][5]==questions[question_number][2] && ui->radioButton_6->isChecked()==true && turn%2==0){player1_score+=1;}
        else if(questions[question_number][5]==questions[question_number][2] && ui->radioButton_6->isChecked()==true && turn%2==1){player2_score+=1;}
        else if(questions[question_number][5]==questions[question_number][3] && ui->radioButton_7->isChecked()==true && turn%2==0){player1_score+=1;}
        else if(questions[question_number][5]==questions[question_number][3] && ui->radioButton_7->isChecked()==true && turn%2==1){player2_score+=1;}
        else if(questions[question_number][5]==questions[question_number][4] && ui->radioButton_8->isChecked()==true && turn%2==0){player1_score+=1;}
        else if(questions[question_number][5]==questions[question_number][4] && ui->radioButton_8->isChecked()==true && turn%2==1){player2_score+=1;}
    }

    if(question_number>=0 && question_number<=3){
        ui->label_3->setText(questions[question_number+1][0]);
        ui->radioButton_5->setText(questions[question_number+1][1]);
        ui->radioButton_6->setText(questions[question_number+1][2]);
        ui->radioButton_7->setText(questions[question_number+1][3]);
        ui->radioButton_8->setText(questions[question_number+1][4]);
    }

    if(question_number==3){
        ui->pushButton_2->setText("Finish!");
    }
    else if(question_number==4 && turn%2==0){

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


        turn+=1;
    }
    else if(question_number==4 && turn%2==1 && turn<7){
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

        turn+=1;
    }
    else if(question_number==4 && turn%2==1 && turn==7 && player1_score == player2_score){
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

        turn-=1;
        golden_round_number += 1;
    }
    else if(question_number==4 && turn%2==1 && turn==7 && player1_score != player2_score){
        QMessageBox *msg = new QMessageBox;
        ui->frame_2->setEnabled(false);
        msg->setWindowTitle("Result");
        if (player1_score > player2_score){
            ui->pushButton_2->setText("Next");
            msg->setText("Player 1 win!");
            msg->exec();
        }
        else{
            ui->pushButton_2->setText("Next");
            msg->setText("Player 2 win!");
            msg->exec();
        }
        question_number=0;
        turn=0;
        player1_score=0;
        player2_score=0;
        close();
    }
    ui->radioButton_9 ->setChecked(true);
    //ui->radioButton_5->setVisible(false);
    //ui->radioButton_9->set
    //if(question_number==4){
    //    std::this_thread::sleep_for(std::chrono::seconds(5));
    //}
    //ui->label_5->setText((QString::number(player1_score))+"-"+QString::number(player2_score)+"-"+QString::number(turn)+"-"+QString::number(question_number));
    ui->label_8->setText(QString::number(player1_score));
    ui->label_9->setText(QString::number(player2_score));
    ui->label_14->setText(QString::number(int(turn/2)+1 + golden_round_number));
    question_number++;

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
