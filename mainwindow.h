#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "common.h"
#include "graphwindow.h"
#include "inputwindow.h"
#include <QMainWindow>
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
    void sig_inputPara(QString name, int index, QString mark, int type);
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

    void drawGraph();

    void buildGraph();

    void slt_InsertBottomRowText(QString text);
private :
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *graphToolBar;
    QToolBar *buildToolBar;

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

    QAction *aboutAct;

    QList <QAction *>m_graphActList;
    QAction *buildAct;


    QDockWidget *m_dockW;
    ListWidget *m_leftW;
    QTextEdit *m_bottomW;

    QMdiArea *m_mdiArea;

    GraphWindow *m_graphWid;
    QWidget *m_cmdWid;
    InputWindow *m_inputW;

};
class ListWidget : public QListWidget
{
public:
    ListWidget(QWidget *parent = 0) {}
    QSize sizeHint() const {
        return QSize(200, 500);
    }
};
class TextEdit : public QTextEdit
{
public:
    TextEdit(QWidget *parent = 0) {}
    QSize sizeHint() const {
        return QSize(200, 150);
    }
};
#endif // MAINWINDOW_H
