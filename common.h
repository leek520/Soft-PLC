#ifndef COMMON_H
#define COMMON_H

#define UNIT_WIDTH      80
#define UNIT_HEIGH      60
#define LINE_WIDTH      10
/*****************************
 * 图元数据结构：梯形图梯级的存储结构采用两个层次的双向链表结构
*****************************/
typedef struct _tElement
{
    int row;
    int col;
    int graphType;      //类型
    int funInsType;     //功能指令
}Element;

enum GraphType
{
  VerticalLine,
  HorizontalLine,
  LogicX,
};
#endif // COMMON_H
