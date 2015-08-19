//
//  main.cpp
//  8Puzzle
//
//  Created by colan connon on 2/11/15.
//  Copyright (c) 2015 colan connon. All rights reserved.
// for linux:
// g++ -std=c++11 8puzzle.cpp
// for mac g++ 8puzzle.cpp

//wrote and tested on OSX yosemite

#include <iostream>
#include <array>
#include <random>
#include <string>
#include <chrono>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;


void printBoard(){
    
}
array<int,9> generateRandomNums(){
    //GENERATE 9 RANDOM NUMBERS SO THE BOARD CAN BE RANDOM AT THE START
    array<int,9> numbers = {0,1,2,3,4,5,6,7,8};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();;
    std::default_random_engine gen(seed);
    shuffle(numbers.begin(), numbers.end(), gen);
    return numbers;
}

class Board{
    // BOARD NODE CLASS
public:
    //CONSTRUCTOR
    Board(Board *pBoard);
    //DEPTH OF BOARD
    int depth;
    // ARRAY OF BOARD ARRANGEMENT
    array<int, 9> boardArray;
    //PARENT PTR
    Board  *parent;
    //BOARD CONSTRUCTOR
    Board();
    //FUNC TO GENERATOR RANDOM BOARD
    void generateRandomBoard();
    //COPY CONSTRUCTOR
    Board(const Board& board);
    
    //UTIL FUNCS
    bool canMoveRight(int i);
    bool canMoveLeft(int i);
    bool canMoveUp(int i);
    bool canMoveDown(int i);
    void generateValidBoardMoves();
    void performHeuristic();
    vector<Board*> children;
    
    //NUM TILES
    // HVAL = F(N) WHERE F(N) = G(N) + H(N)
    //H(N) = NUM MISPLACES TILES
    int hVal;
    int numMisplacedTiles;
};

Board::Board() {
    
}
Board::Board(const Board& board){
    //COPY CONSTRUCTOR, NEED THIS SO WE DON'T CREATE A SHALLOW COPY AND CREATE A DEEP COPY
    this->boardArray = board.boardArray;
    this->parent = board.parent;
    this->depth = board.depth;
    if(board.children.size() > 0){
        this->children = board.children;
    }
    else{
        
    }
    this->hVal = board.hVal;
    this->numMisplacedTiles = board.numMisplacedTiles;
}
void Board::performHeuristic(){
    int count = 0;
    for (int i = 0; i < this->boardArray.size(); i++) {
        if(i == 8){
            //count the empty tile in slot as being +1
            if(boardArray[8] == 0){
                
            }
        }
        else{
            if(boardArray[i] == i+1){
                count++;
            }
        }
    }
    this->numMisplacedTiles = (int)boardArray.size() - count;
    this->hVal = depth + numMisplacedTiles;
}

void Board::generateRandomBoard(){
    //GENERATE A NEW BOARD OF RANDOM INTS
    this->boardArray = generateRandomNums();
    this->depth = 0;
};
bool Board::canMoveRight(int i){
    // check if the board can move right at this index
    if(i == 2 || i == 5 || i ==8){
        return false;
    }
    else{
        return true;
    }
}
bool Board::canMoveLeft(int i){
    //check if the board can move left at this index
    if(i == 0 || i == 3 || i ==6){
        return false;
    }
    else{
        return true;
    }
}
bool Board::canMoveUp(int i){
    //check if the board can move up at this index
    if( i <= 2){
        return false;
    }
    else{
        return true;
    }
}
bool Board::canMoveDown(int i){
    if(i >= 6){
        return false;
    }
    else{
        return true;
    }
}
void Board::generateValidBoardMoves() {
    // get the valid board moves and perform them
    int zeroIndex = 0;
    for(int i = 0; i < 9;  i++){
        if(boardArray[i] == 0){
            zeroIndex = i;
        }
    }
    if(canMoveRight(zeroIndex)){
        //IF THE BOARD CAN MOVE RIGHT THEN CREATE A NEW BOARD NODE AND THEN MAKE A NEW BOARD ARRAY FOR IT
        Board *board = new Board();
        board->boardArray = this->boardArray;
        int temp1 = board->boardArray[zeroIndex];
        int temp2 = board->boardArray[zeroIndex +1];
        
        board->boardArray[zeroIndex+1] = temp1;
        board->boardArray[zeroIndex] = temp2;
        
        board->parent = new Board(*this);
        board->depth = this->depth + 1;
        this->children.push_back(board);
        
    }
    if(canMoveLeft(zeroIndex)){
        
        Board *board = new Board();
        board->boardArray = this->boardArray;
        int temp1 = board->boardArray[zeroIndex];
        int temp2 = board->boardArray[zeroIndex -1];
        
        board->boardArray[zeroIndex-1] = temp1;
        board->boardArray[zeroIndex] = temp2;
        board->parent = new Board(*this);
        
        board->depth = this->depth + 1;
        this->children.push_back(board);
    }
    if (canMoveUp(zeroIndex)){
        Board *board = new Board();
        board->boardArray = this->boardArray;
        int temp1 = board->boardArray[zeroIndex];
        int temp2 = board->boardArray[zeroIndex -3];
        
        board->boardArray[zeroIndex-3] = temp1;
        board->boardArray[zeroIndex] = temp2;
        board->depth = this->depth + 1;
        board->parent = new Board(*this);
        this->children.push_back(board);
    }
    if (canMoveDown(zeroIndex)){
        Board *board = new Board();
        board->boardArray = this->boardArray;
        int temp1 = board->boardArray[zeroIndex];
        int temp2 = board->boardArray[zeroIndex +3];
        
        board->boardArray[zeroIndex+3] = temp1;
        board->boardArray[zeroIndex] = temp2;
        board->depth = this->depth + 1;
        //HAVE TO DO A DEEP COPY
        board->parent = new Board(*this);
        this->children.push_back(board);
    }
}


bool isInList(vector<Board> list, Board *board){
    //UTIL FUNCTION FOR CHECKING IF A BOARD IS IN A LIST
    if (list.size() ==0){
        return false;
    }
    for(int i = 0; i < list.size(); i++){
        if(list[i].boardArray == board->boardArray){
            
            return true;
        }
    }
    return false;
}
bool checkForCorrectState(Board *board){
    // if the winning combo then return true, else return false
    array<int,9> winningCombo = {1,2,3,4,5,6,7,8,0};
    
    if (board->boardArray == winningCombo){
        return true;
    }
    else{
        return false;
    }
}

Board* breadthFirsSearch(Board *board){
    //if board is already a win just return
    if(checkForCorrectState(board)){
        return board;
    }
    /*
     BREADTH FIRST SEARCH ALGORITHM, START WITH A BOARD NODE, AND ADD IT TO THE FRONTIER LIST
     
     */
    vector<Board> frontierList;
    vector<Board> exploredList;
    frontierList.insert(frontierList.end(), *board);
    
    while(!frontierList.empty()){
        //TERMINATE SO THE ALGORITHM DOESN'T RUN FOREVER
        if(exploredList.size() > 100000){
            return NULL;
        }
        //POP A BOARD OFF THE ARRAY, AND ERASE IT
        Board board1 = frontierList.front();
        frontierList.erase(frontierList.begin());
        if(checkForCorrectState(&board1)){
            Board *board2 = new Board(board1);
            return board2;
        }
        //ADD IT TO THE EXPLORED LIST
        exploredList.push_back(board1);
        //GENERATE THE CHILDREN FOR THE BOARD
        board1.generateValidBoardMoves();
        for(int i = 0; i < board1.children.size(); i++){
            // FOR EVERY CHILD IN THE BOARDS CHILDREN, CHECK AND SEE IF THEY HAVE BEEN EXPLORED OR ARE IN THE FRONTIER LIST
            // IF THEY AREN'T EXPLORED OR IN THE FRONTIER LIST THEN TERMINATE
            Board *childBoard = board1.children[i];
            
            if(isInList(frontierList, childBoard) || isInList(exploredList, childBoard)){
                // do nothing
            }
            else{
                //check if that child wins
                if(checkForCorrectState(childBoard)){
                    return  childBoard;
                }
                frontierList.push_back(*childBoard);
            }
        }
        
        
    }
    return board;
}
struct SortingVectorFunctor
{   // NEED THIS FOR SORTING THE ARRAY
    bool operator()(const Board& b1, const Board& b2) const
    {
        return b1.hVal < b2.hVal;
    }
};

Board* aStarSearch(Board *board){
    /*
     
        ASTAR SEARCH 
        GIVEN A BOARD NODE ADD IT TO THE FRONTIER LIST
     
     
     */
    if(checkForCorrectState(board)){
        return board;
    }
    board->generateValidBoardMoves();
    board->performHeuristic();
    vector<Board> frontierList;
    vector<Board> exploredList;
    
    frontierList.insert(frontierList.end(), *board);
    
    while(!frontierList.empty()){
        //WHLE FRONTIER LIST NOT EMPTY AND SIZE NOT GREATER THAN 100K
        if(exploredList.size() > 100000){
            return NULL;
        }
        
        //POP THE BOARD OFF THE LIST AND ADD IT TO THE EXPLORED
        Board board1 = frontierList.front();
        frontierList.erase(frontierList.begin());
        if(checkForCorrectState(&board1)){
            Board *board2 = new Board(board1);
            return board2;
        }
        exploredList.push_back(board1);
        //GENERATE THE CHILDREN
        board1.generateValidBoardMoves();
        for(int i = 0; i < board1.children.size(); i++){
            //FOR EACH CHILD ADD TO FRONTIER LIST
            Board *childBoard = board1.children[i];
            
            childBoard->performHeuristic();
            
            if(isInList(frontierList, childBoard) || isInList(exploredList, childBoard)){
                
            }
            else{
                frontierList.push_back(*childBoard);
            }
        }
        if(frontierList.size() >= 2){
            //SORT THE LIST BY THE HVAL VARIABLE SO IT SELECTS THE BEST ONE
            sort(frontierList.begin(), frontierList.end(), SortingVectorFunctor());
        }
        
        
        
    }
    return board;
}

void printWinningInformation(Board *board1){
    
    //util function for priting the board information in a nice way
    
    cout << "WINNING COMBO BACKWARDS!!!!!!!!!" << endl;
    cout << endl;
    cout << "DEPTH: " << board1->depth << endl;
    for(int i = 1; i < board1->boardArray.size() + 1; i++){
        cout << board1->boardArray[i - 1];
        if(i % 3 == 0){
            cout << endl;
        }
    }
    cout << endl;
    while(board1->depth != 0){
        board1 = board1->parent;
        cout << "DEPTH: " << board1->depth << endl;
        for(int i = 1; i < board1->boardArray.size() + 1; i++){
            
            cout << board1->boardArray[i - 1];
            if(i % 3 == 0){
                cout << endl;
            }
        }
        cout << endl;
        
    }
    cout << endl;
}
int main() {
    
    
    //    TEST ARRAGNMENT THIS ONE IS EASY TO WIN
    //     REMOVE THIS FOR RANDOM BOARD
    //    board->boardArray[0] = 1;
    //    board->boardArray[1] = 2;
    //    board->boardArray[2] = 3;
    //    board->boardArray[3] = 4;
    //    board->boardArray[4] = 5;
    //    board->boardArray[5] = 6;
    //    board->boardArray[6] = 0;
    //    board->boardArray[7] = 7;
    //    board->boardArray[8] = 8;
    //
    for(int i = 0; i < 3; i++){
        Board *board =new Board();
        board->generateRandomBoard();
        //        board->boardArray[0] = 1;
        //        board->boardArray[1] = 2;
        //        board->boardArray[2] = 3;
        //        board->boardArray[3] = 4;
        //        board->boardArray[4] = 5;
        //        board->boardArray[5] = 6;
        //        board->boardArray[6] = 0;
        //        board->boardArray[7] = 7;
        //        board->boardArray[8] = 8;
        cout << "STARTING BOARD FOR BFS SEARCH RUN NUMBER: " << i << endl;
        for(int j = 1; j < board->boardArray.size()+ 1; j++){
            
            cout << board->boardArray[j - 1];
            if(j % 3 == 0){
                cout << endl;
            }
        }
        cout << endl;
        Board *board1 = breadthFirsSearch(board);
        if(board1 != NULL){
            printWinningInformation(board1);
        }
        else{
            cout << "NO SOLUTION COULD BE FOUND FOR THIS BOARD" << endl<< endl;
        }
        
    }
    
    for(int i = 0; i < 3; i++){
        Board *board =new Board();
        board->generateRandomBoard();
        //        board->boardArray[0] = 1;
        //        board->boardArray[1] = 2;
        //        board->boardArray[2] = 3;
        //        board->boardArray[3] = 4;
        //        board->boardArray[4] = 5;
        //        board->boardArray[5] = 6;
        //        board->boardArray[6] = 0;
        //        board->boardArray[7] = 7;
        //        board->boardArray[8] = 8;
        cout << "STARTING BOARD FOR ASTAR SEARCH RUN NUMBER: " << i << endl;
        for(int j = 1; j < board->boardArray.size()+ 1; j++){
            
            cout << board->boardArray[j - 1];
            if(j % 3 == 0){
                cout << endl;
            }
        }
        cout << endl;
        Board *board1 = aStarSearch(board);
        if(board1 != NULL){
            printWinningInformation(board1);
        }
        else{
            cout << "NO SOLUTION COULD BE FOUND FOR THIS BOARD" << endl<< endl;
        }
    }
    
    
    
    
    
    return 0;
}



