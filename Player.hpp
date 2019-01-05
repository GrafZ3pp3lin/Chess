#ifndef DHBW_CHESS_PLAYER
#define DHBW_CHESS_PLAYER

#include "MoveGenerator.hpp"

#include <iostream>

int8_t square_to_index(char square[2]);
const char* index_to_square(int8_t index);
Move get_move_of_player(ChessBoard *board);

#endif
