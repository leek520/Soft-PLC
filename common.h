#ifndef COMMON_H
#define COMMON_H

#include <QString>
/*********主窗体************/
#define SCREEN_WIDTH	1480						//屏幕宽
#define SCREEN_HEIGHT	960							//屏幕高

#define UNIT_WIDTH          80
#define UNIT_HEIGH          60
#define UNIT_ZOOM_FACTOR    1.2
#define UNIT_W_FACTOR       3
#define UNIT_H_FACTOR       4       //这里确保该因子能被对应长度整除
#define UNIT_INST_HEIGH     30
#define MAX_COL         12
#define MAX_ROW         1000
#define INIT_ROW        12

#define CalIdx(row, col) row * MAX_COL + col -1
/*****************************
 * 图元数据结构：梯形图梯级的存储结构采用两个层次的双向链表结构
*****************************/
typedef struct _tElement
{
    int row;
    int col;
    int width;
    int height;
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
    NoneGraph,
    NumLine,

    verticalLine,

    HorizontalLine,

    InputOpen = 10,
    InputClose,
    InputPedge,
    InputNedge,
    StepNode,
    OutputNode,
    ReverseLogic,
    EndGraph,
};
enum OptType{Insert, Remove, CutPaste, CopyPaste};



#endif // COMMON_H
