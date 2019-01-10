#ifndef DHBW_CHESS_KI
#define DHBW_CHESS_KI

#include "MoveGenerator.hpp"

union Value {
    int highest;
    int lowest;
};

class RatedMove
{
    public:
    RatedMove(Move move, int value) : move(move), value(value) { };
    Move move;
    int value = 0;
};

Move getNextMove(ChessBoard *board);

#endif