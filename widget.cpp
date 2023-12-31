#include "widget.h"
#include "mazegenerator.h"
#include "mazepane.h"
#include "rectanglewidget.h"
#include <iostream>

using namespace std;

Widget::Widget(QWidget *parent) : QWidget(parent){
    cout << "Widget constructor invoked" << endl;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(Widget::WIDGET_WIDTH, Widget::WIDGET_HEIGHT);
    qDebug() << "Horizontal Policy:" << this->sizePolicy().horizontalPolicy();
    this->setParent(parent);
    this->mazeGenerator = new MazeGenerator(Widget::TRAVERSABLE_MAZE_THRESHOLD, Widget::AUTO_GENERATED_MAZE_COUNT, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, MazePane::START_POS_VALUE, MazePane::TARGET_POS_VALUE, MazePane::EMPTY_GRID_VALUE, MazePane::WALL_GRID_VALUE);
    this->generateMazes(); //problem occurs here
    this->setGenerationSelector();
    this->setRadioButtons();
    this->setButtons();
    this->setColorPane();
    this->setLayoutManagement();
    cout << "Widget constructor exited" << endl;
}

Widget::~Widget(){

}
void Widget::setLayoutManagement(){
    //this->verticalBox = new QVBoxLayout(this);
    //set hbox1
    this->hbox1 = new QHBoxLayout();
    this->hbox1->addLayout(this->radioButtonHolder);
    this->radioButtonHolder->addWidget(this->btSolve);
    this->hbox1->addWidget(this->btRegenerate); //btRegen is visible initially
    this->radioButtonHolder->addStretch();
    //this->hbox1->addWidget(this->btSolve);
    //QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum); //this spacer will push the elements preceding it
    //towards left, add it to the hbox1
    //this->hbox1->addSpacerItem(spacer);
    this->hbox1->addStretch();
    this->hbox1->addWidget(this->generationSelector);
    this->hbox1->setSpacing(Widget::HBOX1_SPACING);
    //set hbox2
    this->hbox2 = new QHBoxLayout();
    this->hbox2->addLayout(this->colorPane);

    this->hbox2->addStretch(0);
    this->hbox2->setSpacing(Widget::HBOX2_SPACING);

    this->verticalBox->addLayout(this->hbox1);
    this->verticalBox->addLayout(this->hbox2);
    this->verticalBox->addLayout(this->currMaze);
    this->verticalBox->setSpacing(Widget::VERTICAL_BOX_SPACING);
    this->verticalBox->addStretch();
    //add the vertical box to the widget itself
    this->setLayout(this->verticalBox);
    //set each child resizable property to fixed
    this->generationSelector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->btAddWall->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->btAddEmpty->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->btSetSource->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->btSetTarget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->btRegenerate->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->btSolve->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->btReset->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->radioLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    for(QLabel* currLabel: this->colorLabels){
        currLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    for(Circle* currCircle: this->colorCircles){
        currCircle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

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
    this->generationSelector = new QComboBox();
    QString s1 = QString::fromStdString(Widget::SELF_GENERATE_VALUE);
    QString s2 = QString::fromStdString(Widget::AUTO_GENERATE_VALUE);
    this->generationSelector->addItem(s1, QVariant(1));
    this->generationSelector->addItem(s2, QVariant(2));
    //set the initial selected option to item 2
    this->generationSelector->setCurrentIndex(1);
    //connect the signal of QComboBox to the slot we have created
    QObject::connect(this->generationSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, (&Widget::generationSelectorHandler));
}
void Widget::setRadioButtons(){
    this->btAddWall = new QRadioButton(QString::fromStdString("Wall"));
    this->btAddEmpty = new QRadioButton(QString::fromStdString("Empty"));
    this->btSetSource = new QRadioButton(QString::fromStdString("Source"));
    this->btSetTarget = new QRadioButton(QString::fromStdString("Target"));
    this->radioButtonGroup = new QButtonGroup(this);

    //set the label
    this->radioLabel = new QLabel(QString::fromStdString("Cell Editor\n(Cilck on cells)"));
    this->radioButtonHolder = new QHBoxLayout(this);
    //this->radioButtonHolder->setSpacing(Widget::RADIO_BUTTON_HOLDER_SPACING);
    //Create a fixed spacer for the radioButtonHolder
    //QSpacerItem* spacer = new QSpacerItem(40, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
    //this->radioButtonHolder->addSpacerItem(spacer);
    //add the children of holder into the holder itself
    this->radioLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->radioButtonHolder->addWidget(this->radioLabel);
    //this->btAddWall->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->radioButtonHolder->addWidget(this->btAddWall);
    //this->btAddEmpty->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->radioButtonHolder->addWidget(this->btAddEmpty);
    //this->btSetSource->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->radioButtonHolder->addWidget(this->btSetSource);
    //this->btSetTarget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->radioButtonHolder->addWidget(this->btSetTarget);
    /*this->radioButtonHolder->addWidget(this->btSolve);
    this->radioButtonHolder->addStretch();*/
    //add functionality to radio buttons
    this->radioButtonGroup->addButton(this->btAddWall, 0);
    this->radioButtonGroup->addButton(this->btAddEmpty, 1);
    this->radioButtonGroup->addButton(this->btSetSource, 2);
    this->radioButtonGroup->addButton(this->btSetTarget, 3);
    //connect using appropriate signals and slots
    QObject::connect(this->radioButtonGroup, (&QButtonGroup::buttonToggled), this, (&Widget::radioButtonHandler));
    //set the layout and ui elements
}
void Widget::setButtons(){
    this->btSolve = new QPushButton("Solve");
    this->btSolve->setFixedWidth(Widget::SOLVE_BUTTON_WIDTH);
    this->btRegenerate = new QPushButton("Regenerate");
    this->btRegenerate->setFixedWidth(Widget::REGENERATE_BUTTON_WIDTH);
    this->btReset = new QPushButton("Reset");
    QObject::connect(this->btSolve, (&QPushButton::clicked), this, (&Widget::solveHandler));
    QObject::connect(this->btRegenerate, (&QPushButton::clicked), this, (&Widget::regenerateHandler));
    QObject::connect(this->btReset, (&QPushButton::clicked), this, (&Widget::resetHandler));
}
void Widget::setColorPane(){
    this->colorPane = new QGridLayout(this);

    //this->colorCircles.push_back(Circle(RectangleWidget::SOURCE_COLOR));
    Circle* c1 = new Circle(RectangleWidget::SOURCE_COLOR);
    Circle* c2 = new Circle(RectangleWidget::TARGET_COLOR);
    Circle* c3 = new Circle(RectangleWidget::DARK_BLUE_COLOR_SET.at(0));
    Circle* c4 = new Circle(RectangleWidget::SOLUTION_COLOR);
    Circle* c5 = new Circle(RectangleWidget::RED_COLOR_SET.at(0));
    Circle* c6 = new Circle(RectangleWidget::WALL_COLOR);
    Circle* c7 = new Circle(RectangleWidget::EMPTY_COLOR);

    this->colorCircles = {c1, c2, c3, c4, c5, c6, c7};
    /*this->colorCircles = {Circle(RectangleWidget::SOURCE_COLOR), Circle(RectangleWidget::TARGET_COLOR), Circle(RectangleWidget::DARK_BLUE_COLOR_SET.at(0)),
                          Circle(RectangleWidget::SOLUTION_COLOR), Circle(RectangleWidget::RED_COLOR_SET.at(0)), Circle(RectangleWidget::WALL_COLOR),
                          Circle(RectangleWidget::EMPTY_COLOR)};*/

    QLabel* l1 = new QLabel(QString::fromStdString("Start \nCell"));
    QLabel* l2 = new QLabel(QString::fromStdString("Target \nCell"));
    QLabel* l3 = new QLabel(QString::fromStdString("Visited"));
    QLabel* l4 = new QLabel(QString::fromStdString("Solution \nPath"));
    QLabel* l5 = new QLabel(QString::fromStdString("Failed \nPath"));
    QLabel* l6 = new QLabel(QString::fromStdString("Wall"));
    QLabel* l7 = new QLabel(QString::fromStdString("Empty \nCell"));

    this->colorPane->setHorizontalSpacing(Widget::COLOR_PANE_H_GAP);
    this->colorPane->setVerticalSpacing(Widget::COLOR_PANE_V_GAP);
    this->colorPane->setContentsMargins(0, 0, 0, 0);

    /*this->colorLabels = {QLabel(QString::fromStdString("Start Cell")), QLabel(QString::fromStdString("Target Cell")), QLabel(QString::fromStdString("Visited")),
                         QLabel(QString::fromStdString("Solution Path")), QLabel(QString::fromStdString("Failed Path")), QLabel(QString::fromStdString("Wall")),
                         QLabel(QString::fromStdString("Empty Cell"))};*/
    this->colorLabels = {l1, l2, l3, l4, l5, l6, l7};
    //set the first row of the colorPane as the circles
    for(int i = 0; i < this->colorCircles.size(); i++){
        Circle* currCircle = (this->colorCircles.at(i));
        currCircle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        //currCircle->setParent(this);
        this->colorPane->addWidget(currCircle, 0, i, Qt::AlignCenter);
        //this->colorPane->addWidget(dynamic_cast<QWidget*>(currCircle), 0, i);
    }
    //set the second row as the labels
    for(int i = 0; i < this->colorLabels.size(); i++){
        QLabel* currLabel = (this->colorLabels.at(i));
        //this->colorPane->addItem(dynamic_cast<QLayoutItem*>(currLabel), 1, i);
        currLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        this->colorPane->addWidget(currLabel, 1, i, Qt::AlignCenter);
    }
    this->colorPane->addWidget(this->btRegenerate);
    //this->colorPane->addStretch();
}
//Makes the btRegenerate visible if @param visible is true
//Else makes it invisible
void Widget::btRegenerateSetVisible(bool visible){
    this->btRegenerate->setVisible(visible);
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
    if(selectionIndex == 0){ //self generate, user generates
        //remove the regenerate button from the pane
        this->btRegenerateSetVisible(false);
        this->btReset->setVisible(true);
        cout << "Debug: selecting self generate in generationSelectorHandler" << endl;
        //this->hbox1->replaceWidget(this->btRegenerate, this->btReset);
        this->hbox1->removeItem(this->radioButtonHolder);
        this->hbox1->removeWidget(this->btSolve);
        this->hbox1->removeWidget(this->btRegenerate);
        this->hbox1->removeWidget(this->generationSelector);
        //add the widgets with their initial order replacing push buttons
        this->hbox1->insertItem(this->hbox1->count() - 1, this->radioButtonHolder);
        this->hbox1->insertWidget(this->hbox1->count() - 1, this->btSolve);
        this->hbox1->insertWidget(this->hbox1->count() - 1, this->btReset);
        //this->hbox1->addStretch();
        this->hbox1->insertWidget(this->hbox1->count(), this->generationSelector);
    }
    else if(selectionIndex == 1){//auto generate
        this->btRegenerateSetVisible(true);
        this->btReset->setVisible(false);
        cout << "Debug: selecting auto generate in generationSelectorHandler" << endl;
        this->hbox1->removeItem(this->radioButtonHolder);
        this->hbox1->removeWidget(this->btSolve);
        this->hbox1->removeWidget(this->btReset);
        this->hbox1->removeWidget(this->generationSelector);
        //add the widgets with their initial order replacing push buttons
        this->hbox1->insertItem(this->hbox1->count() - 1, this->radioButtonHolder);
        this->hbox1->insertWidget(this->hbox1->count() - 1, this->btSolve);
        this->hbox1->insertWidget(this->hbox1->count() - 1, this->btRegenerate);
        //this->hbox1->addStretch();
        this->hbox1->insertWidget(this->hbox1->count(), this->generationSelector);
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
    //retrieve another maze
    int** newMaze = this->generatedMazes.at(rand() % this->generatedMazes.size());
    //remove the previous mazepane from its container
    this->verticalBox->removeItem(this->currMaze);
    //deallocate the previous mazepane
    delete this->currMaze;
    //set the newMaze
    this->currMaze = new MazePane(true);
    //add the new maze pane onto our vbox
    this->verticalBox->addLayout(this->currMaze);
}
void Widget::resetHandler(bool checked){
    //create a brand new thoroughly empty maze
    this->verticalBox->removeItem(this->currMaze);
    delete this->currMaze;
    cout << "Debug: currMaze deletion has been made in the resetHandler" << endl;
    this->currMaze = new MazePane(false);
    cout << "Debug: new currMaze has been created in the resetHandler" << endl;
    this->verticalBox->addLayout(this->currMaze);
    cout << "Debug: new currMaze has been added in the resetHandler" << endl;
}
//protected visibility
//Ignores the size change of our main widget
void Widget::resizeEvent(QResizeEvent* event){
    event->ignore();
}
//void Widget::radioButtonToggleHandler(QAbstractButton* button, bool checked){}
const string Widget::SELF_GENERATE_VALUE = "Self Generate";
const string Widget::AUTO_GENERATE_VALUE = "Auto Generate";
int Widget::currRadioSelection = RADIO_SELECTION_VALUES::RADIO_WALL;
vector<int**> Widget::generatedMazes;
