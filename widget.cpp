#include "widget.h"

Widget::Widget(QWidget *parent){ //: QWidget(parent){
    this->setParent(parent);
    this->mazeGenerator = new MazeGenerator(Widget::TRAVERSABLE_MAZE_THRESHOLD, Widget::AUTO_GENERATED_MAZE_COUNT, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, MazePane::START_POS_VALUE, MazePane::TARGET_POS_VALUE, MazePane::EMPTY_GRID_VALUE, MazePane::WALL_GRID_VALUE);
    this->generateMazes();
    this->setGenerationSelector();
}

Widget::~Widget(){

}

void Widget::setGenerationSelector(){
    this->generationSelector = new QComboBox(this);
    this->generationSelector->addItem(Widget::SELF_GENERATE_VALUE, QVariant(1));
    this->generationSelector->addItem(Widget::AUTO_GENERATE_VALUE, QVariant(2));

}
void Widget::setRadioButtons(){

}
void Widget::setButtons(){

}
//Invoke only once during initialization, from constructor
void Widget::generateMazes(){
    this->generatedMazes = this->mazeGenerator->generateMazes();
}
const string Widget::SELF_GENERATE_VALUE = "Self Generate";
const string Widget::AUTO_GENERATE_VALUE = "Auto Generate";
