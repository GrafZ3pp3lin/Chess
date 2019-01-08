#ifndef DHBW_CHESS_MAIN
#define DHBW_CHESS_MAIN

#include "MoveGenerator.hpp"
#include "Player.hpp"
#include <string>
#include <fstream>


ChessBoard* chessBoard;

void save(ChessBoard &game, std::string);
ChessBoard* load(std::string);

#endif