#include "widget.h"
#include <cstddef>
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w(nullptr);
    w.show();
    return a.exec();
}
