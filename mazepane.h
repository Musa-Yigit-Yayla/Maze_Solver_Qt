#include <cstddef>
#include <QGridLayout>
#include <QSpacerItem>
#include <stack>
#include <unordered_set>
#include <vector>
#include <QTimer>
#include "widget.h"
#include <cmath>

using namespace std;
#ifndef MAZEPANE_H
#define MAZEPANE_H

class MazePane: public QGridLayout{
Q_OBJECT //object macro
public:
    static const int ROW_LENGTH = 12;
    static const int COLUMN_LENGTH = 16;
    static const int EMPTY_GRID_VALUE = 1;
    static const int VISITED_GRID_VALUE = 4;
    static const int WALL_GRID_VALUE = 0;
    static const int SOLUTION_GRID_VALUE = 5;
    static const int FAILED_GRID_VALUE = 6;
    static const int START_POS_VALUE = 2;
    static const int TARGET_POS_VALUE = 3;
    static const int FAILED_TIMER_MILLIS = 125;
    static const int SOLVED_TIMER_MILLIS = 200;
    static const int MAX_WEIGHT = pow(10, 6) + 1; //max weight possible between two vertices to avoid overflow
private:
    int** mazeArr = nullptr;
    int rowLength = ROW_LENGTH;
    int columnLength = COLUMN_LENGTH;
    int sourceLabelPos = 0;
    int targetLabelPos = this->rowLength * this->columnLength - 1;
    bool isMutable = true; //will be used to determine whether user can alternate the slots of the maze
    QTimer* solveTimer = nullptr;

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
    static vector<int> getSourceTargetLabels(int* givenMaze[], int rowLength, int columnLength);
    QWidget* getGridElement(int row, int column) const; //return pointer to the grid element
    int getElementLabel(QWidget* widget) const;
    void printMaze() const;
    bool getIsMutable() const;
    bool isAdjacent( int label1, int label2) const;
    int getSmallestAdjacentLabel(const int weights[], const int vertexCount, const int currLabel, const vector<int>& solutionPath) const;
    static int getSmallestUnvisitedLabel(const unordered_set<int>& vertexSet, const int weights[], const int vertexCount);
private:
    void drawMazePane(); //call from constructor during initialization or when you need to redraw the maze after an update
    void setDefaultMaze();
    void visualizeSolution(bool isSolved, const int weights[], int* adjMatrix[], const int vertexCount); //visualize the solution
    void bfsMoveables(vector<vector<int>>& prioritizedVertices) const;

public slots:
    //void squareClicked(int row, int column);
    void solveTimerSlot();
};

#endif // MAZEPANE_H
