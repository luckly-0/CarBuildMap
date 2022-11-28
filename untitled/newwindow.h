#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QDialog>
#include <QVector>
#include <map.h>
namespace Ui {
class NewWindow;
}

class NewWindow : public QDialog
{
    Q_OBJECT

public:
    double carx=10,cary=10,carr=20,carr1=20;
    double endx=10,endy=10,endr=20,endr1=20;
    int nowpoint = 0;
    QVector<int> map[20000];
    NewWindow(QWidget *parent,QPoint begin,QVector<QPoint > barrier,QPoint end);
    ~NewWindow();
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    QPoint begin,end;
    QVector<QPoint> barrier,Path;
    QVector<QPoint> Point ;
    QVector<QPoint> A_slove(int begin,int end);
    void toPoint();
    int time_state=1;
    QVector<QPoint> vis;
public slots:
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_continue_clicked();

private:
    Ui::NewWindow *ui;

};

#endif // NEWWINDOW_H
