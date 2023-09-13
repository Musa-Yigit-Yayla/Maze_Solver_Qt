#include "circle.h"

Circle::Circle(QWidget *parent): QWidget{parent}{

}
Circle::Circle(int r, int g, int b, QWidget* parent): Circle(parent){
    this->color = new QColor(r, g, b);
}
void Circle::paintEvent(QPaintEvent* event){
    if(this->color != NULL){

    }
}
void Circle::setColor(int r, int g, int b){
    if(this->color != NULL){
        delete this->color;
        this->color = nullptr;
    }
    this->color = new QColor(r, g, b);
    this->paintEvent(nullptr);
}
void Circle::setColor(QColor* color){
    if(this->color != NULL){
        delete this->color;
        this->color = nullptr;
    }
    this->color = color;
    this->paintEvent(nullptr);
}
//returns a copy of the color field
QColor* Circle::getColor() const{

}
void Circle::setRadius(int radius){

}
int Circle::getRadius() const{

}
