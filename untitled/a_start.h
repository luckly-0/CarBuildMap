#ifndef A_START_H
#define A_START_H
#include<QPair>
#include<QVector>

#include<QPoint>
#include <QPainter>
#include <complex>
#include <math.h>
#include <QDialog>
#include <QMainWindow>
// 传入点集实现A_start和CBS传出path；
class A_start : public QDialog
{
public:

    A_start( QPoint begin={},
             QVector<QPoint > barrier={},
             QPoint end={},
             QVector<QPoint > Point={},
             QVector <int> map[20000]={});
    QPoint begin,end;
    QVector<QPoint > barrier,Point;
    QVector<QPoint > A_start_slove(int begin_index,int end_index);
    QVector<int> map[20000];
    void build_map();
    void paintEvent(QPaintEvent *);
    double PointToSegDist(double x, double y, double x1, double y1, double x2, double y2);

};

#endif // A_START_H
