#include "a_start.h"
#include <QDebug>
#include<complex>
#include <queue>
#include<QPair>
//#include <algorithm>
#define _USE_MATH_DEFINES
//
A_start::A_start( QPoint begin,
                  QVector<QPoint > barrier,
                  QPoint end,
                  QVector<QPoint > Point ,
                  QVector <int> map[20000]){
    this->begin = begin;
    this->end = end;
    this->barrier = barrier;
    this->Point = Point;
    for(int i = 0;i<20000;i++) this->map[i] = map[i];
    resize(1200,800);
    setWindowTitle("建图");

   // qDebug() << "成功运行";
   // build_map();
}
//
void A_start::build_map(){ // 判断两点之间是否存在障碍物
 //   qDebug() << "成功运行";
    for(int i = 0;i<Point.size();i++){
        for(int j = 0;j<Point.size();j++){
            if(i==j) continue;
            int ok = 1;
            double x1 = Point[i].x(),x2 = Point[j].x(),y1 = Point[i].y(),y2 = Point[j].y();
            for(QPoint k : barrier){
                double x0 = k.x(),y0 = k.y();
                double d = PointToSegDist(x0,y0,x1,y1,x2,y2);
              //  qDebug() << d << " "<<k<<Point[i]<<Point[j]<<'\n';
                if(d <= 10) {
                    ok = 0;
                    break;
                }
            }
            if(ok==0) continue;
            map[i].push_back(j);
           // qDebug() << i << " " << j<<'\n';
        }
    }
}
void A_start::paintEvent(QPaintEvent *){
   // qDebug() <<"A_start开始运行";
    QPainter painter(this);
    for(int i = 0;i<100;i++){
        for(int j : map[i]){
            painter.drawLine(Point[i],Point[j]);
        }
    }
}
QVector<QPoint > A_start::A_start_slove(int begin_index,int end_index){
    using namespace  std;
    QVector<QPoint> Path;
    vector<int> open(3000,0),close(3000,0),dist(3000,0x3f3f3f3f),H(3000,0x3f3f3f3f);
    QVector<QPoint> vis;
    priority_queue<pair<double,int>,vector<pair<double,int>>,std::greater<pair<double,int> > > q;
    vector<int> fa(3000,0);  //  记录第i个点父亲节点，用来回溯找路径
    q.push({0,begin_index});
    dist[begin_index] = 0;
    qDebug() << Point[begin_index] << begin_index<<Point[end_index]<<end_index<<'\n';
    while(!q.empty()){
        int u = q.top().second;
        if(u == end_index) break;
        close[u] = 1;
       // qDebug() << u << '\n';
        q.pop();
        for(int i : map[u]){
            if(close[i]) continue;
            double g = sqrt((Point[i].x()-Point[u].x())*(Point[i].x()-Point[u].x())+(Point[i].y()-Point[u].y())*(Point[i].y()-Point[u].y()));
            H[i] = sqrt((Point[i].x()-Point[end_index].x())*(Point[i].x()-Point[end_index].x())+(Point[i].y()-Point[end_index].y())*(Point[i].y()-Point[end_index].y()));
            double t = H[i];
           // qDebug() << "坐标1" << Point[i]<<" "<<i<<'\n';
            vis.push_back(Point[i]);
           // qDebug() <<open[i]<< " "<<dist[u] + H[u]<<" "<<dist[i] + g + H[i] <<'\n';
            if(!open[i]||dist[i] > dist[u] + g) {
                dist[i] = dist[u] + g;
                open[i] = 1;
                q.push({dist[i]+H[i],i});
                fa[i] = u;
            }
        }
    }
    int now = end_index;
    int k = dist[end_index];
    qDebug() << k << '\n';
    if(dist[end_index] < 0x3f3f3f3f) {
        while(1){
            Path.push_back(Point[now]);
            if(now == begin_index) break;
            now = fa[now];
        }
    }
    else qDebug()<< "找不到路"<<" ";
    QVector<QPoint> t(Path.rbegin(),Path.rend());
   // return vis;
    return t;
}
double A_start::PointToSegDist(double x, double y, double x1, double y1, double x2, double y2)
{
    double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);
    if (cross <= 0) return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));

    double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    if (cross >= d2) return sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
    double S = 0.5*abs((x1-x)*(y2-y)-(x2-x)*(y1-y))/(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
    return S;
    }
