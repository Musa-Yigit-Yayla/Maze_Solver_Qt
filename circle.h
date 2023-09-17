#ifndef CIRCLE_H
#define CIRCLE_H

#include <QWidget>
#include <QColor>
#include <QPaintEvent>
#include <cstddef>
#include <QPainter>
#include <QBrush>
#include <QRect>

/**
 * @brief The Circle class represents a widget containing a circle
 * The drawn circle is centered on the widget
 * @author yiğit
 */

class Circle : public QWidget{
    Q_OBJECT
public:
    const int DEFAULT_RADIUS_VALUE = 15;
private:
    QColor color;
    int radius = DEFAULT_RADIUS_VALUE;
public:
    explicit Circle(QWidget *parent = nullptr);
    Circle(int r, int g, int b, QWidget* parent = nullptr);
    Circle(QColor color, QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void setColor(int r, int g, int b);
    void setColor(QColor* color);
    QColor getColor() const; //returns a copy of the color field
    void setRadius(int radius);
    int getRadius() const;
signals:

};

#endif // CIRCLE_H
