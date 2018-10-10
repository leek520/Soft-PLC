#ifndef INSTSWINDOW_H
#define INSTSWINDOW_H

#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include "common.h"
class InstsWindow : public QTableWidget
{
    Q_OBJECT
public:
    explicit InstsWindow(QWidget *parent = 0);

signals:

public slots:

};

#endif // INSTSWINDOW_H
