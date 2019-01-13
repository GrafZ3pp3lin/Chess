#ifndef DHBW_CHESS_MAIN
#define DHBW_CHESS_MAIN

#include "MoveGenerator.hpp"
#include "Player.hpp"
#include <string>
#include <fstream>



void save(std::string);
const char* load(std::string);
void help();
void quit();

#endif