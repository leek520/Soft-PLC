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

    m_inputGW = new InputGraphWindow();
    m_inputIW = new InputInstsWindow();
    connect(this, SIGNAL(sig_inputPara(Element)),
            m_graphWid->m_graphTable,SLOT(slt_inputPara(Element)));

    connect(m_inputGW, SIGNAL(sig_inputPara(Element)),
            m_graphWid->m_graphTable,SLOT(slt_inputPara(Element)));

    connect(m_inputIW, SIGNAL(sig_inputPara(Element)),
            m_graphWid->m_graphTable,SLOT(slt_inputPara(Element)));


    connect(m_graphWid->m_graphTable, SIGNAL(sig_InsertBottomRowText(QString)),
            this, SLOT(slt_InsertBottomRowText(QString)));

    connect(m_graphWid->m_graphTable, SIGNAL(sig_zoomin()),
            this, SLOT(zoomin()));

    connect(m_graphWid->m_graphTable, SIGNAL(sig_zoomout()),
            this, SLOT(zoomout()));

    connect(m_graphWid->m_graphTable, SIGNAL(sig_enableUndo(bool)),
            this, SLOT(slt_enableUndo(bool)));

    connect(m_graphWid->m_graphTable, SIGNAL(sig_enableRedo(bool)),
            this, SLOT(slt_enableRedo(bool)));

    connect(m_graphWid->m_graphTable, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(slt_doubleClickTable(QTableWidgetItem *)));

    connect(m_graphWid->m_graphTable, SIGNAL(sig_InsertInst(int,QString)),
            m_instsWid, SLOT(slt_InsertInst(int,QString)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    /****file****/
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

    /****edit****/
    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("撤销(Ctrl+Z)"));
    undoAct->setEnabled(false);
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(QIcon(":/images/redo.png"), tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("恢复(Ctrl+Y)"));
    redoAct->setEnabled(false);
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
    removeAct->setStatusTip(tr("Remove the current selection"));
    connect(removeAct, SIGNAL(triggered()), this, SLOT(remove()));

    /****view****/
    zoomInAct = new QAction(QIcon(":/images/zoomin.png"), tr("&ZoomIn"), this);
    zoomInAct->setStatusTip(tr("Zoom in"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomin()));

    zoomOutAct = new QAction(QIcon(":/images/zoomout.png"), tr("&ZoomOut"), this);
    zoomOutAct->setStatusTip(tr("Zoom out"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomout()));

    findAct = new QAction(QIcon(":/images/find.png"), tr("&Find"), this);
    findAct->setStatusTip(tr("find the graph"));
    connect(findAct, SIGNAL(triggered()), this, SLOT(find()));

    aboutAct = new QAction(QIcon(":/images/about.png"), tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));


    /****graph****/
    QAction *act = new QAction(QIcon(":/images/graph/Btn0.bmp"), tr("常开开关"), this);
    act->setStatusTip(tr("常开开关"));
    act->setShortcut(Qt::Key_F3);
    connect(act, SIGNAL(triggered()), this, SLOT(drawGraph()));
    m_graphActList.append(act);

    act = new QAction(QIcon(":/images/graph/Btn1.bmp"), tr("常闭开关"), this);
    act->setStatusTip(tr("常闭开关"));
    act->setShortcut(Qt::Key_F4);
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

    /****build****/
    buildAct = new QAction(QIcon(":/images/build.png"), tr("Build"), this);
    buildAct->setStatusTip(tr("Build"));
    connect(buildAct, SIGNAL(triggered()), this, SLOT(buildGraph()));

    runAct = new QAction(QIcon(":/images/run.png"), tr("Run"), this);
    runAct->setStatusTip(tr("Run"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(runGraph()));
    runAct->setCheckable(true);

    /****window****/
    wstackAct = new QAction(QIcon(":/images/wstack.png"), tr("WStack"), this);
    wstackAct->setStatusTip(tr("WStack"));
    connect(wstackAct, SIGNAL(triggered()), this, SLOT(reorderSubWindow()));

    whsideAct = new QAction(QIcon(":/images/whside.png"), tr("WHSide"), this);
    whsideAct->setStatusTip(tr("WHSide"));
    connect(whsideAct, SIGNAL(triggered()), this, SLOT(reorderSubWindow()));

    wvsideAct = new QAction(QIcon(":/images/wvside.png"), tr("WVSide"), this);
    wvsideAct->setStatusTip(tr("WVSide"));
    connect(wvsideAct, SIGNAL(triggered()), this, SLOT(reorderSubWindow()));
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

    buildMenu = menuBar()->addMenu(tr("&Build"));
    buildMenu->addAction(buildAct);
    buildMenu->addAction(runAct);


    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(findAct);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    windowMenu->addAction(wstackAct);
    windowMenu->addAction(whsideAct);
    windowMenu->addAction(wvsideAct);

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

    viewToolBar = addToolBar(tr("View"));
    viewToolBar->addAction(findAct);
    viewToolBar->addAction(zoomInAct);
    viewToolBar->addAction(zoomOutAct);

    windowToolBar = addToolBar(tr("Window"));
    windowToolBar->addAction(wstackAct);
    windowToolBar->addAction(whsideAct);
    windowToolBar->addAction(wvsideAct);

    buildToolBar = addToolBar(tr("Build"));
    buildToolBar->addAction(buildAct);
    buildToolBar->addAction(runAct);

    addToolBarBreak(Qt::TopToolBarArea);

    graphToolBar = addToolBar(tr("Graph"));
    graphToolBar->addWidget(new QLabel("装置"));
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

    m_graphWid = NULL;
    m_instsWid = NULL;

//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    //获取可用桌面大小
//    QRect deskRect = desktopWidget->availableGeometry();
//    // 如果分辨率已经在1920及以上，则不需要进行处理
//    if(deskRect.width() >= SCREEN_WIDTH)
//    {
//        resize(SCREEN_WIDTH, SCREEN_HEIGHT);
//        //居中显示
//        move((QApplication::desktop()->width() - width())/2,
//                (QApplication::desktop()->height() - height())/2-15);
//    }
//    else
//    {
//        resize(deskRect.width(), deskRect.height());
//        move(0, 0);
//    }
}

void MainWindow::SetupMdiArea()
{
    m_instsWid = new InstsWindow;
    QMdiSubWindow *instsChild = m_mdiArea->addSubWindow(m_instsWid);
    instsChild->setWindowIcon(QIcon(":/images/instswindow.png"));

    m_graphWid = new GraphWindow;
    QMdiSubWindow *graphChild = m_mdiArea->addSubWindow(m_graphWid);
    graphChild->setWindowIcon(QIcon(":/images/graphwindow.png"));
    //m_graphWid->setWindowState(Qt::WindowMaximized);

    m_mdiArea->setActiveSubWindow(graphChild);

    //m_mdiArea->tileSubWindows();   //SubWindows并列
    //m_mdiArea->cascadeSubWindows();   //SubWindows重叠排列

    //release下500可缩短为200
    QTimer::singleShot(500, this, SLOT(reorderSubWindow()));
}

void MainWindow::newFile()
{
    //提示是否要保存
    if (m_graphWid){
        int result = QMessageBox::question(
                    this, tr("QMessageBox::question()"),
                    "是否将变更的文件保存至...？",
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (result == QMessageBox::Yes){
            //保存
        }else if (result == QMessageBox::No){
            //不保存
        }else{
            return;
        }
    }
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
    if (this->focusWidget() != m_graphWid->m_graphTable) return;
    m_graphWid->m_graphTable->redo();
}

void MainWindow::undo()
{
    if (this->focusWidget() != m_graphWid->m_graphTable) return;
    m_graphWid->m_graphTable->undo();
}

void MainWindow::copy()
{
    if (this->focusWidget() != m_graphWid->m_graphTable) return;
    m_graphWid->m_graphTable->copy();
}

void MainWindow::paste()
{
    if (this->focusWidget() != m_graphWid->m_graphTable) return;
    m_graphWid->m_graphTable->paste();
}

void MainWindow::cut()
{
    m_graphWid->m_graphTable->cut();
}

void MainWindow::remove()
{
    if (this->focusWidget() != m_graphWid->m_graphTable) return;
    m_graphWid->m_graphTable->remove();
}

void MainWindow::zoomin()
{

    if (!GraphFB::zoom(true)) return;

    m_graphWid->m_graphTable->zoom();

    m_instsWid->setZoom(GraphFB::g_factor);
}

void MainWindow::zoomout()
{
    if (!GraphFB::zoom(false)) return;

    m_graphWid->m_graphTable->zoom();

    m_instsWid->setZoom(GraphFB::g_factor);
}

void MainWindow::find()
{
    m_graphWid->m_graphTable->find();
}

void MainWindow::drawGraph()
{
    Element emt;
    int index = m_graphActList.indexOf((QAction *)sender());
    if (index < 0) return;

    if (index < 6){
        m_inputGW->SetCurrentName(index);
        m_inputGW->show();
    }else{
        emt.name = "";
        emt.index = 0;
        emt.mark = "";

        switch (index) {
        case 6:
            emt.graphType = HorizontalLine;
            emit sig_inputPara(emt);
            break;
        case 7:
            emt.graphType = verticalLine;
            emit sig_inputPara(emt);
            break;
        case 8:
            emt.graphType = ReverseLogic;
            emit sig_inputPara(emt);
            break;
        default:
            break;
        }

    }


}

void MainWindow::buildGraph()
{
    m_bottomW->clear();
    m_instsWid->buildInsts();
    m_graphWid->m_graphTable->BuildGraph();
}

void MainWindow::runGraph()
{
    if (runAct->isChecked()){
        m_graphWid->m_graphTable->RunGraph(true);
    }else{
        m_graphWid->m_graphTable->RunGraph(false);
    }
}

void MainWindow::slt_enableUndo(bool status)
{
    undoAct->setEnabled(status);
}

void MainWindow::slt_enableRedo(bool status)
{
    redoAct->setEnabled(status);
}

void MainWindow::slt_doubleClickTable(QTableWidgetItem *item)
{
    m_inputIW->show();
}

void MainWindow::reorderSubWindow(int type)
{
    int width = m_mdiArea->width();
    int height = m_mdiArea->height();
    QList<QMdiSubWindow *> subwinList = m_mdiArea->subWindowList();

    if ((sender() == whsideAct) || (type == 0)){
        switch (subwinList.count()) {
        case 1:
            subwinList[0]->setGeometry(0,0,width,height);
            break;
        case 2:
            subwinList[1]->setGeometry(0,0,width*3/4,height);
            subwinList[0]->setGeometry(width*3/4,0,width/4,height);
            break;
        default:
            break;
        }
    }else if ((sender() == wvsideAct) || (type == 1)){
        switch (subwinList.count()) {
        case 1:
            subwinList[0]->setGeometry(0,0,width,height);
            break;
        case 2:
            subwinList[1]->setGeometry(0,0,width,height/2);
            subwinList[0]->setGeometry(0,height/2,width,height/2);
            break;
        default:
            break;
        }
    }else{  //    if (sender() == wstackAct)
        m_mdiArea->cascadeSubWindows();
        switch (subwinList.count()) {
        case 1:
            subwinList[0]->setGeometry(0,0,width*3/4,height*3/4);
            break;
        case 2:
            subwinList[0]->setGeometry(0,0,width*3/4,height*3/4);
            subwinList[1]->setGeometry(100,80,width*3/4,height*3/4);
            break;
        default:
            break;
        }

    }
}

void MainWindow::slt_InsertBottomRowText(QString text)
{
    m_bottomW->insertPlainText(text);
}
