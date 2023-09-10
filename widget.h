#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>

#include "mazegenerator.h"
#include "mazepane.h"
//main container which will act like a scene
class Widget : public QWidget{
    Q_OBJECT
public:
    static const int AUTO_GENERATED_MAZE_COUNT = 50;
    static const int TRAVERSABLE_MAZE_THRESHOLD = 20;
private:
    QComboBox generationSelector; //will be used to select maze generation procedure
    QRadioButton* btAddWall = nullptr;
    QRadioButton* btAddEmpty = nullptr;
    QRadioButton* btSetSource = nullptr;
    QRadioButton* btSetTarget = nullptr;
    QButtonGroup* radioButtonGroup = nullptr;
    QPushButton* btRegenerate = nullptr; //regenerate a new maze, will be visible when auto generate is selected from combobox
    QPushButton* btSolve = nullptr;

    MazeGenerator* mazeGenerator = nullptr;
    vector<int**> generatedMazes;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    void setGenerationSelector();
    void setRadioButtons();
    void setButtons();
signals:

public slots:

};
#endif // WIDGET_H
