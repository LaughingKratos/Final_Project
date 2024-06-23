/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QPushButton *pushButton;
    QFrame *frame;
    QRadioButton *radioButton_3;
    QLabel *label_2;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_1;
    QFrame *frame_2;
    QRadioButton *radioButton_8;
    QRadioButton *radioButton_7;
    QRadioButton *radioButton_6;
    QRadioButton *radioButton_5;
    QLabel *label_3;
    QPushButton *pushButton_2;
    QRadioButton *radioButton_9;
    QFrame *frame_3;
    QComboBox *comboBox;
    QLabel *label;
    QLabel *label_4;
    QFrame *frame_4;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_6;
    QLabel *label_9;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_10;
    QLabel *label_11;
    QFrame *frame_5;
    QLabel *label_5;
    QLabel *label_12;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->setEnabled(true);
        Form->resize(814, 463);
        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(450, 90, 81, 31));
        frame = new QFrame(Form);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(40, 80, 371, 41));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        radioButton_3 = new QRadioButton(frame);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(230, 10, 61, 17));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 47, 13));
        radioButton_4 = new QRadioButton(frame);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));
        radioButton_4->setGeometry(QRect(300, 10, 51, 17));
        radioButton_2 = new QRadioButton(frame);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(170, 10, 51, 17));
        radioButton_1 = new QRadioButton(frame);
        radioButton_1->setObjectName(QString::fromUtf8("radioButton_1"));
        radioButton_1->setGeometry(QRect(70, 10, 91, 17));
        radioButton_1->setChecked(true);
        frame_2 = new QFrame(Form);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setEnabled(false);
        frame_2->setGeometry(QRect(40, 150, 721, 251));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Raised);
        radioButton_8 = new QRadioButton(frame_2);
        radioButton_8->setObjectName(QString::fromUtf8("radioButton_8"));
        radioButton_8->setGeometry(QRect(370, 160, 331, 17));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        radioButton_8->setFont(font);
        radioButton_8->setAutoRepeatDelay(300);
        radioButton_7 = new QRadioButton(frame_2);
        radioButton_7->setObjectName(QString::fromUtf8("radioButton_7"));
        radioButton_7->setGeometry(QRect(20, 160, 341, 17));
        radioButton_7->setFont(font);
        radioButton_7->setAutoRepeatDelay(300);
        radioButton_6 = new QRadioButton(frame_2);
        radioButton_6->setObjectName(QString::fromUtf8("radioButton_6"));
        radioButton_6->setGeometry(QRect(370, 110, 331, 17));
        radioButton_6->setFont(font);
        radioButton_6->setAutoRepeatDelay(300);
        radioButton_5 = new QRadioButton(frame_2);
        radioButton_5->setObjectName(QString::fromUtf8("radioButton_5"));
        radioButton_5->setGeometry(QRect(20, 110, 341, 17));
        radioButton_5->setFont(font);
        radioButton_5->setAutoRepeatDelay(300);
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 20, 651, 71));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(12);
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_3->setWordWrap(true);
        pushButton_2 = new QPushButton(frame_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(600, 200, 81, 31));
        radioButton_9 = new QRadioButton(frame_2);
        radioButton_9->setObjectName(QString::fromUtf8("radioButton_9"));
        radioButton_9->setGeometry(QRect(370, 160, 331, 17));
        QFont font2;
        font2.setPointSize(10);
        radioButton_9->setFont(font2);
        radioButton_9->raise();
        radioButton_8->raise();
        radioButton_7->raise();
        radioButton_6->raise();
        radioButton_5->raise();
        label_3->raise();
        pushButton_2->raise();
        frame_3 = new QFrame(Form);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setEnabled(true);
        frame_3->setGeometry(QRect(39, 30, 371, 41));
        frame_3->setFrameShape(QFrame::Panel);
        frame_3->setFrameShadow(QFrame::Raised);
        comboBox = new QComboBox(frame_3);
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(70, 10, 291, 21));
        QFont font3;
        font3.setPointSize(9);
        comboBox->setFont(font3);
        comboBox->setContextMenuPolicy(Qt::DefaultContextMenu);
        label = new QLabel(frame_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 51, 16));
        label_4 = new QLabel(Form);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(450, 40, 81, 31));
        QFont font4;
        font4.setPointSize(13);
        label_4->setFont(font4);
        label_4->setStyleSheet(QString::fromUtf8("color:Blue"));
        label_4->setAlignment(Qt::AlignCenter);
        frame_4 = new QFrame(Form);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(610, 40, 111, 81));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_7 = new QLabel(frame_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 30, 71, 21));
        QFont font5;
        font5.setPointSize(8);
        label_7->setFont(font5);
        label_8 = new QLabel(frame_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(90, 10, 21, 21));
        label_8->setFont(font5);
        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 10, 71, 21));
        label_6->setFont(font5);
        label_9 = new QLabel(frame_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(90, 30, 21, 21));
        label_9->setFont(font5);
        label_14 = new QLabel(frame_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(90, 50, 21, 21));
        label_14->setFont(font5);
        label_15 = new QLabel(frame_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 50, 71, 21));
        label_15->setFont(font5);
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_10 = new QLabel(frame_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(100, 10, 21, 21));
        label_10->setFont(font5);
        label_11 = new QLabel(frame_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(100, 30, 21, 21));
        label_11->setFont(font5);
        frame_5 = new QFrame(Form);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setEnabled(true);
        frame_5->setGeometry(QRect(39, 150, 721, 251));
        frame_5->setFrameShape(QFrame::Box);
        frame_5->setFrameShadow(QFrame::Raised);
        label_5 = new QLabel(Form);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(780, 40, 20, 20));
        label_5->setFont(font2);
        label_5->setAlignment(Qt::AlignCenter);
        label_12 = new QLabel(Form);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(560, 60, 31, 31));
        QFont font6;
        font6.setPointSize(15);
        label_12->setFont(font6);
        label_12->setStyleSheet(QString::fromUtf8("color:black"));
        frame_5->raise();
        pushButton->raise();
        frame->raise();
        frame_2->raise();
        frame_3->raise();
        label_4->raise();
        frame_4->raise();
        label_5->raise();
        label_12->raise();

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("Form", "Start Round", nullptr));
        radioButton_3->setText(QCoreApplication::translate("Form", "Medium", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "Difficulty:", nullptr));
        radioButton_4->setText(QCoreApplication::translate("Form", "Hard", nullptr));
        radioButton_2->setText(QCoreApplication::translate("Form", "Easy", nullptr));
        radioButton_1->setText(QCoreApplication::translate("Form", "Any Difficulty", nullptr));
        radioButton_8->setText(QCoreApplication::translate("Form", "Option 4", nullptr));
        radioButton_7->setText(QCoreApplication::translate("Form", "Option 3", nullptr));
        radioButton_6->setText(QCoreApplication::translate("Form", "Option 2", nullptr));
        radioButton_5->setText(QCoreApplication::translate("Form", "Option 1", nullptr));
        label_3->setText(QCoreApplication::translate("Form", "Question", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Form", "Next", nullptr));
        radioButton_9->setText(QString());
        comboBox->setItemText(0, QCoreApplication::translate("Form", "Any Category", nullptr));

        label->setText(QCoreApplication::translate("Form", "Category:", nullptr));
        label_4->setText(QCoreApplication::translate("Form", "Player 1", nullptr));
        label_7->setText(QCoreApplication::translate("Form", "Player 2 score:", nullptr));
        label_8->setText(QCoreApplication::translate("Form", "0", nullptr));
        label_6->setText(QCoreApplication::translate("Form", "Player 1 score:", nullptr));
        label_9->setText(QCoreApplication::translate("Form", "0", nullptr));
        label_14->setText(QCoreApplication::translate("Form", "0", nullptr));
        label_15->setText(QCoreApplication::translate("Form", "Round:", nullptr));
        label_10->setText(QCoreApplication::translate("Form", "0", nullptr));
        label_11->setText(QCoreApplication::translate("Form", "0", nullptr));
        label_5->setText(QString());
        label_12->setText(QCoreApplication::translate("Form", "30", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
