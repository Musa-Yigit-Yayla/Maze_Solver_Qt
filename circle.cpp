#include "circle.h"
#include <iostream>

using namespace std;

Circle::Circle(QWidget *parent): QWidget{parent}{

}
Circle::Circle(int r, int g, int b, QWidget* parent): Circle(parent){
    this->color.setRgb(r, g, b);
    this->setFixedSize(Circle::DEFAULT_RADIUS_VALUE * 2, Circle::DEFAULT_RADIUS_VALUE * 2);
    this->update();
}
Circle::Circle(QColor color, QWidget* parent): QWidget{parent}{
    this->color = color;
    int r, g, b;
    this->color.getRgb(&r, &g, &b);
    cout << "Constructor value " << r << ", " << g << ", " << b << endl;
    this->setFixedSize(Circle::DEFAULT_RADIUS_VALUE * 2, Circle::DEFAULT_RADIUS_VALUE * 2);
    this->update();
}
void Circle::paintEvent(QPaintEvent* event){
    //cout << "morning freeman" << endl;
    //if(this->color != NULL){
        //cout << "Looks like you're running late" << endl;
    int r, g, b;
    this->color.getRgb(&r, &g, &b);
    cout << "paintEvent value" << r << ", " << g << ", " << b << endl;
        QPainter circlePainter;
        QBrush brush(this->color);
        circlePainter.begin(this);
        circlePainter.setBrush(brush);
        circlePainter.setPen(Qt::black);
        QPoint center(this->radius, this->radius);
        circlePainter.drawEllipse(center, this->radius, this->radius);
        circlePainter.setRenderHint(QPainter::Antialiasing); // enable antialiasing for smoother circles
        int centerX = this->width() / 2;
        int centerY = this->height() / 2;

        //cout << "Top left x: " << centerX - this->radius << " top left y: " << centerY - this->radius << endl;
        //QRect boundingRect(centerX - this->radius, centerY - this->radius, 2 * this->radius, 2 * this->radius);
        //QRect boundingRect(0, 0, this->radius * 2, this->radius * 2);
        //circlePainter.drawEllipse(boundingRect);
        circlePainter.end();
    //}
}
void Circle::setColor(int r, int g, int b){
        this->color.setRgb(r, g, b);
    //this->paintEvent(nullptr);
    this->update();
}
void Circle::setColor(QColor* color){
    if(color != NULL){
        this->color = *color;
        //this->paintEvent(nullptr);
        this->update();
    }
}
//returns a copy of the color field
QColor Circle::getColor() const{
    return this->color;
}
void Circle::setRadius(int radius){
    if(radius >= 0){
        this->radius = radius;
        this->setFixedSize(this->radius * 2, this->radius * 2);
        this->update();
    }
}
int Circle::getRadius() const{
    return this->radius;
}
