#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <newwindow.h>
#include <QFileDialog>
#include <QString>
#include <QPainter>
#include <QMouseEvent>
#include <map.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("小车调度算法演示");
    resize(800,800);
   // setAcceptDrops(true);
    ui->Widget->installEventFilter(this);
}
void MainWindow::on_pushButton_clicked()
{

    NewWindow d(this,begin,barrier,end) ;
    d.exec();
}
void MainWindow::on_pushButton_barrier_clicked()
{
    QString strTxtEdt = ui->textEdit->toPlainText();
    QString strTxtEdt2 = ui->textEdit_2->toPlainText();
    barrier.push_back(QPoint(strTxtEdt.toDouble(),strTxtEdt2.toDouble()));
    locate.push_back('('+strTxtEdt+","+strTxtEdt2+") ");
    show_data();
}
void MainWindow::on_pushButton_begin_clicked()
{
    QString strTxtEdt = ui->textEdit->toPlainText();
    QString strTxtEdt2 = ui->textEdit_2->toPlainText();
    begin = {QPoint(strTxtEdt.toDouble(),strTxtEdt2.toDouble())};
    show_data();
}
void MainWindow::on_pushButton_end_clicked()
{
    QString strTxtEdt = ui->textEdit->toPlainText();
    QString strTxtEdt2 = ui->textEdit_2->toPlainText();
    end = {QPoint(strTxtEdt.toDouble(),strTxtEdt2.toDouble())};
    show_data();
}
void MainWindow::on_pushButton_del_clicked(){
    qDebug()<<"撤销中"<<'\n';
    if(barrier.size())barrier.pop_back();
    else qDebug()<<"撤销失败"<<'\n';
    show_data();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow:: show_data(){
    QString ans = "";
    ans.clear();
    ans += "开始位置: ("+QString::number(begin.x())+","+QString::number(begin.y())+")\n障碍位置: ";
    for(int i = 0 ;i<barrier.size();i++){
        ans += "("+QString::number(barrier[i].x())+","+QString::number(barrier[i].y())+") ";
    }
    ans += "\n结束位置: ("+QString::number(end.x())+","+QString::number(end.y())+")";
    ui->textBrowser->setText(ans);
}
void MainWindow::on_actionopen_triggered(){
    QString file_path = QFileDialog :: getOpenFileName(this,"打开文件","C:\\1x\\untitled");
    QFile file(file_path),ffile(file_path);
    file.open(QIODevice::ReadOnly);
    ffile.open(QIODevice::ReadOnly);
    QTextStream cin(&file);//将文件绑定到流对象上
    int t ;
    cin >> begin.rx()>> begin.ry();
    cin >> t ;
    barrier.clear();
    barrier.resize(t);
    for(int i = 0;i<t;i++){
        cin >> barrier[i].rx() >> barrier[i].ry();
        qDebug() << barrier[i].rx() << " " << barrier[i].ry()<<'\n';
    }
    cin >> end.rx() >> end.ry();
    QByteArray arrary = ffile.readAll();
    ui ->textBrowser->setText(arrary);
    show_data();
}
void MainWindow::on_actionsave_triggered(){
    QString file_path = QFileDialog :: getSaveFileName(this,"保存文件","C:\\1x\\untitled");
    QFile file(file_path);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out<<begin.x()<<" "<<begin.y()<<"\n"<<barrier.size()<<" ";
    for (int i = 0;i<barrier.size();i++){
        out<<barrier[i].x()<<" "<<barrier[i].y()<<" ";
    }
    out<<end.x()<<" "<<end.y();
    file.close();
}
void MainWindow::paintEvent(QPaintEvent *){
    int win_width = this->geometry().width()*0.7;
    int win_height = this->geometry().height();

    QPainter painter(this);

   // qDebug()<<"win size:"<<win_width<<"   "<<win_height;
    for(int x=0;x<win_width;x+=32)
    {
        painter.drawLine(QPoint(x,1),QPoint(x,win_height));
    }

    for(int y = 0;y<win_height;y+=32)
    {
        painter.drawLine(QPoint(1,y),QPoint(win_width,y));
    }
    painter.setBrush(Qt::black);
    for(QPoint i : barrier){
        QPoint j = i;
        painter.drawRect(j.x()-12,j.y()-12,24,24);
    }
    painter.setBrush(Qt::red);
     painter.drawRect(begin.x()-8,begin.y()-8,16,16);
    painter.setBrush(Qt::green);
       painter.drawRect(end.x()-8,end.y()-8,16,16);

}
void MainWindow::mousePressEvent(QMouseEvent *event){
    QPoint pos = event->pos();
    qDebug() << pos <<'\n';
    if(mouse_state==1) {
        begin = pos;
    }
    else if(mouse_state==2){
        barrier.push_back(pos);
    }
    else if(mouse_state==3){
        end = pos;
    }
    update();
    show_data();
}
void MainWindow::on_pushButton_mouse1_clicked(){
    mouse_state = 1;
}
void MainWindow::on_pushButton_mouse2_clicked(){
    mouse_state = 2;
}
void MainWindow::on_pushButton_mouse3_clicked(){
    mouse_state = 3;
}
void MainWindow::on_pushButton_mouse4_clicked(){
    if(barrier.size())barrier.pop_back();
    update();
}
