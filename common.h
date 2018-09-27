#ifndef COMMON_H
#define COMMON_H
#include <QPoint>
#include <QString>

#define UNIT_WIDTH      80
#define UNIT_HEIGH      60
/*****************************
 * 图元数据结构：梯形图梯级的存储结构采用两个层次的双向链表结构
*****************************/
typedef struct Element
{
    QPoint point;       //图元所在坐标
    QString address;    //对应地址
    int GraphType;      //类型
    int FunInsType;     //功能指令

    Element *PreElem;   //前一个图元指针
    Element *NextElem;  //后一个图元
}Element;
//Element *LineNode;
/*****************************
 * 行头结点：为了方便连接行与行而定义的抽象结点，
 * 并不是实际存在的图元
*****************************/
typedef struct HeadNode
{
    QPoint point;       //行所在的位置
    HeadNode *up;       //上面的行头结点指针
    HeadNode *down;     //下面的行头结点指针

    Element *next;      //其后的第一个图元指针
}HeadNode;
//HeadNode *HeadList;
#endif // COMMON_H
