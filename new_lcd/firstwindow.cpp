#include "firstwindow.h"
#include "ui_firstwindow.h"
#include <QPixmap>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDebug>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>

using namespace std;

FirstWindow::FirstWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirstWindow)
{
    ui->setupUi(this);
    QPixmap pix("Background.png");
    QIcon ButtonIcon(pix);
    QSize size(200,200);
    ui->pushButton_2->setIcon(ButtonIcon);
    ui->pushButton_2->setIconSize(size);
    ui->pushButton_2->setFixedSize(size);
}

FirstWindow::~FirstWindow()
{
    delete ui;
}

void FirstWindow::on_pushButton_2_clicked()
{
    string line = "0";
    QString str = QString::fromStdString(line);
    ui->lcdNumber->display(str);
    ui->lcdNumber_2->display(str);

    ui->stackedWidget->setCurrentIndex(1);

    // Player one goes first
    QMessageBox::about(this, "Player 1", "Player 1 get ready!!");

    ofstream myfile;
    myfile.open ("/dev/hcsr04");
    myfile << "1";
    myfile.close();

    QFont font = ui->label_6->font();
    font.setPointSize(20); 
    font.setBold(true);
    ui->label_6->setFont(font);

    count = 60;
    do{
        update_score1();
	usleep(500000);
	count--;
    }while(count > 0);
    myfile.open ("/dev/hcsr04");
    myfile << "2";
    myfile.close();

    // Player 2 goes now
    QMessageBox::about(this, "Player 2", "Player 2 get ready!!");
    myfile.open ("/dev/hcsr04");
    myfile << "1";
    myfile.close();

    count = 60;
    do{
        update_score2();
	usleep(500000);
	count--;
    }while(count > 0);
    
    myfile.open ("/dev/hcsr04");
    myfile << "3";
    myfile.close();
    if(p1_score > p2_score){
	QMessageBox::about(this,"Game Over", "Player 1 Wins!!!!!!");
    }
    else if (p2_score > p1_score){
	QMessageBox::about(this,"Game Over", "Player 2 Wins!!!!!");
    }
    else{
	QMessageBox::about(this,"Game Over", "It's a Tie!!!!!!");
    }

    myfile.open ("/dev/hcsr04");
    myfile << "4";
    myfile.close();
	
    ui->stackedWidget->setCurrentIndex(0);
}

void FirstWindow::update_score1()
{
    string line;
    ifstream myfile ("/proc/hcsr04");
    if (myfile.is_open()){
    	getline(myfile, line);
	myfile.close();
    }
    
    QString str = QString::fromStdString(line);
    ui->lcdNumber->display(str);

    istringstream iss1 (line);
    iss1 >> p1_score;

    char time_remaining[256];
    sprintf(time_remaining, "%s%d%s", "<font color='white'>Time Left: ", count / 2, " seconds</font>");
    ui->label_6->setText(time_remaining);

    QApplication::processEvents();
}

void FirstWindow::update_score2()
{
    string line;
    ifstream myfile ("/proc/hcsr04");
    if (myfile.is_open()){
    	getline(myfile, line);
	myfile.close();
    }
   
    //display current score
    QString str = QString::fromStdString(line);
    ui->lcdNumber_2->display(str);

    //get p2_score
    istringstream iss1 (line);
    iss1 >> p2_score;

    //time remaining
    char time_remaining[256];
    sprintf(time_remaining, "%s%d%s", "<font color='white'>Time Left: ", count / 2, " seconds</font>");
    ui->label_6->setText(time_remaining);

    QApplication::processEvents();
}
