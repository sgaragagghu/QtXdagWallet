#include "QtWalletMain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWalletMain w;
    w.show();

    return a.exec();
}
