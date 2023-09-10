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
    QString s1 = QString::fromStdString(Widget::SELF_GENERATE_VALUE);
    QString s2 = QString::fromStdString(Widget::AUTO_GENERATE_VALUE);
    this->generationSelector->addItem(s1, QVariant(1));
    this->generationSelector->addItem(s2, QVariant(2));
    //connect the signal of QComboBox to the slot we have created
    QObject::connect(this->generationSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, (&Widget::generationSelectorHandler));
}
void Widget::setRadioButtons(){

}
void Widget::setButtons(){

}
//Invoke only once during initialization, from constructor
void Widget::generateMazes(){
    this->generatedMazes = this->mazeGenerator->generateMazes();
}
void Widget::generationSelectorHandler(int){

}
const string Widget::SELF_GENERATE_VALUE = "Self Generate";
const string Widget::AUTO_GENERATE_VALUE = "Auto Generate";
