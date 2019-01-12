#include "Evaluation.hpp"
#include "MoveGenerator.hpp"
#include "KI.hpp"
#include <iostream>
#include <future>
#include <limits>
#include <mutex>
#include <ctime>
#include <algorithm>

int MAX_DEPTH = 4;
int gameValueOffset = 75;
bool useDirectestCheckMate = true;
std::vector<Move> opening;

void setDifficulty(Difficulty dif) {
    switch (dif) 
    {
        case Difficulty::EASY:
            MAX_DEPTH = 3;
            gameValueOffset = 150;
            useDirectestCheckMate = false;
            break;
        case Difficulty::HARD:
            MAX_DEPTH = 5;
            gameValueOffset = 10;
            useDirectestCheckMate = true;
            break;
        default:
            MAX_DEPTH = 4;
            gameValueOffset = 75;
            useDirectestCheckMate = true;
    }
}

//------------------------------------DEBUG------------------------------------------------
const char* convert(int8_t index)
{
    char* square = new char[3];
    square[0] = (char)((index % 10) + 64);
    square[1] = (char)(10 - (index / 10) + 48);
    square[2] = '\0';
    return square;
}
//------------------------------------DEBUG------------------------------------------------

int calculate(ChessBoard &board, bool oponent, int depth, int alpha, int beta);
RatedMove startCalculateMove(Move m, ChessBoard *board);

Move getNextAIMove(ChessBoard *board) {
    std::vector<Move> moveset = board->get_moveset_all(board->activePlayer);
    
    //Eröffnungen
    Move m = Move{(uint8_t) 0,(uint8_t)0};
    if(board->moveCounter < 3 && std::find(moveset.begin(), moveset.end(), (m = opening_move(board))) != moveset.end() && board->is_legal(m, board->activePlayer)){
        std::cout << "from opening Database" << std::endl;
        return m;
    }

    std::cout << "Databasemove from " << convert(m.from) << " to " << convert(m.from) << std::endl;

    std::vector<std::future<RatedMove>> futures;
    std::vector<RatedMove> moves;
    int index;
    for (int i = 0; i < moveset.size(); i++) {
        if (board->is_legal(moveset[i], board->activePlayer)) {
            //start for each possible Move one Thread
            futures.push_back(std::async(std::launch::async, &startCalculateMove, moveset[i], board));
        }
    }
    Value v;
    v.highest = 1;
    std::vector<RatedMove> bestMoves;
    for (int i = 0; i < futures.size(); i++) {
        auto temp = futures[i].get();
        //DEBUG 
        std::cout << "Move: from " << convert(temp.move.from) << " to " << convert(temp.move.to) << " has value of " << temp.value << std::endl;
        if (board->activePlayer == Color::WHITE) {
            if (temp.value != 1 && (temp.value > v.highest || v.highest == 1)) {
                v.highest = temp.value;
            }
        }
        else {
            if (temp.value != 1 && (temp.value < v.lowest || v.lowest == 1)) {
                v.lowest = temp.value;
            }
        }
        moves.push_back(temp);
    }
    /*for (RatedMove &r : moves) {
        //std::cout << "Move: from " << convert(temp.move.from) << " to " << convert(temp.move.to) << " has value of " << temp.value << std::endl;
        if (board->activePlayer == Color::WHITE) {
            if (r.value != 1 && (r.value > v.highest || v.highest == 1)) {
                v.highest = r.value;
            }
        }
        else {
            if (r.value != 1 && (r.value < v.lowest || v.lowest == 1)) {
                v.lowest = r.value;
            }
        }
    }*/
    for (RatedMove &r : moves) {
        if ((board->activePlayer == Color::BLACK && r.value <= (v.lowest + gameValueOffset))
            || (board->activePlayer == Color::WHITE && r.value >= (v.highest - gameValueOffset))) {
            bestMoves.push_back(r);
        }
    }
    if (bestMoves.size() == 0) {
        throw std::runtime_error("NoPossibleMoveError");
    }
    return bestMoves[rand() % bestMoves.size()].move;
}

RatedMove startCalculateMove(Move m, ChessBoard *board) {
    ChessBoard boardCopy{*board};
    boardCopy.move_piece(m, true);
    boardCopy.activePlayer = !boardCopy.activePlayer;
    return RatedMove {m, calculate(boardCopy, true, 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max())};
}

//calculate Moves recursive 
int calculate(ChessBoard &board, bool oponent, int depth, int alpha, int beta) {
    if (depth == MAX_DEPTH) {
        return evaluate_board(&board);
    }
    std::vector<Move> moveset = board.get_moveset_all(board.activePlayer);
    std::vector<ChessBoard> boards;
    for (Move m : moveset) {
        if (board.is_legal(m, board.activePlayer)) {
            ChessBoard boardCopy{board};
            boardCopy.move_piece(m, true);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            boards.push_back(boardCopy);
        }
    }
    //Alpha Beta Pruning
    Value v;
    v.highest = board.activePlayer == Color::BLACK ? beta : alpha;
    for (ChessBoard &b : boards) {
        int temp = calculate(b, !oponent, depth+1, board.activePlayer == Color::BLACK ? alpha : v.highest, board.activePlayer == Color::WHITE ? beta : v.lowest);
        if (board.activePlayer == Color::BLACK) {
            if (useDirectestCheckMate && temp < -1000000) {
                temp += 10000;
            }
            if (temp < v.lowest || v.lowest == 1) {
                v.lowest = temp;
                if (v.lowest <= alpha) {
                    break;
                }
            }
        }
        else {
            if (useDirectestCheckMate && temp > 1000000) {
                temp -= 10000;
            }
            if (temp > v.highest || v.highest == 1) {
                v.highest = temp;
                if (v.highest >= beta) {
                    break;
                }
            }
        }
    }
    return v.highest;
}

void add_opening_move(Move m, int moveCounter){
    if(moveCounter < 5){
        opening.push_back(m);
    }
}

Move opening_move(ChessBoard *board){
    srand(time(0));
    std::vector<Move> possible;

    switch(board->moveCounter){
        //white
        case 0:
        // == Operator ist überschrieben -> Move.to == square_to_index("e4")
            possible.push_back(Move{"e2","e4"});
            possible.push_back(Move{"d2","d4"});
            possible.push_back(Move{"g1","f3"});
            break;
        //black
        case 1:
            if(opening[0] == "e4"){
                possible.push_back(Move{"e7", "e6"});
                possible.push_back(Move{"e7", "e5"});
                possible.push_back(Move{"c7", "c5"});
                std::cout << "reached" << std::endl;
            }
            else if(opening[0] == "d4"){
                possible.push_back(Move{"g8", "f6"});
                possible.push_back(Move{"d7", "d5"});
            }
            else if(opening[0] == "f3"){
                possible.push_back(Move{"g8", "f6"});
            }
            break;
        //white
        case 2:
            if(opening[0] == "e4" && (opening[1] == "c5" || opening[1] == "e5")){
                possible.push_back(Move{"g1", "f3"});
            }
            else if(opening[0] == "e4" && opening[1] == "e6"){
                possible.push_back(Move{"d2", "d4"});
                possible.push_back(Move{"d2", "d3"});
            }

            else if(opening[0] == "d4" && (opening[1] == "f6" || opening[1] == "d5")){
                possible.push_back(Move{"c2", "c4"});
                possible.push_back(Move{"g1","f3"});
            }

            else if(opening[0] == "f3" && opening[1] == "f6"){
                possible.push_back(Move{"c2", "c4"});
                possible.push_back(Move{"g2","g3"});
                possible.push_back(Move{"d2", "d4"});
            }
            break;
         //black
        case 3:
            if(opening[0] == "e4" && (opening[1] == "c5" || opening[1] == "e5") && opening[2] == "f3"){
                possible.push_back(Move{"d3", "d6"});
                possible.push_back(Move{"b8", "c6"});
            }
            else if(opening[0] == "e4" && opening[1] == "e6" && (opening[2] == "d4" || opening[2] == "d3")){
                possible.push_back(Move{"d7", "d5"});
            }

            else if(opening[0] == "d4" && (opening[1] == "f6") && (opening[2] == "c4")){
                possible.push_back(Move{"e7", "e6"});
                possible.push_back(Move{"g7", "g6"});
            }
            else if(opening[0] == "d4" && (opening[1] == "d5") && (opening[2] == "c4")){
                possible.push_back(Move{"c7", "c6"});
                possible.push_back(Move{"e7", "e6"});
            }
            else if(opening[0] == "d4" && (opening[1] == "f6") && (opening[2] == "f3")){
                possible.push_back(Move{"d7", "d5"});
                possible.push_back(Move{"g7", "g6"});
                possible.push_back(Move{"e7", "e6"});
            }
            else if(opening[0] == "d4" && (opening[1] == "d5") && (opening[2] == "f3")){
                possible.push_back(Move{"g8", "f6"});
            }

            else if(opening[0] == "f3" && (opening[1] == "f6") && (opening[2] == "c4")){
                possible.push_back(Move{"g7", "g6"});
                possible.push_back(Move{"e7", "e6"});
                possible.push_back(Move{"c7", "c5"});
            }
            else if(opening[0] == "f3" && (opening[1] == "f6") && (opening[2] == "d4")){
                possible.push_back(Move{"g7", "g6"});
                possible.push_back(Move{"e7", "e6"});
                possible.push_back(Move{"d7", "d5"});
            }
            else if(opening[0] == "f3" && (opening[1] == "f6") && (opening[2] == "g3")){
                
                possible.push_back(Move{"g7", "g6"});
                possible.push_back(Move{"d7", "d5"});
            }
            break;
    }
    if(possible.size() > 0){
        return possible[rand() % possible.size()];
    }
    return Move{(uint8_t) 0, (uint8_t) 0};
}