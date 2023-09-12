
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

//#include "mazegenerator.h"
//#include "mazepane.h"
#ifndef WIDGET_H
#define WIDGET_H
/**
 *High-level container class for maze traversal app
 *This class must have one and only one instance during run-time due to use of static fields for increasing accessability
 */
class MazePane;
class MazeGenerator;
using namespace std;
//main container which will act like a scene
class Widget : public QWidget{
    Q_OBJECT
public:
    static const int AUTO_GENERATED_MAZE_COUNT = 50;
    static const int TRAVERSABLE_MAZE_THRESHOLD = 20;
    static const string SELF_GENERATE_VALUE;
    static const string AUTO_GENERATE_VALUE;
    enum RADIO_SELECTION_VALUES{RADIO_WALL, RADIO_EMPTY, RADIO_SOURCE, RADIO_TARGET};

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
    static vector<int**> generatedMazes;
    static int currRadioSelection;
public:
    Widget(QWidget *parent = nullptr);
    static int** getGeneratedMaze(); //return by copy
    static int getRadioSelection();
    //string getSelectedRadioButton() const;
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
    void radioButtonHandler(QAbstractButton *button, bool checked);
};
#endif // WIDGET_H
