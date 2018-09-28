#include "inputwindow.h"

InputWindow::InputWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);
    setWindowTitle(tr("装置指令输入"));
    QGridLayout *grid = new QGridLayout(this);
    m_nameCom = new QComboBox();

    m_indexSpi = new QSpinBox();

    m_nameLabel = new  QLabel();
    m_indexLabel = new  QLabel();
    m_indexLabel->setStyleSheet("color: red;");
    m_textEdit = new QTextEdit();
    m_textEdit->setFixedHeight(60);
    m_yesBtn = new QPushButton(tr("确定"));
    m_cancelBtn = new QPushButton(tr("取消"));
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_yesBtn);
    hbox->addWidget(m_cancelBtn);
    grid->addWidget(new QLabel(tr("装置名称：")), 0, 0);
    grid->addWidget(new QLabel(tr("装置编号：")), 1, 0);
    grid->addWidget(m_nameCom, 0, 1);
    grid->addWidget(m_indexSpi, 1, 1);

    grid->addWidget(m_nameLabel, 0, 2);
    grid->addWidget(m_indexLabel, 1, 2);

    grid->addWidget(new QLabel(tr("说明：")), 2, 0);
    grid->addWidget(m_textEdit, 3, 0, 1, 3);

    grid->addLayout(hbox, 4, 0, 2, 3);

    connect(m_nameCom, SIGNAL(currentIndexChanged(int)), this, SLOT(on_m_nameCom_triggered(int)));
    connect(m_yesBtn, SIGNAL(clicked()), this, SLOT(on_yesBtn_triggered()));
    connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(on_cancelBtn_triggered()));

    //快捷键
    QShortcut *key_Return=new QShortcut(QKeySequence(Qt::Key_Return), this);    //创建一个快捷键"Key_Return"键
    connect(key_Return, SIGNAL(activated()), m_yesBtn, SIGNAL(clicked()));    //连接到指定槽函数
    QShortcut *key_Enter=new QShortcut(QKeySequence(Qt::Key_Enter), this);    //创建一个快捷键"Key_Return"键
    connect(key_Enter, SIGNAL(activated()), m_yesBtn, SIGNAL(clicked()));    //连接到指定槽函数

    QShortcut *key_Esc=new QShortcut(QKeySequence(Qt::Key_Escape), this);    //创建一个快捷键"Key_Return"键
    connect(key_Esc, SIGNAL(activated()), m_cancelBtn, SIGNAL(clicked()));    //连接到指定槽函数


    m_nameCom->addItems(QStringList()<<"X"<<"Y"<<"M"<<"S"<<"T"<<"C");
}

void InputWindow::SetCurrentName(int type)
{
    m_type = type;
    switch (m_type) {
    case 0:
        //设置图标
        setWindowIcon(QIcon(":/images/graph/Xopen.png"));
        m_nameCom->setCurrentIndex(0);
        break;
    case 1:
        setWindowIcon(QIcon(":/images/graph/Xclose.png"));
        m_nameCom->setCurrentIndex(0);
        break;
    default:
        break;
    }

}


void InputWindow::on_m_nameCom_triggered(int idx)
{
    m_nameLabel->setText(tr("输入继电器"));
    m_indexLabel->setText(tr("范围为0~127"));
}


void InputWindow::on_yesBtn_triggered()
{
    emit sig_inputPara(m_nameCom->currentText(),
                       m_indexSpi->value(),
                       m_textEdit->toPlainText(),
                       m_type);
    close();
    m_indexSpi->setValue(0);
}

void InputWindow::on_cancelBtn_triggered()
{
    close();
    m_indexSpi->setValue(0);
}
