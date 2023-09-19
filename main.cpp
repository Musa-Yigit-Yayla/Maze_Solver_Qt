#include "widget.h"
#include <cstddef>
#include <QApplication>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]){
    srand(static_cast<unsigned>(time(nullptr)));
    QApplication a(argc, argv);
    Widget w(nullptr);
    w.show();
    return a.exec();
}
