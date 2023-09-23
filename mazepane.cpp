#include "mazepane.h"
#include "mazegenerator.h"
#include "rectanglewidget.h"
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <algorithm>

using namespace std;

MazePane::MazePane(bool autoGenerate){
    if(autoGenerate){
        //select a pregenerated maze
        this->mazeArr = Widget::getGeneratedMaze();
        //set the source and target position data fields and keep track of them when they are alternated
        vector<int> sourceTarPositions = MazePane::getSourceTargetLabels(this->mazeArr, this->rowLength, this->columnLength);
        if(sourceTarPositions.size() > 0){
            this->sourceLabelPos = sourceTarPositions.at(0);
            this->targetLabelPos = sourceTarPositions.at(1);
        }

    }
    else{
        //generate empty maze for editing by user
        this->setDefaultMaze();
    }
    //initialize QTimer object and connect it to corresponding slot
    this->solveTimer = new QTimer(this);
    QObject::connect(this->solveTimer, (&QTimer::timeout), this, (&MazePane::solveTimerSlot));
    this->setHorizontalSpacing(0);
    this->setVerticalSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
    this->drawMazePane();
}
MazePane::~MazePane(){

}
void MazePane::solve(){
    //as soon as the maze is being solved we set the isMutable to false
    this->isMutable = false;
    //apply Dijkstra's algorithm where source label is the origin vertex and retrieve min distance to each empty cell and the target cell
    //if the distance to target cell is not infinity, visualize the path after having done backtracking and also visualize visited cells
    //otherwise paint cells stemming from source to failed state
    //2d maze resembles a unweighted and undirected graph

    //initialize adjacency matrix
    int vertexCount = this->rowLength * this->columnLength;
    int** adjMatrix = new int*[vertexCount];
    for(int i = 0; i < vertexCount; i++){
        adjMatrix[i] = new int[vertexCount];
    }

    MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
    for(int i = 0; i < vertexCount; i++){
        int currLabelRow = mzg.getLabelRow(i);
        int currLabelColumn = mzg.getLabelColumn(i);
        if(this->mazeArr[currLabelRow][currLabelColumn] == MazePane::WALL_GRID_VALUE){
            //continue as we cannot be on a wall hence there is no need to compute travel cost to its adjacent square
            //!!!PROBLEM MIGHT BE STEMMING FROM HERE
            continue;
        }
        for(int j = 0; j < vertexCount; j++){
            if(this->isAdjacent(i, j)){
                int adjRow = mzg.getLabelRow(j);
                int adjColumn = mzg.getLabelColumn(j);
                if(this->mazeArr[adjRow][adjColumn] == MazePane::WALL_GRID_VALUE){
                    //set to int max as it's a wall
                    adjMatrix[i][j] = INT_MAX;
                }
                else{
                    //set it to 1
                    adjMatrix[i][j] = 1;
                }
            }
            else if(i == j){
                adjMatrix[i][j] = 0;
            }
            else{
                adjMatrix[i][j] = INT_MAX;
            }
        }
    }
    //apply dijktra's shortest path algorithm

    int originVertex = this->getSourceLabel();
    unordered_set<int> vertexSet;
    vertexSet.insert(originVertex);

    int weight[vertexCount]; //minimum total weight of traveling to each moveable cell starting from origin vertex
    for(int i = 0; i < vertexCount; i++){
        weight[i] = INT_MAX;
    }
    for(int i = 0; i < vertexCount; i++){
        weight[i] = adjMatrix[originVertex][i];
    }

    //apply 2nd step
    for(int i = 0; i < vertexCount - 1; i++){
        //find the smallest weighted vertex v which is not in vertex set and which is not a wall
        int v = MazePane::getSmallestUnvisitedLabel(vertexSet, weight, vertexCount);
        vertexSet.insert(v); //add v to the vertex set
        if(v != -1){
            //check weight j for all vertices not included in vertex set
            for(int j = 0; j < vertexCount; j++){
                if(vertexSet.count(j) == 0){
                    if((weight[v] != INT_MAX && adjMatrix[v][j] != INT_MAX) && weight[j] > weight[v] + adjMatrix[v][j]){
                        weight[j] = weight[j] + adjMatrix[v][j];
                    }
                }
            }
        }
    }
    //visualize the solution with a timer event
    bool solved = weight[this->targetLabelPos] != INT_MAX;
    cout << "Debug: Solved state of the current maze is " << solved << endl << "Weight of the tarLabel is " << weight[this->targetLabelPos] << endl
         << "target label position is " << this->targetLabelPos << endl;
    //set the source pos weight to 0 for later use in solution visualization
    weight[this->sourceLabelPos] = 0;
    this->visualizeSolution(solved, weight, adjMatrix, vertexCount);
    //deallocate adjMatrix since it's dynamically allocated
    for(int i = 0; i < vertexCount; i++){
        delete[] adjMatrix[i];
    }
    delete[] adjMatrix;
}
void MazePane::visualizeSolution(bool isSolved, const int weights[], int* adjMatrix[], const int vertexCount){
    //Retrieve prioritizedVertices regardless of solved state
    vector<vector<int>> prioritizedVertices; //vertices stored in vectors which will be painted with respect to their closeness to source pos
    //prioritize vertices which are not target nor a wall, stemming from source position using bfs
    this->bfsMoveables(prioritizedVertices);
    if(isSolved){
        //vector<vector<int>> prioritizedVertices; //vertices stored in vectors which will be painted with respect to their closeness to source pos
        //backtrack the shortest path from source to target and store them in a vector
        vector<int> solutionPath;
        int currLabel = this->targetLabelPos;
        while(currLabel != this->sourceLabelPos){ //&& currLabel != -1){ //CURRLABEL != -1 IS PROBLEMATIC
            //retrieve the minimum weighted adjacent of currLabel and set it as currLabel after having pushed it into the vector
            int minAdjacent = this->getSmallestAdjacentLabel(weights, vertexCount, currLabel, solutionPath);
            //cout << "Debug: Solution path vertices are: ";
            if(minAdjacent != -1 && weights[minAdjacent] != 0){
                solutionPath.push_back(minAdjacent);
                cout << "Pushing " << minAdjacent << " to solution path" << endl;
            }
            else{
                cout << minAdjacent << " is not a sol cell,";
                break;
            }
            currLabel = minAdjacent;
        }
        cout << endl;

        //start setting the states of rectangles at the respective labels with a timer object

        QVariant variant2 = QVariant::fromValue(solutionPath);

        this->solveTimer->setProperty("isSolved", true);
        this->solveTimer->setProperty("solutionPath", variant2);
        this->solveTimer->setInterval(MazePane::SOLVED_TIMER_MILLIS);
    }
    else{
        //apply bfs just to retrieve the priority vector
       this->solveTimer->setProperty("isSolved", false);
        this->solveTimer->setInterval(MazePane::FAILED_TIMER_MILLIS);
    }
    QVariant variant1 = QVariant::fromValue(prioritizedVertices);
    this->solveTimer->setProperty("priorityVector", variant1);

    this->solveTimer->start();
}
//Slot connected to QTimer data field
void MazePane::solveTimerSlot(){
    static int cycleCounter = 0;
    //retrieve priorityVector property
    vector<vector<int>> prioritizedVertices;
    QVariant v1 = this->solveTimer->property("priorityVector");
    //ensure that the data contains our prioritizedVertices, else print an error message
    if(v1.canConvert<vector<vector<int>>>()){
        prioritizedVertices = v1.value<vector<vector<int>>>();
    }
    else{
        cout << "Error: cannot convert the QVariant to 2d vector" << endl;
    }
    if(cycleCounter >=  prioritizedVertices.size()){
        cycleCounter = 0;
        //stop the timer
        this->solveTimer->stop();
    }
    else{
        vector<int> currLabels = prioritizedVertices.at(cycleCounter);
        //retrieve the isSolved property
        bool solved = this->solveTimer->property("isSolved").toBool();
        MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);;
        if(solved){
            vector<int> solutionPath;
            QVariant v2 = this->solveTimer->property("solutionPath");
            solutionPath = v2.value<vector<int>>();
            for(int currLabel: currLabels){
                int currLabelRow = mzg.getLabelRow(currLabel);
                int currLabelColumn = mzg.getLabelColumn(currLabel);

                QWidget* gridElt = this->getGridElement(currLabelRow, currLabelColumn);
                RectangleWidget* currRect = dynamic_cast<RectangleWidget*>(gridElt);

                vector<int>::iterator solBegin = solutionPath.begin();
                vector<int>::iterator solTerminate = solutionPath.end();
                if(currLabel == this->sourceLabelPos){
                    continue;
                }
                if(std::count(solBegin, solTerminate, currLabel) == 0){//&& currLabel != this->sourceLabelPos){
                    //regular visited label
                    //retrieve the rectangle widget and set the state to visited state
                    this->mazeArr[currLabelRow][currLabelColumn] = MazePane::VISITED_GRID_VALUE;
                    currRect->setState(RectangleWidget::VISITED_STATE);
                }
                else{
                    //retrieve the rectangle widget and set it to solution path state
                    this->mazeArr[currLabelRow][currLabelColumn] = MazePane::SOLUTION_GRID_VALUE;
                    currRect->setState(RectangleWidget::SOLUTION_STATE);
                }
            }
        }
        else{ //failed state
            for(int currLabel: currLabels){
                int currLabelRow = mzg.getLabelRow(currLabel);
                int currLabelColumn = mzg.getLabelColumn(currLabel);

                QWidget* gridElt = this->getGridElement(currLabelRow, currLabelColumn);
                RectangleWidget* currRect = dynamic_cast<RectangleWidget*>(gridElt);

                this->mazeArr[currLabelRow][currLabelColumn] = MazePane::FAILED_GRID_VALUE;
                currRect->setState(RectangleWidget::FAILED_STATE);
            }
        }
        cycleCounter++; //increment by one to avoid infinite signal emissions
    }
    //cycle count depends on the priorityVector property's length
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
                    MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
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
                    MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
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
    this->printMaze();
    int prevSourceLabel = this->sourceLabelPos;
    cout << this->sourceLabelPos << " is our initial source pos" << endl;
    MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
    int prevRow = mzg.getLabelRow(prevSourceLabel);
    int prevColumn = mzg.getLabelColumn(prevSourceLabel);
    this->sourceLabelPos = mzg.getLabel(row, column);
    if(prevSourceLabel != this->sourceLabelPos && this->targetLabelPos != this->sourceLabelPos){
        this->mazeArr[prevRow][prevColumn] = MazePane::EMPTY_GRID_VALUE;
        //retrieve the prev source rectangle widget and set its state to an empty path state
        //first cast the QLayoutItem to Qwidget using reinterpret cast
        //QWidget* rectWidget = reinterpret_cast<QWidget*>(this->itemAtPosition(prevRow, prevColumn));
        RectangleWidget* prevSourceRect = dynamic_cast<RectangleWidget*>(this->itemAtPosition(prevRow, prevColumn)->widget());
        prevSourceRect->setState(MazePane::EMPTY_GRID_VALUE, true);
        //set the new source rectangle's state
        //rectWidget = reinterpret_cast<QWidget*>(this->itemAtPosition(row, column));
        RectangleWidget* newSourceRect = dynamic_cast<RectangleWidget*>(this->itemAtPosition(row, column)->widget());
        this->mazeArr[row][column] = MazePane::START_POS_VALUE;
        newSourceRect->setState(MazePane::START_POS_VALUE, true);
    }
    else if(this->sourceLabelPos == this->targetLabelPos){
        //swap the source with target
        //retrieve the prevSource and target labels
        RectangleWidget* prevSourceRect = dynamic_cast<RectangleWidget*>(this->itemAtPosition(prevRow, prevColumn)->widget()); //set it as target
        RectangleWidget* prevTargetLabel = dynamic_cast<RectangleWidget*>(this->itemAtPosition(row, column)->widget());

        this->mazeArr[prevRow][prevColumn] = MazePane::TARGET_POS_VALUE;
        this->mazeArr[row][column] = MazePane::START_POS_VALUE;

        //set the rectangles' state
        prevSourceRect->setState(MazePane::TARGET_POS_VALUE, true);
        prevTargetLabel->setState(MazePane::START_POS_VALUE, true);
        //update source and target labels
        this->sourceLabelPos = mzg.getLabel(row, column);
        this->targetLabelPos = mzg.getLabel(prevRow, prevColumn);
    }
    else{
        //restore the sourceLabel data field to previous state and prompt an error message
        this->sourceLabelPos = prevSourceLabel;
        cout << "Error: restoring sourceLabelPos back to its previous state" << endl;
    }
}
//Sets the prev target as an empty grid
//Works in an identical fashion to setSourcePos
void MazePane::setTargetPos(int row, int column){
    int prevTargetLabel = this->targetLabelPos;
    MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
    this->targetLabelPos = mzg.getLabel(row, column);
    int prevRow = mzg.getLabelRow(prevTargetLabel);
    int prevColumn = mzg.getLabelColumn(prevTargetLabel);
    if(prevTargetLabel != this->targetLabelPos && this->sourceLabelPos != this->targetLabelPos){
        this->mazeArr[prevRow][prevColumn] = MazePane::EMPTY_GRID_VALUE;
        //retrieve the prev source rectangle widget and set its state to an empty path state
        RectangleWidget* prevTargetRect = dynamic_cast<RectangleWidget*>(this->itemAtPosition(prevRow, prevColumn)->widget());
        prevTargetRect->setState(MazePane::EMPTY_GRID_VALUE, true);
        //set the new source rectangle's state
        RectangleWidget* newTargetRect = dynamic_cast<RectangleWidget*>(this->itemAtPosition(row, column)->widget());
        this->mazeArr[row][column] = MazePane::TARGET_POS_VALUE;
        newTargetRect->setState(MazePane::TARGET_POS_VALUE, true);
    }
    else if(this->sourceLabelPos == this->targetLabelPos){
        //swap the source with target
        RectangleWidget* prevSourceRect = dynamic_cast<RectangleWidget*>(this->itemAtPosition(row, column)->widget());
        RectangleWidget* prevTargetLabel = dynamic_cast<RectangleWidget*>(this->itemAtPosition(prevRow, prevColumn)->widget()); //set it as target

        this->mazeArr[prevRow][prevColumn] = MazePane::START_POS_VALUE;
        this->mazeArr[row][column] = MazePane::TARGET_POS_VALUE;

        //set the rectangles' state
        prevSourceRect->setState(MazePane::TARGET_POS_VALUE, true);
        prevTargetLabel->setState(MazePane::START_POS_VALUE, true);
        //update source and target labels
        this->sourceLabelPos = mzg.getLabel(prevRow, prevColumn);
        this->targetLabelPos = mzg.getLabel(row, column);
    }
    else{
        //restore the previous target label position data field and prompt an error message
        this->targetLabelPos = prevTargetLabel;
        cout << "Error: restoring targetLabelPos back to its previous state" << endl;
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
    for(int i = 0; i < this->rowLength; i++){
        for(int j = 0; j < this->columnLength; j++){
            QWidget* currWidget = nullptr;
            QLayoutItem* currItem = this->itemAtPosition(i, j);
            //ensure that current element is widget despite we know it
            if(currItem != NULL && currItem->widget() != NULL){
                currWidget = currItem->widget();
                if(currWidget == widget){
                    MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
                    result = mzg.getLabel(i, j);
                    return result;
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
//Static function to retrieve source and target labels and return them in a vector respectively
//If no appropriate target OR source is found we return empty vector
vector<int> MazePane::getSourceTargetLabels(int* givenMaze[], int rowLength, int columnLength){
    vector<int> result;
    int sourceLabel = -1;
    int targetLabel = -1;

    bool sourceFound = false;
    bool targetFound = false;

    for(int i = 0; i < rowLength; i++){
        for(int j = 0; j < columnLength; j++){
            int currValue = givenMaze[i][j];
            if(currValue == MazePane::START_POS_VALUE){
                MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
                sourceLabel = mzg.getLabel(i, j);
                sourceFound = true;
            }
            else if(currValue == MazePane::TARGET_POS_VALUE){
                MazeGenerator mzg(0, 0, MazePane::ROW_LENGTH, MazePane::COLUMN_LENGTH, 0, 0, 0, 0);
                targetLabel = mzg.getLabel(i, j);
                targetFound = true;
            }

            if(sourceFound && targetFound){
                break;
            }
        }
        if(sourceFound && targetFound){
            result.push_back(sourceLabel);
            result.push_back(targetLabel);
            break;
        }
    }
    return result;
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
void MazePane::printMaze() const{
    if(this->mazeArr != NULL){
        for(int i = 0; i < this->rowLength; i++){
            for(int j = 0; j < this->columnLength; j++){
                cout << this->mazeArr[i][j] << " ";
            }
            cout << endl;
        }
    }
    else{
        cout << "Error: cannot print null matrix" << endl;
    }
}
bool MazePane::getIsMutable() const{
    return this->isMutable;
}
//returns whether if given labels are mathematically adjacent to one another
bool MazePane::isAdjacent(int label1, int label2) const{
    int p1, p2, p3, p4; //possible adjacent positions of label1
    //start from top end on left clockwise
    p1 = label1 - this->columnLength;
    p2 = label1 + 1;
    p3 = label1 + this->columnLength;
    p4 = label1 - 1;

    bool result = (label2 >= 0 && label2 < this->rowLength * this->columnLength) && (label1 >= 0 && label1 < this->rowLength * this->columnLength)
                  && (p1 == label2 || p2 == label2 || p3 == label2 || p4 == label2);
    return result;
}
//static utility function
//disregards walls
//returns -1 if no candidate is found
int MazePane::getSmallestUnvisitedLabel(const unordered_set<int>& vertexSet, const int weights[], const int vertexCount){
    int minResult = INT_MAX;

    for(int i = 0; i < vertexCount; i++){
        if(vertexSet.count(i) == 0 && weights[i] != -1 && weights[i] < minResult){
            minResult = i;
        }
    }
    if(minResult == INT_MAX){
        minResult = -1;
    }
    return minResult;
}
//static utility function to return the smallest adjacent label
//returns -1 if no candidate has been found
int MazePane::getSmallestAdjacentLabel(const int weights[], const int vertexCount, const int currLabel, const vector<int>& solutionPath) const{
    int p1, p2, p3, p4; //possible adjacent positions of label1
    //start from top end on left clockwise
    p1 = currLabel - this->columnLength;
    p2 = currLabel + 1;
    p3 = currLabel + this->columnLength;
    p4 = currLabel - 1;

    int result = -1;
    int minValue = INT_MAX;
    //print out the exact solution path for debugging purposes
    cout << "Debug: The current exact solution path is; ";
    for(int i = 0; i < solutionPath.size(); i++){
        cout << solutionPath.at(i) << " " << endl;
    }
    cout << endl;
    vector<int>::const_iterator itBegin = solutionPath.begin();
    vector<int>::const_iterator itEnd = solutionPath.end();
    if(p1 >= 0 && p1 < vertexCount && (p1 != this->targetLabelPos) && std::count(itBegin, itEnd, p1) == 0){
        if(weights[p1] != INT_MAX && minValue > weights[p1]){ //!!!swap INT_MAX with -1 if necessary
            minValue = weights[p1];
            result = p1;
        }
    }
    if(p2 >= 0 && p2 < vertexCount && (p2 != this->targetLabelPos) && std::count(itBegin, itEnd, p2) == 0){
        if(weights[p2] != INT_MAX && minValue > weights[p2]){
            minValue = weights[p2];
            result = p2;
        }
    }
    if(p3 >= 0 && p3 < vertexCount && (p3 != this->targetLabelPos) && std::count(itBegin, itEnd, p3) == 0){
        if(weights[p3] != INT_MAX && minValue > weights[p3]){
            minValue = weights[p3];
            result = p3;
        }
    }
    if(p4 >= 0 && p4 < vertexCount && (p4 != this->targetLabelPos) && std::count(itBegin, itEnd, p4) == 0){
        if(weights[p4] != INT_MAX && minValue > weights[p4]){
            minValue = weights[p4];
            result = p4;
        }
    }
    return result;
}
//Applies bfs traversal on our maze and adds the traversed moveable cells into the given 2d vector reference with respect to current priority
void MazePane::bfsMoveables(vector<vector<int>>& prioritizedVertices) const{
    int currPriority = 0; //direct adjacent moveable vertices of the source pos has priority 0, then priority increases by one

    //create a visit array which disregards wall
    int vertexCount = this->rowLength * this->columnLength;
    bool visit[vertexCount];
    MazeGenerator mzg(0, 0, this->rowLength, this->columnLength, 0, 0, 0, 0);
    //set the target and wall visit values to true for disregarding them during bfs traversal
    for(int i = 0; i < this->rowLength; i++){
        for(int j = 0; j < this->columnLength; j++){
            int currLabel = mzg.getLabel(i, j);
            if(this->mazeArr[i][j] == MazePane::WALL_GRID_VALUE || this->mazeArr[i][j] == MazePane::TARGET_POS_VALUE){
                visit[currLabel] = true;
            }
            else{
                visit[currLabel] = false;
            }
        }
    }
    //apply bfs traversal to the queue and push visited labels to prioritizedVertices simultaneously with respect to their priority
    queue<int> q;
    //mark source vertex as visited and push it onto the queue
    int source = this->sourceLabelPos;
    q.push(source);
    visit[source];

    while(!q.empty()){
        int currLabel = q.front();
        q.pop();

        vector<int> currAdj = mzg.getAdjLabels(currLabel);
        vector<int> unvisitedAdj;
        for(int label: currAdj){
            int labelRow = mzg.getLabelRow(label);
            int labelColumn = mzg.getLabelColumn(label);

            int labelState = this->mazeArr[labelRow][labelColumn];
            //Below condition could be problematic due to value mismatch
            if(!visit[label] && labelState != MazePane::WALL_GRID_VALUE && labelState != MazePane::TARGET_POS_VALUE){
                unvisitedAdj.push_back(label);
            }
        }
        for(int unvisitedLabel: unvisitedAdj){
            q.push(unvisitedLabel);
            visit[unvisitedLabel] = true;
        }
        //push the unvisitedAdj vector to prioritizedVertices' priority index
        vector<vector<int>>::iterator posIterator = prioritizedVertices.begin() + currPriority++;
        prioritizedVertices.insert(posIterator, unvisitedAdj);
    }
}
