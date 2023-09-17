#include "circle.h"
#include <iostream>

using namespace std;

Circle::Circle(QWidget *parent): QWidget{parent}{

}
Circle::Circle(int r, int g, int b, QWidget* parent): Circle(parent){
    this->color = new QColor(r, g, b);
    this->setFixedSize(Circle::DEFAULT_RADIUS_VALUE, Circle::DEFAULT_RADIUS_VALUE);
    this->update();
}
Circle::Circle(QColor color, QWidget* parent): QWidget{parent}{
    this->color = &color;
    this->setFixedSize(Circle::DEFAULT_RADIUS_VALUE, Circle::DEFAULT_RADIUS_VALUE);
    this->update();
}
void Circle::paintEvent(QPaintEvent* event){
    cout << "morning freeman" << endl;
    if(this->color != NULL){
        cout << "Looks like you're running late" << endl;
        QPainter circlePainter;
        QBrush brush(*this->color);
        circlePainter.setBrush(brush);
        circlePainter.setPen(*this->color);
        circlePainter.begin(this);
        QPoint center(this->radius, this->radius);
        //circlePainter.drawEllipse(center, this->radius, this->radius);
        QRect boundingRect(0, 0, this->radius * 2, this->radius * 2);
        circlePainter.drawEllipse(boundingRect);
        circlePainter.end();
    }
}
void Circle::setColor(int r, int g, int b){
    if(this->color != NULL){
        delete this->color;
        this->color = nullptr;
    }
    this->color = new QColor(r, g, b);
    //this->paintEvent(nullptr);
    this->update();
}
void Circle::setColor(QColor* color){
    if(this->color != NULL){
        delete this->color;
        this->color = nullptr;
    }
    this->color = color;
    //this->paintEvent(nullptr);
    this->update();
}
//returns a copy of the color field
QColor* Circle::getColor() const{
    return this->color;
}
void Circle::setRadius(int radius){
    if(radius >= 0){
        this->radius = radius;
        this->setFixedSize(this->radius, this->radius);
        this->update();
    }
}
int Circle::getRadius() const{
    return this->radius;
}
