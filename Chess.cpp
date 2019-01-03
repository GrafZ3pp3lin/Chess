#include <iostream>
#include "Game.hpp"

// Init Game Board Array
void initGame(Game &game) {
    for (int i = 0; i < 21; i++) {
        game.board[i] = -1;
    }
    game.board[21] = 21;
    game.board[22] = 22;
    game.board[23] = 23;
    game.board[24] = 24;
    game.board[25] = 20;
    game.board[26] = 23;
    game.board[27] = 22;
    game.board[28] = 21;
    game.board[29] = -1;
    game.board[30] = -1;
    for (int i = 0; i < 8; i++) {
        game.board[31 + i] = 2;
    }
    game.board[39] = -1;
    for (int i = 0; i < 4; i++) {
        game.board[40 + i * 10] = -1;
        for (int j = 0; j < 8; j++) {
            game.board[41 + i * 10 + j] = 0;
        }
        game.board[49 + i * 10] = -1;
    }
    game.board[80] = -1;
    for (int i = 0; i < 8; i++) {
        game.board[81 + i] = 1;
    }
    game.board[89] = -1;
    game.board[90] = -1;
    game.board[91] = 11;
    game.board[92] = 12;
    game.board[93] = 13;
    game.board[94] = 14;
    game.board[95] = 10;
    game.board[96] = 13;
    game.board[97] = 12;
    game.board[98] = 11;
    for (int i = 0; i < 21; i++) {
        game.board[99 + i] = -1;
    }
}

// Print whole Array
void printArray(Game &game) {
    for (int i = 0; i < sizeof(game.board); i++) {
        std::cout << (int)game.board[i] << " ";
        if ((i+1) % 10 == 0) {
            std::cout << std::endl;
        }
    }
}

char transfer(int id) {
    switch(id) {
        case 0: return ' ';
        case 1: return 'P';
        case 2: return 'p';
        case 11: return 'R';
        case 21: return 'r';
        case 12: return 'N';
        case 22: return 'n';
        case 13: return 'B';
        case 23: return 'b';
        case 14: return 'Q';
        case 24: return 'q';
        case 10: return 'K';
        case 20: return 'k';
    }
}

// Print Board Segment to Console 
void printBoard(Game &game) {
    std::cout << "  ";
    for (int i = 0; i < 8; i++) {
        std::cout << "  " << char(97 + i) << " ";
    }
    std::cout << std::endl << "  " << char(0xDA);
    for (int i = 0; i < 7; i++) {
        std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xC2);
    }
    std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xBF) << std::endl;

    for (int i = 0; i < 8; i++) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            std::cout << char(0xB3) << " " << transfer(game.board[i * 10 + 21 + j]) << " ";
        }
        std::cout << char(0xB3) << std::endl;
        if (i < 7) {
            std::cout << "  " << char(0xC3);
            for (int i = 0; i < 7; i++) {
                std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xC5);
            }
            std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xB4) << std::endl;
        }
    }
    std::cout << "  " << char(0xC0);
    for (int i = 0; i < 7; i++) {
        std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xC1);
    }
    std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xD9) << std::endl;
}

int main() {
    //Pointer auf Game vielleicht.
    Game game;
    initGame(game);
    printBoard(game);
    return 0;
}