#include "circle.h"

Circle::Circle(QWidget *parent): QWidget{parent}{

}
Circle::Circle(int r, int g, int b, QWidget* parent): Circle(parent){
    this->color = new QColor(r, g, b);
}
Circle::Circle(QColor color, QWidget* parent): QWidget{parent}{
    this->color = &color;
}
void Circle::paintEvent(QPaintEvent* event){
    if(this->color != NULL){
        QPainter circlePainter;
        QBrush brush(*this->color);
        circlePainter.setBrush(brush);
        circlePainter.setPen(*this->color);
        circlePainter.begin(this);
        QPoint center(this->radius, this->radius);
        circlePainter.drawEllipse(center, this->radius, this->radius);
        circlePainter.end();
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
    return this->color;
}
void Circle::setRadius(int radius){
    this->radius = radius;
}
int Circle::getRadius() const{
    return this->radius;
}
