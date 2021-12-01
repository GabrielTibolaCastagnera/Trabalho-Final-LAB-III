#include "Widget.h"
#include "ui_Widget.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QIODevice>
#include <QSettings>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Horta Automática");
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
        arduino->setBaudRate(QSerialPort::Baud115200);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        // give error message if not available
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
    }
    readSettings();


}

Widget::~Widget()
{
    delete ui;
}
void Widget::readSettings()

{

    QSettings sett;
    int M = sett.value("Maximum", int(60)).toInt();
    int m = sett.value("Minimum", int(40)).toInt();
    ui->Maximum_Humity_spinBox->setValue(M);
    ui->Minimum_humity_spinBox->setValue(m);
    ui->estado_label->clear();
    ui->humity_label->clear();

}
void Widget::writeSettings()
{
    QSettings sett;
    sett.setValue("Maximum", ui->Maximum_Humity_spinBox->value());
    sett.setValue("Minimum", ui->Minimum_humity_spinBox->value());

}
void Widget::closeEvent(QCloseEvent *event)
{
    writeSettings();
    arduino->write("0\n");
}
void Widget::on_Update_PushButton_clicked()
{
    Update();
}

void Widget::on_State_PushButton_clicked()
{
    if(arduino->isWritable()){
        arduino->write("3\n");
    }else{
        qDebug() << "Couldn't write to serial!";
    }
}
void Widget::readSerial()
{
    char data[101];
    arduino->waitForReadyRead(100);
    arduino->readLine(data, 100);
    ui->humity_label->setText(QString(data));
    arduino->waitForReadyRead(100);
    arduino->readLine(data, 100);
    ui->estado_label->setText(QString(data));
}
void Widget::Update()
{
    int maximum = ui->Maximum_Humity_spinBox->value();
    int minimum = ui->Minimum_humity_spinBox->value();
    char max_humity[10];
    char min_humity[10];
    std::sprintf(max_humity, "%d\n", maximum);
    std::sprintf(min_humity, "%d\n", minimum);
    if(arduino->isWritable()){
        arduino->write("2\n");
        arduino->write(max_humity);
        arduino->write(min_humity);

    }else{
        qDebug() << "Couldn't write to serial!";
    }
}

void Widget::on_Maximum_Humity_spinBox_valueChanged(int arg1)
{
    ui->Minimum_humity_spinBox->setMaximum(arg1 - 1);
}
