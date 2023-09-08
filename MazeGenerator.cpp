#include "mazegenerator.h"
/**
* The purpose of this program is to generate a certain number of 2d mazes of certain row and column size which apply to all.
* Generated mazes will be 2D matrices
* A threshold will exist which ensures that there will be at least a certain number of matrices which can be solved using graph traversal.
* This program will be used as a module for a larger size maze traversal program which is undergoing implementation with Qt.
*/

using namespace std;
    MazeGenerator::MazeGenerator(int threshold, int mazeCount, int rowLength, int columnLength, int slv, int tlv, int elv, int wlv):
    sourceLabelValue(slv), targetLabelValue(tlv), emptyLabelValue(elv), wallLabelValue(wlv){
        if(mazeCount >= threshold && rowLength > 0 && columnLength > 0){
            this->threshold = threshold;
            this->mazeCount = mazeCount;
            if(this->threshold == 0){
                this->isThresholdSatisfied = true;
            }
            this->rowLength = rowLength;
            this->columnLength = columnLength;
            this->vertexCount = this->rowLength * this->columnLength;
        }
        else{
            cout << "Error: given parameters during MazeGenerator instantiation are not suitable" << endl;
        }
    }

    vector<int**> MazeGenerator::generateMazes(){
        vector<int**> result;
        for(int i = 0; i < this->mazeCount; i++){
            const int* const currColumnLength = const_cast<int*>(& (this->columnLength));
            const int* const currRowLength = const_cast<int*>(& (this->rowLength));
            int** currMaze = new int[*currRowLength][*currColumnLength];
            //generate a valid index for the source location
            int source = rand() % this->vertexCount;
            int sourceRow = this->getLabelRow(source);
            int sourceColumn = this->getLabelColumn(source);
            currMaze[sourceRow][sourceColumn] = this->sourceLabelValue;
        }
    }
    vector<int> MazeGenerator::getAdjLabels( int label){
        int labelRow = this->getLabelRow(label);
        int labelColumn = this->getLabelColumn(label);

        vector<int> result;

        //check top vertex
        int currRow = labelRow - 1;
        int currColumn = labelColumn;

        int nextLabel;

        if(currRow >= 0){
            nextLabel = this->getLabel(currRow, currColumn);
            result.push_back(nextLabel);
        }
        //check left
        currRow =  labelRow;
        currColumn = labelColumn - 1;
        if(currColumn  >= 0){
            nextLabel = this->getLabel(currRow, currColumn);
            result.push_back(nextLabel);
        }
        //check bottom
        currRow =  labelRow + 1;
        currColumn = labelColumn;
        if(currRow < this->rowLength){
            nextLabel = this->getLabel(currRow, currColumn);
            result.push_back(nextLabel);
        }
        //check right
        currRow =  labelRow;
        currColumn = labelColumn + 1;
        if(currColumn < this->columnLength){
            nextLabel = this->getLabel(currRow, currColumn);
            result.push_back(nextLabel);
        }
        return result;
    }
    int MazeGenerator::getLabel(int row, int column){
        return row * this->columnLength + column;
    }
    int MazeGenerator::getLabelRow(int label){
        return label / this->columnLength;
    }
    int MazeGenerator::getLabelColumn(int label){
        return label % this->columnLength;
    }


