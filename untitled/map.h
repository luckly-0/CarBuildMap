#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QMap>
#include <QPair>
#include "newwindow.h"
#include "ui_newwindow.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include<complex>
#include "math.h"
#include<mainwindow.h>
#include<a_start.h>
// 表示不同大小的格子
const int BIG_Map_SIZE = 32;
const int MID_Map_SIZE = 16;
const int SMALL_Map_SIZE = 8;
const int BASIC_Map_SIZE = 4;
namespace Ui {
class Map;
}

class Map : public QWidget
{
    Q_OBJECT
    struct node{
        QPair<QPoint,int> t;
        bool operator <(const node a)const {
            if(a.t.first.x() !=t.first.x())return t.first.x() < a.t.first.x();
            else if(t.first.y() != a.t.first.y())return t.first.y() < a.t.first.y();
            else return t.second < a.t.second;
        }
    };
public:
    Map(QWidget *parent = 0,int height=800,int weight=600,QVector <QPair<QPoint,QPoint>> barrier ={},QPoint begin={},QPoint end={});
    ~Map();
    QMap<node,int> to_node;

    QVector <int> map[20000];
    int tot_node = 0;
    int size[10] = {0,32,16,8,4};
    QPoint begin,end;
    int dir[9][2] = {1,0,0,1,-1,0,0,-1,1,1,-1,-1,1,-1,-1,1,0,0};
    QVector <QPair<QPoint,QPoint>> barrier; // 障碍物的中心和半径
    int height,weight;
    int big_total_row,big_total_col;
    int mid_total_row,mid_total_col;
    int small_total_row,small_total_col;
    int basic_total_row,basic_total_col;
    QPoint get_xy(int idx,int idy,int type);
    void barrier_to_Map();
    int cnt_edge = 0 ;
    QPoint get_smaller_id(int idx,int idy,int kind);
    QMap<QPair<int,int>,bool> basic_occupy,big_occupy;
    QPair<int,int> get_id(int x,int y,int type,int kind);
    QPoint get_center(int idx,int idy,int type);
    QVector<QPair<QPoint,int> > Point[2000][2000];
    void build_edge();
    bool check_neighbor(QPoint a,QPoint a1,QPoint b,QPoint b1);
    void build();
    bool split(int idx,int idy,int type,int ori_i,int ori_j); // 表示是否需要分裂
    void paintEvent(QPaintEvent *);
    QVector<QPoint> v;
    void to_Point();

private:
    Ui::Map *ui;
};
#endif // MAP_H
