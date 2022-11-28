#include "map.h"
#include "ui_map.h"
#include <QPair>
#include<QWidget>
#include <QPainter>
#include <QDebug>
#include<mainwindow.h>
Map::Map(QWidget *parent,int height,int weight,QVector <QPair<QPoint,QPoint>> barrier,QPoint begin,QPoint end) :
    QWidget(parent),
    ui(new Ui::Map)
{
    ui->setupUi(this);
    resize(height,weight);
    setWindowTitle("test");
    this->begin = begin;
    this->end = end;
    this->height = height;
    this->weight = weight;
    this->barrier = barrier;
    this->big_total_row = height / BIG_Map_SIZE;
    this->big_total_col = weight / BIG_Map_SIZE;
    this->mid_total_row = height / MID_Map_SIZE;
    this->mid_total_col = weight / MID_Map_SIZE;
    this->small_total_row = height / SMALL_Map_SIZE;
    this->small_total_col = weight / SMALL_Map_SIZE;
    this->basic_total_row = height / BASIC_Map_SIZE;
    this->basic_total_col = weight / BASIC_Map_SIZE;
    build();
    to_Point();
    build_edge();
    update();
}
Map::~Map()
{
    delete ui;
}
void Map::build(){
    // 遍历所有的32*32的大格子，如果四叉树的四个孩子都没被占用则结束递归，否则递归找到不能分裂为止
    barrier_to_Map();
    for(int i = 0; i<=big_total_row;i++){
        for(int j = 0;j<=big_total_col;j++){
            big_occupy[{i,j}] = split(i,j,1,i,j);
        }
    }
}
void Map::to_Point(){
    for(int i = 0; i<=big_total_row;i++){
        for(int j = 0;j<=big_total_col;j++){
             for(QPair<QPoint,int> d : Point[i][j]){
                 v.push_back(get_center(d.first.x(),d.first.y(),d.second));
                 to_node[{d}] = tot_node++;
             }
        }
    }
    v.push_back({begin.x()+8,begin.y()+8});
    to_node[{{{begin},3}}] = tot_node++;
    v.push_back({end.x()+8,end.y()+8});
    to_node[{{{end},3}}] = tot_node++;
}
void Map::build_edge(){
    QPair<int,int> b1 =get_id(begin.x(),begin.y(),1,1);
    QPair<int,int> e1 =get_id(end.x(),end.y(),1,1);
    map[to_node[{{{begin},3}}]].push_back(to_node[{{{b1.first,b1.second},1}}]);
    map[to_node[{{{end},3}}]].push_back(to_node[{{{e1.first,e1.second},1}}]);
    map[to_node[{{{b1.first,b1.second},1}}]].push_back(to_node[{{{begin},3}}]);
    map[to_node[{{{e1.first,e1.second},1}}]].push_back(to_node[{{{end},3}}]);
    for(int i = 0; i<=big_total_row;i++){

        for(int j = 0;j<=big_total_col;j++){
            for(QPair<QPoint,int> d : Point[i][j]){
                int X = d.first.x(), Y = d.first.y();
                int sz = size[d.second];
                if(d.second==4&&basic_occupy[{X,Y}]) continue;
                QPoint p1 = get_xy(X,Y,d.second);
                QPoint p2 = {p1.x() + sz, p1.y() + sz};
                for(int k = 0;k<9;k++){
                    int x = i + dir[k][0];
                    int y = j + dir[k][1];
                    if(x>=0&&y>=0&&x<=big_total_row&&y<=big_total_col) {
                        for(QPair<QPoint,int> z : Point[x][y]){
                             int px = z.first.x(),py = z.first.y();
                             int sz1 = size[z.second];
                             if(z.second==4&&basic_occupy[{px,py}]) continue;
                             QPoint p3 = get_xy(px,py,z.second);
                             QPoint p4 = {p3.x()+sz1,p3.y()+sz1};
                             if(check_neighbor(p1,p2,p3,p4)){
                                 map[to_node[{d}]].push_back(to_node[{z}]);
                                 map[to_node[{z}]].push_back(to_node[{d}]);
                             }
                        }
                    }
                }
            }
        }
    }
}
bool Map::split(int idx,int idy,int type,int ori_i,int ori_j){ // 表示是否需要分裂
    if(type == 4) {
        Point[ori_i][ori_j].push_back({{idx,idy},type});
        return basic_occupy[{idx,idy}]; // 底层
    }
    bool need_split = 0;
    QPoint point[4];
    for(int i = 1;i<=4;i++){
        point[i] = get_smaller_id(idx,idy,i);
        if(split(point[i].x(),point[i].y(),type+1,ori_i,ori_j) == 1) need_split = 1;
    }
    if(need_split == 0){
        Point[ori_i][ori_j].push_back({{idx,idy},type});
        for(int j = 1;j<=4;j++){
            int ix = 0;
            point[j] = get_smaller_id(idx,idy,j);
        for(QPair<QPoint,int> i : Point[ori_i][ori_j]){
            QPair<QPoint,int> t = {point[j],type+1};
            if(i == t) {
                Point[ori_i][ori_j].erase(Point[ori_i][ori_j].begin()+ix);
                break;
            }
            ix++;
        }
        }
    }
    return need_split;
}
QPoint Map::get_xy(int idx,int idy,int type){
    int size ;
    int m ;
    if(type == 1) size = BIG_Map_SIZE,m = big_total_col;
    else if(type == 2) size = MID_Map_SIZE,m = mid_total_col;
    else if(type == 3) size = SMALL_Map_SIZE,m = small_total_col;
    else size = BASIC_Map_SIZE,m = basic_total_col;
    return {(idx)*size,idy*size};
}
QPoint Map::get_smaller_id(int idx,int idy,int kind){
    if(kind == 1) {
        return {idx*2,idy*2};
    }// 左上
    else if(kind == 2){
        return {idx*2+1,idy*2};
    }// 右上
    else if(kind == 3){
        return {idx*2,idy*2+1};
    }//左下
    else
       return {idx*2+1,idy*2+1};
    // 右下
}
QPair<int,int> Map:: get_id(int x,int y,int type,int kind){
    int size ;
    if(type == 1) size = BIG_Map_SIZE;
    else if(type == 2) size = MID_Map_SIZE;
    else if(type == 3) size = SMALL_Map_SIZE;
    else  size = BASIC_Map_SIZE;
    if( kind ==1 )return {x/size,y/size};
    else if(kind == 2)return {(x+size-1)/size,(y+size-1)/size};
    else if(kind ==3 )return {(x+size-1)/size,y/size};
    else return {x/size,(y+size-1)/size};
}
void Map::barrier_to_Map(){
    for(QPair p : barrier){
        QPoint a = p.first, b = p.second;
        for(int x = a.x() ,y = a.y(); x<=b.x();x++){
            basic_occupy[get_id(x,y,4,1)] = 1;
            //qDebug() <<x<<" "<<y<<" "<< get_xy(get_id(x,y,4).first,get_id(x,y,4).second,4) << '\n';
        }
        for(int x = a.x() ,y = b.y(); x<=b.x();x++){
            basic_occupy[get_id(x,y,4,2)] = 1;
        }
        for(int x = a.x() ,y = a.y(); y<=b.y();y++){
            basic_occupy[get_id(x,y,4,1)] = 1;
        }
        for(int x = b.x() ,y = a.y(); y<=b.y();y++){
            basic_occupy[get_id(x,y,4,2)] = 1;
        }
         for(int x = a.x() ; x<=b.x();x++){
              for(int y = a.y(); y<=b.y();y++){
                   basic_occupy[get_id(x,y,4,1)] = 1;
                   basic_occupy[get_id(x,y,4,2)] = 1;
                   basic_occupy[get_id(x,y,4,3)] = 1;
                   basic_occupy[get_id(x,y,4,4)] = 1;
              }
         }
    }
}
bool Map::check_neighbor(QPoint a,QPoint a1,QPoint b,QPoint b1){
    if(a.x()>=b.x()&&a.x()<=b1.x()&&a.y()>=b.y()&&a.y()<=b1.y()) return 1;
    if(a1.x()>=b.x()&&a.x()<=b1.x()&&a1.y()>=b.y()&&a.y()<=b1.y()) return 1;
    if(a.x()>=b.x()&&a.x()<=b1.x()&&a1.y()>=b.y()&&a.y()<=b1.y()) return 1;
    if(a1.x()>=b.x()&&a.x()<=b1.x()&&a.y()>=b.y()&&a.y()<=b1.y()) return 1;
    return 0;
}
QPoint Map::get_center(int idx,int idy,int type){
    int size ;
    int m ;
    if(type == 1) size = BIG_Map_SIZE,m = big_total_col;
    else if(type == 2) size = MID_Map_SIZE,m = mid_total_col;
    else if(type == 3) size = SMALL_Map_SIZE,m = small_total_col;
    else size = BASIC_Map_SIZE,m = basic_total_col;
    return {(idx)*size+size/2,idy*size+size/2};
}
void Map::paintEvent(QPaintEvent *){ // 测试格子是不是正确的
    QPainter painter(this);
    for(int i = 0; i<=big_total_row;i++){
        for(int j = 0;j<=big_total_col;j++){
            for(QPair<QPoint,int> k : Point[i][j]){
                QPoint point = get_xy(k.first.x(),k.first.y(),k.second);
                int sz = size[k.second];
                painter.drawRect(point.x(),point.y(),sz,sz);
            }
        }
    }
    painter.setBrush(Qt::red);
    for(QPair<QPoint,QPoint> i : barrier){
       painter.drawRect(i.first.x(),i.first.y(),24,24);
        qDebug() << i.first.x()<<" "<<i.first.y() <<'\n';
    }
    painter.drawRect(begin.x(),begin.y(),16,16);
    painter.drawRect(end.x(),end.y(),16,16);
    painter.setPen(Qt::green);
    QPoint beg = get_xy(begin.x()+8,begin.y()+8,3);
    QPoint ed = get_xy(end.x()+8,end.y()+8,3);
    QPair<int,int> b1 =get_id(begin.x(),begin.y(),1,1);
    QPair<int,int> e1 =get_id(end.x(),end.y(),1,1);
    QPoint t = v[to_node[{{{b1.first,b1.second},1}}]];
    QPoint tt = v[to_node[{{{e1.first,e1.second},1}}]];
    painter.setPen(Qt::black);
    painter.drawLine({begin.x()+8,begin.y()+8},t);
    painter.drawLine({end.x()+8,end.y()+8},tt);
    for(int i = 0; i<=big_total_row;i++){
        for(int j = 0;j<=big_total_col;j++){
            for(QPair<QPoint,int> d : Point[i][j]){
                int X = d.first.x(), Y = d.first.y();
                int sz = size[d.second];
                if(d.second==4&&basic_occupy[{X,Y}]) continue;
                painter.drawEllipse(get_center(X,Y,d.second),1,1);
                QPoint p1 = get_xy(X,Y,d.second);
                QPoint p2 = {p1.x() + sz, p1.y() + sz};
                for(int k = 0;k<9;k++){
                    int x = i + dir[k][0];
                    int y = j + dir[k][1];
                    if(x>=0&&y>=0&&x<=big_total_row&&y<=big_total_col) {
                        for(QPair<QPoint,int> z : Point[x][y]){
                             int px = z.first.x(),py = z.first.y();
                             int sz1 = size[z.second];
                             if(z.second==4&&basic_occupy[{px,py}]) continue;
                             QPoint p3 = get_xy(px,py,z.second);
                             QPoint p4 = {p3.x()+sz1,p3.y()+sz1};
                             if(check_neighbor(p1,p2,p3,p4)){
                             // painter.drawLine(get_center(X,Y,d.second),get_center(px,py,z.second));
                                // qDebug() << get_xy(X,Y,d.second) << " " <<get_xy(px,py,z.second)<<'\n';
                             }
                        }
                    }
                }
            }
        }
    }
}
