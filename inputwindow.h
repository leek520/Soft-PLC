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
class InputWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InputWindow(QWidget *parent = 0);
    void SetCurrentName(int type);
private:
    void InitUi();
signals:
    void sig_inputPara(QString name, int index, QString mark, int type);
private slots:
    void on_m_nameCom_triggered(int idx);
    void on_yesBtn_triggered();
    void on_cancelBtn_triggered();
private:
    int m_type;
    QComboBox *m_nameCom;
    QSpinBox *m_indexSpi;
    QLabel *m_nameLabel;
    QLabel *m_indexLabel;
    QTextEdit *m_textEdit;
    QPushButton *m_yesBtn;
    QPushButton *m_cancelBtn;
};

#endif // INPUTWINDOW_H
