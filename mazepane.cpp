#include "mazepane.h"
#include "mazegenerator.h"

MazePane::MazePane(bool autoGenerate){

}
MazePane::~MazePane(){

}
void MazePane::solve(){

}
//static function to check whether a given maze, which is a valid maze, can be solved
bool MazePane::isTraversable(int** givenMaze, int mazeLength, int columnLength){
    MazeGenerator mg(0,0,0,0,0,0,0,0);
    //retrieve the source and target positions with the help of mazeGenerator methods
}
