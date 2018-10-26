#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QColor>
#include <QDebug>

#define SOFT_NAME    "Soft PLC"
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
    StepGraph,
    OutputGraph,
    ReverseLogic,
    LogicGraph,
    EndGraph,
};
/*****************************
 * 图元数据结构：梯形图梯级的存储结构采用两个层次的双向链表结构
*****************************/
struct Element
{
    int row;
    int col;
    int type;      //类型
    int index;
    bool upFlag;
    bool dnFlag;
    QString name;
    QString mark;
    int conRGB;
    int entRGB;
    int backRGB;
    int textRGB;
    Element(){
        row = 0;
        col = 0;
        type = 0;
        index = 0;
        dnFlag = false;
        upFlag = false;
        name = "";
        mark = "";
        conRGB = 0x000000;
        entRGB = 0xffff00;
        backRGB = 0xffffff;
        textRGB = 0x000000;
    }
    Element(const int r, const int c, const int tp=NoneGraph){
        row = r;
        col = c;
        type = tp;
        index = 0;
        dnFlag = false;
        upFlag = false;
        name = "";
        mark = "";
        conRGB = 0x000000;
        entRGB = 0x000000;
        backRGB = 0xffffff;
        textRGB = 0x000000;
    }
    Element(const int r, const int c,
            int tp, int idx,
            const QString nameStr, const QString maskStr,
            const bool up=false, const bool dn=false){
        row = r;
        col = c;
        type = tp;
        index = idx;
        dnFlag = up;
        upFlag = dn;
        name = nameStr;
        mark = maskStr;
        conRGB = 0x000000;
        entRGB = 0x000000;
        backRGB = 0xffffff;
        textRGB = 0x000000;
    }

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
