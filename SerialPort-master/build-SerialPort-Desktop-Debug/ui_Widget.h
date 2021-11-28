/********************************************************************************
** Form generated from reading UI file 'Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *ligar_PushButton;
    QPushButton *apagarPushButton;
    QLabel *estado_label;
    QLabel *humity_label;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(427, 197);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ligar_PushButton = new QPushButton(Widget);
        ligar_PushButton->setObjectName(QString::fromUtf8("ligar_PushButton"));

        verticalLayout->addWidget(ligar_PushButton);

        apagarPushButton = new QPushButton(Widget);
        apagarPushButton->setObjectName(QString::fromUtf8("apagarPushButton"));

        verticalLayout->addWidget(apagarPushButton);

        estado_label = new QLabel(Widget);
        estado_label->setObjectName(QString::fromUtf8("estado_label"));

        verticalLayout->addWidget(estado_label);

        humity_label = new QLabel(Widget);
        humity_label->setObjectName(QString::fromUtf8("humity_label"));

        verticalLayout->addWidget(humity_label);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        ligar_PushButton->setText(QApplication::translate("Widget", "LIGAR", nullptr));
        apagarPushButton->setText(QApplication::translate("Widget", "APAGAR", nullptr));
        estado_label->setText(QString());
        humity_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
