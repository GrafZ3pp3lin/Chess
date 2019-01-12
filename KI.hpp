#ifndef DHBW_CHESS_KI
#define DHBW_CHESS_KI

#include "MoveGenerator.hpp"

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

Move getNextAIMove(ChessBoard *board);
void setDifficulty(Difficulty dif);

#endif