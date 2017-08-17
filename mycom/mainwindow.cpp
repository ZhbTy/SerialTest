#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtCore/qtextcodec.h>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    this->setWindowTitle(tr("benan"));
    ui->closeMyComBtn->setEnabled(false);
    ui->sendMsgBtn->setEnabled(false);
    ui->saveMsgBtn->setEnabled(false);
    ui->recvMsgBtn->setEnabled(false);
    ui->clearMyComBtn->setEnabled(false);

    sDialog = NULL;
    sDialog = new saveDialog();

    recv_buf = (unsigned char *)malloc(4096);
    memset(recv_buf, 0, sizeof(recv_buf));

}


MainWindow::~MainWindow()
{
    free(recv_buf);
    delete ui;
}

int MainWindow::readMycom()//读串口函数
{
    m_revDataby.clear();
    m_revDataby = myCom->readAll();
    //读取串口缓冲区的所有数据给临时变量temp
    //qDebug() << "m_revDataby.size = " << m_revDataby.size();
    ui->textBrowser->insertPlainText(m_revDataby);
    //将串口的数据显示在窗口的文本浏览器中
    //m_revDataby.append(ba);
}

//添加日志
void MainWindow::addLog(QString msg)
{
  ui->logBrowser->append(msg);
  ui->logBrowser->append("===============================");
}


void MainWindow::on_openMyComBtn_clicked()
{

    QString portName = ui->portNameComboBox->currentText();  //获取串口名
    qDebug()<<"OPEN COM:"<<portName;
    myCom = new Win_QextSerialPort(portName, QextSerialBase::EventDriven);
    //定义串口对象， 并传递参数， 在构造函数里对其进行初始化
    bool ok =  myCom->open(QIODevice::ReadWrite);
    //以读写方式打开串口

    if(ui->baudRateComboBox->currentText()==tr("9600")) //根据组合框的内容对串口进行设置
    myCom->setBaudRate(BAUD9600);
    else if(ui->baudRateComboBox->currentText()==tr("115200"))
    myCom->setBaudRate(BAUD115200);
    //设置波特率

    if(ui->dataBitsComboBox->currentText()==tr("8"))
    myCom->setDataBits(DATA_8);
    else if(ui->dataBitsComboBox->currentText()==tr("7"))
    myCom->setDataBits(DATA_7);
    //设置数据位

    if(ui->parityComboBox->currentText()==tr("无"))
    myCom->setParity(PAR_NONE);
    else if(ui->parityComboBox->currentText()==tr("奇"))
    myCom->setParity(PAR_ODD);
    else if(ui->parityComboBox->currentText()==tr("偶"))
    myCom->setParity(PAR_EVEN);
    //设置奇偶校验

    if(ui->stopBitsComboBox->currentText()==tr("1"))
    myCom->setStopBits(STOP_1);
    else if(ui->stopBitsComboBox->currentText()==tr("2"))
    myCom->setStopBits(STOP_2);
    //设置停止位

    myCom->setFlowControl(FLOW_OFF); //设置数据流控制，我们使用无数据流控制的默认设置
    myCom->setTimeout(500); //设置延时

    connect(myCom, SIGNAL(readyRead()), this, SLOT(readMycom()));
    //信号和槽函数相连， 当串口缓冲区有数据时， 进行读串口操作

    if(ok == false)
    {
        addLog("open serialPort:"+myCom->portName()+" fail");
        QMessageBox::warning(this, tr("warning"), tr("this port is no exist or be used"), QMessageBox::Yes);
    }
    else
    {
        addLog("open serialPort:"+myCom->portName()+" Success");
        ui->openMyComBtn->setEnabled(false); //打开串口后“打开串口”按钮不可用
        ui->closeMyComBtn->setEnabled(true); //打开串口后“关闭串口”按钮可用
        ui->sendMsgBtn->setEnabled(true); //打开串口后“发送数据”按钮可用
        ui->saveMsgBtn->setEnabled(true); //打开串口后“保存”按钮可用
        ui->recvMsgBtn->setEnabled(true);
        ui->clearMyComBtn->setEnabled(true);
        ui->baudRateComboBox->setEnabled(false); //设置各个组合框不可用
        ui->dataBitsComboBox->setEnabled(false);
        ui->parityComboBox->setEnabled(false);
        ui->stopBitsComboBox->setEnabled(false);
        ui->portNameComboBox->setEnabled(false);
    }

#if 0
    if(myCom->isOpen())
    {
        addLog("open serialPort:"+myCom->portName()+" Success");
        ui->openMyComBtn->setEnabled(false); //打开串口后“打开串口”按钮不可用
        ui->closeMyComBtn->setEnabled(true); //打开串口后“关闭串口”按钮可用
        ui->sendMsgBtn->setEnabled(true); //打开串口后“发送数据”按钮可用
        ui->saveMsgBtn->setEnabled(true); //打开串口后“保存”按钮可用
        ui->recvMsgBtn->setEnabled(true);
        ui->clearMyComBtn->setEnabled(true);
        ui->baudRateComboBox->setEnabled(false); //设置各个组合框不可用
        ui->dataBitsComboBox->setEnabled(false);
        ui->parityComboBox->setEnabled(false);
        ui->stopBitsComboBox->setEnabled(false);
        ui->portNameComboBox->setEnabled(false);
    }
#endif

}

void MainWindow::on_closeMyComBtn_clicked() //关闭串口槽函数
{
    myCom->close(); //关闭串口，该函数在win_qextserialport.cpp文件中定义
    addLog("close serialPort:"+myCom->portName()+" Success");
    ui->openMyComBtn->setEnabled(true); //关闭串口后“打开串口”按钮可用
    ui->closeMyComBtn->setEnabled(false); //关闭串口后“关闭串口不可用”按钮不可用
    ui->sendMsgBtn->setEnabled(false); //关闭串口后“发送数据”按钮不可用
    ui->saveMsgBtn->setEnabled(false); //关闭串口后“保存”按钮不可用
    ui->recvMsgBtn->setEnabled(false);
    ui->clearMyComBtn->setEnabled(false);
    ui->baudRateComboBox->setEnabled(true); //设置各个组合框可用
    ui->dataBitsComboBox->setEnabled(true);
    ui->parityComboBox->setEnabled(true);
    ui->stopBitsComboBox->setEnabled(true);
    ui->portNameComboBox->setEnabled(true);
    ui->textBrowser->clear();
    ui->logBrowser->clear();
}

void MainWindow::on_sendMsgBtn_clicked()
{
    myCom->write(ui->sendMsgLineEdit->text().toAscii());
    ui->sendMsgLineEdit->clear();
}

void MainWindow::on_saveMsgBtn_clicked()
{
    string key((const char *)recv_buf);
    if(ui->productNum->text().isEmpty())
    {
        QMessageBox::warning(this, tr("warning"), tr("ProductID is empty"), QMessageBox::Yes);
        return;
    }
    saveInformation(key);
}



void MainWindow::saveInformation(string key)
{
    //QDateTime time = QDateTime::currentDateTime();
    //QString str = time.toString("yyyy-MM-dd hh-mm-ss");
    QString strPath = "./";
    strPath.append(ui->productNum->text());
    strPath.append(".txt");

    QFile iniFile("./signalLog.ini");
    if(!iniFile.open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(this, "sdf", "Save Inifile Fail", QMessageBox::Yes);
    }

    QFile file(strPath);
    if(file.exists())
    {
        int result = sDialog->exec();
        if(result == QDialog::Accepted)
        {
            sDialog->close();
            file.remove();
        }
        else if(result == QDialog::Rejected)
        {
            sDialog->close();
            return;
        }
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QMessageBox::warning(this, "sdf", "Save Information Fail", QMessageBox::Yes);
    }

    iniFile.write(key.c_str());
    iniFile.close();

    QSettings *configIniRead = new QSettings("./signalLog.ini", QSettings::IniFormat);

    QString date = configIniRead->value("/Date/checkDate").toString();
    //qDebug() << date;
    QString dateName = configIniRead->value("/Date/name").toString();
    if(date.length() > 0)
    {
    file.write(dateName.toStdString().c_str());
    file.write(":");
    file.write(date.toStdString().c_str());
    file.write("\r\n");
    }

    QString productNum = ui->productNum->text();
    if(productNum.length() > 0)
    {
        file.write("产品编号:");
        file.write(productNum.toStdString().c_str());
        file.write("\r\n");
        file.write("\r\n");
    }

    QString arm9_boot_versionName = configIniRead->value("/arm9_boot_version/name").toString();
    QString arm9_boot_version = configIniRead->value("/arm9_boot_version/arm9Ver").toString();
    if(arm9_boot_versionName.length() > 0)
    {
    file.write(arm9_boot_versionName.toStdString().c_str());
    file.write(":");
    file.write(arm9_boot_version.toStdString().c_str());
    file.write("\r\n");
    }

    QString currentName = configIniRead->value("/1current/name").toString();
    QString current = configIniRead->value("/1current/currentValue").toString();
    if(currentName.length() > 0)
    {
    file.write(currentName.toStdString().c_str());
    file.write(":");
    file.write(current.toStdString().c_str());
    file.write("\r\n");
    }

    QString accName = configIniRead->value("/Acc/name").toString();
    QString acc = configIniRead->value("Acc/checkAcc").toString();
    if(accName.length() > 0)
    {
    file.write(accName.toStdString().c_str());
    file.write(":");
    file.write(acc.toStdString().c_str());
    file.write("\r\n");
    }

    QString canName = configIniRead->value("/Can/name").toString();
    QString can = configIniRead->value("Can/checkCan").toString();
    if(canName.length() > 0)
    {
    file.write(canName.toStdString().c_str());
    file.write(":");
    file.write(can.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameDI0 = configIniRead->value("/DI0/name").toString();  //前门
    QString checkDI0 = configIniRead->value("DI0/checkDI0").toString();
    if(nameDI0.length() > 0)
    {
    file.write(nameDI0.toStdString().c_str());
    file.write(":");
    file.write(checkDI0.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameDI1 = configIniRead->value("/DI1/name").toString();  //远光灯
    QString checkDI1 = configIniRead->value("DI1/checkDI1").toString();
    if(nameDI1.length() > 0)
    {
    file.write(nameDI1.toStdString().c_str());
    file.write(":");
    file.write(checkDI1.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameDI2 = configIniRead->value("/DI2/name").toString();  //右转向灯
    QString checkDI2 = configIniRead->value("DI2/checkDI2").toString();
    if(nameDI2.length() > 0)
    {
    file.write(nameDI2.toStdString().c_str());
    file.write(":");
    file.write(checkDI2.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameDI3 = configIniRead->value("/DI3/name").toString();  //中门
    QString checkDI3 = configIniRead->value("/DI3/checkDI3").toString();
    if(nameDI3.length() > 0)
    {
    file.write(nameDI3.toStdString().c_str());
    file.write(":");
    file.write(checkDI3.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameDI4 = configIniRead->value("/DI4/name").toString();  //左转向灯
    QString checkDI4 = configIniRead->value("/DI4/checkDI4").toString();
    if(nameDI4.length() > 0)
    {
    file.write(nameDI4.toStdString().c_str());
    file.write(":");
    file.write(checkDI4.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameDI5 = configIniRead->value("/DI5/name").toString();  //近光灯
    QString checkDI5 = configIniRead->value("/DI5/checkDI5").toString();
    if(nameDI5.length() > 0)
    {
    file.write(nameDI5.toStdString().c_str());
    file.write(":");
    file.write(checkDI5.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameDI6 = configIniRead->value("/DI6/name").toString();  //离合器风扇
    QString checkDI6 = configIniRead->value("/DI6/checkDI6").toString();
    if(nameDI6.length() > 0)
    {
    file.write(nameDI6.toStdString().c_str());
    file.write(":");
    file.write(checkDI6.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameDI7 = configIniRead->value("/DI7/name").toString();  //刹车灯
    QString checkDI7 = configIniRead->value("/DI7/checkDI7").toString();
    if(nameDI7.length() > 0)
    {
    file.write(nameDI7.toStdString().c_str());
    file.write(":");
    file.write(checkDI7.toStdString().c_str());
    file.write("\r\n");
    }

    QString dataLog = configIniRead->value("/DataLog/name").toString();
    QString checkDataLog = configIniRead->value("/DataLog/checkDataLog").toString();
    if(dataLog.length() > 0)
    {
    file.write(dataLog.toStdString().c_str());
    file.write(":");
    file.write(checkDataLog.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameEXDI0 = configIniRead->value("/EXDI0/name").toString();  //空调
    QString checkEXDI0 = configIniRead->value("/EXDI0/checkEXDI0").toString();
    if(nameEXDI0.length() > 0)
    {
    file.write(nameEXDI0.toStdString().c_str());
    file.write(":");
    file.write(checkEXDI0.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameEXDI1 = configIniRead->value("/EXDI1/name").toString();  //水暖加热器
    QString checkEXDI1 = configIniRead->value("/EXDI1/checkEXDI1").toString();
    if(nameEXDI1.length() > 0)
    {
    file.write(nameEXDI1.toStdString().c_str());
    file.write(":");
    file.write(checkEXDI1.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameEXDI2 = configIniRead->value("/EXDI2/name").toString();  //空档
    QString checkEXDI2 = configIniRead->value("/EXDI2/checkEXDI2").toString();
    if(nameEXDI2.length() > 0)
    {
    file.write(nameEXDI2.toStdString().c_str());
    file.write(":");
    file.write(checkEXDI2.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameEXDI3 = configIniRead->value("/EXDI3/name").toString();  //后雾灯
    QString checkEXDI3 = configIniRead->value("/EXDI3/checkEXDI3").toString();
    if(nameEXDI3.length() >0)
    {
    file.write(nameEXDI3.toStdString().c_str());
    file.write(":");
    file.write(checkEXDI3.toStdString().c_str());
    file.write("\r\n");
    }

    QString nameEXDI4 = configIniRead->value("/EXDI4/name").toString();  //扩展开关量
    QString checkEXDI4 = configIniRead->value("/EXDI4/checkEXDI4").toString();
    if(nameEXDI4.length() > 0)
    {
    file.write(nameEXDI4.toStdString().c_str());
    file.write(":");
    file.write(checkEXDI4.toStdString().c_str());
    file.write("\r\n");
    }

    QString gprsModule = configIniRead->value("/GprsModule/name").toString();
    QString checkGprs = configIniRead->value("/GprsModule/checkGprs").toString();
    if(gprsModule.length() > 0)
    {
    file.write(gprsModule.toStdString().c_str());
    file.write(":");
    file.write(checkGprs.toStdString().c_str());
    file.write("\r\n");
    }

    QString gpsModule = configIniRead->value("/GpsModule/name").toString();
    QString checkGps = configIniRead->value("/GpsModule/checkGps").toString();
    if(gpsModule.length() > 0)
    {
    file.write(gpsModule.toStdString().c_str());
    file.write(":");
    file.write(checkGps.toStdString().c_str());
    file.write("\r\n");
    }

    QString mainPower = configIniRead->value("/MainPower/name").toString();
    QString checkMainPower = configIniRead->value("/MainPower/MainPower").toString();
    if(mainPower.length() > 0)
    {
    file.write(mainPower.toStdString().c_str());
    file.write(":");
    file.write(checkMainPower.toStdString().c_str());
    file.write("\r\n");
    }

    QString sos = configIniRead->value("/SOS/name").toString();
    QString checkSos = configIniRead->value("/SOS/checkSos").toString();
    if(sos.length() > 0)
    {
    file.write(sos.toStdString().c_str());
    file.write(":");
    file.write(checkSos.toStdString().c_str());
    file.write("\r\n");
    }

    QString usb = configIniRead->value("/USB/name").toString();
    QString checkUsb = configIniRead->value("/USB/checkUSB").toString();
    if(usb.length() > 0)
    {
    file.write(usb.toStdString().c_str());
    file.write(":");
    file.write(checkUsb.toStdString().c_str());
    file.write("\r\n");
    }

    QString serial = configIniRead->value("/serial/name").toString();
    QString checkSerial = configIniRead->value("/serial/checkSerial").toString();
    if(serial.length() > 0)
    {
    file.write(serial.toStdString().c_str());
    file.write(":");
    file.write(checkSerial.toStdString().c_str());
    file.write("\r\n");
    }

    QString speed = configIniRead->value("/speed/name").toString();
    QString checkSpeed = configIniRead->value("/speed/checkSpeed").toString();
    if(speed.length() > 0)
    {
    file.write(speed.toStdString().c_str());
    file.write(":");
    file.write(checkSpeed.toStdString().c_str());
    file.write("\r\n");
    }

    file.write("\r\n");
    file.write("\r\n");
    file.write("检测人签名:");

    delete configIniRead;










    //file.write(key.c_str());
    //file.write("\r\n");
    file.close();

}



void MainWindow::out_buf(unsigned char *buf, int size)
{
    myCom->write((char *)buf, 1);
}


int MainWindow::in_buf(unsigned char *buf)
{
    int readSize = 0;
    last_error = 0;
    //connect(myCom, SIGNAL(readyRead()), this, SLOT(readMycom()));
    m_revDataby.clear();

    //QTime time;
    clock.start();
    int flag = 0;
    while(1)
    {
        QByteArray ba = myCom->readAll();
        if(!ba.isEmpty())
        {
            m_revDataby.append(ba);
            if(m_revDataby.size() == 133)
                break;
            if(m_revDataby[0] == ACK || m_revDataby[0] == CAN || m_revDataby[0] == NAK || m_revDataby[0] == EOT)
                break;
        }
        if(clock.elapsed()/1000.0 == 8)
        {
            flag = 1;
            qDebug() << "time out";
            break;
        }
    }
    //qDebug() << m_revDataby.toHex();
    readSize = m_revDataby.size();
    //qDebug() << "readSize = " << readSize;
    for(int i = 0; i < readSize; ++i)
    {
        buf[i] = m_revDataby[i];
        //qDebug() << buf[i];
    }

    //QTextCodec *codec = QTextCodec::codecForName("GBK");
    //QString str = codec->toUnicode(m_revDataby);
    QString str = QString::fromLocal8Bit(m_revDataby);
    ui->textBrowser->insertPlainText(str);
    QCoreApplication::processEvents();
    //qDebug() << buf;
    if(0 == readSize)
        last_error = 1;
    return readSize;
}

int MainWindow::calcrc(const unsigned char *ptr, int count)
{
    int crc;
    char i;
    crc = 0;
    while(--count >= 0)
    {
        crc = crc ^ (int)*ptr++ << 8;
        i = 8;
        do
        {
            if(crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        }while(--i);
    }

    return (crc);
}

int MainWindow::check(int crc, const unsigned char *buf, int sz)
{
    if(crc)
    {
        unsigned short crc = calcrc(buf, sz);
        unsigned short tcrc = (buf[sz] << 8)+ buf[sz+1];
        if(crc == tcrc)
            return 1;
    }
    else
    {
        int i = 0;
        unsigned char cks = 0;

        for(i = 0; i < sz; i++)
        {
            cks += buf[i];
        }

        if(cks == buf[sz])
            return 1;
    }

    return 0;
}

int MainWindow::xmodemReceive(unsigned char *dest, int destsz)
{
    unsigned char xbuff[140];
    int bufsz = 0;
    int crc = 0;
    unsigned char trychar = 'C';
    unsigned char packetno = 1;
    int c = 0;
    int len = 0;
    int retry = 0;
    int retrans = MAXRETRANS;
    int recvSize = 0;

    for(;;)
    {
            for(retry = 0; retry < 16; retry ++)
            {
                    if(trychar)
                    {
                            xbuff[0] = trychar;
                            out_buf(xbuff, 1);
                            //qDebug() << "send" << xbuff[0];
                    }

                    recvSize = in_buf(xbuff);
                    //qDebug() << "recvSize = " << recvSize;
                    c = xbuff[0];

                    if (last_error == 0)
                    {
                            switch(c)
                            {
                                    case SOH:
                                            bufsz = 128;
                                            goto start_recv;
                                    case STX:

                                    {
                                            xbuff[0] = CAN;
                                            out_buf(xbuff, 1);
                                    }
                                    return -1;
                                    case EOT:

                                            {
                                                    xbuff[0] = ACK;
                                                    out_buf(xbuff, 1);
                                            }
                                            return len;
                                    case CAN:

                                            in_buf(xbuff);
                                            c = xbuff[0];
                                            if(c == CAN)
                                            {
                                                    {
                                                            xbuff[0] = ACK;
                                                            out_buf(xbuff, 1);
                                                    }
                                                    return -1;
                                            }
                                            break;
                                    default:
                                            break;
                            }
                    }
            }

            if (trychar == 'C')
            {
                    trychar = NAK;
                    continue;
            }

            {
                    xbuff[0] = CAN;
                    out_buf(xbuff, 1);
                    out_buf(xbuff, 1);
                    out_buf(xbuff, 1);
            }

            return -2;

start_recv:
            if(trychar == 'C')
                    crc = 1;

            trychar = 0;

            if(recvSize != (bufsz + (crc ? 1 : 0) + 4))
                    goto reject;


            if(xbuff[1] == (unsigned char)(~xbuff[2]) &&
                    (xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno - 1) &&
                    check(crc, &xbuff[3], bufsz))
            {
                //qDebug() << "send";
                    if(xbuff[1] == packetno)
                    {
                            int count = destsz - len;

                            if (count > bufsz)
                                    count = bufsz;

                            if (count > 0)
                            {
                                    memcpy(&dest[len], &xbuff[3], count);
                                    len += count;
                            }

                            packetno ++;

                            retrans = MAXRETRANS+1;
                    }

                    if(-- retrans <= 0)
                    {
                            {
                                    xbuff[0] = CAN;
                                    out_buf(xbuff, 1);
                                    out_buf(xbuff, 1);
                                    out_buf(xbuff, 1);
                            }
                            return -3;
                    }

                    {
                                    xbuff[0] = ACK;
                                    out_buf(xbuff, 1);
                    }

                    continue;
            }

reject:
            {
                            xbuff[0] = NAK;
                            out_buf(xbuff, 1);
            }

    }
}



















void MainWindow::on_recvMsgBtn_clicked()
{
    disconnect(myCom, SIGNAL(readyRead()), this, SLOT(readMycom()));
    recvLength = xmodemReceive(recv_buf, 4096);
}

void MainWindow::on_clearMyComBtn_clicked()
{
    ui->textBrowser->clear();
}
