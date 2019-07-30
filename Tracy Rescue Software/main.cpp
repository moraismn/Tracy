#include <QApplication>
#include "body.h"


#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    body b;
    b.show();
    return a.exec();
}
