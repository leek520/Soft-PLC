#ifndef COMMON_H
#define COMMON_H

#include <QString>
#define SCREEN_WIDTH    1200
#define SCREEN_HEIGHT   700
#define UNIT_WIDTH      80
#define UNIT_HEIGH      60
#define UNIT_W_FACTOR   3
#define UNIT_H_FACTOR   4

#define MAX_COL         12
#define MAX_ROW         1000
#define INIT_ROW        10
/*****************************
 * 图元数据结构：梯形图梯级的存储结构采用两个层次的双向链表结构
*****************************/
typedef struct _tElement
{
    int row;
    int col;
    int graphType;      //类型
    int funInsType;     //功能指令 
    int index;
    bool upFlag;
    bool dnFlag;
    QString name;
    QString mark;
}Element;

enum GraphType
{
    NumLine=1,
    verticalLine1,
    verticalLine2,

    HorizontalLine,

    InputOpen = 10,
    InputClose,
    InputPedge,
    InputNedge,
    StepNode,
    OutputNode,
    ReverseLogic,
};
enum OptType{Insert, Remove, CutPaste, CopyPaste};

#endif // COMMON_H
