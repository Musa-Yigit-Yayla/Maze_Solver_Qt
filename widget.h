#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVariant>
#include <QLabel>
#include <string>
#include <QString>
#include <QOverload>

#include "mazegenerator.h"
#include "mazepane.h"

using namespace std;
//main container which will act like a scene
class Widget : public QWidget{
    Q_OBJECT
public:
    static const int AUTO_GENERATED_MAZE_COUNT = 50;
    static const int TRAVERSABLE_MAZE_THRESHOLD = 20;
    static const string SELF_GENERATE_VALUE;
    static const string AUTO_GENERATE_VALUE;

    //ui constants
    static const int RADIO_BUTTON_HOLDER_SPACING = 40;
private:
    QComboBox* generationSelector = nullptr; //will be used to select maze generation procedure
    QRadioButton* btAddWall = nullptr;
    QRadioButton* btAddEmpty = nullptr;
    QRadioButton* btSetSource = nullptr;
    QRadioButton* btSetTarget = nullptr;
    QButtonGroup* radioButtonGroup = nullptr;
    QPushButton* btRegenerate = nullptr; //regenerate a new maze, will be visible when auto generate is selected from combobox
    QPushButton* btSolve = nullptr;
    QLabel* radioLabel = nullptr; //label for explaining radio button functionalities

    QHBoxLayout* radioButtonHolder = nullptr;


    MazeGenerator* mazeGenerator = nullptr;
    MazePane* currMaze = nullptr;
    vector<int**> generatedMazes;
public:
    Widget(QWidget *parent = nullptr);
    int** getGeneratedMaze() const; //return by copy
    ~Widget();
private:
    void generateMazes();
    void setGenerationSelector();
    void setRadioButtons();
    void setButtons();
signals:

public slots:
    void generationSelectorHandler(int);
    //void radioButtonToggleHandler(QAbstractButton *button, bool checked);
    void solveHandler(bool checked = true);
    void regenerateHandler(bool checked = true);
};
#endif // WIDGET_H
