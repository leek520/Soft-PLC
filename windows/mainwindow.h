#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
Copyright (c) 2018 PLT. All Rights Reserved.
    *@file：
    *@brief： 主界面窗口
    *@author ：李奎
    *@date ：2018/10/01
    *@description：
*/
#include "common.h"
#include "graphwindow.h"
#include "instswindow.h"
#include "inputwindow.h"
#include <QMainWindow>
#include <QDesktopWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QListWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDockWidget>
#include <QLabel>
#include <QComboBox>
#include <QTimer>
#include <QDir>
#include <QMutex>
#include <QDateTime>
#include <QFileDialog>
class ListWidget;
class TextEdit;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void SetupUi();
    void SetupMdiArea();
signals:
    void sig_inputPara(Element emt);
private slots:
    void newFile();
    bool open();
    bool save();
    bool saveAs();
    void about();

    void redo();
    void undo();
    void copy();
    void paste();
    void cut();
    void remove();

    void zoomin();
    void zoomout();
    void find();
    //type=0：默认打开水平排列，type=1：默认打开数值排列，type=其他值：默认打开层叠排列
    void reorderSubWindow();


    void drawGraph();

    void buildGraph();
    void runGraph();

    void slt_enableUndo(bool status);
    void slt_enableRedo(bool status);

    void slt_showInputWindow(QString text, bool isSelected);
    void slt_InsertBottomRowText(QString text);
private :
    QStatusBar *stateBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *buildMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;


    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *viewToolBar;
    QToolBar *graphToolBar;
    QToolBar *buildToolBar;
    QToolBar *windowToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    QAction *redoAct;
    QAction *undoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *removeAct;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *findAct;

    QAction *wstackAct;
    QAction *whsideAct;
    QAction *wvsideAct;

    QAction *aboutAct;

    QList <QAction *>m_graphActList;
    QAction *buildAct;
    QAction *runAct;

    QDockWidget *m_dockW;
    ListWidget *m_leftW;
    QTextEdit *m_bottomW;

    QMdiArea *m_mdiArea;

    GraphWindow *m_graphWid;
    InstsWindow *m_instsWid;

    InputGraphWindow *m_inputGW;
    InputInstsWindow *m_inputIW;

};
class ListWidget : public QListWidget
{
public:
    ListWidget(QWidget *parent = 0) : QListWidget(parent) {}
    QSize sizeHint() const {
        return QSize(100, 500);
    }
};
class TextEdit : public QTextEdit
{
public:
    TextEdit(QWidget *parent = 0) : QTextEdit(parent) {}
    QSize sizeHint() const {
        return QSize(200, 80);
    }
};
#endif // MAINWINDOW_H
