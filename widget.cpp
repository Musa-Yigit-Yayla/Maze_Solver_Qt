#include "widget.h"

Widget::Widget(QWidget *parent){ //: QWidget(parent){
    this->setParent(parent);
    this->mazeGenerator = new MazeGenerator();
}

Widget::~Widget(){

}

