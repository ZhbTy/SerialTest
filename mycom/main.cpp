#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>  //加入头文件

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //使程序可处理中文
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    MainWindow w;
    w.show();

    return a.exec();
}
