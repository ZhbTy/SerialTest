#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>  //����ͷ�ļ�

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //ʹ����ɴ�������
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    MainWindow w;
    w.show();

    return a.exec();
}
