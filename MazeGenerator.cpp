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
        int generatedMazeCount = 0;
        while(generatedMazeCount < this->mazeCount){
            cout << "generateMaze main loop" << endl;
            int** currMaze = this->generateMaze();
            if(!this->isThresholdSatisfied){
                //generate a valid traversable maze
                if(MazePane::isTraversable(currMaze, this->rowLength, this->columnLength)){
                    result.push_back(currMaze);
                    generatedMazeCount++;
                    if(generatedMazeCount == this->threshold){
                        this->isThresholdSatisfied = true;
                    }
                }
                else{
                    //deallocate the currMaze and continue
                    for(int i = 0; i < this->rowLength; i++){
                        delete[] currMaze[i];
                    }
                    delete[] currMaze;
                }
            }
            else{
                //generate regardless of traversability
                result.push_back(currMaze);
                generatedMazeCount++;
            }
        }
        return result;
    }
    //Generates a single maze and returns the pointer to it
    int** MazeGenerator::generateMaze() const{
            int** currMaze = new int*[this->rowLength];
            for(int i = 0; i < this->rowLength; i++){
                currMaze[i] = new int[this->columnLength];
            }
            //generate a valid index for the source location
            int source = rand() % this->vertexCount;
            int sourceRow = this->getLabelRow(source);
            int sourceColumn = this->getLabelColumn(source);
            currMaze[sourceRow][sourceColumn] = this->sourceLabelValue;
            //generate a random destination but ensure that we do not have it directly adjacent to the source
            vector<int> sourceAdj = this->getAdjLabels(source);
            int target = rand() % this->vertexCount;
            while(count(sourceAdj.begin(), sourceAdj.end(), target) != 0 || target == source){
                target = rand() % this->vertexCount;
            }
            int targetRow = this->getLabelRow(target);
            int targetColumn = this->getLabelColumn(target);
            currMaze[targetRow][targetColumn] = this->targetLabelValue;
            //fill the rest of the maze with walls and empty
            for(int i = 0; i < this->rowLength; i++){
                for(int j = 0; j < this->columnLength; j++){
                    if((i == sourceRow && j == sourceColumn) || (targetRow == i && targetColumn == j)){
                        continue;
                    }
                    //the logic for placing a wall is if we have a wall in an adjacent pos, we simply have 7/10 chance of having a wall placed
                    //else we have 1/2 chance of placing a wall
                    int currLabel = this->getLabel(i, j);
                    vector<int> currAdj = this->getAdjLabels(currLabel);
                    double generationProportion = 0.5; //proportion for generating a wall
                    for(int adjLabel: currAdj){
                        int adjLabelRow = this->getLabelRow(adjLabel);
                        int adjLabelColumn = this->getLabelColumn(adjLabel);
                        if(currMaze[adjLabelRow][adjLabelColumn] == this->wallLabelValue){
                            generationProportion = 0.7;
                        }
                    }
                    const int divisor = 10;
                    const int generationConstant = generationProportion * 10;
                    bool generateWall = false;
                    if(rand() % divisor < generationConstant){
                        //set the current label as a wall
                        currMaze[i][j] = this->wallLabelValue;
                    }
                    else{
                        //set it as an empty slot which can be used as a path
                        currMaze[i][j] = this->emptyLabelValue;
                    }
                }
            }
            return currMaze;
    }

    int MazeGenerator::getRowLength() const{
        return this->rowLength;
    }
    int MazeGenerator::getColumnLength() const{
        return this->columnLength;
    }
    vector<int> MazeGenerator::getAdjLabels( int label) const{
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
    int MazeGenerator::getLabel(int row, int column) const{
        return row * this->columnLength + column;
    }
    int MazeGenerator::getLabelRow(int label) const{
        return label / this->columnLength;
    }
    int MazeGenerator::getLabelColumn(int label) const{
        return label % this->columnLength;
    }
    int MazeGenerator::getSourceLValue() const{
        return this->sourceLabelValue;
    }
    int MazeGenerator::getTargetLValue() const{
        return this->targetLabelValue;
    }
    int MazeGenerator::getWallLValue() const{
        return this->wallLabelValue;
    }
    int MazeGenerator::getEmptyLValue() const{
        return this->emptyLabelValue;
    }



