#include <cstddef>
#include <QGridLayout>


using namespace std;
#ifndef MAZEPANE_H
#define MAZEPANE_H


class MazePane: public QGridLayout{

Q_OBJECT //object macro

private:
    int** mazeArr = nullptr;
    int rowLength = 0;
    int columnLength = 0;
public:
    MazePane();
public slots:
    void squareClicked(int row, int column);
};

#endif // MAZEPANE_H
