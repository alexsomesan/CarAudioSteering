#include "maindialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDialog w;
    QStringList args = QCoreApplication::arguments();
    if ( args.length() > 1 && args.at(1).startsWith("--port") ) {
        w.setPortOverride(args.at(1).split('=', Qt::SkipEmptyParts).last());
    }
    w.show();
    return a.exec();
}
