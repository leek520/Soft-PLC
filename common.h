#ifndef COMMON_H
#define COMMON_H

#include <QString>
#define UNIT_WIDTH      80
#define UNIT_HEIGH      60
#define UNIT_W_FACTOR   3
#define UNIT_H_FACTOR   4

#define LINE_WIDTH      2
/*****************************
 * 图元数据结构：梯形图梯级的存储结构采用两个层次的双向链表结构
*****************************/
typedef struct _tElement
{
    int row;
    int col;
    int graphType;      //类型
    int funInsType;     //功能指令
    QString name;
    int index;
    bool upFlag;
    bool dnFlag;
}Element;

enum GraphType
{
    VerticalLine,
    HorizontalLine,

    InputOpen = 10,
    InputClose,
};
#endif // COMMON_H
