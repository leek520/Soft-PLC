#include "inputwindow.h"

InputWindow::InputWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);
    setWindowTitle(tr("装置指令输入"));
    resize(340,200);
    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(10);
    QLabel *namelbl = new QLabel(tr("装置名称："));
    namelbl->setFixedWidth(60);
    m_nameCom = new QComboBox();
    m_nameCom->setFixedWidth(50);
    m_indexSpi = new QSpinBox();
    m_indexSpi->setFixedWidth(50);
    m_indexSpi->setFixedHeight(40);
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
    grid->addWidget(namelbl, 0, 0);
    grid->addWidget(new QLabel(tr("装置编号：")), 1, 0);
    grid->addWidget(m_nameCom, 0, 1,1,1,Qt::AlignLeft);
    grid->addWidget(m_indexSpi, 1, 1);

    grid->addWidget(m_nameLabel, 0, 2);
    grid->addWidget(m_indexLabel, 1, 2);

    grid->addWidget(new QLabel(tr("说明：")), 2, 0);
    grid->addWidget(m_textEdit, 3, 0, 1, 3);

    grid->addLayout(hbox, 4, 0, 2, 3);

    connect(m_nameCom, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_m_nameCom_triggered(QString)));
    connect(m_yesBtn, SIGNAL(clicked()), this, SLOT(on_yesBtn_triggered()));
    connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(on_cancelBtn_triggered()));

    //快捷键
    QShortcut *key_Return=new QShortcut(QKeySequence(Qt::Key_Return), this);    //创建一个快捷键"Key_Return"键
    connect(key_Return, SIGNAL(activated()), m_yesBtn, SIGNAL(clicked()));    //连接到指定槽函数
    QShortcut *key_Enter=new QShortcut(QKeySequence(Qt::Key_Enter), this);    //创建一个快捷键"Key_Return"键
    connect(key_Enter, SIGNAL(activated()), m_yesBtn, SIGNAL(clicked()));    //连接到指定槽函数

    QShortcut *key_Esc=new QShortcut(QKeySequence(Qt::Key_Escape), this);    //创建一个快捷键"Key_Return"键
    connect(key_Esc, SIGNAL(activated()), m_cancelBtn, SIGNAL(clicked()));    //连接到指定槽函数

    ReadInputDeviceInfo();
}

void InputWindow::SetCurrentName(int index)
{

    switch (index) {
    case 0:
        //设置图标
        m_type = InputOpen;
        setWindowIcon(QIcon(":/images/graph/Btn0.bmp"));
        m_nameCom->setCurrentIndex(0);
        break;
    case 1:
        m_type = InputClose;
        setWindowIcon(QIcon(":/images/graph/Btn1.bmp"));
        m_nameCom->setCurrentIndex(0);
        break;
    case 2:
        m_type = InputPedge;
        setWindowIcon(QIcon(":/images/graph/Btn2.bmp"));
        m_nameCom->setCurrentIndex(0);
        break;
    case 3:
        m_type = InputNedge;
        setWindowIcon(QIcon(":/images/graph/Btn3.bmp"));
        m_nameCom->setCurrentIndex(0);
        break;
    case 4:
        m_type = StepNode;
        setWindowIcon(QIcon(":/images/graph/Btn4.bmp"));
        m_nameCom->setCurrentIndex(3);
        break;
    case 5:
        m_type = OutputNode;
        setWindowIcon(QIcon(":/images/graph/Btn5.bmp"));
        m_nameCom->setCurrentIndex(1);
        break;
    default:
        break;
    }

}

bool InputWindow::ReadInputDeviceInfo()
{
    if (!m_deviceInfo.isEmpty()) return true;

    QString errorStr;
    int errorLine;
    int errorCol;

    QDomDocument sdoc;

    QFile file(":/config/input_device_info.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    //setContent是将指定的内容指定给QDomDocument解析，
    //第一参数可以是QByteArray或者是文件名等
    if(!sdoc.setContent(&file, true, &errorStr, &errorLine, &errorCol))
    {
        file.close();
        qDebug() << errorStr << "line: " << errorLine << "col: " << errorCol;
        return false;
    }
    //读取输入设备列表
    QDomElement root = sdoc.documentElement();
    QDomNode deviceNode = root.firstChild();
    while (!deviceNode.isNull())
    {
        QDomElement deviceInfo = deviceNode.toElement();
        QString deviceType = deviceInfo.tagName();
        QString deviceName = deviceInfo.attribute(tr("name"));
        QString minnum = deviceInfo.attribute(tr("minnum"));
        QString maxnum = deviceInfo.attribute(tr("maxnum"));

        m_deviceInfo.insert(deviceType, QStringList()<<deviceName<<minnum<<maxnum);
        m_nameCom->addItem(deviceType);

        deviceNode = deviceNode.nextSibling();
    }

    file.close();
    return true;
}


void InputWindow::on_m_nameCom_triggered(QString idx)
{
    m_nameLabel->setText(m_deviceInfo[idx][0]);
    m_indexLabel->setText(QString("范围：%1~%2")
                          .arg(m_deviceInfo[idx][1])
                          .arg(m_deviceInfo[idx][2]));
    m_indexSpi->setRange(m_deviceInfo[idx][1].toInt(), m_deviceInfo[idx][2].toInt());
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
