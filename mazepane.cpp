#include "mazepane.h"
#include "mazegenerator.h"

MazePane::MazePane(bool autoGenerate){

}
MazePane::~MazePane(){

}
void MazePane::solve(){

}
//Returns a deep copy of the current maze
//User is responsible of deallocating the returned copy of the maze
int** MazePane::getMazeCopy() const{
    int** result = nullptr;
    if(this->mazeArr != NULL && this->columnLength > 0 && this->rowLength > 0){
        result = new int*[this->rowLength];
        for(int i = 0; i < this->rowLength; i++){
            result[i] = new int[this->columnLength];
            for(int j = 0; j < this->columnLength; j++){
                result[i][j] = this->mazeArr[i][j];;
            }
        }
    }
    return result;

}
//static function to check whether a given maze, which is a valid maze, can be solved
//given maze is assumed to be valid
//Column length refers t onumber of columns in a row
bool MazePane::isTraversable(int** givenMaze, int mazeLength, int columnLength){
    //retrieve the source and target positions with the help of mazeGenerator methods
    int sourceLabel = 0;
    int targetLabel = 0;
    bool result = false;

    bool sourceFound = false;
    bool targetFound = false;
    if(givenMaze != NULL){
        MazeGenerator mg(0, 0, mazeLength, columnLength, 0, 0, 0, 0);
        for(int i = 0; i < mazeLength; i++){
            bool broken = false;
            for(int j = 0; j < columnLength; j++){
                if(givenMaze[i][j] == MazePane::START_POS_VALUE){
                    sourceLabel = mg.getLabel(i, j);
                    sourceFound = true;
                }
                else if(givenMaze[i][j] == MazePane::TARGET_POS_VALUE){
                    targetLabel = mg.getLabel(i, j);
                    targetFound = true;
                }
                if(sourceFound && targetFound){
                    broken = true;
                    break;
                }
            }
            if(broken){
                break;
            }
        }
    }
    if(sourceFound && targetFound){
        //create and adjacency list from the givenMaze and apply dfs to check whether we can solve the maze
        vector<int, int>
    }
}
