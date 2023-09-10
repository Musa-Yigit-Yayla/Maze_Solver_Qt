#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include "mazegenerator.h"
#include "mazepane.h"
//main container which will act like a scene
class Widget : public QWidget{
    Q_OBJECT
private:
    QComboBox generationSelector; //will be used to select maze generation
    MazeGenerator* mazeGenerator = nullptr;
    vector<int**> generatedMazes;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
