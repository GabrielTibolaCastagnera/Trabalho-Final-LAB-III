#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui
{
class Widget;
}

class QSerialPort;

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:

    void on_Update_PushButton_clicked();

    void on_State_PushButton_clicked();

    void readSerial();

    void readSettings();

    void writeSettings();

    void Update();

    void closeEvent(QCloseEvent *event);

    void on_Maximum_Humity_spinBox_valueChanged(int arg1);

private:
    Ui::Widget *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
};

#endif // WIDGET_H
