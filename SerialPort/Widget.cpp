#include "Widget.h"
#include "ui_Widget.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QIODevice>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;


    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }


    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    if(arduino_is_available){
        // open and configure the serialport
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud57600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        // give error message if not available
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
    }

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_Update_PushButton_clicked()
{
    if(arduino->isWritable()){
        int maximum = ui->Maximum_Humity_spinBox->value();
        int minimum = ui->Minimum_humity_spinBox->value();
        char max_humity[10];
        char min_humity[10];
        std::sprintf(max_humity, "%d", maximum);
        std::sprintf(min_humity, "%d", minimum);
        arduino->write("2");
        arduino->waitForBytesWritten(100);
        arduino->write(max_humity);
        arduino->waitForBytesWritten(100);
        arduino->write(min_humity);
    }else{
        qDebug() << "Couldn't write to serial!";
    }


    //arduino->waitForReadyRead();

//    char data[101];
//    arduino->readLine(data, 100);
//    ui->estado_label->setText(QString(data));
//    arduino->clear();



}

void Widget::on_State_PushButton_clicked()
{
    if(arduino->isWritable()){
        arduino->write("3");
    }else{
        qDebug() << "Couldn't write to serial!";
    }
}
void Widget::readSerial()
{
    char data[101];
    arduino->waitForReadyRead(100);
    arduino->readLine(data, 100);
    ui->estado_label->setText(QString(data));
    arduino->waitForReadyRead(100);
    arduino->readLine(data, 100);
    ui->humity_label->setText(QString(data));
}
