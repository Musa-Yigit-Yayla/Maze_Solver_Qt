#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <cstddef>
#include <QColor>
#include <QBrush>
#include <QSize>
#include <vector>
#include <cstdlib>
/**
 *This class represents a Rectangle which will be used in our Maze program
 *Private data fields will be used for the purpose of the maze traversal program
 *This class represents a generic rectangle, namely a pervasively utilizable.
 *Hence, you can reuse it in your applications
 *@author Musa Yiğit Yayla
 */

using namespace std;
#ifndef RECTANGLEWIDGET_H
#define RECTANGLEWIDGET_H

class RectangleWidget: public QWidget{
Q_OBJECT

public:
    static const int EMPTY_STATE = 0; //white
    static const int VISITED_STATE = 1; //dark blue
    static const int WALL_STATE = 2; //dark grey
    static const int SOLUTION_STATE = 3; //green
    static const int FAILED_STATE = 4; //red
    constexpr static const int RECTANGLE_WIDTH = 30; //30 pixels
    constexpr static const int RECTANGLE_HEIGHT = 30; //30 pixels
    static  const vector< QColor> DARK_BLUE_COLOR_SET;
    static const vector<QColor> RED_COLOR_SET;
    static const QColor WALL_COLOR;
    static const QColor SOLUTION_COLOR;
    static const QColor EMPTY_COLOR;
private:
    QWidget* parent = nullptr;
    int state = 0;
    QColor* color = nullptr;
public:
    RectangleWidget(QWidget* parent = nullptr);
    ~RectangleWidget();
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void setParent(QWidget* parent);
    QWidget* getParent() const;
    void setState(const int state); //invalid value sets the rectangle to an empty path
    int getState() const;
    void setColor(QColor* color); //Will be used for external purposes not in this project's context
    QColor getColor() const; //returns a copy of the current rectangle's color
    static QColor generateColor(int state); //based on a given state we will return a color
signals:
    void stateChanged(const int state);
public slots:
    void handleStateChange(const int state);

};

#endif // RECTANGLEWIDGET_H
