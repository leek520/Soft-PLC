﻿#include "mainwindow.h"
#include <QApplication>
//http://jz.docin.com/p-313859620.html

#include <QTextCodec>
#include <QSplashScreen>
#include <QThread>
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("Debug:");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:");
        break;
    }

    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
//    QString strMessage = QString("Message:%1 File:%2  Line:%3  Function:%4  DateTime:%5")
//            .arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);
    QString strMessage = QString("%1")
            .arg(localMsg.constData());

    // 输出信息至文件中（读写、追加形式）
    QFile file("log.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}
/*************加载qss文件的函数*************/
class CommonHelper
{
public:
    static void setStyle(const QString &style)
    {
        QFile qss(style);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //加载并显示启动画面
    QSplashScreen splash(QPixmap(":/images/splash.png"));
    splash.setDisabled(true); //禁用用户的输入事件响应
    splash.show();
    splash.showMessage(QObject::tr("正在启动中...."),
        Qt::AlignCenter|Qt::AlignBottom,Qt::green);

    a.processEvents();              //使程序在显示启动画面的同时仍能响应鼠标等其他事件
    QThread::sleep(3);

    //设置程序中文字的编码方式
    QTextCodec *codec = QTextCodec::codecForName("UTF_8");
    QTextCodec::setCodecForLocale(codec);


    //设置路径为应用程序启动路径:打包exe时使用
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    //注册MessageHandler
    //qInstallMessageHandler(outputMessage);
    //取消输出到log.txt
    qInstallMessageHandler(0);

    QFile file("log.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    file.close();

    // 加载QSS样式
    CommonHelper::setStyle(":/style.qss");

    //设置字体
    QFont font;
    font.setFamily(QObject::tr("微软雅黑"));
    font.setPixelSize(12);
    a.setFont(font);

    MainWindow w;
    w.show();

    splash.finish(&w);              //主窗体初始化完成后，启动画面关闭

    return a.exec();
}
