#include "rectanglewidget.h"

RectangleWidget::RectangleWidget(QWidget* parent){
    this->parent = parent;
    //state is initially set to 0
    this->color = RectangleWidget::generateColor(this->state);
    //paint the rectangle manually
    this->setColor(this->color);
}
RectangleWidget::~RectangleWidget(){
    //deallocate each dynamically allocated property except from parent
    delete this->color;
}
void RectangleWidget::paintEvent(QPaintEvent* event){

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
QColor RectangleWidget::getColor() const{

} //returns a copy of the current rectangle's color
//Static function
QColor* RectangleWidget::generateColor(int state){

}
