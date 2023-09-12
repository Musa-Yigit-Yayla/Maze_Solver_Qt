#include "rectanglewidget.h"
#include "widget.h"
#include <iostream>

using namespace std;

RectangleWidget::RectangleWidget(int state, QWidget* parent){
    this->parent = parent;
    if(state < 0 || state > 4){
        state = 0;
    }
    this->state = state;
    this->color = new QColor(RectangleWidget::generateColor(this->state));
    //paint the rectangle manually
    this->setColor(this->color);
    //set size properties by using the constant expressions
    int currWidth = RectangleWidget::RECTANGLE_WIDTH;
    int currHeight = RectangleWidget::RECTANGLE_HEIGHT;
    QSize size(currWidth, currHeight);
    this->setFixedSize(size);
    //connect the signal with the slot for event handling
    QObject::connect(this, (&RectangleWidget::stateChanged), this, (&RectangleWidget::handleStateChange));

}
RectangleWidget::~RectangleWidget(){
    //deallocate each dynamically allocated property except from parent
    delete this->color;
}
void RectangleWidget::paintEvent(QPaintEvent* event){
    if(this->color != NULL){
        delete this->color;
    }
    this->color = new QColor(RectangleWidget::generateColor(this->state));
    QPainter rectPainter;
    rectPainter.begin(this);
    QBrush brush(*this->color);
    rectPainter.setBrush(brush);
}
void RectangleWidget::setParent(QWidget* parent){
    this->parent = parent;
}
QWidget* RectangleWidget::getParent() const{
    return this->parent;
}
//Invokes set color automatically
void RectangleWidget::setState(const int state){
    QColor* newColor = nullptr;
    switch(state){
        case EMPTY_STATE:
        case VISITED_STATE:
        case WALL_STATE:
        case SOLUTION_STATE:
        case FAILED_STATE: this->state = state; break;
        default: this->state = 0;
    }
        newColor = new QColor(RectangleWidget::generateColor(this->state));
    this->setColor(newColor);
} //invalid value sets the rectangle to an empty path
int RectangleWidget::getState() const{
    return this->state;
}
void RectangleWidget::setColor(QColor* color){
    this->color = color;
    this->update();
} //Will be used for external purposes not in this project's context

//Caller is responsible of deallocation of the QColor
QColor RectangleWidget::getColor() const{
    return QColor(*this->color); // return by copy

}
//returns a copy of the current rectangle's color
//Static function
//If the given state is invalid white color is returned
QColor RectangleWidget::generateColor(int state){
    QColor result;
    switch(state){
        case VISITED_STATE: //dark blue
            result = RectangleWidget::DARK_BLUE_COLOR_SET.at(rand() % DARK_BLUE_COLOR_SET.size()); break;
        case WALL_STATE: //dark grey fixed color
            result = RectangleWidget::WALL_COLOR; break;
        case SOLUTION_STATE:
            result = RectangleWidget::SOLUTION_COLOR; break;
        case FAILED_STATE:
            result = RectangleWidget::RED_COLOR_SET.at(rand() % RED_COLOR_SET.size()); break;
        default:
            result = RectangleWidget::EMPTY_COLOR; break;
    }
    return result;
}
//Signal which will be emitted by mouse clicked
void RectangleWidget::stateChanged(const int state){
    switch(state){
        case Widget::RADIO_SELECTION_VALUES::RADIO_EMPTY:
    }
}
//slot which will handle the state change
void RectangleWidget::handleStateChange(const int state){
    this->setState(state); //sets the new state, updates the color and repaints the widget
}
void RectangleWidget::mousePressEvent(QMouseEvent* event){
    //we determine the next state based on the current radio button selection
    //if(this->parent != NULL){
        //Widget* container = reinterpret_cast<Widget*>(this->parent->parentWidget());

        int nextState = Widget::getRadioSelection();
        emit stateChanged(nextState);
    //}
}

const vector<QColor> RectangleWidget::DARK_BLUE_COLOR_SET = {QColor(0, 0, 128), QColor(25, 25, 112), QColor(72, 61, 139), QColor(65, 105, 225), QColor(75, 0, 139)};
const vector<QColor> RectangleWidget::RED_COLOR_SET = {QColor(139, 0, 0), QColor(178, 34, 34), QColor(220, 20, 60), QColor(255, 0, 0), QColor(255, 69, 0)};
const QColor RectangleWidget::WALL_COLOR(64, 64, 64);
const QColor RectangleWidget::SOLUTION_COLOR(50, 205, 50);
const QColor RectangleWidget::EMPTY_COLOR(255, 87, 51);


