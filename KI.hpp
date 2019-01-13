#ifndef DHBW_CHESS_KI
#define DHBW_CHESS_KI

#include "MoveGenerator.hpp"
#include <memory>

template <typename T>
bool contains(std::vector<T> vec, T val){
    for(T a: vec){
        if(a == val){
            return true;
        }
    }
    return false;
}

union Value {
    int highest;
    int lowest;
};

enum class Difficulty
{
    EASY,
    NORMAL,
    HARD
};

class RatedMove
{
    public:
    RatedMove(Move move, int value) : move(move), value(value) { };
    Move move;
    int value = 0;
};

Move getNextAIMove(std::shared_ptr<ChessBoard>& board);
void setDifficulty(Difficulty dif);
Move opening_move(std::shared_ptr<ChessBoard>& board);
void add_opening_move(Move,int);

#endif