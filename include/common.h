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
#define MAX_COL         14
#define MAX_ROW         1000
#define INIT_ROW        14

#define CalIdx(row, col) (row) * MAX_COL + (col) -1
#define GraIdx(row, col) (row) * MAX_COL + (col)
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

    //加线条在该区域
    CompGraph,


    InputOpen = 10,
    InputClose,
    InputPedge,
    InputNedge,
    StepNode,
    OutputNode,
    ReverseLogic,
    LogicGraph,
    EndGraph,
};
enum Direction{
    TurnStart,
    TurnDown,
    TurnUp,
    TurnLeft,
    TurnRight,
};
enum OptType{RedoGraphInsert=0, UndoGraphInsert,    //插入删除图形
             RedoVLineInsert, UndoVLineInsert,      //插入删除竖直线
             RedoRowInsert, UndoRowInsert,          //插入删除行
             RedoGraphDelete, UndoGraphDelete,      //删除添加图形（delete键）
             RedoCutPaste, UndoCutPaste,           //剪贴
             RedoCopyPaste, UndoCopyPaste,         //复制
             RedoDelete, UndoDelete,
             RedoDeleteVLine, UndoDeleteVLine};              //删除



#endif // COMMON_H
