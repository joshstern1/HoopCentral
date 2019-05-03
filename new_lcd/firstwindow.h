#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class FirstWindow;
}

class FirstWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FirstWindow(QWidget *parent = 0);
    ~FirstWindow();

    int count;
    int p1_score;
    int p2_score;

private slots:
    void MyTimerSlot();
    void update_score1();
    void update_score2();

    void on_pushButton_2_clicked();


private:
    Ui::FirstWindow *ui;
};

#endif // FIRSTWINDOW_H
