#include "newwindow.h"
#include "ui_newwindow.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include<complex>
#include<QDebug>
#include "math.h"
#include<mainwindow.h>
#include<a_start.h>
#include<map.h>
#include<algorithm>
#include <queue>
#define _USE_MATH_DEFINES
NewWindow::NewWindow(QWidget *parent,QPoint begin,QVector<QPoint > barrier,QPoint end):
    QDialog(parent),
    ui(new Ui::NewWindow)
{
    ui->setupUi(this);
    setWindowTitle("实际演示");
    resize(800,600);
    this->barrier = Path = barrier;
    this->begin = {begin.x()-8,begin.y()-8};
    this->end = {end.x()-8,end.y()-8};
    //toPoint();
    QVector<QPair<QPoint,QPoint>> barrier_rect;

    for(QPoint j : barrier){
        barrier_rect.push_back({{j.x()-12,j.y()-12},{j.x()+12,j.y()+12}});
    }

    Map *k = new Map(this,800,800,barrier_rect,{begin.x()-8,begin.y()-8},{end.x()-8,end.y()-8});
    k->show();

    for(int i = 0;i<20000;i++){
        this->map[i] = k->map[i];
    }

    Point = k->v;
    A_start p(begin,barrier,end,k->v,k->map);
    QPair<int,int> bx= k->get_id(begin.x(),begin.y(),1,1),ex= k->get_id(end.x(),end.y(),1,1);
    qDebug() << "begin" << begin.x()-8<<" "<<begin.y()-8<<'\n';
    qDebug() << "end" << end.x()-8<<" "<<end.y()-8<<'\n';
    qDebug() << "newbegin" << k->get_center(bx.first,bx.second,1) <<'\n';
    qDebug() << "newend" << k->get_center(ex.first,ex.second,1)<<'\n';
    qDebug() << "newbegin" << Point[k->to_node[{{{bx.first,bx.second},1}}]] <<'\n';
    qDebug() << "newend" <<Point[k->to_node[{{{ex.first,ex.second},1}}]]<<'\n';
    int begin_ind = k->to_node[{{{bx.first,bx.second},1}}],end_ind = k->to_node[{{{ex.first,ex.second},1}}];
    Path = p.A_start_slove(begin_ind,end_ind);
    //vis = p.A_start_slove(begin_ind,end_ind);
    QTimer *timer1 = new QTimer(this);
    for (QPoint i : Path){
        qDebug() << i << " ";
    }
    if(Path.size()==0) {
        qDebug() << "无路";
    }
    else {


    Path.insert(0,begin);
    Path.push_back(end);
    carx = Path[0].x(),cary = Path[0].y();
    nowpoint = 1;
    timer1->start(0.1);
    connect(timer1,&QTimer::timeout,this,[=](){
        static int num = 0;
        QString str=QString::number(num++);
        ui->Label->setText(str);
        if(nowpoint==Path.size()) timer1->stop();
        else {
        double tx1 = Path[nowpoint].x(),ty1 = Path[nowpoint].y();
        double temp = sqrt((tx1-carx)*(tx1-carx)+(ty1-cary)*(ty1-cary));
        double k1 = (tx1-carx)/temp,k2 = (ty1-cary)/temp;
        if(time_state==1){
        carx += k1*1;
        cary += k2*1;}
        // qDebug()<<"小车位置"<<carx<<" "<<cary<<" "<<nowpoint<<'\n';
        if(abs(carx-tx1)<=0.5&&abs(cary-ty1)<=0.5)nowpoint++;
        update();}
    });
    }
}
QVector<QPoint> NewWindow::A_slove(int begin_index,int end_index){
    using namespace  std;
    QVector<QPoint> Path;
    vector<int> open(1000,0),close(1000,0),dist(1000,0x3f3f3f3f),H(1000,0x3f3f3f3f);
    priority_queue<pair<double,int>,vector<pair<double,int>>,std::greater<pair<double,int> > > q;
    vector<int> fa(1000,0);  //  记录第i个点父亲节点，用来回溯找路径
    q.push({0,begin_index});
    dist[0] = 0;
    while(!q.empty()){
        int u = q.top().second;
        if(u == end_index)
            break;
        close[u] = 1;
        vis.push_back(Point[u]);
        q.pop();
        for(int i : map[u]){
            if(close[i]) continue;
            double g = sqrt((Point[i].x()-Point[u].x())*(Point[i].x()-Point[u].x())+(Point[i].y()-Point[u].y())*(Point[i].y()-Point[u].y()));
            H[i] = sqrt((Point[i].x()-Point[end_index].x())*(Point[i].x()-Point[end_index].x())+(Point[i].y()-Point[end_index].y())*(Point[i].y()-Point[end_index].y()));
            if(!open[i]||dist[u] + H[u] > dist[u] + g + H[i]) {
                dist[i] = dist[u] + g;
                open[i] = 1;
                q.push({dist[i]+H[i],i});
                fa[i] = u;
            }
        }
    }
    int now = end_index;
    if(dist[end_index] < 0x3f3f3f3f) {
        while(1){
            Path.push_back(Point[now]);
            if(now == begin_index) break;
            now = fa[now];
        }
    }
    else qDebug()<< "找不到路"<<" ";
    QVector<QPoint> t(Path.rbegin(),Path.rend());
    return t;
}
NewWindow::~NewWindow()
{
    delete ui;
}
void NewWindow::timerEvent(QTimerEvent *){

    static int num = 0;
    QString str=QString::number(num++);
    ui->Label->setText(str);
}
void NewWindow::paintEvent(QPaintEvent *){
//    int win_width = this->geometry().width();
//    int win_height = this->geometry().height();

    QPainter painter(this);

////    //qDebug()<<"win size:"<<win_width<<"   "<<win_height;
////    for(int x=0;x<win_width;x+=25)
////    {
////        painter.drawLine(QPoint(x,1),QPoint(x,win_height));
////    }

////    for(int y = 0;y<win_height;y+=25)
////    {
////        painter.drawLine(QPoint(1,y),QPoint(win_width,y));
////    }
////    painter.setRenderHint(QPainter::Antialiasing, true);
//    QPen mypen;
//    mypen.setWidth(1);
//    mypen.setColor(Qt::black);

//    painter.setBrush(Qt::red);
//    painter.drawRect(end.x()-8,end.y()-8,16,16);
//    painter.setBrush(Qt::yellow);
//    painter.drawRect(begin.x()-8,begin.y()-8,16,16);
//   // qDebug ()<< end << '\n';
//    painter.setBrush(Qt::black);
    painter.setBrush(Qt::yellow);
    painter.drawRect(carx-8,cary-8,16,16);
//    painter.setPen(mypen);
//    for(int i = 0;i+1<Path.size();i++){
//        QPoint a = Path[i],b = Path[i+1];
//        painter.drawLine(a,b);
//    }
////    for(int i = 0;i<barrier.size();i++){
////        painter.drawRect(barrier[i].x()-8,barrier[i].y()-8,16,16);
////    }
////    painter.setBrush(Qt::blue);
////    for(QPoint i : Point){
////        double x = i.x();
////        double y = i.y();
////        painter.drawEllipse(QPoint(x,y),2,2);
////    }
        for(QPoint i : vis){
            painter.drawEllipse(i,2,2);
        }
}
void NewWindow::toPoint(){
    Point.clear();
    int n = 7;
    Point.push_back({begin.x(),begin.y()});
    Point.push_back({end.x(),end.y()});
    for(QPoint i : barrier){
        double x = i.x();
        double y = i.y();
       // qDebug()<<x<<" "<<y<<" "<<'\n';
        for(int j = 0;j<n;j++){
            double x1 = 30*cos(j*2*M_PI/n)+x;
            double y1 = 30*sin(j*2*M_PI/n)+y;
            Point.push_back(QPoint(x1,y1));
            //qDebug()<<x1<<" "<<y1<<" "<<'\n';
        }
    }
}
void NewWindow::on_pushButton_up_clicked(){
    cary -= 5;
}
void NewWindow::on_pushButton_down_clicked(){
    cary += 5;
}
void NewWindow::on_pushButton_left_clicked(){
    carx -= 5;
}
void NewWindow::on_pushButton_right_clicked(){
    carx += 5;
}
void NewWindow::on_pushButton_pause_clicked(){
    time_state = 0;
}
void NewWindow::on_pushButton_continue_clicked(){
    time_state = 1;
}
