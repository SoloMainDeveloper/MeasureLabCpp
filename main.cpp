#include "QtWidgetsMeasureLab.h"
#include <QtWidgets/QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QLocale::setDefault(QLocale::Russian);
    QApplication a(argc, argv);
    QtWidgetsMeasureLab w;
    w.show();
    return a.exec();
}