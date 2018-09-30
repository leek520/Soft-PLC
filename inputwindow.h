#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QShortcut>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

#include "common.h"
class InputWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InputWindow(QWidget *parent = 0);
    void SetCurrentName(int index);
private:
    void InitUi();
    bool ReadInputDeviceInfo();
signals:
    void sig_inputPara(QString name, int index, QString mark, int type);
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

#endif // INPUTWINDOW_H
