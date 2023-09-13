#include "widget.h"
#include "mazegenerator.h"
#include "mazepane.h"

Widget::Widget(QWidget *parent){ //: QWidget(parent){
    this->setParent(parent);
    this->mazeGenerator = new MazeGenerator(Widget::TRAVERSABLE_MAZE_THRESHOLD, Widget::AUTO_GENERATED_MAZE_COUNT, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, MazePane::START_POS_VALUE, MazePane::TARGET_POS_VALUE, MazePane::EMPTY_GRID_VALUE, MazePane::WALL_GRID_VALUE);
    this->generateMazes();
    this->setGenerationSelector();
    this->setRadioButtons();
    this->setButtons();
}

Widget::~Widget(){

}
//Caller is responsible of proper deallocation of the returned maze
int** Widget::getGeneratedMaze(){
    int generatedIndex = rand() % Widget::generatedMazes.size();
    int** originalMaze = Widget::generatedMazes.at(generatedIndex);
    int** mazeCopy = new int*[MazePane::ROW_LENGTH];
    for(int i = 0; i < MazePane::ROW_LENGTH; i++){
        mazeCopy[i] = new int[MazePane::COLUMN_LENGTH];
        for(int j = 0; j < MazePane::COLUMN_LENGTH; j++){
            mazeCopy[i][j] = originalMaze[i][j];
        }
    }
    return mazeCopy;
}
//Static function to retrieve the current radio button selection
int Widget::getRadioSelection(){
    return Widget::currRadioSelection;
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
    this->btAddWall = new QRadioButton(QString::fromStdString("Wall"), this);
    this->btAddEmpty = new QRadioButton(QString::fromStdString("Empty"), this);
    this->btSetSource = new QRadioButton(QString::fromStdString("Source"), this);
    this->btSetTarget = new QRadioButton(QString::fromStdString("Target"), this);
    this->radioButtonGroup = new QButtonGroup(this);

    //set the label
    this->radioLabel = new QLabel(QString::fromStdString("Cell Editor\n(Cilck on cells)"));
    this->radioButtonHolder = new QHBoxLayout(this);
    this->radioButtonHolder->setSpacing(Widget::RADIO_BUTTON_HOLDER_SPACING);
    //add the children of holder into the holder itself
    this->radioButtonHolder->addWidget(this->radioLabel);
    this->radioButtonHolder->addWidget(this->btAddWall);
    this->radioButtonHolder->addWidget(this->btAddEmpty);
    this->radioButtonHolder->addWidget(this->btSetSource);
    this->radioButtonHolder->addWidget(this->btSetTarget);
    //add functionality to radio buttons
    this->radioButtonGroup->addButton(this->btAddWall, 0);
    this->radioButtonGroup->addButton(this->btAddEmpty, 1);
    this->radioButtonGroup->addButton(this->btSetSource, 2);
    this->radioButtonGroup->addButton(this->btSetTarget, 3);
    QObject::connect(this->radioButtonGroup, (&QButtonGroup::buttonToggled), this, (&Widget::radioButtonHandler));

}
void Widget::setButtons(){
    this->btSolve = new QPushButton("Solve");
    this->btRegenerate = new QPushButton("Regenerate");
    QObject::connect(this->btSolve, (&QPushButton::clicked), this, (&Widget::solveHandler));
    QObject::connect(this->btRegenerate, (&QPushButton::clicked), this, (&Widget::regenerateHandler));
}
//Invoke only once during initialization, from constructor
//Selects an auto generated maze initially
void Widget::generateMazes(){
    Widget::generatedMazes = this->mazeGenerator->generateMazes();
    this->currMaze = new MazePane(true);
}
void Widget::generationSelectorHandler(int selectionIndex){
    //o based indexing
    string selectedString;
    if(selectionIndex == 0){ //self generate

    }
    else if(selectionIndex == 1){//auto generate

    }
}
//string Widget::getSelectedRadioButton() const{}
void Widget::solveHandler(bool checked){
    if(this->currMaze != NULL){
        this->currMaze->solve();
    }
}
void Widget::radioButtonHandler(QAbstractButton* button, bool checked){
    if(button == this->btAddEmpty){
        Widget::currRadioSelection = Widget::RADIO_SELECTION_VALUES::RADIO_EMPTY;
    }
    else if(button == this->btAddWall){
        Widget::currRadioSelection = Widget::RADIO_SELECTION_VALUES::RADIO_WALL;
    }
    else if(button == this->btSetSource){
        Widget::currRadioSelection = Widget::RADIO_SELECTION_VALUES::RADIO_SOURCE;
    }
    else if(button == this->btSetTarget){
        Widget::currRadioSelection = Widget::RADIO_SELECTION_VALUES::RADIO_TARGET;
    }
    else{
        cout << "Error occured during radio button selection regarding pointer comparison" << endl;
    }
}
void Widget::regenerateHandler(bool checked){

}
//void Widget::radioButtonToggleHandler(QAbstractButton* button, bool checked){}
const string Widget::SELF_GENERATE_VALUE = "Self Generate";
const string Widget::AUTO_GENERATE_VALUE = "Auto Generate";
int Widget::currRadioSelection = RADIO_SELECTION_VALUES::RADIO_WALL;
vector<int**> Widget::generatedMazes;
