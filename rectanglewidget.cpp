#include "rectanglewidget.h"

RectangleWidget::RectangleWidget(QWidget* parent){
    this->parent = parent;
    //state is initially set to 0
    this->color = RectangleWidget::generateColor(this->state);
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
    this->color = RectangleWidget::generateColor(this->state);
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
        RectangleWidget::generateColor(this->state);
} //invalid value sets the rectangle to an empty path
int RectangleWidget::getState() const{
    return this->state;
}
void RectangleWidget::setColor(QColor* color){
    this->update();
} //Will be used for external purposes not in this project's context

//Caller is responsible of deallocation of the QColor
QColor RectangleWidget::getColor() const{
    return *(this->color()); // return by copy

} //returns a copy of the current rectangle's color
//Static function
QColor& RectangleWidget::generateColor(int state){
    switch(state){
        case VISITED_STATE: //dark blue

        break;
        case WALL_STATE: //dark grey fixed color

        break;

    }
}
void RectangleWidget::stateChanged(const int state){

}
void RectangleWidget::handleStateChange(const int state){

}
void RectangleWidget::mousePressEvent(QMouseEvent* event){

}

static  const vector< QColor> DARK_BLUE_COLOR_SET = {QColor(0, 0, 128), QColor(25, 25, 112), QColor(72, 61, 139), QColor(65, 105, 225), QColor(75, 0, 139)};



