#include <vector>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <algorithm>


#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H
/**
* The purpose of this program is to generate a certain number of 2d mazes of certain row and column size which apply to all.
* Generated mazes will be 2D matrices
* A threshold will exist which ensures that there will be at least a certain number of matrices which can be solved using graph traversal.
* This program will be used as a module for a larger size maze traversal program which is undergoing implementation with Qt.
*/

using namespace std;
class MazeGenerator{
private:
    bool isThresholdSatisfied = false; //set to true when the given number of traversable matrices are generated
    int mazeCount = 0;
    int threshold = 0;
    int rowLength = 0;
    int columnLength = 0;
    int vertexCount = 0;

    int sourceLabelValue = 0;
    int targetLabelValue = 0;
    int emptyLabelValue =  0;
    int wallLabelValue = 0;
public:
    MazeGenerator(int threshold, int mazeCount, int rowLength, int columnLength, int slv, int tlv, int elv, int wlv);
    vector<int**> generateMazes() const;
    int** generateMaze() const;
    int getRowLength() const;
    int getColumnLength() const;
    int getSourceLValue() const;
    int getTargetLValue() const;
    int getWallLValue() const;
    int getEmptyLValue() const;
    vector<int> getAdjLabels( int label) const;
    int getLabel(int row, int column) const;
    int getLabelRow(int label) const;
    int getLabelColumn(int label) const;
};



#endif
