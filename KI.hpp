#ifndef DHBW_CHESS_KI
#define DHBW_CHESS_KI

#include "MoveGenerator.hpp"

union Value {
    int highest;
    int lowest;
};

Move getNextMove(ChessBoard *board);

#endif