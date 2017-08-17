#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "win_qextserialport.h"  //����ͷ�ļ�
#include <QtGui>
#include <string>
#include <stdlib.h>
#include <QTime>
#include "qextserialenumerator.h"
#include "savedialog.h"

#define SOH 0x01
#define STX 0x02
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15
#define CAN 0x18
#define CTRLZ 0x1A
#define DLY_1S 1000
#define MAXRETRANS 25

static int last_error = 0;



using namespace std;


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Win_QextSerialPort *myCom;  //��������
    void saveInformation(string);
    void out_buf(unsigned char *buf, int size);
    int in_buf(unsigned char *buf);
    int calcrc(const unsigned char *ptr, int count);
    int xmodemReceive(unsigned char *dest, int destsz);
    int check(int crc, const unsigned char *buf, int sz);

private:
    QByteArray m_revDataby;   //���յ�����
    unsigned char *recv_buf; //���յ�����
    int dataSize;
    int recvLength;
    QTime clock;
    saveDialog *sDialog;



private slots:
    int readMycom();    //��������ڲۺ���
    void on_openMyComBtn_clicked();
    void on_closeMyComBtn_clicked();
    void on_sendMsgBtn_clicked();

    void addLog(QString msg); //��־
    void on_saveMsgBtn_clicked();
    void on_recvMsgBtn_clicked();
    void on_clearMyComBtn_clicked();
};

#endif // MAINWINDOW_H
