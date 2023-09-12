#include <cstddef>
#include <QGridLayout>
#include <stack>
#include "widget.h"

using namespace std;
#ifndef MAZEPANE_H
#define MAZEPANE_H

class MazePane: public QGridLayout{
Q_OBJECT //object macro
public:
    static const int ROW_LENGTH = 36;
    static const int COLUMN_LENGTH = 62;
    static const int EMPTY_GRID_VALUE = 0;
    static const int VISITED_GRID_VALUE = 1;
    static const int WALL_GRID_VALUE = 2;
    static const int SOLUTION_GRID_VALUE = 3;
    static const int FAILED_GRID_VALUE = 4;
    static const int START_POS_VALUE = 5;
    static const int TARGET_POS_VALUE = 6;
private:
    int** mazeArr = nullptr;
    int rowLength = ROW_LENGTH;
    int columnLength = COLUMN_LENGTH;
    int sourceLabelPos = 0;
    int targetLabelPos = this->rowLength * this->columnLength - 1;
    bool isMutable = true; //will be used to determine whether user can alternate the slots of the maze

public:
    MazePane(bool autoGenerate = true);
    ~MazePane();
    void solve();
    void updateMazeArr(); //updates the maze array wrt rectangles' color
    int** getMazeCopy() const;
    int getSourceLabel() const; //returns the source value's label
    int getTargetLabel() const; //returns the target value's label
    void setSourcePos(int row, int column);
    void setTargetPos(int row, int column);
    static bool isTraversable(int** givenMaze, int mazeLength, int columnLength);
private:
    void drawMazePane(); //call from constructor during initialization or when you need to redraw the maze after an update
    void setDefaultMaze();

//public slots:
    //void squareClicked(int row, int column);
};

#endif // MAZEPANE_H
