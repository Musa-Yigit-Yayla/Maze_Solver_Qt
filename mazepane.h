#include <cstddef>
#include <QGridLayout>


using namespace std;
#ifndef MAZEPANE_H
#define MAZEPANE_H


class MazePane: public QGridLayout{
Q_OBJECT //object macro
public:
    static const int ROW_LENGTH = 62;
    static const int COLUMN_LENGTH = 36;

private:
    static const int EMPTY_GRID_VALUE = 0;
    static const int VISITED_GRID_VALUE = 1;
    static const int WALL_GRID_VALUE = 2;
    static const int SOLUTION_GRID_VALUE = 3;
    static const int FAILED_GRID_VALUE = 4;
    static const int START_POS_VALUE = 5;
    static const int TARGET_POS_VALUE = 6;

    int** mazeArr = nullptr;
    int rowLength = ROW_LENGTH;
    int columnLength = COLUMN_LENGTH;
    bool isMutable = true;
public:
    MazePane(bool autoGenerate = true);
    ~MazePane();
    void solve();

//public slots:
    //void squareClicked(int row, int column);
};

#endif // MAZEPANE_H
