
#include <QWidget>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QVariant>
#include <QLabel>
#include <string>
#include <QString>
#include <QOverload>
#include <QSizePolicy>
#include "circle.h"
#include <vector>

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
    static const int TRAVERSABLE_MAZE_THRESHOLD = 0;
    static const string SELF_GENERATE_VALUE;
    static const string AUTO_GENERATE_VALUE;
    enum RADIO_SELECTION_VALUES{RADIO_WALL = 0, RADIO_EMPTY, RADIO_SOURCE, RADIO_TARGET};

    //ui constants
    static const int RADIO_BUTTON_HOLDER_SPACING = 20;
    static const int COLOR_PANE_H_GAP = 10;
    static const int COLOR_PANE_V_GAP = 15;
    static const int HBOX1_SPACING = 30;
    static const int HBOX2_SPACING = 30;
    static const int VERTICAL_BOX_SPACING = 45;
    static const int SOLVE_BUTTON_WIDTH = 40;
    static const int REGENERATE_BUTTON_WIDTH = 70;
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
    vector<QLabel*> colorLabels; //labels for explaining colors' meanings
    vector<Circle*> colorCircles;

    QHBoxLayout* radioButtonHolder = nullptr;
    QGridLayout* colorPane = nullptr;
    QVBoxLayout* verticalBox = new QVBoxLayout(this); //high most level container
    QHBoxLayout* hbox1 = nullptr;
    QHBoxLayout* hbox2 = nullptr;

    MazeGenerator* mazeGenerator = nullptr;
    MazePane* currMaze = nullptr;

    static int currRadioSelection;
    static vector<int**> generatedMazes;
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
    void setColorPane();
    void setLayoutManagement();

    void btRegenerateSetVisible(bool visible); //set the visibility status for btRegenerate
signals:

public slots:
    void generationSelectorHandler(int);
    //void radioButtonToggleHandler(QAbstractButton *button, bool checked);
    void solveHandler(bool checked = true);
    void regenerateHandler(bool checked = true);
    void radioButtonHandler(QAbstractButton *button, bool checked);
};
#endif // WIDGET_H
