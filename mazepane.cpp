#include "mazepane.h"
#include "mazegenerator.h"
#include "rectanglewidget.h"

MazePane::MazePane(bool autoGenerate){
    if(autoGenerate){
        //select a pregenerated maze
        this->mazeArr = Widget::getGeneratedMaze();
    }
    else{
        //generate empty maze for editing by user
        this->setDefaultMaze();
    }
    this->setHorizontalSpacing(0);
    this->setVerticalSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
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
            int currState = currRect->getState();
            //!!!YOU MAY HAVE TO ADD ADDITIONAL SWITCH CONDITIONS INSIDE THE SWITCH STATEMENT SUCH AS VISITED VALUE
            this->mazeArr[i][j] = currState;
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
//Sets the prev source as an empty grid
//Does not update the whole matrix but just adjusts the source index
//Given row and column values are assumed to be correct, no validation is performed
void MazePane::setSourcePos(int row, int column){
    int prevSourceLabel = this->sourceLabelPos;
    MazeGenerator mzg(0, 0, 0, 0, 0, 0, 0, 0);
    int prevRow = mzg.getLabelRow(prevSourceLabel);
    int prevColumn = mzg.getLabelColumn(prevSourceLabel);
    this->sourceLabelPos = mzg.getLabel(row, column);
    if(prevSourceLabel != this->sourceLabelPos && this->targetLabelPos != this->sourceLabelPos){
        this->mazeArr[prevRow][prevColumn] = MazePane::EMPTY_GRID_VALUE;
        //retrieve the prev source rectangle widget and set its state to an empty path state
        RectangleWidget* prevSourceRect = reinterpret_cast<RectangleWidget*>(this->itemAtPosition(prevRow, prevColumn));
        prevSourceRect->setState(MazePane::EMPTY_GRID_VALUE);
        //set the new source rectangle's state
        RectangleWidget* newSourceRect = reinterpret_cast<RectangleWidget*>(this->itemAtPosition(row, column));
        this->mazeArr[row][column] = MazePane::START_POS_VALUE;
        newSourceRect->setState(MazePane::START_POS_VALUE);
    }
    else if(this->sourceLabelPos == this->targetLabelPos){
        //swap the source with target
        //retrieve the prevSource and target labels
        RectangleWidget* prevSourceRect = reinterpret_cast<RectangleWidget*>(this->itemAtPosition(prevRow, prevColumn)); //set it as target
        RectangleWidget* prevTargetLabel = reinterpret_cast<RectangleWidget*>(this->itemAtPosition(row, column));

        this->mazeArr[prevRow][prevColumn] = MazePane::TARGET_POS_VALUE;
        this->mazeArr[row][column] = MazePane::START_POS_VALUE;

        //set the rectangles' state
        prevSourceRect->setState(MazePane::TARGET_POS_VALUE);
        prevTargetLabel->setState(MazePane::START_POS_VALUE);
    }
    else{

    }
}
//Sets the prev target as an empty grid
//Works in an identical fashion to setSourcePos
void MazePane::setTargetPos(int row, int column){
    int prevTargetLabel = this->targetLabelPos;
    MazeGenerator mzg(0, 0, 0, 0, 0, 0, 0, 0);
    this->targetLabelPos = mzg.getLabel(row, column);
    if(prevTargetLabel != this->targetLabelPos && this->sourceLabelPos != this->targetLabelPos){
        int prevRow = mzg.getLabelRow(prevTargetLabel);
        int prevColumn = mzg.getLabelColumn(prevTargetLabel);
        this->mazeArr[prevRow][prevColumn] = MazePane::EMPTY_GRID_VALUE;
        //retrieve the prev source rectangle widget and set its state to an empty path state
        RectangleWidget* prevTargetRect = reinterpret_cast<RectangleWidget*>(this->itemAtPosition(prevRow, prevColumn));
        prevTargetRect->setState(MazePane::EMPTY_GRID_VALUE);
        //set the new source rectangle's state
        RectangleWidget* newTargetRect = reinterpret_cast<RectangleWidget*>(this->itemAtPosition(row, column));
        this->mazeArr[row][column] = MazePane::TARGET_POS_VALUE;
        newTargetRect->setState(MazePane::TARGET_POS_VALUE);
    }
    else if(this->sourceLabelPos == this->targetLabelPos){
        //swap the source with target
    }
    else{
        //restore the previous target label position data field and prompt an error message
        this->targetLabelPos = prevTargetLabel;
    }
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
//returns nullptr if given row and column indexes are invalid
//returns a pointer to the exact grid element
QWidget* MazePane::getGridElement(int row, int column) const{
    QWidget* result = nullptr;
    if(row >= 0 && row < this->rowLength && column >= 0 && column < columnLength){
        QLayoutItem* item = this->itemAtPosition(row, column);
        if(item != NULL && item->widget() != NULL){
            result = item->widget();
        }

    }
    return result;
}
//Return the position of the matching widget
//return -1 if not found
int MazePane::getElementLabel(QWidget* widget) const{
    int result = -1;
}
//Draws the whole maze from scratch based on the current mazeArr
//Invoke from constructor
void MazePane::drawMazePane(){
    if(this->mazeArr != NULL){
        for(int i = 0; i < this->rowLength; i++){
            for(int j = 0; j < this->columnLength; j++){
                int currState = this->mazeArr[i][j];
                RectangleWidget* currRect = new RectangleWidget(currState, nullptr, this);
                this->addWidget(currRect, i, j);
            }
        }
        //add spacer items to the gridLayout so as to have our cells next to each other
        QSpacerItem* horizontalSpacer = new QSpacerItem(RectangleWidget::RECTANGLE_WIDTH, RectangleWidget::RECTANGLE_HEIGHT, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QSpacerItem* verticalSpacer = new QSpacerItem(RectangleWidget::RECTANGLE_HEIGHT, RectangleWidget::RECTANGLE_WIDTH, QSizePolicy::Minimum, QSizePolicy::Expanding);

        this->addItem(horizontalSpacer, 0, this->columnLength);
        this->addItem(verticalSpacer, this->rowLength, 0);
    }
    else{
        cout << "Error: Unable to draw mazePane which is null pointer" << endl;
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
