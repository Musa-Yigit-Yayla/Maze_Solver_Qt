#include "rectanglewidget.h"
#include "widget.h"
#include "mazegenerator.h"
#include <iostream>

using namespace std;

//mazePane argument has a default value of nullptr unless anything has been provided during instantiation call
RectangleWidget::RectangleWidget(int state, QWidget* parent, MazePane* mazePane){
    this->parent = parent;
    if(state < 0 || state > 4){
        state = 0;
    }
    this->mazePane = mazePane;
    this->state = state;
    this->color = new QColor(RectangleWidget::generateColor(this->state));
    //paint the rectangle manually
    this->setColor(this->color);
    //set size properties by using the constant expressions
    int currWidth = RectangleWidget::RECTANGLE_WIDTH;
    int currHeight = RectangleWidget::RECTANGLE_HEIGHT;
    QSize size(currWidth, currHeight);
    this->setFixedSize(size);
    this->update(); //draw the rectangle
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
    rectPainter.setPen(*this->color);
    rectPainter.drawRect(0, 0, RectangleWidget::RECTANGLE_WIDTH, RectangleWidget::RECTANGLE_HEIGHT); //you may need to alter the first 2 parameters
    rectPainter.end();
}
void RectangleWidget::setParent(QWidget* parent){
    this->parent = parent;
}
QWidget* RectangleWidget::getParent() const{
    return this->parent;
}
//Invokes set color automatically
//recursivelyCalled method will be used for logic determining execution flow, do not pass anything if called from outside
void RectangleWidget::setState(const int state, const bool recursivelyCalled){
    QColor* newColor = nullptr;
    MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
    int currLabel;
    switch(state){
        case EMPTY_STATE:
            //if the current rectangle is source or target rectangle we do not perform any operations and return
            if(this->color != NULL && !recursivelyCalled && (*this->color == RectangleWidget::SOURCE_COLOR || *this->color == RectangleWidget::TARGET_COLOR)){
                //do not perform any state changes and return immediately
                return;
            }

            currLabel = this->mazePane->getElementLabel(this);
            this->mazePane->setCellState(mzg.getLabelRow(currLabel), mzg.getLabelColumn(currLabel), state);
            this->state = state;
        break;
        case WALL_STATE:
            //if the current rectangle is source or target rectangle we do not perform any operations and return
        if(this->color != NULL && !recursivelyCalled && (*this->color == RectangleWidget::SOURCE_COLOR || *this->color == RectangleWidget::TARGET_COLOR)){
            //do not perform any state changes and return immediately
            return;
        }
        currLabel = this->mazePane->getElementLabel(this);
        this->mazePane->setCellState(mzg.getLabelRow(currLabel), mzg.getLabelColumn(currLabel), state);
        this->state = state;
        break;
        case VISITED_STATE:
        case SOLUTION_STATE:
        case FAILED_STATE:
            currLabel = this->mazePane->getElementLabel(this);
            this->mazePane->setCellState(mzg.getLabelRow(currLabel), mzg.getLabelColumn(currLabel), state);
            this->state = state; break;
        case MazePane::START_POS_VALUE: this->state = state;
            if(this->mazePane != NULL){
                currLabel = this->mazePane->getElementLabel(this); //label for current rectangle which represents its position in matrix
                if(currLabel != this->mazePane->getSourceLabel()){
                    int currLabelRow = mzg.getLabelRow(currLabel);
                    int currLabelColumn = mzg.getLabelColumn(currLabel);
                    this->mazePane->setSourcePos(currLabelRow, currLabelColumn); //required operations will be performed regarding specified label's
                    //state within setSourcePos method
                }
            }
            else{
                cout << "Error: MazePane* data field of this RectangleWidget is nullptr. Unable to set state." << endl;
            }
        break;
        case MazePane::TARGET_POS_VALUE: this->state = state;
        if(this->mazePane != NULL){
            currLabel = this->mazePane->getElementLabel(this); //label for current rectangle which represents its position in matrix
            if(currLabel != this->mazePane->getTargetLabel()){
                MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
                int currLabelRow = mzg.getLabelRow(currLabel);
                int currLabelColumn = mzg.getLabelColumn(currLabel);
                this->mazePane->setTargetPos(currLabelRow, currLabelColumn); //required operations will be performed regarding specified label's
                //state within setTargetPos method
            }
        }
        else{
            cout << "Error: MazePane* data field of this RectangleWidget is nullptr. Unable to set state." << endl;
        }
        break;
        default: this->state = 0;
    }
    newColor = new QColor(RectangleWidget::generateColor(this->state));
    this->setColor(newColor);
    this->update();
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
        case MazePane::START_POS_VALUE:
            result = RectangleWidget::SOURCE_COLOR; break;
        case MazePane::TARGET_POS_VALUE:
            result = RectangleWidget::TARGET_COLOR; break;
        default:
            result = RectangleWidget::EMPTY_COLOR; break;
    }
    return result;
}
//Signal which will be emitted by mouse clicked
/*void RectangleWidget::stateChanged(const int state){
   //The main logic flow for changing the state(color) of our rectangle is as follows
    switch(state){
        case Widget::RADIO_SELECTION_VALUES::RADIO_EMPTY:
            //If the clicked rectangle represents the source or target location we do not apply any changes
            if(this->state != Widget::RADIO_SOURCE && this->state != Widget::RADIO_TARGET){
                //set the current state and color
                this->setState(state);
            }
        break;
        case Widget::RADIO_WALL:
            //apply the same procedure we have used for empty radio
            if(this->state != Widget::RADIO_SOURCE && this->state != Widget::RADIO_TARGET){
                //set the current state and color
                this->setState(state);
            }
        break;
        case Widget::RADIO_SOURCE:
           //if the current state is not source or destination, set this as source and set the previous source as empty grid element
        if(this->state != Widget::RADIO_SOURCE && this->state != Widget::RADIO_TARGET && this->mazePane != NULL){
                //retrieve the rectangle which is regarded as the previous source
                //int prevSourceLabel = this->mazePane->getSourceLabel();

        }
    }
}*/
//slot which will handle the state change
void RectangleWidget::handleStateChange(const int state){
    this->setState(state); //sets the new state, updates the color and repaints the widget
}
bool RectangleWidget::isSource() const{
    bool result = false;
    if(this->color != NULL){
        int* currR = nullptr;
        int* currG = nullptr;
        int* currB = nullptr;
        this->color->getRgb(currR, currG, currB);
        int* sourceR = nullptr;
        int* sourceG = nullptr;
        int* sourceB = nullptr;
        RectangleWidget::SOURCE_COLOR.getRgb(sourceR, sourceG, sourceB);
        result = ((*currR == *sourceR) && (*currG == *sourceG) && (*currB == *sourceB));
    }
    return result;
}
bool RectangleWidget::isTarget() const{
    bool result = false;
    if(this->color != NULL){
        int* currR = nullptr;
        int* currG = nullptr;
        int* currB = nullptr;
        this->color->getRgb(currR, currG, currB);
        int* targetR = nullptr;
        int* targetG = nullptr;
        int* targetB = nullptr;
        RectangleWidget::TARGET_COLOR.getRgb(targetR, targetG, targetB);
        result = ((*currR == *targetR) && (*currG == *targetG) && (*currB == *targetB));
    }
    return result;
}
void RectangleWidget::mousePressEvent(QMouseEvent* event){
    //we determine the next state based on the current radio button selection
    //if(this->parent != NULL){
        //Widget* container = reinterpret_cast<Widget*>(this->parent->parentWidget());
    if(this->mazePane != NULL && this->mazePane->getIsMutable()){ // do not alternate the maze if it has been solved
        int nextState = Widget::getRadioSelection();
        cout <<  "Next state at RectangleWidget::mousePressEvent is " << nextState << endl;
        emit stateChanged(nextState);
    }
    //}
}

const vector<QColor> RectangleWidget::DARK_BLUE_COLOR_SET = {QColor(0, 0, 128), QColor(25, 25, 112), QColor(72, 61, 139), QColor(65, 105, 225), QColor(75, 0, 139)};
const vector<QColor> RectangleWidget::RED_COLOR_SET = {QColor(139, 0, 0), QColor(178, 34, 34), QColor(220, 20, 60), QColor(255, 0, 0), QColor(255, 69, 0)};
const QColor RectangleWidget::WALL_COLOR(64, 64, 64);
const QColor RectangleWidget::SOLUTION_COLOR(50, 205, 50);
const QColor RectangleWidget::EMPTY_COLOR(215, 213, 198);
const QColor RectangleWidget::SOURCE_COLOR(253, 185, 18);
const QColor RectangleWidget::TARGET_COLOR(108,29,69);

