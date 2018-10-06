#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Soft PLC"));//设置窗口标题
    setWindowIcon(QIcon(":/mamtool.ico"));
    //resize(SCREEN_WIDTH,SCREEN_HEIGHT);
    setWindowState(Qt::WindowMaximized);
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    SetupUi();
    SetupMdiArea();

    m_inputW = new InputWindow();
    connect(this, SIGNAL(sig_inputPara(QString,int,QString,int)),
            m_graphWid,SLOT(slt_inputPara(QString,int,QString,int)));
    connect(m_inputW, SIGNAL(sig_inputPara(QString,int,QString,int)),
            m_graphWid,SLOT(slt_inputPara(QString,int,QString,int)));
    connect(m_graphWid->m_graphTable, SIGNAL(sig_InsertBottomRowText(QString)),
            this, SLOT(slt_InsertBottomRowText(QString)));
}

MainWindow::~MainWindow()
{

}
void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);

    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("撤销(Ctrl+Z)"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(QIcon(":/images/redo.png"), tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("恢复(Ctrl+Y)"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    removeAct = new QAction(QIcon(":/images/remove.png"), tr("&Remove"), this);
    removeAct->setShortcuts(QKeySequence::Delete);
    removeAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(removeAct, SIGNAL(triggered()), this, SLOT(remove()));


    aboutAct = new QAction(QIcon(":/images/about.png"), tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));



    QAction *act = new QAction(QIcon(":/images/graph/Btn0.bmp"), tr("常开开关"), this);
    act->setStatusTip(tr("常开开关"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn1.bmp"), tr("常闭开关"), this);
    act->setStatusTip(tr("常闭开关"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn2.bmp"), tr("正缘触发开关"), this);
    act->setStatusTip(tr("正缘触发开关"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn3.bmp"), tr("负缘触发开关"), this);
    act->setStatusTip(tr("负缘触发开关"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn4.bmp"), tr("步进接点"), this);
    act->setStatusTip(tr("步进接点"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn5.bmp"), tr("输出接点"), this);
    act->setStatusTip(tr("输出接点"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn6.bmp"), tr("水平线"), this);
    act->setStatusTip(tr("水平线"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn7.bmp"), tr("垂直线"), this);
    act->setStatusTip(tr("垂直线"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn8.bmp"), tr("反向逻辑"), this);
    act->setStatusTip(tr("反向逻辑"));
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);


    buildAct = new QAction(QIcon(":/images/build.png"), tr("编译"), this);
    buildAct->setStatusTip(tr("编译"));
    connect(buildAct, SIGNAL(triggered()), this, SLOT(buildGraph()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);

    fileMenu->addAction(openAct);

    fileMenu->addAction(saveAct);

    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(removeAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);

    fileToolBar->addAction(openAct);

    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addAction(removeAct);

    buildToolBar = addToolBar(tr("Build"));
    buildToolBar->addAction(buildAct);


    graphToolBar = addToolBar(tr("Graph"));
    for(int i=0;i<m_graphActList.count();i++){
        graphToolBar->addAction(m_graphActList[i]);
    }



}
void MainWindow::createStatusBar()
{

}
void MainWindow::SetupUi()
{

    m_mdiArea = new QMdiArea(this);
    setCentralWidget(m_mdiArea);

    m_leftW = new ListWidget();
    m_dockW = new QDockWidget(tr(""), this);
    m_dockW->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);//可移动
    m_dockW->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_dockW->setWidget(m_leftW);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_dockW);//初始位置

    m_bottomW = new TextEdit();
    m_dockW = new QDockWidget(tr(""), this);
    m_dockW->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);//可移动可关闭
    m_dockW->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);
    m_dockW->setWidget(m_bottomW);
    this->addDockWidget(Qt::BottomDockWidgetArea, m_dockW);//初始位置

}

void MainWindow::SetupMdiArea()
{
    m_graphWid = new GraphWindow;
    QMdiSubWindow *child = m_mdiArea->addSubWindow(m_graphWid);

    child->resize(700,400);

    child->show();
    m_graphWid->setWindowState(Qt::WindowMaximized);



}

void MainWindow::newFile()
{
    SetupMdiArea();
}

bool MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("打开文件"),
                                                    "",
                                                    tr("梯形图文件(*.tgf);;All files(*.*)"));
    if (filename.isEmpty()) return false;

    m_graphWid->OpenGraph(filename);
}

bool MainWindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("保存文件"),
                                                    "",
                                                    tr("保存为 (*.tgf)"));
    if (filename.isEmpty()) return false;

    m_graphWid->SaveGraph(filename);
}

bool MainWindow::saveAs()
{

}

void MainWindow::about()
{

}

void MainWindow::redo()
{
    m_graphWid->m_graphTable->redo();
}

void MainWindow::undo()
{
    m_graphWid->m_graphTable->undo();
}

void MainWindow::copy()
{
    m_graphWid->m_graphTable->copy();
}

void MainWindow::paste()
{
    m_graphWid->m_graphTable->paste();
}

void MainWindow::cut()
{
    m_graphWid->m_graphTable->cut();
}

void MainWindow::remove()
{
    m_graphWid->m_graphTable->remove();
}

void MainWindow::drawGraph()
{
    int index = m_graphActList.indexOf((QAction *)sender());
    if (index < 0) return;

    if (index < 6){
        m_inputW->SetCurrentName(index);
        m_inputW->show();
    }else{
        switch (index) {
        case 6:
            emit sig_inputPara("", 0, "", HorizontalLine);
            break;
        case 7:
            emit sig_inputPara("", 0, "", verticalLine1);
            break;
        case 8:
            emit sig_inputPara("", 0, "", ReverseLogic);
            break;
        default:
            break;
        }

    }


}

void MainWindow::buildGraph()
{
    m_bottomW->clear();
    m_graphWid->BuildGraph();
}

void MainWindow::slt_InsertBottomRowText(QString text)
{
    m_bottomW->insertPlainText(text);
}
