#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H
/**
Copyright (c) 2018 PLT. All Rights Reserved.
    *@file：
    *@brief： 输入装置窗口
    *@author ：李奎
    *@date ：2018/10/01
    *@description：
*/
#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QShortcut>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

#include "common.h"
class InputGraphWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InputGraphWindow(QWidget *parent = 0);
    void SetCurrentName(int index);
private:
    void InitUi();
    bool ReadInputDeviceInfo();
signals:
    void sig_inputPara(Element emt);
private slots:
    void on_m_nameCom_triggered(QString idx);
    void on_yesBtn_triggered();
    void on_cancelBtn_triggered();
private:
    int m_type;
    QMap<QString, QStringList> m_deviceInfo;
    QComboBox *m_nameCom;
    QSpinBox *m_indexSpi;
    QLabel *m_nameLabel;
    QLabel *m_indexLabel;
    QTextEdit *m_textEdit;
    QPushButton *m_yesBtn;
    QPushButton *m_cancelBtn;
};
class InputInstsWindow : public QFrame
{
    Q_OBJECT
public:
    explicit InputInstsWindow(QWidget *parent = 0);
private:
    void InitUi();
    int InstsDecoder();
protected:
    void showEvent(QShowEvent *event);
signals:
    void sig_inputPara(Element emt);
private slots:
    void on_okBtn_triggerred();
    void on_cancelBtn_triggerred();
private:
    QLineEdit *lineEdit;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

};
#endif // INPUTWINDOW_H
