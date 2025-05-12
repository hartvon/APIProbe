#include "apiprobe.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    APIProbe window;
    window.show();
    return app.exec();
}
