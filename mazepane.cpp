#include "mazepane.h"
#include "mazegenerator.h"

MazePane::MazePane(bool autoGenerate){
    if(autoGenerate){
        //select a pregenerated maze
        this->mazeArr = Widget::getGeneratedMaze();
    }
    else{
        //generate empty maze for editing by user
        this->setDefaultMaze();
    }
    this->drawMazePane();
}
MazePane::~MazePane(){

}
void MazePane::solve(){

}
//Updates the mazeArr with respect to current colors of rectangles
void MazePane::updateMazeArr(){
    for(int i = 0; i < this->rowLength; i++){
        for(int j = 0; j < this->columnLength; j++){
            QLayoutItem* item = this->itemAtPosition(i, j);
            RectangleWidget* currRect = reinterpret_cast<RectangleWidget*>(item);
        }
    }
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
//Returns based on the mazeArr, therefore you must keep mazeArr updated during program execution
//Returns -1 if source does not exist
int MazePane::getSourceLabel() const{
    if(this->mazeArr != NULL){
        for(int i = 0; i < this->rowLength; i++){
            for(int j = 0; j < this->columnLength; j++){
                if(this->mazeArr[i][j] == MazePane::START_POS_VALUE){
                    MazeGenerator mzg(0, 0, 0, 0, 0, 0, 0, 0);
                    return mzg.getLabel(i, j);
                }
            }
        }
    }
    else{
        cout << "mazeArr field has been observed as NULL in getSourceLabel" << endl;
    }
    return -1;
}
int MazePane::getTargetLabel() const{
    if(this->mazeArr != NULL){
        for(int i = 0; i < this->rowLength; i++){
            for(int j = 0; j < this->columnLength; j++){
                if(this->mazeArr[i][j] == MazePane::TARGET_POS_VALUE){
                    MazeGenerator mzg(0, 0, 0, 0, 0, 0, 0, 0);
                    return mzg.getLabel(i, j);
                }
            }
        }
    }
    else{
        cout << "mazeArr field has been observed as NULL in getTargetLabel" << endl;
    }
    return -1;
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
        if(sourceFound && targetFound){
            //create an adjacency list from the givenMaze and apply dfs to check whether we can solve the maze
            vector<vector<int>> adjList;
            for(int i = 0; i < mazeLength; i++){
                for(int j = 0; j < columnLength; j++){
                    int currLabel = mg.getLabel(i, j);
                    vector<int> currAdj;
                    if(givenMaze[i][j] != MazePane::WALL_GRID_VALUE){
                        currAdj = mg.getAdjLabels(currLabel);
                    }
                    adjList.push_back(currAdj);
                }
            }
            //notice that adjList also contains walls as adjacent labels to moveable labels
            //apply iterative dfs algorithm
            bool visit[mazeLength * columnLength];
            for(int i = 0; i < mazeLength * columnLength; i++){
                int currLabelRow = mg.getLabelRow(i);
                int currLabelColumn = mg.getLabelColumn(i);
                if(givenMaze[currLabelRow][currLabelColumn] == MazePane::WALL_GRID_VALUE){
                    visit[i] = true;
                }
                else{
                    visit[i] = false;
                }
            }
            stack<int> s;
            //mark the source vertex as visited and push it onto the stack
            visit[sourceLabel] = true;
            s.push(sourceLabel);
            while(!s.empty()){
                int top = s.top();
                s.pop();

                vector<int> currAdj = adjList.at(top);
                vector<int> validAdj;
                for(int adjLabel: currAdj){
                    if(adjLabel == MazePane::EMPTY_GRID_VALUE || adjLabel == targetLabel){
                        validAdj.push_back(adjLabel);
                    }
                }
                for(int validLabel: validAdj){
                    if(validLabel == targetLabel){
                        //If the given maze is traversable this block will be executed eventually
                        //return true
                        return true;
                    }
                    else if(!visit[validLabel]){
                        visit[validLabel] = true;
                        s.push(validLabel);
                    }
                }
            }
        }
    }
    return result;
}
//Draws the whole maze from scratch based on the current mazeArr
//Invoke from constructor
void MazePane::drawMazePane(){
    if(this->mazeArr != NULL){

    }
}
//Sets the current maze as an empty maze to be edited by user, which has target as label 0 and destination as last label and rest as empty
//Does not draw the maze, only sets the int** mazeArr datafield
void MazePane::setDefaultMaze(){
    if(this->mazeArr != NULL){
        for(int i = 0; i < this->rowLength; i++){
            delete[] this->mazeArr[i];
        }
        delete[] this->mazeArr;
        this->mazeArr = nullptr;
    }
    this->mazeArr = new int*[MazePane::ROW_LENGTH];
    for(int i = 0; i < this->columnLength; i++){
        for(int j = 0; j < this->columnLength; j++){
            mazeArr[i][j] = EMPTY_GRID_VALUE;
        }
    }
    //set the source and target manually
    mazeArr[0][0] = START_POS_VALUE;
    mazeArr[this->rowLength - 1][this->columnLength - 1] = TARGET_POS_VALUE;
}
