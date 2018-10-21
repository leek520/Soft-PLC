#include "inputwindow.h"

InputGraphWindow::InputGraphWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);
    setWindowTitle(tr("装置指令输入"));
    setWindowModality(Qt::ApplicationModal);    //设置为模态
    resize(340,200);
    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(10);
    QLabel *namelbl = new QLabel(tr("装置名称："));
    namelbl->setFixedWidth(60);
    m_nameCom = new QComboBox();
    m_nameCom->setFixedWidth(50);
    m_indexSpi = new QSpinBox();
    m_indexSpi->setFixedWidth(50);
    m_indexSpi->setFixedHeight(30);
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

    m_indexSpi->setFocus();
}

void InputGraphWindow::SetCurrentName(int index)
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
        m_type = OutputGraph;
        setWindowIcon(QIcon(":/images/graph/Btn5.bmp"));
        m_nameCom->setCurrentIndex(1);
        break;
    default:
        break;
    }

}

bool InputGraphWindow::ReadInputDeviceInfo()
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


void InputGraphWindow::on_m_nameCom_triggered(QString idx)
{
    m_nameLabel->setText(m_deviceInfo[idx][0]);
    m_indexLabel->setText(QString("范围：%1~%2")
                          .arg(m_deviceInfo[idx][1])
                          .arg(m_deviceInfo[idx][2]));
    m_indexSpi->setRange(m_deviceInfo[idx][1].toInt(), m_deviceInfo[idx][2].toInt());
}


void InputGraphWindow::on_yesBtn_triggered()
{
    Element emt;
    emt.graphType = m_type;
    emt.name = m_nameCom->currentText();
    emt.index = m_indexSpi->value();
    emt.mark = m_textEdit->toPlainText();

    emit sig_inputPara(emt);

    close();
    m_indexSpi->setValue(m_indexSpi->value()+1);
}

void InputGraphWindow::on_cancelBtn_triggered()
{
    close();

}
void InputGraphWindow::showEvent(QShowEvent *event)
{

    m_indexSpi->selectAll();
    QWidget::showEvent(event);
}
InputInstsWindow::InputInstsWindow(QWidget *parent) : QFrame(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);    //设置为模态
    setFixedSize(400, 60);
    setObjectName("InputInstsWindow");
    setStyleSheet("QWidget#InputInstsWindow{border: 1px solid black; border-radius: 5px;}");
    InitUi();

}

void InputInstsWindow::InitUi()
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    lineEdit = new QLineEdit();
    lineEdit->setFixedWidth(150);
    okBtn = new QPushButton("确定");
    cancelBtn = new QPushButton("取消");
    hbox->addWidget(new QLabel("输入指令"));
    hbox->addWidget(lineEdit);
    hbox->addWidget(okBtn);
    hbox->addWidget(cancelBtn);

    connect(okBtn, SIGNAL(clicked(bool)),
            this, SLOT(on_okBtn_triggerred()));
    connect(cancelBtn, SIGNAL(clicked(bool)),
            this, SLOT(on_cancelBtn_triggerred()));


    //快捷键
    QShortcut *key_Return=new QShortcut(QKeySequence(Qt::Key_Return), this);    //创建一个快捷键"Key_Return"键
    connect(key_Return, SIGNAL(activated()), okBtn, SIGNAL(clicked()));    //连接到指定槽函数
    QShortcut *key_Enter=new QShortcut(QKeySequence(Qt::Key_Enter), this);    //创建一个快捷键"Key_Return"键
    connect(key_Enter, SIGNAL(activated()), okBtn, SIGNAL(clicked()));    //连接到指定槽函数

    QShortcut *key_Esc=new QShortcut(QKeySequence(Qt::Key_Escape), this);    //创建一个快捷键"Key_Return"键
    connect(key_Esc, SIGNAL(activated()), cancelBtn, SIGNAL(clicked()));    //连接到指定槽函数

}

int InputInstsWindow::InstsDecoder()
{
    Element emt;
    QString str = lineEdit->text();
    str = str.toUpper();
    QStringList instsStr = str.split(" ");
    int cnt = instsStr.count();
    QString inst = instsStr[0];
    if (inst.indexOf("LD") > -1){
        if (cnt < 2) return -1;
        QRegExp re("^[XYMSTC][0-9]{1,3}");
        if (instsStr[1].indexOf(re) > -1){
            if (inst.indexOf("LDI") > -1){
                emt.graphType = InputClose;
            }else{
                emt.graphType = InputOpen;
            }
            emt.name = instsStr[1][0];
            emt.index = instsStr[1].mid(1).toInt();
            emt.mark = str;
            emit sig_inputPara(emt);
        }
    }else if (inst.indexOf("ADD") > -1){
        if (cnt < 2) return -1;
        emt.graphType = LogicGraph;
        emt.mark = str;
        emit sig_inputPara(emt);
    }else if (inst.indexOf("OUT") > -1){
        if (cnt < 2) return -1;
        emt.graphType = OutputGraph;
        emt.name = instsStr[1][0];
        emt.index = instsStr[1].mid(1).toInt();
        emt.mark = str;
        emit sig_inputPara(emt);
    }
}

void InputInstsWindow::showEvent(QShowEvent *event)
{
    lineEdit->clear();
    QWidget::showEvent(event);
}

void InputInstsWindow::on_okBtn_triggerred()
{
    InstsDecoder();
    close();
}

void InputInstsWindow::on_cancelBtn_triggerred()
{
    close();
}
