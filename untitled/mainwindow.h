#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QString>
#include <QMainWindow>
#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int build_state;
    QString bg,ed;
    QVector<QString>locate;
    QPoint begin,end;
    QVector<QPoint> barrier;
    QVector<QPair<QPoint,QPoint>> barrier_rect;
    Ui::MainWindow *ui;
    int mouse_state;
    void show_data();
   // bool eventFilter(QObject *,QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
public slots:
    void on_pushButton_clicked();
    void on_pushButton_begin_clicked();
    void on_pushButton_end_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_barrier_clicked();
    void on_actionopen_triggered();
    void on_actionsave_triggered();
    void on_pushButton_mouse1_clicked();
    void on_pushButton_mouse2_clicked();
    void on_pushButton_mouse4_clicked();
    void on_pushButton_mouse3_clicked();
private:


};
#endif // MAINWINDOW_H
