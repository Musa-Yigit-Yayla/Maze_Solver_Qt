#include <QWidget>
#include <cstddef>
#include <QColor>
/**
 *This class represents a Rectangle which will be used in our Maze program
 *This class represents a generic rectangle, namely a pervasively utilizable.
 *Hence, you can reuse it in your applications
 *@author Musa Yiğit Yayla
 */
#ifndef RECTANGLEWIDGET_H
#define RECTANGLEWIDGET_H

class RectangleWidget: public QWidget{
public:
    static const int EMPTY_STATE = 0;
    static const int VISITED_STATE = 1;
    static const int WALL_STATE = 2;
    static const int SOLUTION_STATE = 3;
private:
    QWidget* parent = nullptr;
    int state = 0;
    QColor* color = nullptr;
public:
    RectangleWidget(QWidget* parent = nullptr);
    ~RectangleWidget();
    void setParent(QWidget* parent);
    QWidget* getParent() const;
    void setState(const int state); //invalid value sets the rectangle to an empty path
    int getState() const;
    void setColor(QColor* color);
    QColor getColor() const; //returns a copy of the current rectangle's color
};

#endif // RECTANGLEWIDGET_H
