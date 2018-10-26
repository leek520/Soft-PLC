#include "graphwindow.h"

GraphWindow::GraphWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("梯形图模式");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(1);
    m_graphView = new GraphView();
    layout->addWidget(m_graphView);

    m_graphView->initTable();

}


void GraphWindow::OpenGraph(QString name)
{
//    QFile fileRead(name);
//    fileRead.open(QIODevice::ReadOnly);
//    QDataStream  readDataStream(&fileRead);
//    GraphFB gfb;
//    m_graphTable->InitTable();
//    while(!fileRead.atEnd()){
//        readDataStream  >> gfb;
//        m_graphTable->InsertGraph(gfb.emt);
//    }
//    fileRead.close();

//    m_graphTable->InitParament();
}
void GraphWindow::SaveGraph(QString name)
{
//    QFile fileWrite(name);
//    fileWrite.open(QIODevice::WriteOnly);
//    QDataStream  writeDataStream(&fileWrite);
//    GraphFB gfb;
//    for (int i=0;i<GM->getCount();i++){
//        GraphFB *graph = GM->getUnit(i);
//        gfb.emt = graph->emt;
//        gfb.entColor = graph->entColor;
//        gfb.conColor = graph->conColor;
//        writeDataStream << gfb;
//    }
//    fileWrite.close();
}







